---
name: constant-space-budget
description: Remaining free space in Pelagios flag/var/item/trainer constant blocks and the hard ceilings on each (updated 2026-06-14 — Aetheron consumed BLOCK 4 0x297-0x29E; block 4 free 0x29F-0x2BB (29), items next 903, trainers 940 (84 free), var spares ONLY 0x410D-0x410F; MAPSEC ceiling RESOLVED — 508, MAPSEC_NONE now 273; boat-menu Convergence FLAG_AETHERON_RESOLVED gate added — see notes)
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
  **UPDATE 2026-06-13 (Schism): BLOCK 2 IS NOW FULL** — Schism took 0x49D-0x4A6 (10).
  Schism needed 16 story flags, so the other 6 went into "STORY BLOCK 3" = SCATTERED
  FLAG_UNUSED slots, because NO contiguous unused run remains below 0x500:
  FLAG_SCHISM_SEAL_NORTH_FOUND 0x468, _SEAL_SOUTH_FOUND 0x470, _SEAL_NORTH_DONE 0x472,
  _SEAL_SOUTH_DONE 0x479, FLAG_SCHISM_CEASEFIRE 0x4F9, FLAG_SCHISM_CIPHER_FOUND 0x4FA.
  Schism REUSED pre-existing FLAG_SCHISM_RESOLVED 0x4AF, FLAG_DRENN_ALIVE 0x4BB,
  FLAG_CIPHER_5_FOUND 0x4C1 — the per-island RESOLVED flags (0x4AC-0x4B4) and CIPHER_n
  flags (0x4BD-0x4C5) were ALL pre-allocated; ALWAYS grep before defining a new one.
  **Only 0x4FF (1 scattered slot) remains below 0x500** in the 0x4xx region. 0x468-0x492
  are vanilla collected-item flags; 0x4F0-0x4FE are vanilla gym/E4 flags.
  **UPDATE 2026-06-13: STORY BLOCK 4 RESERVED = 0x26C-0x2BB (80 contiguous FLAG_UNUSED,
  BELOW the hidden-item region).** Blocks 1-3 are FULL; block 4 is the open region for
  Thalvern/Gildhaven/Primalis/Ashenveil/Aetheron/Convergence. Bounded below by Pelagios
  hidden-item flags (end 0x26B), above by vanilla object-hide flags (0x2BC). Boundary
  placeholders in flags.h: FLAG_PELAGIOS_BLOCK4_RESERVED_START 0x26C / _END 0x2BB, with a
  full comment header; interior 0x26D-0x2BA stay FLAG_UNUSED until each island renames the
  next contiguous slots (do NOT pre-name per-island flags).
  **THALVERN CONSUMED 0x26C-0x275 (10 flags, 2026-06-13):** ARRIVED 0x26C, GYM1/2/3_CLEAR
  0x26D-0x26F, DEX_MET 0x270, LENS_DEFECTED 0x271, THRONE_CHOICE 0x272, SEAL_FOUND 0x273,
  CIPHER_FOUND 0x274, FLAG_NUMA_VESS_CONFRONTED 0x275. REUSED pre-allocated
  FLAG_THALVERN_RESOLVED 0x4B0, FLAG_DEX_ALIVE 0x4BA, FLAG_CIPHER_6_FOUND 0x4C2 (cipher 6).
  Thalvern had NO hidden items.
  **GILDHAVEN CONSUMED 0x276-0x281 (12 flags, 2026-06-13):** ARRIVED 0x276, GYM1/2/3/4_CLEAR
  0x277-0x27A, DAGAN_MET 0x27B, LACE_TALKED 0x27C, MANOR_ACCESS 0x27D, COVENANT_MAP_SEEN 0x27E,
  SEAL_FOUND 0x27F, CIPHER_FOUND 0x280, FLAG_CASS_GILDHAVEN_SEEN 0x281. REUSED pre-allocated
  FLAG_GILDHAVEN_RESOLVED 0x4B1, FLAG_CIPHER_7_FOUND 0x4C3 (cipher 7). Gildhaven had NO
  hidden items (its BlackMarket/SealChamber wilds are encounter tables, not bg_event hidden
  items).
  **PRIMALIS CONSUMED 0x282-0x28E (13 flags, 2026-06-14):** ARRIVED 0x282, GYM1/2/3/4_CLEAR
  0x283-0x286 (Fern/Scale/Thorn/Mako), TRUST_EARNED 0x287, ORAL_HISTORY_HEARD 0x288,
  LENS_MET 0x289, RUINS_FOUND 0x28A, SEAL_FOUND 0x28B, CIPHER_FOUND 0x28C,
  FLAG_BEAST_WHISTLE_OBTAINED 0x28D, FLAG_PRIMALIS_TOKEN_GIVEN 0x28E. REUSED pre-allocated
  FLAG_PRIMALIS_RESOLVED 0x4B2, FLAG_CIPHER_8_FOUND 0x4C4 (cipher 8). Primalis had NO hidden
  items.
  **ASHENVEIL CONSUMED 0x28F-0x296 (8 flags, 2026-06-14 — DEAD ISLAND, no gyms/no trainers):**
  ARRIVED 0x28F, OUTPOST_MET 0x290, FLAG_PHANTOM_LANTERN_OBTAINED 0x291,
  COVENANT_DOCS_FOUND 0x292, DORNE_MET 0x293, FLAG_SEA_CHART_FOUND 0x294 (activates Aetheron
  in boat menu, NOT boat tier), FLAG_MORTHAS_ENCOUNTERED 0x295, ASHENVEIL_CIPHER_FOUND 0x296.
  REUSED pre-existing (verified, NOT redefined): FLAG_DORNE_CHOICE_STOP/HELP/DEFER 0x4B5-0x4B7
  (these ALREADY existed from original setup), FLAG_ASHENVEIL_VISITED 0x4B3,
  FLAG_TRUE_ENDING_UNLOCKED 0x4B8, FLAG_CIPHER_9_FOUND 0x4C5 (cipher 9). NO hidden items.
  **AETHERON CONSUMED 0x297-0x29E (8 flags, 2026-06-14 — SKY ISLAND, reached via Knock Up
  Stream + ITEM_SEA_CHART, NOT boat tier):** ARRIVED 0x297, GYM1/2/3_CLEAR 0x298-0x29A
  (Gale/Arc/Voss), CASS_SEEN 0x29B, INSTALLATION_FOUND 0x29C, SEAL_FOUND 0x29D,
  FLAG_STORM_COMPASS_OBTAINED 0x29E. REUSED pre-existing (verified, NOT redefined):
  FLAG_AETHERON_RESOLVED 0x4B4 (per-island RESOLVED block), FLAG_CASS_DEFECTED 0x4BC
  (existed from original Cass-storyline setup). **NO new cipher — CIPHER_1-9 set is COMPLETE
  (Ashenveil took 9); brief explicitly says no cipher on Aetheron; do NOT invent CIPHER_10.**
  NO hidden items. **BLOCK 4 FREE: 0x29F-0x2BB (29 flags)** for Convergence/future.
  COLLISION-VERIFIED: every
  value 0x26C-0x2BB has exactly one FLAG_UNUSED_* def and zero external refs (grepped all
  names + raw hex across data/src/include/asm). Per-island RESOLVED (0x4B0-0x4B4) and
  CIPHER_6-9 (0x4C2-0x4C5) are PRE-ALLOCATED in 0x4xx — grep before redefining; real fresh
  need is ~10-13/island = ~50-60 total, so 80 is comfortable. HIDDEN-ITEM PRESSURE: block 4
  ate the last big hidden-item-capable run; only 0x264 (1 slot) free for hidden items now —
  future-island hidden items carve from the TOP of block 4 (0x2Bx) or cut a vanilla Hoenn
  hidden item. Compiles exit 0 (EWRAM 86.45% / IWRAM 86.63% / ROM 79.66%).
