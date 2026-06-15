#!/usr/bin/env python3
"""Generate map.json for all 13 Primalis maps. Mirrors build_gildhaven_mapjson.py.

Primalis = the Beast Island. Grass/Dragon. The OLDEST island - an ancient jungle that
grew around the sealed legendary Verdath over millennia. A Zoan guardian community
(Elder Mako) lives here; earning Mako's trust is the island's mechanic. The Lens
reappears at the inn. ITEM_BEAST_WHISTLE (Cut/Sweet Scent replacement) is obtained
here, post-Gym4.

Tilesets: outdoor maps pair gTileset_General (primary) + gTileset_Fortree (secondary)
- Fortree is the vanilla treehouse/jungle-canopy tileset, the closest dense-jungle
match. Interiors reuse vanilla LAYOUT_* wholesale (zero new binary):
  Inn / PokeCenter        : POKEMON_CENTER_1F / _2F
  ElderHall / _Interior   : MOSSDEEP_CITY_SPACE_CENTER_1F / _2F (a grand multi-room
                            hall + upper chamber for the oral-history scene; Gildhaven
                            VaneManor / Sirocco DaganPalace precedent)
  Heartwood_SealChamber   : SEALED_CHAMBER_INNER_ROOM (the seal room every island uses)

Weather: WEATHER_FOG_HORIZONTAL on all outdoor jungle maps (the brief's "dense fog";
note WEATHER_FOGGY from the task brief does NOT exist - 6=FOG_HORIZONTAL, 9=FOG_DIAGONAL
unused, 22=FOG aggregate; FOG_HORIZONTAL is the real fog overlay). TheHeartwood +
SealChamber = WEATHER_NONE (per the user spec). Indoors = WEATHER_NONE.

BEAST-WHISTLE OBSTACLES (impassable OVERGROW tiles baked into the layouts):
  JungleRoute1: an OPTIONAL NE reward nook sealed by undergrowth (main path stays open;
    the Whistle is post-Gym4, so the early route must not gate main progress).
  JungleRoute2: the NORTH EXIT (Heartwood approach) is sealed - correct, since the
    player reaches AncientRuinsCamp/TheHeartwood only after the Whistle. Gym 3 (Thorn)
    is met SOUTH of the seal and stays reachable.
  Each seal gets TODO coord triggers the script-writer wires to the field effect.

Harbor rule: SS_TIDAL (Tennyson, Galleon) is pure decoration (script 0x0); boarding via
sign bg_events on the dock -> *_EventScript_Tennyson, which sets
VAR_TEMP_1 = PELAGIOS_ISLAND_PRIMALIS then goto Pelagios_EventScript_BoardTennyson.

SealChamber pattern mirrors Ironhold/Schism/Thalvern/Gildhaven: Verdath (SS_TIDAL
decoration) at (10,5), seal apparatus CABLE_CAR at (10,8), discovery triggers row 14
(progress 5), warp back at (10,19).

All EventScript labels are TODO stubs filled by build_primalis_scripts_stubs.py.
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

M = lambda s: 'MAP_PRIMALIS_' + s
FOG = 'WEATHER_FOG_HORIZONTAL'
NONE = 'WEATHER_NONE'

MUS_PORT   = 'MUS_SLATEPORT'
MUS_JUNGLE = 'MUS_ROUTE119'
MUS_VILLAGE= 'MUS_FORTREE'
MUS_HALL   = 'MUS_PETALBURG'
MUS_RUINS  = 'MUS_ROUTE120'
MUS_HEART  = 'MUS_SAFARI_ZONE'
MUS_SEAL   = 'MUS_SEALED_CHAMBER'
MUS_CENTER = 'MUS_POKE_CENTER'

# sprite placeholders (no Zoan/shark/tribal gfx exist; documented in CLAUDE.md)
GFX_LENS   = 'OBJ_EVENT_GFX_SCIENTIST_1'     # The Lens (researcher)
GFX_FERN   = 'OBJ_EVENT_GFX_LASS'            # Fern, young Zoan guardian (Gym 1, Grass)
GFX_SCALE  = 'OBJ_EVENT_GFX_MAN_5'           # Scale, mid-rank guardian (Gym 2, Dragon)
GFX_THORN  = 'OBJ_EVENT_GFX_HIKER'           # Thorn, route guardian (Gym 3)
GFX_MAKO   = 'OBJ_EVENT_GFX_HIKER'           # Elder Mako, part-shark elder (Gym 4)
GFX_ZOAN   = 'OBJ_EVENT_GFX_CAMPER'          # Zoan guardian trainers
GFX_ZOANF  = 'OBJ_EVENT_GFX_PICNICKER'       # Zoan guardian trainers (F)
GFX_VILLAGER='OBJ_EVENT_GFX_WOMAN_5'         # village NPCs

# =====================================================================
#  VerdantLanding (20x18) — Galleon arrival, jungle-edge landing
# =====================================================================
mapjson('Primalis_VerdantLanding', 'LAYOUT_PRIMALIS_VERDANT_LANDING',
    'MAPSEC_PRIMALIS_VERDANT_LANDING', MUS_PORT, 'MAP_TYPE_TOWN', weather=FOG,
    mapid=M('VERDANT_LANDING'),
    connections=[conn('up', 0, M('JUNGLE_ROUTE1'))],
    warps=[warp(5, 4, M('VERDANT_LANDING_INN'), 0)],   # 0: into the Inn
    objects=[
        # Greeter (a wary Zoan villager meeting the boat).
        obj(GFX_VILLAGER, 7, 11, 'PrimalisVerdantLanding_EventScript_Greeter',
            local_id='LOCALID_PRIMALIS_LANDING_GREETER'),
        # The Tennyson (Galleon). Pure decoration; boarding via dock bg_events.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 12, 15, '0x0',
            local_id='LOCALID_PRIMALIS_TENNYSON', elevation=1,
            movement='MOVEMENT_TYPE_NONE'),
    ],
    coords=[
        # Arrival cutscene: fires walking north off the dock at progress 0.
        trigger(9, 12, 'VAR_PRIMALIS_PROGRESS', 0, 'PrimalisVerdantLanding_EventScript_Arrival'),
        trigger(10, 12, 'VAR_PRIMALIS_PROGRESS', 0, 'PrimalisVerdantLanding_EventScript_Arrival'),
    ],
    bgs=[
        # Boarding the Tennyson: sign bg_events on the dock tiles (never the ship obj).
        sign(9, 15, 'PrimalisVerdantLanding_EventScript_Tennyson'),
        sign(10, 15, 'PrimalisVerdantLanding_EventScript_Tennyson'),
        sign(9, 14, 'PrimalisVerdantLanding_EventScript_Tennyson'),
        sign(10, 14, 'PrimalisVerdantLanding_EventScript_Tennyson'),
    ])

# ---- VerdantLanding_Inn (POKEMON_CENTER_1F) - heal + the Lens ----
mapjson('Primalis_VerdantLanding_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_PRIMALIS_VERDANT_LANDING', MUS_CENTER, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('VERDANT_LANDING_INN'),
    warps=[
        warp(7, 8, M('VERDANT_LANDING'), 0),                              # 0
        warp(6, 8, M('VERDANT_LANDING'), 0),                              # 1
        warp(1, 6, M('VERDANT_LANDING_INN_INTERIOR'), 0, elevation=4),    # 2: up
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 1, 'PrimalisVerdantLandingInn_EventScript_Innkeeper',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_PRIMALIS_VERDANT_INNKEEPER'),
        # The Lens - reappears here, gives context (brief beat 2).
        obj(GFX_LENS, 4, 4, 'PrimalisVerdantLandingInn_EventScript_Lens',
            local_id='LOCALID_PRIMALIS_LENS'),
    ])

mapjson('Primalis_VerdantLanding_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_PRIMALIS_VERDANT_LANDING', MUS_CENTER, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('VERDANT_LANDING_INN_INTERIOR'),
    warps=[warp(1, 6, M('VERDANT_LANDING_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj(GFX_VILLAGER, 8, 5, 'PrimalisVerdantLandingInnInterior_EventScript_Lodger'),
    ])

# =====================================================================
#  JungleRoute1 (20x32) — dense jungle, optional Beast-Whistle nook
# =====================================================================
mapjson('Primalis_JungleRoute1', 'LAYOUT_PRIMALIS_JUNGLE_ROUTE_1',
    'MAPSEC_PRIMALIS_JUNGLE_ROUTE_1', MUS_JUNGLE, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('JUNGLE_ROUTE1'),
    connections=[
        conn('down', 0, M('VERDANT_LANDING')),
        conn('up', -3, M('JUNGLE_INTERIOR')),
    ],
    objects=[
        obj(GFX_ZOAN, 6, 24, 'PrimalisJungleRoute1_EventScript_Zoan1',
            trainer_type='TRAINER_TYPE_NORMAL', sight='4', movement='MOVEMENT_TYPE_FACE_UP',
            local_id='LOCALID_PRIMALIS_ZOAN1'),
    ],
    coords=[
        # Beast-Whistle nook seal (optional NE pocket). TODO field-effect clear.
        trigger(16, 7, 'VAR_PRIMALIS_PROGRESS', 6, 'PrimalisJungleRoute1_EventScript_BeastGate'),
        trigger(17, 7, 'VAR_PRIMALIS_PROGRESS', 6, 'PrimalisJungleRoute1_EventScript_BeastGate'),
        trigger(18, 7, 'VAR_PRIMALIS_PROGRESS', 6, 'PrimalisJungleRoute1_EventScript_BeastGate'),
    ])

# =====================================================================
#  JungleInterior (26x24) — deep jungle hub with a river
# =====================================================================
mapjson('Primalis_JungleInterior', 'LAYOUT_PRIMALIS_JUNGLE_INTERIOR',
    'MAPSEC_PRIMALIS_JUNGLE_INTERIOR', MUS_JUNGLE, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('JUNGLE_INTERIOR'),
    connections=[
        conn('down', 3, M('JUNGLE_ROUTE1')),
        conn('up', -1, M('ZOAN_VILLAGE')),
    ],
    objects=[
        obj(GFX_ZOANF, 7, 6, 'PrimalisJungleInterior_EventScript_Zoan2',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_PRIMALIS_ZOAN2'),
    ],
    bgs=[
        sign(13, 9, 'PrimalisJungleInterior_EventScript_RiverSign',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  ZoanVillage (28x24) — guardian community; Fern (Gym1) + Scale (Gym2)
# =====================================================================
mapjson('Primalis_ZoanVillage', 'LAYOUT_PRIMALIS_ZOAN_VILLAGE',
    'MAPSEC_PRIMALIS_ZOAN_VILLAGE', MUS_VILLAGE, 'MAP_TYPE_TOWN', weather=FOG,
    mapid=M('ZOAN_VILLAGE'),
    connections=[
        conn('down', 1, M('JUNGLE_INTERIOR')),
        conn('right', -3, M('JUNGLE_ROUTE2')),
    ],
    warps=[
        warp(10, 7, M('ZOAN_VILLAGE_ELDER_HALL'), 0),       # 0: ElderHall
        warp(19, 7, M('ZOAN_VILLAGE_POKEMON_CENTER'), 0),   # 1: PokeCenter
    ],
    objects=[
        # Fern - Gym 1 (Grass), at the training-area edge. Talk-initiated.
        obj(GFX_FERN, 6, 16, 'PrimalisZoanVillage_EventScript_Fern',
            local_id='LOCALID_PRIMALIS_FERN'),
        # Scale - Gym 2 (Dragon), village center. Talk-initiated.
        obj(GFX_SCALE, 14, 13, 'PrimalisZoanVillage_EventScript_Scale',
            local_id='LOCALID_PRIMALIS_SCALE'),
        # Zoan guardian trainers (sight).
        obj(GFX_ZOAN, 22, 11, 'PrimalisZoanVillage_EventScript_Zoan3',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_LEFT',
            local_id='LOCALID_PRIMALIS_ZOAN3'),
        obj(GFX_ZOANF, 5, 11, 'PrimalisZoanVillage_EventScript_Zoan4',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_RIGHT',
            local_id='LOCALID_PRIMALIS_ZOAN4'),
        # Village flavor NPCs.
        obj(GFX_VILLAGER, 12, 18, 'PrimalisZoanVillage_EventScript_Villager',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=2, ry=2),
        obj('OBJ_EVENT_GFX_BOY_1', 17, 16, 'PrimalisZoanVillage_EventScript_Child',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        sign(10, 8, 'PrimalisZoanVillage_EventScript_ElderHallSign'),
        sign(19, 8, 'PrimalisZoanVillage_EventScript_CenterSign'),
    ])

mapjson('Primalis_ZoanVillage_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_PRIMALIS_ZOAN_VILLAGE', MUS_CENTER, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('ZOAN_VILLAGE_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('ZOAN_VILLAGE'), 1),
        warp(6, 8, M('ZOAN_VILLAGE'), 1),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 1, 'PrimalisZoanVillagePokemonCenter_EventScript_Nurse',
            local_id='LOCALID_PRIMALIS_VILLAGE_NURSE'),
    ])

# ---- ZoanVillage_ElderHall (foyer) + _Interior (oral-history chamber) ----
mapjson('Primalis_ZoanVillage_ElderHall', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F',
    'MAPSEC_PRIMALIS_ZOAN_VILLAGE', MUS_HALL, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('ZOAN_VILLAGE_ELDER_HALL'),
    warps=[
        warp(7, 9, M('ZOAN_VILLAGE'), 0),                                   # 0: out
        warp(8, 9, M('ZOAN_VILLAGE'), 0),                                   # 1
        warp(13, 1, M('ZOAN_VILLAGE_ELDER_HALL_INTERIOR'), 0),              # 2: in to chamber
    ],
    objects=[
        obj(GFX_VILLAGER, 5, 5, 'PrimalisElderHall_EventScript_Attendant'),
    ])

# The oral-history chamber: room for TWO elders facing each other + the player.
mapjson('Primalis_ZoanVillage_ElderHall_Interior', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_2F',
    'MAPSEC_PRIMALIS_ZOAN_VILLAGE', MUS_HALL, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('ZOAN_VILLAGE_ELDER_HALL_INTERIOR'),
    warps=[warp(13, 1, M('ZOAN_VILLAGE_ELDER_HALL'), 2)],   # 0: back down
    objects=[
        # Elder Mako - Gym 4. Talk-initiated. First meeting suspicious (post-Gym2);
        # reveals the oral history post-Gym4. Hidden until accessible (script-gated).
        obj(GFX_MAKO, 6, 5, 'PrimalisElderHallInterior_EventScript_Mako',
            movement='MOVEMENT_TYPE_FACE_RIGHT', local_id='LOCALID_PRIMALIS_MAKO'),
        # Second elder, facing Mako (the oral-history scene's two-NPC tableau).
        obj(GFX_VILLAGER, 9, 5, 'PrimalisElderHallInterior_EventScript_Elder2',
            movement='MOVEMENT_TYPE_FACE_LEFT', local_id='LOCALID_PRIMALIS_ELDER2'),
    ])

# =====================================================================
#  JungleRoute2 (20x30) — Thorn (Gym3); north exit Beast-Whistle gate
# =====================================================================
mapjson('Primalis_JungleRoute2', 'LAYOUT_PRIMALIS_JUNGLE_ROUTE_2',
    'MAPSEC_PRIMALIS_JUNGLE_ROUTE_2', MUS_JUNGLE, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('JUNGLE_ROUTE2'),
    connections=[
        conn('left', 3, M('ZOAN_VILLAGE')),
        conn('up', -2, M('ANCIENT_RUINS_CAMP')),
    ],
    objects=[
        # Thorn - Gym 3, met on the route. Talk-initiated (flag-gated on Gym2).
        obj(GFX_THORN, 10, 18, 'PrimalisJungleRoute2_EventScript_Thorn',
            local_id='LOCALID_PRIMALIS_THORN'),
        # ZOAN5 converted from a sight trainer to a plain talk NPC: only four
        # TRAINER_ZOAN_PRIMALIS_ ids (925-928) exist, mapped 1:1 to ZOAN1-4.
        # This fifth Zoan is a guardian-villager with battle-flavor/lore dialogue.
        obj(GFX_ZOAN, 6, 22, 'PrimalisJungleRoute2_EventScript_Zoan5',
            trainer_type='TRAINER_TYPE_NONE', sight='0', movement='MOVEMENT_TYPE_FACE_UP',
            local_id='LOCALID_PRIMALIS_ZOAN5'),
    ],
    coords=[
        # North-exit Beast-Whistle gate (the Heartwood approach). TODO field effect.
        trigger(9, 3, 'VAR_PRIMALIS_PROGRESS', 5, 'PrimalisJungleRoute2_EventScript_BeastGate'),
        trigger(10, 3, 'VAR_PRIMALIS_PROGRESS', 5, 'PrimalisJungleRoute2_EventScript_BeastGate'),
    ])

# =====================================================================
#  AncientRuinsCamp (24x20) — ruins, central altar (cipher 8 area)
# =====================================================================
mapjson('Primalis_AncientRuinsCamp', 'LAYOUT_PRIMALIS_ANCIENT_RUINS_CAMP',
    'MAPSEC_PRIMALIS_ANCIENT_RUINS_CAMP', MUS_RUINS, 'MAP_TYPE_ROUTE', weather=FOG,
    mapid=M('ANCIENT_RUINS_CAMP'),
    connections=[
        conn('down', 2, M('JUNGLE_ROUTE2')),
        conn('up', -2, M('THE_HEARTWOOD')),
    ],
    objects=[
        obj(GFX_LENS, 14, 12, 'PrimalisAncientRuinsCamp_EventScript_LensCamp',
            local_id='LOCALID_PRIMALIS_LENS_CAMP', flag='FLAG_PRIMALIS_LENS_MET'),
    ],
    bgs=[
        # Central altar - examine -> FLAG_PRIMALIS_RUINS_FOUND / cipher reading.
        sign(11, 8, 'PrimalisAncientRuinsCamp_EventScript_Altar',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  TheHeartwood (28x34) — THE VAST ANCIENT SANCTUARY
# =====================================================================
mapjson('Primalis_TheHeartwood', 'LAYOUT_PRIMALIS_THE_HEARTWOOD',
    'MAPSEC_PRIMALIS_HEARTWOOD', MUS_HEART, 'MAP_TYPE_ROUTE', weather=NONE,
    mapid=M('THE_HEARTWOOD'),
    connections=[conn('down', 2, M('ANCIENT_RUINS_CAMP'))],
    warps=[warp(10, 2, M('HEARTWOOD_SEAL_CHAMBER'), 0)],   # 0: into the seal chamber
    objects=[
        # Mako + the Lens stand here for the climactic seal sequence (script-gated).
        obj(GFX_MAKO, 12, 8, 'PrimalisTheHeartwood_EventScript_Mako',
            local_id='LOCALID_PRIMALIS_HEARTWOOD_MAKO', flag='FLAG_PRIMALIS_RUINS_FOUND'),
        obj(GFX_LENS, 15, 8, 'PrimalisTheHeartwood_EventScript_Lens',
            local_id='LOCALID_PRIMALIS_HEARTWOOD_LENS', flag='FLAG_PRIMALIS_RUINS_FOUND'),
    ],
    bgs=[
        # The root-knot centerpiece (Verdath woven into the roots) - examine.
        sign(13, 5, 'PrimalisTheHeartwood_EventScript_RootKnot',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ---- Heartwood_SealChamber (SEALED_CHAMBER_INNER_ROOM) ----
mapjson('Primalis_Heartwood_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_PRIMALIS_HEARTWOOD', MUS_SEAL, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('HEARTWOOD_SEAL_CHAMBER'),
    warps=[
        warp(10, 19, M('THE_HEARTWOOD'), 0),
        warp(11, 19, M('THE_HEARTWOOD'), 0),
    ],
    objects=[
        # Verdath - the oldest sealed legendary (sealed decoration, script 0x0).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_PRIMALIS_VERDATH', elevation=1,
            movement='MOVEMENT_TYPE_NONE'),
        # Seal reinforcement point.
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'PrimalisSealChamber_EventScript_Apparatus',
            local_id='LOCALID_PRIMALIS_APPARATUS'),
    ],
    coords=[
        trigger(9, 14, 'VAR_PRIMALIS_PROGRESS', 5, 'PrimalisSealChamber_EventScript_Discovery'),
        trigger(10, 14, 'VAR_PRIMALIS_PROGRESS', 5, 'PrimalisSealChamber_EventScript_Discovery'),
        trigger(11, 14, 'VAR_PRIMALIS_PROGRESS', 5, 'PrimalisSealChamber_EventScript_Discovery'),
    ],
    bgs=[
        sign(7, 7, 'PrimalisSealChamber_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

print('Primalis map.json files written (13 maps)')
