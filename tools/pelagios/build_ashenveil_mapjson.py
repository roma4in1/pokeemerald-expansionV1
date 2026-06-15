#!/usr/bin/env python3
"""Generate map.json for all 9 Ashenveil maps. Mirrors build_primalis_mapjson.py.

Ashenveil = the DEAD ISLAND (Ghost/Dark). Fog + permanent dusk, ash falling.
HARD CONSTRAINTS (unlike every prior island):
  - NO heal location (no inn, no nurse). Do not register one.
  - NO trainers, NO gym leaders. Zero trainer/gym objects.
  - WILD encounters in ONLY 3 maps: AshFields1, DeadCity_Exterior, MorthasGrove.
  - The ONLY living NPC is Orin at the Greyport outpost. Every other "NPC" is
    environmental detail = examine bg_events (signs). Plus Morthas (overworld
    object, hidden after encounter) and Dorne (script-managed, hidden object).

Tilesets: outdoor maps pair gTileset_General (primary) + gTileset_Slateport
(secondary) - grey stone = ash-grey ruined city (closest vanilla dusk look).
Interiors reuse vanilla LAYOUT_* wholesale (zero new binary):
  Greyport_Outpost / _Outpost_Interior : LAYOUT_HOUSE2 (small research outpost)
  DeadCity_Ruins1                      : LAYOUT_GRANITE_CAVE_B1F (ruined building)
  DeadCity_Ruins2                      : LAYOUT_GRANITE_CAVE_B2F (deeper basement)

Weather: WEATHER_FOG_HORIZONTAL on all 9 outdoor maps; interiors WEATHER_NONE
(Outpost / _Interior / Ruins1 / Ruins2). NOTE the brief's "WEATHER_FOGGY" does
not exist - 6 = FOG_HORIZONTAL is the real fog overlay. Dusk palette is a Porymap
limitation (see build_ashenveil.py header) - grey metatiles + fog approximate it.

PHANTOM-LANTERN ash-gates: impassable ASHWALL tiles baked into AshFields1 seal
OPTIONAL side nooks only; the main spine stays open (softlock-safe). TODO coord
triggers (VAR_ASHENVEIL_PROGRESS) front each seal for the script-writer's field
effect. The Lantern is obtained EARLY (Greyport outpost), so gates are safe.

Boat: SS_TIDAL (Tennyson, Galleon) is pure decoration (script 0x0); boarding via
sign bg_events on the dock -> AshenveilGreyport_EventScript_Tennyson, which sets
VAR_TEMP_1 = PELAGIOS_ISLAND_ASHENVEIL then gotos Pelagios_EventScript_BoardTennyson.

NO bg_hidden_item events anywhere. The Sea Chart is a SCRIPTED examine bg_event
(sign), not a hidden item (no hidden-item flag consumed).

All EventScript labels are TODO stubs filled by build_ashenveil_scripts_stubs.py
(except the Tennyson boat-boarding wiring).
"""
import json, os

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')

def obj(gfx, x, y, script='0x0', movement='MOVEMENT_TYPE_FACE_DOWN', rx=0, ry=0,
        elevation=3, trainer_type='TRAINER_TYPE_NONE', sight='0', flag='0', local_id=None):
    d = {}
    if local_id: d['local_id'] = local_id
    d.update({
        'graphics_id': gfx, 'x': x, 'y': y, 'elevation': elevation,
        'movement_type': movement, 'movement_range_x': rx, 'movement_range_y': ry,
        'trainer_type': trainer_type, 'trainer_sight_or_berry_tree_id': sight,
        'script': script, 'flag': flag})
    return d

def warp(x, y, dest, dest_id, elevation=0):
    return {'x': x, 'y': y, 'elevation': elevation, 'dest_map': dest, 'dest_warp_id': str(dest_id)}

