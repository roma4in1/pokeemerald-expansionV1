---
name: primalis-script-build-blocker
description: Untracked Primalis_VerdantLanding/scripts.inc has an invalid SE_M_GROWL symbol that breaks the whole-tree link once event_scripts.s rebuilds
metadata:
  type: project
---

`data/maps/Primalis_VerdantLanding/scripts.inc` (line ~13, `playse SE_M_GROWL`) references
a sound symbol that does NOT exist in include/constants/songs.h. This breaks the entire
build at link time: `undefined reference to SE_M_GROWL`.

**Why:** discovered 2026-06-14 when enabling Following Pokémon (a header change in
include/config/overworld.h forced `data/event_scripts.s` to rebuild, which `.include`s all
scripts.inc and surfaced the latent symbol). The file is UNTRACKED (`??`) — it came from the
inline Primalis MAPS build and had never been compiled into a fresh event_scripts.o, so older
"clean" builds only passed because event_scripts.o was stale.

**How to apply:** if a build suddenly fails on `SE_M_GROWL` (or another Primalis script symbol)
after an unrelated header/config change, it is THIS pre-existing script bug, not your change.
Confirm by reverting your change + `touch data/event_scripts.s` + rebuild — the same error
recurs. The fix is a one-line SCRIPT edit (SE_M_GROWL -> a real cry SE like SE_M_ROAR /
SE_M_UPROAR) owned by the script-writer, NOT the systems-engineer (no .inc edits in
systems-engineer tasks). Until fixed, the tree cannot reach `gmake exit 0`. Related: the
follower config change itself ([[constant-space-budget]] area is untouched) links cleanly.
