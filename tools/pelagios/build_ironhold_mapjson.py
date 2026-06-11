#!/usr/bin/env python3
"""Generate map.json for all 17 Ironhold maps. Mirrors build_mapjson.py (Haven)."""
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

M = lambda s: 'MAP_IRONHOLD_' + s

# ============================ GatemarkPort ============================
mapjson('Ironhold_GatemarkPort', 'LAYOUT_IRONHOLD_GATEMARK_PORT',
    'MAPSEC_IRONHOLD_GATEMARK_PORT', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN',
    mapid=M('GATEMARK_PORT'),
    connections=[conn('up', -2, M('OUTER_DISTRICT'))],
    warps=[
        warp(4, 8, M('GATEMARK_PORT_INN'), 0),       # 0: into Inn
    ],
    objects=[
        # Dock guard blocks entry until Navigator's Log check (arrival script)
        obj('OBJ_EVENT_GFX_MAN_3', 9, 2, 'IronholdGatemarkPort_EventScript_DockGuard',
            local_id='LOCALID_IRONHOLD_DOCK_GUARD'),
        obj('OBJ_EVENT_GFX_SAILOR', 12, 11, 'IronholdGatemarkPort_EventScript_Sailor'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'IronholdGatemarkPort_EventScript_Merchant'),
        # Resistance contact disguised as fisherman (hint after Gym 1)
        obj('OBJ_EVENT_GFX_FISHERMAN', 14, 11, 'IronholdGatemarkPort_EventScript_DisguisedFisherman',
            local_id='LOCALID_IRONHOLD_DISGUISED_FISHERMAN'),
        # The Tennyson, docked east of the pier. Pure decoration (script 0x0)
        # like every vanilla SS_TIDAL - the 96x40 sprite is not reliably
        # interactable via its 1-tile anchor. Boarding runs through the sign
        # bg_events on the pier edge below.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_IRONHOLD_TENNYSON', elevation=1),
    ],
    coords=[
        # Arrival cutscene trigger just north of the dock head.
        # NOTE: coord triggers compare a VAR (not a flag) against var_value -
        # VAR_IRONHOLD_PROGRESS == 0 means "not yet arrived".
        trigger(9, 11, 'VAR_IRONHOLD_PROGRESS', 0, 'IronholdGatemarkPort_EventScript_Arrival'),
        trigger(10, 11, 'VAR_IRONHOLD_PROGRESS', 0, 'IronholdGatemarkPort_EventScript_Arrival'),
    ],
    bgs=[
        sign(12, 2, 'IronholdGatemarkPort_EventScript_GarrisonSign'),
        sign(6, 11, 'IronholdGatemarkPort_EventScript_BerthSign'),
        # Boarding: face the ship from the pier (east edge or south end).
        sign(11, 13, 'IronholdGatemarkPort_EventScript_Tennyson'),
        sign(11, 14, 'IronholdGatemarkPort_EventScript_Tennyson'),
        sign(11, 15, 'IronholdGatemarkPort_EventScript_Tennyson'),
        sign(10, 16, 'IronholdGatemarkPort_EventScript_Tennyson'),
        sign(9, 16, 'IronholdGatemarkPort_EventScript_Tennyson'),
    ])

# ============================ GatemarkPort_Inn (lobby) ============================
mapjson('Ironhold_GatemarkPort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_IRONHOLD_GATEMARK_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('GATEMARK_PORT_INN'),
    warps=[
        warp(7, 8, M('GATEMARK_PORT'), 0),                 # 0
        warp(6, 8, M('GATEMARK_PORT'), 0),                 # 1
        warp(1, 6, M('GATEMARK_PORT_INN_INTERIOR'), 0, elevation=4),  # 2: up to rooms
    ],
    objects=[
        # Innkeeper doubles as healing NPC (heal location respawn target)
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'IronholdGatemarkPortInn_EventScript_Innkeeper',
            local_id='LOCALID_IRONHOLD_INNKEEPER'),
    ])

