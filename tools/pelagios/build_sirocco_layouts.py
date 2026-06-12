#!/usr/bin/env python3
"""Compose Sirocco Isle outdoor layout blockdata (map.bin/border.bin) from stamps
sampled out of vanilla layouts, then register the new layouts.

Mirrors tools/pelagios/build_ironhold.py (Ironhold) + build_layouts.py (Haven).

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All metatile words are copied verbatim from vanilla layouts (same tileset pair),
so collision and elevation are inherently correct.

Outdoor Sirocco maps use the General/Mauville tileset pair (Route 111's desert
section: sand 0x3251, deep-sand encounter tile, desert rock walls 0x471).
BuriedCity interiors reuse vanilla LAYOUT_* (General/Cave) wholesale - zero new
binary work. Palace/town building interiors reuse vanilla building LAYOUT_*.
Only the eight OUTDOOR maps get new composed blockdata here.
"""
import json, os, struct
from collections import Counter

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

def load_layouts():
    with open(LAYOUTS_JSON) as f:
        return {l['id']: l for l in json.load(f)['layouts'] if 'id' in l}

def load_blocks(layout):
    with open(os.path.join(ROOT, layout['blockdata_filepath']), 'rb') as f:
        data = f.read()
    w, h = layout['width'], layout['height']
    words = struct.unpack(f'<{w*h}H', data[:w*h*2])
    return [list(words[y*w:(y+1)*w]) for y in range(h)]

L = load_layouts()
R111 = load_blocks(L['LAYOUT_ROUTE111'])   # General/Mauville desert

# ---- Desert (General/Mauville) metatile palette ----------------------------
# Verified by dumping LAYOUT_ROUTE111's desert section (rows 40-95).
D_SAND   = 0x3121     # plain desert sand floor, walkable, elevation 3
D_DEEP   = 0x3251     # METATILE_Mauville_DeepSand_Center - wild-encounter sand
D_ROCK   = 0x0471     # desert rock wall block (collision)
D_ROCK2  = 0x0473     # desert rock wall variant (collision)
# General-tileset shared tiles (present in every General-primary layout)
D_WATER  = 0x1170     # METATILE_General_CalmWater (deep water, collision)
D_W_N    = 0x1179     # water with land to the north (shore edge)
D_SIGN   = 0x0473     # marker block (collision) - bg_event signs face this tile
D_GRASS  = 0x3001     # plain green ground (oasis grass), walkable elevation 3

# Sanity: confirm sampled desert words actually occur in Route 111.
present = set(v for row in R111 for v in row)
for nm, v in [('D_SAND', D_SAND), ('D_DEEP', D_DEEP), ('D_ROCK', D_ROCK)]:
    assert v in present, f'{nm} {hex(v)} not found in Route111 - resample'

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

BORDER_SAND  = [D_ROCK, D_ROCK, D_ROCK, D_ROCK]
BORDER_WATER = [D_WATER, D_WATER, D_WATER, D_WATER]

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

def patch(grid, x0, y0, x1, y1, val):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            grid[y][x] = val

PRIM, SEC = 'gTileset_General', 'gTileset_Mauville'

# =====================================================================
#  DustmouthPort (20x18)  - sea to the south, north exit -> DesertRoute1
# =====================================================================
W, H = 20, 18
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK if not (9 <= x <= 10) else D_SAND   # north exit -> DesertRoute1
for y in range(H):
    g[y][0] = D_ROCK
    g[y][W-1] = D_ROCK
# southern sea (Brigantine docks here)
patch(g, 1, 13, W-2, H-1, D_WATER)
for x in range(1, W-1):
    g[12][x] = D_W_N                  # shore edge, land to north
# stone dock planks down the centre into the water
for y in range(12, 16):
    g[y][9] = D_SAND
    g[y][10] = D_SAND
g[16][9] = D_W_N
g[16][10] = D_W_N
# gate pillars flanking north exit
g[1][8] = D_ROCK
g[1][11] = D_ROCK
g[2][12] = D_SIGN                     # north-exit sign (Miraden Oasis 2 days)
g[11][6] = D_SIGN                     # berth sign
results['Sirocco_DustmouthPort'] = write_layout('Sirocco_DustmouthPort', g, BORDER_WATER)
reg_layout('LAYOUT_SIROCCO_DUSTMOUTH_PORT', 'Sirocco_DustmouthPort', W, H, PRIM, SEC)

