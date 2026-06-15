---
name: gildhaven-gotchas
description: Gildhaven scripting techniques - wrong-direction map.json hide flag fix, two-scenes-one-flag guard, corruption-via-progress-compare (no new var), cutscene-vs-catchable callback, three-island order-independent gate
metadata:
  type: project
---

Techniques established building Gildhaven's scripts (2026-06-14). Engine-level
facts that generalize to future islands (Ashenveil next). See [[scripting-gotchas]]
and [[thalvern-gotchas]] for prior constraints.

- WRONG-DIRECTION map.json hide flag (object should APPEAR after event X, but its
  map.json `flag` is X itself): the engine HIDES an object while its hide flag is SET,
  so `flag = FLAG_X` makes the object vanish AFTER X, not appear. You cannot change which
  flag the engine checks without editing map.json (generator-owned). FIX without touching
  map.json: drive visibility explicitly in MAP_SCRIPT_ON_TRANSITION -
  `goto_if_unset FLAG_X -> removeobject` (hide pre-X); else `addobject` (force-show post-X,
  overriding the spawn-flag hide). Gildhaven's BlackMarket Dagan used exactly this
  (hide flag FLAG_GILDHAVEN_GYM1_CLEAR, wanted-to-appear-after-Gym1).
- TWO ONE-TIME SCENES SHARING ONE STORY FLAG (Gildhaven had two Cass cameos both with
  map.json hide flag FLAG_CASS_GILDHAVEN_SEEN): let only the LATER scene set the shared
  flag. The EARLIER scene must be made once-only by other means and must NOT set it - else
  it pre-hides the later scene's object. Gildhaven: scene 1 (harbor) is part of the
  progress-0->1 arrival trigger (naturally once) + an ON_TRANSITION removeobject keyed on
  `progress != 0` so its object never respawns; it never touches the shared flag. Scene 2
  (noble quarter) is `goto_if_set FLAG... -> done`, sets the flag at the end, and its
  object's map.json hide flag IS that flag -> gone forever after. Clean, no extra flag.
- CORRUPTION / ENVIRONMENTAL DIALOGUE SHIFT needs NO new var when it is progress-gated:
  every affected NPC does `compare VAR_<ISLAND>_PROGRESS, N` + `goto_if_ge ...Corrupt`,
  with a FLAG_<ISLAND>_RESOLVED post-state branch checked FIRST. Gildhaven shifts at
  progress>=3 (Gym 2). Three text variants per NPC (normal / corrupt / resolved), one var
  read. Reuse for any "the island changes you the deeper you go" island.
- SCRIPTED-ENCOUNTER CALLBACK, cutscene vs catchable: if the map.json has NO overworld
  object for the creature and the brief frames it as a fleeting appearance, render it as a
  PURE cutscene (playse SE_PIN + playmoncry SPECIES_X + narration), folded into an already
  once-gated scene (Gildhaven's Ralts went inside the progress-5 SealChamber discovery).
  Do NOT spawn a wildbattle/trainerbattle for a "it appears and flees" beat. (A separate
  20%-Ralts wild-table slot can coexist - it's unrelated.)
- THREE-ISLAND ORDER-INDEPENDENT GATE (Ashenveil = Schism AND Thalvern AND Gildhaven all
  resolved): each island's resolution checks the OTHER TWO RESOLVED flags
  (`goto_if_unset FLAG_A_RESOLVED -> NotYet` x2 -> else opens). Whichever resolves LAST
  fires the opens branch. VERIFY all three siblings use the SAME check before trusting it
  (Schism's dual ports BOTH funnel through one CheckDualSeals->Resolution->AshenveilCheck;
  Thalvern's ThroneRoom + Gildhaven's SealChamber each mirror it). The "all done" branch can
  be a real handler even when the destination island isn't built (narration only, NO warp /
  NO boat-tier) so only one branch needs wiring later. Generalizes the two-sided Galleon
  check in [[scripting-gotchas]] to N islands.
- MAP_SCRIPT table: ON_TRANSITION/ON_FRAME entries MUST use the `map_script` macro, NOT a
  raw `.byte MAP_SCRIPT_ON_TRANSITION, Label`. Raw .byte emits an 8-bit reloc against the
  script pointer -> "relocation truncated to fit: R_ARM_ABS8". (The map-builder's stub
  emits `.byte 0` for no-script maps, which is fine; adding a transition means switching to
  `map_script`.) Bit me once on GoldportHarbor.
- Gildhaven TM substitutions (no Fairy TM and only THIEF/TORMENT/SNATCH/TAUNT/SHADOW_BALL
  for Dark/dark-flavor in the vanilla 50): Glint Dazzling Gleam -> TM Light Screen;
  Shade Crunch -> TM Thief; Lace Play Rough -> TM Torment; Serel Moonblast -> TM Shadow Ball.
  Lampshade each. (Extends the Thalvern Water-TM mapping note.)
