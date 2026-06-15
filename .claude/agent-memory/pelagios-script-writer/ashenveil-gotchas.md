---
name: ashenveil-gotchas
description: Verified scripting patterns for Ashenveil's two climax maps (Dorne 3-choice, Morthas silent-vision endurance, cipher-9 / true-ending, sea-chart Aetheron gate)
metadata:
  type: project
---

Patterns/constraints verified building Ashenveil_TheMemorial + Ashenveil_MorthasGrove
(2026-06-14, gmake exit 0). Reusable for Aetheron/Convergence climax scenes.

**Why:** these are the game's emotional-climax scenes; each pattern was checked
against the engine and prior islands so the next agent can reuse without re-deriving.

**How to apply:** read before writing any future climax/choice/vision scene.

- 3-WAY STORY CHOICE without a C multichoice list: chain two `MSGBOX_YESNO` prompts
  (established Schism/Drenn pattern). Prompt A YES -> option 1; A NO -> prompt B;
  B YES -> option 2; B NO -> option 3 (fall-through). Set EXACTLY one flag per branch.
  Custom `multichoice` macros need a list ID registered in src/data/script_menu.h (C) -
  forbidden for the script-writer. Do NOT invent a list.
- HIDDEN CAMEO REVEAL FOR A TRIGGERED SCENE: object has `flag` = its hide flag in
  map.json (default 0 = hidden by setflag). To show it mid-scene: `clearflag <hide>`
  then `addobject LOCALID`; after it walks off, `removeobject LOCALID` + re-`setflag
  <hide>` so it stays gone on every future load. (TheMemorial Dorne uses
  FLAG_ASHENVEIL_DORNE_MET as both hide flag AND scene-done flag.)
- SILENT VISION (music must stop): `fadeoutbgm 4` produces silence, pace the vision
  with `delay` (90 ticks ~ a few seconds for the "six seconds" beat), then
  `fadedefaultbgm` restores the map's default track. Both macros exist in
  asm/macros/event.inc. Verified working in the Morthas final vision.
- ENDURANCE / multi-turn loop counter: use `VAR_TEMP_1` (NOT a permanent spare -
  only 0x410D/E/F remain reserved, do not consume). Temp vars reset to 0 on map load,
  so "flee and retry" restarts cleanly with zero permanent state. Loop with
  `goto_if_eq VAR_TEMP_1, N` dispatch + `addvar VAR_TEMP_1, 1` +
  `goto_if_ge VAR_TEMP_1, 5, <done>`. The whole sequence runs inside one locked
  trigger invocation (player never walks between turns), so the loop is self-contained.
- ORDER critical state BEFORE a full-bag-safe `giveitem`: set all story flags
  (resolution / cipher / true-ending) and progress var FIRST, then `giveitem` the
  key item. A full bag silently loses the item but never strands the story. Cipher-9
  text is shown LAST (it is the emotional climax; nothing follows "I love you").
- SEA CHART -> AETHERON: SeaChartStone sets FLAG_SEA_CHART_FOUND. The boat menu's
  Pelagios_EventScript_SailToAetheron checks `goto_if_unset FLAG_SEA_CHART_FOUND` -
  do NOT edit the boat menu; just set the flag. Until Aetheron's port is built the
  menu falls through to SailNoChart ("no port yet") even with the chart - expected.
- FLAG_ASHENVEIL_RESOLVED does NOT exist in flags.h. Ashenveil's resolution flag is
  FLAG_ASHENVEIL_VISITED (0x4B3). Do not set a RESOLVED flag for this island.
- Coord-trigger scenes that should fire once and never again: gate on the progress
  var (DorneScene progress==3 -> advances to 4; MorthasApproach progress==5 ->
  advances to 6/7). MorthasApproach intentionally LEAVES progress at 5 if the player
  turns away (only the temp counter resets), so it re-arms for a retry.
