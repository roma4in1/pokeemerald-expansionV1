#!/usr/bin/env python3
"""Compose Gildhaven Isle layout blockdata (map.bin/border.bin).

Mirrors tools/pelagios/build_schism.py / build_emberveil.py.

Gildhaven is an ENTIRELY URBAN merchant island — glittering surface, rotten
underneath. No custom Pelagios tileset is used: every OUTDOOR map pairs
gTileset_General (primary) with gTileset_Lilycove (secondary). Lilycove was
chosen over Slateport for the wealthy/glittering "big city" aesthetic: it has
ornate fenced ground, fancy paving (0x3124), and richer building fronts that
read as a gold-trimmed trading port. (Slateport's market-stall tiles are nicer
in isolation but its overall vibe is a working port, not a wealthy one.)

Outdoor metatile words are sampled VERBATIM from LAYOUT_LILYCOVE_CITY so the
collision (bits 10-11) and elevation (bits 12-15) bits come along for free:
  0x3124  Lilycove fancy paving, walkable (col0 elev3) — main urban ground
  0x3001  General green ground, walkable (col0 elev3) — garden/grass accents
  0x0475  General fence/wall block (col1 elev0) — building walls / borders
  0x0470  General wall block (col1 elev0)
  0x0473  General wall block (col1 elev0)
  0x0403  General sign post
  0x1170  General deep water (sea), blocked by MB behavior
  0x1189  shore edge, land-to-north
  0x1190  shore edge variant
INTERIORS reuse vanilla LAYOUT_* wholesale (zero new binary) in
build_gildhaven_mapjson.py — see that file's header.

This script emits ONLY the composed outdoor layouts:
  GoldportHarbor, MerchantDistrict, NobleQuarter, TheExchange_Exterior.
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

# --- sampled Lilycove/General words (collision+elevation baked in) ---
GROUND = 0x3124   # fancy paving, walkable
GRASS  = 0x3001   # green ground accents, walkable
WALL   = 0x0475   # fence/wall block (solid)
WALL2  = 0x0470
WALL3  = 0x0473
WATER  = 0x1170   # deep sea (blocked by behavior)
SHORE  = 0x1189   # shore edge land-to-north
SHORE2 = 0x1190

BORDER = [WALL] * 4

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

def reg_layout(layout_id, name, w, h):
    layout_entries.append({
        'id': layout_id,
        'name': name + '_Layout',
        'width': w, 'height': h,
        'primary_tileset': 'gTileset_General',
        'secondary_tileset': 'gTileset_Lilycove',
        'border_filepath': f'data/layouts/{name}/border.bin',
        'blockdata_filepath': f'data/layouts/{name}/map.bin',
        'layout_version': 'emerald',
    })

def ring(g, w, h, north_gap=(), south_gap=(), west_gap=(), east_gap=(), wall=WALL):
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
#  GoldportHarbor (20x18) — entry port, sea to the south, north -> MerchantDistrict
# =====================================================================
W, H = 20, 18
g = new_grid(W, H, GROUND)
ring(g, W, H, north_gap=(9, 10), wall=WALL)
patch(g, 1, 13, W-2, H-1, WATER)          # southern sea (Covenant ships visible)
# central stone dock down into the water for the Tennyson (Galleon)
for y in range(12, 16):
    g[y][9] = GROUND
    g[y][10] = GROUND
# gold-trimmed checkpoint pillars flanking the north exit
g[1][8] = WALL
g[1][11] = WALL
results['Gildhaven_GoldportHarbor'] = write_layout('Gildhaven_GoldportHarbor', g, BORDER)
reg_layout('LAYOUT_GILDHAVEN_GOLDPORT_HARBOR', 'Gildhaven_GoldportHarbor', W, H)

# =====================================================================
#  MerchantDistrict (28x24) — main city hub
#  down -> GoldportHarbor, warps to BlackMarket alley / NobleQuarter / PC / Gyms
# =====================================================================
W, H = 28, 24
g = new_grid(W, H, GROUND)
ring(g, W, H, south_gap=(13, 14), wall=WALL)
# south gate pillars (to the harbor)
g[H-2][12] = WALL
g[H-2][15] = WALL
# market-stall blocks (flavor walls; gym/center/black-market doors sit beside them)
patch(g, 4, 5, 6, 6, WALL)      # silk stall (BlackMarket alley is behind it)
patch(g, 20, 5, 22, 6, WALL)    # produce stall
patch(g, 11, 9, 16, 10, WALL)   # Glint's converted-stall gym front
patch(g, 4, 14, 6, 15, WALL)    # Shade's hidden-alley gym front
# garden accents
patch(g, 22, 16, 25, 19, GRASS)
results['Gildhaven_MerchantDistrict'] = write_layout('Gildhaven_MerchantDistrict', g, BORDER)
reg_layout('LAYOUT_GILDHAVEN_MERCHANT_DISTRICT', 'Gildhaven_MerchantDistrict', W, H)

# =====================================================================
#  NobleQuarter (24x20) — wealthy residential, Vane manor visible
#  down -> MerchantDistrict, warp to VaneManor, garden gym 3 (Lace)
# =====================================================================
W, H = 24, 20
g = new_grid(W, H, GROUND)
ring(g, W, H, south_gap=(11, 12), wall=WALL)
g[H-2][10] = WALL
g[H-2][13] = WALL
# manor facade block (north) — VaneManor door sits at its base
patch(g, 8, 2, 15, 4, WALL)
# manicured garden hedges (the garden arena is the open patch east of them)
patch(g, 16, 6, 21, 12, GRASS)
# hedge dividers
patch(g, 5, 8, 5, 14, WALL)
patch(g, 18, 14, 18, 16, WALL)
results['Gildhaven_NobleQuarter'] = write_layout('Gildhaven_NobleQuarter', g, BORDER)
reg_layout('LAYOUT_GILDHAVEN_NOBLE_QUARTER', 'Gildhaven_NobleQuarter', W, H)

# =====================================================================
#  TheExchange_Exterior (20x16) — imposing financial HQ frontage
#  Reached from VaneManor_Interior (hidden passage warp). Two elite-guard
#  battles at the entrance. Warp -> Interior1.
# =====================================================================
W, H = 20, 16
g = new_grid(W, H, GROUND)
ring(g, W, H, wall=WALL)
# imposing building facade across the north (door gap at cols 9-10)
patch(g, 3, 1, 16, 3, WALL)
g[3][9] = GROUND
g[3][10] = GROUND
patch(g, 4, 1, 15, 2, WALL)
# grand approach plaza (paving already GROUND); flank planters
patch(g, 2, 6, 3, 9, GRASS)
patch(g, 16, 6, 17, 9, GRASS)
# entry vestibule walls funnelling to the door
g[2][8] = WALL
g[2][11] = WALL
results['Gildhaven_TheExchange_Exterior'] = write_layout('Gildhaven_TheExchange_Exterior', g, BORDER)
reg_layout('LAYOUT_GILDHAVEN_THE_EXCHANGE_EXTERIOR', 'Gildhaven_TheExchange_Exterior', W, H)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'gildhaven_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)

# --- merge into layouts.json idempotently (by id) ---
with open(LAYOUTS_JSON) as f:
    doc = json.load(f)
existing = {l.get('id'): i for i, l in enumerate(doc['layouts']) if l}
for e in layout_entries:
    if e['id'] in existing:
        doc['layouts'][existing[e['id']]] = e
    else:
        doc['layouts'].append(e)
with open(LAYOUTS_JSON, 'w') as f:
    json.dump(doc, f, indent=2)
    f.write('\n')
print(f'merged {len(layout_entries)} Gildhaven layout entries into layouts.json')
