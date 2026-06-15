#!/usr/bin/env python3
"""Compose Ironhold layout blockdata (map.bin/border.bin) from stamps sampled
out of existing vanilla layouts, then generate all 17 Ironhold map.json files.

Mirrors tools/pelagios/build_layouts.py + build_mapjson.py (Haven Isle).

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All metatile words are copied verbatim from vanilla layouts (same tileset pair),
so collision and elevation are inherently correct.

Outdoor maps (port/city/route/fortress) get NEW composed layouts using the
General/Slateport tileset pair (stone, industrial, port aesthetic).
Cave/route-rock maps use General/Cave. Interiors reuse vanilla LAYOUT_* wholesale.
"""
import json, os, struct

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
SLATEPORT = load_blocks(L['LAYOUT_SLATEPORT_CITY'])
VROAD     = load_blocks(L['LAYOUT_VICTORY_ROAD_1F'])

# ---- Slateport (General/Slateport) metatile palette -------------------------
# Verified from a dump of LAYOUT_SLATEPORT_CITY.
S_GROUND = 0x3001     # plaza ground, walkable, elevation 3
S_WALL   = 0x04C6     # solid stone/paving wall block (collision)
S_WALL2  = 0x04C7     # solid stone wall variant
S_SIGN   = 0x0403     # sign metatile
S_WATER  = 0x1170     # deep water
# water edges (land on the named side) sampled from Slateport column 0/right edge
S_W_N    = 0x1189     # water, land to north
S_W_S    = 0x1179     # water, land to south
S_W_W    = 0x1190     # water, land to west
S_W_E    = 0x1192     # water, land to east

# ---- Cave (General/Cave) metatile palette, sampled from Victory Road --------
def vr(y, x):
    return VROAD[y][x]
C_FLOOR = None
C_WALL  = None
# find a common floor tile (collision 0) and wall tile (collision 1) in VRoad
from collections import Counter
floor_c, wall_c = Counter(), Counter()
for row in VROAD:
    for wv in row:
        col = (wv >> 10) & 3
        if col == 0:
            floor_c[wv] += 1
        else:
            wall_c[wv] += 1
C_FLOOR = floor_c.most_common(1)[0][0]
C_WALL  = wall_c.most_common(1)[0][0]

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

BORDER_STONE = [S_WALL, S_WALL, S_WALL, S_WALL]
BORDER_WATER = [S_WATER, S_WATER, S_WATER, S_WATER]
BORDER_CAVE  = [C_WALL, C_WALL, C_WALL, C_WALL]

results = {}
layout_entries = []  # (id, name, w, h, prim, sec)

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

# Helper to lay a solid stone-walled rectangular room with a gap opening list.
# openings: dict side->list of coords kept walkable on the border.
def walled(w, h, north_gap=(), south_gap=(), west_gap=(), east_gap=(), wall=S_WALL, floor=S_GROUND):
    g = new_grid(w, h, floor)
    for x in range(w):
        if x not in north_gap: g[0][x] = wall
        if x not in south_gap: g[h-1][x] = wall
    for y in range(h):
        if y not in west_gap: g[y][0] = wall
        if y not in east_gap: g[y][w-1] = wall
    return g

# =====================================================================
#  BUILDING STAMPS (verbatim Slateport blockdata blocks) ---------------
# =====================================================================
# Copied as full metatile WORDS (id|collision|elevation) straight out of
# LAYOUT_SLATEPORT_CITY, so every tile is guaranteed valid for the
# General/Slateport pair AND internally tile-consistent. The DOOR tile is
# the bottom-centre walkable column; stamp so that door lands on a warp.
# Building A (5 wide x 6 tall) -- has a door (rows 3-5, centre col walkable):
BLD_A = [
    [0x3270, 0x3271, 0x3271, 0x3271, 0x3272],
    [0x3278, 0x3279, 0x3279, 0x3279, 0x327a],
    [0x0663, 0x3001, 0x3001, 0x3001, 0x0664],
    [0x3001, 0x3208, 0x3209, 0x320a, 0x3001],
    [0x3001, 0x3210, 0x3211, 0x3212, 0x3001],
    [0x3001, 0x3210, 0x3211, 0x3212, 0x3001],
]
BLD_A_DOOR = (5, 2)   # (row, col) of the door tile within BLD_A (bottom-centre)

