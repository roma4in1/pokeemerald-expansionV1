#!/usr/bin/env python3
"""Generate map.json for all 15 Emberveil maps. Mirrors build_ironhold_mapjson.py.

Tilesets: outdoor = General/Lavaridge (composed layouts from build_emberveil.py).
Interiors reuse vanilla LAYOUT_*: Inn lobby = POKEMON_CENTER_1F, Inn rooms =
POKEMON_CENTER_2F, City PokeCenter = POKEMON_CENTER_1F, Temple Hall foyer = HOUSE2,
Temple Hall interior (grand ceremonial space) = SEALED_CHAMBER_OUTER_ROOM,
SealChamber = SEALED_CHAMBER_INNER_ROOM.

Weather: SUNNY outdoors, DROUGHT on VolcanoAscent/Summit, NONE indoors + SealChamber.
Lava-Boots obstacle corridors on LavaRoute2 and CalderaApproach use VAR_EMBERVEIL_PROGRESS
coord triggers (TODO scripts wired by the script-writer), mirroring Ironhold's rubble gate.
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

def hidden(x, y, item, flag, quantity=1, elevation=3):
    return {'type': 'hidden_item', 'x': x, 'y': y, 'elevation': elevation,
            'item': item, 'flag': flag, 'quantity': quantity, 'underfoot': False}

def trigger(x, y, var, value, script, elevation=3):
    return {'type': 'trigger', 'x': x, 'y': y, 'elevation': elevation,
            'var': var, 'var_value': str(value), 'script': script}

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_SUNNY',
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

M = lambda s: 'MAP_EMBERVEIL_' + s
P = 'MUS_LILYCOVE'        # placeholder town/port music (volcanic island)
MUS_ROUTE = 'MUS_ROUTE110'

# ============================ AshmouthPort ============================
mapjson('Emberveil_AshmouthPort', 'LAYOUT_EMBERVEIL_ASHMOUTH_PORT',
    'MAPSEC_EMBERVEIL_ASHMOUTH_PORT', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN',
    mapid=M('ASHMOUTH_PORT'),
    connections=[conn('up', 0, M('LAVA_ROUTE1'))],
    warps=[
        warp(4, 8, M('ASHMOUTH_PORT_INN'), 0),    # 0: into Inn
    ],
    objects=[
        obj('OBJ_EVENT_GFX_WOMAN_3', 8, 5, 'EmberveilAshmouthPort_EventScript_CultGreeter',
            local_id='LOCALID_EMBERVEIL_CULT_GREETER'),
        obj('OBJ_EVENT_GFX_SAILOR', 12, 11, 'EmberveilAshmouthPort_EventScript_Sailor'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'EmberveilAshmouthPort_EventScript_Merchant'),
        # The Tennyson, docked. Pure decoration (script 0x0) like every vanilla
        # SS_TIDAL - boarding runs through the pier bg_events below.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_EMBERVEIL_TENNYSON', elevation=1),
    ],
    # Arrival cutscene runs as an ON_FRAME script (VAR_EMBERVEIL_PROGRESS == 0)
    # in scripts.inc - coord triggers at the dock head were bypassable (the
    # whole west half of the map is walkable), and the future boat-transition
    # warp position is unknown. ON_FRAME fires exactly once on first map load.
    coords=[],
    bgs=[
        sign(12, 2, 'EmberveilAshmouthPort_EventScript_PortSign'),
        sign(6, 11, 'EmberveilAshmouthPort_EventScript_BerthSign'),
        # Boarding from the pier edge / south end.
        sign(11, 13, 'EmberveilAshmouthPort_EventScript_Tennyson'),
        sign(11, 14, 'EmberveilAshmouthPort_EventScript_Tennyson'),
        sign(11, 15, 'EmberveilAshmouthPort_EventScript_Tennyson'),
        sign(10, 16, 'EmberveilAshmouthPort_EventScript_Tennyson'),
        sign(9, 16, 'EmberveilAshmouthPort_EventScript_Tennyson'),
    ])

# ============================ AshmouthPort_Inn (lobby) ============================
mapjson('Emberveil_AshmouthPort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_EMBERVEIL_ASHMOUTH_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('ASHMOUTH_PORT_INN'),
    warps=[
        warp(7, 8, M('ASHMOUTH_PORT'), 0),                          # 0
        warp(6, 8, M('ASHMOUTH_PORT'), 0),                          # 1
        warp(1, 6, M('ASHMOUTH_PORT_INN_INTERIOR'), 0, elevation=4),  # 2: up to rooms
    ],
    objects=[
        # Innkeeper doubles as the island heal NPC (respawn target).
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'EmberveilAshmouthPortInn_EventScript_Innkeeper',
            local_id='LOCALID_EMBERVEIL_INNKEEPER'),
    ])

# ============================ AshmouthPort_Inn_Interior (rooms) ============================
mapjson('Emberveil_AshmouthPort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_EMBERVEIL_ASHMOUTH_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('ASHMOUTH_PORT_INN_INTERIOR'),
    warps=[warp(1, 6, M('ASHMOUTH_PORT_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'EmberveilAshmouthPortInnInterior_EventScript_TravelerBeauty',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 9, 5, 'EmberveilAshmouthPortInnInterior_EventScript_TravelerLeaving',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ LavaRoute1 ============================
mapjson('Emberveil_LavaRoute1', 'LAYOUT_EMBERVEIL_LAVA_ROUTE1',
    'MAPSEC_EMBERVEIL_LAVA_ROUTE1', MUS_ROUTE, 'MAP_TYPE_ROUTE',
    mapid=M('LAVA_ROUTE1'),
    connections=[
        conn('down', 0, M('ASHMOUTH_PORT')),
        conn('up', -2, M('ASH_FIELDS')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_YOUNGSTER', 5, 10, 'EmberveilLavaRoute1_EventScript_InitiateSera',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_YOUNGSTER', 13, 16, 'EmberveilLavaRoute1_EventScript_InitiateBren',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        # Two Rawst Berry trees (berry tree ids 90/91, reused from Haven setup).
        obj('OBJ_EVENT_GFX_BERRY_TREE', 8, 5, 'BerryTreeScript',
            movement='MOVEMENT_TYPE_BERRY_TREE_GROWTH', sight='90',
            local_id='LOCALID_EMBERVEIL_BERRY_TREE_1'),
        obj('OBJ_EVENT_GFX_BERRY_TREE', 15, 20, 'BerryTreeScript',
            movement='MOVEMENT_TYPE_BERRY_TREE_GROWTH', sight='91',
            local_id='LOCALID_EMBERVEIL_BERRY_TREE_2'),
    ],
    bgs=[
        # Two hidden Rawst Berries (flags in the hidden-items range).
        hidden(3, 6, 'ITEM_RAWST_BERRY', 'FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1'),
        hidden(16, 21, 'ITEM_RAWST_BERRY', 'FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY2'),
    ])

# ============================ AshFields ============================
mapjson('Emberveil_AshFields', 'LAYOUT_EMBERVEIL_ASH_FIELDS',
    'MAPSEC_EMBERVEIL_ASH_FIELDS', MUS_ROUTE, 'MAP_TYPE_ROUTE',
    mapid=M('ASH_FIELDS'),
    connections=[
        conn('down', 2, M('LAVA_ROUTE1')),
        conn('up', -2, M('CINDERHOLD_CITY')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_WOMAN_2', 12, 10, 'EmberveilAshFields_EventScript_DevoteeMira',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[
        sign(7, 13, 'EmberveilAshFields_EventScript_DeadTree1',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(18, 6, 'EmberveilAshFields_EventScript_DeadTree2',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ CinderholdCity ============================
mapjson('Emberveil_CinderholdCity', 'LAYOUT_EMBERVEIL_CINDERHOLD_CITY',
    'MAPSEC_EMBERVEIL_CINDERHOLD_CITY', 'MUS_LILYCOVE', 'MAP_TYPE_TOWN',
    mapid=M('CINDERHOLD_CITY'),
    connections=[
        conn('down', 2, M('ASH_FIELDS')),
        conn('right', 3, M('LAVA_ROUTE2')),
    ],
    warps=[
        warp(6, 5, M('CINDERHOLD_CITY_TEMPLE_HALL'), 0),       # 0: Temple Hall (lore) - north
        warp(20, 11, M('CINDERHOLD_CITY_POKEMON_CENTER'), 0),  # 1
        # Gym 1 (Cinder) gatehouse at the south entrance - in-overworld NPC, no map.
        # Gym 2 (Slag) lava works - in-overworld NPC near east, no separate map.
    ],
    objects=[
        # Gym 1 - Cinder (post-Gym1 he stands here in the city; pre-Gym1 his
        # gatehouse battle is also handled by this talk-initiated NPC at the
        # south gate). Talk-initiated leader (TRAINER_TYPE_NONE).
        obj('OBJ_EVENT_GFX_YOUNGSTER', 14, 20, 'EmberveilCinderholdCity_EventScript_Cinder',
            local_id='LOCALID_EMBERVEIL_CINDER'),
        # Gym 2 - Slag near the lava works (east side). Talk-initiated leader.
        obj('OBJ_EVENT_GFX_HIKER', 21, 14, 'EmberveilCinderholdCity_EventScript_Slag',
            local_id='LOCALID_EMBERVEIL_SLAG'),
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 10, 8, 'EmberveilCinderholdCity_EventScript_Historian',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 9, 14, 'EmberveilCinderholdCity_EventScript_DoubtingNpc',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_LITTLE_BOY', 16, 9, 'EmberveilCinderholdCity_EventScript_Child',
            movement='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_MAN_2', 11, 18, 'EmberveilCinderholdCity_EventScript_CultMember',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
    ],
    coords=[
        # East exit to LavaRoute2 (caldera path) blocked until Gym 2 cleared.
        # VAR_EMBERVEIL_PROGRESS: 1=arrived, 2=Gym1, 3=Gym2 (+Lava Boots). Block at 0,1,2
        # (0 is defensive - covers a hypothetical arrival-scene skip).
        trigger(26, 11, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
        trigger(26, 12, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
        trigger(26, 11, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
        trigger(26, 12, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
        trigger(26, 11, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
        trigger(26, 12, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilCinderholdCity_EventScript_CalderaGate'),
    ],
    bgs=[
        sign(6, 4, 'EmberveilCinderholdCity_EventScript_GymSign'),
        sign(6, 10, 'EmberveilCinderholdCity_EventScript_TempleSign'),
        sign(20, 10, 'EmberveilCinderholdCity_EventScript_CenterSign'),
        sign(21, 16, 'EmberveilCinderholdCity_EventScript_LavaWorksSign'),
    ])

# ============================ CinderholdCity PokemonCenter ============================
mapjson('Emberveil_CinderholdCity_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_EMBERVEIL_CINDERHOLD_CITY', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('CINDERHOLD_CITY_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('CINDERHOLD_CITY'), 1),   # 0
        warp(6, 8, M('CINDERHOLD_CITY'), 1),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'EmberveilCinderholdCityPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_EMBERVEIL_CITY_NURSE'),
    ])

# ============================ Temple Hall (foyer/exterior) ============================
mapjson('Emberveil_CinderholdCity_TempleHall', 'LAYOUT_HOUSE2',
    'MAPSEC_EMBERVEIL_CINDERHOLD_CITY', 'MUS_LILYCOVE_MUSEUM', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('CINDERHOLD_CITY_TEMPLE_HALL'),
    warps=[
        warp(3, 7, M('CINDERHOLD_CITY'), 0),                            # 0: back to city (doormat)
        warp(4, 7, M('CINDERHOLD_CITY'), 0),                            # 1
        warp(8, 2, M('CINDERHOLD_CITY_TEMPLE_HALL_INTERIOR'), 0),       # 2: up into the grand hall
    ],
    objects=[
        obj('OBJ_EVENT_GFX_WOMAN_3', 7, 3, 'EmberveilTempleHall_EventScript_Attendant',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ============================ Temple Hall Interior (grand ceremonial space) ============================
mapjson('Emberveil_CinderholdCity_TempleHall_Interior', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_EMBERVEIL_CINDERHOLD_CITY', 'MUS_MT_PYRE', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('CINDERHOLD_CITY_TEMPLE_HALL_INTERIOR'),
    warps=[
        warp(10, 2, M('CINDERHOLD_CITY_TEMPLE_HALL'), 2),   # 0: back down to the foyer (north door)
    ],
    objects=[
        # High Ember Solace, here until FLAG_EMBERVEIL_GYM2_CLEAR moves her to the summit.
        # SEALED_CHAMBER_OUTER_ROOM walkable rows: 3-4 wide; place all on row 4.
        obj('OBJ_EVENT_GFX_WOMAN_5', 10, 4, 'EmberveilTempleHallInterior_EventScript_Solace',
            local_id='LOCALID_EMBERVEIL_SOLACE_TEMPLE', flag='FLAG_EMBERVEIL_GYM2_CLEAR'),
        obj('OBJ_EVENT_GFX_MAN_2', 6, 4, 'EmberveilTempleHallInterior_EventScript_Worshipper1',
            movement='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 14, 4, 'EmberveilTempleHallInterior_EventScript_Worshipper2',
            movement='MOVEMENT_TYPE_FACE_UP'),
    ],
    bgs=[
        # Central flame on the north wall; examined from the walkable tile below it.
        sign(10, 3, 'EmberveilTempleHallInterior_EventScript_CentralFlame',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ LavaRoute2 ============================
mapjson('Emberveil_LavaRoute2', 'LAYOUT_EMBERVEIL_LAVA_ROUTE2',
    'MAPSEC_EMBERVEIL_LAVA_ROUTE2', MUS_ROUTE, 'MAP_TYPE_ROUTE',
    mapid=M('LAVA_ROUTE2'),
    connections=[
        conn('left', -3, M('CINDERHOLD_CITY')),
        conn('right', -2, M('CALDERA_APPROACH')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_2', 5, 8, 'EmberveilLavaRoute2_EventScript_AcolyteRem',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_4', 16, 5, 'EmberveilLavaRoute2_EventScript_AcolyteVoss',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 18, 12, 'EmberveilLavaRoute2_EventScript_AcolyteTane',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    coords=[
        # Lava-Boots gate: the lava wall mid-route leaves a corridor at rows 8-9.
        # Block crossing until FLAG_LAVA_BOOTS_OBTAINED (progress 3 = Gym2+Boots).
        # Coord triggers compare VAR_EMBERVEIL_PROGRESS; armed for progress 0,1,2
        # (0/1 defensive - the city gate normally stops the player first).
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilLavaRoute2_EventScript_LavaGate'),
        trigger(10, 9, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilLavaRoute2_EventScript_LavaGate'),
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilLavaRoute2_EventScript_LavaGate'),
        trigger(10, 9, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilLavaRoute2_EventScript_LavaGate'),
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilLavaRoute2_EventScript_LavaGate'),
        trigger(10, 9, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilLavaRoute2_EventScript_LavaGate'),
    ])

# ============================ CalderaApproach ============================
mapjson('Emberveil_CalderaApproach', 'LAYOUT_EMBERVEIL_CALDERA_APPROACH',
    'MAPSEC_EMBERVEIL_CALDERA', 'MUS_MT_PYRE', 'MAP_TYPE_ROUTE',
    mapid=M('CALDERA_APPROACH'),
    connections=[
        conn('left', 2, M('LAVA_ROUTE2')),
    ],
    warps=[
        warp(10, 1, M('CALDERA_RUINS'), 0),   # 0: up into the ruins (after Gym 3 / Lava Boots)
    ],
    objects=[
        # Gym 3 - Vex, stationed to bar caldera entry. Talk-initiated leader.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 10, 7, 'EmberveilCalderaApproach_EventScript_Vex',
            local_id='LOCALID_EMBERVEIL_VEX'),
    ],
    coords=[
        # The wall at rows 9-10 leaves a single corridor at x=10-11; any northbound
        # path crosses row 8 at x=10 or x=11, so these two tiles gate the whole
        # north half (ruins stairway).
        # Progress 0,1,2: Lava-Boots heat gate (0/1 defensive).
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        trigger(11, 8, 'VAR_EMBERVEIL_PROGRESS', 0, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        trigger(11, 8, 'VAR_EMBERVEIL_PROGRESS', 1, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        trigger(11, 8, 'VAR_EMBERVEIL_PROGRESS', 2, 'EmberveilCalderaApproach_EventScript_LavaGate'),
        # Progress 3 (boots obtained, Vex undefeated): Vex intercepts - the player
        # cannot slip past him through the x=11 column to the ruins warp.
        trigger(10, 8, 'VAR_EMBERVEIL_PROGRESS', 3, 'EmberveilCalderaApproach_EventScript_VexIntercept'),
        trigger(11, 8, 'VAR_EMBERVEIL_PROGRESS', 3, 'EmberveilCalderaApproach_EventScript_VexIntercept'),
    ],
    bgs=[
        sign(7, 13, 'EmberveilCalderaApproach_EventScript_HeatSign'),
    ])

# ============================ CalderaRuins ============================
mapjson('Emberveil_CalderaRuins', 'LAYOUT_EMBERVEIL_CALDERA_RUINS',
    'MAPSEC_EMBERVEIL_CALDERA', 'MUS_MT_PYRE', 'MAP_TYPE_ROUTE',
    mapid=M('CALDERA_RUINS'),
    warps=[
        warp(9, 22, M('CALDERA_APPROACH'), 0),    # 0: back down to the approach
        warp(9, 1, M('VOLCANO_ASCENT'), 0),       # 1: up to the volcano ascent
        warp(10, 1, M('VOLCANO_ASCENT'), 0),      # 2
    ],
    objects=[],
    bgs=[
        # Path-B panel sequence (examine 1 -> 2 -> 3; the script-writer uses a local
        # step counter). Panel 3 is the Warden's notes alcove (Path B unlock).
        sign(7, 7, 'EmberveilCalderaRuins_EventScript_Panel1',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(12, 7, 'EmberveilCalderaRuins_EventScript_Panel2',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(9, 4, 'EmberveilCalderaRuins_EventScript_Panel3',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ VolcanoAscent ============================
mapjson('Emberveil_VolcanoAscent', 'LAYOUT_EMBERVEIL_VOLCANO_ASCENT',
    'MAPSEC_EMBERVEIL_VOLCANO', 'MUS_MT_PYRE', 'MAP_TYPE_ROUTE', weather='WEATHER_DROUGHT',
    mapid=M('VOLCANO_ASCENT'),
    connections=[
        conn('up', -1, M('VOLCANO_SUMMIT')),
    ],
    warps=[
        warp(8, 29, M('CALDERA_RUINS'), 1),    # 0: down to the ruins
        warp(9, 29, M('CALDERA_RUINS'), 1),    # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 6, 10, 'EmberveilVolcanoAscent_EventScript_DiscipleHorne',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_2', 11, 18, 'EmberveilVolcanoAscent_EventScript_DiscipleCael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ])

# ============================ VolcanoSummit ============================
mapjson('Emberveil_VolcanoSummit', 'LAYOUT_EMBERVEIL_VOLCANO_SUMMIT',
    'MAPSEC_EMBERVEIL_VOLCANO', 'MUS_MT_CHIMNEY', 'MAP_TYPE_ROUTE',
    weather='WEATHER_DROUGHT', mapid=M('VOLCANO_SUMMIT'),
    connections=[
        conn('down', 1, M('VOLCANO_ASCENT')),
    ],
    warps=[
        warp(9, 5, M('SEAL_CHAMBER'), 0),     # 0: down into the seal chamber (after resolution)
        warp(10, 5, M('SEAL_CHAMBER'), 0),    # 1
    ],
    objects=[
        # High Ember Solace performing the ritual on the platform (appears after Gym 2).
        obj('OBJ_EVENT_GFX_WOMAN_5', 10, 6, 'EmberveilVolcanoSummit_EventScript_Solace',
            local_id='LOCALID_EMBERVEIL_SOLACE_SUMMIT'),
    ],
    coords=[
        # Pre-battle scene fires at the ritual platform's ONLY entrance - the
        # two-tile gap at (9,3)/(10,3) in the platform's north wall. (The old
        # row-8 triggers sat on a fully open row and could be walked around.)
        # progress 4 = Gym 3 cleared, Solace confrontation not yet done.
        trigger(9, 3, 'VAR_EMBERVEIL_PROGRESS', 4, 'EmberveilVolcanoSummit_EventScript_SolaceScene'),
        trigger(10, 3, 'VAR_EMBERVEIL_PROGRESS', 4, 'EmberveilVolcanoSummit_EventScript_SolaceScene'),
    ])

# ============================ SealChamber ============================
mapjson('Emberveil_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_EMBERVEIL_VOLCANO', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SEAL_CHAMBER'),
    warps=[warp(10, 19, M('VOLCANO_SUMMIT'), 0)],   # 0: back up to the summit
    objects=[
        # Pyrath, sealed in the chamber (not interactable during main story).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_EMBERVEIL_PYRATH'),
        # Seal apparatus the player interacts with to reinforce the seal.
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'EmberveilSealChamber_EventScript_Apparatus',
            local_id='LOCALID_EMBERVEIL_APPARATUS'),
    ],
    coords=[
        # First crossing at progress 5 (Solace resolved) plays the discovery cutscene.
        trigger(9, 14, 'VAR_EMBERVEIL_PROGRESS', 5, 'EmberveilSealChamber_EventScript_Discovery'),
        trigger(10, 14, 'VAR_EMBERVEIL_PROGRESS', 5, 'EmberveilSealChamber_EventScript_Discovery'),
        trigger(11, 14, 'VAR_EMBERVEIL_PROGRESS', 5, 'EmberveilSealChamber_EventScript_Discovery'),
    ])

print('Emberveil map.json files written')
