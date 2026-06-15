#!/usr/bin/env python3
"""Compose Convergence (the FINAL ISLAND) layout blockdata (map.bin/border.bin)
+ merge layout entries into data/layouts/layouts.json.

Mirrors tools/pelagios/build_ashenveil.py (the freshest story-only island).

Convergence is the ancient capital - overwhelming scale, mixed-era architecture.
NO inn, NO heal location, NO wild encounters, NO trainers/gyms, NO hidden items.
Pure story. Three endings play in the KillSwitchChamber. Return travel is the
Storm Compass (no Tennyson / boarding object on this island).

Composed (custom) layouts (the rest reuse vanilla LAYOUT_* in the mapjson script):
  Convergence_Approach              General + Slateport (grey ancient coastal/ruins)
  Convergence_AncientCapital_Exterior  General + Slateport (LARGE ruined greatest city)
  Convergence_KillSwitchChamber     General + Cave (the most ancient room in the game)
  Convergence_Epilogue              General + Slateport (neutral post-ending stage)

Interiors reused (build_convergence_mapjson.py, zero new binary):
  AncientCapital_Interior1 = LAYOUT_SEALED_CHAMBER_OUTER_ROOM (temple outer, murals)
  AncientCapital_Interior2 = LAYOUT_SEALED_CHAMBER_INNER_ROOM (inner sanctum, switch)

TILESET NOTE: there is NO gTileset_Ruins in this project. The brief's "ancient
secondary / Ruins" intent is served by gTileset_Slateport (grey ancient stone -
the Ashenveil dead-city precedent) for the outdoor capital, and the Sealed Chamber
family (gTileset_Cave) for the temple interiors + KillSwitchChamber (the deepest /
most ancient aesthetic available). Documented substitution.

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
Outdoor words sampled verbatim from LAYOUT_SLATEPORT_CITY (General/Slateport) so
collision/elevation come for free; cave words from LAYOUT_VICTORY_ROAD_1F
(General/Cave). Flat-fill terrain; richer metatiles (cliffs/edges/temple facade)
are a Porymap hand-tuning pass later (established caveat on every Pelagios island).
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

def mt(metatile_id, collision=0, elevation=3):
    return (metatile_id & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

# --- ancient grey stone (General + Slateport primaries), Ashenveil precedent ---
STONE    = mt(0x3001, 0, 3)   # grey plaza ground (walkable) = ancient paving
WALL     = mt(0x04C6, 1, 0)   # solid grey stone wall block (impassable)
WALL2    = mt(0x04C7, 1, 0)   # grey stone wall variant (impassable) = pillar/rubble
WATER    = mt(0x1170, 0, 1)   # deep sea (the Covenant fleet's water)
W_S      = mt(0x1179, 0, 1)   # water, land to south (shore edge north of sea)

# --- cave / deepest ancient (General + Cave primaries), Victory Road sample ---
CAVE     = mt(0x0211, 0, 4)   # cave floor (walkable, elev 4)  word 0x4211
CWALL    = mt(0x0211, 1, 0)   # cave wall (impassable)         word 0x0611
CWALL2   = mt(0x0212, 1, 0)   # cave wall variant              word 0x0612

BORDER_STONE = [WALL] * 4
BORDER_CAVE  = [CWALL] * 4

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

CS = ('gTileset_General', 'gTileset_Slateport')   # capital / coastal stone
CV = ('gTileset_General', 'gTileset_Cave')         # deepest ancient chamber

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
#  1. Convergence_Approach (24x20) — shore gathering point
# =====================================================================
# The island rises from the sea. The Covenant fleet is implied offshore as bg
# detail (decorative ship objects in the southern sea, placed in map.json). Room
# for the gathering scene (Dorne, Sollis, Cass, player) on the central shore.
# Arrival tile (the boat menu warps here). NO return warp (Storm Compass is the
# return). Connects up -> AncientCapital_Exterior.
W, H = 24, 20
g = new_grid(W, H, STONE)
ring(g, W, H, north_gap=(11, 12), wall=WALL)
# southern sea band (the open water the Covenant fleet sits on)
patch(g, 1, 15, W-2, H-1, WATER)
hline(g, 1, W-2, 14, W_S)            # shore edge (land to north of the sea)
# central stone landing jetty reaching toward the water (the arrival point)
patch(g, 10, 12, 13, 14, STONE)
g[14][11] = STONE; g[14][12] = STONE  # jetty foot meets the shore edge
# mixed-era ruined masonry framing the shore (decorative impassable pillars)
for (x, y) in [(3, 4), (6, 3), (19, 4), (16, 3), (4, 9), (20, 9), (8, 11), (16, 11)]:
    g[y][x] = WALL2
# keep a wide clear central gathering plaza (8-15, 6-12) walkable for the scene
patch(g, 8, 6, 15, 12, STONE)
# guarantee the central N-S spine (cols 11-12) to the temple connection is open
for y in range(1, 13):
    g[y][11] = STONE
    g[y][12] = STONE
results['Convergence_Approach'] = write_layout('Convergence_Approach', g, BORDER_STONE)
reg_layout('LAYOUT_CONVERGENCE_APPROACH', 'Convergence_Approach', W, H, *CS)

# =====================================================================
#  2. Convergence_AncientCapital_Exterior (36x34) — LARGE ruined greatest city
# =====================================================================
# Overwhelming scale - everything else in the game was a fragment of this. NPC
# positions for all returning characters: Eira (north), Mako (east), Drenn
# (south), Lace (west), The Lens (cataloguing, central), Dagan (leaning on a
# wall). The temple path is visible ahead (north gap). Connects down -> Approach,
# up -> Interior1. A grand grid of monumental ruined blocks with broad processional
# avenues between them.
W, H = 36, 34
g = new_grid(W, H, STONE)
ring(g, W, H, north_gap=(17, 18), south_gap=(17, 18), wall=WALL)
# a grid of monumental ruined building blocks (impassable) with wide stone avenues
for by in range(4, H-7, 8):
    for bx in range(4, W-6, 9):
        patch(g, bx, by, bx+5, by+4, WALL)
# colonnade pillars lining the central processional avenue (scale/rhythm)
for y in range(3, H-3, 3):
    g[y][14] = WALL2
    g[y][21] = WALL2
# broad clear central processional avenue (cols 16-19) the full height
for y in range(1, H-1):
    for x in range(16, 20):
        g[y][x] = STONE
# clear NPC tableau pockets so the returning-character objects sit on open stone:
patch(g, 16, 2, 19, 4, STONE)     # Eira - north edge
patch(g, 30, 14, 33, 17, STONE)   # Mako - east
patch(g, 16, 29, 19, 31, STONE)   # Drenn - south
patch(g, 2, 14, 5, 17, STONE)     # Lace - west
patch(g, 15, 15, 20, 19, STONE)   # The Lens - central crossing
g[20][24] = STONE; g[19][24] = STONE  # Dagan leaning near an east wall block
results['Convergence_AncientCapital_Exterior'] = write_layout(
    'Convergence_AncientCapital_Exterior', g, BORDER_STONE)
reg_layout('LAYOUT_CONVERGENCE_ANCIENT_CAPITAL_EXTERIOR',
           'Convergence_AncientCapital_Exterior', W, H, *CS)

# =====================================================================
#  3. Convergence_KillSwitchChamber (26x40) — the kill switch, ancient + vast
# =====================================================================
# The most ancient room in the game. Built to last forever. All three endings
# play here; multi-character space (player + Dorne + Cass). Connects FROM
# Interior2 (warp), NO return warp. A long ascending nave of ancient cave stone
# with twin colonnades, flooded/void outer aisles framed as impassable cave wall,
# the kill-switch mechanism on a raised dais at the far north (object in map.json).
W, H = 26, 40
g = new_grid(W, H, CAVE)
ring(g, W, H, south_gap=(12, 13), wall=CWALL)
# impassable outer aisles (cols 1-5 west, 20-24 east) leave only the central nave
patch(g, 1, 1, 5, H-2, CWALL)
patch(g, 20, 1, 24, H-2, CWALL)
# the central nave (cols 6-19) is open cave floor
patch(g, 6, 1, 19, H-2, CAVE)
# twin colonnades of ancient pillars lining the nave (a pillar pair every 3 rows)
for y in range(5, H-6, 3):
    g[y][8]  = CWALL2
    g[y][17] = CWALL2
# flooded reflecting basins partway up break the processional walk (impassable)
patch(g, 9, 18, 11, 19, CWALL)
patch(g, 14, 18, 16, 19, CWALL)
# the raised kill-switch dais at the far north (the mechanism object sits at 12,4)
patch(g, 8, 2, 17, 6, CAVE)
# re-assert the central spine (cols 12-13) walkable the FULL length (entry -> dais)
for y in range(1, H-1):
    g[y][12] = CAVE
    g[y][13] = CAVE
results['Convergence_KillSwitchChamber'] = write_layout(
    'Convergence_KillSwitchChamber', g, BORDER_CAVE)
reg_layout('LAYOUT_CONVERGENCE_KILL_SWITCH_CHAMBER',
           'Convergence_KillSwitchChamber', W, H, *CV)

# =====================================================================
#  4. Convergence_Epilogue (24x20) — single neutral post-ending stage
# =====================================================================
# One map; per-ending NPC positions are flag-gated in map.json (Ending 1 warm /
# Haven-like, Ending 2 open-sea / legendaries, Ending 3 same space full of
# characters). Credits trigger from here. A calm shore-and-stone stage: open
# central plaza, a southern sea horizon (where legendaries appear in Endings 2/3),
# a harbour edge to the south the player walks toward. NO warps (terminal map).
W, H = 24, 20
g = new_grid(W, H, STONE)
ring(g, W, H, wall=WALL)
# southern sea horizon (the legendaries appear over this in Endings 2/3)
patch(g, 1, 15, W-2, H-2, WATER)
hline(g, 1, W-2, 14, W_S)            # shore edge
# a central harbour walk (cols 11-12) down toward the sea (credits walk path)
for y in range(1, 15):
    g[y][11] = STONE
    g[y][12] = STONE
# a couple of low ruined-wall accents framing the stage (decorative impassable)
for (x, y) in [(4, 5), (19, 5), (5, 10), (18, 10)]:
    g[y][x] = WALL2
# wide clear central plaza for the flag-gated NPC roster (Ending 3 is crowded)
patch(g, 5, 4, 18, 12, STONE)
for y in range(1, 15):              # re-assert the harbour walk after the plaza
    g[y][11] = STONE; g[y][12] = STONE
results['Convergence_Epilogue'] = write_layout('Convergence_Epilogue', g, BORDER_STONE)
reg_layout('LAYOUT_CONVERGENCE_EPILOGUE', 'Convergence_Epilogue', W, H, *CS)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'convergence_layout_entries.json'), 'w') as f:
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
print(f'merged {len(layout_entries)} Convergence layout entries into layouts.json')
