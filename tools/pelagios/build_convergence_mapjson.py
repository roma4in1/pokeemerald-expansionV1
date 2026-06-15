#!/usr/bin/env python3
"""Generate map.json for all 6 Convergence (FINAL ISLAND) maps.
Mirrors build_ashenveil_mapjson.py.

HARD CONSTRAINTS (this island):
  - NO heal location (no inn, no nurse). Do not register one.
  - NO wild encounters. NO trainers / gym leaders objects.
  - NO boat / Tennyson boarding object, NO return warp anywhere (Storm Compass is
    the return travel). The inbound arrival is handled by the boat menu handler
    (Pelagios_EventScript_SailToConvergence) warping to the Approach arrival tile.
  - NO hidden items.
  - Coord triggers compare VAR_CONVERGENCE_PROGRESS (never a FLAG in the var field).

The 6 maps:
  1 Approach                    composed (24x18 usable)  General+Slateport  shore gathering
  2 AncientCapital_Exterior     composed LARGE 36x34     General+Slateport  returning NPCs
  3 AncientCapital_Interior1    LAYOUT_SEALED_CHAMBER_OUTER_ROOM  temple outer / murals
  4 AncientCapital_Interior2    LAYOUT_SEALED_CHAMBER_INNER_ROOM  inner sanctum / switch ahead
  5 KillSwitchChamber           composed VAST 26x40      General+Cave       the mechanism
  6 Epilogue                    composed 24x20           General+Slateport  flag-gated roster

Returning-NPC visibility (Exterior):
  - Eira    : object 'flag' = FLAG_SCHISM_CEASEFIRE. NOTE: the engine HIDES an
              object when its flag is SET. The brief wants Eira PRESENT only if
              FLAG_SCHISM_CEASEFIRE. That is the INVERSE of the spawn flag, so the
              script-writer must drive her visibility from the Exterior
              ON_TRANSITION (removeobject when ceasefire unset, addobject when set),
              exactly like Gildhaven's Dagan. We set her spawn flag to
              FLAG_SCHISM_CEASEFIRE anyway so she defaults hidden once the flag is
              set unless the transition overrides - the transition is authoritative.
              Documented for the script-writer.
  - Drenn   : same inverse-flag problem with FLAG_DRENN_ALIVE (present only if alive,
              but engine hides on SET). Spawn flag = FLAG_DRENN_ALIVE; the Exterior
              ON_TRANSITION must addobject when FLAG_DRENN_ALIVE set / removeobject
              when unset. Documented for the script-writer (Gildhaven Dagan precedent).
  - Mako, Lace, The Lens, Dagan : always present (flag '0').
  - Dex is mentioned in the brief as arriving if FLAG_DEX_ALIVE, but Dex has no
    Convergence object placed (the gathering/epilogue script-writer can addobject a
    Dex cameo if wanted; we leave the roster to the 6 placed returning NPCs + the
    flag-gated epilogue copies). Documented as a script-writer option.

All EventScript labels are TODO stubs filled by build_convergence_scripts_stubs.py
(this island has NO Tennyson exception - no boat wiring here).
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

def mapjson(name, layout, mapsec, music, map_type, weather='WEATHER_NONE',
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

M = lambda s: 'MAP_CONVERGENCE_' + s
NONE = 'WEATHER_NONE'

# Music placeholders (documented in CLAUDE.md; swap to themed tracks later)
MUS_APPROACH = 'MUS_ABNORMAL_WEATHER'   # ominous arrival / fleet offshore
MUS_CAPITAL  = 'MUS_SEALED_CHAMBER'     # the ancient capital
MUS_TEMPLE   = 'MUS_SEALED_CHAMBER'     # temple interiors
MUS_SWITCH   = 'MUS_SEALED_CHAMBER'     # the kill switch chamber
MUS_EPILOGUE = 'MUS_LITTLEROOT'         # calm post-ending (Haven-warm)

# sprite placeholders (no Convergence-specific gfx; documented in CLAUDE.md)
GFX_DORNE = 'OBJ_EVENT_GFX_MAN_4'           # Marshal Vael Dorne
GFX_SOLLIS = 'OBJ_EVENT_GFX_WOMAN_3'        # Prof. Maren Sollis (FEMALE_SCIENTIST placeholder family)
GFX_CASS  = 'OBJ_EVENT_GFX_RIVAL_MAY_NORMAL'  # rival Cass placeholder
GFX_EIRA  = 'OBJ_EVENT_GFX_WOMAN_5'         # Eira (Schism ice leader)
GFX_MAKO  = 'OBJ_EVENT_GFX_HIKER'           # Elder Mako (no shark/elder gfx)
GFX_DRENN = 'OBJ_EVENT_GFX_SCIENTIST_1'     # Drenn (Schism poison leader)
GFX_LACE  = 'OBJ_EVENT_GFX_WOMAN_2'         # Lace Vane (Gildhaven)
GFX_LENS  = 'OBJ_EVENT_GFX_SCIENTIST_1'     # The Lens (Thalvern, continuing Dex's work)
GFX_DAGAN = 'OBJ_EVENT_GFX_RICH_BOY'        # Dagan (Sirocco/Gildhaven recurring)
GFX_SWITCH = 'OBJ_EVENT_GFX_SS_TIDAL'       # kill-switch mechanism (huge decoration)
GFX_FLEET = 'OBJ_EVENT_GFX_SS_TIDAL'        # Covenant fleet ships offshore (bg detail)

# =====================================================================
#  1. Convergence_Approach (24x20) — shore gathering point
# =====================================================================
# Boat menu warps the player here (arrival tile (11,13) on the jetty foot). The
# gathering scene fires from a coord trigger one tile north of the landing (the
# player's first natural step), comparing VAR_CONVERGENCE_PROGRESS == 0. Dorne is
# already here; Sollis + Cass are script-managed (objects present for the scene).
# Covenant fleet implied offshore via decorative SS_TIDAL ship objects in the sea.
# NO return warp (Storm Compass). Connects up -> AncientCapital_Exterior.
mapjson('Convergence_Approach', 'LAYOUT_CONVERGENCE_APPROACH',
    'MAPSEC_CONVERGENCE_APPROACH', MUS_APPROACH, 'MAP_TYPE_TOWN', weather=NONE,
    mapid=M('APPROACH'),
    connections=[conn('up', -6, M('ANCIENT_CAPITAL_EXTERIOR'))],
    objects=[
        # Dorne - already at the shore when the player lands (always present here).
        obj(GFX_DORNE, 11, 9, 'ConvergenceApproach_EventScript_Dorne',
            local_id='LOCALID_CONVERGENCE_DORNE'),
        # Sollis - arrives by boat during the scene. Hidden until the gathering
        # scene reveals her (script-writer addobjects her on the boat arrival).
        # Spawn flag FLAG_CONVERGENCE_GATHERING_SEEN: hidden once the scene played;
        # the scene itself drives her entrance. Documented for the script-writer.
        obj(GFX_SOLLIS, 12, 13, 'ConvergenceApproach_EventScript_Sollis',
            flag='FLAG_CONVERGENCE_GATHERING_SEEN',
            local_id='LOCALID_CONVERGENCE_SOLLIS'),
        # Cass - arrives alongside the player.
        obj(GFX_CASS, 13, 10, 'ConvergenceApproach_EventScript_Cass',
            local_id='LOCALID_CONVERGENCE_CASS'),
        # Covenant fleet offshore (pure bg detail; script 0x0, elevation 1).
        obj(GFX_FLEET, 4, 18, '0x0', elevation=1, movement='MOVEMENT_TYPE_NONE',
            local_id='LOCALID_CONVERGENCE_FLEET1'),
        obj(GFX_FLEET, 16, 18, '0x0', elevation=1, movement='MOVEMENT_TYPE_NONE',
            local_id='LOCALID_CONVERGENCE_FLEET2'),
    ],
    coords=[
        # Gathering scene: fires on the first step north off the landing jetty.
        # Landing tile is (11,13)/(12,13); trigger row at y11 (NOT the landing tile).
        trigger(11, 11, 'VAR_CONVERGENCE_PROGRESS', 0, 'ConvergenceApproach_EventScript_Gathering'),
        trigger(12, 11, 'VAR_CONVERGENCE_PROGRESS', 0, 'ConvergenceApproach_EventScript_Gathering'),
    ])

# =====================================================================
#  2. Convergence_AncientCapital_Exterior (36x34) — LARGE ruined greatest city
# =====================================================================
# Returning NPCs at the cardinal edges; the temple path is the north connection
# into Interior1. Connects down -> Approach, up -> Interior1.
mapjson('Convergence_AncientCapital_Exterior', 'LAYOUT_CONVERGENCE_ANCIENT_CAPITAL_EXTERIOR',
    'MAPSEC_CONVERGENCE_ANCIENT_CAPITAL', MUS_CAPITAL, 'MAP_TYPE_CITY', weather=NONE,
    mapid=M('ANCIENT_CAPITAL_EXTERIOR'),
    connections=[
        conn('down', 6, M('APPROACH')),
        conn('up', -6, M('ANCIENT_CAPITAL_INTERIOR1')),
    ],
    objects=[
        # Eira - north edge. Present ONLY if FLAG_SCHISM_CEASEFIRE (inverse of the
        # engine spawn flag). spawn flag set to FLAG_SCHISM_CEASEFIRE; the Exterior
        # ON_TRANSITION (script-writer) is AUTHORITATIVE: removeobject when ceasefire
        # unset, addobject when set (Gildhaven Dagan inverse-flag precedent).
        obj(GFX_EIRA, 17, 3, 'ConvergenceExterior_EventScript_Eira',
            movement='MOVEMENT_TYPE_FACE_DOWN', flag='FLAG_SCHISM_CEASEFIRE',
            local_id='LOCALID_CONVERGENCE_EIRA'),
        # Mako - east (always present).
        obj(GFX_MAKO, 31, 15, 'ConvergenceExterior_EventScript_Mako',
            movement='MOVEMENT_TYPE_FACE_LEFT', local_id='LOCALID_CONVERGENCE_MAKO'),
        # Drenn - south. Present ONLY if FLAG_DRENN_ALIVE (inverse of spawn flag).
        # spawn flag = FLAG_DRENN_ALIVE; Exterior ON_TRANSITION authoritative:
        # addobject when FLAG_DRENN_ALIVE set, removeobject when unset.
        obj(GFX_DRENN, 17, 30, 'ConvergenceExterior_EventScript_Drenn',
            movement='MOVEMENT_TYPE_FACE_UP', flag='FLAG_DRENN_ALIVE',
            local_id='LOCALID_CONVERGENCE_DRENN'),
        # Lace - west (always).
        obj(GFX_LACE, 3, 15, 'ConvergenceExterior_EventScript_Lace',
            movement='MOVEMENT_TYPE_FACE_RIGHT', local_id='LOCALID_CONVERGENCE_LACE'),
        # The Lens - central crossing, cataloguing everything (always).
        obj(GFX_LENS, 18, 17, 'ConvergenceExterior_EventScript_Lens',
            local_id='LOCALID_CONVERGENCE_LENS'),
        # Dagan - leaning on an east wall, eating something (always).
        obj(GFX_DAGAN, 24, 20, 'ConvergenceExterior_EventScript_Dagan',
            movement='MOVEMENT_TYPE_FACE_LEFT', local_id='LOCALID_CONVERGENCE_DAGAN'),
    ])

# =====================================================================
#  3. AncientCapital_Interior1 (SEALED_CHAMBER_OUTER_ROOM) — temple outer / murals
# =====================================================================
# The temple's outer chamber. Murals on every wall section = examine bg_events.
# Dorne is here (the confrontation begins). Connects (warps) Exterior <-> Interior2.
# SEALED_CHAMBER_OUTER_ROOM is 21x23. Entrance from the south (cols 9-11 walkable
# bottom pockets are 3-7 and 13-17 per memory; the inner-room warp is at (10,2)).
# We land the Exterior->Interior1 warp in the SW pocket and put the Interior2 warp
# at the vanilla (10,2) north door.
mapjson('Convergence_AncientCapital_Interior1', 'LAYOUT_SEALED_CHAMBER_OUTER_ROOM',
    'MAPSEC_CONVERGENCE_ANCIENT_CAPITAL', MUS_TEMPLE, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('ANCIENT_CAPITAL_INTERIOR1'),
    warps=[
        warp(5, 18, M('ANCIENT_CAPITAL_EXTERIOR'), 1),   # 0: out to the capital (SW pocket, walkable)
        warp(16, 18, M('ANCIENT_CAPITAL_EXTERIOR'), 1),  # 1
        warp(10, 2, M('ANCIENT_CAPITAL_INTERIOR2'), 0),  # 2: deeper, into the inner sanctum
    ],
    objects=[
        # Dorne - the confrontation begins here (always present in Interior1).
        # (10,7) is open central floor (10,8 is a pillar = impassable).
        obj(GFX_DORNE, 10, 7, 'ConvergenceInterior1_EventScript_Dorne',
            local_id='LOCALID_CONVERGENCE_I1_DORNE'),
    ],
    bgs=[
        # Murals on every wall section - examine for extended lore (TODO scripts).
        # One bg_event per wall section around the chamber (8 sections).
        sign(3, 4, 'ConvergenceInterior1_EventScript_MuralNW', facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(7, 4, 'ConvergenceInterior1_EventScript_MuralN1', facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(13, 4, 'ConvergenceInterior1_EventScript_MuralN2', facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(17, 4, 'ConvergenceInterior1_EventScript_MuralNE', facing='BG_EVENT_PLAYER_FACING_NORTH'),
        sign(2, 9, 'ConvergenceInterior1_EventScript_MuralW1', facing='BG_EVENT_PLAYER_FACING_WEST'),
        sign(2, 13, 'ConvergenceInterior1_EventScript_MuralW2', facing='BG_EVENT_PLAYER_FACING_WEST'),
        sign(18, 9, 'ConvergenceInterior1_EventScript_MuralE1', facing='BG_EVENT_PLAYER_FACING_EAST'),
        sign(18, 13, 'ConvergenceInterior1_EventScript_MuralE2', facing='BG_EVENT_PLAYER_FACING_EAST'),
    ])

# =====================================================================
#  4. AncientCapital_Interior2 (SEALED_CHAMBER_INNER_ROOM) — inner sanctum
# =====================================================================
# The inner sanctum. The kill switch is visible ahead (an examine sign on the
# north dais - the actual mechanism + endings are in the KillSwitchChamber). Dorne
# explains; the player makes / confirms the final choice. Connects Interior1 <->
# KillSwitchChamber. INNER_ROOM is 21x23; vanilla exit warp is (10,19) -> outer.
mapjson('Convergence_AncientCapital_Interior2', 'LAYOUT_SEALED_CHAMBER_INNER_ROOM',
    'MAPSEC_CONVERGENCE_ANCIENT_CAPITAL', MUS_TEMPLE, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('ANCIENT_CAPITAL_INTERIOR2'),
    warps=[
        warp(10, 19, M('ANCIENT_CAPITAL_INTERIOR1'), 2),  # 0: back up to the outer chamber
        warp(11, 19, M('ANCIENT_CAPITAL_INTERIOR1'), 2),  # 1
        warp(10, 5, M('KILL_SWITCH_CHAMBER'), 0),         # 2: forward (north walkable band) into the kill switch chamber
        warp(11, 5, M('KILL_SWITCH_CHAMBER'), 0),         # 3
    ],
    objects=[
        # Dorne - explains what the switch does (again, clearly), final choice here.
        # (10,8) is open central floor in the inner sanctum.
        obj(GFX_DORNE, 10, 8, 'ConvergenceInterior2_EventScript_Dorne',
            local_id='LOCALID_CONVERGENCE_I2_DORNE'),
    ],
    coords=[
        # The final-choice scene fires walking up into the sanctum (progress 2 =
        # temple entered). One tile south of the forward (kill switch) door.
        trigger(10, 6, 'VAR_CONVERGENCE_PROGRESS', 2, 'ConvergenceInterior2_EventScript_FinalChoice'),
        trigger(11, 6, 'VAR_CONVERGENCE_PROGRESS', 2, 'ConvergenceInterior2_EventScript_FinalChoice'),
    ],
    bgs=[
        # The kill switch visible ahead through the sanctum arch - examine sign.
        sign(10, 4, 'ConvergenceInterior2_EventScript_KillSwitchAhead',
             facing='BG_EVENT_PLAYER_FACING_NORTH'),
    ])

# =====================================================================
#  5. Convergence_KillSwitchChamber (26x40) — the mechanism, vast + ancient
# =====================================================================
# Warp lands the player at the south entrance (12/13,37 walkable spine). All three
# endings play here (script-writer). Player + Dorne + Cass space. The kill-switch
# mechanism object is on the raised north dais (12,4). NO return warp - the only
# exits are the ending sequences (which warp to the Epilogue). The ending trigger
# fires inside the entrance (progress 3 = inner sanctum cleared), NOT on the
# landing tile.
mapjson('Convergence_KillSwitchChamber', 'LAYOUT_CONVERGENCE_KILL_SWITCH_CHAMBER',
    'MAPSEC_CONVERGENCE_KILL_SWITCH', MUS_SWITCH, 'MAP_TYPE_UNDERGROUND', weather=NONE,
    indoor=True, mapid=M('KILL_SWITCH_CHAMBER'),
    warps=[
        # 0: arrival from Interior2 (lands on the walkable south spine, NOT a trigger).
        warp(12, 37, M('ANCIENT_CAPITAL_INTERIOR2'), 2),
        warp(13, 37, M('ANCIENT_CAPITAL_INTERIOR2'), 2),
    ],
    objects=[
        # The kill switch mechanism - enormous ancient decoration on the dais.
        obj(GFX_SWITCH, 12, 4, 'ConvergenceKillSwitch_EventScript_Mechanism',
            elevation=1, movement='MOVEMENT_TYPE_NONE',
            local_id='LOCALID_CONVERGENCE_KILLSWITCH'),
        # Dorne at the mechanism (always present here for the endings).
        obj(GFX_DORNE, 14, 7, 'ConvergenceKillSwitch_EventScript_Dorne',
            local_id='LOCALID_CONVERGENCE_KS_DORNE'),
        # Cass beside the player (always present here).
        obj(GFX_CASS, 11, 9, 'ConvergenceKillSwitch_EventScript_Cass',
            local_id='LOCALID_CONVERGENCE_KS_CASS'),
    ],
    coords=[
        # Ending sequence fires on the first step north into the chamber (progress
        # 3). Branches on FLAG_DORNE_CHOICE_* / FLAG_TRUE_ENDING_UNLOCKED (script).
        trigger(12, 34, 'VAR_CONVERGENCE_PROGRESS', 3, 'ConvergenceKillSwitch_EventScript_Ending'),
        trigger(13, 34, 'VAR_CONVERGENCE_PROGRESS', 3, 'ConvergenceKillSwitch_EventScript_Ending'),
    ])

# =====================================================================
#  6. Convergence_Epilogue (24x20) — single neutral post-ending stage
# =====================================================================
# Per-ending NPC positions are flag-gated (the script-writer sets each object's
# visibility from an ON_TRANSITION keyed off FLAG_ENDING_*_PLAYED). The credits
# trigger fires from here (script-writer). NO warps (terminal map; the ending
# sequence warps the player IN, post-credits the Storm Compass handles return).
# We place the full Ending-3 roster as objects, all defaulting hidden behind the
# per-ending flags; the script-writer's ON_TRANSITION reveals the right subset.
mapjson('Convergence_Epilogue', 'LAYOUT_CONVERGENCE_EPILOGUE',
    'MAPSEC_CONVERGENCE_APPROACH', MUS_EPILOGUE, 'MAP_TYPE_TOWN', weather=NONE,
    mapid=M('EPILOGUE'),
    objects=[
        # Sollis - Ending 1 (Haven epilogue) and Ending 3 (full roster).
        obj(GFX_SOLLIS, 9, 8, 'ConvergenceEpilogue_EventScript_Sollis',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_SOLLIS'),
        # Cass - present in the True-ending epilogue (final line of the game).
        obj(GFX_CASS, 12, 9, 'ConvergenceEpilogue_EventScript_Cass',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_CASS'),
        # Dorne - present in Endings 2 and 3 epilogue.
        obj(GFX_DORNE, 14, 8, 'ConvergenceEpilogue_EventScript_Dorne',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_DORNE'),
        # Ending-3 full roster (script-writer reveals via ON_TRANSITION when
        # FLAG_ENDING_TRUE_PLAYED). All default hidden behind FLAG_CONVERGENCE_COMPLETE.
        obj(GFX_EIRA, 6, 7, 'ConvergenceEpilogue_EventScript_Eira',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_EIRA'),
        obj(GFX_MAKO, 16, 7, 'ConvergenceEpilogue_EventScript_Mako',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_MAKO'),
        obj(GFX_LENS, 7, 11, 'ConvergenceEpilogue_EventScript_Lens',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_LENS'),
        obj(GFX_DRENN, 17, 11, 'ConvergenceEpilogue_EventScript_Drenn',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_DRENN'),
        obj(GFX_LACE, 6, 9, 'ConvergenceEpilogue_EventScript_Lace',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_LACE'),
        obj(GFX_DAGAN, 17, 9, 'ConvergenceEpilogue_EventScript_Dagan',
            flag='FLAG_CONVERGENCE_COMPLETE', local_id='LOCALID_CONVERGENCE_EP_DAGAN'),
    ],
    coords=[
        # Credits trigger: fires once the player steps into the epilogue stage
        # (progress 5 = an ending played). The script-writer rolls credits here.
        trigger(11, 12, 'VAR_CONVERGENCE_PROGRESS', 5, 'ConvergenceEpilogue_EventScript_Credits'),
        trigger(12, 12, 'VAR_CONVERGENCE_PROGRESS', 5, 'ConvergenceEpilogue_EventScript_Credits'),
    ])

print('Convergence map.json files written (6 maps)')
