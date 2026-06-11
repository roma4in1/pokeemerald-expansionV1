---
name: constant-space-budget
description: Remaining free space in Pelagios flag/var/item/trainer constant blocks and the hard ceilings on each (updated after 2026-06-11 capacity refactor)
metadata:
  type: project
---

Pelagios constant-space budget as of 2026-06-11 (AFTER the capacity refactor that
expanded trainers and vars). Always re-verify by grepping the live headers.

- **Flags** (include/constants/flags.h): Pelagios story block 0x4A7 onward; Ironhold
  flags end 0x4E3 (FLAG_HIDE_IRONHOLD_SEVER_GATE = 0x4E3). Free: 0x4E4-0x4EF (12 flags)
  before vanilla resumes at 0x4F0. Plenty of headroom. SYSTEM_FLAGS base shifted
  0x860 -> 0x900 (floats off MAX_TRAINERS_COUNT — never hardcode). FLAGS_COUNT ~0xA00,
  far below 0x4000.
- **Hidden-item flags** are SEPARATE and MUST live in the hidden-items range
  (>= FLAG_HIDDEN_ITEMS_START = 0x1F4 for the Emerald/Hoenn map block). The
  bg_hidden_item_event macro in asm/macros/map.inc hard-errors (.error) if a flag is
  below FLAG_HIDDEN_ITEMS_START — NEVER put a hidden-item flag in the 0x4xx story block.
  Vanilla Emerald uses +0x00..0x6F. Pelagios claims +0x71 (FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE
  = 0x265) and +0x72 (FLAG_HIDDEN_ITEM_IRONHOLD_IRON = 0x266). Next free: +0x73 (0x267).
- **Vars** (include/constants/vars.h): EXPANDED. VARS_END raised 0x40FF -> 0x410F
  (+16 u16 vars, +32 bytes SaveBlock). New block 0x4100-0x410F: VAR_BOAT_TIER 0x4100
  (0=Dinghy/1=Sloop/2=Brigantine/3=Galleon), then one progress var per remaining island
  (SIROCCO 0x4101 ... CONVERGENCE 0x4109), then 6 reserved spares 0x410A-0x410F.
  Beyond 0x410F needs another VARS_END bump — STOP and flag.
- **Items** (include/constants/items.h): Pelagios key items 874-883, plus
  ITEM_DOCUMENT_FRAGMENT 884. No hard ceiling nearby — adding items is cheap.
  ITEM_SEAL_SHARD_IRONHOLD is a #define alias to ITEM_SEAL_SHARD_1 (882), not a new ID.
- **Trainers**: EXPANDED to 1024 (was 864). 156 free slots. See
  [[trainer-flag-space-full]] for the expansion mechanism and the save-break note.

**SAVE-BREAKING:** the 2026-06-11 refactor invalidated all saves (system flag IDs
shifted, SaveBlock1 grew 52 bytes total). Accepted in dev phase. Build still exit 0
at EWRAM 86.45% / ROM 79.00%.

Naming/where things live: flags FLAG_ISLANDNAME_DESC, vars VAR_DESC, items
ITEM_DESC_NAME, trainers TRAINER_CLASS_ISLANDNAME_NUMBER. Map group registration for
a new island is SKIPPED by this agent — the map-builder registers the group with the
island's first real map.