def sign(x, y, script, facing='BG_EVENT_PLAYER_FACING_ANY', elevation=0):
    return {'type': 'sign', 'x': x, 'y': y, 'elevation': elevation,
            'player_facing_dir': facing, 'script': script}

def trigger(x, y, var, value, script, elevation=3):
    return {'type': 'trigger', 'x': x, 'y': y, 'elevation': elevation,
            'var': var, 'var_value': str(value), 'script': script}

def conn(direction, offset, mapid):
    return {'map': mapid, 'offset': offset, 'direction': direction}

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_FOG_HORIZONTAL',
            connections=None, objects=(), warps=(), coords=(), bgs=(),
            show_name=True, indoor=False, mapid=None, battle_scene='MAP_BATTLE_SCENE_NORMAL'):
    d = {
        'id': mapid, 'name': name, 'layout': layout, 'music': music,
        'region': 'REGION_HOENN', 'region_map_section': mapsec,
        'requires_flash': False, 'weather': weather, 'map_type': map_type,
        'allow_cycling': not indoor, 'allow_escaping': False, 'allow_running': not indoor,
        'show_map_name': show_name, 'battle_scene': battle_scene,
        'connections': connections, 'object_events': list(objects),
        'warp_events': list(warps), 'coord_events': list(coords),
        'bg_events': list(bgs),
    }
    out = os.path.join(ROOT, 'data/maps', name)
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, 'map.json'), 'w') as f:
        json.dump(d, f, indent=2)
        f.write('\n')

M = lambda s: 'MAP_ASHENVEIL_' + s
FOG = 'WEATHER_FOG_HORIZONTAL'
NONE = 'WEATHER_NONE'

MUS_PORT  = 'MUS_ABNORMAL_WEATHER'   # bleak port theme placeholder
MUS_FIELD = 'MUS_ROUTE120'           # fog route placeholder
MUS_CITY  = 'MUS_MT_PYRE'            # dead-city dirge placeholder
MUS_MEM   = 'MUS_MT_PYRE_EXTERIOR'   # memorial placeholder
MUS_GROVE = 'MUS_SEALED_CHAMBER'     # grove / seal placeholder
MUS_RUINS = 'MUS_MT_PYRE'            # ruins interior placeholder

# sprite placeholders (no Ashenveil-specific gfx; documented in CLAUDE.md)
GFX_ORIN  = 'OBJ_EVENT_GFX_SCIENTIST_1'   # Orin, the sole living researcher
GFX_DORNE = 'OBJ_EVENT_GFX_MAN_4'         # Marshal Dorne (script-managed)
GFX_MORTHAS = 'OBJ_EVENT_GFX_SS_TIDAL'    # Morthas (sealed legendary decoration)

# =====================================================================
#  1. Greyport (20x18) — tiny ruined Galleon port + outpost
# =====================================================================
mapjson('Ashenveil_Greyport', 'LAYOUT_ASHENVEIL_GREYPORT',
    'MAPSEC_ASHENVEIL_GREYPORT', MUS_PORT, 'MAP_TYPE_TOWN', weather=FOG,
    mapid=M('GREYPORT'),
    connections=[conn('up', -1, M('ASH_FIELDS1'))],
    warps=[warp(5, 7, M('GREYPORT_OUTPOST'), 0)],   # 0: into the outpost
    objects=[
        # Orin - the ONLY living person on the island, at the outpost entrance.
        obj(GFX_ORIN, 7, 8, 'AshenveilGreyport_EventScript_Orin',
            local_id='LOCALID_ASHENVEIL_ORIN'),
        # The Tennyson (Galleon). Pure decoration; boarding via dock bg_events.
        obj(GFX_MORTHAS, 12, 15, '0x0',
            local_id='LOCALID_ASHENVEIL_TENNYSON', elevation=1,
            movement='MOVEMENT_TYPE_NONE'),
    ],
    coords=[
        # Arrival cutscene: fires walking north off the dock at progress 0.
        trigger(9, 12, 'VAR_ASHENVEIL_PROGRESS', 0, 'AshenveilGreyport_EventScript_Arrival'),
        trigger(10, 12, 'VAR_ASHENVEIL_PROGRESS', 0, 'AshenveilGreyport_EventScript_Arrival'),
    ],
    bgs=[
        # Boarding the Tennyson: sign bg_events on the dock tiles (never the ship).
        sign(9, 15, 'AshenveilGreyport_EventScript_Tennyson'),
        sign(10, 15, 'AshenveilGreyport_EventScript_Tennyson'),
        sign(9, 14, 'AshenveilGreyport_EventScript_Tennyson'),
        sign(10, 14, 'AshenveilGreyport_EventScript_Tennyson'),
    ])