- **Hidden-item flags** are SEPARATE and MUST live in the hidden-items range
  (>= FLAG_HIDDEN_ITEMS_START = 0x1F4 for the Emerald/Hoenn map block). The
  bg_hidden_item_event macro in asm/macros/map.inc hard-errors (.error) if a flag is
  below FLAG_HIDDEN_ITEMS_START — NEVER put a hidden-item flag in the 0x4xx story block.
  Vanilla Emerald uses +0x00..0x6F. Pelagios claims +0x71 (FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE
  = 0x265), +0x72 (FLAG_HIDDEN_ITEM_IRONHOLD_IRON = 0x266), +0x73
  (FLAG_HIDDEN_ITEM_SIROCCO_BERRY = 0x267), +0x74/+0x75
  (FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1/2 = 0x268/0x269, 2026-06-12), +0x76/+0x77
  (FLAG_HIDDEN_ITEM_SCHISM_1/2 = 0x26A/0x26B, 2026-06-13). Next free: +0x78 (0x26C).
  (A duplicate FLAG_UNUSED_0x26A definition existed and was removed when claiming 0x26A.)
- **Vars** (include/constants/vars.h): EXPANDED. VARS_END raised 0x40FF -> 0x410F
  (+16 u16 vars, +32 bytes SaveBlock). New block 0x4100-0x410F: VAR_BOAT_TIER 0x4100
  (0=Dinghy/1=Sloop/2=Brigantine/3=Galleon), then one progress var per remaining island
  (SIROCCO 0x4101 ... CONVERGENCE 0x4109), then 6 reserved spares 0x410A-0x410F.
  Sirocco AND Emberveil added NO new vars (VAR_SIROCCO_PROGRESS 0x4101,
  VAR_EMBERVEIL_PROGRESS 0x4102, relationship vars all pre-existed). Schism added ONE:
  VAR_SCHISM_CEASEFIRE_PROGRESS 0x410A (was a reserved spare). VAR_SCHISM_PROGRESS 0x4103
  pre-existed. Thalvern added NO new vars at CONSTANTS time but its SCRIPTS later consumed
  0x410B (PATH A/B determinant) + 0x410C (3-day recovery counter) — script-owned, vars.h
  may still label them RESERVED but they are USED. Gildhaven (2026-06-13) added ZERO new
  vars: VAR_GILDHAVEN_PROGRESS 0x4105 + VAR_DAGAN_RELATIONSHIP 0x40FB both pre-existed; the
  corruption mechanic is purely progress-gated (0-2 normal / 3+ corrupted) — no spare eaten.
  **Genuinely-free spares: ONLY 0x410D, 0x410E, 0x410F (3) remain.** Beyond 0x410F needs
  another VARS_END bump (saveblock impact) — STOP and flag. TIGHT: if 2+ remaining islands
  each want a dedicated extra var, the 3 spares run out and a bump becomes necessary.
  AUDIT 2026-06-13: EVERY remaining island's PROGRESS var is ALREADY pre-allocated
  (Thalvern 0x4104 .. Convergence 0x4109) — those 5 islands + Convergence need ZERO new
  progress vars. The 5 spares cover only ~5 island-specific extras (relationship/ceasefire
  style). Sufficient now but foreseeably TIGHT by Aetheron/Convergence; if 2+ islands each
  want a dedicated extra var, a VARS_END bump (saveblock-impacting) becomes necessary —
  flag then, don't do it preemptively.
