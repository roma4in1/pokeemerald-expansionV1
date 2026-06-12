---
name: constant-space-budget
description: Remaining free space in Pelagios flag/var/item/trainer constant blocks and the hard ceilings on each (updated after 2026-06-11 capacity refactor)
metadata:
  type: project
---

Pelagios constant-space budget as of 2026-06-11 (AFTER the capacity refactor that
expanded trainers and vars). Always re-verify by grepping the live headers.

- **Flags** (include/constants/flags.h): Pelagios STORY BLOCK 1 (0x4A7-0x4EF) is now
  **FULL** — FLAG_EMBERVEIL_ARRIVED 0x4EF took the last slot (2026-06-12). Sirocco used
  0x4E4-0x4EE. Overflow goes to **STORY BLOCK 2 = 0x493-0x4A6** (20 contiguous vanilla-
  unused flags immediately BEFORE block 1 — NOT past 0x4F0; the 0x4F0-0x4FF region is
  vanilla gym/E4 flags with only scattered gaps, unusable as a contiguous run). Emberveil
  claimed block 2 0x493-0x49C (10 flags). **Block 2 free: 0x49D-0x4A6 (10 flags)** for
  Schism+. FLAG_EMBERVEIL_RESOLVED 0x4AE / FLAG_SOLACE_ALT_ENDING 0x4B9 already existed
  (reuse, don't dup). Never collide with trainer flags (0x500-0x8FF) or SYSTEM_FLAGS
  (0x900+, floats off MAX_TRAINERS_COUNT — never hardcode). FLAGS_COUNT ~0xA00, far below
  0x4000. After block 2 fills, find another contiguous FLAG_UNUSED run.
- **Hidden-item flags** are SEPARATE and MUST live in the hidden-items range
  (>= FLAG_HIDDEN_ITEMS_START = 0x1F4 for the Emerald/Hoenn map block). The
  bg_hidden_item_event macro in asm/macros/map.inc hard-errors (.error) if a flag is
  below FLAG_HIDDEN_ITEMS_START — NEVER put a hidden-item flag in the 0x4xx story block.
  Vanilla Emerald uses +0x00..0x6F. Pelagios claims +0x71 (FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE
  = 0x265), +0x72 (FLAG_HIDDEN_ITEM_IRONHOLD_IRON = 0x266), +0x73
  (FLAG_HIDDEN_ITEM_SIROCCO_BERRY = 0x267), +0x74/+0x75
  (FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1/2 = 0x268/0x269, 2026-06-12). Next free: +0x76 (0x26A).
- **Vars** (include/constants/vars.h): EXPANDED. VARS_END raised 0x40FF -> 0x410F
  (+16 u16 vars, +32 bytes SaveBlock). New block 0x4100-0x410F: VAR_BOAT_TIER 0x4100
  (0=Dinghy/1=Sloop/2=Brigantine/3=Galleon), then one progress var per remaining island
  (SIROCCO 0x4101 ... CONVERGENCE 0x4109), then 6 reserved spares 0x410A-0x410F.
  Sirocco AND Emberveil added NO new vars (VAR_SIROCCO_PROGRESS 0x4101,
  VAR_EMBERVEIL_PROGRESS 0x4102, relationship vars all pre-existed). 6 reserved spares
  0x410A-0x410F remain. Beyond 0x410F needs another VARS_END bump — STOP and flag.
- **Items** (include/constants/items.h): Pelagios key items 874-885, plus
  ITEM_WARDEN_NOTES 886 and ITEM_SEAL_SHARD_EMBERVEIL 887 (2026-06-12, Emberveil).
  ITEM_LAVA_BOOTS 877 already existed (Haven key-item batch). **Next free item ID: 888.**
  No hard ceiling nearby — adding items is cheap. ITEM_SEAL_SHARD_IRONHOLD is a #define
  alias to ITEM_SEAL_SHARD_1 (882), not a new ID.
- **Trainers**: EXPANDED to 1024 (was 864). After Emberveil (888-899, 12 trainers:
  cult 1-8 = 888-895, gym leaders Cinder 896 / Slag 897 / Vex 898 / Solace 899),
  TRAINERS_COUNT_EMERALD = 900, **124 free slots**. See
  [[trainer-flag-space-full]] for the expansion mechanism and the save-break note.

**SAVE-BREAKING:** the 2026-06-11 refactor invalidated all saves (system flag IDs
shifted, SaveBlock1 grew 52 bytes total). Accepted in dev phase. Build still exit 0
at EWRAM 86.45% / ROM 79.00%.

Naming/where things live: flags FLAG_ISLANDNAME_DESC, vars VAR_DESC, items
ITEM_DESC_NAME, trainers TRAINER_CLASS_ISLANDNAME_NUMBER. Map group registration for
a new island is SKIPPED by this agent — the map-builder registers the group with the
island's first real map.
