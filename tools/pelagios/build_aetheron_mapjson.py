#!/usr/bin/env python3
"""Generate map.json for all 11 Aetheron maps. Mirrors build_ashenveil_mapjson.py.

Aetheron = the SKY ISLAND (Electric/Flying). Above the clouds, permanent
thunderstorm, reached via the Knock Up Stream (ITEM_SEA_CHART). Cass defects here.

ARRIVAL TOPOLOGY (documented):
  boat (Pelagios_EventScript_SailToAetheron) --warp--> MAP_AETHERON_KNOCK_UP_STREAM
  at arrival tile (8,11); the ascent cutscene plays (script-writer); then a ONE-WAY
  forward warp --> MAP_AETHERON_CLOUD_LANDING (the cloud dock). KnockUpStream has NO
  warp back (per brief). CloudLanding's Tennyson bg_events handle sail-back via the
  shared boat menu (VAR_TEMP_1 = PELAGIOS_ISLAND_AETHERON).

WEATHER: the brief says "WEATHER_THUNDERSTORM" but that symbol does NOT exist in
include/constants/weather.h. The real heavy-storm constant is
WEATHER_RAIN_THUNDERSTORM (5). KnockUpStream uses WEATHER_RAIN (3). All interiors +
SealChamber use WEATHER_NONE. DOCUMENTED in CLAUDE.md.

TILESETS:
  CloudLanding / SkyRoute / AetherVillage / StormPeak / KnockUpStream:
      gTileset_General + gTileset_Slateport (light ground = cloud; void = abyss).
  CovenantInstallation (exterior): gTileset_General + gTileset_Mauville (angular
      industrial = feels WRONG / out of place on a sky island).
  Interiors reuse vanilla LAYOUT_* wholesale (zero new binary):
      CloudLanding_Inn          : LAYOUT_POKEMON_CENTER_1F (inn lobby/heal)
      CloudLanding_Inn_Interior : LAYOUT_POKEMON_CENTER_2F (inn upstairs)
      AetherVillage_PokemonCenter : LAYOUT_POKEMON_CENTER_1F (nurse)
      CovenantInstallation_Interior : LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F (roomy
          tech floor - Gym 3 Voss + the Cass DEFECTION 3-character tableau space)
      SealChamber : LAYOUT_SEALED_CHAMBER_INNER_ROOM (bright-still eye of the storm)

TWO CASS OBJECTS:
  (a) AetherVillage Cass SIGHTING: object's `flag` = FLAG_AETHERON_CASS_SEEN, so she
      is VISIBLE until the sighting scene plays then sets the flag (object vanishes).
  (b) StormPeak post-defection Cass: object's `flag` = FLAG_AETHERON_CASS_SEEN too so
      she is hidden during normal play; the defection scene clears that and the
      StormPeak scripts addobject/manage her "walks alongside" presence. (Script-
      writer owns the runtime visibility; map-builder just provides the object +
      clear floor.) A THIRD Cass-doorway object lives in the Installation Interior,
      hidden by FLAG_AETHERON_GYM3_CLEAR, for the defection itself.

HEAL: HEAL_LOCATION_AETHERON_CLOUD_LANDING -> CloudLanding_Inn via
LOCALID_AETHERON_INNKEEPER (registered in heal_locations.json; NOT added to
IsLastHealLocationPlayerHouse - nurse-gfx innkeeper takes the standard path).

NO bg_hidden_item events anywhere. The Storm Compass is a SCRIPTED examine bg_event
on the SealChamber apparatus wreckage (gives ITEM_STORM_COMPASS +
FLAG_STORM_COMPASS_OBTAINED), NOT a hidden item.

All EventScript labels are TODO stubs filled by build_aetheron_scripts_stubs.py
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

def mapjson(name, layout, mapsec, music, map_type, weather,
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

M = lambda s: 'MAP_AETHERON_' + s
STORM = 'WEATHER_RAIN_THUNDERSTORM'   # brief's "WEATHER_THUNDERSTORM" doesn't exist
RAIN  = 'WEATHER_RAIN'
NONE  = 'WEATHER_NONE'

MUS_STREAM = 'MUS_ABNORMAL_WEATHER'   # ascent / storm placeholder
MUS_LAND   = 'MUS_SOOTOPOLIS'         # cloud landing placeholder (high, airy)
MUS_ROUTE  = 'MUS_ROUTE120'           # sky route placeholder
MUS_VILLG  = 'MUS_SOOTOPOLIS'         # village placeholder
MUS_INSTAL = 'MUS_AQUA_MAGMA_HIDEOUT' # covenant installation placeholder
MUS_PEAK   = 'MUS_SEALED_CHAMBER'     # storm peak / seal placeholder
MUS_INN    = 'MUS_POKE_CENTER'        # inn / PC placeholder

# sprite placeholders (no Aetheron-specific gfx; documented in CLAUDE.md)
GFX_DOCKHAND = 'OBJ_EVENT_GFX_SAILOR'
GFX_SCHOLAR  = 'OBJ_EVENT_GFX_SCIENTIST_1'
GFX_INNKEEP  = 'OBJ_EVENT_GFX_NURSE'        # nurse-gfx innkeeper (standard heal path)
GFX_NURSE    = 'OBJ_EVENT_GFX_NURSE'
GFX_GALE     = 'OBJ_EVENT_GFX_HIKER'        # Gym1 community elder (Flying)
GFX_ARC      = 'OBJ_EVENT_GFX_SCIENTIST_1'  # Gym2 Covenant defector engineer (Electric)
GFX_VOSS     = 'OBJ_EVENT_GFX_MAN_4'        # Gym3 Covenant commander (Electric/Steel)
GFX_CASS     = 'OBJ_EVENT_GFX_RIVAL_MAY_NORMAL'  # Cass (rival), placeholder
GFX_TENNYSON = 'OBJ_EVENT_GFX_SS_TIDAL'     # Tennyson (Galleon) decoration
GFX_STORMVEIL = 'OBJ_EVENT_GFX_SS_TIDAL'    # Stormveil (sealed legendary) decoration
GFX_APPARATUS = 'OBJ_EVENT_GFX_CABLE_CAR'   # extraction apparatus (examine rig)
GFX_GUARDIAN = 'OBJ_EVENT_GFX_HIKER'
GFX_OFFICER  = 'OBJ_EVENT_GFX_MAN_4'

# =====================================================================
#  1. KnockUpStream (16x14) — SCRIPTED ASCENT cutscene map; one-way forward
# =====================================================================
# The boat warps the player here (arrival tile 8,11). Ascent cutscene (script-
# writer) plays, then a one-way warp FORWARD to CloudLanding. NO warps back, NO
# NPCs, NO wild encounters. An arrival coord trigger fires the ascent at progress 0.
mapjson('Aetheron_KnockUpStream', 'LAYOUT_AETHERON_KNOCK_UP_STREAM',
    'MAPSEC_AETHERON_KNOCK_UP_STREAM', MUS_STREAM, 'MAP_TYPE_ROUTE', weather=RAIN,
    mapid=M('KNOCK_UP_STREAM'), show_name=False,
    warps=[
        # 0: the ONE-WAY forward warp to CloudLanding (the ascent's destination).
        # Points at CloudLanding warp index 1 = the dedicated cloud-pier arrival tile
        # (10,13), NOT the inn door (index 0). The ascent script ends by warping the
        # player through here. The player can never reach this warp tile again (the
        # arrival tile (8,11) is the only walkable space and it has no path back here).
        warp(8, 12, M('CLOUD_LANDING'), 1),
    ],
    coords=[
        # Ascent cutscene fires on the arrival tile/just above it at progress 0.
        trigger(8, 11, 'VAR_AETHERON_PROGRESS', 0, 'AetheronKnockUpStream_EventScript_Ascent'),
        trigger(8, 10, 'VAR_AETHERON_PROGRESS', 0, 'AetheronKnockUpStream_EventScript_Ascent'),
    ])

# =====================================================================
#  2. CloudLanding (20x18) — cloud-pier dock, settlement, inn
# =====================================================================
mapjson('Aetheron_CloudLanding', 'LAYOUT_AETHERON_CLOUD_LANDING',
    'MAPSEC_AETHERON_CLOUD_LANDING', MUS_LAND, 'MAP_TYPE_TOWN', weather=STORM,
    mapid=M('CLOUD_LANDING'),
    connections=[conn('up', 0, M('SKY_ROUTE'))],
    warps=[
        warp(5, 7, M('CLOUD_LANDING_INN'), 0),   # 0: into the inn
        # 1: the ARRIVAL tile the KnockUpStream ascent lands on. A plain walkable
        # settlement-ground tile (8,13) just WEST of the cloud-pier and OFF the
        # Tennyson boarding path (cols 9-10) - so the player never re-steps this warp
        # tile during normal play. Walking north/NE from here crosses the arrival
        # coord triggers at (9,12)/(10,12). Points back at KnockUpStream's one-way warp
        # (harmless - the player can never reach that map's warp tile again).
        warp(8, 13, M('KNOCK_UP_STREAM'), 0),
    ],
    objects=[
        # Cloud-pier dockhand (Orin-equivalent) - talk NPC stub.
        obj(GFX_DOCKHAND, 11, 12, 'AetheronCloudLanding_EventScript_Dockhand',
            local_id='LOCALID_AETHERON_DOCKHAND'),
        # A community member on the cloud surface - talk NPC stub.
        obj(GFX_SCHOLAR, 14, 6, 'AetheronCloudLanding_EventScript_CommunityMember',
            movement='MOVEMENT_TYPE_FACE_LEFT',
            local_id='LOCALID_AETHERON_CLOUD_COMMUNITY'),
        # The Tennyson (Galleon). Pure decoration; boarding via dock bg_events.
        obj(GFX_TENNYSON, 12, 15, '0x0',
            local_id='LOCALID_AETHERON_TENNYSON', elevation=1,
            movement='MOVEMENT_TYPE_NONE'),
    ],
    coords=[
        # Arrival atmosphere: fires walking north off the dock at progress 0 (in case
        # the KnockUpStream ascent set progress but not via this map; belt-and-braces).
        trigger(9, 12, 'VAR_AETHERON_PROGRESS', 0, 'AetheronCloudLanding_EventScript_Arrival'),
        trigger(10, 12, 'VAR_AETHERON_PROGRESS', 0, 'AetheronCloudLanding_EventScript_Arrival'),
    ],
    bgs=[
        # Boarding the Tennyson: sign bg_events on the dock tiles (never the ship).
        sign(9, 15, 'AetheronCloudLanding_EventScript_Tennyson'),
        sign(10, 15, 'AetheronCloudLanding_EventScript_Tennyson'),
        sign(9, 14, 'AetheronCloudLanding_EventScript_Tennyson'),
        sign(10, 14, 'AetheronCloudLanding_EventScript_Tennyson'),
    ])

# =====================================================================
#  3. CloudLanding_Inn (POKEMON_CENTER_1F) — innkeeper (heal)
# =====================================================================
mapjson('Aetheron_CloudLanding_Inn', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_AETHERON_CLOUD_LANDING', MUS_INN, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('CLOUD_LANDING_INN'),
    warps=[
        warp(7, 8, M('CLOUD_LANDING'), 0),                 # 0: out to the cloud landing
        warp(6, 8, M('CLOUD_LANDING'), 0),                 # 1
        warp(1, 6, M('CLOUD_LANDING_INN_INTERIOR'), 0),    # 2: upstairs
    ],
    objects=[
        # Innkeeper (nurse-gfx) - heal via the standard nurse/innkeeper path.
        obj(GFX_INNKEEP, 7, 2, 'AetheronCloudLandingInn_EventScript_Innkeeper',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_INNKEEPER'),
    ])

# =====================================================================
#  4. CloudLanding_Inn_Interior (POKEMON_CENTER_2F) — one scholar NPC
# =====================================================================
mapjson('Aetheron_CloudLanding_Inn_Interior', 'LAYOUT_POKEMON_CENTER_2F',
    'MAPSEC_AETHERON_CLOUD_LANDING', MUS_INN, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('CLOUD_LANDING_INN_INTERIOR'),
    warps=[
        warp(1, 6, M('CLOUD_LANDING_INN'), 2),             # 0: back downstairs
    ],
    objects=[
        # Scholar NPC stub (the Covenant-came-two-years-ago lore). Open floor (5,5).
        obj(GFX_SCHOLAR, 5, 5, 'AetheronCloudLandingInnInterior_EventScript_Scholar',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_INN_SCHOLAR'),
    ])

# =====================================================================
#  5. SkyRoute (18x32) — open sky both sides, 2 guardians, wild storm patches
# =====================================================================
mapjson('Aetheron_SkyRoute', 'LAYOUT_AETHERON_SKY_ROUTE',
    'MAPSEC_AETHERON_SKY_ROUTE', MUS_ROUTE, 'MAP_TYPE_ROUTE', weather=STORM,
    mapid=M('SKY_ROUTE'),
    connections=[
        conn('down', 0, M('CLOUD_LANDING')),
        conn('up', -4, M('AETHER_VILLAGE')),
    ],
    objects=[
        # Two community-guardian sight trainers.
        obj(GFX_GUARDIAN, 8, 12, 'AetheronSkyRoute_EventScript_Guardian1',
            movement='MOVEMENT_TYPE_FACE_DOWN', trainer_type='TRAINER_TYPE_NORMAL',
            sight='4', local_id='LOCALID_AETHERON_GUARDIAN1'),
        obj(GFX_GUARDIAN, 10, 23, 'AetheronSkyRoute_EventScript_Guardian2',
            movement='MOVEMENT_TYPE_FACE_DOWN', trainer_type='TRAINER_TYPE_NORMAL',
            sight='4', local_id='LOCALID_AETHERON_GUARDIAN2'),
    ])

# =====================================================================
#  6. AetherVillage (24x20) — settlement, Gym1 Gale + Gym2 Arc, FIRST CASS SIGHTING
# =====================================================================
mapjson('Aetheron_AetherVillage', 'LAYOUT_AETHERON_AETHER_VILLAGE',
    'MAPSEC_AETHERON_AETHER_VILLAGE', MUS_VILLG, 'MAP_TYPE_TOWN', weather=STORM,
    mapid=M('AETHER_VILLAGE'),
    connections=[
        conn('down', 4, M('SKY_ROUTE')),
        conn('right', 1, M('COVENANT_INSTALLATION')),
    ],
    warps=[
        warp(4, 6, M('AETHER_VILLAGE_POKEMON_CENTER'), 0),   # 0: into the PC
    ],
    objects=[
        # Gym 1 - Gale (community elder, Flying). Talk-initiated (TRAINER_TYPE_NONE).
        obj(GFX_GALE, 11, 8, 'AetheronAetherVillage_EventScript_Gale',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_GALE'),
        # Gym 2 - Arc (Covenant defector engineer, Electric). Talk-initiated.
        # On the central plaza (col-0 GROUND), in front of the workshop block.
        obj(GFX_ARC, 13, 12, 'AetheronAetherVillage_EventScript_Arc',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_ARC'),
        # FIRST CASS SIGHTING. Object visible until the sighting scene sets
        # FLAG_AETHERON_CASS_SEEN (its hide flag) - then she vanishes. The script
        # briefly shows her across the square then removeobjects her.
        obj(GFX_CASS, 20, 5, 'AetheronAetherVillage_EventScript_CassSighting',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_AETHERON_CASS_SEEN',
            local_id='LOCALID_AETHERON_CASS_VILLAGE'),
    ],
    coords=[
        # First-entry Cass sighting trigger (progress 1, before any gym).
        trigger(11, 17, 'VAR_AETHERON_PROGRESS', 1, 'AetheronAetherVillage_EventScript_CassSightingScene'),
        trigger(12, 17, 'VAR_AETHERON_PROGRESS', 1, 'AetheronAetherVillage_EventScript_CassSightingScene'),
    ],
    bgs=[
        # The Covenant Installation visible on the horizon (examine the east skyline).
        sign(22, 9, 'AetheronAetherVillage_EventScript_InstallationView',
             facing='BG_EVENT_PLAYER_FACING_ANY'),
    ])

# =====================================================================
#  7. AetherVillage_PokemonCenter (POKEMON_CENTER_1F) — nurse
# =====================================================================
mapjson('Aetheron_AetherVillage_PokemonCenter', 'LAYOUT_POKEMON_CENTER_1F',
    'MAPSEC_AETHERON_AETHER_VILLAGE', MUS_INN, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('AETHER_VILLAGE_POKEMON_CENTER'),
    warps=[
        warp(7, 8, M('AETHER_VILLAGE'), 0),    # 0: out to the village
        warp(6, 8, M('AETHER_VILLAGE'), 0),    # 1
    ],
    objects=[
        obj(GFX_NURSE, 7, 2, 'AetheronAetherVillagePokemonCenter_EventScript_Nurse',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_VILLAGE_NURSE'),
    ])

# =====================================================================
#  8. CovenantInstallation (22x18) — angular industrial exterior, 2 officer trainers
# =====================================================================
mapjson('Aetheron_CovenantInstallation', 'LAYOUT_AETHERON_COVENANT_INSTALLATION',
    'MAPSEC_AETHERON_COVENANT_INSTALLATION', MUS_INSTAL, 'MAP_TYPE_ROUTE', weather=STORM,
    mapid=M('COVENANT_INSTALLATION'),
    connections=[conn('left', -1, M('AETHER_VILLAGE'))],
    warps=[
        warp(10, 5, M('COVENANT_INSTALLATION_INTERIOR'), 0),   # 0: into the facility
    ],
    objects=[
        # Covenant officer sight trainer (exterior).
        obj(GFX_OFFICER, 6, 9, 'AetheronCovenantInstallation_EventScript_Officer1',
            movement='MOVEMENT_TYPE_FACE_DOWN', trainer_type='TRAINER_TYPE_NORMAL',
            sight='4', local_id='LOCALID_AETHERON_OFFICER1'),
        # A second covenant officer sight trainer (exterior).
        obj(GFX_OFFICER, 15, 9, 'AetheronCovenantInstallation_EventScript_Officer2',
            movement='MOVEMENT_TYPE_FACE_DOWN', trainer_type='TRAINER_TYPE_NORMAL',
            sight='4', local_id='LOCALID_AETHERON_INSTALL_OFFICER'),
    ],
    coords=[
        # Installation-discovered atmosphere on first approach (progress 2, after Gym2).
        trigger(10, 16, 'VAR_AETHERON_PROGRESS', 2, 'AetheronCovenantInstallation_EventScript_Approach'),
        trigger(11, 16, 'VAR_AETHERON_PROGRESS', 2, 'AetheronCovenantInstallation_EventScript_Approach'),
    ])

# =====================================================================
#  9. CovenantInstallation_Interior (MOSSDEEP_SPACE_CENTER_1F) — Gym3 Voss + DEFECTION
# =====================================================================
# Roomy tech floor. Gym 3 (Voss). Control panel bg_event. THE CASS DEFECTION 3-char
# tableau: Voss (defeated, near the panel), Cass appears in the DOORWAY (the north
# stairs warp tile area, (13,1)), and the player. The Cass-doorway object is hidden
# until FLAG_AETHERON_GYM3_CLEAR (defection fires after Voss is beaten). Open floor
# between Voss (centre) and the doorway gives a clean readable tableau.
mapjson('Aetheron_CovenantInstallation_Interior', 'LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F',
    'MAPSEC_AETHERON_COVENANT_INSTALLATION', MUS_INSTAL, 'MAP_TYPE_INDOOR', weather=NONE,
    indoor=True, mapid=M('COVENANT_INSTALLATION_INTERIOR'),
    battle_scene='MAP_BATTLE_SCENE_GYM',
    warps=[
        warp(7, 9, M('COVENANT_INSTALLATION'), 0),   # 0: out to the exterior
        warp(8, 9, M('COVENANT_INSTALLATION'), 0),   # 1
        warp(13, 1, M('STORM_PEAK'), 0),             # 2: deeper -> StormPeak (the back stairs)
    ],
    objects=[
        # Gym 3 - Commander Voss (Electric/Steel). Talk-initiated.
        obj(GFX_VOSS, 8, 5, 'AetheronCovenantInstallationInterior_EventScript_Voss',
            movement='MOVEMENT_TYPE_FACE_DOWN',
            local_id='LOCALID_AETHERON_VOSS'),
        # Covenant officer sight trainer (interior). On open floor (3,3).
        obj(GFX_OFFICER, 3, 3, 'AetheronCovenantInstallationInterior_EventScript_Officer2',
            movement='MOVEMENT_TYPE_FACE_DOWN', trainer_type='TRAINER_TYPE_NORMAL',
            sight='3', local_id='LOCALID_AETHERON_INTERIOR_OFFICER'),
        # CASS DEFECTION object - appears in the DOORWAY (north stairs, (13,2) just
        # below the (13,1) warp). Hidden until FLAG_AETHERON_GYM3_CLEAR; the defection
        # scene (script-writer) shows + choreographs her here. 3-char tableau:
        # Voss at (8,5), Cass at (13,2), player on the open floor between them.
        obj(GFX_CASS, 13, 2, 'AetheronCovenantInstallationInterior_EventScript_CassDefection',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_AETHERON_GYM3_CLEAR',
            local_id='LOCALID_AETHERON_CASS_DEFECT'),
    ],
    bgs=[
        # Control panel - examine for the yield log; sets FLAG_AETHERON_INSTALLATION_FOUND.
        sign(11, 3, 'AetheronCovenantInstallationInterior_EventScript_ControlPanel',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  10. StormPeak (18x20) — highest point, extraction apparatus, post-defection Cass
# =====================================================================
# <- Installation_Interior (warp 2), -> SealChamber (warp). The post-defection Cass
# object "walks alongside" (script-managed; placed near the player's entry). The
# extraction apparatus as a sign bg_event. Lightning-strike FX = script TODO.
mapjson('Aetheron_StormPeak', 'LAYOUT_AETHERON_STORM_PEAK',
    'MAPSEC_AETHERON_STORM_PEAK', MUS_PEAK, 'MAP_TYPE_ROUTE', weather=STORM,
    mapid=M('STORM_PEAK'), show_name=True,
    warps=[
        warp(8, 18, M('COVENANT_INSTALLATION_INTERIOR'), 2),   # 0: back down to the facility
        warp(12, 4, M('SEAL_CHAMBER'), 0),                     # 1: into the SealChamber (platform tile east of the rig)
    ],
    objects=[
        # Post-defection Cass - hidden during normal play (FLAG_AETHERON_CASS_SEEN as
        # a placeholder hide flag); the defection/StormPeak scripts manage her real
        # "walks alongside" visibility at runtime. Placed by the south entry.
        obj(GFX_CASS, 9, 16, 'AetheronStormPeak_EventScript_CassAlongside',
            movement='MOVEMENT_TYPE_FACE_UP', flag='FLAG_AETHERON_CASS_SEEN',
            local_id='LOCALID_AETHERON_CASS_PEAK'),
    ],
    coords=[
        # StormPeak arrival / apparatus reveal (progress 4, after the defection).
        trigger(8, 17, 'VAR_AETHERON_PROGRESS', 4, 'AetheronStormPeak_EventScript_Arrival'),
        trigger(9, 17, 'VAR_AETHERON_PROGRESS', 4, 'AetheronStormPeak_EventScript_Arrival'),
    ],
    bgs=[
        # The extraction apparatus (largest yet) - examine sign at the rig's south face.
        sign(8, 7, 'AetheronStormPeak_EventScript_Apparatus',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  11. SealChamber (SEALED_CHAMBER_INNER_ROOM) — eye of the storm, bright + still
# =====================================================================
# The tonal OPPOSITE of Ashenveil's MorthasGrove: bright, silent, still. Space for
# player + Cass + Stormveil (SS_TIDAL decoration). The extraction apparatus
# (CABLE_CAR object). The STORM COMPASS as a SCRIPTED examine bg_event on the
# apparatus wreckage AFTER the seal (gives ITEM_STORM_COMPASS + FLAG, NOT a hidden
# item). Discovery triggers (row 14, progress==appropriate). Warp back to StormPeak.
mapjson('Aetheron_SealChamber', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_AETHERON_STORM_PEAK', MUS_PEAK, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('SEAL_CHAMBER'),
    warps=[
        warp(10, 19, M('STORM_PEAK'), 1),   # 0: back out to StormPeak
    ],
    objects=[
        # Stormveil - the sealed legendary, pure decoration (script 0x0), present.
        obj(GFX_STORMVEIL, 10, 5, '0x0',
            movement='MOVEMENT_TYPE_NONE', elevation=1,
            local_id='LOCALID_AETHERON_STORMVEIL'),
        # The extraction apparatus (the seal reinforcement point) - examine.
        obj(GFX_APPARATUS, 10, 8, 'AetheronSealChamber_EventScript_Apparatus',
            movement='MOVEMENT_TYPE_NONE', elevation=3,
            local_id='LOCALID_AETHERON_SEAL_APPARATUS'),
        # Cass - present in the chamber alongside the player (script-managed; hidden
        # placeholder flag, the seal-reinforcement scene choreographs her).
        obj(GFX_CASS, 12, 9, 'AetheronSealChamber_EventScript_CassChamber',
            movement='MOVEMENT_TYPE_FACE_LEFT', flag='FLAG_AETHERON_CASS_SEEN',
            local_id='LOCALID_AETHERON_CASS_CHAMBER'),
    ],
    coords=[
        # SealChamber discovery (row 14) at progress 5 (entered with Cass).
        trigger(10, 14, 'VAR_AETHERON_PROGRESS', 5, 'AetheronSealChamber_EventScript_Discovery'),
        trigger(9, 14, 'VAR_AETHERON_PROGRESS', 5, 'AetheronSealChamber_EventScript_Discovery'),
    ],
    bgs=[
        # The STORM COMPASS in the apparatus wreckage - SCRIPTED examine (gives
        # ITEM_STORM_COMPASS + FLAG_STORM_COMPASS_OBTAINED). NOT a hidden item.
        sign(11, 8, 'AetheronSealChamber_EventScript_StormCompass',
             facing='BG_EVENT_PLAYER_FACING_ANY'),
    ])

print('Aetheron map.json files written (11 maps)')