# =====================================================================
#  DesertRoute1 (22x40)  - long route, south->Port, north->Oasis, east->DexCamp
# =====================================================================
W, H = 22, 40
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK if not (10 <= x <= 11) else D_SAND     # north -> Oasis
    g[H-1][x] = D_ROCK if not (9 <= x <= 10) else D_SAND     # south -> Port
for y in range(H):
    g[y][0] = D_ROCK
    g[y][W-1] = D_ROCK if not (18 <= y <= 19) else D_SAND    # east -> DexCamp
# deep-sand encounter patches
for (x0, y0, x1, y1) in [(3, 5, 8, 10), (13, 14, 19, 19), (3, 24, 9, 30), (12, 30, 18, 35)]:
    patch(g, x0, y0, x1, y1, D_DEEP)
# dried riverbed - a meandering line of rock down the middle (impassable scar)
for y in range(8, 34, 1):
    rx = 11 + (1 if (y // 3) % 2 == 0 else -1)
    g[y][rx] = D_ROCK2
# leave a crossing gap so the route stays traversable
patch(g, 9, 20, 13, 21, D_SAND)
# sparse dead vegetation as rock pillars
for (x, y) in [(5, 16), (16, 8), (7, 33), (17, 27)]:
    g[y][x] = D_ROCK2
g[36][5] = D_SIGN                      # hidden item bush marker
results['Sirocco_DesertRoute1'] = write_layout('Sirocco_DesertRoute1', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_DESERT_ROUTE_1', 'Sirocco_DesertRoute1', W, H, PRIM, SEC)

# =====================================================================
#  MiradenOasis (28x24) - town around shrinking oasis. south->Route1,
#  east->Route2, north->GiltClawTerritory
# =====================================================================
W, H = 28, 24
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK if not (13 <= x <= 14) else D_SAND      # north -> GiltClawTerritory
    g[H-1][x] = D_ROCK if not (13 <= x <= 14) else D_SAND    # south -> Route1
for y in range(H):
    g[y][0] = D_ROCK
    g[y][W-1] = D_ROCK if not (11 <= y <= 12) else D_SAND    # east -> Route2
# the oasis: a ring of grass around central water, in the town centre
patch(g, 10, 9, 17, 14, D_GRASS)       # green ring
patch(g, 12, 11, 15, 12, D_WATER)      # shrunken water core
for x in range(12, 16):
    g[10][x] = D_W_N                    # shore edge top
# building signs (gyms + center/shop)
g[4][5]   = D_SIGN     # Gym 1 (Silt) warehouse sign
g[4][22]  = D_SIGN     # Gym 2 (Crag) excavation sign
g[16][5]  = D_SIGN     # PokeCenter sign
g[16][22] = D_SIGN     # Shop sign
results['Sirocco_MiradenOasis'] = write_layout('Sirocco_MiradenOasis', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_MIRADEN_OASIS', 'Sirocco_MiradenOasis', W, H, PRIM, SEC)

# =====================================================================
#  DexCamp (16x14) - small camp west exit -> DesertRoute1
# =====================================================================
W, H = 16, 14
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK
    g[H-1][x] = D_ROCK
for y in range(H):
    g[y][0] = D_ROCK if not (6 <= y <= 7) else D_SAND        # west -> DesertRoute1
    g[y][W-1] = D_ROCK
g[2][8] = D_SIGN       # research-notes table marker
results['Sirocco_DexCamp'] = write_layout('Sirocco_DexCamp', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_DEX_CAMP', 'Sirocco_DexCamp', W, H, PRIM, SEC)

# =====================================================================
#  DesertRoute2 (24x18) - west->Oasis, east->BuriedCity_Exterior
# =====================================================================
W, H = 24, 18
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK
    g[H-1][x] = D_ROCK
for y in range(H):
    g[y][0] = D_ROCK if not (8 <= y <= 9) else D_SAND        # west -> Oasis
    g[y][W-1] = D_ROCK if not (8 <= y <= 9) else D_SAND      # east -> BuriedCity_Exterior
# deep-sand encounter patches
for (x0, y0, x1, y1) in [(4, 3, 9, 7), (14, 10, 20, 14)]:
    patch(g, x0, y0, x1, y1, D_DEEP)
# sand columns (geological features) as rock pillars
for (x, y) in [(7, 12), (12, 4), (17, 6), (19, 3)]:
    g[y][x] = D_ROCK2
results['Sirocco_DesertRoute2'] = write_layout('Sirocco_DesertRoute2', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_DESERT_ROUTE_2', 'Sirocco_DesertRoute2', W, H, PRIM, SEC)

# =====================================================================
#  BuriedCity_Exterior (22x20) - excavation. west->Route2, down into Interior1
# =====================================================================
W, H = 22, 20
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK
    g[H-1][x] = D_ROCK
for y in range(H):
    g[y][0] = D_ROCK if not (9 <= y <= 10) else D_SAND       # west -> Route2
    g[y][W-1] = D_ROCK
# exposed ancient stonework: rectangular stone outline mid-map
for x in range(7, 16):
    g[5][x] = D_ROCK2
    g[14][x] = D_ROCK2
for y in range(5, 15):
    g[y][7] = D_ROCK2
    g[y][15] = D_ROCK2
# clear the interior of the stone ring (dig site floor) and a doorway gap
patch(g, 8, 6, 14, 13, D_SAND)
g[5][11] = D_SAND       # doorway in the north stone wall (warp down to Interior1)
# deep-sand encounter patches outside the ruins
for (x0, y0, x1, y1) in [(2, 14, 5, 18), (17, 3, 20, 8)]:
    patch(g, x0, y0, x1, y1, D_DEEP)
g[6][8] = D_SIGN        # ancient-script wall marker
results['Sirocco_BuriedCity_Exterior'] = write_layout('Sirocco_BuriedCity_Exterior', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_BURIED_CITY_EXTERIOR', 'Sirocco_BuriedCity_Exterior', W, H, PRIM, SEC)

# =====================================================================
#  GiltClawTerritory (20x22) - south->Oasis, north->DaganPalace_Exterior
# =====================================================================
W, H = 20, 22
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK if not (9 <= x <= 10) else D_SAND       # north -> DaganPalace_Exterior
    g[H-1][x] = D_ROCK if not (9 <= x <= 10) else D_SAND     # south -> Oasis
for y in range(H):
    g[y][0] = D_ROCK
    g[y][W-1] = D_ROCK
# branded posts (rock pillars) lining a controlled corridor
for y in range(4, 18, 3):
    g[y][6] = D_ROCK2
    g[y][13] = D_ROCK2
# deep-sand encounter patches
for (x0, y0, x1, y1) in [(2, 6, 5, 11), (15, 12, 18, 17)]:
    patch(g, x0, y0, x1, y1, D_DEEP)
results['Sirocco_GiltClawTerritory'] = write_layout('Sirocco_GiltClawTerritory', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_GILT_CLAW_TERRITORY', 'Sirocco_GiltClawTerritory', W, H, PRIM, SEC)

# =====================================================================
#  DaganPalace_Exterior (18x16) - south->GiltClawTerritory, into Interior1
# =====================================================================
W, H = 18, 16
g = new_grid(W, H, D_SAND)
for x in range(W):
    g[0][x] = D_ROCK if not (8 <= x <= 9) else D_SAND        # north door -> Interior1
    g[H-1][x] = D_ROCK if not (8 <= x <= 9) else D_SAND      # south -> GiltClawTerritory
for y in range(H):
    g[y][0] = D_ROCK
    g[y][W-1] = D_ROCK
# imposing gate pillars + ornamental fountains (water tiles, walkable around)
g[1][7] = D_ROCK
g[1][10] = D_ROCK
patch(g, 3, 7, 4, 8, D_WATER)          # left fountain
patch(g, 13, 7, 14, 8, D_WATER)        # right fountain
results['Sirocco_DaganPalace_Exterior'] = write_layout('Sirocco_DaganPalace_Exterior', g, BORDER_SAND)
reg_layout('LAYOUT_SIROCCO_DAGAN_PALACE_EXTERIOR', 'Sirocco_DaganPalace_Exterior', W, H, PRIM, SEC)

print(json.dumps({'sizes': results}, indent=1))

with open(os.path.join(os.path.dirname(__file__), 'sirocco_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)
