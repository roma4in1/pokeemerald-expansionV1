#!/usr/bin/env python3
"""Generate map.json for all 18 Sirocco maps. Mirrors build_ironhold_mapjson.py.

Hard rules encoded here:
- coord_events compare a VAR (VAR_SIROCCO_PROGRESS), never a FLAG constant.
- boat boarding runs through `sign` bg_events on the shore-edge water tiles along
  the hull; the SS_TIDAL object is pure decoration (script 0x0, elevation 1).
- warps never land on a coord trigger tile; arrival triggers sit one tile inward
  from the warp landing.
- gym leaders (Silt, Crag, Miria, Dagan) are talk-initiated overworld NPCs
  (TRAINER_TYPE_NONE) so their scripts can be flag-gated, per the Ironhold pattern.
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

def conn(direction, offset, mapid):
    return {'map': mapid, 'offset': offset, 'direction': direction}

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

M = lambda s: 'MAP_SIROCCO_' + s
VAR = 'VAR_SIROCCO_PROGRESS'

# ============================ DustmouthPort ============================
# Entry from sea (Brigantine). North exit -> DesertRoute1.
mapjson('Sirocco_DustmouthPort', 'LAYOUT_SIROCCO_DUSTMOUTH_PORT',
    'MAPSEC_SIROCCO_DUSTMOUTH_PORT', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN',
    weather='WEATHER_SANDSTORM', mapid=M('DUSTMOUTH_PORT'),
    connections=[conn('up', -1, M('DESERT_ROUTE1'))],
    warps=[
        warp(4, 8, M('DUSTMOUTH_PORT_INN'), 0),       # 0: into the Inn
    ],
    objects=[
        # Dock enforcer - menacing flavor, does not hard-block.
        obj('OBJ_EVENT_GFX_TWIN', 8, 4, 'SiroccoDustmouthPort_EventScript_DockEnforcer',
            local_id='LOCALID_SIROCCO_DOCK_ENFORCER'),
        obj('OBJ_EVENT_GFX_MAN_3', 12, 6, 'SiroccoDustmouthPort_EventScript_DesperateLocal',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'SiroccoDustmouthPort_EventScript_Merchant'),
        obj('OBJ_EVENT_GFX_SAILOR', 14, 10, 'SiroccoDustmouthPort_EventScript_Sailor'),
        # Gilt Claw water-distribution NPC (queue flavor).
        obj('OBJ_EVENT_GFX_MAN_4', 4, 11, 'SiroccoDustmouthPort_EventScript_WaterDistributor',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # The Tennyson (Brigantine), docked offshore. Decoration, script 0x0, elev 1.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_SIROCCO_TENNYSON', elevation=1),
    ],
    coords=[
        # Arrival cutscene. Trigger sits north of the dock head; the player walks
        # onto it after stepping off the boat. VAR == 0 => not yet arrived.
        trigger(9, 11, VAR, 0, 'SiroccoDustmouthPort_EventScript_Arrival'),
        trigger(10, 11, VAR, 0, 'SiroccoDustmouthPort_EventScript_Arrival'),
    ],
    bgs=[
        sign(12, 2, 'SiroccoDustmouthPort_EventScript_NorthSign'),
        sign(6, 11, 'SiroccoDustmouthPort_EventScript_BerthSign'),
        # Boarding signs along the pier edge (hull), per harbor/boat rules.
        sign(11, 13, 'SiroccoDustmouthPort_EventScript_Tennyson'),
        sign(11, 14, 'SiroccoDustmouthPort_EventScript_Tennyson'),
        sign(11, 15, 'SiroccoDustmouthPort_EventScript_Tennyson'),
        sign(10, 16, 'SiroccoDustmouthPort_EventScript_Tennyson'),
        sign(9, 16, 'SiroccoDustmouthPort_EventScript_Tennyson'),
    ])

# ============================ DustmouthPort_Inn (lobby) ============================
mapjson('Sirocco_DustmouthPort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SIROCCO_DUSTMOUTH_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('DUSTMOUTH_PORT_INN'),
    warps=[
        warp(7, 8, M('DUSTMOUTH_PORT'), 0),                 # 0
        warp(6, 8, M('DUSTMOUTH_PORT'), 0),                 # 1
        warp(1, 6, M('DUSTMOUTH_PORT_INN_INTERIOR'), 0, elevation=4),  # 2: up to rooms
    ],
    objects=[
        # Innkeeper doubles as the heal-location respawn NPC.
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SiroccoDustmouthPortInn_EventScript_Innkeeper',
            local_id='LOCALID_SIROCCO_INNKEEPER'),
        # Hidden door to the black market (the script reveals/uses warp on examine).
        obj('OBJ_EVENT_GFX_MAN_2', 11, 5, 'SiroccoDustmouthPortInn_EventScript_BlackMarketDoor',
            local_id='LOCALID_SIROCCO_BLACKMARKET_DOOR', movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ DustmouthPort_Inn_Interior (rooms) ============================
mapjson('Sirocco_DustmouthPort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_SIROCCO_DUSTMOUTH_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('DUSTMOUTH_PORT_INN_INTERIOR'),
    warps=[
        warp(1, 6, M('DUSTMOUTH_PORT_INN'), 2, elevation=4),  # 0: back down
        # Stairs at the far side descend to the hidden black market basement.
        warp(10, 4, M('DUSTMOUTH_PORT_BLACKMARKET'), 0, elevation=4),  # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'SiroccoDustmouthPortInnInterior_EventScript_TravelerA',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 9, 5, 'SiroccoDustmouthPortInnInterior_EventScript_TravelerB',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ============================ DustmouthPort_BlackMarket (basement) ============================
mapjson('Sirocco_DustmouthPort_BlackMarket', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_SIROCCO_DUSTMOUTH_PORT', 'MUS_MT_PYRE', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('DUSTMOUTH_PORT_BLACKMARKET'),
    warps=[warp(10, 3, M('DUSTMOUTH_PORT_INN_INTERIOR'), 1)],  # 0: back up to inn rooms
    objects=[
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 5, 7, 'SiroccoBlackMarket_EventScript_VendorA',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 10, 7, 'SiroccoBlackMarket_EventScript_VendorB',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 15, 7, 'SiroccoBlackMarket_EventScript_VendorC',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_MAN_5', 10, 13, 'SiroccoBlackMarket_EventScript_InfoBroker',
            local_id='LOCALID_SIROCCO_INFO_BROKER', movement='MOVEMENT_TYPE_FACE_UP'),
    ])

# ============================ DesertRoute1 ============================
# south -> Port, north -> Oasis, east -> DexCamp.
mapjson('Sirocco_DesertRoute1', 'LAYOUT_SIROCCO_DESERT_ROUTE_1',
    'MAPSEC_SIROCCO_DESERT_ROUTE_1', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('DESERT_ROUTE1'),
    connections=[
        conn('down', 1, M('DUSTMOUTH_PORT')),
        conn('up', -1, M('MIRADEN_OASIS')),
        conn('right', 18, M('DEX_CAMP')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_4', 8, 12, 'SiroccoDesertRoute1_EventScript_EnforcerRael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_HIKER', 15, 26, 'SiroccoDesertRoute1_EventScript_TravelerYoss',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[
        sign(5, 36, 'SiroccoDesertRoute1_EventScript_DriedBush',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        hidden(5, 35, 'ITEM_RAWST_BERRY', 'FLAG_HIDDEN_ITEM_SIROCCO_BERRY'),
    ])

# ============================ MiradenOasis ============================
# south -> Route1, east -> Route2, north -> GiltClawTerritory (gated).
mapjson('Sirocco_MiradenOasis', 'LAYOUT_SIROCCO_MIRADEN_OASIS',
    'MAPSEC_SIROCCO_MIRADEN_OASIS', 'MUS_LILYCOVE', 'MAP_TYPE_TOWN',
    weather='WEATHER_SUNNY', mapid=M('MIRADEN_OASIS'),
    connections=[
        conn('down', 1, M('DESERT_ROUTE1')),
        conn('right', 11, M('DESERT_ROUTE2')),
        conn('up', -1, M('GILT_CLAW_TERRITORY')),
    ],
    warps=[
        warp(5, 5, M('MIRADEN_OASIS_POKEMON_CENTER'), 0),   # 0  (Gym1 sign area NW)
        warp(22, 5, M('MIRADEN_OASIS_SHOP'), 0),            # 1
    ],
    objects=[
        # Gym 1 leader Silt - converted water warehouse, talk-initiated overworld.
        obj('OBJ_EVENT_GFX_HIKER', 7, 6, 'SiroccoMiradenOasis_EventScript_Silt',
            local_id='LOCALID_SIROCCO_SILT'),
        # Gym 2 leader Crag - excavation site building, talk-initiated overworld.
        obj('OBJ_EVENT_GFX_EXPERT_M', 20, 6, 'SiroccoMiradenOasis_EventScript_Crag',
            local_id='LOCALID_SIROCCO_CRAG'),
        obj('OBJ_EVENT_GFX_LITTLE_BOY', 16, 13, 'SiroccoMiradenOasis_EventScript_Child',
            movement='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_OLD_WOMAN', 9, 16, 'SiroccoMiradenOasis_EventScript_OldWoman',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MAN_4', 18, 8, 'SiroccoMiradenOasis_EventScript_WaterDistributor',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_SCIENTIST_2', 6, 13, 'SiroccoMiradenOasis_EventScript_Scholar',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 13, 6, 'SiroccoMiradenOasis_EventScript_Elder',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # North-exit blocker: enforcer turns the player back until Gym 2 cleared.
        # Stands ON the (13,1) gate tile so the bounce trigger's walk_down at
        # (14,1) can never overlap him (scripted movement ignores collision).
        obj('OBJ_EVENT_GFX_MAN_4', 13, 1, 'SiroccoMiradenOasis_EventScript_NorthBlocker',
            local_id='LOCALID_SIROCCO_NORTH_BLOCKER',
            flag='FLAG_SIROCCO_NORTH_TERRITORY_OPEN'),
    ],
    coords=[
        # North-territory gate as a coord fallback (armed for every progress value
        # before Gym 2 clears it). VAR compare, never a flag.
        trigger(13, 1, VAR, 1, 'SiroccoMiradenOasis_EventScript_NorthGate'),
        trigger(14, 1, VAR, 1, 'SiroccoMiradenOasis_EventScript_NorthGate'),
        trigger(13, 1, VAR, 2, 'SiroccoMiradenOasis_EventScript_NorthGate'),
        trigger(14, 1, VAR, 2, 'SiroccoMiradenOasis_EventScript_NorthGate'),
    ],
    bgs=[
        sign(5, 4, 'SiroccoMiradenOasis_EventScript_Gym1Sign',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(22, 4, 'SiroccoMiradenOasis_EventScript_Gym2Sign',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(5, 16, 'SiroccoMiradenOasis_EventScript_CenterSign'),
        sign(22, 16, 'SiroccoMiradenOasis_EventScript_ShopSign'),
    ])

# ============================ MiradenOasis_PokemonCenter ============================
mapjson('Sirocco_MiradenOasis_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SIROCCO_MIRADEN_OASIS', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('MIRADEN_OASIS_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('MIRADEN_OASIS'), 0),   # 0
        warp(6, 8, M('MIRADEN_OASIS'), 0),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SiroccoMiradenOasisPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_SIROCCO_OASIS_NURSE'),
    ])

# ============================ MiradenOasis_Shop ============================
mapjson('Sirocco_MiradenOasis_Shop', 'LAYOUT_MART',
    'MAPSEC_SIROCCO_MIRADEN_OASIS', 'MUS_POKE_MART', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('MIRADEN_OASIS_SHOP'),
    warps=[
        warp(3, 7, M('MIRADEN_OASIS'), 1),   # 0
        warp(4, 7, M('MIRADEN_OASIS'), 1),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 1, 3, 'SiroccoMiradenOasisShop_EventScript_Clerk',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
    ])

# ============================ DexCamp ============================
# west -> DesertRoute1. Dex meeting (flag-gated). No wild encounters.
mapjson('Sirocco_DexCamp', 'LAYOUT_SIROCCO_DEX_CAMP',
    'MAPSEC_SIROCCO_DESERT_ROUTE_1', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('DEX_CAMP'),
    connections=[conn('left', -6, M('DESERT_ROUTE1'))],
    objects=[
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 8, 6, 'SiroccoDexCamp_EventScript_Dex',
            local_id='LOCALID_SIROCCO_DEX', movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    coords=[
        # First-entry Dex meeting trigger, one tile in from the west connection edge.
        trigger(3, 6, VAR, 1, 'SiroccoDexCamp_EventScript_FirstEntry'),
        trigger(3, 7, VAR, 1, 'SiroccoDexCamp_EventScript_FirstEntry'),
    ],
    bgs=[
        sign(8, 2, 'SiroccoDexCamp_EventScript_ResearchNotes',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ DesertRoute2 ============================
# west -> Oasis, east -> BuriedCity_Exterior. One enforcer gate trainer.
mapjson('Sirocco_DesertRoute2', 'LAYOUT_SIROCCO_DESERT_ROUTE_2',
    'MAPSEC_SIROCCO_DESERT_ROUTE_2', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('DESERT_ROUTE2'),
    connections=[
        conn('left', -11, M('MIRADEN_OASIS')),
        conn('right', 1, M('BURIED_CITY_EXTERIOR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_4', 18, 9, 'SiroccoDesertRoute2_EventScript_EnforcerWex',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3',
            local_id='LOCALID_SIROCCO_ENFORCER_WEX'),
    ])

# ============================ BuriedCity_Exterior ============================
# west -> Route2, down into Interior1. Two grunt trainers + discovery cutscene.
mapjson('Sirocco_BuriedCity_Exterior', 'LAYOUT_SIROCCO_BURIED_CITY_EXTERIOR',
    'MAPSEC_SIROCCO_BURIED_CITY', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('BURIED_CITY_EXTERIOR'),
    connections=[conn('left', -9, M('DESERT_ROUTE2'))],
    warps=[
        warp(11, 5, M('BURIED_CITY_INTERIOR1'), 0),   # 0: down the ancient doorway
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_3', 5, 16, 'SiroccoBuriedCityExterior_EventScript_GruntPell',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 18, 6, 'SiroccoBuriedCityExterior_EventScript_GruntSera',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    coords=[
        # Discovery cutscene on first entry (one tile in from west edge).
        trigger(2, 9, VAR, 1, 'SiroccoBuriedCityExterior_EventScript_Discovery'),
        trigger(2, 10, VAR, 1, 'SiroccoBuriedCityExterior_EventScript_Discovery'),
        trigger(2, 9, VAR, 2, 'SiroccoBuriedCityExterior_EventScript_Discovery'),
        trigger(2, 10, VAR, 2, 'SiroccoBuriedCityExterior_EventScript_Discovery'),
    ],
    bgs=[
        sign(8, 6, 'SiroccoBuriedCityExterior_EventScript_AncientScript',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ BuriedCity_Interior1 (Gym 3, Miria) ============================
mapjson('Sirocco_BuriedCity_Interior1', 'LAYOUT_VICTORY_ROAD_1F',
    'MAPSEC_SIROCCO_BURIED_CITY', 'MUS_GYM', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('BURIED_CITY_INTERIOR1'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(15, 40, M('BURIED_CITY_EXTERIOR'), 0),   # 0: back up to the surface
        warp(39, 5, M('BURIED_CITY_INTERIOR2'), 0),   # 1: deeper (opens after Miria)
    ],
    objects=[
        # Gym 3 leader Miria - talk-initiated (flag-gated script).
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 21, 20, 'SiroccoBuriedCityInterior1_EventScript_Miria',
            local_id='LOCALID_SIROCCO_MIRIA'),
    ])

# ============================ BuriedCity_Interior2 (mural / cipher) ============================
mapjson('Sirocco_BuriedCity_Interior2', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_SIROCCO_BURIED_CITY', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('BURIED_CITY_INTERIOR2'),
    warps=[
        # LAYOUT_SEALED_CHAMBER_OUTER_ROOM: central columns 8-12 are wall on the
        # bottom rows; land the Interior1 return warp on the walkable SE pocket.
        warp(16, 18, M('BURIED_CITY_INTERIOR1'), 1),   # 0: back to Interior1
        warp(10, 2, M('BURIED_CITY_SEAL_CHAMBER'), 0), # 1: deeper to the SealChamber
    ],
    bgs=[
        # Central mural - examine for the Xerath vision + cipher 3. Faced from
        # below (player stands at row 4, walkable, facing the row-3 wall).
        sign(10, 3, 'SiroccoBuriedCityInterior2_EventScript_CentralMural',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(4, 7, 'SiroccoBuriedCityInterior2_EventScript_WallMuralLeft',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(16, 7, 'SiroccoBuriedCityInterior2_EventScript_WallMuralRight',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ BuriedCity_SealChamber ============================
mapjson('Sirocco_BuriedCity_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_SIROCCO_BURIED_CITY', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('BURIED_CITY_SEAL_CHAMBER'),
    warps=[warp(10, 19, M('BURIED_CITY_INTERIOR2'), 1)],  # 0: back out
    objects=[
        # Xerath sealed beneath the stone (decoration, not interactable in main story).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_SIROCCO_XERATH', elevation=1),
        # Gilt Claw apparatus - player disables it, then reinforces the seal.
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'SiroccoSealChamber_EventScript_Apparatus',
            local_id='LOCALID_SIROCCO_APPARATUS'),
    ],
    coords=[
        # First crossing plays the discovery cutscene. Progress 5 = Dagan defeated,
        # chamber unblocked by Miria. VAR compare, never a flag.
        trigger(9, 14, VAR, 5, 'SiroccoSealChamber_EventScript_Discovery'),
        trigger(10, 14, VAR, 5, 'SiroccoSealChamber_EventScript_Discovery'),
        trigger(11, 14, VAR, 5, 'SiroccoSealChamber_EventScript_Discovery'),
    ])

# ============================ GiltClawTerritory ============================
# south -> Oasis, north -> DaganPalace_Exterior. Two elite enforcer trainers.
mapjson('Sirocco_GiltClawTerritory', 'LAYOUT_SIROCCO_GILT_CLAW_TERRITORY',
    'MAPSEC_SIROCCO_DAGAN_PALACE', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('GILT_CLAW_TERRITORY'),
    connections=[
        conn('down', 1, M('MIRADEN_OASIS')),
        conn('up', -1, M('DAGAN_PALACE_EXTERIOR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_4', 8, 8, 'SiroccoGiltClawTerritory_EventScript_EliteVoss',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_3', 12, 14, 'SiroccoGiltClawTerritory_EventScript_EliteMave',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ])

# ============================ DaganPalace_Exterior ============================
# south -> GiltClawTerritory, north door -> Interior1. Two gate guards.
mapjson('Sirocco_DaganPalace_Exterior', 'LAYOUT_SIROCCO_DAGAN_PALACE_EXTERIOR',
    'MAPSEC_SIROCCO_DAGAN_PALACE', 'MUS_ROUTE110', 'MAP_TYPE_ROUTE',
    weather='WEATHER_SANDSTORM', mapid=M('DAGAN_PALACE_EXTERIOR'),
    connections=[conn('down', -1, M('GILT_CLAW_TERRITORY'))],
    warps=[
        warp(8, 0, M('DAGAN_PALACE_INTERIOR1'), 0),   # 0
        warp(9, 0, M('DAGAN_PALACE_INTERIOR1'), 0),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_3', 6, 8, 'SiroccoDaganPalaceExterior_EventScript_GuardThorn',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_4', 11, 8, 'SiroccoDaganPalaceExterior_EventScript_GuardFen',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ])

# ============================ DaganPalace_Interior1 ============================
mapjson('Sirocco_DaganPalace_Interior1', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F',
    'MAPSEC_SIROCCO_DAGAN_PALACE', 'MUS_LILYCOVE', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('DAGAN_PALACE_INTERIOR1'),
    warps=[
        warp(7, 9, M('DAGAN_PALACE_EXTERIOR'), 0),       # 0
        warp(8, 9, M('DAGAN_PALACE_EXTERIOR'), 0),       # 1
        warp(13, 1, M('DAGAN_PALACE_INTERIOR2'), 0),     # 2: deeper to Dagan's hall
    ],
    objects=[
        obj('OBJ_EVENT_GFX_MAN_3', 5, 3, 'SiroccoDaganPalaceInterior1_EventScript_GuardRusk',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 10, 3, 'SiroccoDaganPalaceInterior1_EventScript_GuardLace',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ])

# ============================ DaganPalace_Interior2 (Gym 4, Dagan) ============================
mapjson('Sirocco_DaganPalace_Interior2', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_2F',
    'MAPSEC_SIROCCO_DAGAN_PALACE', 'MUS_GYM', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('DAGAN_PALACE_INTERIOR2'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[warp(13, 1, M('DAGAN_PALACE_INTERIOR1'), 2)],  # 0: back down
    objects=[
        # Dagan Mire - RICH_BOY placeholder. Talk-initiated (flag-gated).
        # Hide flag = DAGAN_ESCAPED: he is gone for good once the escape plays.
        obj('OBJ_EVENT_GFX_RICH_BOY', 8, 5, 'SiroccoDaganPalaceInterior2_EventScript_Dagan',
            local_id='LOCALID_SIROCCO_DAGAN', movement='MOVEMENT_TYPE_FACE_DOWN',
            flag='FLAG_SIROCCO_DAGAN_ESCAPED'),
        # Miria appears ONLY during the escape cutscene (addobject). FLAG_TEMP_1
        # is re-asserted by the map's ON_TRANSITION on every load, so she never
        # spawns on her own. (Hide-flag semantics: flag SET = object hidden.)
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 8, 8, 'SiroccoDaganPalaceInterior2_EventScript_MiriaPost',
            local_id='LOCALID_SIROCCO_MIRIA_POST', flag='FLAG_TEMP_1',
            movement='MOVEMENT_TYPE_FACE_UP'),
    ])

print('Sirocco map.json files written (18 maps)')

# =====================================================================
#  Post-pass: recompute every Sirocco-to-Sirocco connection offset from
#  the actual layout openings so reciprocal connections always align.
#  offset = thisMap_openMin - otherMap_openMin, measured on the matching
#  axis (X for up/down, Y for left/right). Reverse uses the negation.
# =====================================================================
import struct
_L = {l['id']: l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts'] if 'id' in l}
def _grid(lid):
    e = _L[lid]
    data = open(os.path.join(ROOT, e['blockdata_filepath']), 'rb').read()
    w, h = e['width'], e['height']
    return [list(struct.unpack(f'<{w}H', data[y*w*2:(y+1)*w*2])) for y in range(h)], w, h
def _walk(v):
    return ((v >> 10) & 3) == 0 and ((v >> 12) & 15) != 1
def _open_min(lid, direction):
    g, w, h = _grid(lid)
    if direction == 'up':    xs = [x for x in range(w) if _walk(g[0][x])]
    elif direction == 'down':xs = [x for x in range(w) if _walk(g[h-1][x])]
    elif direction == 'left':xs = [y for y in range(h) if _walk(g[y][0])]
    else:                    xs = [y for y in range(h) if _walk(g[y][w-1])]
    return min(xs) if xs else None
_opp = {'up': 'down', 'down': 'up', 'left': 'right', 'right': 'left'}
_dir = os.path.join(ROOT, 'data/maps')
_sirocco = {}
for d in os.listdir(_dir):
    if d.startswith('Sirocco') and os.path.exists(os.path.join(_dir, d, 'map.json')):
        j = json.load(open(os.path.join(_dir, d, 'map.json')))
        _sirocco[j['id']] = (d, j)
for mid, (d, j) in _sirocco.items():
    changed = False
    for c in (j.get('connections') or []):
        other = c['map']
        if other not in _sirocco:
            continue
        a = _open_min(j['layout'], c['direction'])
        b = _open_min(_sirocco[other][1]['layout'], _opp[c['direction']])
        if a is not None and b is not None:
            c['offset'] = a - b
            changed = True
    if changed:
        with open(os.path.join(_dir, d, 'map.json'), 'w') as f:
            json.dump(j, f, indent=2); f.write('\n')
print('Connection offsets recomputed from layout openings')
