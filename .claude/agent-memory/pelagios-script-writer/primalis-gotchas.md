---
name: primalis-gotchas
description: Primalis scripting gotchas - apprentice-text stale-link fix via touch, invalid SE constants, 5th-sight-trainer->NPC conversion via generator, appear-after-event objects via ON_TRANSITION, talk-NPC gym ordering
metadata:
  type: project
---

Techniques established building Primalis's scripts (2026-06-14). See
[[scripting-gotchas]], [[thalvern-gotchas]], [[gildhaven-gotchas]] for prior facts.
Ashenveil is next (heaviest narrative island).

- STALE-LINK FALSE ERROR: editing pelagios_speaker_names.inc (or any .inc included by
  data/event_scripts.s) can leave an incremental build linking the OLD event_scripts.o,
  producing a flood of `undefined reference to gText_ApprenticeChallenge0..N` (and similar
  battle-frontier text symbols). These texts live in data/text/*.inc included by
  event_scripts.s. FIX: `touch data/event_scripts.s` then rebuild - it is NOT a real error
  in your scripts. I now `touch data/event_scripts.s` before every compile after the first.
- INVALID SE CONSTANTS bite repeatedly - songs.h has FEWER move-SE names than you'd guess.
  Verified on Primalis: SE_M_GROWL, SE_M_GROWTH do NOT exist. SE_M_GRASSWHISTLE (jungle
  ambient / whistle), SE_M_SUPERSONIC (reinforce flash), SE_SUDOWOODO_SHAKE (root/tree
  stirring), SE_PIN, SE_M_SUPERSONIC all DO. ALWAYS `grep -c SE_X include/constants/songs.h`
  before using a move-SE you haven't used before.
- 5TH SIGHT-TRAINER WITH ONLY 4 IDS: the map placed 5 sight-trainer objects (ZOAN1-5) but
  only TRAINER_ZOAN_PRIMALIS_1..4 exist. Cleanest fix = convert ONE object to a plain talk
  NPC in the GENERATOR (build_primalis_mapjson.py: trainer_type TRAINER_TYPE_NONE, sight 0),
  regenerate that island's map.json (python3 the mapjson generator - it does NOT touch
  scripts.inc), then write the object as a lore NPC (lock/faceplayer, no trainerbattle).
  The remaining 4 sight trainers map 1:1 to the 4 ids. Trainer-NAME order follows the
  trainer ID (trainers.party Name:), NOT the brief's map placement - honor the actual
  object placement + ID order (ZOAN1=SERA, 2=VEX, 3=RAEL, 4=CAEL on Primalis).
- "APPEARS AFTER EVENT X" objects whose map.json hide flag IS X: same wrong-direction trap
  as [[gildhaven-gotchas]]. Primalis had THREE (camp Lens hide=LENS_MET; Heartwood Mako +
  Lens hide=RUINS_FOUND). Drive each in MAP_SCRIPT_ON_TRANSITION: goto_if_unset FLAG_X ->
  removeobject(s); else addobject(s). Multiple objects can share one transition handler.
- TALK-INITIATED GYM gating pattern (no trainer-see, so it can be flag-gated): put
  goto_if_defeated FIRST, then goto_if_unset FLAG_PREV_GYM -> Locked (BEFORE any lock), then
  setspeaker + trainerbattle_single. The Locked branch does its own lock/faceplayer. Do NOT
  lock-then-trainerbattle (trainerbattle self-locks) and do NOT release-then-trainerbattle.
- KEY-ITEM-PLUS-TM scene full-bag safety: give the two key items via
  checkitemspace + call_if_eq VAR_RESULT, TRUE, GiveSub (each GiveSub does additem + setflag
  + fanfare + return), set the story flags AFTER, then the TM LAST via giveitem +
  goto_if_eq VAR_RESULT, FALSE, <local TMFull that just release/end> - NOT
  Common_EventScript_ShowBagIsFull (that ends the script and would skip the parting line if
  placed mid-scene; only safe as the LAST statement of a gym victory). Mako's oral-history
  handoff uses this (Beast Whistle + Primal Token + Dragon Claw TM).
- Primalis TM substitutions (no Leaf Storm/Dragon Dance/Power Whip/Outrage TM in the 50-set):
  Fern Leaf Storm -> TM Giga Drain; Scale Dragon Dance -> TM Bulk Up (setup); Thorn Power
  Whip -> TM Bullet Seed; Mako Outrage -> TM Dragon Claw. Lampshade each. Extends the
  Thalvern/Gildhaven TM-mapping notes.
- A SINGLE LINE THAT MUST STAND ALONE ("The Root remembers us.", and the recovery
  "I'm still in here."): wrap with delay before (delay 40-50 after the prior closemessage)
  and after (closemessage + delay), and put NOTHING in the same msgbox. The recovery scene
  also gets a follow-up narration msgbox AFTER its own post-delay (hand-finding-yours) - the
  pause is between the line and the narration, both deliberate.
- Primalis is NOT an Ashenveil gate (Ashenveil = Schism+Thalvern+Gildhaven, already wired).
  So Primalis resolution has NO three-island check and NO boat-tier change (Galleon is the
  cap). Just FLAG_PRIMALIS_RESOLVED + progress 7 + the Sollis call. Don't bolt an Ashenveil
  check onto a non-gating island.
- Verdath (Grass/Dragon, "oldest sealed legendary") cry placeholder = SPECIES_RAYQUAZA
  (ancient Dragon legendary). SPECIES_ZARUDE also exists if a Grass flavor is wanted later.
