#!/usr/bin/env python3
"""Compose Primalis Isle layout blockdata (map.bin/border.bin) + merge layout
entries into data/layouts/layouts.json.

Mirrors tools/pelagios/build_thalvern.py / build_gildhaven.py.

Primalis is the BEAST ISLAND: the oldest island in Pelagios, an ancient jungle
that grew around the sealed legendary Verdath (Grass/Dragon) over millennia.
A Zoan guardian community lives here. Dense fog, primal growth.

Tilesets:
  Jungle outdoor maps (JungleRoute1, JungleInterior, JungleRoute2, TheHeartwood,
    VerdantLanding, ZoanVillage, AncientRuinsCamp):
      gTileset_General (primary) + gTileset_Fortree (secondary)
      Fortree is the vanilla treehouse/jungle-canopy tileset (Fortree City,
      Route 119/120) - the closest match to a dense overgrown jungle. General
      provides grass/tall-grass/tree metatiles; Fortree provides canopy/treehouse
      detail and richer foliage for hand-tuning later.
  Interiors (Inn, PokemonCenter, ElderHall, SealChamber):
      reuse vanilla LAYOUT_* wholesale in build_primalis_mapjson.py (zero new
      binary). The seal room reuses LAYOUT_SEALED_CHAMBER_INNER_ROOM.

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
We use General-primary metatiles (id < 512) with collision/elevation baked into
the layout word, exactly as the other Pelagios outdoor builders do. The Fortree
secondary is paired for canopy/treehouse hand-tuning in Porymap later.

Metatiles used (all gTileset_General primary, ids < 512):
  GRASS      0x001  walkable jungle ground
  TALLGRASS  0x00D  wild-encounter grass
  LONGGRASS  0x015  taller decorative grass
  TREE       0x00E  tree (Grass_TreeUp) - forced col 1 = jungle wall
  OVERGROWTH 0x015  LongGrass forced col 1 = the Beast-Whistle obstacle (dense
                    undergrowth that visually reads as passable-looking growth
                    but blocks until the Beast Whistle clears it). Documented so
                    the script-writer's coord triggers + field effect line up.

TheHeartwood is the VAST ancient sanctuary - built oversized (28x34) as a grand
overgrown root-network hall with a central walkable clearing (the seal/altar
centerpiece) ringed by impassable ancient growth, a long processional approach,
and flanking root walls. Monumental scale. The actual seal apparatus +
discovery triggers live on Heartwood_SealChamber (SEALED_CHAMBER_INNER_ROOM
reuse) per the established SealChamber pattern.
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

def mt(metatile_id, collision=0, elevation=3):
    return (metatile_id & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

# --- jungle terrain palette (General primary metatiles) ---
GRASS      = mt(0x001, 0, 3)   # walkable jungle floor
TALLGRASS  = mt(0x00D, 0, 3)   # wild-encounter grass (walkable, spawns mons)
LONGGRASS  = mt(0x015, 0, 3)   # decorative taller grass (walkable)
TREE       = mt(0x00E, 1, 0)   # jungle tree / canopy wall (impassable)
OVERGROW   = mt(0x015, 1, 0)   # BEAST-WHISTLE obstacle: dense undergrowth (impassable)
BORDER_J   = [TREE] * 4

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

JG = ('gTileset_General', 'gTileset_Fortree')

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
#  VerdantLanding (20x18) — Galleon arrival, jungle-edge landing
# =====================================================================
# South sea-edge landing dock for the Galleon. North -> JungleRoute1.
# Inn warp on the map. Arrival tile (10,14) on the dock (matches other ports).
W, H = 20, 18
g = new_grid(W, H, GRASS)
ring(g, W, H, north_gap=(9, 10), wall=TREE)
# southern sea edge (impassable water -> use TREE-as-wall row so dock juts in;
# the brief keeps this a jungle landing, not open sea, so a thin growth band)
patch(g, 1, 16, W-2, H-1, TREE)
# central wooden dock down to the water for the Tennyson (Galleon)
for y in range(13, 17):
    g[y][9] = GRASS
    g[y][10] = GRASS
# a few flanking trees framing the clearing
for (x, y) in [(3, 4), (16, 4), (4, 11), (15, 11)]:
    g[y][x] = TREE
results['Primalis_VerdantLanding'] = write_layout('Primalis_VerdantLanding', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_VERDANT_LANDING', 'Primalis_VerdantLanding', W, H, *JG)

# =====================================================================
#  JungleRoute1 (20x32) — dense jungle, BEAST-WHISTLE GATE
# =====================================================================
# south <- VerdantLanding, north -> JungleInterior. Tall-grass encounter
# patches. A band of dense undergrowth (OVERGROW, impassable) gates the north
# half until the Beast Whistle clears it. Coord triggers on the tiles just
# SOUTH of the band (script-writer wires VAR_PRIMALIS_PROGRESS + field effect).
W, H = 20, 32
g = new_grid(W, H, GRASS)
ring(g, W, H, north_gap=(9, 10), south_gap=(9, 10), wall=TREE)
# tall-grass encounter patches (south half, before the gate)
patch(g, 3, 22, 7, 27, TALLGRASS)
patch(g, 12, 23, 16, 28, TALLGRASS)
# decorative growth
patch(g, 2, 5, 4, 7, LONGGRASS)
patch(g, 15, 8, 17, 10, LONGGRASS)
# BEAST-WHISTLE OBSTACLE (OPTIONAL side nook). The Beast Whistle is obtained
# POST-GYM4, so the MAIN north-south path must stay fully open here (this route is
# pre-Gym1). The undergrowth instead seals an OPTIONAL reward nook in the NE corner
# that the player returns for once they have the Whistle. Coord triggers in front of
# the seal are TODO (script-writer wires the field-effect clear).
patch(g, 16, 3, 18, 5, TALLGRASS)   # nook interior (encounter grass, the reward)
vline(g, 15, 3, 6, TREE)            # west wall of the nook
hline(g, 16, 18, 2, TREE)           # north wall (col 19 = ring is the east wall)
hline(g, 16, 18, 6, OVERGROW)       # south seal - Beast Whistle clears THIS row
# light route character (NEVER block the central spine x 9-10)
for (x, y) in [(5, 12), (6, 19), (13, 19)]:
    g[y][x] = TREE
results['Primalis_JungleRoute1'] = write_layout('Primalis_JungleRoute1', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_JUNGLE_ROUTE_1', 'Primalis_JungleRoute1', W, H, *JG)

# =====================================================================
#  JungleInterior (26x24) — deep jungle hub with a river
# =====================================================================
# south <- JungleRoute1, up -> ZoanVillage, right -> JungleRoute2.
# A river runs through (rare Dratini "river only" encounters live in the water).
# Encounter grass on the banks.
W, H = 26, 24
g = new_grid(W, H, GRASS)
ring(g, W, H, south_gap=(12, 13), north_gap=(12, 13), east_gap=(11, 12), wall=TREE)
# the river (impassable water band, here a TREE-as-deep-growth channel; the
# wild "river only" table is attached in map.json - Dratini in the water)
patch(g, 3, 10, 22, 11, TREE)
# stepping log crossing so the river is passable at the centre
g[10][12] = GRASS; g[10][13] = GRASS
g[11][12] = GRASS; g[11][13] = GRASS
# tall-grass encounter patches on both banks
patch(g, 4, 4, 8, 7, TALLGRASS)
patch(g, 16, 5, 21, 8, TALLGRASS)
patch(g, 5, 15, 9, 19, TALLGRASS)
patch(g, 16, 15, 21, 19, TALLGRASS)
# decorative canopy clumps
for (x, y) in [(11, 5), (14, 5), (3, 18), (22, 17)]:
    g[y][x] = TREE
# keep vertical + horizontal spines open
for y in range(1, H-1):
    g[y][12] = GRASS if g[y][12] != TREE or y in (10, 11) else g[y][12]
results['Primalis_JungleInterior'] = write_layout('Primalis_JungleInterior', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_JUNGLE_INTERIOR', 'Primalis_JungleInterior', W, H, *JG)

# =====================================================================
#  ZoanVillage (28x24) — the guardian community's home
# =====================================================================
# down <- JungleInterior, right -> JungleRoute2 (Thorn meets player there).
# Treehouse settlement clearing. Building warps: Inn? no - PC + ElderHall +
# gym-leader NPCs live here. Fog. Central village green.
W, H = 28, 24
g = new_grid(W, H, GRASS)
ring(g, W, H, south_gap=(13, 14), east_gap=(11, 12), wall=TREE)
# ring of canopy/treehouse trees framing the clearing (decorative)
for (x, y) in [(4, 4), (8, 3), (13, 3), (19, 3), (23, 4),
               (3, 9), (24, 9), (3, 15), (24, 15),
               (6, 20), (21, 20)]:
    g[y][x] = TREE
# the elder hall + pokecenter building footprints (warp tiles in front sit on
# plain GRASS; the buildings are drawn with TREE-clumps as placeholder walls)
patch(g, 9, 5, 12, 6, TREE)     # ElderHall footprint (warp at front, y=7)
patch(g, 18, 5, 21, 6, TREE)    # PokemonCenter footprint (warp at front, y=7)
# clear the doorways in front
g[7][10] = GRASS; g[7][19] = GRASS
# central village green (decorative long grass, walkable - NOT encounter)
patch(g, 12, 12, 16, 15, LONGGRASS)
results['Primalis_ZoanVillage'] = write_layout('Primalis_ZoanVillage', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_ZOAN_VILLAGE', 'Primalis_ZoanVillage', W, H, *JG)

# =====================================================================
#  JungleRoute2 (20x30) — second jungle route, BEAST-WHISTLE GATE + Gym 3
# =====================================================================
# west <- ZoanVillage, north -> AncientRuinsCamp. Thorn (Gym 3) meets the
# player on this route. A second dense-undergrowth band gates the north half.
W, H = 20, 30
g = new_grid(W, H, GRASS)
ring(g, W, H, north_gap=(9, 10), west_gap=(14, 15), wall=TREE)
# tall-grass encounter patches
patch(g, 3, 20, 7, 25, TALLGRASS)
patch(g, 12, 21, 16, 26, TALLGRASS)
# decorative growth
patch(g, 14, 4, 17, 6, LONGGRASS)
# BEAST-WHISTLE GATE on the MAIN PATH (correct here): this route's NORTH EXIT leads
# to AncientRuinsCamp -> TheHeartwood, which the player reaches only AFTER the Whistle
# (post-Gym4). Gym 3 (Thorn) is met on this route SOUTH of the seal, so he stays
# reachable; only the north exit to the Heartwood approach is sealed until the Whistle.
# A full-width undergrowth band at y=2 seals the north exit (cols 9-10 spine included).
hline(g, 1, W-2, 2, OVERGROW)
# everything from y>=3 stays open so the player can roam the route and reach Thorn.
for y in range(3, H-1):
    g[y][9] = GRASS
    g[y][10] = GRASS
# light route character (south half, away from the seal)
for (x, y) in [(5, 18), (14, 18), (6, 24), (13, 24)]:
    g[y][x] = TREE
results['Primalis_JungleRoute2'] = write_layout('Primalis_JungleRoute2', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_JUNGLE_ROUTE_2', 'Primalis_JungleRoute2', W, H, *JG)

# =====================================================================
#  AncientRuinsCamp (24x20) — ruins excavation camp, central altar
# =====================================================================
# south <- JungleRoute2, north -> TheHeartwood. Ancient stone ruins reclaimed
# by jungle. A central altar (examine bg_event -> FLAG_PRIMALIS_RUINS_FOUND /
# cipher). Camp NPCs around the clearing.
W, H = 24, 20
g = new_grid(W, H, GRASS)
ring(g, W, H, south_gap=(11, 12), north_gap=(11, 12), wall=TREE)
# exposed ruin walls framing a central plaza (the altar sits in the middle)
patch(g, 7, 4, 16, 4, TREE)
patch(g, 7, 4, 7, 9, TREE)
patch(g, 16, 4, 16, 9, TREE)
g[4][11] = GRASS                  # doorway gap north into the ruin face
g[4][12] = GRASS
patch(g, 8, 5, 15, 9, GRASS)      # walkable plaza around the altar
# the central altar (a single solid block the player faces from the south;
# the examine bg_event sits just south of it)
g[7][11] = TREE                   # altar block
g[7][12] = TREE
# jungle reclaiming the edges (decorative growth)
patch(g, 1, 13, 4, 17, LONGGRASS)
patch(g, 19, 13, 22, 17, LONGGRASS)
results['Primalis_AncientRuinsCamp'] = write_layout('Primalis_AncientRuinsCamp', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_ANCIENT_RUINS_CAMP', 'Primalis_AncientRuinsCamp', W, H, *JG)

# =====================================================================
#  TheHeartwood (28x34) — THE VAST ANCIENT SANCTUARY
# =====================================================================
# south <- AncientRuinsCamp. The oldest place on the island: Verdath woven into
# a colossal root network. A grand overgrown sanctuary - enter from the south,
# walk a long processional clearing flanked by towering ancient growth and root
# walls, up to a central raised clearing where the root-network / seal
# centerpiece sits. Mako + the Lens stand here for the climactic sequence; the
# warp into Heartwood_SealChamber (the actual seal room) is at the far north.
# Monumental scale, deliberately oversized.
W, H = 28, 34
g = new_grid(W, H, GRASS)
ring(g, W, H, south_gap=(13, 14), wall=TREE)
# Dense ancient-growth outer bands down the long sides (impassable) - only the
# central nave is walkable, framing the procession.
patch(g, 1, 1, 6, H-2, TREE)        # west growth wall
patch(g, 21, 1, 26, H-2, TREE)      # east growth wall
# Central processional nave (walkable clearing down the middle, x 8-19)
patch(g, 8, 1, 19, H-2, GRASS)
# Twin colonnades of great trees lining the nave (rows of solid trunks, leaving
# the central walkway open) - this is what reads as "monumental".
for y in range(7, H-7, 3):
    g[y][8]  = TREE                 # west colonnade
    g[y][19] = TREE                 # east colonnade
    g[y][11] = TREE                 # inner west row
    g[y][16] = TREE                 # inner east row
# Keep the very centre (x 13-14) a clear processional path the whole length
for y in range(1, H-1):
    g[y][13] = GRASS
    g[y][14] = GRASS
# THE CENTRAL CLEARING / ROOT-NETWORK CENTERPIECE at the far north (a raised
# walkable clearing) with the seal centerpiece (the Verdath root-knot, a solid
# block the player faces) and the warp into the SealChamber behind it.
patch(g, 9, 2, 18, 7, GRASS)        # the clearing floor (walkable)
patch(g, 11, 13, 16, 14, LONGGRASS) # a ring of sacred growth mid-nave (walkable)
# the root-knot centerpiece (Verdath woven into the roots) - solid, faced from S
g[4][13] = TREE
g[4][14] = TREE
g[3][13] = TREE
g[3][14] = TREE
# small sacred-growth basins flanking the clearing (decorative LONGGRASS)
patch(g, 9, 6, 10, 7, LONGGRASS)
patch(g, 17, 6, 18, 7, LONGGRASS)
# a break in the long walk - flanking growth clumps partway down for scale
patch(g, 9, 20, 10, 23, LONGGRASS)
patch(g, 17, 20, 18, 23, LONGGRASS)
# guarantee the central processional spine stays open after all patches
for y in range(1, H-1):
    g[y][13] = GRASS
    g[y][14] = GRASS
results['Primalis_TheHeartwood'] = write_layout('Primalis_TheHeartwood', g, BORDER_J)
reg_layout('LAYOUT_PRIMALIS_THE_HEARTWOOD', 'Primalis_TheHeartwood', W, H, *JG)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'primalis_layout_entries.json'), 'w') as f:
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
print(f'merged {len(layout_entries)} Primalis layout entries into layouts.json')
