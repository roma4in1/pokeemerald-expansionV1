# Memory Index

- [Scripting gotchas](scripting-gotchas.md) — verified engine constraints (trigger vars, trainer-see, TM set, bag-full, hide flags) for every future island's scripts
- [Thalvern gotchas](thalvern-gotchas.md) — longest-vision pacing, in-place multi-day recovery counter, mutually-exclusive paths via spare var, removeobject-on-transition hides, bracket charmap miss
- [Gildhaven gotchas](gildhaven-gotchas.md) — wrong-direction hide-flag fix, two-scenes-one-flag guard, corruption via progress-compare (no new var), cutscene-vs-catchable callback, three-island order-independent gate, map_script-vs-.byte reloc
- [Primalis gotchas](primalis-gotchas.md) — apprentice-text stale-link (touch event_scripts.s), invalid SE constants, 5th-trainer->NPC via generator, appear-after-event objects, talk-NPC gym gating, key-item+TM full-bag safety, standalone-line pacing
- [Ashenveil gotchas](ashenveil-gotchas.md) — 3-way choice via 2 chained YESNO, hidden-cameo reveal, silent vision (fadeoutbgm/fadedefaultbgm), endurance loop on VAR_TEMP_1, cipher-9/true-ending ordering, sea-chart Aetheron gate, no FLAG_ASHENVEIL_RESOLVED
- [Aetheron gotchas](aetheron-gotchas.md) — Cass-defection exact timing (fadeoutbgm 4 / delay 60 silence / "Okay."(delay 20)"Let's go." / hard-set VAR_CASS_RELATIONSHIP=3), reveal-cameo-on-gymclear-flag via ON_TRANSITION, walk-alongside via force-spawn addobject (no shared-flag clear), Storm Compass field-item script vs C bag-use split, Convergence gate
- [Convergence gotchas](convergence-gotchas.md) — FINAL ISLAND endings: TRUE-first override gate, `special GameClear` credits (+ FLAG_CONVERGENCE_COMPLETE before it), spawn-gated roster -> fully-narrative cutscene, exact-verbatim final line ({PLAYER} buffer), legendary-cry roster per island