- **Items** (include/constants/items.h): Pelagios key items 874-887, Schism's 888-891
  (ITEM_SCAR_PASS_ICE/POISON, ITEM_SEAL_SHARD_GLACITH/TOXARA), then Thalvern's
  ITEM_COVENANT_ACCESS_CARD 892, ITEM_SEAL_SHARD_THALVERN 893 (Feraligatr Mega trigger,
  Mega wiring future), ITEM_DEX_NOTES 894 (2026-06-13), then Gildhaven's
  ITEM_VANE_MANOR_KEY 895 (icon BasementKey/OldKey — Lace post-Gym3 key),
  ITEM_SEAL_SHARD_GILDHAVEN 896 (icon RedOrb — Fairy/Dark seal-shard stub), then Primalis's
  ITEM_PRIMALIS_TOKEN 897 (icon ContestPass — Mako's carved token, Final Island) and
  ITEM_SEAL_SHARD_PRIMALIS 898 (icon RedOrb — Grass/Dragon seal-shard stub). ITEM_BEAST_WHISTLE
  879 (Cut/Sweet Scent replacement) PRE-EXISTED from the Haven batch — REUSED for Primalis, not
  re-added. ITEM_SONAR_LENS 878 (Dive) already existed. Then Ashenveil's ITEM_SEA_CHART 899
  (icon TownMap — found MorthasGrove; FLAG_SEA_CHART_FOUND activates Aetheron) and
  ITEM_SEAL_SHARD_ASHENVEIL 900 (icon RedOrb — Decidueye Mega trigger, stub). ITEM_PHANTOM_LANTERN
  881 (Defog/Flash replacement) PRE-EXISTED from the Haven batch — REUSED for Ashenveil, not
  re-added. Then Aetheron's ITEM_SEAL_SHARD_AETHERON 901 (icon RedOrb — Stormveil Electric/
  Flying Mega trigger stub) and ITEM_CASS_DOCUMENTS 902 (icon Powder/EnergyPowder — lore/
  examine key item). ITEM_STORM_COMPASS 880 (Fly replacement / island-select fast travel)
  PRE-EXISTED from the Haven batch — REUSED for Aetheron, not re-added (its data block already
  had a CannotUse field-effect stub; the island-select field effect is FUTURE systems work).
  **Next free item ID: 903.** Data goes in BOTH
  include/constants/items.h (enum) AND src/data/items.h (struct; mirror an existing
  Pelagios key item — importance 1, CannotUse, placeholder icon). Names charmap-safe.
  No hard ceiling nearby — adding items is cheap. ITEM_SEAL_SHARD_IRONHOLD is a #define
  alias to ITEM_SEAL_SHARD_1 (882), not a new ID.
