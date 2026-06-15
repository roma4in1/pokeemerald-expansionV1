#!/usr/bin/env python3
"""Compose Schism Isle layout blockdata (map.bin/border.bin).

Mirrors tools/pelagios/build_emberveil.py.

Schism is a SPLIT island: an ice/north half (gTileset_PelagiosIce, snow), a
poison/south half (gTileset_PelagiosPoison, acid rain), and a devastated grey
middle (the Scar + deeper ScarRuins) on plain gTileset_General/Cave.

Tilesets:
  North outdoor: gTileset_General (primary) + gTileset_PelagiosIce  (secondary)
  South outdoor: gTileset_General (primary) + gTileset_PelagiosPoison(secondary)
  Scar/ScarRuins: gTileset_General (primary) + gTileset_Cave        (secondary, grey)

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
The custom Pelagios secondary tilesets are flat-fill terrain. Paired with the
512-metatile General primary, a secondary's LOCAL metatile id L is GLOBAL id
512+L (NUM_METATILES_IN_PRIMARY). We reference secondary id 1 (a filled terrain
metatile; id 0 is blank by convention) for floors and walls, and bake
collision/elevation into the layout word (the flat-fill metatiles carry no
collision themselves), exactly as Emberveil baked collision into General/Lavaridge
words.

Composed outdoor layouts only. Interiors (Inns / PokeCenters / Barracks / Lab /
SealChambers) reuse vanilla LAYOUT_* wholesale in build_schism_mapjson.py
(zero new binary): the IceCity Barracks puzzle reuses SHOAL_CAVE_LOW_TIDE_ICE_ROOM
(real MB_ICE sliding tiles), the PoisonCity Laboratory reuses AQUA_HIDEOUT_1F
(industrial two-level facility), both SealChambers reuse SEALED_CHAMBER_INNER_ROOM.
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')

SEC_BASE = 512  # NUM_METATILES_IN_PRIMARY

def mt(local_id, collision=0, elevation=3):
    """Build a layout word for a SECONDARY-tileset metatile (local id -> +512)."""
    gid = SEC_BASE + local_id
    return (gid & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

# Secondary flat-fill terrain metatile (id 1) is the visible ground for each half.
FLOOR_ID = 1

# --- ICE half tiles ---
I_GROUND = mt(FLOOR_ID, 0, 3)   # snow ground (walkable)
I_WALL   = mt(FLOOR_ID, 1, 0)   # ice cliff/wall (solid)
I_WATER  = mt(FLOOR_ID, 1, 0)   # frozen sea (impassable here; deep water look)
BORDER_I = [I_WALL] * 4

# --- POISON half tiles ---
P_GROUND = mt(FLOOR_ID, 0, 3)   # swamp ground / walkway (walkable)
P_WALL   = mt(FLOOR_ID, 1, 0)   # corroded wall (solid)
P_WATER  = mt(FLOOR_ID, 1, 0)   # toxic water (impassable)
BORDER_P = [P_WALL] * 4

# --- SCAR / ruins: plain grey General+Cave (vanilla Cave metatiles, collision baked) ---
# Cave floor 0x3211 (col0 elev3) and cave wall 0x0251 (col1 elev0), proven on the
# Haven ruins copies.
S_GROUND = 0x3211
S_WALL   = 0x0251
BORDER_S = [S_WALL] * 4

def new_grid(w, h, fill):
    return [[fill] * w for _ in range(h)]

def write_layout(name, grid, border):
    d = os.path.join(ROOT, f'data/layouts/{name}')
    os.makedirs(d, exist_ok=True)
    h, w = len(grid), len(grid[0])
    with open(os.path.join(d, 'map.bin'), 'wb') as f:
        for row in grid:
            f.write(struct.pack(f'<{w}H', *row))
    with open(os.path.join(d, 'border.bin'), 'wb') as f:
        f.write(struct.pack('<4H', *border))
    return w, h

results = {}
layout_entries = []

def reg_layout(layout_id, name, w, h, prim, sec):
    layout_entries.append({
        'id': layout_id,
        'name': name + '_Layout',
        'width': w, 'height': h,
        'primary_tileset': prim, 'secondary_tileset': sec,
        'border_filepath': f'data/layouts/{name}/border.bin',
        'blockdata_filepath': f'data/layouts/{name}/map.bin',
        'layout_version': 'emerald',
    })

ICE    = ('gTileset_General', 'gTileset_PelagiosIce')
POISON = ('gTileset_General', 'gTileset_PelagiosPoison')
SCAR   = ('gTileset_General', 'gTileset_Cave')

def ring(g, w, h, north_gap=(), south_gap=(), west_gap=(), east_gap=(), wall=None):
    for x in range(w):
        if x not in north_gap: g[0][x] = wall
        if x not in south_gap: g[h-1][x] = wall
    for y in range(h):
        if y not in west_gap: g[y][0] = wall
        if y not in east_gap: g[y][w-1] = wall

def patch(g, x0, y0, x1, y1, tile):
    for y in range(y0, y1+1):
        for x in range(x0, x1+1):
            g[y][x] = tile

# =====================================================================
#  NORTH / ICE HALF (General + PelagiosIce, WEATHER_SNOW)
# =====================================================================

# ---------------- FrostmarkPort (20x18) ----------------
# Sea to the south (Galleon docks). North exit -> FrozenTundra.
W, H = 20, 18
g = new_grid(W, H, I_GROUND)
ring(g, W, H, north_gap=(9, 10), wall=I_WALL)
patch(g, 1, 13, W-2, H-1, I_WATER)          # southern frozen sea
# central stone dock down into the water for the Tennyson (Galleon)
for y in range(12, 16):
    g[y][9] = I_GROUND
    g[y][10] = I_GROUND
# checkpoint pillars flanking north exit
g[1][8] = I_WALL
g[1][11] = I_WALL
results['Schism_FrostmarkPort'] = write_layout('Schism_FrostmarkPort', g, BORDER_I)
reg_layout('LAYOUT_SCHISM_FROSTMARK_PORT', 'Schism_FrostmarkPort', W, H, *ICE)

# ---------------- FrozenTundra (20x32) ----------------
# south -> FrostmarkPort, north -> IceCity, east -> TheScar (faction-soldier gated).
W, H = 20, 32
g = new_grid(W, H, I_GROUND)
ring(g, W, H, north_gap=(9, 10), south_gap=(9, 10), east_gap=(15, 16), wall=I_WALL)
# snow encounter patches
patch(g, 2, 5, 6, 9, I_GROUND)
patch(g, 13, 20, 17, 25, I_GROUND)
# frozen lake (passable ice; here just open snow ground - field-move free)
patch(g, 3, 14, 8, 18, I_GROUND)
# scattered ice pillars to shape the route
for (x, y) in [(8, 11), (11, 22), (6, 27), (12, 7)]:
    g[y][x] = I_WALL
results['Schism_FrozenTundra'] = write_layout('Schism_FrozenTundra', g, BORDER_I)
reg_layout('LAYOUT_SCHISM_FROZEN_TUNDRA', 'Schism_FrozenTundra', W, H, *ICE)

# ---------------- IceCity (28x24) ----------------
# south -> FrozenTundra, east -> IceCave. Fortress settlement. Eira's command post.
W, H = 28, 24
g = new_grid(W, H, I_GROUND)
ring(g, W, H, south_gap=(13, 14), east_gap=(11, 12), wall=I_WALL)
# iron pillars at south gate
g[H-2][12] = I_WALL
g[H-2][15] = I_WALL
results['Schism_IceCity'] = write_layout('Schism_IceCity', g, BORDER_I)
reg_layout('LAYOUT_SCHISM_ICE_CITY', 'Schism_IceCity', W, H, *ICE)

# ---------------- IceCave (20x20) ----------------
# west -> IceCity, east -> SealChamber_North (warp). Short linear cave path.
W, H = 20, 20
g = new_grid(W, H, I_GROUND)
ring(g, W, H, west_gap=(9, 10), wall=I_WALL)
# cave-narrowing ice columns
patch(g, 6, 4, 7, 8, I_WALL)
patch(g, 12, 11, 13, 16, I_WALL)
# inscription pedestal (sign sits adjacent)
g[3][10] = I_WALL
results['Schism_IceCave'] = write_layout('Schism_IceCave', g, BORDER_I)
reg_layout('LAYOUT_SCHISM_ICE_CAVE', 'Schism_IceCave', W, H, *ICE)

# =====================================================================
#  THE SCAR + RUINS (General + Cave, grey, WEATHER_NONE)
# =====================================================================

# ---------------- TheScar (28x20) ----------------
# west <- FrozenTundra (ice access), east <- ToxicSwamp (poison access),
# south -> ScarRuins (warp). Devastated grey no-man's-land. Central ruins.
W, H = 28, 20
g = new_grid(W, H, S_GROUND)
ring(g, W, H, west_gap=(9, 10), east_gap=(9, 10), wall=S_WALL)
# central ruin structure (walls forming a broken ring; the ruin sign sits south of it)
patch(g, 11, 6, 16, 6, S_WALL)
patch(g, 11, 6, 11, 10, S_WALL)
patch(g, 16, 6, 16, 10, S_WALL)
g[6][13] = S_GROUND                 # broken gap in the ruin wall (north)
g[6][14] = S_GROUND
patch(g, 12, 7, 15, 9, S_GROUND)    # walkable interior of the ruin
# Dorne's footprints trail (flavor; just open ground, sign placed in mapjson)
results['Schism_TheScar'] = write_layout('Schism_TheScar', g, BORDER_S)
reg_layout('LAYOUT_SCHISM_THE_SCAR', 'Schism_TheScar', W, H, *SCAR)

# ---------------- ScarRuins (20x22) ----------------
# north warp <- TheScar. Deeper intact ruins; murals + cipher 5.
W, H = 20, 22
g = new_grid(W, H, S_GROUND)
ring(g, W, H, wall=S_WALL)
# mural chambers: inner walls forming a central hall
patch(g, 4, 4, 15, 4, S_WALL)
patch(g, 4, 4, 4, 12, S_WALL)
patch(g, 15, 4, 15, 12, S_WALL)
g[4][9] = S_GROUND                  # doorway into the central hall
g[4][10] = S_GROUND
# mural pedestals (signs adjacent)
g[6][7]  = S_WALL                   # Glacith mural
g[6][12] = S_WALL                   # Toxara mural
g[10][9] = S_WALL                   # central cipher pedestal
g[10][10] = S_WALL
results['Schism_ScarRuins'] = write_layout('Schism_ScarRuins', g, BORDER_S)
reg_layout('LAYOUT_SCHISM_SCAR_RUINS', 'Schism_ScarRuins', W, H, *SCAR)

# =====================================================================
#  SOUTH / POISON HALF (General + PelagiosPoison, WEATHER_RAIN)
# =====================================================================

# ---------------- VenomquayPort (20x18) ----------------
# Sea to the south (Galleon docks). North exit -> ToxicSwamp.
W, H = 20, 18
g = new_grid(W, H, P_GROUND)
ring(g, W, H, north_gap=(9, 10), wall=P_WALL)
patch(g, 1, 13, W-2, H-1, P_WATER)          # southern toxic sea
for y in range(12, 16):
    g[y][9] = P_GROUND
    g[y][10] = P_GROUND
g[1][8] = P_WALL
g[1][11] = P_WALL
results['Schism_VenomquayPort'] = write_layout('Schism_VenomquayPort', g, BORDER_P)
reg_layout('LAYOUT_SCHISM_VENOMQUAY_PORT', 'Schism_VenomquayPort', W, H, *POISON)

# ---------------- ToxicSwamp (20x32) ----------------
# south -> VenomquayPort, north -> PoisonCity, west -> TheScar (faction-gated).
# Elevated walkways over toxic water.
W, H = 20, 32
g = new_grid(W, H, P_GROUND)
ring(g, W, H, north_gap=(9, 10), south_gap=(9, 10), west_gap=(15, 16), wall=P_WALL)
# toxic-patch encounter areas
patch(g, 2, 5, 6, 9, P_GROUND)
patch(g, 13, 20, 17, 25, P_GROUND)
# toxic water pools flanking the walkway (impassable)
patch(g, 2, 13, 5, 17, P_WATER)
patch(g, 14, 12, 17, 16, P_WATER)
for (x, y) in [(8, 11), (11, 22), (7, 27), (12, 7)]:
    g[y][x] = P_WALL
results['Schism_ToxicSwamp'] = write_layout('Schism_ToxicSwamp', g, BORDER_P)
reg_layout('LAYOUT_SCHISM_TOXIC_SWAMP', 'Schism_ToxicSwamp', W, H, *POISON)

# ---------------- PoisonCity (28x24) ----------------
# south -> ToxicSwamp, west -> poison cave (warp to SealChamber_South via Laboratory? no:
# brief: PoisonCity west -> poison cave -> SealChamber_South). We model the poison cave
# entrance as a warp on PoisonCity directly into SealChamber_South (short, like IceCave
# is its own map north; south's "cave" is condensed into the chamber warp to keep 21 maps).
W, H = 28, 24
g = new_grid(W, H, P_GROUND)
ring(g, W, H, south_gap=(13, 14), wall=P_WALL)
g[H-2][12] = P_WALL
g[H-2][15] = P_WALL
# west cave-mouth alcove (warp tile to SealChamber_South)
patch(g, 1, 10, 3, 13, P_GROUND)
results['Schism_PoisonCity'] = write_layout('Schism_PoisonCity', g, BORDER_P)
reg_layout('LAYOUT_SCHISM_POISON_CITY', 'Schism_PoisonCity', W, H, *POISON)

print(json.dumps({'sizes': results}, indent=1))

with open(os.path.join(os.path.dirname(__file__), 'schism_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)
