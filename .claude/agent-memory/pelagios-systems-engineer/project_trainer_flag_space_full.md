---
name: trainer-flag-space-full
description: Trainer flag space ceiling — was full at 864, EXPANDED to 1024 on 2026-06-11; how the expansion works and where the new ceiling is
metadata:
  type: project
---

RESOLVED 2026-06-11 via capacity refactor. Previously `TRAINERS_COUNT_EMERALD ==
MAX_TRAINERS_COUNT_EMERALD == 864` left zero trainer slots and blocked the 4 Ironhold
gym leaders. Now `MAX_TRAINERS_COUNT_EMERALD == 1024` (include/constants/opponents.h),
`TRAINERS_COUNT_EMERALD == 868`. **156 free trainer slots remain.**

**How the expansion works (reuse this pattern for the next bump):** Trainer defeat
flags live in a fixed block starting at TRAINER_FLAGS_START 0x500 (include/constants/
flags.h). `SYSTEM_FLAGS` is defined as `(TRAINER_FLAGS_END + 1)` where TRAINER_FLAGS_END
derives from MAX_TRAINERS_COUNT. Because every FLAG_SYS_*/badge/landmark/daily flag is
SYSTEM_FLAGS-relative (not hardcoded), raising MAX_TRAINERS_COUNT shifts the whole block
up automatically — NO per-flag edits, NO collisions. The only constraints: stay below
SPECIAL_FLAGS_START (0x4000) — lots of room (FLAGS_COUNT is ~0xA00) — and accept that
SaveBlock1 grows (~1 byte per 8 trainers) and every system flag ID changes (save-breaking).

After expansion: trainer flags 0x500-0x8FF, SYSTEM_FLAGS base 0x900. Ironhold gym
leaders are TRAINER_LEADER_IRONHOLD_PETRA 864 / FORGE 865 / ROOK 866 / SEVER 867,
with full parties in src/data/trainers.party (from IRONHOLD_BRIEF.md).

**How to apply:** Adding future-island trainers is now cheap — just append IDs up to
1023 and entries in trainers.party. Only STOP and flag the user if a request would
exceed 1024 (then another MAX bump is needed, same save-breaking pattern). Always
re-verify the live count (grep MAX_TRAINERS_COUNT_EMERALD) before relying on this.
See [[constant-space-budget]].