# Building B (7 wide x 5 tall) -- bigger roofed hall, NO door (decorative
# facade; place where no warp is needed, e.g. flanking the plaza):
BLD_B = [
    [0x0549, 0x0771, 0x0772, 0x0773, 0x0774, 0x0775, 0x0549],
    [0x3001, 0x0779, 0x077a, 0x077b, 0x077c, 0x077d, 0x3001],
    [0x3643, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x3643],
    [0x3643, 0x0789, 0x078a, 0x078b, 0x078c, 0x078d, 0x3643],
    [0x3643, 0x0791, 0x0792, 0x0793, 0x0794, 0x0795, 0x3643],
]

def stamp(grid, blk, top, left):
    """Paint blk into grid at (top,left). Bounds-checked; skips if OOB."""
    h, w = len(grid), len(grid[0])
    bh, bw = len(blk), len(blk[0])
    if top < 0 or left < 0 or top + bh > h or left + bw > w:
        raise ValueError(f'building stamp {bh}x{bw} at ({top},{left}) is OOB for {h}x{w}')
    for dy in range(bh):
        for dx in range(bw):
            grid[top + dy][left + dx] = blk[dy][dx]

def stamp_door_on_warp(grid, warp_x, warp_y):
    """Place BLD_A so its door tile lands exactly on (warp_x, warp_y); the
    building rises NORTH of the warp so the door is enterable from the south."""
    dr, dc = BLD_A_DOOR
    top = warp_y - dr
    left = warp_x - dc
    stamp(grid, BLD_A, top, left)

# =====================================================================
#  OUTDOOR LAYOUTS (General/Slateport)  -- new composed blockdata
# =====================================================================

# ---------------- GatemarkPort (20x18) ----------------
# Sea to the south (Tennyson docks). North exit -> OuterDistrict.
W, H = 20, 18
g = new_grid(W, H, S_GROUND)
# stone wall ring, north gap (exit to OuterDistrict) at x=9..10
for x in range(W):
    g[0][x] = S_WALL if not (9 <= x <= 10) else S_GROUND
for y in range(H):
    g[y][0] = S_WALL
    g[y][W-1] = S_WALL
# southern sea: rows 13..17 water, with a stone dock jutting from row 12
for y in range(13, H):
    for x in range(1, W-1):
        g[y][x] = S_WATER
# water edge row 12 -> land to north
for x in range(1, W-1):
    g[12][x] = S_W_N
# dock planks (walkable ground) down the centre into the water for the Tennyson
for y in range(12, 16):
    g[y][9] = S_GROUND
    g[y][10] = S_GROUND
g[16][9] = S_W_N  # dock head meets water
g[16][10] = S_W_N
# checkpoint gate: stone pillars flanking the north exit
g[1][8] = S_WALL
g[1][11] = S_WALL
# Inn building: door aligned to the inn warp at (4,8) so the harbor reads as a
# port building, not a warp tile on bare stone. Rises north of the warp.
stamp_door_on_warp(g, 4, 8)
# signs
g[2][12] = S_SIGN   # "Ironhold City - Covenant Garrison HQ"
g[11][6] = S_SIGN   # berth sign
results['Ironhold_GatemarkPort'] = write_layout('Ironhold_GatemarkPort', g, BORDER_WATER)
reg_layout('LAYOUT_IRONHOLD_GATEMARK_PORT', 'Ironhold_GatemarkPort', W, H,
           'gTileset_General', 'gTileset_Slateport')

# ---------------- OuterDistrict (24x20) ----------------
# south -> GatemarkPort, north -> IronholdCity, west -> MountainPass (rubble gate)
W, H = 24, 20
g = new_grid(W, H, S_GROUND)
for x in range(W):
    g[0][x] = S_WALL if not (11 <= x <= 12) else S_GROUND      # north exit -> City
    g[H-1][x] = S_WALL if not (11 <= x <= 12) else S_GROUND    # south exit -> Port
