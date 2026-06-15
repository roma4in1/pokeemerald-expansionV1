#!/usr/bin/env python3
"""Generate map.json for all 13 Thalvern maps. Mirrors build_schism_mapjson.py.

Thalvern = the Sunken Kingdom. Water/Psychic. Stilt settlements over rising
water + ancient submerged ruins. Single port (TidespirePort, Galleon-tier).

Tilesets:
  Surface + submerged maps : gTileset_General + gTileset_PelagiosUnderwater
  Inns / PokeCenter        : vanilla LAYOUT_POKEMON_CENTER_1F / _2F (reuse)

Weather:
  All outdoor maps         : WEATHER_RAIN
  CoastalRoute             : WEATHER_FOG_HORIZONTAL (overrides rain; the brief
                             names "WEATHER_FOG_1" which is NOT a real symbol -
                             WEATHER_FOG_HORIZONTAL is the vanilla fog used by
                             Cave of Origin etc.)
  ThroneRoom / CovenantSite: WEATHER_NONE
  Submerged interiors      : WEATHER_RAIN (light) per brief

Harbor rule: the SS_TIDAL (Tennyson, Galleon) is pure decoration (script 0x0);
boarding runs through sign bg_events on the pier -> *_EventScript_Tennyson, which
sets VAR_TEMP_1 = PELAGIOS_ISLAND_THALVERN then goto Pelagios_EventScript_BoardTennyson.

SubmergedRuins_Interior1 is the platform puzzle map (geometry from build_thalvern.py).
ThroneRoom is the vast deepest chamber. Both use gTileset_PelagiosUnderwater.

All EventScript labels are TODO stubs filled by build_thalvern_scripts_stubs.py;
the script-writer implements them.
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

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_RAIN',
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

def conn(direction, offset, mapid):
    return {'map': mapid, 'offset': offset, 'direction': direction}

M = lambda s: 'MAP_THALVERN_' + s
MUS_PORT = 'MUS_SLATEPORT'
MUS_MARKET = 'MUS_LILYCOVE'
MUS_ROUTE = 'MUS_ROUTE110'
MUS_RUINS = 'MUS_SEALED_CHAMBER'
MUS_THRONE = 'MUS_SEALED_CHAMBER'
MUS_COVENANT = 'MUS_MT_PYRE'

# =====================================================================
#  SURFACE
# =====================================================================

# ============================ TidespirePort ============================
# Galleon arrival tile = (10,14): the central stone dock, verified walkable
# (U_GROUND col0). Walking north up the dock crosses the arrival area.
mapjson('Thalvern_TidespirePort', 'LAYOUT_THALVERN_TIDESPIRE_PORT',
    'MAPSEC_THALVERN_TIDESPIRE_PORT', MUS_PORT, 'MAP_TYPE_TOWN', weather='WEATHER_RAIN',
    mapid=M('TIDESPIRE_PORT'),
    connections=[conn('up', -4, M('FLOATING_MARKET'))],
    warps=[warp(4, 8, M('TIDESPIRE_PORT_INN'), 0)],   # 0: into Inn
    objects=[
        obj('OBJ_EVENT_GFX_FISHERMAN', 8, 5, 'ThalvernTidespirePort_EventScript_Fisherman',
            local_id='LOCALID_THALVERN_TIDESPIRE_FISHERMAN'),
        obj('OBJ_EVENT_GFX_MAN_4', 12, 6, 'ThalvernTidespirePort_EventScript_Porter',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'ThalvernTidespirePort_EventScript_Merchant'),
        # The Tennyson (Galleon). Pure decoration; boarding via pier bg_events.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_THALVERN_TIDESPIRE_TENNYSON', elevation=1),
    ],
    coords=[
        # Arrival cutscene fires on first step (ON_FRAME-style guard via progress 0).
        trigger(9, 13, 'VAR_THALVERN_PROGRESS', 0, 'ThalvernTidespirePort_EventScript_Arrival'),
        trigger(10, 13, 'VAR_THALVERN_PROGRESS', 0, 'ThalvernTidespirePort_EventScript_Arrival'),
    ],
    bgs=[
        sign(2, 2, 'ThalvernTidespirePort_EventScript_PortSign'),
        sign(6, 11, 'ThalvernTidespirePort_EventScript_BerthSign'),
        # Boarding from the dock edge / south end.
        sign(11, 13, 'ThalvernTidespirePort_EventScript_Tennyson'),
        sign(11, 14, 'ThalvernTidespirePort_EventScript_Tennyson'),
        sign(11, 15, 'ThalvernTidespirePort_EventScript_Tennyson'),
        sign(8, 13, 'ThalvernTidespirePort_EventScript_Tennyson'),
        sign(8, 14, 'ThalvernTidespirePort_EventScript_Tennyson'),
    ])

# ============================ TidespirePort_Inn (lobby) ============================
mapjson('Thalvern_TidespirePort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_THALVERN_TIDESPIRE_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('TIDESPIRE_PORT_INN'),
    warps=[
        warp(7, 8, M('TIDESPIRE_PORT'), 0),                              # 0
        warp(6, 8, M('TIDESPIRE_PORT'), 0),                              # 1
        warp(1, 6, M('TIDESPIRE_PORT_INN_INTERIOR'), 0, elevation=4),    # 2: up to rooms
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'ThalvernTidespirePortInn_EventScript_Innkeeper',
            local_id='LOCALID_THALVERN_TIDESPIRE_INNKEEPER'),
    ])

# ============================ TidespirePort_Inn_Interior (rooms) ============================
mapjson('Thalvern_TidespirePort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_THALVERN_TIDESPIRE_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('TIDESPIRE_PORT_INN_INTERIOR'),
    warps=[warp(1, 6, M('TIDESPIRE_PORT_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'ThalvernTidespirePortInnInterior_EventScript_Innkeeper2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 9, 5, 'ThalvernTidespirePortInnInterior_EventScript_Scholar',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ FloatingMarket ============================
# Main settlement. down->Port, up->DexCamp, right->CoastalRoute. Gym 1 (Tide) is a
# talk-initiated overworld NPC outside his converted warehouse (Petra/Cinder pattern -
# no separate gym map). No interior warps (no PC map - heal at the port Inn).
mapjson('Thalvern_FloatingMarket', 'LAYOUT_THALVERN_FLOATING_MARKET',
    'MAPSEC_THALVERN_FLOATING_MARKET', MUS_MARKET, 'MAP_TYPE_TOWN', weather='WEATHER_RAIN',
    mapid=M('FLOATING_MARKET'),
    connections=[
        conn('down', 4, M('TIDESPIRE_PORT')),
        conn('up', 6, M('DEX_CAMP')),
        conn('right', -5, M('COASTAL_ROUTE')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 22, 4, 'ThalvernFloatingMarket_EventScript_NumaVessBalcony',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_MARKET_NUMA'),
        obj('OBJ_EVENT_GFX_LITTLE_GIRL', 10, 14, 'ThalvernFloatingMarket_EventScript_DrawingChild',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_MARKET_CHILD'),
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 18, 16, 'ThalvernFloatingMarket_EventScript_Elder',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # Gym 1 - Tide, talk-initiated NPC outside his converted warehouse.
        obj('OBJ_EVENT_GFX_FISHERMAN', 8, 8, 'ThalvernFloatingMarket_EventScript_Tide',
            local_id='LOCALID_THALVERN_TIDE'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 15, 19, 'ThalvernFloatingMarket_EventScript_Vendor',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
    ],
    bgs=[
        sign(8, 6, 'ThalvernFloatingMarket_EventScript_WarehouseSign'),
        sign(24, 4, 'ThalvernFloatingMarket_EventScript_BalconySign'),
    ])

# NOTE: no separate FloatingMarket PokeCenter map - the brief's map group is exactly
# 13 maps and the island's ONLY heal point is the TidespirePort Inn. The market's
# "PokeCenter building" mentioned in the brief is represented by flavor only; players
# heal at the port. (Keeps the canonical 13-map count.)

# ============================ DexCamp ============================
mapjson('Thalvern_DexCamp', 'LAYOUT_THALVERN_DEX_CAMP',
    'MAPSEC_THALVERN_FLOATING_MARKET', MUS_MARKET, 'MAP_TYPE_TOWN', weather='WEATHER_RAIN',
    mapid=M('DEX_CAMP'),
    connections=[conn('down', -6, M('FLOATING_MARKET'))],
    objects=[
        # Dex - the archaeologist. Talk-initiated; first meeting sets DEX_MET.
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 8, 5, 'ThalvernDexCamp_EventScript_Dex',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_DEX'),
        # The Lens, post-defection, joins Dex's camp (hidden until LENS_DEFECTED).
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 11, 6, 'ThalvernDexCamp_EventScript_LensAtCamp',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_THALVERN_LENS_DEFECTED',
            local_id='LOCALID_THALVERN_DEXCAMP_LENS'),
    ],
    bgs=[
        # Research notes on the table (examine for lore - TODO script-writer).
        sign(5, 4, 'ThalvernDexCamp_EventScript_ResearchNotes1',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(10, 4, 'ThalvernDexCamp_EventScript_ResearchNotes2',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(13, 8, 'ThalvernDexCamp_EventScript_EquipmentCrate'),
    ])

# ============================ CoastalRoute ============================
# Route between FloatingMarket and SubmergedRuins. Fog. Vision-patch coord triggers
# on specific tiles (purely atmospheric; the script-writer wires the flash + text).
# Two trainers: Scholar Wren, Researcher Holt.
mapjson('Thalvern_CoastalRoute', 'LAYOUT_THALVERN_COASTAL_ROUTE',
    'MAPSEC_THALVERN_COASTAL_ROUTE', MUS_ROUTE, 'MAP_TYPE_ROUTE',
    weather='WEATHER_FOG_HORIZONTAL',
    mapid=M('COASTAL_ROUTE'),
    connections=[
        conn('left', 5, M('FLOATING_MARKET')),
        conn('up', -2, M('SUBMERGED_RUINS_EXTERIOR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 9, 20, 'ThalvernCoastalRoute_EventScript_ScholarWren',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 10, 9, 'ThalvernCoastalRoute_EventScript_ResearcherHolt',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    coords=[
        # Vision patches - bare TODO coord triggers on the central walkway tiles.
        # The script-writer gates these (e.g. one-shot local flags) and plays the
        # screen-flash + ancient-text line. Left at var_value 0 / VAR_TEMP_2 as a
        # placeholder the script-writer rewires; each tile is its own scene.
        trigger(10, 28, 'VAR_TEMP_2', 0, 'ThalvernCoastalRoute_EventScript_VisionPatch1'),
        trigger(9, 18, 'VAR_TEMP_2', 0, 'ThalvernCoastalRoute_EventScript_VisionPatch2'),
        trigger(10, 7, 'VAR_TEMP_2', 0, 'ThalvernCoastalRoute_EventScript_VisionPatch3'),
    ],
    bgs=[
        sign(8, 16, 'ThalvernCoastalRoute_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ SubmergedRuins_Exterior ============================
# Gym 3 (The Lens) stationed outside. Warp down into Interior1.
mapjson('Thalvern_SubmergedRuins_Exterior', 'LAYOUT_THALVERN_SUBMERGED_RUINS_EXTERIOR',
    'MAPSEC_THALVERN_SUBMERGED_RUINS', MUS_RUINS, 'MAP_TYPE_ROUTE', weather='WEATHER_RAIN',
    mapid=M('SUBMERGED_RUINS_EXTERIOR'),
    connections=[conn('down', 2, M('COASTAL_ROUTE'))],
    warps=[
        warp(11, 5, M('SUBMERGED_RUINS_INTERIOR1'), 0),   # 0: into the ruins
        warp(12, 5, M('SUBMERGED_RUINS_INTERIOR1'), 0),   # 1
    ],
    objects=[
        # Gym 3 - The Lens, stationed at the entrance (talk-initiated). Controls access.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 11, 9, 'ThalvernSubmergedRuinsExterior_EventScript_Lens',
            movement='MOVEMENT_TYPE_FACE_UP', local_id='LOCALID_THALVERN_LENS'),
        # Covenant researchers in the background (flavor NPCs).
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 4, 12, 'ThalvernSubmergedRuinsExterior_EventScript_Researcher1',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 19, 12, 'ThalvernSubmergedRuinsExterior_EventScript_Researcher2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        sign(8, 9, 'ThalvernSubmergedRuinsExterior_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ SubmergedRuins_Interior1 (platform puzzle, Gym 2 Psalm) ============================
mapjson('Thalvern_SubmergedRuins_Interior1', 'LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR1',
    'MAPSEC_THALVERN_SUBMERGED_RUINS', MUS_RUINS, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_RAIN',
    mapid=M('SUBMERGED_RUINS_INTERIOR1'), show_name=True, battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(10, 20, M('SUBMERGED_RUINS_EXTERIOR'), 0),   # 0: back up to the exterior
        warp(11, 20, M('SUBMERGED_RUINS_EXTERIOR'), 0),   # 1
        warp(10, 1, M('SUBMERGED_RUINS_INTERIOR2'), 0),   # 2: deeper, off the dais
        warp(11, 1, M('SUBMERGED_RUINS_INTERIOR2'), 0),   # 3
    ],
    objects=[
        # Gym 2 - Psalm, studying the inscription on the north dais. Talk-initiated.
        obj('OBJ_EVENT_GFX_WOMAN_5', 10, 2, 'ThalvernSubmergedRuinsInterior1_EventScript_Psalm',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_PSALM'),
    ],
    bgs=[
        # TODO (script-writer): the periodic-platform-submerge animation + any Dive
        # hint sign. The platform geometry (elev/collision split over flooded
        # sections) is baked into the layout; sliding/submerge is engine + triggers.
        sign(7, 4, 'ThalvernSubmergedRuinsInterior1_EventScript_PuzzleSign',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ SubmergedRuins_Interior2 (mural) ============================
mapjson('Thalvern_SubmergedRuins_Interior2', 'LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR2',
    'MAPSEC_THALVERN_SUBMERGED_RUINS', MUS_RUINS, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_RAIN',
    mapid=M('SUBMERGED_RUINS_INTERIOR2'), show_name=True,
    warps=[
        warp(10, 18, M('SUBMERGED_RUINS_INTERIOR1'), 2),   # 0: back to Interior1
        warp(11, 18, M('SUBMERGED_RUINS_INTERIOR1'), 2),   # 1
        warp(10, 1, M('SUBMERGED_RUINS_INTERIOR3'), 0),    # 2: deeper to Interior3
        warp(11, 1, M('SUBMERGED_RUINS_INTERIOR3'), 0),    # 3
    ],
    objects=[
        # Covenant researcher blocking the east side passage (TODO gating).
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 16, 7, 'ThalvernSubmergedRuinsInterior2_EventScript_Blocker',
            movement='MOVEMENT_TYPE_FACE_RIGHT', local_id='LOCALID_THALVERN_RUINS2_BLOCKER'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 6, 9, 'ThalvernSubmergedRuinsInterior2_EventScript_Researcher',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # The Lens here (if Gym 3 cleared) - defects quietly, gives the keycard.
        # Hidden until LENS_DEFECTED would normally hide it; here we make him appear
        # when GYM3 done via a hide flag the script-writer manages. Placeholder flag 0
        # (always visible) - script-writer swaps to the proper hide flag.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 13, 6, 'ThalvernSubmergedRuinsInterior2_EventScript_LensDefect',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_THALVERN_GYM3_CLEAR',
            local_id='LOCALID_THALVERN_RUINS2_LENS'),
    ],
    bgs=[
        # Central mural - examine for the extended vision (clearest yet).
        sign(10, 6, 'ThalvernSubmergedRuinsInterior2_EventScript_CentralMural',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(5, 6, 'ThalvernSubmergedRuinsInterior2_EventScript_SideMuralWest',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(15, 6, 'ThalvernSubmergedRuinsInterior2_EventScript_SideMuralEast',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ SubmergedRuins_Interior3 (choice chamber) ============================
mapjson('Thalvern_SubmergedRuins_Interior3', 'LAYOUT_THALVERN_SUBMERGED_RUINS_INTERIOR3',
    'MAPSEC_THALVERN_SUBMERGED_RUINS', MUS_RUINS, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_RAIN',
    mapid=M('SUBMERGED_RUINS_INTERIOR3'), show_name=True,
    warps=[
        warp(8, 15, M('SUBMERGED_RUINS_INTERIOR2'), 2),   # 0: back to Interior2 (south)
        warp(9, 15, M('SUBMERGED_RUINS_INTERIOR2'), 2),   # 1
        warp(8, 1, M('THRONE_ROOM'), 0),                   # 2: into the Throne Room (north)
        warp(9, 1, M('THRONE_ROOM'), 0),                   # 3
        # Access from the DeepApproach (Covenant route) lands here too.
        warp(8, 14, M('DEEP_APPROACH'), 2),                # 4: from DeepApproach
        warp(9, 14, M('DEEP_APPROACH'), 2),                # 5
    ],
    objects=[
        # The Lens delivers the choice warning here (if not met earlier). Talk + the
        # choice scene is a coord trigger as the player approaches the Throne stair.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 8, 8, 'ThalvernSubmergedRuinsInterior3_EventScript_Lens',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_RUINS3_LENS'),
        # Dex, right behind the player for the choice (hidden until the scene; the
        # script-writer reveals/positions him).
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 9, 12, 'ThalvernSubmergedRuinsInterior3_EventScript_Dex',
            movement='MOVEMENT_TYPE_FACE_UP', flag='FLAG_TEMP_1',
            local_id='LOCALID_THALVERN_RUINS3_DEX'),
    ],
    coords=[
        # The choice scene fires as the player nears the Throne Room stair (progress 4,
        # Gym 3 cleared). TODO script-writer wires the "I'll go" / "Let Dex go" choice.
        trigger(8, 3, 'VAR_THALVERN_PROGRESS', 4, 'ThalvernSubmergedRuinsInterior3_EventScript_ChoiceScene'),
        trigger(9, 3, 'VAR_THALVERN_PROGRESS', 4, 'ThalvernSubmergedRuinsInterior3_EventScript_ChoiceScene'),
    ])

# ============================ ThroneRoom (vast) ============================
mapjson('Thalvern_ThroneRoom', 'LAYOUT_THALVERN_THRONE_ROOM',
    'MAPSEC_THALVERN_THRONE_ROOM', MUS_THRONE, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_NONE',
    mapid=M('THRONE_ROOM'), show_name=False,
    warps=[
        warp(13, 38, M('SUBMERGED_RUINS_INTERIOR3'), 2),   # 0: back up to Interior3
        warp(14, 38, M('SUBMERGED_RUINS_INTERIOR3'), 2),   # 1
    ],
    objects=[
        # Pelagios - the guardian deity, barely visible, enormous, on the throne dais.
        # Pure decoration (sealed; not interactable in main story), like every SealChamber
        # legendary. SS_TIDAL placeholder, script 0x0.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 13, 4, '0x0',
            local_id='LOCALID_THALVERN_PELAGIOS', elevation=1),
        # The seal apparatus before the throne (interact to reinforce the seal).
        obj('OBJ_EVENT_GFX_CABLE_CAR', 13, 8, 'ThalvernThroneRoom_EventScript_Apparatus',
            local_id='LOCALID_THALVERN_APPARATUS'),
        # Dex, who goes ahead on PATH B (hidden until the throne sequence; script-writer
        # reveals/positions him). Placeholder hide flag FLAG_TEMP_1.
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 14, 10, 'ThalvernThroneRoom_EventScript_Dex',
            movement='MOVEMENT_TYPE_FACE_UP', flag='FLAG_TEMP_1',
            local_id='LOCALID_THALVERN_THRONE_DEX'),
    ],
    coords=[
        # Throne sequence fires on entry (progress 5 set by the choice scene). Both
        # PATH A (player first) and PATH B (Dex first) branch from here. The discovery
        # row is just inside the north door, on the central processional spine.
        trigger(13, 34, 'VAR_THALVERN_PROGRESS', 5, 'ThalvernThroneRoom_EventScript_ThroneSequence'),
        trigger(14, 34, 'VAR_THALVERN_PROGRESS', 5, 'ThalvernThroneRoom_EventScript_ThroneSequence'),
    ],
    bgs=[
        # Ancient inscriptions flanking the dais (cipher 6 found here).
        sign(10, 7, 'ThalvernThroneRoom_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ CovenantSite ============================
# Numa Vess's base. Accessible after Gym 3 (Lens gives the access card). WEATHER_NONE.
mapjson('Thalvern_CovenantSite', 'LAYOUT_THALVERN_COVENANT_SITE',
    'MAPSEC_THALVERN_COVENANT_SITE', MUS_COVENANT, 'MAP_TYPE_ROUTE', weather='WEATHER_NONE',
    mapid=M('COVENANT_SITE'),
    connections=[conn('down', 2, M('DEEP_APPROACH'))],
    objects=[
        # Director Numa Vess - story antagonist, refuses to battle, then leaves.
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 10, 6, 'ThalvernCovenantSite_EventScript_NumaVess',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_THALVERN_NUMA_VESS'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 6, 8, 'ThalvernCovenantSite_EventScript_Researcher1',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 15, 8, 'ThalvernCovenantSite_EventScript_Researcher2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        # Covenant documents (examine for lore about their plans for Pelagios).
        sign(5, 4, 'ThalvernCovenantSite_EventScript_Documents1',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(16, 4, 'ThalvernCovenantSite_EventScript_Documents2',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ DeepApproach ============================
# Final route to the Throne Room. Covenant blockade (researcher trainers).
# north <- CovenantSite, south warp into Interior3.
mapjson('Thalvern_DeepApproach', 'LAYOUT_THALVERN_DEEP_APPROACH',
    'MAPSEC_THALVERN_COVENANT_SITE', MUS_COVENANT, 'MAP_TYPE_ROUTE', weather='WEATHER_NONE',
    mapid=M('DEEP_APPROACH'),
    connections=[conn('up', -2, M('COVENANT_SITE'))],
    warps=[
        warp(8, 26, M('SUBMERGED_RUINS_INTERIOR3'), 4),   # 0: down into Interior3 (DeepApproach landing)
        warp(9, 26, M('SUBMERGED_RUINS_INTERIOR3'), 4),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 8, 13, 'ThalvernDeepApproach_EventScript_ResearcherSael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 9, 8, 'ThalvernDeepApproach_EventScript_ResearcherCael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[
        sign(2, 4, 'ThalvernDeepApproach_EventScript_CheckpointSign'),
    ])

print('Thalvern map.json files written (13 maps)')
