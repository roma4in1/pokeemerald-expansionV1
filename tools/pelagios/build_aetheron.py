#!/usr/bin/env python3
"""Compose Aetheron Isle (the SKY ISLAND) layout blockdata (map.bin/border.bin)
+ merge layout entries into data/layouts/layouts.json.

Mirrors tools/pelagios/build_ashenveil.py (the freshest island template).

Aetheron floats ABOVE THE CLOUDS - permanent thunderstorm, electric, reached via
the Knock Up Stream (ITEM_SEA_CHART). The cloud aesthetic (white luminous ground,
everything low/aerodynamic) is APPROXIMATED with light General ground metatiles;
the void at the map edges (SkyRoute especially) is deep water (reads as the open
sky abyss below). TRUE cloud-palette tinting (white/luminous) is a Porymap per-map
palette pass - DOCUMENTED LIMITATION: maps read green-ground-on-General until then.

Tilesets:
  Cloud/sky outdoor maps (KnockUpStream, CloudLanding, SkyRoute, AetherVillage,
      StormPeak):
      gTileset_General (primary) + gTileset_Slateport (secondary)
      Light stone/ground = cloud-pier + village; deep water = sky void framing.
  CovenantInstallation (exterior):
      gTileset_General (primary) + gTileset_Mauville (secondary)
      Mauville's angular industrial walls = the installation feels WRONG / out of
      place / too-angular for a sky island (the brief's "angular, wrong" aesthetic).
  Interiors (CloudLanding_Inn / _Inn_Interior, AetherVillage_PokemonCenter,
      CovenantInstallation_Interior, SealChamber):
      reuse vanilla LAYOUT_* wholesale in build_aetheron_mapjson.py (zero new binary).

Metatile word format: bits 0-9 metatile id, 10-11 collision, 12-15 elevation.
All metatiles are gTileset_General/Slateport/Mauville primaries-or-secondaries with
collision/elevation baked into the layout word, exactly as build_ashenveil.py does.

Metatiles used (sampled verbatim from LAYOUT_SLATEPORT_CITY / LAYOUT_MAUVILLE_CITY):
  GROUND    0x3001  plain ground (walkable, elev3) = cloud surface / village floor
  WALL      0x04C6  solid grey stone wall block (impassable, elev0)
  WALL2     0x04C7  solid grey stone wall variant (impassable, elev0)
  VOID      0x1170  deep water (impassable) = open-sky abyss framing the edges
  V_S       0x1179  water, land to south (cloud-edge against the void)
  TALLGRASS 0x300D  General tall-grass (wild-encounter "storm patch" tile)
  TECH      0x0691  Mauville angular industrial wall (Installation - feels wrong)
  TECH2     0x0688  Mauville angular industrial wall variant
"""
import json, os, struct

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
LAYOUTS_JSON = os.path.join(ROOT, 'data/layouts/layouts.json')

def mt(metatile_id, collision=0, elevation=3):
    return (metatile_id & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)

# --- sky / cloud terrain palette (General + Slateport primaries) ---
GROUND    = mt(0x3001, 0, 3)   # cloud surface / village floor (walkable)
WALL      = mt(0x04C6, 1, 0)   # solid stone wall (impassable)
WALL2     = mt(0x04C7, 1, 0)   # solid stone wall variant (impassable)
VOID      = mt(0x1170, 1, 3)   # open-sky abyss (impassable) framing the edges
V_S       = mt(0x1179, 1, 3)   # cloud-edge against the void
TALLGRASS = mt(0x300D, 0, 3)   # "storm patch" wild-encounter grass
# --- Covenant Installation industrial palette (General + Mauville) ---
TECH      = mt(0x0691, 1, 0)   # angular industrial wall (feels wrong)
TECH2     = mt(0x0688, 1, 0)   # angular industrial wall variant

BORDER_SKY = [VOID] * 4

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