for y in range(H):
    g[y][0] = S_WALL if not (9 <= y <= 10) else S_GROUND       # west exit -> MountainPass
    g[y][W-1] = S_WALL
# scrubland grass patches at the edges (wild encounters) - use tall grass tile
S_TALL = 0x300D  # METATILE_General_TallGrass (0x00D) | elevation 3 -> wild encounters
for (x0, y0, x1, y1) in [(2, 3, 6, 6), (17, 13, 21, 17)]:
    for y in range(y0, y1+1):
        for x in range(x0, x1+1):
            g[y][x] = S_TALL
# graffiti wall section (a stone wall block to examine) near centre-left
g[8][3] = S_WALL  # resistance graffiti (bg event points here)
# rubble blocking the west exit until Grapple Hook - stone blocks in the gap
g[9][1] = S_WALL
g[10][1] = S_WALL
# decorative building facade (door-less) so the district reads as a built-up
# quarter, not bare ground. Placed clear of the grass patches (2..6/17..21),
# graffiti wall (3,8), NPCs (9,8)/(16,11), exits and rubble.
stamp(g, BLD_B, 2, 9)     # central-north hall block (cols 9..15, rows 2..6)
results['Ironhold_OuterDistrict'] = write_layout('Ironhold_OuterDistrict', g, BORDER_STONE)
reg_layout('LAYOUT_IRONHOLD_OUTER_DISTRICT', 'Ironhold_OuterDistrict', W, H,
           'gTileset_General', 'gTileset_Slateport')

# ---------------- IronholdCity (28x24) ----------------
# south -> OuterDistrict, north -> SummitApproach (blocked until Gym3)
W, H = 28, 24
g = new_grid(W, H, S_GROUND)
for x in range(W):
    g[0][x] = S_WALL if not (13 <= x <= 14) else S_GROUND     # north exit -> SummitApproach
    g[H-1][x] = S_WALL if not (13 <= x <= 14) else S_GROUND   # south exit -> OuterDistrict
for y in range(H):
    g[y][0] = S_WALL
    g[y][W-1] = S_WALL
# iron-gate pillars at north exit
g[1][12] = S_WALL
g[1][15] = S_WALL
# building doormat ground left walkable; buildings are warp tiles (no stamp needed,
# the warp coords below define entrances). Place signs near key buildings.
# --- building dressing -------------------------------------------------
# Stamp recognizable Slateport buildings so the city reads as a town, not bare
# ground. Door tiles align to the existing warp coords (mapjson warps are at
# (6,11),(20,11),(23,11),(20,17)); each building rises NORTH of its warp so the
# door is enterable from the south. Buildings (20,11) and (23,11) are only 3
# tiles apart while BLD_A is 5 wide, so (23,11) gets a door-only patch instead of
# a full overlapping stamp. All metatile words are verbatim-valid Slateport.
stamp_door_on_warp(g, 6, 11)    # north-west building (Gym / town hall)
stamp_door_on_warp(g, 20, 11)   # east building (PokeCenter)
stamp_door_on_warp(g, 20, 17)   # south-east building (Sever HQ)
# (23,11): just give it a doorway facade (avoid overlapping the (20,11) stamp)
g[10][23] = 0x0664              # door-frame post
g[9][23]  = 0x3279             # wall above the door (Mart annex)
g[8][23]  = 0x3271             # roof
# a couple of decorative (door-less) hall facades in the open plaza
stamp(g, BLD_B, 18, 2)         # west plaza hall
stamp(g, BLD_B, 3, 19)         # north-east plaza hall
# sign metatiles sit on plaza ground LEFT of each building door (match mapjson)
g[4][6]   = S_SIGN   # Gym sign (Petra / town hall, north end - no building here)
g[11][3]  = S_SIGN   # Armory sign  (left of building @ warp 6,11)
g[11][17] = S_SIGN   # PokeCenter sign (left of building @ warp 20,11)
g[17][17] = S_SIGN   # Sever HQ sign (left of building @ warp 20,17)
results['Ironhold_IronholdCity'] = write_layout('Ironhold_IronholdCity', g, BORDER_STONE)
reg_layout('LAYOUT_IRONHOLD_IRONHOLD_CITY', 'Ironhold_IronholdCity', W, H,
           'gTileset_General', 'gTileset_Slateport')

