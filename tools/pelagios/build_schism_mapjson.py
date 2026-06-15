#!/usr/bin/env python3
"""Generate map.json for all 21 Schism maps. Mirrors build_emberveil_mapjson.py.

Schism is a SPLIT island with TWO ports (dual-arrival). North = ice
(gTileset_PelagiosIce, WEATHER_SNOW), South = poison (gTileset_PelagiosPoison,
WEATHER_RAIN), the middle Scar + ScarRuins = grey General/Cave (WEATHER_NONE).

Composed outdoor layouts come from build_schism.py. Interiors reuse vanilla
LAYOUT_*:
  Inn lobby           = POKEMON_CENTER_1F   (innkeeper = nurse-gfx heal NPC)
  Inn rooms           = POKEMON_CENTER_2F
  City PokeCenter     = POKEMON_CENTER_1F
  Barracks (foyer)    = HOUSE2
  Barracks Interior   = SHOAL_CAVE_LOW_TIDE_ICE_ROOM  (real MB_ICE sliding puzzle)
  Laboratory (foyer)  = HOUSE2
  Laboratory Interior = AQUA_HIDEOUT_1F   (industrial two-level facility, toxic vats)
  SealChamber N/S     = SEALED_CHAMBER_INNER_ROOM

Harbor rule: the SS_TIDAL (Tennyson, Galleon) is pure decoration (script 0x0);
boarding runs through sign bg_events on the pier -> *_EventScript_Tennyson.

Scar gates: faction soldiers block the FrozenTundra->TheScar and ToxicSwamp->TheScar
connection openings (TODO scripts gated by FLAG_EIRA_SCAR_PASS / FLAG_DRENN_SCAR_PASS
and VAR_SCHISM_CEASEFIRE_PROGRESS by the script-writer).
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

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_SNOW',
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

M = lambda s: 'MAP_SCHISM_' + s
MUS_ICE = 'MUS_LILYCOVE'        # placeholder cold-city music
MUS_POISON = 'MUS_LILYCOVE'     # placeholder grimy-city music
MUS_ROUTE_N = 'MUS_ROUTE110'
MUS_ROUTE_S = 'MUS_ROUTE110'
MUS_SCAR = 'MUS_MT_PYRE'        # eerie placeholder
MUS_CAVE = 'MUS_SEALED_CHAMBER'

# =====================================================================
#  NORTH / ICE HALF (WEATHER_SNOW)
# =====================================================================

# ============================ FrostmarkPort ============================
mapjson('Schism_FrostmarkPort', 'LAYOUT_SCHISM_FROSTMARK_PORT',
    'MAPSEC_SCHISM_FROSTMARK_PORT', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN', weather='WEATHER_SNOW',
    mapid=M('FROSTMARK_PORT'),
    connections=[conn('up', 0, M('FROZEN_TUNDRA'))],
    warps=[warp(4, 8, M('FROSTMARK_PORT_INN'), 0)],   # 0: into Inn
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 8, 5, 'SchismFrostmarkPort_EventScript_IceSoldier',
            local_id='LOCALID_SCHISM_FROSTMARK_SOLDIER'),
        obj('OBJ_EVENT_GFX_SAILOR', 12, 11, 'SchismFrostmarkPort_EventScript_Sailor'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'SchismFrostmarkPort_EventScript_Merchant'),
        # The Tennyson (Galleon). Pure decoration; boarding via pier bg_events.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_SCHISM_FROSTMARK_TENNYSON', elevation=1),
    ],
    bgs=[
        sign(2, 2, 'SchismFrostmarkPort_EventScript_TerritorySign'),
        sign(6, 11, 'SchismFrostmarkPort_EventScript_BerthSign'),
        # Boarding from the pier edge / south end.
        sign(11, 13, 'SchismFrostmarkPort_EventScript_Tennyson'),
        sign(11, 14, 'SchismFrostmarkPort_EventScript_Tennyson'),
        sign(11, 15, 'SchismFrostmarkPort_EventScript_Tennyson'),
        sign(8, 13, 'SchismFrostmarkPort_EventScript_Tennyson'),
        sign(8, 14, 'SchismFrostmarkPort_EventScript_Tennyson'),
    ])

# ============================ FrostmarkPort_Inn (lobby) ============================
mapjson('Schism_FrostmarkPort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SCHISM_FROSTMARK_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('FROSTMARK_PORT_INN'),
    warps=[
        warp(7, 8, M('FROSTMARK_PORT'), 0),                              # 0
        warp(6, 8, M('FROSTMARK_PORT'), 0),                              # 1
        warp(1, 6, M('FROSTMARK_PORT_INN_INTERIOR'), 0, elevation=4),    # 2: up to rooms
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SchismFrostmarkPortInn_EventScript_Innkeeper',
            local_id='LOCALID_SCHISM_FROSTMARK_INNKEEPER'),
    ])

# ============================ FrostmarkPort_Inn_Interior (rooms) ============================
mapjson('Schism_FrostmarkPort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_SCHISM_FROSTMARK_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('FROSTMARK_PORT_INN_INTERIOR'),
    warps=[warp(1, 6, M('FROSTMARK_PORT_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'SchismFrostmarkPortInnInterior_EventScript_Traveler',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ FrozenTundra ============================
mapjson('Schism_FrozenTundra', 'LAYOUT_SCHISM_FROZEN_TUNDRA',
    'MAPSEC_SCHISM_FROZEN_TUNDRA', MUS_ROUTE_N, 'MAP_TYPE_ROUTE', weather='WEATHER_SNOW',
    mapid=M('FROZEN_TUNDRA'),
    connections=[
        conn('down', 0, M('FROSTMARK_PORT')),
        conn('up', -4, M('ICE_CITY')),
        conn('right', 6, M('THE_SCAR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 5, 12, 'SchismFrozenTundra_EventScript_ScoutHeln',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 13, 22, 'SchismFrozenTundra_EventScript_ScoutVera',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        # Scar-access blocker: ice faction soldier barring the east opening (rows 15-16)
        # until FLAG_EIRA_SCAR_PASS. Stands one tile inside the opening.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 17, 15, 'SchismFrozenTundra_EventScript_ScarGuard',
            movement='MOVEMENT_TYPE_FACE_RIGHT',
            local_id='LOCALID_SCHISM_TUNDRA_SCAR_GUARD'),
    ],
    # Scar access is gated by the ScarGuard object's TODO script (script-writer
    # toggles its hide/position on FLAG_EIRA_SCAR_PASS). No flag-as-var coord
    # trigger here: coord-event var-compare runs VarGet, which returns the flag id
    # itself for a flag - it can never fire (CLAUDE.md Known Issues).
    bgs=[
        hidden(4, 7, 'ITEM_NEVER_MELT_ICE', 'FLAG_HIDDEN_ITEM_SCHISM_1'),
    ])

# ============================ IceCity ============================
mapjson('Schism_IceCity', 'LAYOUT_SCHISM_ICE_CITY',
    'MAPSEC_SCHISM_ICE_CITY', MUS_ICE, 'MAP_TYPE_TOWN', weather='WEATHER_SNOW',
    mapid=M('ICE_CITY'),
    connections=[
        conn('down', 4, M('FROZEN_TUNDRA')),
        conn('right', 2, M('ICE_CAVE')),
    ],
    warps=[
        warp(6, 5, M('ICE_CITY_BARRACKS'), 0),         # 0: Barracks (Gym 1)
        warp(20, 11, M('ICE_CITY_POKEMON_CENTER'), 0), # 1
    ],
    objects=[
        # Eira (Gym 2) - command post NPC, appears post-Gym1 (hide flag GYM1_CLEAR
        # inverts: she's hidden until Sleet falls). Talk-initiated leader.
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 14, 6, 'SchismIceCity_EventScript_Eira',
            local_id='LOCALID_SCHISM_EIRA'),
        obj('OBJ_EVENT_GFX_MAN_2', 10, 10, 'SchismIceCity_EventScript_SoldierRemembers',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_LITTLE_BOY', 18, 14, 'SchismIceCity_EventScript_SalutingChild',
            movement='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 9, 16, 'SchismIceCity_EventScript_Soldier2',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
    ],
    bgs=[
        sign(6, 4, 'SchismIceCity_EventScript_BarracksSign'),
        sign(20, 10, 'SchismIceCity_EventScript_CenterSign'),
        sign(14, 4, 'SchismIceCity_EventScript_CommandSign'),
    ])

# ============================ IceCity PokemonCenter ============================
mapjson('Schism_IceCity_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SCHISM_ICE_CITY', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('ICE_CITY_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('ICE_CITY'), 1),   # 0
        warp(6, 8, M('ICE_CITY'), 1),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SchismIceCityPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_SCHISM_ICE_CITY_NURSE'),
    ])

# ============================ IceCity Barracks (foyer) ============================
mapjson('Schism_IceCity_Barracks', 'LAYOUT_HOUSE2',
    'MAPSEC_SCHISM_ICE_CITY', MUS_ICE, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('ICE_CITY_BARRACKS'),
    warps=[
        warp(3, 7, M('ICE_CITY'), 0),                        # 0: back to city (doormat)
        warp(4, 7, M('ICE_CITY'), 0),                        # 1
        warp(8, 2, M('ICE_CITY_BARRACKS_INTERIOR'), 0),      # 2: into the training hall
    ],
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 7, 3, 'SchismIceCityBarracks_EventScript_Quartermaster',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ============================ IceCity Barracks Interior (ice sliding puzzle) ============================
# Reuses SHOAL_CAVE_LOW_TIDE_ICE_ROOM: a vanilla 4x4-style ice sliding puzzle with
# real MB_ICE / cracked-ice tiles. Sleet waits past the ice at the far (north) end.
mapjson('Schism_IceCity_Barracks_Interior', 'LAYOUT_SHOAL_CAVE_LOW_TIDE_ICE_ROOM',
    'MAPSEC_SCHISM_ICE_CITY', MUS_ICE, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('ICE_CITY_BARRACKS_INTERIOR'), battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(12, 28, M('ICE_CITY_BARRACKS'), 2),   # 0: back to the foyer (bottom entrance, walkable)
        warp(13, 28, M('ICE_CITY_BARRACKS'), 2),   # 1
    ],
    objects=[
        # Gym 1 - Sleet, at the far (top-left) end past the ice puzzle. Talk-initiated.
        obj('OBJ_EVENT_GFX_YOUNGSTER', 3, 2, 'SchismIceCityBarracksInterior_EventScript_Sleet',
            local_id='LOCALID_SCHISM_SLEET'),
    ],
    bgs=[
        # TODO (script-writer): the ice slide is engine MB_ICE behavior; no extra
        # trigger needed for sliding. A puzzle-reset/hint sign can go here if desired.
        sign(12, 24, 'SchismIceCityBarracksInterior_EventScript_PuzzleSign'),
    ])

# ============================ IceCave ============================
mapjson('Schism_IceCave', 'LAYOUT_SCHISM_ICE_CAVE',
    'MAPSEC_SCHISM_ICE_CAVE', MUS_CAVE, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_SNOW',
    mapid=M('ICE_CAVE'), show_name=True,
    connections=[
        conn('left', -2, M('ICE_CITY')),
    ],
    warps=[
        warp(18, 9, M('SEAL_CHAMBER_NORTH'), 0),   # 0: deeper to Glacith's seal
        warp(18, 10, M('SEAL_CHAMBER_NORTH'), 0),  # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 10, 12, 'SchismIceCave_EventScript_GuardRael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
    ],
    bgs=[
        sign(10, 3, 'SchismIceCave_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ SealChamber_North (Glacith) ============================
mapjson('Schism_SealChamber_North', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_SCHISM_ICE_CAVE', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SEAL_CHAMBER_NORTH'),
    warps=[warp(10, 19, M('ICE_CAVE'), 0)],   # 0: back up to the cave
    objects=[
        # Glacith, sealed (not interactable in main story).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_SCHISM_GLACITH'),
        # Crude faction machinery the player interacts with to reinforce the seal.
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'SchismSealChamberNorth_EventScript_Apparatus',
            local_id='LOCALID_SCHISM_APPARATUS_NORTH'),
    ],
    coords=[
        trigger(9, 14, 'VAR_SCHISM_PROGRESS', 3, 'SchismSealChamberNorth_EventScript_Discovery'),
        trigger(10, 14, 'VAR_SCHISM_PROGRESS', 3, 'SchismSealChamberNorth_EventScript_Discovery'),
        trigger(11, 14, 'VAR_SCHISM_PROGRESS', 3, 'SchismSealChamberNorth_EventScript_Discovery'),
    ])

# =====================================================================
#  THE SCAR + RUINS (WEATHER_NONE, grey)
# =====================================================================

# ============================ TheScar ============================
mapjson('Schism_TheScar', 'LAYOUT_SCHISM_THE_SCAR',
    'MAPSEC_SCHISM_THE_SCAR', MUS_SCAR, 'MAP_TYPE_ROUTE', weather='WEATHER_NONE',
    mapid=M('THE_SCAR'),
    connections=[
        conn('left', -6, M('FROZEN_TUNDRA')),
        conn('right', -6, M('TOXIC_SWAMP')),
    ],
    warps=[
        warp(13, 18, M('SCAR_RUINS'), 0),   # 0: down into the deeper ruins
        warp(14, 18, M('SCAR_RUINS'), 0),   # 1
    ],
    objects=[
        # Ceasefire-meeting actors (hidden until the ceasefire scene; the script-writer
        # reveals them). Placeholder positions on opposite sides of the central ruin.
        obj('OBJ_EVENT_GFX_COOLTRAINER_F', 5, 9, 'SchismTheScar_EventScript_EiraCeasefire',
            flag='FLAG_SCHISM_CEASEFIRE', local_id='LOCALID_SCHISM_SCAR_EIRA'),
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 22, 9, 'SchismTheScar_EventScript_DrennCeasefire',
            flag='FLAG_SCHISM_CEASEFIRE', local_id='LOCALID_SCHISM_SCAR_DRENN'),
    ],
    coords=[
        # Ceasefire cutscene fires when both leaders are willing (the script-writer
        # guards on FLAG_EIRA_CEASEFIRE_WILLING && FLAG_DRENN_CEASEFIRE_WILLING).
        trigger(13, 11, 'VAR_SCHISM_CEASEFIRE_PROGRESS', 2, 'SchismTheScar_EventScript_CeasefireScene'),
        trigger(14, 11, 'VAR_SCHISM_CEASEFIRE_PROGRESS', 2, 'SchismTheScar_EventScript_CeasefireScene'),
    ],
    bgs=[
        # Central unified ruins (examine -> vision -> FLAG_SCAR_RUINS_FOUND).
        sign(13, 9, 'SchismTheScar_EventScript_CentralRuins',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        # Dorne's footprints in the ash (flavor lore).
        sign(20, 12, 'SchismTheScar_EventScript_Footprints'),
        # Hidden item in the ash.
        hidden(8, 14, 'ITEM_MAX_REVIVE', 'FLAG_HIDDEN_ITEM_SCHISM_2'),
    ])

# ============================ ScarRuins ============================
mapjson('Schism_ScarRuins', 'LAYOUT_SCHISM_SCAR_RUINS',
    'MAPSEC_SCHISM_THE_SCAR', MUS_CAVE, 'MAP_TYPE_UNDERGROUND', weather='WEATHER_NONE',
    mapid=M('SCAR_RUINS'), show_name=True,
    warps=[
        warp(9, 20, M('THE_SCAR'), 0),    # 0: back up to the Scar
        warp(10, 20, M('THE_SCAR'), 0),   # 1
    ],
    objects=[],
    bgs=[
        sign(7, 7, 'SchismScarRuins_EventScript_GlacithMural',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(12, 7, 'SchismScarRuins_EventScript_ToxaraMural',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        # Central cipher pedestal (cipher 5 - FLAG_SCHISM_CIPHER_FOUND).
        sign(9, 11, 'SchismScarRuins_EventScript_CipherPedestal',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  SOUTH / POISON HALF (WEATHER_RAIN)
# =====================================================================

# ============================ VenomquayPort ============================
mapjson('Schism_VenomquayPort', 'LAYOUT_SCHISM_VENOMQUAY_PORT',
    'MAPSEC_SCHISM_VENOMQUAY_PORT', 'MUS_SLATEPORT', 'MAP_TYPE_TOWN', weather='WEATHER_RAIN',
    mapid=M('VENOMQUAY_PORT'),
    connections=[conn('up', 0, M('TOXIC_SWAMP'))],
    warps=[warp(4, 8, M('VENOMQUAY_PORT_INN'), 0)],   # 0: into Inn
    objects=[
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 8, 5, 'SchismVenomquayPort_EventScript_PoisonGuard',
            local_id='LOCALID_SCHISM_VENOMQUAY_GUARD'),
        obj('OBJ_EVENT_GFX_FISHERMAN', 12, 11, 'SchismVenomquayPort_EventScript_Fisherman'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 6, 9, 'SchismVenomquayPort_EventScript_Merchant'),
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_SCHISM_VENOMQUAY_TENNYSON', elevation=1),
    ],
    bgs=[
        sign(2, 2, 'SchismVenomquayPort_EventScript_TerritorySign'),
        sign(6, 11, 'SchismVenomquayPort_EventScript_BerthSign'),
        sign(11, 13, 'SchismVenomquayPort_EventScript_Tennyson'),
        sign(11, 14, 'SchismVenomquayPort_EventScript_Tennyson'),
        sign(11, 15, 'SchismVenomquayPort_EventScript_Tennyson'),
        sign(8, 13, 'SchismVenomquayPort_EventScript_Tennyson'),
        sign(8, 14, 'SchismVenomquayPort_EventScript_Tennyson'),
    ])

# ============================ VenomquayPort_Inn (lobby) ============================
mapjson('Schism_VenomquayPort_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SCHISM_VENOMQUAY_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('VENOMQUAY_PORT_INN'),
    warps=[
        warp(7, 8, M('VENOMQUAY_PORT'), 0),                              # 0
        warp(6, 8, M('VENOMQUAY_PORT'), 0),                              # 1
        warp(1, 6, M('VENOMQUAY_PORT_INN_INTERIOR'), 0, elevation=4),    # 2: up to rooms
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SchismVenomquayPortInn_EventScript_Innkeeper',
            local_id='LOCALID_SCHISM_VENOMQUAY_INNKEEPER'),
    ])

# ============================ VenomquayPort_Inn_Interior (rooms) ============================
mapjson('Schism_VenomquayPort_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_SCHISM_VENOMQUAY_PORT', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('VENOMQUAY_PORT_INN_INTERIOR'),
    warps=[warp(1, 6, M('VENOMQUAY_PORT_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_MAN_1', 6, 4, 'SchismVenomquayPortInnInterior_EventScript_Traveler',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ ToxicSwamp ============================
mapjson('Schism_ToxicSwamp', 'LAYOUT_SCHISM_TOXIC_SWAMP',
    'MAPSEC_SCHISM_TOXIC_SWAMP', MUS_ROUTE_S, 'MAP_TYPE_ROUTE', weather='WEATHER_RAIN',
    mapid=M('TOXIC_SWAMP'),
    connections=[
        conn('down', 0, M('VENOMQUAY_PORT')),
        conn('up', -4, M('POISON_CITY')),
        conn('left', 6, M('THE_SCAR')),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_FISHERMAN', 5, 12, 'SchismToxicSwamp_EventScript_WaderOsk',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        obj('OBJ_EVENT_GFX_MAN_4', 13, 22, 'SchismToxicSwamp_EventScript_WaderFen',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        # Scar-access blocker: poison faction guard barring the west opening (rows 15-16)
        # until FLAG_DRENN_SCAR_PASS.
        obj('OBJ_EVENT_GFX_MAN_4', 2, 15, 'SchismToxicSwamp_EventScript_ScarGuard',
            movement='MOVEMENT_TYPE_FACE_LEFT',
            local_id='LOCALID_SCHISM_SWAMP_SCAR_GUARD'),
    ])

# ============================ PoisonCity ============================
mapjson('Schism_PoisonCity', 'LAYOUT_SCHISM_POISON_CITY',
    'MAPSEC_SCHISM_POISON_CITY', MUS_POISON, 'MAP_TYPE_TOWN', weather='WEATHER_RAIN',
    mapid=M('POISON_CITY'),
    connections=[
        conn('down', 4, M('TOXIC_SWAMP')),
    ],
    warps=[
        warp(6, 5, M('POISON_CITY_LABORATORY'), 0),         # 0: Laboratory (Gym 3/4)
        warp(20, 11, M('POISON_CITY_POKEMON_CENTER'), 0),   # 1
        warp(1, 11, M('SEAL_CHAMBER_SOUTH'), 0),            # 2: west poison cave mouth -> Toxara's seal
    ],
    objects=[
        obj('OBJ_EVENT_GFX_OLD_MAN_1', 10, 10, 'SchismPoisonCity_EventScript_Painter',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MAN_4', 18, 14, 'SchismPoisonCity_EventScript_FactionMember',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_WOMAN_2', 16, 8, 'SchismPoisonCity_EventScript_FactionMember2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        sign(6, 4, 'SchismPoisonCity_EventScript_LabSign'),
        sign(20, 10, 'SchismPoisonCity_EventScript_CenterSign'),
        sign(3, 11, 'SchismPoisonCity_EventScript_CaveSign'),
    ])

# ============================ PoisonCity PokemonCenter ============================
mapjson('Schism_PoisonCity_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_SCHISM_POISON_CITY', 'MUS_POKE_CENTER', 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('POISON_CITY_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('POISON_CITY'), 1),   # 0
        warp(6, 8, M('POISON_CITY'), 1),   # 1
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'SchismPoisonCityPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_SCHISM_POISON_CITY_NURSE'),
    ])

# ============================ PoisonCity Laboratory (foyer) ============================
mapjson('Schism_PoisonCity_Laboratory', 'LAYOUT_HOUSE2',
    'MAPSEC_SCHISM_POISON_CITY', MUS_POISON, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('POISON_CITY_LABORATORY'),
    warps=[
        warp(3, 7, M('POISON_CITY'), 0),                          # 0: back to city (doormat)
        warp(4, 7, M('POISON_CITY'), 0),                          # 1
        warp(8, 2, M('POISON_CITY_LABORATORY_INTERIOR'), 0),      # 2: into the lab proper
    ],
    objects=[
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 7, 3, 'SchismPoisonCityLaboratory_EventScript_LabAssistant',
            movement='MOVEMENT_TYPE_FACE_LEFT'),
    ])

# ============================ PoisonCity Laboratory Interior (two-level: vats + upper office) ============================
# Reuses AQUA_HIDEOUT_1F: an industrial facility with internal stairs and water
# platforms - water reads as toxic vats, the railed upper area as Drenn's office.
# Murk (Gym 3) is on the lower vat floor; Drenn (Gym 4) at the upper office.
mapjson('Schism_PoisonCity_Laboratory_Interior', 'LAYOUT_AQUA_HIDEOUT_1F',
    'MAPSEC_SCHISM_POISON_CITY', MUS_POISON, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('POISON_CITY_LABORATORY_INTERIOR'), battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        # AQUA_HIDEOUT_1F entry is the bottom corridor (row 27, x 12-14).
        warp(12, 27, M('POISON_CITY_LABORATORY'), 2),   # 0: back to the foyer
        warp(13, 27, M('POISON_CITY_LABORATORY'), 2),   # 1
    ],
    objects=[
        # Gym 3 - Murk, lower vat floor (elev1 platforms over toxic water).
        # Talk-initiated leader.
        obj('OBJ_EVENT_GFX_COOLTRAINER_M', 10, 21, 'SchismPoisonCityLaboratoryInterior_EventScript_Murk',
            elevation=1, local_id='LOCALID_SCHISM_MURK'),
        # Researcher Siv (sight trainer, lower floor).
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 13, 17, 'SchismPoisonCityLaboratoryInterior_EventScript_ResearcherSiv',
            elevation=1, trainer_type='TRAINER_TYPE_NORMAL', sight='3'),
        # Gym 4 - Drenn, upper office (elev3, north - visible across the floor
        # pre-Gym3). Talk-initiated leader.
        obj('OBJ_EVENT_GFX_SCIENTIST_1', 12, 3, 'SchismPoisonCityLaboratoryInterior_EventScript_Drenn',
            local_id='LOCALID_SCHISM_DRENN'),
    ],
    bgs=[
        sign(9, 22, 'SchismPoisonCityLaboratoryInterior_EventScript_VatSign'),
    ])

# ============================ SealChamber_South (Toxara) ============================
mapjson('Schism_SealChamber_South', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_SCHISM_POISON_CITY', 'MUS_SEALED_CHAMBER', 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=False, mapid=M('SEAL_CHAMBER_SOUTH'),
    warps=[warp(10, 19, M('POISON_CITY'), 2)],   # 0: back to PoisonCity cave mouth
    objects=[
        # Toxara, sealed (not interactable in main story).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_SCHISM_TOXARA'),
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'SchismSealChamberSouth_EventScript_Apparatus',
            local_id='LOCALID_SCHISM_APPARATUS_SOUTH'),
    ],
    coords=[
        trigger(9, 14, 'VAR_SCHISM_PROGRESS', 5, 'SchismSealChamberSouth_EventScript_Discovery'),
        trigger(10, 14, 'VAR_SCHISM_PROGRESS', 5, 'SchismSealChamberSouth_EventScript_Discovery'),
        trigger(11, 14, 'VAR_SCHISM_PROGRESS', 5, 'SchismSealChamberSouth_EventScript_Discovery'),
    ])

print('Schism map.json files written (21 maps)')