# =====================================================================
#  2. Greyport_Outpost (HOUSE2) — Orin's research outpost (foyer)
# =====================================================================
# Examine bg_events: research notes on a desk; the Phantom Lantern handoff sign.
mapjson('Ashenveil_Greyport_Outpost', 'LAYOUT_HOUSE2',
    'MAPSEC_ASHENVEIL_GREYPORT', MUS_RUINS, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('GREYPORT_OUTPOST'),
    warps=[
        warp(3, 7, M('GREYPORT'), 0),                          # 0: out to port
        warp(4, 7, M('GREYPORT'), 0),                          # 1
        warp(8, 2, M('GREYPORT_OUTPOST_INTERIOR'), 0),         # 2: into back room (walkable floor)
    ],
    bgs=[
        # Research notes (the math: 847 people, 400 capacity, no follow-up report).
        sign(2, 1, 'AshenveilGreyportOutpost_EventScript_ResearchNotes',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        # ITEM_PHANTOM_LANTERN handoff (script-writer gives the Lantern +
        # FLAG_PHANTOM_LANTERN_OBTAINED). Examine sign on the lantern shelf.
        sign(6, 1, 'AshenveilGreyportOutpost_EventScript_LanternShelf',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  3. Greyport_Outpost_Interior (HOUSE2) — back room, Cass-was-here detail
# =====================================================================
mapjson('Ashenveil_Greyport_Outpost_Interior', 'LAYOUT_HOUSE2',
    'MAPSEC_ASHENVEIL_GREYPORT', MUS_RUINS, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('GREYPORT_OUTPOST_INTERIOR'),
    warps=[
        warp(8, 2, M('GREYPORT_OUTPOST'), 2),                  # 0: back to foyer (walkable floor)
        warp(9, 2, M('GREYPORT_OUTPOST'), 2),                  # 1
    ],
    bgs=[
        # Half-finished cup of tea, still faintly warm (Cass was here).
        sign(5, 1, 'AshenveilGreyportOutpostInterior_EventScript_TeaCup',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  4. AshFields1 (22x32) — ash plains, hidden paths, environmental detail
# =====================================================================
# Wild Ghost encounters. 4 environmental examine bg_events. Phantom-Lantern
# ash-gates seal OPTIONAL nooks (main spine open - softlock-safe).
mapjson('Ashenveil_AshFields1', 'LAYOUT_ASHENVEIL_ASH_FIELDS1',
    'MAPSEC_ASHENVEIL_ASH_FIELDS', MUS_FIELD, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('ASH_FIELDS1'),
    connections=[
        conn('down', 1, M('GREYPORT')),
        conn('up', -5, M('DEAD_CITY_EXTERIOR')),
    ],
    coords=[
        # Phantom-Lantern ash-gate triggers (optional nooks). TODO field effect:
        # script-writer checks ITEM_PHANTOM_LANTERN and clears the ash-wall row.
        trigger(2, 16, 'VAR_ASHENVEIL_PROGRESS', 2, 'AshenveilAshFields1_EventScript_LanternGate'),
        trigger(3, 16, 'VAR_ASHENVEIL_PROGRESS', 2, 'AshenveilAshFields1_EventScript_LanternGate'),
        trigger(18, 16, 'VAR_ASHENVEIL_PROGRESS', 2, 'AshenveilAshFields1_EventScript_LanternGate'),
    ],
    bgs=[
        # 4 environmental examine signs (the dead island's quiet detail).
        sign(5, 13, 'AshenveilAshFields1_EventScript_ChildShoe'),       # (1) red child's shoe
        sign(16, 13, 'AshenveilAshFields1_EventScript_DoorHandle'),     # (2) polished brass handle
        sign(6, 20, 'AshenveilAshFields1_EventScript_StreetSign'),      # (3) "RIVER LANE"
        sign(15, 20, 'AshenveilAshFields1_EventScript_Footprints'),     # (4) Dorne's footprints
    ])

# =====================================================================
#  5. DeadCity_Exterior (32x30) — full ruined city, LARGE. Ghost wild. No NPCs.
# =====================================================================
mapjson('Ashenveil_DeadCity_Exterior', 'LAYOUT_ASHENVEIL_DEAD_CITY_EXTERIOR',
    'MAPSEC_ASHENVEIL_DEAD_CITY', MUS_CITY, 'MAP_TYPE_CITY', weather=FOG,
    mapid=M('DEAD_CITY_EXTERIOR'),
    connections=[
        conn('down', 5, M('ASH_FIELDS1')),
        conn('up', 6, M('THE_MEMORIAL')),
    ],
    warps=[
        warp(5, 7, M('DEAD_CITY_RUINS1'), 0),    # 0: into Ruins1 (official report)
        warp(13, 7, M('DEAD_CITY_RUINS2'), 0),   # 1: into Ruins2 (real documents)
    ])

# =====================================================================
#  6. DeadCity_Ruins1 (GRANITE_CAVE_B1F) — official sanitized report
# =====================================================================
mapjson('Ashenveil_DeadCity_Ruins1', 'LAYOUT_GRANITE_CAVE_B1F',
    'MAPSEC_ASHENVEIL_DEAD_CITY', MUS_RUINS, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('DEAD_CITY_RUINS1'),
    warps=[
        warp(25, 13, M('DEAD_CITY_EXTERIOR'), 0),   # 0: out
        warp(4, 21, M('DEAD_CITY_EXTERIOR'), 0),    # 1
    ],
    bgs=[
        # The official Covenant evacuation report (sanitized) on a notice board.
        sign(25, 11, 'AshenveilDeadCityRuins1_EventScript_OfficialReport',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  7. DeadCity_Ruins2 (GRANITE_CAVE_B2F) — the REAL Covenant documents
# =====================================================================
mapjson('Ashenveil_DeadCity_Ruins2', 'LAYOUT_GRANITE_CAVE_B2F',
    'MAPSEC_ASHENVEIL_DEAD_CITY', MUS_RUINS, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('DEAD_CITY_RUINS2'),
    warps=[
        warp(29, 13, M('DEAD_CITY_EXTERIOR'), 1),   # 0: out
        warp(28, 21, M('DEAD_CITY_EXTERIOR'), 1),   # 1
    ],
    bgs=[
        # The real documents (Cass's copy, on the floor) -> sets
        # FLAG_ASHENVEIL_COVENANT_DOCS_FOUND (script-writer).
        sign(29, 11, 'AshenveilDeadCityRuins2_EventScript_RealDocuments',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  8. TheMemorial (20x18) — small garden, grave markers, DORNE SCENE space
# =====================================================================
# NO wild, NO living NPC placed; a hidden script-managed Dorne object (hide flag
# FLAG_ASHENVEIL_DORNE_MET - visible until the scene plays, gone after). The scene
# itself (Dorne walks in / player tableau) is the script-writer's; we provide the
# object + clear walkable space. 4 grave markers as examine signs; one is newer.
mapjson('Ashenveil_TheMemorial', 'LAYOUT_ASHENVEIL_THE_MEMORIAL',
    'MAPSEC_ASHENVEIL_MEMORIAL', MUS_MEM, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('THE_MEMORIAL'),
    connections=[
        conn('down', -6, M('DEAD_CITY_EXTERIOR')),
        conn('right', -2, M('MORTHAS_GROVE')),
    ],
    objects=[
        # Dorne - script-managed. Hidden by FLAG_ASHENVEIL_DORNE_MET so he is
        # PRESENT until the memorial scene plays, then removed (the scene sets the
        # flag on the player's choice). Placed at his daughter's grave (the newest).
        obj(GFX_DORNE, 14, 6, 'AshenveilTheMemorial_EventScript_Dorne',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_ASHENVEIL_DORNE_MET',
            local_id='LOCALID_ASHENVEIL_DORNE'),
    ],
    coords=[
        # Dorne scene fires on entry into the garden (TODO script). Progress 3
        # (dead city reached) -> the memorial scene. Walking up into the garden.
        trigger(9, 11, 'VAR_ASHENVEIL_PROGRESS', 3, 'AshenveilTheMemorial_EventScript_DorneScene'),
        trigger(10, 11, 'VAR_ASHENVEIL_PROGRESS', 3, 'AshenveilTheMemorial_EventScript_DorneScene'),
    ],
    bgs=[
        # Grave markers (examine). The 4th (rightmost) is the newest - Vael Jr.
        sign(6, 5, 'AshenveilTheMemorial_EventScript_Grave1',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(9, 5, 'AshenveilTheMemorial_EventScript_Grave2',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(12, 5, 'AshenveilTheMemorial_EventScript_Grave3',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(14, 5, 'AshenveilTheMemorial_EventScript_GraveNewest',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  9. MorthasGrove (22x22) — cathedral of dead trees, dark pool, Sea Chart
# =====================================================================
# Darkest map. Morthas overworld object near the pool, HIDDEN after encounter via
# FLAG_MORTHAS_ENCOUNTERED (object visible while unset, gone once set). The Sea
# Chart is a SCRIPTED examine bg_event near the tree line (gives ITEM_SEA_CHART +
# FLAG_SEA_CHART_FOUND - NOT a hidden item). Wild Ghost (pre-encounter).
mapjson('Ashenveil_MorthasGrove', 'LAYOUT_ASHENVEIL_MORTHAS_GROVE',
    'MAPSEC_ASHENVEIL_MORTHAS_GROVE', MUS_GROVE, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('MORTHAS_GROVE'),
    connections=[conn('left', 2, M('THE_MEMORIAL'))],
    objects=[
        # Morthas - the sealed legendary, visible at the pool until encountered.
        # Hide flag FLAG_MORTHAS_ENCOUNTERED: present while unset, gone once set.
        obj(GFX_MORTHAS, 10, 8, 'AshenveilMorthasGrove_EventScript_Morthas',
            movement='MOVEMENT_TYPE_NONE', elevation=1,
            flag='FLAG_MORTHAS_ENCOUNTERED',
            local_id='LOCALID_ASHENVEIL_MORTHAS'),
    ],
    coords=[
        # Approaching the pool triggers the Morthas endurance encounter (TODO).
        # Progress 5 (sea chart found) - the player approaches the pool's edge.
        trigger(10, 13, 'VAR_ASHENVEIL_PROGRESS', 5, 'AshenveilMorthasGrove_EventScript_MorthasApproach'),
        trigger(11, 13, 'VAR_ASHENVEIL_PROGRESS', 5, 'AshenveilMorthasGrove_EventScript_MorthasApproach'),
    ],
    bgs=[
        # The ash-covered stone near the tree line - examine gives ITEM_SEA_CHART
        # + FLAG_SEA_CHART_FOUND (Cass left it). Scripted examine, NOT hidden item.
        sign(15, 16, 'AshenveilMorthasGrove_EventScript_SeaChartStone'),
    ])

print('Ashenveil map.json files written (9 maps)')