SKY = ('gTileset_General', 'gTileset_Slateport')
TEK = ('gTileset_General', 'gTileset_Mauville')

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
#  1. KnockUpStream (16x14) — SCRIPTED ASCENT CUTSCENE MAP ONLY
# =====================================================================
# Tiny map: just enough floor for the ascent script. NO NPCs, NO wild encounters,
# NO warps back. The boat (SailToAetheron) warps the player here to the arrival
# tile (8,11); the ascent cutscene plays; then a ONE-WAY warp FORWARD to
# CloudLanding. Framed entirely by VOID (open sky / the rising current). A small
# walkable cloud-disc at the bottom centre is the only stand-on space.
W, H = 16, 14
g = new_grid(W, H, VOID)
# small walkable cloud platform the ascent plays on (bottom-centre)
patch(g, 6, 9, 9, 12, GROUND)
g[11][8] = GROUND   # arrival tile (8,11)
results['Aetheron_KnockUpStream'] = write_layout('Aetheron_KnockUpStream', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_KNOCK_UP_STREAM', 'Aetheron_KnockUpStream', W, H, *SKY)

# =====================================================================
#  2. CloudLanding (20x18) — cloud-pier dock, small settlement, inn
# =====================================================================
# The KnockUpStream one-way warp lands here. The Tennyson docks (SS_TIDAL decoration
# + boarding sign bg_events for sail-back). Inn building (warp). Dockhand NPC stub.
# Arrival coord triggers (VAR_AETHERON_PROGRESS==0). north -> SkyRoute. The cloud-
# pier juts south into the void; the settlement sits on the cloud surface.
W, H = 20, 18
g = new_grid(W, H, GROUND)
ring(g, W, H, north_gap=(9, 10), wall=WALL)
# southern sky-edge: a band of VOID so the cloud-pier juts into open sky
patch(g, 1, 16, W-2, H-1, VOID)
# the cloud-pier (walkable) down toward the dock for the Tennyson
for y in range(13, 17):
    g[y][9] = GROUND
    g[y][10] = GROUND
# the inn building footprint (low aerodynamic block; door warp on plain ground)
patch(g, 4, 4, 7, 6, WALL)
g[6][5] = GROUND   # inn doorway gap (warp tile in front sits on ground at (5,7))
# low cloud-settlement structures (decorative impassable, kept short/aerodynamic)
for (x, y) in [(13, 5), (15, 8), (3, 10), (16, 11), (12, 11)]:
    g[y][x] = WALL2
results['Aetheron_CloudLanding'] = write_layout('Aetheron_CloudLanding', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_CLOUD_LANDING', 'Aetheron_CloudLanding', W, H, *SKY)

# =====================================================================
#  3. SkyRoute (18x32) — narrow walkable path framed by void (vertiginous)
# =====================================================================
# south <- CloudLanding, north -> AetherVillage. Open sky on BOTH sides: a narrow
# cloud-path down the centre framed by VOID columns (the vertigo). Two sight-
# trainer spots (Guardians). Wild Pokemon in "storm patch" grass. The path stays
# >=4 tiles wide so trainers + the player never softlock.
W, H = 18, 32
g = new_grid(W, H, VOID)
# carve the central walkable cloud-path (cols 6-11) down the full length
patch(g, 6, 1, 11, H-2, GROUND)
# north/south openings into the path
patch(g, 7, 0, 10, 0, GROUND)
patch(g, 7, H-1, 10, H-1, GROUND)
# storm-patch encounter grass along the path
patch(g, 6, 6, 8, 9, TALLGRASS)
patch(g, 9, 18, 11, 21, TALLGRASS)
patch(g, 6, 24, 8, 27, TALLGRASS)
# a couple of low cloud-pillars narrowing the path slightly (still >=4 wide)
g[13][6] = WALL2
g[13][11] = WALL2
g[22][6] = WALL2
results['Aetheron_SkyRoute'] = write_layout('Aetheron_SkyRoute', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_SKY_ROUTE', 'Aetheron_SkyRoute', W, H, *SKY)

# =====================================================================
#  4. AetherVillage (24x20) — main settlement, low aerodynamic buildings
# =====================================================================
# south <- SkyRoute, right -> CovenantInstallation. Gym 1 (Gale) + Gym 2 (Arc) as
# talk NPCs. FIRST CASS SIGHTING object (hide flag). PokemonCenter warp. Covenant
# Installation visible in the distance (a sign bg_event on the east edge). Buildings
# are low blocks (nothing tall) with ash-grass-free cloud streets between them.
W, H = 24, 20
g = new_grid(W, H, GROUND)
ring(g, W, H, south_gap=(11, 12), east_gap=(9, 10), wall=WALL)
# low aerodynamic building footprints (impassable blocks)
patch(g, 3, 3, 6, 5, WALL)      # PokemonCenter block (door warp at 5,6 area)
g[5][4] = GROUND                # PC doorway gap (warp tile in front at (4,6))
patch(g, 16, 3, 19, 5, WALL2)   # village hall block
patch(g, 4, 12, 7, 14, WALL2)   # storehouse block
patch(g, 16, 12, 19, 14, WALL2) # workshop block
# keep central plaza + the south/east exit spines clear
patch(g, 10, 6, 13, 17, GROUND)        # central N-S plaza
patch(g, 11, 16, 12, 19, GROUND)       # south exit to SkyRoute
patch(g, 20, 9, 23, 10, GROUND)        # east exit toward CovenantInstallation
results['Aetheron_AetherVillage'] = write_layout('Aetheron_AetherVillage', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_AETHER_VILLAGE', 'Aetheron_AetherVillage', W, H, *SKY)

# =====================================================================
#  5. CovenantInstallation (22x18) — angular industrial exterior (feels WRONG)
# =====================================================================
# west <- AetherVillage, warp -> Interior. Two Covenant-officer sight-trainer spots.
# Mauville secondary = harsh angular industrial walls, out of place on a sky island.
# A blocky, rigid grid of TECH walls (deliberately too-angular) around a central
# approach to the facility entrance (warp into Interior at the north).
W, H = 22, 18
g = new_grid(W, H, GROUND)
ring(g, W, H, west_gap=(8, 9), north_gap=(10, 11), wall=TECH)
# rigid angular industrial blocks (the "wrong" geometry)
patch(g, 3, 3, 6, 6, TECH)
patch(g, 15, 3, 18, 6, TECH)
patch(g, 3, 11, 6, 14, TECH2)
patch(g, 15, 11, 18, 14, TECH2)
# the facility entrance structure at the north (door warp on ground just south)
patch(g, 9, 2, 12, 4, TECH)
g[4][10] = GROUND   # entrance doorway (warp tile in front at (10,5))
# keep the central approach + the west entry spine fully walkable
patch(g, 9, 5, 12, H-2, GROUND)       # central N-S approach
for x in range(1, 9):
    g[8][x] = GROUND
    g[9][x] = GROUND
results['Aetheron_CovenantInstallation'] = write_layout('Aetheron_CovenantInstallation', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_COVENANT_INSTALLATION', 'Aetheron_CovenantInstallation', W, H, *TEK)

# =====================================================================
#  6. StormPeak (18x20) — highest point, directly below the storm
# =====================================================================
# <- CovenantInstallation (warp), -> SealChamber (warp). The extraction apparatus
# (sign bg_event). Post-defection Cass object (script-managed). Lightning-strike FX
# are a script concern (TODO). A bare wind-scoured cloud summit framed by void; the
# largest extraction rig (decorative WALL clump) dominates the centre-north.
W, H = 18, 20
g = new_grid(W, H, VOID)
# the walkable summit platform
patch(g, 4, 3, 13, H-2, GROUND)
g[H-1][8] = GROUND; g[H-1][9] = GROUND   # south entry from the Installation warp
# the extraction apparatus rig (decorative impassable mass, centre-north)
patch(g, 7, 4, 10, 6, WALL2)
g[5][8] = GROUND   # a gap so a sign bg_event can sit at the rig's south face (8,7)
# a couple of scoured cloud-pillars
g[10][5] = WALL
g[14][12] = WALL
results['Aetheron_StormPeak'] = write_layout('Aetheron_StormPeak', g, BORDER_SKY)
reg_layout('LAYOUT_AETHERON_STORM_PEAK', 'Aetheron_StormPeak', W, H, *SKY)

print(json.dumps({'sizes': results}, indent=1))

# write the standalone entries file (mirrors the other islands)
with open(os.path.join(os.path.dirname(__file__), 'aetheron_layout_entries.json'), 'w') as f:
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
print(f'merged {len(layout_entries)} Aetheron layout entries into layouts.json')