# ---------------- MountainPass (20x34) (General/Cave rocky route) ----------------
# east -> IronholdCity, north -> SummitApproach, contains cave warp midway
W, H = 20, 34
g = new_grid(W, H, C_FLOOR)
# cave walls ring
for x in range(W):
    g[0][x] = C_WALL if not (8 <= x <= 9) else C_FLOOR       # north exit -> SummitApproach
    g[H-1][x] = C_WALL
for y in range(H):
    g[y][0] = C_WALL
    g[y][W-1] = C_WALL if not (15 <= y <= 16) else C_FLOOR   # east exit -> IronholdCity
# scatter wall pillars to create a winding rocky path
for (x, y) in [(4,5),(5,5),(6,5),(13,9),(14,9),(15,9),(4,14),(5,14),(6,14),(7,14),
               (12,20),(13,20),(14,20),(8,26),(9,26),(10,26)]:
    g[y][x] = C_WALL
# entrance rubble (Grapple Hook gate) just inside east entry
g[15][18] = C_WALL
g[16][18] = C_WALL
results['Ironhold_MountainPass'] = write_layout('Ironhold_MountainPass', g, BORDER_CAVE)
reg_layout('LAYOUT_IRONHOLD_MOUNTAIN_PASS', 'Ironhold_MountainPass', W, H,
           'gTileset_General', 'gTileset_Cave')

# ---------------- SummitApproach (22x20) (General/Cave rocky vista) ----------------
# south -> MountainPass, north -> SummitFortress_Exterior
W, H = 22, 20
g = new_grid(W, H, C_FLOOR)
for x in range(W):
    g[0][x] = C_WALL if not (10 <= x <= 11) else C_FLOOR     # north exit
    g[H-1][x] = C_WALL if not (10 <= x <= 11) else C_FLOOR   # south exit
for y in range(H):
    g[y][0] = C_WALL
    g[y][W-1] = C_WALL
for (x, y) in [(5,6),(6,6),(15,6),(16,6),(5,13),(6,13),(15,13),(16,13)]:
    g[y][x] = C_WALL
results['Ironhold_SummitApproach'] = write_layout('Ironhold_SummitApproach', g, BORDER_CAVE)
reg_layout('LAYOUT_IRONHOLD_SUMMIT_APPROACH', 'Ironhold_SummitApproach', W, H,
           'gTileset_General', 'gTileset_Cave')

# ---------------- SummitFortress_Exterior (18x16) (stone gate) ----------------
# south -> SummitApproach, north -> Interior1
W, H = 18, 16
g = new_grid(W, H, S_GROUND)
for x in range(W):
    g[0][x] = S_WALL if not (8 <= x <= 9) else S_GROUND      # north exit -> Interior1 (fortress door)
    g[H-1][x] = S_WALL if not (8 <= x <= 9) else S_GROUND    # south exit -> SummitApproach
for y in range(H):
    g[y][0] = S_WALL
    g[y][W-1] = S_WALL
# imposing gate pillars
g[1][7] = S_WALL
g[1][10] = S_WALL
results['Ironhold_SummitFortress_Exterior'] = write_layout('Ironhold_SummitFortress_Exterior', g, BORDER_STONE)
reg_layout('LAYOUT_IRONHOLD_SUMMIT_FORTRESS_EXTERIOR', 'Ironhold_SummitFortress_Exterior', W, H,
           'gTileset_General', 'gTileset_Slateport')

print(json.dumps({'sizes': results,
                  'cave_floor': hex(C_FLOOR), 'cave_wall': hex(C_WALL)}, indent=1))

# Persist the new layout entries so the mapjson step / layouts.json patch can use them.
with open(os.path.join(os.path.dirname(__file__), 'ironhold_layout_entries.json'), 'w') as f:
    json.dump(layout_entries, f, indent=2)
