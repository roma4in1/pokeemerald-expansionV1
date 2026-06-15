---
name: thalvern-gotchas
description: Scripting techniques established on Thalvern - longest-vision pacing, multi-day recovery counter, mutually-exclusive path branching via a spare var, parent-flash callback, pokemart berries
metadata:
  type: project
---

Techniques established building Thalvern's scripts (2026-06-13). Engine-level
facts that generalize to future emotional/branching islands (Ashenveil next).
See [[scripting-gotchas]] for the base engine constraints.

- MUTUALLY-EXCLUSIVE STORY PATHS without a dedicated flag: record the choice in a
  spare general var (Thalvern used VAR_PELAGIOS_RESERVED_0x410B: 1=PATH A, 2=PATH B)
  set at the choice scene, then have the downstream scene `goto_if_eq` on that ONE
  var. Exactly one branch can run. Do NOT reuse the "outcome" flag (FLAG_DEX_ALIVE)
  as the path determinant when the outcome must be set LATER - here FLAG_DEX_ALIVE
  is set only inside PATH A's recovery, never at the choice, so the path needed its
  own carrier. The reserved Pelagios spare vars (0x410B-0x410F) are perfect for this
  (no vars.h edit; they already exist).
- MULTI-DAY RECOVERY / montage: cannot continue a script after a `warp`+`waitstate`
  (warp ends the current script context; the destination map does not resume it).
  So a "recover at the OTHER map" montage that must flow back into the CURRENT map's
  next beat is rendered IN-PLACE over a held black/white screen (fadescreen
  FADE_TO_BLACK, never fade back until done). Day counter = a spare var
  (VAR_PELAGIOS_RESERVED_0x410C), advanced 1->2->3, each value a `call` to a sub-
  script that `goto_if_eq`s to that day's beat then `return`s. Player never sees the
  real map during the montage, so no actual relocation/bed object is needed (DexCamp
  had neither, and map.json is generator-owned). This deviates from a literal
  "warp to a bed at DexCamp" but is robust and fires exactly once.
- LONGEST-VISION PACING: the throne vision is 5 white-flash beats
  (playse SE_M_SUPERSONIC; fadescreen FADE_TO_WHITE; delay 50-60; fadescreen
  FADE_FROM_WHITE; msgbox). Escalate the delay slightly each beat (50->55->60) so it
  feels heavier as it goes. A `playmoncry SPECIES_KYOGRE` (Pelagios placeholder,
  Water legendary) opens it. The PARENT FLASH is the final beat before collapse -
  the emotional payload; its journal cipher text (cipher 6) calls back to it
  ("I saw my successor in the vision") and is shared verbatim by BOTH paths.
- PER-MAP "permanently gone" NPC with NO hide flag in map.json: use
  MAP_SCRIPT_ON_TRANSITION + `removeobject` gated on the story flag. Thalvern did
  this for FOUR objects (DexCamp Dex when resolved-and-dead; Exterior Lens after
  Gym3; Interior2 Lens after defection; CovenantSite Numa after confrontation).
  removeobject in ON_TRANSITION runs before the map draws = clean permanent hide.
- pokemart list of BERRIES works fine: `.align 2`, list of `.2byte ITEM_*`,
  terminated by `pokemartlistend` (NOT ITEM_NONE + release/end). Berries get a
  sensible auto price. Persim/Lum are the "psychic-resist" flavor items.
- TM substitutions on a Water/Psychic island: there is NO Surf TM (Surf is HM03)
  and NO Scald TM in the vanilla 50-TM set. The only offensive Water TM is
  ITEM_TM_WATER_PULSE. Thalvern: Tide=Water Pulse (for "Surf equiv"), Psalm=Psychic
  (exact), Lens=Rain Dance (Water support, for "Scald") - lampshaded in each gym's
  TM dialogue. Reuse this mapping for any Water gym.
- CHARMAP: square brackets `[` `]` are NOT in charmap.txt (U+5B error). Use
  parentheses for stage directions, e.g. "(The child sketches...)". This bit me
  once - the em-dash/curly-quote rules are well known but brackets are an easy miss.