- **Trainers**: EXPANDED to 1024 (was 864). After Thalvern (910-916, 7 trainers, 3 gyms),
  Gildhaven (917-924, 8 trainers — 4 generic guards 917-920 + FOUR gym leaders GLINT
  921 / SHADE 922 / LACE 923 / SEREL 924), then Primalis (925-932, 8 trainers — 4 Zoan
  guardians TRAINER_ZOAN_PRIMALIS_1-4 925-928 + FOUR gym leaders FERN 929 / SCALE 930 /
  THORN 931 / MAKO 932), then Aetheron (933-939, 7 trainers — 2 community guardians
  TRAINER_GUARDIAN_AETHERON_1/2 933-934 + 2 Covenant officers TRAINER_COVENANT_AETHERON_1/2
  935-936 + THREE gym leaders GALE 937 Flying / ARC 938 Electric / VOSS 939 Electric-Steel),
  TRAINERS_COUNT_EMERALD = **940, 84 free slots** (verified 2026-06-14
  in include/constants/opponents.h). Aetheron leader pics: Gale=Cooltrainer F, Arc/Voss=
  Cooltrainer M (Pic Leader); guardians/officers Cooltrainer F/M. Voss's 4th mon = Zekrom
  Lv.64 (brief's captured legendary; Zekrom exists so no Electivire substitute needed).
  Remaining islands ≈ 30-40 more → ample headroom. NO
  trainer MAX bump foreseeable. Primalis leader pics: Fern=Cooltrainer F, Scale/Thorn=Cooltrainer
  M, Mako=Hiker (no shark/elder pic exists; Zoan guardians = Cooltrainer F/M). Gildhaven leader
  pics: Glint/Shade=Cooltrainer M,
  Lace=Cooltrainer F, Serel=Rich Boy (Serel's Mawile Lv.55 is plain, Mega not wired).
  Thalvern leader pics: Tide=Fisherman, Psalm=Cooltrainer F,
  Lens=Cooltrainer M; generics all Scientist FRLG. See
  [[trainer-flag-space-full]] for the expansion mechanism and the save-break note.
  trainers.party uses Showdown format: Alolan Grimer = `Grimer-Alola` (hyphen ->
  underscore in the SPECIES_ macro). No vanilla "Scientist" Pic in Emerald — use
  `Scientist FRLG` (TRAINER_PIC_SCIENTIST_FRLG) for a scientist look.

**SAVE-BREAKING:** the 2026-06-11 refactor invalidated all saves (system flag IDs
shifted, SaveBlock1 grew 52 bytes total). Accepted in dev phase. Build still exit 0
at EWRAM 86.45% / ROM 79.00%.

**MAPSEC CEILING — RESOLVED 2026-06-14 (was the FIRST Pelagios constant to hit a real
ceiling).** Full detail in [[mapsec-u8-ceiling]]. Summary: `mapsec_u8_t`/`metloc_u8_t`
(include/gametypes.h) widened u8 -> **u16** so handler vars hold ids past 255; the stored
Pokemon metLocation became a **9-bit bitfield** repacked into the existing 12-byte
PokemonSubstruct3 (reclaimed `unused_0B`) so BoxPokemon does NOT grow (PC-box flash budget
can't absorb a wider mon). METLOC_* sentinels moved 0xFD/E/F -> **0x1FD/E/F** (top of 9 bits).
**New ceiling = 508 real MAPSECs** (sentinels 509-511). The MAPSEC enum is plain-sequential
(no explicit MAPSEC_NONE=255); MAPSEC_NONE = the count of preceding entries and FLOATS as
Pelagios MAPSECs are appended (Primalis pushed it to 262; Ashenveil's 5 pushed it to 267;
**Aetheron's 6 pushed it to 273** on 2026-06-14). 508 is the real hardware-relevant ceiling and
273 is far below it. Kanto kept —
removing it would break ~400 live FRLG map.json + region_map.c/regions.c. **MAPSEC additions
are now UNBLOCKED.** MAPSECs go in BOTH src/data/region_map/region_map_sections.json (source of
truth) AND region_map_entries.h (gRegionMapEntries placeholder 0,0,1,1 coords); the .h enum
(include/constants/region_map_sections.h) auto-regenerates from the .json via the Inja
template. SAVE-BREAKING (substruct layout changed). Caveat: map-header `region_map_section` is
still a u8 field — maps storing a MAPSEC >= 0x100 emit a harmless truncation warning at assemble
time (map-builder concern, separate from the resolved enum ceiling).

**BOAT MENU — sea-chart activation pattern (NEW, Ashenveil 2026-06-14).** The Tennyson
destination menu (data/scripts/pelagios_boat.inc + src/data/script_menu.h MultichoiceList_Boat*
+ include/constants/script_menu.h MULTI_BOAT_*) is one STATIC list per VAR_BOAT_TIER. Most
islands are tier-gated (appear in the higher-tier list). Aetheron is the exception: it is
ALWAYS in the Galleon list but its SailTo handler does
`goto_if_unset FLAG_SEA_CHART_FOUND, <no-chart msg>` and only sails once the Sea Chart is found
— a FLAG gate, not a tier gate. This is architecture (i): keep the static list, gate inside the
handler. Cleaner than a dynamic/conditional multichoice. To add an island: give it a
PELAGIOS_ISLAND_* id, append a list entry (Cancel stays last — its index shifts), add a
matching dispatch case (index must equal list position), and a SailTo handler (route to
SailNoChart until its maps exist).
**CONVERGENCE FLAG-GATE PATTERN (NEW, Aetheron 2026-06-14):** same architecture (i) — the
final island CONVERGENCE is ALWAYS in the Galleon list but its
Pelagios_EventScript_SailToConvergence handler does
`goto_if_unset FLAG_AETHERON_RESOLVED, Pelagios_EventScript_ConvergenceClosed` ("the way there
is not yet open") and only sails once Aetheron resolves. So the final-island unlock is a FLAG
gate (FLAG_AETHERON_RESOLVED), mirroring Aetheron's FLAG_SEA_CHART_FOUND gate. Galleon list now:
0=Haven, 1=Ironhold, 2=Sirocco, 3=Emberveil, 4=Schism-N, 5=Schism-S, 6=Thalvern, 7=Gildhaven,
8=Primalis, 9=Ashenveil, 10=Aetheron, **11=Convergence**, **12=Cancel**. Dispatch cases verified
in sync with list indices. (The Ashenveil pass added Aetheron but NOT Convergence — Convergence
id 11 + entry + case + handler were all added in the Aetheron pass.)

Naming/where things live: flags FLAG_ISLANDNAME_DESC, vars VAR_DESC, items
ITEM_DESC_NAME, trainers TRAINER_CLASS_ISLANDNAME_NUMBER. Map group registration for
a new island is SKIPPED by this agent — the map-builder registers the group with the
island's first real map.
