#!/usr/bin/env python3
"""Generate map.json files for all Haven Isle maps."""
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

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_SUNNY',
            connections=None, objects=(), warps=(), coords=(), bgs=(),
            show_name=True, indoor=False, mapid=None):
    d = {
        'id': mapid,
        'name': name,
        'layout': layout,
        'music': music,
        'region': 'REGION_HOENN',
        'region_map_section': mapsec,
        'requires_flash': False,
        'weather': weather,
        'map_type': map_type,
        'allow_cycling': not indoor,
        'allow_escaping': False,
        'allow_running': True,
        'show_map_name': show_name,
        'battle_scene': 'MAP_BATTLE_SCENE_NORMAL',
        'connections': connections,
        'object_events': list(objects),
        'warp_events': list(warps),
        'coord_events': list(coords),
        'bg_events': list(bgs),
    }
    out = os.path.join(ROOT, 'data/maps', name)
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, 'map.json'), 'w') as f:
        json.dump(d, f, indent=2)
        f.write('\n')

def conn(direction, offset, mapid):
    return {'map': mapid, 'offset': offset, 'direction': direction}

M = lambda s: 'MAP_HAVEN_ISLE_' + s

# ---------------- Tidemark Village ----------------
mapjson('HavenIsle_TidemarkVillage', 'LAYOUT_HAVEN_ISLE_TIDEMARK_VILLAGE',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_LITTLEROOT', 'MAP_TYPE_TOWN',
    mapid=M('TIDEMARK_VILLAGE'),
    connections=[
        conn('up', 2, M('ROUTE1')),
        conn('down', 2, M('HARBOR')),
        conn('right', 3, M('ROUTE2')),
    ],
    warps=[
        warp(5, 7, M('PLAYER_HOUSE_1F'), 1),
        warp(9, 7, M('CASS_HOUSE'), 1),
        warp(19, 7, M('SOLLIS_LAB'), 0),
        warp(8, 14, M('POKEMON_CENTER_1F'), 0),
        warp(14, 14, M('MART'), 0),
        warp(18, 18, M('ELDER_HOUSE'), 0),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 8, 10, 'TidemarkVillage_EventScript_Cass',
            local_id='LOCALID_TIDEMARK_CASS', flag='FLAG_HIDE_TIDEMARK_CASS_INTRO'),
        # Keep her wander column clear of (5,10), where the Cass intercept
        # parks the player.
        obj('OBJ_EVENT_GFX_OLD_WOMAN', 3, 11, 'TidemarkVillage_EventScript_OldWoman',
            movement='MOVEMENT_TYPE_WANDER_UP_AND_DOWN', rx=0, ry=1),
        obj('OBJ_EVENT_GFX_LITTLE_BOY', 12, 16, 'TidemarkVillage_EventScript_Child',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=2, ry=2),
        obj('OBJ_EVENT_GFX_SAILOR', 12, 18, 'TidemarkVillage_EventScript_Sailor'),
    ],
    coords=[
        # Fires on the doormat tile the door-exit step lands on (just south of
        # the player house door at (5,7)).
        trigger(5, 8, 'VAR_PELAGIOS_INTRO_STATE', 2, 'TidemarkVillage_EventScript_CassIntercept'),
        trigger(10, 2, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockNorth'),
        trigger(11, 2, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockNorth'),
        trigger(12, 2, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockNorth'),
        trigger(13, 2, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockNorth'),
        trigger(21, 9, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockEast'),
        trigger(21, 10, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockEast'),
        trigger(21, 11, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockEast'),
        trigger(21, 12, 'VAR_HAVEN_RUINS_STATE', 0, 'TidemarkVillage_EventScript_BlockEast'),
    ],
    bgs=[
        sign(9, 9, 'TidemarkVillage_EventScript_VillageSign'),
        sign(17, 8, 'TidemarkVillage_EventScript_LabSign'),
    ])

# ---------------- Harbor ----------------
mapjson('HavenIsle_Harbor', 'LAYOUT_HAVEN_ISLE_HARBOR',
    'MAPSEC_HAVEN_ISLE_HARBOR', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN',
    mapid=M('HARBOR'),
    connections=[conn('up', -2, M('TIDEMARK_VILLAGE'))],
    objects=[
        obj('OBJ_EVENT_GFX_SAILOR', 5, 8, 'HavenIsleHarbor_EventScript_Sailor'),
        # The SS_TIDAL sprite (96x40, drawn centered over ~6x2.5 tiles) is pure
        # decoration, exactly like every vanilla SS Tidal (script 0x0). Actual
        # boarding interaction is the row of sign bg_events on the shore-edge
        # water tiles below - reliable from the sand regardless of sprite size.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 13, 11, '0x0',
            local_id='LOCALID_HARBOR_TENNYSON', elevation=1),
    ],
    bgs=[
        sign(7, 4, 'HavenIsleHarbor_EventScript_VillageSign'),
        sign(12, 8, 'HavenIsleHarbor_EventScript_BerthSign'),
        # Boarding interaction: face the water/ship from the sand at row 9
        # anywhere along the hull and press A.
        sign(12, 10, 'HavenIsleHarbor_EventScript_Tennyson'),
        sign(13, 10, 'HavenIsleHarbor_EventScript_Tennyson'),
        sign(14, 10, 'HavenIsleHarbor_EventScript_Tennyson'),
    ])

# ---------------- Coastal Route 1 ----------------
mapjson('HavenIsle_Route1', 'LAYOUT_HAVEN_ISLE_ROUTE1',
    'MAPSEC_HAVEN_COASTAL_ROUTE_1', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    mapid=M('ROUTE1'),
    connections=[
        conn('down', -2, M('TIDEMARK_VILLAGE')),
        conn('up', 1, M('ANCIENT_RUINS_EXTERIOR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_YOUNGSTER', 10, 20, 'HavenIsleRoute1_EventScript_Jay',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_BERRY_TREE', 4, 28, 'BerryTreeScript',
            movement='MOVEMENT_TYPE_BERRY_TREE_GROWTH', sight='BERRY_TREE_HAVEN_ROUTE1_ORAN_1'),
        obj('OBJ_EVENT_GFX_BERRY_TREE', 5, 28, 'BerryTreeScript',
            movement='MOVEMENT_TYPE_BERRY_TREE_GROWTH', sight='BERRY_TREE_HAVEN_ROUTE1_ORAN_2'),
        obj('OBJ_EVENT_GFX_ITEM_BALL', 13, 34, 'Common_EventScript_FindItem',
            movement='MOVEMENT_TYPE_LOOK_AROUND', sight='ITEM_POTION',
            flag='FLAG_ITEM_HAVEN_ROUTE1_POTION'),
    ],
    bgs=[sign(9, 36, 'HavenIsleRoute1_EventScript_RouteSign')])

# ---------------- Ancient Ruins Exterior ----------------
mapjson('HavenIsle_AncientRuinsExterior', 'LAYOUT_HAVEN_ISLE_ANCIENT_RUINS_EXTERIOR',
    'MAPSEC_HAVEN_ANCIENT_RUINS', 'MUS_ROUTE113', 'MAP_TYPE_ROUTE',
    mapid=M('ANCIENT_RUINS_EXTERIOR'),
    connections=[conn('down', -1, M('ROUTE1'))],
    warps=[warp(8, 10, M('ANCIENT_RUINS_INTERIOR1'), 0)],
    # The disturbance scene runs as an ON_FRAME entry script on Interior1
    # (guaranteed inside the ruins) - no exterior coord triggers.
    bgs=[sign(10, 11, 'AncientRuinsExterior_EventScript_Sign')])

# ---------------- Ancient Ruins Interior 1 ----------------
mapjson('HavenIsle_AncientRuinsInterior1', 'LAYOUT_HAVEN_ISLE_ANCIENT_RUINS_INTERIOR1',
    'MAPSEC_HAVEN_ANCIENT_RUINS', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('ANCIENT_RUINS_INTERIOR1'),
    warps=[
        warp(18, 22, M('ANCIENT_RUINS_EXTERIOR'), 0),
        warp(5, 3, M('ANCIENT_RUINS_INTERIOR2'), 0),
    ],
    objects=[
        # (16,19): verified walkable floor (0x3211) in the entrance chamber,
        # on-screen from the entry tile (18,22) when the ON_FRAME scene plays.
        obj('OBJ_EVENT_GFX_SPECIES(RALTS)', 16, 19, '0x0',
            local_id='LOCALID_RUINS_RALTS', flag='FLAG_HIDE_RUINS_RALTS'),
    ],
    bgs=[sign(17, 10, 'AncientRuinsInterior1_EventScript_GlowingTile')])

# ---------------- Ancient Ruins Interior 2 ----------------
mapjson('HavenIsle_AncientRuinsInterior2', 'LAYOUT_HAVEN_ISLE_ANCIENT_RUINS_INTERIOR2',
    'MAPSEC_HAVEN_ANCIENT_RUINS', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('ANCIENT_RUINS_INTERIOR2'),
    warps=[warp(7, 3, M('ANCIENT_RUINS_INTERIOR1'), 1)],
    bgs=[sign(4, 5, 'AncientRuinsInterior2_EventScript_Inscription',
              facing='BG_EVENT_PLAYER_FACING_NORTH')])

# ---------------- Coastal Route 2 ----------------
mapjson('HavenIsle_Route2', 'LAYOUT_HAVEN_ISLE_ROUTE2',
    'MAPSEC_HAVEN_COASTAL_ROUTE_2', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    mapid=M('ROUTE2'),
    connections=[
        conn('left', -3, M('TIDEMARK_VILLAGE')),
        conn('right', 3, M('FISHING_DOCKS')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_FISHERMAN', 15, 10, 'HavenIsleRoute2_EventScript_OldBren',
            trainer_type='TRAINER_TYPE_NORMAL', sight='2'),
    ],
    bgs=[sign(3, 5, 'HavenIsleRoute2_EventScript_RouteSign')])

# ---------------- Fishing Docks ----------------
mapjson('HavenIsle_FishingDocks', 'LAYOUT_HAVEN_ISLE_FISHING_DOCKS',
    'MAPSEC_HAVEN_FISHING_DOCKS', 'MUS_DEWFORD', 'MAP_TYPE_ROUTE',
    mapid=M('FISHING_DOCKS'),
    connections=[conn('left', -3, M('ROUTE2'))],
    objects=[
        obj('OBJ_EVENT_GFX_FISHERMAN', 5, 6, 'FishingDocks_EventScript_OldFisherman',
            local_id='LOCALID_DOCKS_OLD_FISHERMAN'),
        obj('OBJ_EVENT_GFX_SAILOR', 9, 7, 'FishingDocks_EventScript_TipsSailor'),
    ],
    bgs=[sign(10, 5, 'FishingDocks_EventScript_Sign')])

# ---------------- Player's House 1F ----------------
mapjson('HavenIsle_PlayerHouse_1F', 'LAYOUT_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_LITTLEROOT', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('PLAYER_HOUSE_1F'),
    warps=[
        warp(8, 8, M('TIDEMARK_VILLAGE'), 0),
        warp(9, 8, M('TIDEMARK_VILLAGE'), 0),
        warp(8, 2, M('PLAYER_HOUSE_2F'), 0),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MOM', 5, 4, 'HavenIslePlayerHouse_1F_EventScript_Mom',
            local_id='LOCALID_HAVEN_MOM', movement='MOVEMENT_TYPE_FACE_DOWN', rx=1, ry=0),
    ])

# ---------------- Player's House 2F ----------------
mapjson('HavenIsle_PlayerHouse_2F', 'LAYOUT_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_LITTLEROOT', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('PLAYER_HOUSE_2F'),
    warps=[warp(7, 1, M('PLAYER_HOUSE_1F'), 2)],
    bgs=[
        sign(0, 1, 'HavenIslePlayerHouse_2F_EventScript_PC',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(1, 1, 'HavenIslePlayerHouse_2F_EventScript_Notebook'),
    ])

# ---------------- Cass's House ----------------
mapjson('HavenIsle_CassHouse', 'LAYOUT_HOUSE1',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_LITTLEROOT', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('CASS_HOUSE'),
    warps=[
        warp(3, 8, M('TIDEMARK_VILLAGE'), 1),
        warp(4, 8, M('TIDEMARK_VILLAGE'), 1),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_WOMAN_1', 5, 4, 'HavenIsleCassHouse_EventScript_CassMother'),
    ])

# ---------------- Village Elder's House ----------------
mapjson('HavenIsle_ElderHouse', 'LAYOUT_HOUSE2',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_LITTLEROOT', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('ELDER_HOUSE'),
    warps=[
        warp(3, 7, M('TIDEMARK_VILLAGE'), 5),
        warp(4, 7, M('TIDEMARK_VILLAGE'), 5),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_EXPERT_M', 7, 4, 'HavenIsleElderHouse_EventScript_Elder',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ---------------- Professor Sollis's Lab ----------------
mapjson('HavenIsle_SollisLab', 'LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_BIRCH_LAB', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('SOLLIS_LAB'),
    warps=[
        warp(6, 12, M('TIDEMARK_VILLAGE'), 2),
        warp(7, 12, M('TIDEMARK_VILLAGE'), 2),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 6, 4, 'SollisLab_EventScript_Sollis',
            local_id='LOCALID_LAB_SOLLIS'),
        obj('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 8, 6, 'SollisLab_EventScript_Cass',
            local_id='LOCALID_LAB_CASS', movement='MOVEMENT_TYPE_FACE_UP',
            flag='FLAG_HIDE_SOLLIS_LAB_CASS'),
        obj('OBJ_EVENT_GFX_SPECIES(TOTODILE)', 5, 10, 'SollisLab_EventScript_Totodile',
            local_id='LOCALID_LAB_TOTODILE', movement='MOVEMENT_TYPE_WANDER_AROUND',
            rx=1, ry=1, flag='FLAG_HIDE_SOLLIS_LAB_TOTODILE'),
        obj('OBJ_EVENT_GFX_SPECIES(CHIMCHAR)', 2, 2, 'SollisLab_EventScript_Chimchar',
            local_id='LOCALID_LAB_CHIMCHAR', flag='FLAG_HIDE_SOLLIS_LAB_CHIMCHAR'),
        obj('OBJ_EVENT_GFX_SPECIES(ROWLET)', 9, 3, 'SollisLab_EventScript_Rowlet',
            local_id='LOCALID_LAB_ROWLET', flag='FLAG_HIDE_SOLLIS_LAB_ROWLET'),
    ],
    bgs=[sign(9, 3, 'SollisLab_EventScript_Journal')])

# ---------------- Pokemon Center 1F ----------------
mapjson('HavenIsle_PokemonCenter_1F', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('POKEMON_CENTER_1F'),
    warps=[
        warp(7, 8, M('TIDEMARK_VILLAGE'), 3),
        warp(6, 8, M('TIDEMARK_VILLAGE'), 3),
        warp(1, 6, M('POKEMON_CENTER_2F'), 0, elevation=4),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'HavenIslePokemonCenter_1F_EventScript_Nurse',
            local_id='LOCALID_TIDEMARK_NURSE'),
        obj('OBJ_EVENT_GFX_BOY_1', 10, 5, 'HavenIslePokemonCenter_1F_EventScript_Boy',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
    ])

# ---------------- Pokemon Center 2F ----------------
mapjson('HavenIsle_PokemonCenter_2F', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('POKEMON_CENTER_2F'),
    warps=[
        warp(1, 6, M('POKEMON_CENTER_1F'), 2, elevation=4),
        warp(5, 1, 'MAP_UNION_ROOM', 0, elevation=3),
        warp(9, 1, 'MAP_TRADE_CENTER', 0, elevation=3),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_TEALA', 6, 2, 'Common_EventScript_UnionRoomAttendant', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_TEALA', 2, 2, 'Common_EventScript_WirelessClubAttendant', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_TEALA', 10, 2, 'Common_EventScript_DirectCornerAttendant', rx=1, ry=1),
    ])

# ---------------- Mart ----------------
mapjson('HavenIsle_Mart', 'LAYOUT_MART',
    'MAPSEC_TIDEMARK_VILLAGE', 'MUS_POKE_MART', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('MART'),
    warps=[
        warp(3, 7, M('TIDEMARK_VILLAGE'), 4),
        warp(4, 7, M('TIDEMARK_VILLAGE'), 4),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 1, 3, 'HavenIsleMart_EventScript_Clerk',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_WOMAN_5', 5, 5, 'HavenIsleMart_EventScript_Woman',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
    ])

print('map.json files written')
