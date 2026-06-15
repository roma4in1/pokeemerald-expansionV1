#!/usr/bin/env python3
"""Compose Thalvern Isle layout blockdata (map.bin/border.bin) + merge layout
entries into data/layouts/layouts.json.

Mirrors tools/pelagios/build_schism.py / build_emberveil.py.

Thalvern is the SUNKEN KINGDOM: a partially drowned archipelago of stilt
settlements over rising water and ancient submerged ruins. Water/Psychic.

Tilesets:
  Outdoor surface maps (Port, FloatingMarket, DexCamp, CoastalRoute,
    SubmergedRuins_Exterior, CovenantSite, DeepApproach):
      gTileset_General (primary) + gTileset_PelagiosUnderwater (secondary)
      (the underwater secondary's terrain palette gives the drowned, watery
       look the brief wants for the surface ruins; CovenantSite/DeepApproach
       reuse it too for visual continuity.)
  Submerged interior maps (Interior1/2/3, ThroneRoom):
      gTileset_General (primary) + gTileset_PelagiosUnderwater (secondary)

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
The custom Pelagios secondary tilesets are flat-fill terrain. Paired with the
512-metatile General primary, a secondary's LOCAL metatile id L is GLOBAL id
512+L (NUM_METATILES_IN_PRIMARY). We reference secondary id 1 (filled terrain
metatile; id 0 is blank by convention) and bake collision/elevation into the
layout word, exactly as Schism/Emberveil did.

SubmergedRuins_Interior1 needs a raised-platform puzzle: walkable stone
platforms (elev 3, col 0) bridging flooded sections (deep water, col 1, elev 0).
We build that elevation/collision split here; the actual Dive/traversal logic is
engine + script-writer triggers.

ThroneRoom is the VAST deepest chamber — built oversized (28x40) as a grand
sunken-throne hall: a wide colonnade leading to a raised throne dais, flooded
side aisles, the seal apparatus before the throne. Monumental scale.

Composed outdoor + the submerged interiors are built here. Inns / PokeCenters
reuse vanilla LAYOUT_* wholesale in build_thalvern_mapjson.py (zero new binary).
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

SEC_BASE = 512  # NUM_METATILES_IN_PRIMARY

def mt(local_id, collision=0, elevation=3):
    """Build a layout word for a SECONDARY-tileset metatile (local id -> +512)."""
    gid = SEC_BASE + local_id
    return (gid & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

FLOOR_ID = 1  # secondary flat-fill terrain metatile

# --- Underwater / drowned terrain palette (PelagiosUnderwater secondary) ---
U_GROUND = mt(FLOOR_ID, 0, 3)    # stone walkway / platform / stilt-deck (walkable)
U_WALL   = mt(FLOOR_ID, 1, 0)    # ruined stone wall / pillar (solid)
U_WATER  = mt(FLOOR_ID, 1, 0)    # flooded section / deep sea (impassable, elev 0)
BORDER_U = [U_WALL] * 4

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

UW = ('gTileset_General', 'gTileset_PelagiosUnderwater')

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
#  SURFACE MAPS (General + PelagiosUnderwater)
# =====================================================================

# ---------------- TidespirePort (20x18) ----------------
# Stilt port over the water. Sea to the south (Galleon docks). North -> FloatingMarket.
W, H = 20, 18
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(9, 10), wall=U_WALL)
patch(g, 1, 13, W-2, H-1, U_WATER)          # southern sea
# central stone dock down into the water for the Tennyson (Galleon)
for y in range(12, 16):
    g[y][9] = U_GROUND
    g[y][10] = U_GROUND
# stilt pillars flanking north exit
g[1][8] = U_WALL
g[1][11] = U_WALL
results['Thalvern_TidespirePort'] = write_layout('Thalvern_TidespirePort', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_TIDESPIRE_PORT', 'Thalvern_TidespirePort', W, H, *UW)

# ---------------- FloatingMarket (28x24) ----------------
# Main settlement: connected floating platforms. south -> TidespirePort,
# right -> CoastalRoute, up -> DexCamp.
W, H = 28, 24
g = new_grid(W, H, U_GROUND)
ring(g, W, H, south_gap=(13, 14), east_gap=(11, 12), north_gap=(13, 14), wall=U_WALL)
# open-water channels between the platforms (impassable) - shape the market
patch(g, 3, 8, 6, 11, U_WATER)
patch(g, 21, 15, 24, 18, U_WATER)
patch(g, 9, 18, 12, 20, U_WATER)
# stilt building footprints (warp tiles sit on plain ground in front of these)
results['Thalvern_FloatingMarket'] = write_layout('Thalvern_FloatingMarket', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_FLOATING_MARKET', 'Thalvern_FloatingMarket', W, H, *UW)

# ---------------- DexCamp (16x14) ----------------
# Dex's research camp on the market's edge. south -> FloatingMarket only.
W, H = 16, 14
g = new_grid(W, H, U_GROUND)
ring(g, W, H, south_gap=(7, 8), wall=U_WALL)
# a little water lapping at the camp's north edge (flavor)
patch(g, 1, 1, 4, 2, U_WATER)
results['Thalvern_DexCamp'] = write_layout('Thalvern_DexCamp', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_DEX_CAMP', 'Thalvern_DexCamp', W, H, *UW)

# ---------------- CoastalRoute (20x34) ----------------
# Partially flooded route, elevated walkways. WEST opening <- FloatingMarket.right,
# NORTH opening -> SubmergedRuins_Exterior.down. Fog. Vision-patch coord triggers.
W, H = 20, 34
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(9, 10), west_gap=(16, 17), wall=U_WALL)
# flooded sections flanking the elevated walkway (impassable deep water)
patch(g, 1, 6, 5, 11, U_WATER)
patch(g, 14, 9, 18, 14, U_WATER)
patch(g, 2, 20, 6, 26, U_WATER)
patch(g, 13, 22, 17, 28, U_WATER)
# narrow the walkway with a couple of ruined pillars
for (x, y) in [(8, 14), (11, 24), (9, 30)]:
    g[y][x] = U_WALL
# keep a continuous walkable spine down the centre (x 9-10) - re-open any
# pillar that landed on it
for y in range(1, H-1):
    g[y][9] = U_GROUND
    g[y][10] = U_GROUND
results['Thalvern_CoastalRoute'] = write_layout('Thalvern_CoastalRoute', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_COASTAL_ROUTE', 'Thalvern_CoastalRoute', W, H, *UW)

# ---------------- SubmergedRuins_Exterior (24x20) ----------------
# Ancient stone structures, water rising. south <- CoastalRoute. Gym 3 (Lens)
# stationed outside. Warp down into Interior1.
W, H = 24, 20
g = new_grid(W, H, U_GROUND)
ring(g, W, H, south_gap=(11, 12), wall=U_WALL)
# exposed ruin walls (the scale is remarkable) + a doorway gap to the interior warp
patch(g, 8, 4, 15, 4, U_WALL)
patch(g, 8, 4, 8, 9, U_WALL)
patch(g, 15, 4, 15, 9, U_WALL)
g[4][11] = U_GROUND                  # doorway gap (north into the ruin face)
g[4][12] = U_GROUND
patch(g, 9, 5, 14, 8, U_GROUND)      # walkable plaza before the entrance
# rising water encroaching from the sides
patch(g, 1, 14, 4, 18, U_WATER)
patch(g, 19, 14, 22, 18, U_WATER)
results['Thalvern_SubmergedRuins_Exterior'] = write_layout('Thalvern_SubmergedRuins_Exterior', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_SUBMERGED_RUINS_EXTERIOR', 'Thalvern_SubmergedRuins_Exterior', W, H, *UW)

# =====================================================================
#  SUBMERGED INTERIORS (General + PelagiosUnderwater)
# =====================================================================

# ---------------- SubmergedRuins_Interior1 (22x22) — PLATFORM PUZZLE ----------------
# First chamber, partially flooded floor. Gym 2 (Psalm) studies an inscription
# at the far north. The PLATFORM PUZZLE: walkable stone platforms (elev 3, col 0)
# bridge a large flooded section (deep water, elev 0, col 1). The player must
# route across the platforms. Wild Water Pokemon live in the flooded sections
# (a wild table is attached to this map). Dive/submerge logic + periodic-submerge
# tile animation are engine + script-writer concerns; the geometry is here.
W, H = 22, 22
g = new_grid(W, H, U_WATER)          # whole floor starts flooded
ring(g, W, H, wall=U_WALL)
# Entry apron (south, walkable) where the warp from the Exterior lands
patch(g, 8, 18, 13, 20, U_GROUND)
# North dais where Psalm stands, walkable
patch(g, 7, 1, 14, 3, U_GROUND)
# Stepping-stone stone platforms across the flood (each a small walkable island)
PLATFORMS = [
    (9, 16, 12, 17),   # near the entry
    (4, 13, 6, 14),    # west branch
    (15, 13, 17, 14),  # east branch
    (9, 12, 12, 13),   # centre
    (4, 9, 6, 10),     # west
    (15, 9, 17, 10),   # east
    (9, 8, 12, 9),     # centre
    (7, 4, 14, 5),     # connects up to the dais
]
for (x0, y0, x1, y1) in PLATFORMS:
    patch(g, x0, y0, x1, y1, U_GROUND)
# Narrow walkable connectors so the platforms form ONE solvable path
# (entry -> centre16 -> centre12 -> centre8 -> dais), plus side spurs to the
# west/east platforms (dead-ends with the wild-water flavor / future Dive).
for y in range(14, 17):              # entry apron up to centre16
    g[y][10] = U_GROUND; g[y][11] = U_GROUND
for y in range(10, 13):              # centre12 up to centre8 region
    g[y][10] = U_GROUND; g[y][11] = U_GROUND
for y in range(6, 9):                # centre8 up to dais
    g[y][10] = U_GROUND; g[y][11] = U_GROUND
g[13][7] = U_GROUND; g[13][8] = U_GROUND   # spur to west-13 platform
g[13][13] = U_GROUND; g[13][14] = U_GROUND # spur to east-13 platform
results['Thalvern_SubmergedRuins_Interior1'] = write_layout('Thalvern_SubmergedRuins_Interior1', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR1', 'Thalvern_SubmergedRuins_Interior1', W, H, *UW)

# ---------------- SubmergedRuins_Interior2 (22x20) ----------------
# Deeper, more intact. Covenant researchers; one blocks a side passage.
# Central mural (examine -> extended vision). The Lens may be here.
W, H = 22, 20
g = new_grid(W, H, U_GROUND)
ring(g, W, H, wall=U_WALL)
# mural hall: inner walls framing a central mural pedestal
patch(g, 4, 4, 17, 4, U_WALL)
patch(g, 4, 4, 4, 12, U_WALL)
patch(g, 17, 4, 17, 12, U_WALL)
g[4][10] = U_GROUND                  # doorway into the mural hall
g[4][11] = U_GROUND
patch(g, 5, 5, 16, 11, U_GROUND)     # walkable hall interior
g[7][10] = U_WALL                    # central mural pedestal (sign adjacent)
g[7][11] = U_WALL
# side passage (east) a researcher blocks; flooded pools for atmosphere
g[7][17] = U_GROUND                  # doorway gap in the east wall to the passage
g[8][17] = U_GROUND
patch(g, 18, 6, 20, 9, U_GROUND)     # the side passage pocket (now reachable)
patch(g, 1, 14, 4, 17, U_WATER)
patch(g, 17, 14, 20, 17, U_WATER)
results['Thalvern_SubmergedRuins_Interior2'] = write_layout('Thalvern_SubmergedRuins_Interior2', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR2', 'Thalvern_SubmergedRuins_Interior2', W, H, *UW)

# ---------------- SubmergedRuins_Interior3 (18x16) ----------------
# Pre-Throne Room chamber. The Lens delivers the choice warning. Quiet, tense.
W, H = 18, 16
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(8, 9), south_gap=(8, 9), wall=U_WALL)
# a reflecting pool flanking the approach
patch(g, 2, 6, 4, 9, U_WATER)
patch(g, 13, 6, 15, 9, U_WATER)
results['Thalvern_SubmergedRuins_Interior3'] = write_layout('Thalvern_SubmergedRuins_Interior3', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR3', 'Thalvern_SubmergedRuins_Interior3', W, H, *UW)

# ---------------- ThroneRoom (28x40) — THE VAST DEEPEST CHAMBER ----------------
# The deepest, most ancient, most significant room in the game so far. A grand
# sunken-throne hall: enter from the south, walk a long central colonnade flanked
# by flooded aisles and pillar rows, up to a raised throne dais where Pelagios's
# seal sits. Monumental scale, deliberately oversized.
W, H = 28, 40
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(13, 14), south_gap=(13, 14), wall=U_WALL)
# Flooded outer aisles down the long sides (impassable deep water) - the hall is
# a drowned cathedral; only the raised central nave is walkable.
patch(g, 1, 1, 6, H-2, U_WATER)       # west aisle (flooded)
patch(g, 21, 1, 26, H-2, U_WATER)     # east aisle (flooded)
# Central nave: a wide walkable stone causeway down the middle (x 8-19)
patch(g, 8, 1, 19, H-2, U_GROUND)
# Twin colonnades of pillars lining the nave (rows of solid pillars, leaving the
# central walkway open) - this is what reads as "monumental".
for y in range(6, H-6, 3):
    g[y][8]  = U_WALL                 # west colonnade
    g[y][19] = U_WALL                 # east colonnade
    g[y][11] = U_WALL                 # inner west row
    g[y][16] = U_WALL                 # inner east row
# Keep the very centre (x 13-14) a clear processional path the whole length
for y in range(1, H-1):
    g[y][13] = U_GROUND
    g[y][14] = U_GROUND
# THE THRONE DAIS at the far north (raised platform, walkable), with the throne
# itself (a solid block the player faces) and the seal apparatus before it.
patch(g, 9, 2, 18, 6, U_GROUND)       # the dais floor
g[3][13] = U_WALL                     # throne block (centre, player faces it from south)
g[3][14] = U_WALL
g[2][13] = U_WALL
g[2][14] = U_WALL
# small reflecting basins flanking the dais (flooded)
patch(g, 9, 5, 10, 6, U_WATER)
patch(g, 17, 5, 18, 6, U_WATER)
# A grand approach: two flanking flooded basins partway down the nave to break
# up the long walk and add scale
patch(g, 9, 20, 10, 23, U_WATER)
patch(g, 17, 20, 18, 23, U_WATER)
# guarantee the central processional spine stays open after all patches
for y in range(1, H-1):
    g[y][13] = U_GROUND
    g[y][14] = U_GROUND
results['Thalvern_ThroneRoom'] = write_layout('Thalvern_ThroneRoom', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_THRONE_ROOM', 'Thalvern_ThroneRoom', W, H, *UW)

# ---------------- CovenantSite (22x18) ----------------
# Covenant temporary base. Numa Vess here. WEATHER_NONE. Accessible after Gym 3.
# down <- (warp from Interior2 side / DeepApproach hub); -> DeepApproach.
W, H = 22, 18
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(10, 11), south_gap=(10, 11), wall=U_WALL)
# tent/crate footprints framed by walls (the base structure)
patch(g, 4, 4, 8, 4, U_WALL)
patch(g, 13, 4, 17, 4, U_WALL)
patch(g, 4, 11, 8, 11, U_WALL)
patch(g, 13, 11, 17, 11, U_WALL)
results['Thalvern_CovenantSite'] = write_layout('Thalvern_CovenantSite', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_COVENANT_SITE', 'Thalvern_CovenantSite', W, H, *UW)

# ---------------- DeepApproach (18x28) ----------------
# Final route to the Throne Room. Covenant blockade (researcher trainers).
# north <- CovenantSite, leads (warp) into Interior3.
W, H = 18, 28
g = new_grid(W, H, U_GROUND)
ring(g, W, H, north_gap=(8, 9), south_gap=(8, 9), wall=U_WALL)
# flooded flanks
patch(g, 1, 6, 4, 11, U_WATER)
patch(g, 13, 14, 16, 20, U_WATER)
# checkpoint pillars (the blockade)
g[12][7] = U_WALL
g[12][10] = U_WALL
# keep the central spine open
for y in range(1, H-1):
    g[y][8] = U_GROUND
    g[y][9] = U_GROUND
results['Thalvern_DeepApproach'] = write_layout('Thalvern_DeepApproach', g, BORDER_U)
reg_layout('LAYOUT_THALVERN_DEEP_APPROACH', 'Thalvern_DeepApproach', W, H, *UW)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'thalvern_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)

# --- merge into layouts.json idempotently (by id) ---
with open(LAYOUTS_JSON) as f:
    doc = json.load(f)
existing = {l.get('id'): i for i, l in enumerate(doc['layouts'])}
for e in layout_entries:
    if e['id'] in existing:
        doc['layouts'][existing[e['id']]] = e
    else:
        doc['layouts'].append(e)
with open(LAYOUTS_JSON, 'w') as f:
    json.dump(doc, f, indent=2)
    f.write('\n')
print(f'merged {len(layout_entries)} Thalvern layout entries into layouts.json')