# ============================ GatemarkPort_Inn_Interior (rooms) ============================
mapjson('Ironhold_GatemarkPort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_IRONHOLD_GATEMARK_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('GATEMARK_PORT_INN_INTERIOR'),
    warps=[warp(1, 6, M('GATEMARK_PORT_INN'), 2, elevation=4)],  # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'IronholdGatemarkPortInnInterior_EventScript_Traveler',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ OuterDistrict ============================
mapjson('Ironhold_OuterDistrict', 'LAYOUT_IRONHOLD_OUTER_DISTRICT',
    'MAPSEC_IRONHOLD_OUTER_DISTRICT', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    mapid=M('OUTER_DISTRICT'),
    connections=[
        conn('down', 2, M('GATEMARK_PORT')),
        conn('up', -2, M('IRONHOLD_CITY')),
        conn('left', -6, M('MOUNTAIN_PASS')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_POKEFAN_M', 9, 8, 'IronholdOuterDistrict_EventScript_OfficerMace',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_4', 16, 11, 'IronholdOuterDistrict_EventScript_WorkerFinn',
            trainer_type='TRAINER_TYPE_NORMAL', sight='2'),
    ],
    coords=[
        # Grapple Hook rubble gate at the west exit. Coord triggers compare a VAR,
        # so the gate is armed for every progress value before the hook handoff
        # (Forge sets VAR_IRONHOLD_PROGRESS=3 together with FLAG_GRAPPLE_HOOK_OBTAINED).
        trigger(2, 9, 'VAR_IRONHOLD_PROGRESS', 0, 'IronholdOuterDistrict_EventScript_Rubble'),
        trigger(2, 9, 'VAR_IRONHOLD_PROGRESS', 1, 'IronholdOuterDistrict_EventScript_Rubble'),
        trigger(2, 9, 'VAR_IRONHOLD_PROGRESS', 2, 'IronholdOuterDistrict_EventScript_Rubble'),
        trigger(2, 10, 'VAR_IRONHOLD_PROGRESS', 0, 'IronholdOuterDistrict_EventScript_Rubble'),
        trigger(2, 10, 'VAR_IRONHOLD_PROGRESS', 1, 'IronholdOuterDistrict_EventScript_Rubble'),
        trigger(2, 10, 'VAR_IRONHOLD_PROGRESS', 2, 'IronholdOuterDistrict_EventScript_Rubble'),
    ],
    bgs=[
        sign(3, 8, 'IronholdOuterDistrict_EventScript_Graffiti',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        # Hidden Antidote tucked in the rubble at the west Grapple-gated exit (rubble at 2,9-2,10)
        hidden(3, 9, 'ITEM_ANTIDOTE', 'FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE'),
    ])

# ============================ IronholdCity ============================
mapjson('Ironhold_IronholdCity', 'LAYOUT_IRONHOLD_IRONHOLD_CITY',
    'MAPSEC_IRONHOLD_CITY', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN',
    mapid=M('IRONHOLD_CITY'),
    connections=[
        conn('down', 2, M('OUTER_DISTRICT')),
    ],
    warps=[
        warp(6, 11, M('IRONHOLD_CITY_ARMORY'), 0),      # 0: Armory (Gym 2)
        warp(20, 11, M('IRONHOLD_CITY_POKEMON_CENTER'), 0),  # 1
        warp(23, 11, M('IRONHOLD_CITY_POKE_MART'), 0),  # 2
        warp(20, 17, M('RESISTANCE_HIDEOUT'), 0),       # 3: contact's house (hidden basement)
    ],
    objects=[
        # Gym 1 - Petra, in the converted town hall at the north end (in-overworld).
        # Leaders are TRAINER_TYPE_NONE (talk-initiated): trainer-see requires the
        # trainerbattle command to be the FIRST script command, which forbids the
        # flag gating their scripts need.
        obj('OBJ_EVENT_GFX_BEAUTY', 6, 5, 'IronholdCity_EventScript_Petra',
            local_id='LOCALID_IRONHOLD_PETRA'),
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 10, 7, 'IronholdCity_EventScript_Elder',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_LITTLE_BOY', 14, 9, 'IronholdCity_EventScript_Child',
            movement='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_MAN_2', 9, 14, 'IronholdCity_EventScript_Citizen',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_HIKER', 17, 6, 'IronholdCity_EventScript_CovenantOfficer'),
        # Covenant soldier patrol pair (set movement scripts handled in scripts.inc)
        obj('OBJ_EVENT_GFX_HIKER', 13, 19, 'IronholdCity_EventScript_SoldierPatrolA',
            movement='MOVEMENT_TYPE_WALK_UP_AND_DOWN', ry=2),
        obj('OBJ_EVENT_GFX_HIKER', 14, 19, 'IronholdCity_EventScript_SoldierPatrolB',
            movement='MOVEMENT_TYPE_WALK_UP_AND_DOWN', ry=2),
        # Covenant soldier posted outside the contact's house (the door at 20,17
        # leads to the ResistanceHideout). He is reassigned (hidden) once Petra
        # falls - FLAG_IRONHOLD_GYM1_CLEAR doubles as his hide flag, opening the
        # hideout per the brief ("accessible after defeating Gym 1").
        obj('OBJ_EVENT_GFX_HIKER', 20, 18, 'IronholdCity_EventScript_HQGuard',
            local_id='LOCALID_IRONHOLD_HQ_GUARD', flag='FLAG_IRONHOLD_GYM1_CLEAR'),
    ],
    bgs=[
        sign(6, 4, 'IronholdCity_EventScript_GymSign'),
        sign(6, 10, 'IronholdCity_EventScript_ArmorySign'),
        sign(20, 10, 'IronholdCity_EventScript_CenterSign'),
        sign(20, 16, 'IronholdCity_EventScript_HQSign'),
    ])

# ============================ Armory (foyer) ============================
mapjson('Ironhold_IronholdCity_Armory', 'LAYOUT_HOUSE1',
    'MAPSEC_IRONHOLD_CITY', 'MUS_POKE_MART', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('IRONHOLD_CITY_ARMORY'),
    warps=[
        warp(3, 8, M('IRONHOLD_CITY'), 0),                    # 0
        warp(4, 8, M('IRONHOLD_CITY'), 0),                    # 1
        warp(8, 1, M('IRONHOLD_CITY_ARMORY_INTERIOR'), 0),    # 2: into the workshop/gym
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_4', 5, 4, 'IronholdArmory_EventScript_Apprentice',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ============================ Armory Interior (Gym 2, Forge) ============================
mapjson('Ironhold_IronholdCity_Armory_Interior', 'LAYOUT_DEWFORD_TOWN_GYM',
    'MAPSEC_IRONHOLD_CITY', 'MUS_GYM', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('IRONHOLD_CITY_ARMORY_INTERIOR'), battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(5, 27, M('IRONHOLD_CITY_ARMORY'), 2),   # 0
        warp(6, 27, M('IRONHOLD_CITY_ARMORY'), 2),   # 1
    ],
    objects=[
        # Talk-initiated (see Petra note): Forge's script is gated on Gym 1.
        obj('OBJ_EVENT_GFX_HIKER', 6, 3, 'IronholdArmoryInterior_EventScript_Forge',
            local_id='LOCALID_IRONHOLD_FORGE'),
    ])

# ============================ IronholdCity PokemonCenter ============================
mapjson('Ironhold_IronholdCity_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_IRONHOLD_CITY', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('IRONHOLD_CITY_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('IRONHOLD_CITY'), 1),   # 0
        warp(6, 8, M('IRONHOLD_CITY'), 1),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'IronholdCityPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_IRONHOLD_CITY_NURSE'),
    ])

# ============================ IronholdCity PokeMart ============================
mapjson('Ironhold_IronholdCity_PokeMart', 'LAYOUT_MART',
    'MAPSEC_IRONHOLD_CITY', 'MUS_POKE_MART', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('IRONHOLD_CITY_POKE_MART'),
    warps=[
        warp(3, 7, M('IRONHOLD_CITY'), 2),   # 0
        warp(4, 7, M('IRONHOLD_CITY'), 2),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 1, 3, 'IronholdMart_EventScript_Clerk',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
    ])

# ============================ Resistance Hideout ============================
mapjson('Ironhold_ResistanceHideout', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_IRONHOLD_CITY', 'MUS_MT_PYRE', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('RESISTANCE_HIDEOUT'),
    warps=[warp(10, 19, M('IRONHOLD_CITY'), 3)],   # 0: back up to the contact's house
    objects=[
        obj('OBJ_EVENT_GFX_YOUNGSTER', 10, 6, 'IronholdResistanceHideout_EventScript_Leader',
            local_id='LOCALID_IRONHOLD_RESISTANCE_LEADER'),
        obj('OBJ_EVENT_GFX_MAN_2', 7, 9, 'IronholdResistanceHideout_EventScript_MemberA',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 13, 9, 'IronholdResistanceHideout_EventScript_MemberB',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ],
    bgs=[sign(10, 4, 'IronholdResistanceHideout_EventScript_MapWall',
              facing='BG_EVENT_PLAYER_FACING_NORTH')])

# ============================ MountainPass ============================
mapjson('Ironhold_MountainPass', 'LAYOUT_IRONHOLD_MOUNTAIN_PASS',
    'MAPSEC_IRONHOLD_MOUNTAIN_PASS', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    mapid=M('MOUNTAIN_PASS'), battle_scene='MAP_BATTLE_SCENE_NORMAL',
    connections=[
        conn('right', 6, M('OUTER_DISTRICT')),
        conn('up', -2, M('SUMMIT_APPROACH')),
    ],
    warps=[warp(10, 18, M('MOUNTAIN_PASS_CAVE'), 0)],  # 0: into the cave
    objects=[
        obj('OBJ_EVENT_GFX_HIKER', 7, 10, 'IronholdMountainPass_EventScript_HikerCrag',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 11, 23, 'IronholdMountainPass_EventScript_SoldierVenn',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[
        sign(8, 22, 'IronholdMountainPass_EventScript_RockPile'),
        # Hidden Iron in the rock pile (sign marks the spot at 8,22)
        hidden(9, 22, 'ITEM_IRON', 'FLAG_HIDDEN_ITEM_IRONHOLD_IRON'),
    ])

# ============================ MountainPass Cave ============================
mapjson('Ironhold_MountainPass_Cave', 'LAYOUT_GRANITE_CAVE_1F',
    'MAPSEC_IRONHOLD_MOUNTAIN_PASS', 'MUS_MT_PYRE', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('MOUNTAIN_PASS_CAVE'),
    battle_scene='MAP_BATTLE_SCENE_NORMAL',
    warps=[
        warp(37, 12, M('MOUNTAIN_PASS'), 0),   # 0: back to MountainPass (south end)
        warp(5, 10, M('SUMMIT_APPROACH'), 0),  # 1: out the far side to SummitApproach
    ],
    objects=[
        obj('OBJ_EVENT_GFX_HIKER', 20, 8, 'IronholdMountainPassCave_EventScript_ScoutHale',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[sign(17, 11, 'IronholdMountainPassCave_EventScript_Inscription',
              facing='BG_EVENT_PLAYER_FACING_NORTH')])

# ============================ SummitApproach ============================
mapjson('Ironhold_SummitApproach', 'LAYOUT_IRONHOLD_SUMMIT_APPROACH',
    'MAPSEC_IRONHOLD_SUMMIT', 'MUS_MT_PYRE', 'MAP_TYPE_ROUTE', weather='WEATHER_SHADE',
    mapid=M('SUMMIT_APPROACH'), battle_scene='MAP_BATTLE_SCENE_NORMAL',
    connections=[
        conn('down', 2, M('MOUNTAIN_PASS')),
        conn('up', 2, M('SUMMIT_FORTRESS_EXTERIOR')),
    ],
    warps=[warp(5, 10, M('MOUNTAIN_PASS_CAVE'), 1)],  # 0: cave far exit lands here (dest warp 1 above)
    objects=[
        obj('OBJ_EVENT_GFX_HIKER', 8, 7, 'IronholdSummitApproach_EventScript_EliteSorn',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 13, 11, 'IronholdSummitApproach_EventScript_Patrol2',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 10, 15, 'IronholdSummitApproach_EventScript_Patrol3',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[sign(11, 17, 'IronholdSummitApproach_EventScript_Vista')])

# ============================ SummitFortress Exterior ============================
mapjson('Ironhold_SummitFortress_Exterior', 'LAYOUT_IRONHOLD_SUMMIT_FORTRESS_EXTERIOR',
    'MAPSEC_IRONHOLD_SUMMIT', 'MUS_MT_PYRE', 'MAP_TYPE_ROUTE', weather='WEATHER_SHADE',
    mapid=M('SUMMIT_FORTRESS_EXTERIOR'), battle_scene='MAP_BATTLE_SCENE_NORMAL',
    connections=[conn('down', -2, M('SUMMIT_APPROACH'))],
    warps=[warp(8, 0, M('SUMMIT_FORTRESS_INTERIOR1'), 0),   # 0
           warp(9, 0, M('SUMMIT_FORTRESS_INTERIOR1'), 0)],  # 1
    objects=[
        obj('OBJ_EVENT_GFX_HIKER', 6, 8, 'IronholdSummitFortressExterior_EventScript_EliteVael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 11, 8, 'IronholdSummitFortressExterior_EventScript_EliteGuard2',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        # Commander Sever appears here after both guards defeated (hidden until then)
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 9, 4, 'IronholdSummitFortressExterior_EventScript_Sever',
            local_id='LOCALID_IRONHOLD_SEVER_GATE', flag='FLAG_HIDE_IRONHOLD_SEVER_GATE'),
    ])

# ============================ SummitFortress Interior1 (Gym 3, Rook) ============================
mapjson('Ironhold_SummitFortress_Interior1', 'LAYOUT_VICTORY_ROAD_1F',
    'MAPSEC_IRONHOLD_SUMMIT', 'MUS_GYM', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SUMMIT_FORTRESS_INTERIOR1'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(15, 40, M('SUMMIT_FORTRESS_EXTERIOR'), 0),   # 0: back out the gate
        warp(39, 5, M('SUMMIT_FORTRESS_INTERIOR2'), 0),   # 1: up to command level (after Rook)
    ],
    objects=[
        # Talk-initiated (see Petra note).
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 21, 20, 'IronholdFortressInterior1_EventScript_Rook',
            local_id='LOCALID_IRONHOLD_ROOK'),
        obj('OBJ_EVENT_GFX_HIKER', 15, 30, 'IronholdFortressInterior1_EventScript_StationedA',
            trainer_type='TRAINER_TYPE_NORMAL', sight='4'),
        obj('OBJ_EVENT_GFX_HIKER', 30, 25, 'IronholdFortressInterior1_EventScript_StationedB',
            trainer_type='TRAINER_TYPE_NORMAL', sight='4'),
    ])

# ============================ SummitFortress Interior2 (Gym 4, Sever) ============================
mapjson('Ironhold_SummitFortress_Interior2', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_IRONHOLD_SUMMIT', 'MUS_GYM', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SUMMIT_FORTRESS_INTERIOR2'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(10, 21, M('SUMMIT_FORTRESS_INTERIOR1'), 1),   # 0: back down to Interior1
        warp(10, 2, M('SUMMIT_FORTRESS_SEAL_CHAMBER'), 0), # 1: into the SealChamber (after Sever)
    ],
    objects=[
        # Talk-initiated (see Petra note): Sever's script is gated on Gym 3.
        # Hide flag FLAG_IRONHOLD_RESOLVED: per the brief, Sever does not appear
        # anywhere on Ironhold after the island is resolved (he has gone quiet).
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 10, 8, 'IronholdFortressInterior2_EventScript_Sever',
            local_id='LOCALID_IRONHOLD_SEVER', flag='FLAG_IRONHOLD_RESOLVED'),
    ],
    bgs=[sign(10, 5, 'IronholdFortressInterior2_EventScript_RegionMap',
              facing='BG_EVENT_PLAYER_FACING_NORTH')])

# ============================ SummitFortress SealChamber ============================
mapjson('Ironhold_SummitFortress_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_IRONHOLD_SUMMIT', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SUMMIT_FORTRESS_SEAL_CHAMBER'),
    warps=[warp(10, 19, M('SUMMIT_FORTRESS_INTERIOR2'), 1)],  # 0: back out
    objects=[
        # Ferrath sealed in the chamber (not interactable during main story)
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_IRONHOLD_FERRATH'),
        # Siphon apparatus the player interacts with to reinforce the seal
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'IronholdSealChamber_EventScript_Apparatus',
            local_id='LOCALID_IRONHOLD_APPARATUS'),
    ],
    coords=[
        # Coord triggers compare a VAR. Progress 3/4 = Sever not yet defeated:
        # a Covenant barrier turns the player back. Progress 5 = chamber open,
        # first crossing plays the discovery cutscene (which sets progress 6).
        trigger(9, 14, 'VAR_IRONHOLD_PROGRESS', 3, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(10, 14, 'VAR_IRONHOLD_PROGRESS', 3, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(11, 14, 'VAR_IRONHOLD_PROGRESS', 3, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(9, 14, 'VAR_IRONHOLD_PROGRESS', 4, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(10, 14, 'VAR_IRONHOLD_PROGRESS', 4, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(11, 14, 'VAR_IRONHOLD_PROGRESS', 4, 'IronholdSealChamber_EventScript_DoorSealed'),
        trigger(9, 14, 'VAR_IRONHOLD_PROGRESS', 5, 'IronholdSealChamber_EventScript_Discovery'),
        trigger(10, 14, 'VAR_IRONHOLD_PROGRESS', 5, 'IronholdSealChamber_EventScript_Discovery'),
        trigger(11, 14, 'VAR_IRONHOLD_PROGRESS', 5, 'IronholdSealChamber_EventScript_Discovery'),
    ])

print('Ironhold map.json files written')
