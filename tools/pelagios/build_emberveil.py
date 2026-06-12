#!/usr/bin/env python3
"""Compose Emberveil layout blockdata (map.bin/border.bin) from stamps sampled
out of vanilla volcanic layouts (Mt Chimney / Lavaridge for outdoors, Cave of
Origin for the seal chamber), then the companion build_emberveil_mapjson.py
generates all 15 Emberveil map.json files.

Mirrors tools/pelagios/build_ironhold.py.

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All words are copied/constructed from vanilla volcanic tilesets (General/Lavaridge),
so collision and elevation are correct.

Outdoor maps use the General/Lavaridge tileset pair (dark volcanic stone, lava,
ash). The SealChamber reuses a vanilla Cave layout wholesale. Inns / Pokecenter /
Mart / Temple interiors reuse vanilla LAYOUT_* wholesale (zero new binary).
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

# ---- Lavaridge / General volcanic metatile palette --------------------------
# Verified against a dump of LAYOUT_MT_CHIMNEY (General/Lavaridge):
#   0x3271 = volcanic ash ground (LavaField metatile, col0 elev3) - WALKABLE floor
#   0x3671 = lava field (col1 elev3)  - impassable lava obstacle
#   0x0671 = lava field (col1 elev0)  - impassable lava (border/edge)
#   0x0674 = rock wall (col1 elev0)   - solid wall
# General tileset shared:
#   0x300D = TallGrass (col0 elev3)   - triggers wild encounters (proven on Ironhold)
#   0x3001 = plain grass/ground (col0 elev3)
#   0x0403 = sign metatile
#   0x1170 = deep water ; water edges from Slateport-range share with General
V_GROUND = 0x3271   # volcanic ash ground (walkable)
V_WALL   = 0x0674   # rock wall (solid)
V_LAVA   = 0x3671   # lava obstacle (impassable; Lava-Boots gate)
V_GRASS  = 0x300D    # tall grass -> wild encounters
V_SIGN   = 0x0403    # sign
V_WATER  = 0x1170    # deep water (port sea)
# water edges (land on the named side), General/Slateport water range
V_W_N    = 0x1189    # water, land to north
BORDER_V = [V_WALL, V_WALL, V_WALL, V_WALL]
BORDER_WATER = [V_WATER, V_WATER, V_WATER, V_WATER]

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

LAV = ('gTileset_General', 'gTileset_Lavaridge')

def ring(g, w, h, north_gap=(), south_gap=(), west_gap=(), east_gap=(), wall=V_WALL):
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
#  OUTDOOR LAYOUTS (General/Lavaridge)
# =====================================================================

# ---------------- AshmouthPort (20x18) ----------------
# Sea to the south (Brigantine docks). North exit -> LavaRoute1.
W, H = 20, 18
g = new_grid(W, H, V_GROUND)
ring(g, W, H, north_gap=(9, 10))
# southern sea
patch(g, 1, 13, W-2, H-1, V_WATER)
for x in range(1, W-1):
    g[12][x] = V_W_N
# central stone dock down into the water for the Tennyson
for y in range(12, 16):
    g[y][9] = V_GROUND
    g[y][10] = V_GROUND
g[16][9] = V_W_N
g[16][10] = V_W_N
# checkpoint pillars flanking north exit
g[1][8] = V_WALL
g[1][11] = V_WALL
# signs
g[2][12] = V_SIGN     # port sign
g[11][6] = V_SIGN     # berth sign
results['Emberveil_AshmouthPort'] = write_layout('Emberveil_AshmouthPort', g, BORDER_WATER)
reg_layout('LAYOUT_EMBERVEIL_ASHMOUTH_PORT', 'Emberveil_AshmouthPort', W, H, *LAV)

# ---------------- LavaRoute1 (20x28) ----------------
# south -> AshmouthPort, north -> AshFields. Lava channels (decorative, not gated).
W, H = 20, 28
g = new_grid(W, H, V_GROUND)
ring(g, W, H, north_gap=(9, 10), south_gap=(9, 10))
# tall-grass encounter patches
patch(g, 2, 4, 6, 7, V_GRASS)
patch(g, 13, 18, 17, 22, V_GRASS)
# decorative lava channels (impassable, flank the path - not blocking progress)
patch(g, 2, 12, 4, 14, V_LAVA)
patch(g, 15, 9, 17, 11, V_LAVA)
# scattered rock pillars to shape the path
for (x, y) in [(8, 9), (11, 14), (7, 19), (12, 6)]:
    g[y][x] = V_WALL
results['Emberveil_LavaRoute1'] = write_layout('Emberveil_LavaRoute1', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_LAVA_ROUTE1', 'Emberveil_LavaRoute1', W, H, *LAV)

# ---------------- AshFields (24x20) ----------------
# south -> LavaRoute1, north -> CinderholdCity, east -> CalderaApproach (narrative-gated)
# NOTE: per the brief the east route to the caldera goes via CinderholdCity ->
# LavaRoute2, so AshFields does NOT carry a real east connection (the "east"
# mention in the brief is the overall island layout; LavaRoute2 is the actual path).
W, H = 24, 20
g = new_grid(W, H, V_GROUND)
ring(g, W, H, north_gap=(11, 12), south_gap=(11, 12))
# broad ash-grass plains (wild encounters)
patch(g, 3, 4, 9, 9, V_GRASS)
patch(g, 15, 11, 20, 16, V_GRASS)
# dead trees preserved in ash (examined via signs; use wall pillars as the trunks)
for (x, y) in [(6, 13), (8, 13), (17, 5), (19, 6)]:
    g[y][x] = V_WALL
results['Emberveil_AshFields'] = write_layout('Emberveil_AshFields', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_ASH_FIELDS', 'Emberveil_AshFields', W, H, *LAV)

# ---------------- CinderholdCity (28x24) ----------------
# south -> AshFields, east -> LavaRoute2 (blocked narratively until Gym 2)
W, H = 28, 24
g = new_grid(W, H, V_GROUND)
ring(g, W, H, south_gap=(13, 14), east_gap=(11, 12))
# iron pillars at south gate
g[H-1-1][12] = V_WALL
g[H-1-1][15] = V_WALL
# building signs (warps defined in mapjson)
g[4][6]   = V_SIGN    # Gym 1 gatehouse sign (south entrance)
g[10][6]  = V_SIGN    # Temple Hall sign
g[10][21] = V_SIGN    # PokeCenter sign
g[16][21] = V_SIGN    # Lava works / Gym 2 sign
results['Emberveil_CinderholdCity'] = write_layout('Emberveil_CinderholdCity', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_CINDERHOLD_CITY', 'Emberveil_CinderholdCity', W, H, *LAV)

# ---------------- LavaRoute2 (24x18) ----------------
# west -> CinderholdCity, east -> CalderaApproach. Lava obstacles gated by Lava Boots.
W, H = 24, 18
g = new_grid(W, H, V_GROUND)
ring(g, W, H, west_gap=(8, 9), east_gap=(8, 9))
# narrow path with lava flows pressing close.
# A lava barrier spans the route mid-way; the Lava-Boots gate trigger sits just
# west of it. Leave a 2-tile walkable corridor the trigger guards.
patch(g, 11, 1, 12, 7, V_LAVA)     # upper lava wall
patch(g, 11, 10, 12, H-2, V_LAVA)  # lower lava wall  -> gap at rows 8-9 (the gated corridor)
# decorative lava pools
patch(g, 3, 13, 5, 15, V_LAVA)
patch(g, 18, 2, 20, 4, V_LAVA)
results['Emberveil_LavaRoute2'] = write_layout('Emberveil_LavaRoute2', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_LAVA_ROUTE2', 'Emberveil_LavaRoute2', W, H, *LAV)

# ---------------- CalderaApproach (22x22) ----------------
# west -> LavaRoute2, contains warp up into CalderaRuins. Gym 3 (Vex) here.
# Lava-Boots-gated ash field guards the ruins stairway.
W, H = 22, 22
g = new_grid(W, H, V_GROUND)
ring(g, W, H, west_gap=(10, 11))
# agitated-encounter ash grass
patch(g, 3, 4, 8, 8, V_GRASS)
patch(g, 14, 13, 18, 18, V_GRASS)
# lava-boots-gated lava band sealing the north half (the ruins entrance is north).
# Leave a 2-tile gated corridor at x=10-11 the trigger guards.
patch(g, 1, 9, 9, 10, V_LAVA)
patch(g, 12, 9, W-2, 10, V_LAVA)
results['Emberveil_CalderaApproach'] = write_layout('Emberveil_CalderaApproach', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_CALDERA_APPROACH', 'Emberveil_CalderaApproach', W, H, *LAV)

# ---------------- CalderaRuins (20x24) ----------------
# south warp <- CalderaApproach, up warp -> VolcanoAscent. Ancient stone ruins.
# Built mostly of rock walls forming chambers; Path-B panels are signs.
W, H = 20, 24
g = new_grid(W, H, V_GROUND)
ring(g, W, H)
# inner ruin walls forming an alcove for the Warden's notes (north-centre)
patch(g, 4, 4, 15, 4, V_WALL)     # north chamber wall
patch(g, 4, 4, 4, 9, V_WALL)
patch(g, 15, 4, 15, 9, V_WALL)
g[4][9] = V_GROUND                # doorway into the alcove
g[4][10] = V_GROUND
# three ruin panel pedestals (signs sit adjacent; pedestals are walls)
g[6][7]  = V_WALL
g[6][12] = V_WALL
g[3][9]  = V_WALL                 # the alcove panel (Path-B trigger), inside top chamber
results['Emberveil_CalderaRuins'] = write_layout('Emberveil_CalderaRuins', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_CALDERA_RUINS', 'Emberveil_CalderaRuins', W, H, *LAV)

# ---------------- VolcanoAscent (18x30) ----------------
# down -> CalderaRuins, up -> VolcanoSummit. Steep final approach, lava channels.
W, H = 18, 30
g = new_grid(W, H, V_GROUND)
ring(g, W, H, north_gap=(8, 9), south_gap=(8, 9))
# winding lava channels narrowing the path
patch(g, 1, 8, 6, 9, V_LAVA)
patch(g, 11, 14, W-2, 15, V_LAVA)
patch(g, 1, 21, 7, 22, V_LAVA)
# rock pillars
for (x, y) in [(9, 5), (5, 12), (12, 19), (7, 25)]:
    g[y][x] = V_WALL
results['Emberveil_VolcanoAscent'] = write_layout('Emberveil_VolcanoAscent', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_VOLCANO_ASCENT', 'Emberveil_VolcanoAscent', W, H, *LAV)

# ---------------- VolcanoSummit (20x20) ----------------
# down -> VolcanoAscent, warp into SealChamber (after resolution). Solace + seal.
W, H = 20, 20
g = new_grid(W, H, V_GROUND)
ring(g, W, H, south_gap=(9, 10))
# the seal formation: a raised lava-glow ring near the top centre (decorative wall
# ring with a walkable centre where the seal object sits)
patch(g, 7, 3, 12, 3, V_LAVA)
patch(g, 7, 3, 7, 7, V_LAVA)
patch(g, 12, 3, 12, 7, V_LAVA)
patch(g, 7, 7, 12, 7, V_LAVA)
patch(g, 8, 4, 11, 6, V_GROUND)   # walkable ritual platform inside the ring
g[3][9] = V_GROUND                 # gap in the ring (north) so the seal is visible
g[3][10] = V_GROUND
results['Emberveil_VolcanoSummit'] = write_layout('Emberveil_VolcanoSummit', g, BORDER_V)
reg_layout('LAYOUT_EMBERVEIL_VOLCANO_SUMMIT', 'Emberveil_VolcanoSummit', W, H, *LAV)

print(json.dumps({'sizes': results}, indent=1))

with open(os.path.join(os.path.dirname(__file__), 'emberveil_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)
