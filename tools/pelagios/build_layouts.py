#!/usr/bin/env python3
"""Compose Haven Isle layout blockdata (map.bin/border.bin) from stamps
sampled out of existing vanilla layouts (same tileset pairs).

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All words below are copied verbatim from vanilla maps, so collision and
elevation are inherently correct.
"""
import json, os, struct, sys

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
LITTLEROOT = load_blocks(L['LAYOUT_LITTLEROOT_TOWN'])
OLDALE = load_blocks(L['LAYOUT_OLDALE_TOWN'])
ROUTE116 = load_blocks(L['LAYOUT_ROUTE116'])
ALTERING = load_blocks(L['LAYOUT_ALTERING_CAVE'])
STEVENS = load_blocks(L['LAYOUT_GRANITE_CAVE_STEVENS_ROOM'])

def rect(src, x0, y0, x1, y1):
    """Inclusive rect copy."""
    return [row[x0:x1+1] for row in src[y0:y1+1]]

# Stamps: (blocks, door dx, door dy) — door coords relative to stamp origin
HOUSE_L = (rect(LITTLEROOT, 2, 4, 6, 8), 3, 4)    # Brendan-style house, 5x5
HOUSE_R = (rect(LITTLEROOT, 13, 17, 4, 8), 1, 4)  # placeholder, fixed below
HOUSE_R = (rect(LITTLEROOT, 13, 4, 17, 8), 1, 4)  # May-style house, 5x5
LAB     = (rect(LITTLEROOT, 3, 12, 9, 16), 4, 4)  # Birch lab, 7x5
HOUSE_O = (rect(OLDALE, 4, 4, 7, 7), 1, 3)        # Oldale house, 4x4
PC      = (rect(OLDALE, 5, 13, 8, 16), 1, 3)      # Pokemon Center, 4x4
MART    = (rect(OLDALE, 13, 3, 16, 6), 1, 3)      # Mart, 4x4
KNOLL   = (rect(ROUTE116, 43, 0, 51, 8), 4, 8)    # cliff knoll w/ cave mouth, 9x9

GRASS    = 0x3001
TALL     = 0x300D
FLOWER   = 0x3004
SAND     = 0x3124
SOIL     = 0x368D  # berry tree soil
MAT      = 0x3201  # doormat
SIGN     = 0x0403
WATER    = 0x1170
CLIFF_W  = 0x0470
CLIFF    = 0x0471
TREES    = [[0x05D4, 0x05D5], [0x05DC, 0x05DD]]
LEDGE_L, LEDGE_M, LEDGE_R = 0x34D5, 0x3487, 0x34D6

# water edges (land on given side)
W_N, W_S, W_W, W_E = 0x1189, 0x1193, 0x1190, 0x1192
W_NW, W_NE = 0x1188, 0x118A

CHARS = {'.': GRASS, ',': TALL, 'f': FLOWER, 's': SAND, 'b': SOIL,
         'g': MAT, 'S': SIGN, 'c': CLIFF, 'v': CLIFF_W}

