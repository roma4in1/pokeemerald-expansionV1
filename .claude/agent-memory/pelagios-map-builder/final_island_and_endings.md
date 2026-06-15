---
name: final-island-and-endings
description: Convergence (final island) map patterns - story-only no-heal/no-boat-object island, 3-ending single-trigger geometry, inverse-flag returning-NPC visibility, vast-temple reuse, and a heal_locations.json truncation gotcha
metadata:
  type: project
---

Built Convergence (the FINAL ISLAND, 6 maps, 2026-06-14,
`tools/pelagios/build_convergence*.py`). Patterns worth reusing / remembering:

**Story-only island checklist deltas (like Ashenveil, more extreme):** NO heal
location, NO inn, NO wild encounters, NO trainers/gym OBJECTS (the one trainer,
Dorne, is a TRAINER_TYPE_NONE talk/script-initiated final battle), NO hidden items,
and NO Tennyson/SS_TIDAL BOARDING object + NO return warp (return travel is the Storm
Compass). The inbound arrival is still wired: the boat-menu handler
`Pelagios_EventScript_SailToConvergence` (already FLAG_AETHERON_RESOLVED-gated and
pre-existing from the Aetheron sea-chart system) had its post-gate `goto SailNoChart`
stub replaced with same-island-guard + cast-off + `warp MAP_CONVERGENCE_APPROACH, 11, 13`.
So "no boat registration on the island" means no boarding OBJECT, NOT skipping the
menu warp swap-in - the handler already existed and needed a real target.

**Three-ending geometry = ONE coord trigger, all branching in script.** All three
endings play in a single KillSwitchChamber map off ONE coord trigger
(VAR_CONVERGENCE_PROGRESS==3, fired on the first step in - NOT the landing tile). The
script-writer branches on FLAG_DORNE_CHOICE_STOP/HELP + FLAG_TRUE_ENDING_UNLOCKED. The
epilogue is ONE map with the FULL final-cast roster placed as objects, all defaulting
hidden behind FLAG_<ISLAND>_COMPLETE; the script-writer's ON_TRANSITION reveals the
per-ending subset. Credits = a coord trigger (progress 5) on the epilogue. The map-builder
provides geometry + objects + triggers only; never the branch logic.

**INVERSE-FLAG returning-NPC visibility (recurring final-act problem).** When a brief
wants an NPC present ONLY IF a flag is SET (e.g. Eira if FLAG_SCHISM_CEASEFIRE, Drenn if
FLAG_DRENN_ALIVE), the object's spawn `flag` field is the WRONG tool - the engine HIDES on
flag SET. Set the spawn flag to that flag anyway as a placeholder, but the MAP'S
ON_TRANSITION (script-writer) is AUTHORITATIVE: addobject when set / removeobject when
unset. This is the Gildhaven-Dagan precedent; document it loudly in the handoff because the
spawn flag alone produces the opposite of the intended behavior.

**Vast/most-ancient interiors:** reuse LAYOUT_SEALED_CHAMBER_OUTER_ROOM (temple outer,
murals - 8 wall-section examine signs; west/east signs sit ON the impassable wall faced
from adjacent floor, which is correct) + LAYOUT_SEALED_CHAMBER_INNER_ROOM (inner sanctum).
Its forward (deeper) warp must land on the inner room's NORTH WALKABLE BAND (rows 3-5,
e.g. (10,5)) - the top-center (10,3) is a WALL. Outer-room exit warps land in the SW/SE
floor pockets ((5,18)/(16,18)), never (10,17-19) (those are walls - see
[[tileset-palettes]] gotcha). For the kill-switch chamber itself use the VAST-ROOM recipe
([[underwater-and-vast-rooms]]) in General+Cave (deepest aesthetic): floor 0x4211 (col0
elev4), wall 0x0611 (col1) sampled from LAYOUT_VICTORY_ROAD_1F.

**TILESET: no gTileset_Ruins exists.** "Ancient ruins" intent = gTileset_Slateport (grey
stone, Ashenveil dead-city precedent) for outdoor capital/approach/epilogue + the Sealed
Chamber family (gTileset_Cave) for temple interiors + the deepest chamber.

**heal_locations.json TRUNCATION GOTCHA (cost a debug cycle).** A prior crashed session left
src/data/heal_locations.json truncated mid-write at the Primalis entry (`"map": "MAP_PRIM` +
unescaped newline) - jsonproc fails BEFORE any new map can link, with a misleading
`control character U+000A must be escaped` error pointing at the truncation line. It also
silently DROPPED the never-written Aetheron heal location, so Aetheron scripts'
`setrespawn HEAL_LOCATION_AETHERON_CLOUD_LANDING` then failed at LINK with "undefined
reference". Lesson: if a build dies in jsonproc on heal_locations/region_map/map_groups JSON,
first `python3 -c "import json; json.load(open(...))"` EACH of those files to find the
truncation, and check that every island's heal location actually made it in (grep the
HEAL_LOCATION_* names referenced in scripts vs. defined in the json). Repair by completing
the entry with the standard shape (id/map/x:4/y:8/respawn_map=<Inn>/respawn_npc=<innkeeper
localid>); Inns are nurse-gfx so they stay OUT of IsLastHealLocationPlayerHouse().
