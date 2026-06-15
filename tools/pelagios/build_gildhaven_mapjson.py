#!/usr/bin/env python3
"""Generate map.json for all 13 Gildhaven maps. Mirrors build_thalvern_mapjson.py.

Gildhaven = the Merchant Island. Fairy/Dark. An ENTIRELY URBAN, glittering trading
port whose prosperity is artificial (Mirath's leaking seal energy drives obsession
with wealth). No custom Pelagios tileset — every map pairs gTileset_General (primary)
with gTileset_Lilycove (secondary). Lilycove was chosen over Slateport for the
wealthy/glittering big-city look (ornate paving, richer building fronts) vs Slateport's
working-port market stalls. (Rationale documented in build_gildhaven.py.)

Tilesets:
  Outdoor maps (GoldportHarbor, MerchantDistrict, NobleQuarter, TheExchange_Exterior):
      gTileset_General + gTileset_Lilycove (composed layouts, build_gildhaven.py)
  Interiors reuse vanilla LAYOUT_* wholesale (zero new binary):
      Inns / PokeCenter : POKEMON_CENTER_1F / _2F
      BlackMarket       : SEALED_CHAMBER_OUTER_ROOM (hidden underground den; Sirocco
                          BlackMarket precedent)
      VaneManor / Int   : MOSSDEEP_CITY_SPACE_CENTER_1F / _2F (multi-room wealthy
                          palace interior + study; Sirocco DaganPalace precedent. The
                          FRLG Pokemon Mansion layouts do NOT link into the Emerald
                          build — their layout binaries are only emitted for FRLG-region
                          maps — so a Hoenn palace interior is used instead.)
      Exchange Int1/Int2: LILYCOVE_CITY_LILYCOVE_MUSEUM_1F / _2F (grand intimidating halls)
      SealChamber       : SEALED_CHAMBER_INNER_ROOM (ancient stone, CONTRAST with the
                          gilt Exchange above it — same chamber every island uses)

Weather:
  All outdoor maps                : WEATHER_SUNNY (artificial optimism)
  TheExchange interiors + SealChamber : WEATHER_NONE
  Inn / PokeCenter / BlackMarket / Manor : WEATHER_NONE (indoor)

Harbor rule: the SS_TIDAL (Tennyson, Galleon) is pure decoration (script 0x0);
boarding runs through sign bg_events on the dock -> *_EventScript_Tennyson, which sets
VAR_TEMP_1 = PELAGIOS_ISLAND_GILDHAVEN then goto Pelagios_EventScript_BoardTennyson.

SealChamber pattern mirrors Ironhold/Schism/Thalvern exactly: SS_TIDAL (Mirath
decoration) at (10,5), seal apparatus CABLE_CAR at (10,8), discovery triggers on row 14
(cols 9-11), warp back at (10,19). NOTE per brief: Gildhaven's seal leaks NATURALLY (no
Covenant siphon) — the apparatus here is the player's reinforcement point, not machinery
to disable; that distinction is dialogue (script-writer), not geometry.

GOTCHA (SEALED_CHAMBER_OUTER_ROOM, BlackMarket): cols 8-12 are walled on rows 17-19;
walkable bottom pockets are cols 3-7 and 13-17. Warp lands at top (10,3) like Sirocco.

All EventScript labels are TODO stubs filled by build_gildhaven_scripts_stubs.py;
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

M = lambda s: 'MAP_GILDHAVEN_' + s
MUS_PORT = 'MUS_SLATEPORT'
MUS_CITY = 'MUS_LILYCOVE'
MUS_MARKET = 'MUS_LILYCOVE'
MUS_NOBLE = 'MUS_LILYCOVE_MUSEUM'
MUS_MANOR = 'MUS_PETALBURG'
MUS_EXCHANGE = 'MUS_GAME_CORNER'
MUS_SEAL = 'MUS_SEALED_CHAMBER'
MUS_BLACK = 'MUS_GAME_CORNER'
MUS_CENTER = 'MUS_POKE_CENTER'

# sprite placeholders (brief-specified)
GFX_CASS   = 'OBJ_EVENT_GFX_RIVAL_MAY_NORMAL'   # Cass (established Haven precedent)
GFX_DAGAN  = 'OBJ_EVENT_GFX_RICH_BOY'           # Dagan (brief: RICH_BOY)
GFX_SEREL  = 'OBJ_EVENT_GFX_RICH_BOY'           # Serel (brief: RICH_BOY, older)
GFX_LACE   = 'OBJ_EVENT_GFX_LASS'               # Lace (brief: YOUNGSTER_F; LASS placeholder)
GFX_GLINT  = 'OBJ_EVENT_GFX_MART_EMPLOYEE'      # Glint, street trader (Fairy gym)
GFX_SHADE  = 'OBJ_EVENT_GFX_MAN_5'              # Shade, enforcer (Dark gym)
GFX_GUARD  = 'OBJ_EVENT_GFX_GENTLEMAN'          # Exchange guards (no GUARD gfx)
GFX_OFFICER= 'OBJ_EVENT_GFX_GENTLEMAN'          # Covenant officers (flavor)

# =====================================================================
#  GoldportHarbor (20x18) — entry port. South sea, north -> MerchantDistrict.
#  Galleon arrival tile = (10,14): central stone dock, verified walkable.
#  Cass GLIMPSE: a RIVAL_MAY object 5 tiles from the arrival, walks off on the
#  arrival trigger. Hidden again afterward via FLAG_CASS_GILDHAVEN_SEEN.
# =====================================================================
mapjson('Gildhaven_GoldportHarbor', 'LAYOUT_GILDHAVEN_GOLDPORT_HARBOR',
    'MAPSEC_GILDHAVEN_GOLDPORT_HARBOR', MUS_PORT, 'MAP_TYPE_TOWN', weather='WEATHER_SUNNY',
    mapid=M('GOLDPORT_HARBOR'),
    connections=[conn('up', -4, M('MERCHANT_DISTRICT'))],
    warps=[warp(4, 8, M('GOLDPORT_HARBOR_INN'), 0)],   # 0: into Inn
    objects=[
        # Harbor official (efficient, slightly cold).
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 8, 6, 'GildhavenGoldportHarbor_EventScript_HarborOfficial',
            local_id='LOCALID_GILDHAVEN_HARBOR_OFFICIAL'),
        # Sailor who has been here too long.
        obj('OBJ_EVENT_GFX_SAILOR', 14, 7, 'GildhavenGoldportHarbor_EventScript_Sailor',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # Cass glimpse — appears at the north end, walks off after the arrival
        # trigger. Hidden once seen. (Script-writer drives the walk-off + hide.)
        obj(GFX_CASS, 9, 2, 'GildhavenGoldportHarbor_EventScript_CassGlimpse',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_CASS_GILDHAVEN_SEEN',
            local_id='LOCALID_GILDHAVEN_CASS_HARBOR'),
        # The Tennyson (Galleon). Pure decoration; boarding via dock bg_events.
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 15, '0x0',
            local_id='LOCALID_GILDHAVEN_TENNYSON', elevation=1),
    ],
    coords=[
        # Arrival cutscene + Cass glimpse fire on first step (progress 0).
        trigger(9, 13, 'VAR_GILDHAVEN_PROGRESS', 0, 'GildhavenGoldportHarbor_EventScript_Arrival'),
        trigger(10, 13, 'VAR_GILDHAVEN_PROGRESS', 0, 'GildhavenGoldportHarbor_EventScript_Arrival'),
    ],
    bgs=[
        sign(2, 2, 'GildhavenGoldportHarbor_EventScript_PortSign'),
        # Boarding from the dock edge (cols 9-10, rows 12-15 are the dock spine).
        sign(8, 13, 'GildhavenGoldportHarbor_EventScript_Tennyson'),
        sign(8, 14, 'GildhavenGoldportHarbor_EventScript_Tennyson'),
        sign(11, 13, 'GildhavenGoldportHarbor_EventScript_Tennyson'),
        sign(11, 14, 'GildhavenGoldportHarbor_EventScript_Tennyson'),
    ])

# ============================ GoldportHarbor_Inn (lobby, heal) ============================
mapjson('Gildhaven_GoldportHarbor_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_GILDHAVEN_GOLDPORT_HARBOR', MUS_CENTER, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('GOLDPORT_HARBOR_INN'),
    warps=[
        warp(7, 8, M('GOLDPORT_HARBOR'), 0),                                  # 0
        warp(6, 8, M('GOLDPORT_HARBOR'), 0),                                  # 1
        warp(1, 6, M('GOLDPORT_HARBOR_INN_INTERIOR'), 0, elevation=4),        # 2: up to rooms
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'GildhavenGoldportHarborInn_EventScript_Innkeeper',
            local_id='LOCALID_GILDHAVEN_HARBOR_INNKEEPER'),
    ])

# ============================ GoldportHarbor_Inn_Interior (rooms) ============================
mapjson('Gildhaven_GoldportHarbor_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_GILDHAVEN_GOLDPORT_HARBOR', MUS_CENTER, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True, mapid=M('GOLDPORT_HARBOR_INN_INTERIOR'),
    warps=[warp(1, 6, M('GOLDPORT_HARBOR_INN'), 2, elevation=4)],   # 0: back down
    objects=[
        obj('OBJ_EVENT_GFX_GENTLEMAN', 6, 4, 'GildhavenGoldportHarborInnInterior_EventScript_Innkeeper2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # Traveler who has lost track of time.
        obj('OBJ_EVENT_GFX_MAN_4', 9, 5, 'GildhavenGoldportHarborInnInterior_EventScript_Traveler',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ MerchantDistrict (28x24) — main city hub ============================
# down->GoldportHarbor; warps to MerchantDistrict_PokemonCenter; the BlackMarket alley
# (behind the silk stall) and NobleQuarter are reached by WARPS. Glint (Gym 1) + Shade
# (Gym 2) are talk-initiated overworld NPCs (Petra/Cinder pattern). Dagan waves from a
# cafe (won't engage until Gym 1). Lace watches the noble-quarter entrance (won't speak
# until Gym 2).
mapjson('Gildhaven_MerchantDistrict', 'LAYOUT_GILDHAVEN_MERCHANT_DISTRICT',
    'MAPSEC_GILDHAVEN_MERCHANT_DISTRICT', MUS_MARKET, 'MAP_TYPE_TOWN', weather='WEATHER_SUNNY',
    mapid=M('MERCHANT_DISTRICT'),
    connections=[conn('down', 4, M('GOLDPORT_HARBOR'))],
    warps=[
        warp(13, 1, M('MERCHANT_DISTRICT_POKEMON_CENTER'), 0),  # 0: PokeCenter (north building)
        warp(5, 7, M('BLACK_MARKET'), 0),                        # 1: hidden alley below silk stall (walkable)
        warp(25, 2, M('NOBLE_QUARTER'), 2),                      # 2: up to NobleQuarter (lands at its south entrance)
    ],
    objects=[
        # Gym 1 — Glint, converted market stall (talk-initiated).
        obj(GFX_GLINT, 13, 11, 'GildhavenMerchantDistrict_EventScript_Glint',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_GLINT'),
        # Gym 2 — Shade, hidden alley (talk-initiated).
        obj(GFX_SHADE, 5, 16, 'GildhavenMerchantDistrict_EventScript_Shade',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_SHADE'),
        # Dagan in a cafe — waves, won't engage until after Gym 1 (flavor here; the
        # real meeting is in the BlackMarket).
        obj(GFX_DAGAN, 20, 8, 'GildhavenMerchantDistrict_EventScript_DaganCafe',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_DAGAN_CAFE'),
        # Lace watching the noble-quarter entrance (won't speak until after Gym 2).
        obj(GFX_LACE, 23, 4, 'GildhavenMerchantDistrict_EventScript_LaceWatching',
            movement='MOVEMENT_TYPE_FACE_UP', local_id='LOCALID_GILDHAVEN_LACE_WATCHING'),
        # Street vendor (transparently overcharging).
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 21, 16, 'GildhavenMerchantDistrict_EventScript_Vendor',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        # Child who has absorbed the island's values.
        obj('OBJ_EVENT_GFX_LITTLE_GIRL', 9, 18, 'GildhavenMerchantDistrict_EventScript_Child',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        # Examine the silk stall FROM BEHIND to reveal the black-market alley (the warp
        # at (5,7) is the alley mouth; this sign is the hint, just east of it).
        sign(7, 7, 'GildhavenMerchantDistrict_EventScript_SilkStall',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(12, 9, 'GildhavenMerchantDistrict_EventScript_GlintGymSign'),
        sign(4, 14, 'GildhavenMerchantDistrict_EventScript_ShadeGymSign'),
    ])

# ============================ MerchantDistrict_PokemonCenter ============================
mapjson('Gildhaven_MerchantDistrict_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_GILDHAVEN_MERCHANT_DISTRICT', MUS_CENTER, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=False, indoor=True,
    mapid=M('MERCHANT_DISTRICT_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('MERCHANT_DISTRICT'), 0),
        warp(6, 8, M('MERCHANT_DISTRICT'), 0),
    ],
    objects=[
        obj('OBJ_EVENT_GFX_NURSE', 7, 2, 'GildhavenMerchantDistrictPokemonCenter_EventScript_Nurse',
            local_id='LOCALID_GILDHAVEN_CITY_NURSE'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 1, 3, 'GildhavenMerchantDistrictPokemonCenter_EventScript_Shopkeeper',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ BlackMarket (hidden den) ============================
# Reused SEALED_CHAMBER_OUTER_ROOM. Dagan's base — he's here AFTER Gym 1
# (FLAG_GILDHAVEN_GYM1_CLEAR as his hide flag). Honest vendors (contrast with the
# overpriced city). Information broker. Warp lands at the top (10,3) — safe (the bottom
# center cols 8-12 are walled). No wild encounters table entry exists (urban hidden den
# has a wild table per the brief — added in wild_encounters.json; rate kept low).
mapjson('Gildhaven_BlackMarket', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_GILDHAVEN_BLACK_MARKET', MUS_BLACK, 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=True, indoor=True, mapid=M('BLACK_MARKET'),
    warps=[
        warp(10, 3, M('MERCHANT_DISTRICT'), 1),   # 0: back up to the alley
        warp(11, 3, M('MERCHANT_DISTRICT'), 1),   # 1
    ],
    objects=[
        # Dagan (returning from Sirocco) — appears after Gym 1.
        obj(GFX_DAGAN, 10, 13, 'GildhavenBlackMarket_EventScript_Dagan',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_GILDHAVEN_GYM1_CLEAR',
            local_id='LOCALID_GILDHAVEN_DAGAN'),
        # Honest vendors (fair prices).
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 5, 7, 'GildhavenBlackMarket_EventScript_Vendor1',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MART_EMPLOYEE', 15, 7, 'GildhavenBlackMarket_EventScript_Vendor2',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        # Information broker who knows about the seal.
        obj('OBJ_EVENT_GFX_MAN_5', 4, 13, 'GildhavenBlackMarket_EventScript_InfoBroker',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ])

# ============================ NobleQuarter (24x20) ============================
# Wealthy residential. Reached by WARP from MerchantDistrict (NE arch) — not a map
# connection (urban gated arch; avoids a one-sided connection). Warp to VaneManor
# (locked until Gym 3). Lace's garden gym (Gym 3) is a talk-initiated NPC at the
# garden's edge. Covenant officers present. Cass appears ONCE (emerges from the manor).
mapjson('Gildhaven_NobleQuarter', 'LAYOUT_GILDHAVEN_NOBLE_QUARTER',
    'MAPSEC_GILDHAVEN_NOBLE_QUARTER', MUS_NOBLE, 'MAP_TYPE_TOWN', weather='WEATHER_SUNNY',
    mapid=M('NOBLE_QUARTER'),
    warps=[
        warp(10, 5, M('NOBLE_QUARTER_VANE_MANOR'), 0),   # 0: manor door (gated post-Gym3)
        warp(13, 5, M('NOBLE_QUARTER_VANE_MANOR'), 0),   # 1
        warp(11, 18, M('MERCHANT_DISTRICT'), 2),         # 2: back down to MerchantDistrict (NE arch)
        warp(12, 18, M('MERCHANT_DISTRICT'), 2),         # 3
    ],
    objects=[
        # Gym 3 — Lace Vane, garden arena (talk-initiated). Post-Gym2 she speaks here.
        obj(GFX_LACE, 18, 9, 'GildhavenNobleQuarter_EventScript_Lace',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_LACE'),
        # Cass — one-time noble-quarter appearance (emerges from the manor). Hidden
        # after FLAG_CASS_GILDHAVEN_SEEN; the scene reveals/positions her.
        obj(GFX_CASS, 11, 5, 'GildhavenNobleQuarter_EventScript_CassWarning',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_CASS_GILDHAVEN_SEEN',
            local_id='LOCALID_GILDHAVEN_CASS_NOBLE'),
        # Covenant officers (flavor).
        obj(GFX_OFFICER, 6, 9, 'GildhavenNobleQuarter_EventScript_CovenantOfficer1',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj(GFX_OFFICER, 4, 13, 'GildhavenNobleQuarter_EventScript_CovenantOfficer2',
            movement='MOVEMENT_TYPE_FACE_RIGHT'),
        # Wealthy resident (corruption-variant NPC).
        obj('OBJ_EVENT_GFX_BEAUTY', 20, 14, 'GildhavenNobleQuarter_EventScript_Resident',
            movement='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
    ],
    coords=[
        # Cass noble-quarter scene fires once on first entry (FLAG_CASS_GILDHAVEN_SEEN
        # guards repeat; the trigger var is a placeholder the script-writer rewires).
        trigger(11, 8, 'VAR_TEMP_2', 0, 'GildhavenNobleQuarter_EventScript_CassScene'),
        trigger(12, 8, 'VAR_TEMP_2', 0, 'GildhavenNobleQuarter_EventScript_CassScene'),
    ],
    bgs=[
        sign(8, 5, 'GildhavenNobleQuarter_EventScript_ManorSign',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(16, 11, 'GildhavenNobleQuarter_EventScript_GardenGymSign'),
    ])

# ============================ NobleQuarter_VaneManor (foyer) ============================
# Reused MOSSDEEP_CITY_SPACE_CENTER_1F (16x10). Locked until post-Gym3 (Lace gives
# ITEM_VANE_MANOR_KEY / FLAG_GILDHAVEN_MANOR_ACCESS). Study upstairs has Covenant
# documents + the hidden passage to TheExchange_Exterior. Warp coords mirror the
# Sirocco DaganPalace_Interior1 usage exactly (entry (7,9)/(8,9); up-stair (13,1)).
mapjson('Gildhaven_NobleQuarter_VaneManor', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F',
    'MAPSEC_GILDHAVEN_VANE_MANOR', MUS_MANOR, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=True, indoor=True, mapid=M('NOBLE_QUARTER_VANE_MANOR'),
    warps=[
        warp(7, 9, M('NOBLE_QUARTER'), 0),                           # 0: out to NobleQuarter
        warp(8, 9, M('NOBLE_QUARTER'), 0),                           # 1
        warp(13, 1, M('NOBLE_QUARTER_VANE_MANOR_INTERIOR'), 0),      # 2: up to study
    ],
    objects=[
        # Manor steward (flavor; gating handled by warp/script).
        obj('OBJ_EVENT_GFX_GENTLEMAN', 5, 6, 'GildhavenVaneManor_EventScript_Steward',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_MANOR_STEWARD'),
        obj('OBJ_EVENT_GFX_BEAUTY', 11, 5, 'GildhavenVaneManor_EventScript_Maid',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        sign(3, 3, 'GildhavenVaneManor_EventScript_DisplayCase',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ NobleQuarter_VaneManor_Interior (study) ============================
# Reused MOSSDEEP_CITY_SPACE_CENTER_2F (16x10). Serel's study — Covenant documents
# (examine), hidden passage warp to TheExchange_Exterior. Down-stair (13,1) mirrors
# Sirocco DaganPalace_Interior2.
mapjson('Gildhaven_NobleQuarter_VaneManor_Interior', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_2F',
    'MAPSEC_GILDHAVEN_VANE_MANOR', MUS_MANOR, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=True, indoor=True,
    mapid=M('NOBLE_QUARTER_VANE_MANOR_INTERIOR'),
    warps=[
        warp(13, 1, M('NOBLE_QUARTER_VANE_MANOR'), 2),               # 0: back down to foyer
        warp(2, 8, M('THE_EXCHANGE_EXTERIOR'), 0),                   # 1: hidden passage -> Exchange
    ],
    objects=[
        obj('OBJ_EVENT_GFX_GENTLEMAN', 9, 5, 'GildhavenVaneManorInterior_EventScript_Butler',
            movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        # Covenant documents — Serel has been in contact with the Covenant for years.
        sign(6, 3, 'GildhavenVaneManorInterior_EventScript_CovenantDocuments',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
        # The hidden passage hint (examine the bookshelf beside warp 1).
        sign(3, 6, 'GildhavenVaneManorInterior_EventScript_HiddenPassage',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ TheExchange_Exterior (20x16) ============================
# Imposing frontage. Reached from VaneManor_Interior (hidden passage warp). Two
# elite-guard battles at the entrance. Warp -> Interior1. Arrival door gap at cols 9-10.
mapjson('Gildhaven_TheExchange_Exterior', 'LAYOUT_GILDHAVEN_THE_EXCHANGE_EXTERIOR',
    'MAPSEC_GILDHAVEN_THE_EXCHANGE', MUS_EXCHANGE, 'MAP_TYPE_TOWN', weather='WEATHER_SUNNY',
    mapid=M('THE_EXCHANGE_EXTERIOR'),
    warps=[
        warp(9, 3, M('THE_EXCHANGE_INTERIOR1'), 0),     # 0: into the Exchange
        warp(10, 3, M('THE_EXCHANGE_INTERIOR1'), 0),    # 1
        # Return to the manor study (the passage is two-way).
        warp(1, 14, M('NOBLE_QUARTER_VANE_MANOR_INTERIOR'), 1),  # 2: back to manor study
    ],
    objects=[
        # Two elite-guard trainers at the entrance (sight trainers).
        obj(GFX_GUARD, 7, 6, 'GildhavenTheExchangeExterior_EventScript_GuardRael',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj(GFX_GUARD, 12, 6, 'GildhavenTheExchangeExterior_EventScript_GuardSera',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_DOWN'),
    ],
    bgs=[
        sign(5, 5, 'GildhavenTheExchangeExterior_EventScript_ExchangeSign'),
    ])

# ============================ TheExchange_Interior1 (grand hall) ============================
# Reused LILYCOVE_CITY_LILYCOVE_MUSEUM_1F. Intimidating wealth, two elite-guard
# trainers, Serel visible on the upper level. Warp up -> Interior2.
mapjson('Gildhaven_TheExchange_Interior1', 'LAYOUT_LILYCOVE_CITY_LILYCOVE_MUSEUM_1F',
    'MAPSEC_GILDHAVEN_THE_EXCHANGE', MUS_EXCHANGE, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=True, indoor=True, mapid=M('THE_EXCHANGE_INTERIOR1'),
    warps=[
        warp(10, 13, M('THE_EXCHANGE_EXTERIOR'), 0),    # 0: back out
        warp(11, 13, M('THE_EXCHANGE_EXTERIOR'), 0),    # 1
        warp(3, 1, M('THE_EXCHANGE_INTERIOR2'), 0),     # 2: up to Interior2 (stairs)
        warp(17, 1, M('THE_EXCHANGE_INTERIOR2'), 2),    # 3: second stair landing
    ],
    objects=[
        # Two more elite-guard trainers (Morn, Daven).
        obj(GFX_GUARD, 6, 8, 'GildhavenTheExchangeInterior1_EventScript_GuardMorn',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_DOWN'),
        obj(GFX_GUARD, 14, 8, 'GildhavenTheExchangeInterior1_EventScript_GuardDaven',
            trainer_type='TRAINER_TYPE_NORMAL', sight='3', movement='MOVEMENT_TYPE_FACE_DOWN'),
        # Serel visible on the upper level — watching (flavor; the real fight is Int2).
        obj(GFX_SEREL, 10, 2, 'GildhavenTheExchangeInterior1_EventScript_SerelWatching',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_SEREL_WATCHING'),
    ],
    bgs=[
        sign(2, 5, 'GildhavenTheExchangeInterior1_EventScript_Plaque',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ TheExchange_Interior2 (Serel's floor, Gym 4) ============================
# Reused LILYCOVE_CITY_LILYCOVE_MUSEUM_2F. Gym 4 (Serel Vane, talk-initiated). The
# Covenant MAP on the wall (examine bg_event -> FLAG_GILDHAVEN_COVENANT_MAP_SEEN). After
# defeating Serel: hidden stairs to the SealChamber (warp gated by the script-writer).
mapjson('Gildhaven_TheExchange_Interior2', 'LAYOUT_LILYCOVE_CITY_LILYCOVE_MUSEUM_2F',
    'MAPSEC_GILDHAVEN_THE_EXCHANGE', MUS_EXCHANGE, 'MAP_TYPE_INDOOR',
    weather='WEATHER_NONE', show_name=True, indoor=True, mapid=M('THE_EXCHANGE_INTERIOR2'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(3, 12, M('THE_EXCHANGE_INTERIOR1'), 2),    # 0: down to Interior1
        warp(18, 12, M('THE_EXCHANGE_INTERIOR1'), 3),   # 1: second stair down
        warp(11, 1, M('THE_EXCHANGE_SEAL_CHAMBER'), 0), # 2: hidden stairs to SealChamber
    ],
    objects=[
        # Gym 4 — Merchant Lord Serel Vane (talk-initiated).
        obj(GFX_SEREL, 11, 5, 'GildhavenTheExchangeInterior2_EventScript_Serel',
            movement='MOVEMENT_TYPE_FACE_DOWN', local_id='LOCALID_GILDHAVEN_SEREL'),
        # Lace enters from a side door for the post-battle moment (hidden until then).
        obj(GFX_LACE, 14, 5, 'GildhavenTheExchangeInterior2_EventScript_LacePostBattle',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_TEMP_1',
            local_id='LOCALID_GILDHAVEN_LACE_EXCHANGE'),
    ],
    bgs=[
        # The Covenant map of Pelagios — every island rated by legendary energy yield.
        sign(11, 3, 'GildhavenTheExchangeInterior2_EventScript_CovenantMap',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# ============================ TheExchange_SealChamber ============================
# Reused SEALED_CHAMBER_INNER_ROOM — ANCIENT STONE, the oldest room in Gildhaven,
# utterly at odds with the gilt Exchange above (the brief's key visual contrast).
# Mirath (SS_TIDAL decoration, script 0x0) at (10,5). The seal leaks NATURALLY — no
# Covenant siphon — so the apparatus here is the player's reinforcement point (dialogue
# distinction, not geometry). Discovery triggers on row 14 (progress 5). Warp back at
# (10,19). Wild table (Carbink/Togepi/Ralts) added in wild_encounters.json.
mapjson('Gildhaven_TheExchange_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_GILDHAVEN_SEAL_CHAMBER', MUS_SEAL, 'MAP_TYPE_UNDERGROUND',
    weather='WEATHER_NONE', show_name=True, indoor=True, mapid=M('THE_EXCHANGE_SEAL_CHAMBER'),
    warps=[
        warp(10, 19, M('THE_EXCHANGE_INTERIOR2'), 2),   # 0: back up to Interior2
        warp(11, 19, M('THE_EXCHANGE_INTERIOR2'), 2),   # 1
    ],
    objects=[
        # Mirath — shimmering Fairy/Dark presence, barely visible (sealed decoration).
        obj('OBJ_EVENT_GFX_SS_TIDAL', 10, 5, '0x0',
            local_id='LOCALID_GILDHAVEN_MIRATH', elevation=1),
        # Seal reinforcement point (no siphon to disable — direct reinforcement).
        obj('OBJ_EVENT_GFX_CABLE_CAR', 10, 8, 'GildhavenSealChamber_EventScript_Apparatus',
            local_id='LOCALID_GILDHAVEN_APPARATUS'),
    ],
    coords=[
        # Discovery cutscene fires on entry at progress 5 (Gym 4 cleared). Row 14,
        # cols 9-11 — verified walkable on this layout (the discovery row other islands use).
        trigger(9, 14, 'VAR_GILDHAVEN_PROGRESS', 5, 'GildhavenSealChamber_EventScript_Discovery'),
        trigger(10, 14, 'VAR_GILDHAVEN_PROGRESS', 5, 'GildhavenSealChamber_EventScript_Discovery'),
        trigger(11, 14, 'VAR_GILDHAVEN_PROGRESS', 5, 'GildhavenSealChamber_EventScript_Discovery'),
    ],
    bgs=[
        # Ancient inscriptions — cipher 7 found here (examine).
        sign(7, 7, 'GildhavenSealChamber_EventScript_Inscription',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

print('Gildhaven map.json files written (13 maps)')