def compose(design):
    rows = [r for r in design.strip('\n').split('\n')]
    h = len(rows)
    w = len(rows[0])
    assert all(len(r) == w for r in rows), [len(r) for r in rows]
    g = [[GRASS]*w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            ch = rows[y][x]
            if ch == '#':
                g[y][x] = TREES[y % 2][x % 2]
            elif ch == 'W':
                def land(yy, xx):
                    if yy < 0 or yy >= h or xx < 0 or xx >= w:
                        return False
                    return rows[yy][xx] != 'W'
                n, s, we, e = land(y-1, x), land(y+1, x), land(y, x-1), land(y, x+1)
                if n and we: g[y][x] = W_NW
                elif n and e: g[y][x] = W_NE
                elif n: g[y][x] = W_N
                elif s: g[y][x] = W_S
                elif we: g[y][x] = W_W
                elif e: g[y][x] = W_E
                else: g[y][x] = WATER
            elif ch == 'L':
                left = x == 0 or rows[y][x-1] != 'L'
                right = x == w-1 or rows[y][x+1] != 'L'
                g[y][x] = LEDGE_L if left else (LEDGE_R if right else LEDGE_M)
            elif ch in CHARS:
                g[y][x] = CHARS[ch]
            else:
                g[y][x] = GRASS
    return g

def stamp(g, st, x0, y0, name=None, manifest=None, mapname=None):
    blocks, dx, dy = st
    for y, row in enumerate(blocks):
        for x, wval in enumerate(row):
            g[y0+y][x0+x] = wval
    if name and manifest is not None:
        manifest.setdefault(mapname, {})[name] = {'door': [x0+dx, y0+dy]}
    return x0+dx, y0+dy

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

BORDER_TREES = [0x01D4, 0x01D5, 0x01DC, 0x01DD]
BORDER_WATER = [0x0170, 0x0170, 0x0170, 0x0170]
with open(os.path.join(ROOT, 'data/layouts/AlteringCave/border.bin'), 'rb') as f:
    BORDER_CAVE = list(struct.unpack('<4H', f.read(8)))

manifest = {}
results = {}

# ---------------- Tidemark Village (24x22) ----------------
W, H = 24, 22
rows = []
for y in range(H):
    row = []
    for x in range(W):
        ch = '.'
        if y <= 1: ch = '#' if not (10 <= x <= 13) else '.'
        elif y >= 20: ch = '#' if not (10 <= x <= 13) else '.'
        elif x <= 1: ch = '#' if not (9 <= y <= 12) else '.'
        elif x >= 22: ch = '#' if not (9 <= y <= 12) else '.'
        row.append(ch)
    rows.append(''.join(row))
# garden flowers
rows = [list(r) for r in rows]
for (x, y) in [(3,10),(4,10),(5,10),(3,11),(4,11),(5,11),(3,12),(4,12),(5,12),(6,11)]:
    rows[y][x] = 'f'
for (x, y) in [(15,17),(16,17),(15,18),(16,18),(3,17),(4,17)]:
    rows[y][x] = 'f'
village = compose('\n'.join(''.join(r) for r in rows))
stamp(village, HOUSE_L, 2, 3, 'player_house', manifest, 'TidemarkVillage')
stamp(village, HOUSE_R, 8, 3, 'cass_house', manifest, 'TidemarkVillage')
stamp(village, LAB, 15, 3, 'lab', manifest, 'TidemarkVillage')
stamp(village, PC, 7, 11, 'pokecenter', manifest, 'TidemarkVillage')
stamp(village, MART, 13, 11, 'mart', manifest, 'TidemarkVillage')
stamp(village, HOUSE_O, 17, 15, 'elder_house', manifest, 'TidemarkVillage')
# doormats under doors + signs
for m in manifest['TidemarkVillage'].values():
    x, y = m['door']
    village[y+1][x] = MAT
village[9][9] = SIGN      # village sign
village[8][17] = SIGN     # lab sign
results['HavenIsle_TidemarkVillage'] = write_layout('HavenIsle_TidemarkVillage', village, BORDER_TREES)

# ---------------- Harbor (20x16) ----------------
W, H = 20, 16
rows = []
for y in range(H):
    row = []
    for x in range(W):
        if y <= 2: ch = '#' if not (8 <= x <= 11) else '.'
        elif y <= 9 and x <= 1: ch = '#'
        elif y <= 9 and x >= 18: ch = '#'
        elif 8 <= y <= 9: ch = 's'
        elif y >= 10: ch = 'W'
        else: ch = '.'
        row.append(ch)
    rows.append(''.join(row))
harbor = compose('\n'.join(rows))
harbor[4][7] = SIGN   # "Tidemark Village ahead"
harbor[8][12] = SIGN  # "The Tennyson" berth sign
results['HavenIsle_Harbor'] = write_layout('HavenIsle_Harbor', harbor, BORDER_WATER)

# ---------------- Coastal Route 1 (20x40) ----------------
W, H = 20, 40
rows = []
for y in range(H):
    row = []
    for x in range(W):
        if y <= 1 or y >= 38:
            ch = '#' if not (8 <= x <= 11) else '.'
        elif x <= 1 or x >= 18:
            ch = '#'
        elif x == 16 and 6 <= y <= 30: ch = 'v'  # cliff west edge
        elif x == 17 and 6 <= y <= 30: ch = 'c'  # cliff fill (east coastal cliffs)
        else: ch = '.'
        row.append(ch)
    rows.append(''.join(row))
rows = [list(r) for r in rows]
for y in range(8, 12):
    for x in range(3, 9): rows[y][x] = ','
for y in range(22, 26):
    for x in range(5, 13): rows[y][x] = ','
for y in range(30, 34):
    for x in range(3, 10): rows[y][x] = ','
for x in range(3, 12): rows[17][x] = 'L'   # ledge, jump south; corridor x12-15
rows[28][4] = 'b'; rows[28][5] = 'b'        # berry soil
route1 = compose('\n'.join(''.join(r) for r in rows))
route1[36][9] = SIGN
results['HavenIsle_Route1'] = write_layout('HavenIsle_Route1', route1, BORDER_TREES)

# ---------------- Ancient Ruins Exterior (18x14) ----------------
W, H = 18, 14
rows = []
for y in range(H):
    row = []
    for x in range(W):
        if y <= 1: ch = '#'
        elif y >= 12: ch = '#' if not (7 <= x <= 10) else '.'
        elif x <= 1 or x >= 16: ch = '#'
        else: ch = '.'
        row.append(ch)
    rows.append(''.join(row))
ruinsext = compose('\n'.join(rows))
cx, cy = stamp(ruinsext, KNOLL, 4, 2)
manifest['RuinsExterior'] = {'cave_mouth': {'door': [cx, cy]}}
ruinsext[11][10] = SIGN
results['HavenIsle_AncientRuinsExterior'] = write_layout('HavenIsle_AncientRuinsExterior', ruinsext, BORDER_TREES)

# ---------------- Coastal Route 2 (32x15) ----------------
W, H = 32, 15
rows = []
for y in range(H):
    row = []
    for x in range(W):
        if y <= 2: ch = '#'
        elif x <= 1 and y <= 10: ch = '#' if not (6 <= y <= 9) else '.'
        elif x >= 30 and y <= 10: ch = '#' if not (6 <= y <= 9) else '.'
        elif 9 <= y <= 10: ch = 's'
        elif y >= 11: ch = 'W'
        else: ch = '.'
        row.append(ch)
    rows.append(''.join(row))
rows = [list(r) for r in rows]
for y in range(3, 6):
    for x in range(4, 11): rows[y][x] = ','
    for x in range(18, 25): rows[y][x] = ','
route2 = compose('\n'.join(''.join(r) for r in rows))
route2[5][3] = SIGN
results['HavenIsle_Route2'] = write_layout('HavenIsle_Route2', route2, BORDER_WATER)

# ---------------- Fishing Docks (14x12) ----------------
W, H = 14, 12
rows = []
for y in range(H):
    row = []
    for x in range(W):
        if y <= 1: ch = '#'
        elif x <= 1 and y <= 7: ch = '#' if not (3 <= y <= 6) else '.'
        elif x >= 12 and y <= 7: ch = '#'
        elif 6 <= y <= 7: ch = 's'
        elif y >= 8: ch = 'W'
        else: ch = '.'
        row.append(ch)
    rows.append(''.join(row))
docks = compose('\n'.join(rows))
docks[5][10] = SIGN
results['HavenIsle_FishingDocks'] = write_layout('HavenIsle_FishingDocks', docks, BORDER_WATER)

# ---------------- Ancient Ruins Interior 1 (Altering Cave copy + ladder) ----------------
int1 = [row[:] for row in ALTERING]
ladder = None
for y in range(3, 10):
    for x in range(4, 14):
        if int1[y][x] == 0x3211 and int1[y-1][x] != 0x3211 and ladder is None:
            ladder = (x, y)
if ladder is None:
    for y in range(3, 12):
        for x in range(3, 16):
            if int1[y][x] == 0x3211:
                ladder = (x, y); break
        if ladder: break
int1[ladder[1]][ladder[0]] = 0x323F
manifest['RuinsInterior1'] = {'entrance': {'door': [18, 22]}, 'ladder': {'door': list(ladder)}}
# centre glow tile: pick a floor tile near the middle
glow = None
for y in range(10, 16):
    for x in range(13, 20):
        if int1[y][x] == 0x3211:
            glow = (x, y); break
    if glow: break
manifest['RuinsInterior1']['glow'] = {'door': list(glow)}
results['HavenIsle_AncientRuinsInterior1'] = write_layout('HavenIsle_AncientRuinsInterior1', int1, BORDER_CAVE)

# ---------------- Ancient Ruins Interior 2 (Steven's room copy) ----------------
int2 = [row[:] for row in STEVENS]
manifest['RuinsInterior2'] = {'entrance': {'door': [7, 3]}}
# find a floor tile adjacent to north wall for the inscription bg event
insc = None
for x in range(4, 12):
    for y in range(2, 8):
        if int2[y][x] == 0x3201 and int2[y-1][x] not in (0x3201, 0x3214):
            insc = (x, y-1); break
    if insc: break
manifest['RuinsInterior2']['inscription_wall'] = {'door': list(insc)}
results['HavenIsle_AncientRuinsInterior2'] = write_layout('HavenIsle_AncientRuinsInterior2', int2, BORDER_CAVE)

print(json.dumps({'sizes': results, 'manifest': manifest}, indent=1))
