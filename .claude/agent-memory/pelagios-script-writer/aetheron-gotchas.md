---
name: aetheron-gotchas
description: Verified patterns for Aetheron's Cass-defection exact-timing scene, hard-set relationship var, force-spawn-via-addobject for walk-alongside NPCs, and the Storm Compass field-item script-vs-C split
metadata:
  type: project
---

Patterns/constraints verified building all 11 Aetheron scripts (2026-06-14,
gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.14%). Aetheron is the SKY
ISLAND and holds the CASS DEFECTION (the rival-arc turn). Reuse for Convergence.

**Why:** the defection is the game's biggest emotional beat with exact engine-level
timing/silence requirements; the field-item split recurs for any bag-usable key item.

**How to apply:** read before writing Convergence or any future timed cutscene /
walk-alongside companion / bag-use key item.

- EXACT-TIMING CUTSCENE (defection): stop music with `fadeoutbgm 4` (true silence),
  pace beats with `delay` between msgboxes, resume with `fadedefaultbgm` at the exact
  scripted point. The "silence between the order and the first reply" beat was
  `delay 60` (NON-NEGOTIABLE per brief - do not shorten). A two-part final line
  ("Okay." then "Let's go.") = two msgboxes with `delay 20` between them. closemessage
  after every line so the box clears before the delay reads as silence.
- HARD-SET a relationship var: `setvar VAR_CASS_RELATIONSHIP, 3` (NOT addvar) when the
  brief says "set to max regardless of previous value". Distinct from the usual
  per-choice `addvar +1`.
- CHAIN a triggered scene off a gym victory with NO free-roam between: put the scene
  as a `goto` at the tail of the leader's *Victory script (player is still locked from
  the trainerbattle). Do NOT use a separate coord trigger if the brief wants Voss's
  last line to flow straight into the doorway reveal.
- REVEAL a cameo whose hide flag is the gym-clear flag: that flag is CLEAR before the
  fight, so the engine would draw the object EARLY. Fix with an ON_TRANSITION that
  `removeobject`s it while the flag is unset, then in the scene `clearflag <flag>` +
  `addobject` to show it, and `setflag <flag>` (+ `removeobject`) at the end so it
  stays hidden on reload. (Object hidden when its flag is SET - confirmed in
  TrySpawnObjectEventTemplate; `addobject` force-spawns regardless of the flag.)
- WALK-ALONGSIDE companion (Cass on StormPeak/SealChamber after defection): the
  companion objects' hide flag was a placeholder shared flag (FLAG_AETHERON_CASS_SEEN)
  that is SET permanently, so they never auto-spawn. Force-spawn them every load with
  an ON_TRANSITION `goto_if_set FLAG_CASS_DEFECTED -> addobject LOCALID`. Do NOT
  `clearflag` the shared hide flag to show them - that would un-hide the SAME-flag
  cameo on OTHER maps (cross-map bug). addobject alone shows them without touching the
  flag; on reload the ON_TRANSITION re-adds them.
- REMOVE a defeated boss permanently when his object has no hide flag (flag 0): set the
  gym-clear flag and add an ON_LOAD `goto_if_set <gymclear> -> removeobject LOCALID`.
- FIELD-ITEM (bag "Use") that opens a menu anywhere: write the SCRIPT in
  data/scripts/pelagios_boat.inc (Pelagios_EventScript_StormCompass) - it `lockall`s,
  sets VAR_TEMP_1 = 255 (a sentinel no PELAGIOS_ISLAND_* uses, so the sail handlers'
  same-island guard never matches), shows MULTI_BOAT_GALLEON, and reuses every
  Pelagios_EventScript_SailTo* handler (one source of truth for destinations/warps).
  The bag-use REGISTRATION is C (forbidden for script-writer): ITEM_STORM_COMPASS has
  .fieldUseFunc = ItemUseOutOfBattle_CannotUse in src/data/items.h. Systems-engineer
  must mirror ItemUseOutOfBattle_PokemonBoxLink in src/item_use.c (a field-CB Task that
  calls ScriptContext_SetupScript(Pelagios_EventScript_StormCompass)) and point the
  item's fieldUseFunc at it. The script compiles + is correct standalone; only that
  one C wire remains. Deviation: the shared SailTo* handlers play the boat "cast off"
  text even when reached from the compass - acceptable, flag if it matters.
- VAR_AETHERON_PROGRESS reconciliation: brief says "4=Gym3+defection, 5=seal
  reinforced". The map-builder armed the SealChamber discovery trigger at progress==5
  and StormPeak arrival at progress==4. Implemented as: defection sets 4; StormPeak
  arrival narration sets 5 (arms chamber discovery); chamber discovery sets 6 (arms
  apparatus); apparatus/compass resolution keeps 6 + sets FLAG_AETHERON_RESOLVED.
  Document the as-built mapping; the vars.h comment may differ.
- Pelagios_Speaker_Voss + Pelagios_Speaker_Cass ALREADY existed - do NOT re-add (dup =
  assembler error). Added only Gale, Arc, Dockhand, Technician this island.
- Convergence gate: SailToConvergence already `goto_if_unset FLAG_AETHERON_RESOLVED` -
  do NOT edit the boat menu; the SealChamber resolution sets FLAG_AETHERON_RESOLVED and
  the in-world Sollis call is the signal that Convergence is open. Sollis line uses
  `--` (two hyphens), NOT an em dash.
