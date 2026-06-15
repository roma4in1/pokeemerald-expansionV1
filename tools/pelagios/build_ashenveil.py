#!/usr/bin/env python3
"""Compose Ashenveil Isle (the DEAD ISLAND) layout blockdata (map.bin/border.bin)
+ merge layout entries into data/layouts/layouts.json.

Mirrors tools/pelagios/build_primalis.py (the freshest island template).

Ashenveil is UNLIKE every prior island: dead, empty, fog + permanent dusk, ash
falling everywhere. No inn, no nurse, no heal location. No trainers, no gyms. The
ONLY living NPC is Orin at the Greyport outpost; everyone else is environmental
detail (examine bg_events). Pure narrative island.

Tilesets:
  Outdoor maps (Greyport, AshFields1, DeadCity_Exterior, TheMemorial, MorthasGrove):
      gTileset_General (primary) + gTileset_Slateport (secondary)
      Slateport's grey stone paving + walls are the closest vanilla match to an
      ash-grey ruined city. We use the GREYEST/most-ruined metatiles to approximate
      "permanent dusk". TRUE dusk palette tinting is a Porymap/C concern (a custom
      darkened palette per map) - approximated here with grey stone metatiles + fog
      weather. DOCUMENTED LIMITATION: maps will read grey-but-not-dark until a
      Porymap palette pass darkens them; MorthasGrove is meant to be the darkest.
  Interiors (Greyport_Outpost, _Outpost_Interior, DeadCity_Ruins1, _Ruins2):
      reuse vanilla LAYOUT_* wholesale in build_ashenveil_mapjson.py (zero new
      binary).

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All metatiles are gTileset_General/Slateport primaries (id < 512) with
collision/elevation baked into the layout word, exactly as build_ironhold.py /
build_primalis.py do.

Metatiles used (sampled verbatim from LAYOUT_SLATEPORT_CITY / General):
  ASH       0x3001  grey plaza ground (walkable) = ash-covered ground
  WALL      0x04C6  solid grey stone wall block (impassable) = ruined masonry
  WALL2     0x04C7  solid grey stone wall variant (impassable)
  WATER     0x1170  deep water (Morthas's dark pool)
  W_S       0x1179  water, land to south (pool north edge)
  GRASS     0x0001  General grass (walkable) = TheMemorial garden, only green
  TALLGRASS 0x000D  General tall-grass (wild-encounter tile)
  RUBBLE    0x04C6  WALL reused as impassable ash-wall (Phantom-Lantern gate)
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

def mt(metatile_id, collision=0, elevation=3):
    return (metatile_id & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

# --- ash / dead-island terrain palette (General + Slateport primaries) ---
ASH       = mt(0x3001, 0, 3)   # grey ash-covered ground (walkable)
WALL      = mt(0x04C6, 1, 0)   # ruined grey stone wall (impassable)
WALL2     = mt(0x04C7, 1, 0)   # ruined grey stone wall variant (impassable)
ASHWALL   = mt(0x04C6, 1, 0)   # PHANTOM-LANTERN ash-wall obstacle (impassable)
WATER     = mt(0x1170, 1, 3)   # dark pool (Morthas's seal)
GRASS     = mt(0x0001, 0, 3)   # garden grass (TheMemorial - only green on island)
TALLGRASS = mt(0x000D, 0, 3)   # wild-encounter grass
GRAVE     = mt(0x04C7, 1, 0)   # grave marker block (impassable, faced from south)

BORDER_ASH   = [WALL] * 4

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

AS = ('gTileset_General', 'gTileset_Slateport')

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

def hline(g, x0, x1, y, tile):
    for x in range(x0, x1+1):
        g[y][x] = tile

def vline(g, x, y0, y1, tile):
    for y in range(y0, y1+1):
        g[y][x] = tile

# =====================================================================
#  Greyport (20x18) — tiny ruined Galleon port, the outpost
# =====================================================================
# South sea-edge with central dock for the Galleon (Tennyson). The small
# research outpost (Orin's) is the only building - warp on the map. North ->
# AshFields1. Arrival tile (10,14) on the dock. Permanent dusk approximated by
# grey stone everywhere; ash falls = fog weather (map.json).
W, H = 20, 18
g = new_grid(W, H, ASH)
ring(g, W, H, north_gap=(9, 10), wall=WALL)
# southern sea edge: a thin ruined band so the dock juts into the harbour
patch(g, 1, 16, W-2, H-1, WALL)
# central dock down to the water for the Tennyson (Galleon)
for y in range(13, 17):
    g[y][9] = ASH
    g[y][10] = ASH
# the outpost building footprint (ruined masonry walls; door warp on plain ash)
patch(g, 4, 4, 7, 6, WALL)
g[6][5] = ASH   # doorway gap (warp tile in front sits on ash at (5,7))
# scattered ruined-wall rubble framing the dead port (decorative impassable)
for (x, y) in [(13, 5), (15, 8), (3, 10), (16, 11), (12, 11)]:
    g[y][x] = WALL2
results['Ashenveil_Greyport'] = write_layout('Ashenveil_Greyport', g, BORDER_ASH)
reg_layout('LAYOUT_ASHENVEIL_GREYPORT', 'Ashenveil_Greyport', W, H, *AS)

# =====================================================================
#  AshFields1 (22x32) — ash plains, Phantom-Lantern hidden paths
# =====================================================================
# south <- Greyport, north -> DeadCity_Exterior. Wild Ghost encounters in ash
# grass. PHANTOM-LANTERN ash-walls gate OPTIONAL side nooks; the MAIN north-south
# spine (cols 10-11) stays fully open so the player can ALWAYS reach DeadCity even
# if a gate is mid-implementation (softlock safety). The Lantern is obtained EARLY
# at the Greyport outpost, so gates can block safely - but we keep main progress
# open as belt-and-braces. Environmental bg_events (4) placed in map.json.
W, H = 22, 32
g = new_grid(W, H, ASH)
ring(g, W, H, north_gap=(10, 11), south_gap=(10, 11), wall=WALL)
# wild-encounter ash-grass patches
patch(g, 3, 22, 7, 27, TALLGRASS)
patch(g, 14, 23, 18, 28, TALLGRASS)
patch(g, 4, 6, 8, 9, TALLGRASS)
# ruined-street masonry character (never blocks the central spine cols 10-11)
for (x, y) in [(5, 14), (16, 14), (6, 19), (15, 19), (4, 28), (17, 6)]:
    g[y][x] = WALL2
# PHANTOM-LANTERN ash-wall obstacle: seals an OPTIONAL west reward nook.
# The nook interior at (2-4, 16-18) holds encounter grass; an ash-wall row at
# y=15 (cols 2-4) blocks it until the Lantern reveals the hidden path. Main spine
# untouched. Coord triggers (TODO, script-writer) sit just south of the wall.
patch(g, 2, 16, 4, 18, TALLGRASS)   # nook interior (the reward)
hline(g, 2, 4, 15, ASHWALL)         # ash-wall seal (Lantern clears this row)
# a SECOND optional east nook, same pattern
patch(g, 17, 16, 19, 18, TALLGRASS)
hline(g, 17, 19, 15, ASHWALL)
results['Ashenveil_AshFields1'] = write_layout('Ashenveil_AshFields1', g, BORDER_ASH)
reg_layout('LAYOUT_ASHENVEIL_ASH_FIELDS1', 'Ashenveil_AshFields1', W, H, *AS)

# =====================================================================
#  DeadCity_Exterior (32x30) — the full ruined city, LARGE (scale is the point)
# =====================================================================
# south <- AshFields1, up -> TheMemorial. Warps down into Ruins1 + Ruins2.
# Wild Ghost encounters. NO NPCs. A grid of ruined building footprints (blocks of
# WALL) with ash streets between them - emphasises the dead-city scale.
W, H = 32, 30
g = new_grid(W, H, ASH)
ring(g, W, H, north_gap=(15, 16), south_gap=(15, 16), wall=WALL)
# a grid of ruined building blocks (impassable masonry) with ash streets between
for by in range(3, H-6, 7):
    for bx in range(3, W-5, 8):
        patch(g, bx, by, bx+4, by+3, WALL)
# carve doorway gaps into two specific buildings (the Ruins1 + Ruins2 warps sit
# on ash just south of these openings, placed in map.json)
g[6][5]  = ASH    # Ruins1 building doorway (warp at (5,7))
g[6][13] = ASH    # Ruins2 building doorway (warp at (13,7))
# encounter ash-grass scattered through the rubble streets
patch(g, 24, 10, 28, 14, TALLGRASS)
patch(g, 8, 18, 12, 22, TALLGRASS)
patch(g, 20, 22, 24, 26, TALLGRASS)
# guarantee the central N-S spine (cols 15-16) is fully walkable
for y in range(1, H-1):
    g[y][15] = ASH
    g[y][16] = ASH
results['Ashenveil_DeadCity_Exterior'] = write_layout('Ashenveil_DeadCity_Exterior', g, BORDER_ASH)
reg_layout('LAYOUT_ASHENVEIL_DEAD_CITY_EXTERIOR', 'Ashenveil_DeadCity_Exterior', W, H, *AS)

# =====================================================================
#  TheMemorial (20x18) — small GARDEN, the ONLY green on the island
# =====================================================================
# south <- DeadCity_Exterior, right -> MorthasGrove. A small maintained garden
# (grass patch) amid the ash. 4 grave markers (examine bg_events); one newer.
# NO living NPCs placed in the layout, but the centre is kept clear/walkable for
# the script-managed two-NPC Dorne tableau (Dorne object hidden via flag in
# map.json). NO wild encounters.
W, H = 20, 18
g = new_grid(W, H, ASH)
ring(g, W, H, south_gap=(9, 10), east_gap=(8, 9), wall=WALL)
# the maintained garden: a green grass patch (the only green on Ashenveil)
patch(g, 5, 4, 14, 12, GRASS)
# low ruined-wall border framing the garden plot
hline(g, 4, 15, 3, WALL2)
hline(g, 4, 15, 13, WALL2)
vline(g, 4, 4, 12, WALL2)
vline(g, 15, 4, 12, WALL2)
# re-open the garden interior + a south entrance gap and an east exit gap
patch(g, 5, 4, 14, 12, GRASS)
g[13][9] = GRASS; g[13][10] = GRASS   # south entrance into the garden
g[8][15] = GRASS                      # east opening toward the grove exit
# grave markers: a short row of stones along the north of the garden (impassable,
# faced from the south). The newest grave (Vael Jr.) is the rightmost.
for x in (6, 9, 12, 14):
    g[5][x] = GRAVE
# keep a clear central tableau space (8-11, 8-11) walkable for the Dorne scene
patch(g, 8, 8, 11, 11, GRASS)
results['Ashenveil_TheMemorial'] = write_layout('Ashenveil_TheMemorial', g, BORDER_ASH)
reg_layout('LAYOUT_ASHENVEIL_THE_MEMORIAL', 'Ashenveil_TheMemorial', W, H, *AS)

# =====================================================================
#  MorthasGrove (22x22) — cathedral of dead trees, dark central pool
# =====================================================================
# west <- TheMemorial. The darkest map: ash-preserved dead trees (WALL clumps as
# trunks) ringing a dark central pool (WATER) - Morthas's seal. Morthas overworld
# object near the pool (hidden once encountered, via FLAG_MORTHAS_ENCOUNTERED, set
# in map.json). The Sea Chart stone (examine bg_event) near the tree line. Wild
# Ghost encounters (pre-encounter). NO heal.
W, H = 22, 22
g = new_grid(W, H, ASH)
ring(g, W, H, west_gap=(10, 11), wall=WALL)
# cathedral colonnade of dead-tree trunks (impassable) framing a central nave
for y in range(3, H-3, 3):
    g[y][4]  = WALL2
    g[y][7]  = WALL2
    g[y][14] = WALL2
    g[y][17] = WALL2
# the dark central pool (Morthas's seal) - a square of dark water
patch(g, 9, 9, 12, 12, WATER)
# walkable approach ring around the pool (the player stops at the pool's edge)
patch(g, 8, 8, 13, 8, ASH)     # north approach
patch(g, 8, 13, 13, 13, ASH)   # south approach
vline(g, 8, 8, 13, ASH)        # west approach
vline(g, 13, 8, 13, ASH)       # east approach
# wild-encounter ash-grass (pre-encounter) in the outer grove
patch(g, 2, 16, 5, 19, TALLGRASS)
patch(g, 16, 16, 19, 19, TALLGRASS)
# keep the west entry spine (rows 10-11) walkable in from TheMemorial
for x in range(1, 9):
    g[10][x] = ASH
    g[11][x] = ASH
results['Ashenveil_MorthasGrove'] = write_layout('Ashenveil_MorthasGrove', g, BORDER_ASH)
reg_layout('LAYOUT_ASHENVEIL_MORTHAS_GROVE', 'Ashenveil_MorthasGrove', W, H, *AS)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'ashenveil_layout_entries.json'), 'w') as f:
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
print(f'merged {len(layout_entries)} Ashenveil layout entries into layouts.json')
