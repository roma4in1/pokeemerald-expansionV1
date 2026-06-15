# Pokémon Pelagios — Claude Code Project Context

## Project Overview
GBA ROM hack built on **pokeemerald-expansion** (Emerald decomp base).
Full design specification: `pelagios_claude_code_brief.md` (Haven Isle) + `IRONHOLD_BRIEF.md`
Build command: `gmake -j$(sysctl -n hw.ncpu)`
Output file: `pokeemerald.gba`
Test in: mGBA emulator

## Critical Rules
- ALWAYS compile after each major change: `gmake -j$(sysctl -n hw.ncpu)`
- ALWAYS fix all compilation errors before moving to the next task
- NEVER reference tilesets, flags, variables, or items that don't exist yet
- ALWAYS match the exact file format of existing maps (reference PetalburgCity)
- ALWAYS register new maps in map_groups.json AND relevant header files
- ALWAYS add new flags to include/constants/flags.h
- ALWAYS add new variables to include/constants/vars.h
- ALWAYS add new items to include/constants/items.h and src/data/items.h

## Runtime Budget (update after every island build)
| Region | Used | Cap | Last measured |
|---|---|---|---|
| EWRAM | **86.46%** (226,640 B / 256 KB) | hard 100% | 2026-06-14, after Aetheron SCRIPTS (+0 B — scripts are ROM) |
| IWRAM | **86.63%** (28,388 B / 32 KB) | hard 100% | 2026-06-14, after Aetheron SCRIPTS (+0 B) |
| ROM | **80.21%** (26,914,296 B / 32 MB) | hard 100% (GBA cap, see ROM Budget) | 2026-06-14, after Convergence SCRIPTS (final 2 maps; +~16 KB scripts/text; RAM unchanged) |

HARD RULE: flag any new runtime system (boat transitions, quest tracker UI,
difficulty modes, etc.) that risks pushing EWRAM or IWRAM over 95% BEFORE
implementing it. RAM, not ROM, is this project's tightest constraint.
Measure by relinking: `rm -f pokeemerald.elf && gmake ...` and read the
"Memory region" table ld prints.

## Project Architecture

### Region: Pelagios
An archipelago navigated by boat. Semi-linear island progression
gated by boat upgrades (Dinghy → Sloop → Brigantine → Galleon).

### Island Progression
```
Haven Isle (home, fixed start)
    ↓ Sloop
Ironhold (Occupied Island — Steel/Fighting)
    ↓ Brigantine
Sirocco Isle (Sand — Ground/Rock) ←→ Emberveil (Volcanic — Fire/Ground)
    ↓ Galleon
Schism Isle (Split — Ice/Poison) ←→ Thalvern (Sunken — Water/Psychic) ←→ Gildhaven (Merchant — Fairy/Dark)
    ↓
Ashenveil (Dead Island — Ghost/Dark)
    ↓ Sea chart → Knock Up Stream
Aetheron (Sky Island — Electric/Flying)
    ↓
Convergence (Final Island)
```

### HM Replacement System
HMs are fully replaced by Key Items. Do NOT use HM moves for field traversal.
| Key Item | Replaces | Obtained |
|---|---|---|
| ITEM_NAVIGATORS_LOG | Surf | Haven Isle |
| ITEM_GRAPPLE_HOOK | Strength/Rock Smash | Ironhold |
| ITEM_LAVA_BOOTS | Rock Climb/Flash | Emberveil |
| ITEM_SONAR_LENS | Dive | Thalvern |
| ITEM_BEAST_WHISTLE | Cut/Sweet Scent | Primalis |
| ITEM_STORM_COMPASS | Fly (between islands) | Aetheron |
| ITEM_PHANTOM_LANTERN | Defog/Flash in caves | Ashenveil |

### Starter Pokémon
Three starters loose in Professor Sollis's lab (NOT on a table in Pokéballs):
- Totodile (Water) — near the door
- Chimchar (Fire) — on the highest bookshelf
- Rowlet (Grass/Flying) — sitting on the Warden's journal on the desk

Rival Cass gets the counter-type:
- Player picks Totodile → Cass gets Chimchar
- Player picks Chimchar → Cass gets Rowlet
- Player picks Rowlet → Cass gets Totodile

### Mega Evolution
Triggered by Seal Shards — one per island, earned by resolving the island's
crisis. NOT obtained from a single NPC like standard Mega Evolution.
- Feraligatr Seal Shard: Thalvern (Sunken Kingdom)
- Infernape Seal Shard: Emberveil (Volcanic Island)
- Decidueye Seal Shard: Ashenveil (Dead Island)

### Z-Moves
Z-Crystals scattered as relics of the ancient civilization. Found in ruins,
given by elders, guarded by gym leaders as sacred objects.

### Battle Terrain
Each island has a default battle terrain/weather matching its environment.
Set per map in map JSON. Gym leader battles inherit island terrain.

---

## Narrative Context (Summary)

### Villain: Marshal Vael Dorne
Former Covenant Marshal. Morally grey — wants to destroy the sealed
legendaries to prevent the corrupt Covenant from weaponizing them.
Right about the problem, catastrophic in his solution. Knew the player's
parent personally. Did not kill the Warden — the Covenant did.

### Player
Child of the Warden — the person who maintained the legendary seal network.
Warden died 6 months before the game starts under suspicious circumstances.
Inherits the Tennyson (boat) and the Warden's incomplete journal.

### Rival: Cass (Cassian Vell)
Childhood friend. Gets recruited by the Covenant early believing it's an
honor. Slowly realizes they're being used. Defects on Aetheron (Sky Island).
Joins the player for the Final Island.

### Professor: Maren Sollis
Based on Haven Isle. The player's guardian after the Warden's death.
Gives the player their starter. Knows more than she says early on.
Use placeholder sprite FEMALE_SCIENTIST until custom sprite is ready.

### The Covenant
The region's naval/political governing body. Publicly claims legendaries
are myths. Privately exploiting seal energy as a power source — the same
thing that destroyed Ashenveil (Dead Island) 20 years ago.

### Legendary Pokémon
Story-only battles throughout the game. Catchable POST-GAME only.
Each legendary is sealed on its island and tied to the environment.
Do NOT make legendaries catchable during main story.

### Branching Narrative
The game tracks player choices via flags and relationship variables.
See PELAGIOS_BRIEF.md for full flag/variable list.
Key relationship variables (0-3 scale):
- VAR_CASS_RELATIONSHIP
- VAR_DORNE_RELATIONSHIP
- VAR_MAREN_RELATIONSHIP
- VAR_SEVER_RELATIONSHIP
- VAR_DAGAN_RELATIONSHIP

---

## Constant Space Layout (CRITICAL — read before adding flags/vars/trainers)

### SAVE-COMPATIBILITY BREAK (2026-06-11)
A capacity refactor expanded trainer and var space. **All old saves are invalid.**
Every system flag's numeric ID shifted up by 0xA0, and SaveBlock1 grew by 52 bytes.
This is accepted (dev phase, no saves to protect). If/when the game ships or enters
a save-stable phase, this is the last cheap moment to make such changes.

### SAVE-COMPATIBILITY BREAK #2 — MAPSEC widening (2026-06-14)
**All old saves are invalid again** (still accepted, dev phase). Cleared the MAPSEC
u8 ceiling (MAPSEC_NONE was 255, the u8 max, blocking Primalis+ region-map sections).
- `mapsec_u8_t`/`metloc_u8_t` (include/gametypes.h) widened **u8 -> u16**: every
  variable/parameter that *handles* a MAPSEC id or Pokémon Met Location is 16-bit now.
- The **stored** Pokémon metLocation could NOT become a full u16 — BoxPokemon cannot
  grow (the PC box storage save region is at its flash budget; a wider BoxPokemon
  trips `PokemonStorageFreeSpace` in src/save.c and `RecordedBattleSaveFreeSpace` in
  recorded_battle.c). Instead metLocation became a **9-bit bitfield** (`u32 metLocation:9`)
  repacked into the existing 12-byte `struct PokemonSubstruct3` (include/pokemon.h),
  reclaiming that substruct's former `unused_0B:1`. **Substruct stays EXACTLY 12 bytes
  → BoxPokemon and SaveBlock sizes are UNCHANGED** — the break is purely the substruct
  *bit layout* changing (existing caught-mon metLocation bits move), so old saves'
  Pokémon read wrong. New saves are correct.
- METLOC_* sentinels moved 0xFD/0xFE/0xFF → **0x1FD/0x1FE/0x1FF** (top of the 9-bit
  range) so a real MAPSEC id can never collide with them.
- **New MAPSEC ceiling = 508** (sentinels 509-511; Kanto NOT removed — see Known Issues
  "MAPSEC CEILING … RESOLVED"). MAPSEC_NONE is the enum sentinel right after the last real
  MAPSEC, so it floats up as MAPSECs are appended: it is **273** after Aetheron's 6 MAPSECs
  (was 267 after Ashenveil). Still **~235 free MAPSEC slots** below the 508 ceiling for
  every later island.
- Accessor/caller fixes: src/pokemon.c SetBoxMonData MET_LOCATION uses SET16 (was SET8);
  src/scrcmd.c setmonmetlocation local widened to metloc_u8_t; src/battle_controllers.c
  REQUEST_MET_LOCATION_BATTLE GET emits 2 bytes (size=2); header signatures aligned to
  the typedef in map_preview_screen.h + pokenav.h. Build clean (gmake exit 0).

#### MAP-HEADER MAPSEC WIDTH FIX (2026-06-14, follow-up to BREAK #2; ROM-only, NO new save break)
The widening of `mapsec_u8_t` -> u16 made `struct MapHeader.regionMapSectionId`
(include/global.fieldmap.h) a **2-byte** field, but the map-header ASM emitter in
**tools/mapjson/mapjson.cpp** was left emitting the region section as `\t.byte ` (1 byte).
This was the source of the `value 0x100 truncated to 0x0` … `0x10a truncated to 0xa`
warnings on every Primalis (256-261) and Ashenveil (262-266) map. WORSE: because the C
struct reads regionMapSectionId as u16 (0x14-0x15) while the asm emitted only 1 byte at
0x14, the **entire header tail was misaligned by 1 byte on EVERY map** — `cave`,
`weather`, `mapType`, `floorNumber`, the flags bitfield, and `battleType` were all read
one byte off from what the asm laid down (silent wrong weather/map-type/battle-scene
region-wide, not just MAPSEC>=256).
- **FIX:** mapjson.cpp now emits `\t.2byte ` for `region_map_section`. This both stops the
  truncation AND realigns every following field to the struct (`.byte requires_flash` now
  lands on `cave` at 0x16, weather 0x17, mapType 0x18, floorNumber 0x19, filler 0x1A,
  map_header_flags bitfield 0x1B, battle_scene 0x1C). The `map_header_flags` macro
  (asm/macros/map.inc) was already a correct single `.byte` for the 0x1B bitfield —
  unchanged. mapjson.cpp is the ONLY raw-asm MapHeader emitter (no hand-written
  `map_header` macro exists; the 9 Pelagios build_*_mapjson.py generators all feed
  region_map_section as a string into mapjson.cpp, so fixing the tool fixes all of them).
- **Stale struct comments corrected** in global.fieldmap.h to match the real compiler
  layout (regionMapSectionId 0x14-0x15, cave 0x16, weather 0x17, mapType 0x18,
  floorNumber 0x19, filler 0x1A, bitfield 0x1B, battleType 0x1C). Field order/types were
  already correct (compiler honored the u16); only the asm emitter + comments were wrong.
- **Rebuild method:** `rm -f tools/mapjson/mapjson` (force tool rebuild from changed .cpp)
  + `find data/maps -name header.inc -delete` + `find build -name header.inc -delete`,
  then `gmake -j$(sysctl -n hw.ncpu)`. All **1074** map header.inc regenerated through the
  fixed tool; **ZERO** truncation warnings; build exit 0; pokeemerald.gba produced.
  Spot-check: data/maps/Primalis_TheHeartwood/header.inc now reads
  `.2byte MAPSEC_PRIMALIS_HEARTWOOD` (was `.byte`); vanilla PetalburgCity likewise
  `.2byte MAPSEC_PETALBURG_CITY`. grep confirms 0 headers still use `.byte MAPSEC*`.
- **This is ROM-only** (map headers live in ROM, not the saveblock) — NO new save break
  beyond the already-accepted BREAK #2. It makes ROM match the (correct) u16 C struct.
- **RULE for all future MAPSEC work:** the map-header region section MUST be emitted as
  `.2byte` (16-bit). Do NOT revert mapsec_u8_t to u8 (that re-breaks the >255 ceiling) and
  do NOT let any new MapHeader emitter use `.byte` for the section. Linker after fix:
  EWRAM 86.46% / IWRAM 86.63% / ROM 80.03%.
- **REMAINING:** in-emulator (mGBA) playtest should confirm the map-name popup AND weather
  read correctly on Primalis/Ashenveil maps (the realignment fix is unverifiable without
  running the ROM).

### Trainer flags
- `MAX_TRAINERS_COUNT_EMERALD`: was 864, now **1024** (include/constants/opponents.h)
- Trainer defeat flags occupy **0x500 - 0x8FF** (was 0x500-0x85F)
- `TRAINERS_COUNT_EMERALD`: **941** (Convergence added 940; Aetheron 933-939)
- **Free trainer headroom: 1024 - 941 = 83 slots** (island set now COMPLETE)
- Convergence (FINAL ISLAND, 940): TRAINER_LEADER_DORNE 940 — the game's final battle
  (Ending 1 / STOP path only). Party verbatim from CONVERGENCE_BRIEF.md: Bisharp 65,
  Hydreigon 66, Weavile 67, Aegislash 68, Garchomp 69, Kyurem 70 (the brief's "Zekrom or
  Kyurem" Lv.70 legendary placeholder — Kyurem chosen so it doesn't dupe Voss's Zekrom).
  Pic: Magma Leader Maxie (no custom pic; commanding-antagonist placeholder per the brief's
  "fitting placeholder" guidance). 4x Full Restore, Smart Switching AI. NO generic trainers,
  NO gyms beyond Dorne (narrative-only badges policy unaffected — Dorne is not a gym).
- Aetheron trainers (933-939, parties from AETHERON_BRIEF.md verbatim; 3 gyms):
  - Community guardians: TRAINER_GUARDIAN_AETHERON_1 933 (Sael, Cooltrainer F),
    _2 934 (Renn, Cooltrainer M). Covenant officers: TRAINER_COVENANT_AETHERON_1
    935 (Mael, Cooltrainer M), _2 936 (Sera, Cooltrainer F).
  - Gym leaders (Pic Leader): TRAINER_LEADER_AETHERON_GALE 937 (Flying, Cooltrainer F),
    _ARC 938 (Electric, Cooltrainer M), _VOSS 939 (Electric/Steel, Cooltrainer M;
    party ends in Zekrom Lv.64 — the brief's "captured legendary"; Zekrom exists in the
    expansion so the Electivire substitute was not needed; Voss holds 4x Full Restore).
    BADGES NARRATIVE-ONLY: the 3 gyms set only FLAG_AETHERON_GYM*_CLEAR, no FLAG_BADGE*_GET.
- Primalis trainers (925-932, parties from PRIMALIS_BRIEF.md verbatim; 4 gyms):
  - Zoan guardians: TRAINER_ZOAN_PRIMALIS_1 925 (Sera, Cooltrainer F), _2 926 (Vex,
    Cooltrainer M), _3 927 (Rael, Cooltrainer M), _4 928 (Cael, Cooltrainer M).
  - Gym leaders: TRAINER_LEADER_PRIMALIS_FERN 929 (Grass, Cooltrainer F), _SCALE 930
    (Dragon, Cooltrainer M), _THORN 931 (Grass/Dragon, Cooltrainer M), _MAKO 932
    (Dragon/Water boss Elder Mako, Hiker pic — no shark/elder pic exists).
- Thalvern trainers (910-916, parties from THALVERN_BRIEF.md verbatim; ONLY 3 gyms):
  - Generic: TRAINER_SCHOLAR_THALVERN_1 910 (Wren), TRAINER_COVENANT_THALVERN_1 911 (Holt),
    _2 912 (Sael), _3 913 (Cael). Pics: all Scientist FRLG (Sael Female).
  - Gym leaders: TRAINER_LEADER_THALVERN_TIDE 914 (Water, Pic Fisherman), _PSALM 915
    (Psychic, Pic Cooltrainer F), _LENS 916 (Water/Psychic, Pic Cooltrainer M).
- Ironhold gym leaders: TRAINER_LEADER_IRONHOLD_PETRA 864, FORGE 865, ROOK 866,
  SEVER 867; summit garrison TRAINER_COVENANT_IRONHOLD_5-9 = 868-872
  (parties from IRONHOLD_BRIEF.md, in src/data/trainers.party)
- Sirocco trainers (873-887, parties from SIROCCO_BRIEF.md verbatim):
  - Generic: TRAINER_GILTCLAW_SIROCCO_1 873, TRAINER_TRAVELER_SIROCCO_1 874,
    TRAINER_GILTCLAW_SIROCCO_2 875, _3 876, _4 877, _5 878, _6 879, _7 880,
    _8 881, _9 882, TRAINER_GILTCLAW_SIROCCO_10 883
  - Gym leaders: TRAINER_LEADER_SIROCCO_SILT 884 (Ground), _CRAG 885 (Rock),
    _MIRIA 886 (Ground/Rock), _DAGAN 887 (Ground/Rock boss). Pic for Dagan =
    Rich Boy placeholder per brief; Silt/Crag = Hiker, Miria = Cooltrainer F.
- Emberveil trainers (888-899, parties from EMBERVEIL_BRIEF.md verbatim):
  - Cult members: TRAINER_CULTMEMBER_EMBERVEIL_1 888 (Sera), _2 889 (Bren),
    _3 890 (Mira), _4 891 (Rem), _5 892 (Voss), _6 893 (Tane), _7 894 (Horne),
    _8 895 (Cael). Pics: _1/_2 Youngster, _3 Cooltrainer F, _4-_8 Cooltrainer M.
  - Gym leaders: TRAINER_LEADER_EMBERVEIL_CINDER 896 (Fire, Pic Youngster),
    _SLAG 897 (Fire/Ground, Pic Hiker — no Worker pic exists), _VEX 898
    (Fire/Flying, Pic Cooltrainer M), _SOLACE 899 (Fire, Pic Lady).
- Schism trainers (900-909, parties from SCHISM_BRIEF.md verbatim):
  - Generic faction (all Pic Cooltrainer M/F): TRAINER_ICEFACTION_SCHISM_1 900 (Heln),
    _SCHISM_2 901 (Vera, F), TRAINER_POISONFACTION_SCHISM_1 902 (Osk), _SCHISM_2 903
    (Fen), TRAINER_ICEFACTION_SCHISM_3 904 (Rael), TRAINER_POISONFACTION_SCHISM_3 905
    (Siv). Alolan Grimer written as `Grimer-Alola` (parser -> SPECIES_GRIMER_ALOLA).
  - Gym leaders: TRAINER_LEADER_SCHISM_SLEET 906 (Ice, Pic Youngster), _EIRA 907
    (Ice, Pic Cooltrainer F), _MURK 908 (Poison/Ghost, Pic Cooltrainer M), _DRENN 909
    (Poison/Dragon, Pic Scientist FRLG — the only scientist pic; brief asked SCIENTIST_M).
    ALL FOUR badges are NARRATIVE-ONLY (engine badge flags exhausted at 08) — they set
    only FLAG_SCHISM_GYM*_CLEAR, no FLAG_BADGE*_GET.

### System flags
- `SYSTEM_FLAGS` base: was 0x860, now **0x900** (derived from TRAINER_FLAGS_END + 1,
  so it floats automatically with MAX_TRAINERS_COUNT — never hardcode it)
- All FLAG_SYS_*/badge/landmark/daily flags are SYSTEM_FLAGS-relative; they shifted
  up transparently. DAILY_FLAGS and FLAGS_COUNT grew by 0xA0 (160).
- `FLAGS_COUNT`: was ~0x960, now **~0xA00** (still far below SPECIAL_FLAGS_START 0x4000)

### Story flags (Pelagios)
- **STORY BLOCK 1 (0x4A7-0x4EF) is now FULL.** 0x4A7 onward. Ironhold flags end at
  0x4E3 (FLAG_HIDE_IRONHOLD_SEVER_GATE = 0x4E3, gate-Sever cameo hide flag).
- Sirocco story flags claim 0x4E4-0x4EE (11 flags):
  FLAG_SIROCCO_ARRIVED 0x4E4, GYM1_CLEAR 0x4E5, GYM2_CLEAR 0x4E6,
  GYM3_CLEAR 0x4E7, GYM4_CLEAR 0x4E8, DAGAN_ESCAPED 0x4E9, DEX_MET 0x4EA,
  BURIED_CITY_FOUND 0x4EB, SEAL_FOUND 0x4EC, CIPHER_FOUND 0x4ED,
  NORTH_TERRITORY_OPEN 0x4EE. (FLAG_SIROCCO_RESOLVED stays at 0x4AD; the Rawst
  Berry is a hidden-item flag at 0x267, not here.)
- FLAG_EMBERVEIL_ARRIVED = 0x4EF — fills the LAST slot of block 1 (block 1 FULL).
  FLAG_EMBERVEIL_RESOLVED stays at 0x4AE; FLAG_SOLACE_ALT_ENDING at 0x4B9 (reused).

### Story flags — Pelagios STORY BLOCK 2 (0x493-0x4A6, 20 contiguous flags)
- Block 1 is full, so Emberveil and later islands overflow into a fresh contiguous
  run of vanilla FLAG_UNUSED slots **immediately preceding** block 1: **0x493-0x4A6**
  (20 flags). Safe: well below trainer flags (0x500), SYSTEM_FLAGS (0x900+), and the
  hidden-items range. These were genuine `FLAG_UNUSED_0x493..0x4A6` with no other refs.
- **Emberveil claims 0x493-0x49C (10 flags):**
  FLAG_EMBERVEIL_GYM1_CLEAR 0x493, GYM2_CLEAR 0x494, GYM3_CLEAR 0x495,
  GYM4_CLEAR 0x496, FLAG_LAVA_BOOTS_OBTAINED 0x497, FLAG_WARDEN_NOTES_EMBERVEIL 0x498,
  FLAG_EMBERVEIL_PATH_B 0x499, FLAG_EMBERVEIL_SEAL_FOUND 0x49A,
  FLAG_EMBERVEIL_CIPHER_FOUND 0x49B, FLAG_SOLACE_TOLD_TRUTH 0x49C.
- **Schism claims block 2 0x49D-0x4A6 (10 flags) — BLOCK 2 IS NOW FULL:**
  FLAG_SCHISM_ARRIVED 0x49D, GYM1_CLEAR 0x49E, GYM2_CLEAR 0x49F, GYM3_CLEAR 0x4A0,
  GYM4_CLEAR 0x4A1, FLAG_EIRA_SCAR_PASS 0x4A2, FLAG_DRENN_SCAR_PASS 0x4A3,
  FLAG_SCAR_RUINS_FOUND 0x4A4, FLAG_EIRA_CEASEFIRE_WILLING 0x4A5,
  FLAG_DRENN_CEASEFIRE_WILLING 0x4A6. (FLAG_SCHISM_RESOLVED 0x4AF, FLAG_DRENN_ALIVE
  0x4BB, FLAG_CIPHER_5_FOUND 0x4C1 already existed — REUSED, not redefined.)
- After block 2 fills, the next overflow must find another contiguous FLAG_UNUSED run
  (never collide with trainer flags 0x500-0x8FF or SYSTEM_FLAGS 0x900+).

### Story flags — Pelagios STORY BLOCK 3 (scattered FLAG_UNUSED slots)
- Schism needed 16 story flags but block 2 had only 10 free, so the remaining 6 went
  into the only remaining genuine FLAG_UNUSED slots in the 0x4xx range. These are NOT
  contiguous — there is no contiguous unused run left below 0x500 (0x468-0x492 are
  vanilla collected-item flags; 0x4F0-0x4FE are vanilla gym/E4 flags). The 6 scattered
  slots used (all were real FLAG_UNUSED with no other refs):
  - FLAG_SCHISM_SEAL_NORTH_FOUND 0x468, FLAG_SCHISM_SEAL_SOUTH_FOUND 0x470,
    FLAG_SCHISM_SEAL_NORTH_DONE 0x472, FLAG_SCHISM_SEAL_SOUTH_DONE 0x479,
    FLAG_SCHISM_CEASEFIRE 0x4F9, FLAG_SCHISM_CIPHER_FOUND 0x4FA.
- **Remaining free scattered FLAG_UNUSED in 0x4xx: 0x4FF only (1 flag).** The 0x4xx
  story region is exhausted. Thalvern/Gildhaven/later islands now draw from STORY
  BLOCK 4 (0x26C-0x2BB) instead — see next subsection. (0x4FF is left as a lone spare.)

### Story flags — Pelagios STORY BLOCK 4 (0x26C-0x2BB, 80 contiguous flags) — RESERVED 2026-06-13

- **The 0x4xx region is full** (blocks 1/2/3 + only 0x4FF spare). Remaining islands draw
  from a fresh contiguous run BELOW the hidden-item region: **0x26C-0x2BB, 80 flags.**
  Bounded below by Pelagios hidden-item flags (end 0x26B) and above by vanilla object-hide
  flags (FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG 0x2BC). Same repurpose pattern as blocks
  2/3 (genuine FLAG_UNUSED slots).
- **For:** Thalvern / Gildhaven / Primalis / Ashenveil / Aetheron / Convergence.
- **CONSUMED 2026-06-13 — Thalvern: 0x26C-0x275 (10 flags).** FLAG_THALVERN_ARRIVED 0x26C ..
  FLAG_NUMA_VESS_CONFRONTED 0x275 (Thalvern had no hidden items, reused FLAG_THALVERN_RESOLVED
  0x4B0 / FLAG_DEX_ALIVE 0x4BA / FLAG_CIPHER_6_FOUND 0x4C2).
- **CONSUMED 2026-06-13 — Gildhaven: 0x276-0x281 (12 flags).** FLAG_GILDHAVEN_ARRIVED 0x276,
  GYM1_CLEAR 0x277, GYM2_CLEAR 0x278, GYM3_CLEAR 0x279, GYM4_CLEAR 0x27A, DAGAN_MET 0x27B,
  LACE_TALKED 0x27C, MANOR_ACCESS 0x27D, COVENANT_MAP_SEEN 0x27E, SEAL_FOUND 0x27F,
  CIPHER_FOUND 0x280, FLAG_CASS_GILDHAVEN_SEEN 0x281. Reused pre-allocated
  FLAG_GILDHAVEN_RESOLVED 0x4B1 / FLAG_CIPHER_7_FOUND 0x4C3 (cipher 7). Gildhaven had NO
  hidden items.
- **CONSUMED 2026-06-14 — Primalis: 0x282-0x28E (13 flags).** FLAG_PRIMALIS_ARRIVED 0x282,
  GYM1_CLEAR 0x283 (Fern), GYM2_CLEAR 0x284 (Scale), GYM3_CLEAR 0x285 (Thorn), GYM4_CLEAR
  0x286 (Mako), TRUST_EARNED 0x287, ORAL_HISTORY_HEARD 0x288, LENS_MET 0x289, RUINS_FOUND
  0x28A, SEAL_FOUND 0x28B, CIPHER_FOUND 0x28C, FLAG_BEAST_WHISTLE_OBTAINED 0x28D,
  FLAG_PRIMALIS_TOKEN_GIVEN 0x28E. Reused pre-allocated FLAG_PRIMALIS_RESOLVED 0x4B2 /
  FLAG_CIPHER_8_FOUND 0x4C4 (cipher 8). Primalis had NO hidden items.
  (BLOCK 4 was 45 free after Primalis; Ashenveil + Aetheron + Convergence have since
  consumed through 0x2A6 — see the consumption lines below.)
- **CONSUMED — Ashenveil: 0x28F-0x296 (8 flags), Aetheron: 0x297-0x29E (8 flags).**
  (Allocated by their constants passes; flags.h is authoritative for exact names.)
- **CONSUMED 2026-06-14 — Convergence (FINAL ISLAND): 0x29F-0x2A6 (8 flags).**
  FLAG_CONVERGENCE_ARRIVED 0x29F, FLAG_CONVERGENCE_GATHERING_SEEN 0x2A0,
  FLAG_SOLLIS_CONFESSION_HEARD 0x2A1, FLAG_DORNE_FINAL_BATTLE_DONE 0x2A2,
  FLAG_ENDING_STOP_PLAYED 0x2A3, FLAG_ENDING_HELP_PLAYED 0x2A4,
  FLAG_ENDING_TRUE_PLAYED 0x2A5, FLAG_CONVERGENCE_COMPLETE 0x2A6. REUSED pre-allocated
  (NOT redefined): FLAG_DORNE_CHOICE_STOP/HELP/DEFER 0x4B5-0x4B7,
  FLAG_TRUE_ENDING_UNLOCKED 0x4B8, FLAG_CASS_DEFECTED 0x4BC. NOTE: there was NO
  pre-allocated FLAG_CONVERGENCE_RESOLVED (the RESOLVED set ends at FLAG_AETHERON_RESOLVED
  0x4B4); per the brief FLAG_CONVERGENCE_COMPLETE 0x2A6 plays the "island resolved / any
  ending played" role. NO new cipher (CIPHER_1-9 complete). Convergence had NO hidden items.
  **BLOCK 4 free from 0x2A7-0x2BB = 21 flags remaining.** The game's island set is now
  complete; this headroom is for post-launch systems.
- **Boundary placeholders in flags.h:** FLAG_PELAGIOS_BLOCK4_RESERVED_START (0x26C) and
  FLAG_PELAGIOS_BLOCK4_RESERVED_END (0x2BB), with a full comment header. The interior
  0x26D-0x2BA stay FLAG_UNUSED_0x### until each island's constants pass renames the next
  contiguous slots (do NOT pre-name per-island flags).
- **COLLISION-VERIFIED 2026-06-13:** every value 0x26C-0x2BB has exactly ONE FLAG_UNUSED_*
  definition in flags.h and is referenced NOWHERE outside flags.h. Method: grepped each of
  the 80 FLAG_UNUSED_0x### names across data/ src/ include/ asm/ (.c/.h/.inc/.s/.json/.txt)
  excluding flags.h — zero external refs; and confirmed each raw hex value has exactly one
  `#define FLAG_* 0x###` in flags.h (no aliasing). SAFE against trainer flags (0x500-0x8FF),
  SYSTEM_FLAGS (0x900+), the hidden-items range, and SPECIAL_FLAGS_START (0x4000).
- **Per-island budget plan:** the per-island RESOLVED flags (0x4B0-0x4B4) and CIPHER_6-9
  (0x4C2-0x4C5) are ALREADY pre-allocated in the 0x4xx region (grep before redefining), so
  each remaining island needs only ~10-13 FRESH story flags (ARRIVED + 4 GYM_CLEAR +
  SEAL_FOUND + island-specific CIPHER_FOUND alias + a few scene/relationship flags). Real
  total for 5 islands + Convergence ≈ 50-60 flags — 80 is comfortable headroom.
- **HIDDEN-ITEM PRESSURE (flagged):** block 4 consumes the last big hidden-item-capable run
  below the trainer wall. After it, the only free hidden-item slot is 0x264 (1). Future
  islands' hidden items (~2/island) will need a carve-out — take it from the TOP of block 4
  (over-provisioned vs. the ~50-60 real need) or cut an unreachable vanilla Hoenn hidden
  item. Not blocking now; revisit at Thalvern/Gildhaven hidden-item time.

### Hidden-item flags (Pelagios)
- Hidden-item flags MUST be in the hidden-items range (>= FLAG_HIDDEN_ITEMS_START,
  which is 0x1F4 for the Emerald/Hoenn map block). The bg_hidden_item_event macro
  (asm/macros/map.inc) hard-errors if flag < FLAG_HIDDEN_ITEMS_START — never put a
  hidden-item flag in the 0x4xx story block.
- Vanilla Emerald hidden items occupy FLAG_HIDDEN_ITEMS_START + 0x00..0x6F.
  Pelagios claims +0x71 onward (reusing the trailing FLAG_UNUSED_0x265+ slots):
  - FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE = +0x71 (0x265)
  - FLAG_HIDDEN_ITEM_IRONHOLD_IRON     = +0x72 (0x266)
  - FLAG_HIDDEN_ITEM_SIROCCO_BERRY     = +0x73 (0x267, Rawst Berry, DesertRoute1)
  - FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1  = +0x74 (0x268, Rawst Berry, LavaRoute1)
  - FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY2  = +0x75 (0x269, Rawst Berry, LavaRoute1 second)
  - FLAG_HIDDEN_ITEM_SCHISM_1          = +0x76 (0x26A, ice side)
  - FLAG_HIDDEN_ITEM_SCHISM_2          = +0x77 (0x26B, the Scar)
  NOTE: a duplicate FLAG_UNUSED_0x26A definition was removed when 0x26A/0x26B were claimed.
  HIDDEN-ITEM SPACE WARNING: 0x26C onward was claimed by STORY BLOCK 4 (see above), so the
  only remaining free hidden-item slot below the trainer wall is **0x264 (1 slot)**. Future
  islands needing hidden items must carve a buffer from the TOP of STORY BLOCK 4 (0x2Bx,
  over-provisioned) or cut an unreachable vanilla Hoenn hidden item. Flag before allocating.

### Story flags — Ashenveil Isle (STORY BLOCK 4, 0x28F-0x296, 8 flags, 2026-06-14)
- Ashenveil is the DEAD ISLAND — NO gyms, NO trainers. Pure narrative. Only 8 fresh story
  flags needed; allocated contiguously from 0x28F (after Primalis, which ended at 0x28E):
  FLAG_ASHENVEIL_ARRIVED 0x28F, FLAG_ASHENVEIL_OUTPOST_MET 0x290,
  FLAG_PHANTOM_LANTERN_OBTAINED 0x291, FLAG_ASHENVEIL_COVENANT_DOCS_FOUND 0x292,
  FLAG_ASHENVEIL_DORNE_MET 0x293, FLAG_SEA_CHART_FOUND 0x294 (ACTIVATES Aetheron in the
  boat menu — NOT boat tier), FLAG_MORTHAS_ENCOUNTERED 0x295,
  FLAG_ASHENVEIL_CIPHER_FOUND 0x296.
- REUSED pre-allocated (verified existing — NOT redefined): FLAG_ASHENVEIL_VISITED 0x4B3,
  FLAG_DORNE_CHOICE_STOP 0x4B5 / _HELP 0x4B6 / _DEFER 0x4B7, FLAG_TRUE_ENDING_UNLOCKED 0x4B8,
  FLAG_CIPHER_9_FOUND 0x4C5 (cipher 9; FLAG_ASHENVEIL_CIPHER_FOUND 0x296 is the island-local
  pair, matching the Primalis/Gildhaven convention).
- **STORY BLOCK 4 FREE after Ashenveil: 0x297-0x2BB (37 flags)** for Aetheron/Convergence.

### Story flags — Aetheron Isle (Sky Island — STORY BLOCK 4, 0x297-0x29E, 8 flags, 2026-06-14)
- Aetheron is the SKY ISLAND — reached via the Knock Up Stream + ITEM_SEA_CHART
  (FLAG_SEA_CHART_FOUND 0x294), NOT by boat tier. 8 fresh story flags, allocated
  contiguously from 0x297 (after Ashenveil ended at 0x296):
  FLAG_AETHERON_ARRIVED 0x297, FLAG_AETHERON_GYM1_CLEAR 0x298 (Gale),
  FLAG_AETHERON_GYM2_CLEAR 0x299 (Arc), FLAG_AETHERON_GYM3_CLEAR 0x29A (Voss),
  FLAG_AETHERON_CASS_SEEN 0x29B, FLAG_AETHERON_INSTALLATION_FOUND 0x29C,
  FLAG_AETHERON_SEAL_FOUND 0x29D, FLAG_STORM_COMPASS_OBTAINED 0x29E.
- REUSED pre-allocated (verified existing — NOT redefined): FLAG_AETHERON_RESOLVED 0x4B4,
  FLAG_CASS_DEFECTED 0x4BC. NO new cipher (CIPHER_1-9 set is COMPLETE — Ashenveil took
  cipher 9; the brief explicitly states "Cipher 9 already found on Ashenveil — no new
  cipher here").
- **STORY BLOCK 4 FREE after Aetheron: 0x29F-0x2BB (29 flags)** for Convergence/future.

### Vars
- `VARS_END`: was 0x40FF, now **0x410F** (include/constants/vars.h). +16 u16 vars.
- 0x4000-0x40FF unchanged (vanilla + Pelagios relationship/Haven/Ironhold vars).
- Expanded Pelagios block **0x4100-0x410F**:
  - VAR_BOAT_TIER 0x4100 (0=Dinghy,1=Sloop,2=Brigantine,3=Galleon)
  - VAR_SIROCCO_PROGRESS 0x4101, VAR_EMBERVEIL_PROGRESS 0x4102,
    VAR_SCHISM_PROGRESS 0x4103, VAR_THALVERN_PROGRESS 0x4104,
    VAR_GILDHAVEN_PROGRESS 0x4105, VAR_PRIMALIS_PROGRESS 0x4106,
    VAR_ASHENVEIL_PROGRESS 0x4107, VAR_AETHERON_PROGRESS 0x4108,
    VAR_CONVERGENCE_PROGRESS 0x4109
  - VAR_SCHISM_CEASEFIRE_PROGRESS 0x410A (Schism, was a reserved spare;
    0=none, 1=skeptical, 2=willing, 3=done)
  - VAR_PELAGIOS_RESERVED_0x410B through 0x410F (5 reserved spare slots remain)
- Beyond 0x410F requires another VARS_END bump (saveblock impact) — STOP and flag.
- Emberveil added NO new vars. Schism added ONE: VAR_SCHISM_CEASEFIRE_PROGRESS 0x410A.
  Thalvern added NO new vars at CONSTANTS time (VAR_THALVERN_PROGRESS 0x4104 pre-existed),
  but its SCRIPTS later consumed 0x410B (path determinant) and 0x410C (day counter) —
  script-owned, vars.h may still label them RESERVED. Gildhaven added NO new vars:
  VAR_GILDHAVEN_PROGRESS 0x4105 pre-existed (states 0=not arrived .. 7=resolved per brief);
  the corruption mechanic is purely progress-gated (0-2 normal dialogue, 3+ corrupted) and
  Dagan rides the existing VAR_DAGAN_RELATIONSHIP 0x40FB — no spare consumed.
  Primalis (2026-06-14) added NO new vars: VAR_PRIMALIS_PROGRESS 0x4106 pre-existed (states
  0=not arrived .. 7=resolved per brief); the Mako-trust mechanic folds into progress states
  + FLAG_PRIMALIS_TRUST_EARNED — no counter var needed. The brief itself says "No additional
  vars needed — 3 spares preserved." No spare consumed; no var need flagged.
  Ashenveil (2026-06-14) added NO new vars: VAR_ASHENVEIL_PROGRESS 0x4107 pre-existed
  (0=not arrived, 1=arrived, 2=lantern, 3=dead city, 4=Dorne met, 5=sea chart, 6=Morthas,
  7=visited/complete) and VAR_DORNE_RELATIONSHIP 0x40F8 pre-existed (set to max on the HELP
  choice). The brief confirms "No new vars needed. 3 spares preserved." No spare consumed.
  **Genuinely-free spares after Thalvern's script usage: 0x410D, 0x410E, 0x410F (3).**
- **VAR AUDIT 2026-06-13:** every remaining island's PROGRESS var is ALREADY pre-allocated
  (Thalvern 0x4104 .. Convergence 0x4109), so those 5 islands + Convergence need ZERO new
  vars for progress. The **5 free spares (0x410B-0x410F)** cover only ~5 island-specific
  extra vars (relationship/ceasefire-style, like Schism's 0x410A). If 2+ of the remaining
  islands each want a dedicated extra var (e.g. Aetheron's Cass-defection track + another),
  the spares run out and a VARS_END bump past 0x410F (saveblock-impacting) becomes necessary
  — STOP and flag then. Sufficient for now; foreseeably tight by Aetheron/Convergence.

### Items (Pelagios key items, include/constants/items.h + src/data/items.h)
- Vanilla items run to 873. Pelagios key items 874-885 (Haven/Ironhold/Sirocco batch).
- Emberveil: ITEM_LAVA_BOOTS 877 already existed (Haven key-item batch; data matches
  brief, icon gItemIcon_Bicycle placeholder — unchanged). NEW: ITEM_WARDEN_NOTES 886
  (icon Powder/EnergyPowder), ITEM_SEAL_SHARD_EMBERVEIL 887 (icon RedOrb, Infernape
  Mega stub). ITEM_SEAL_SHARD_IRONHOLD stays a #define alias to ITEM_SEAL_SHARD_1 (882).
- Schism: 4 new key items 888-891 (all importance 1, CannotUse, placeholder icons):
  ITEM_SCAR_PASS_ICE 888 (Eira's Scar pass, icon Powder/EnergyPowder),
  ITEM_SCAR_PASS_POISON 889 (Drenn's Scar pass, icon Powder/EnergyPowder),
  ITEM_SEAL_SHARD_GLACITH 890 (icon BlueOrb), ITEM_SEAL_SHARD_TOXARA 891 (icon BlueOrb,
  both Seal Shard stubs — not awarded yet).
- Thalvern: 3 new key items 892-894 (all importance 1, CannotUse, placeholder icons):
  ITEM_COVENANT_ACCESS_CARD 892 (icon Powder/EnergyPowder), ITEM_SEAL_SHARD_THALVERN 893
  (icon BlueOrb — THE Feraligatr Mega Evolution trigger; Mega wiring is future work),
  ITEM_DEX_NOTES 894 (icon Powder/EnergyPowder). ITEM_SONAR_LENS (878, Dive replacement)
  already existed — not re-added.
- Gildhaven: 2 new key items 895-896 (both importance 1, CannotUse, placeholder icons):
  ITEM_VANE_MANOR_KEY 895 ("Manor Key", icon BasementKey/OldKey — Lace gives it post-Gym3,
  unlocks VaneManor + the Exchange passage), ITEM_SEAL_SHARD_GILDHAVEN 896 ("Seal Shard",
  icon RedOrb — Fairy/Dark seal-shard stub, not awarded yet; Mega wiring is future work).
- Primalis (2026-06-14): ITEM_BEAST_WHISTLE PRE-EXISTED at 879 ("Beast Whistle", icon
  SilphScope — Cut/Sweet Scent replacement, batch-created in the Haven key-item setup) —
  REUSED, NOT redefined. 2 new key items 897-898 (both importance 1, CannotUse, placeholder
  icons): ITEM_PRIMALIS_TOKEN 897 ("Primal Token", icon ContestPass — Elder Mako's carved
  bone token, used on the Final Island), ITEM_SEAL_SHARD_PRIMALIS 898 ("Seal Shard", icon
  RedOrb — Grass/Dragon seal-shard stub, not awarded; plain stub per brief, NOT a Mega
  trigger).
- Ashenveil (2026-06-14): ITEM_PHANTOM_LANTERN PRE-EXISTED at 881 ("Phantom Lantern",
  Defog/Flash-in-caves replacement, batch-created in the Haven key-item setup) — REUSED,
  NOT redefined. 2 new key items 899-900 (both importance 1, CannotUse, placeholder icons):
  ITEM_SEA_CHART 899 ("Sea Chart", icon TownMap — found in MorthasGrove; FLAG_SEA_CHART_FOUND
  activates Aetheron in the boat menu), ITEM_SEAL_SHARD_ASHENVEIL 900 ("Seal Shard", icon
  RedOrb — THE Decidueye Mega Evolution trigger; Mega wiring is future systems work).
- Aetheron (2026-06-14): ITEM_STORM_COMPASS PRE-EXISTED at 880 ("Storm Compass", icon
  TownMap, Fly replacement / island-select fast travel, batch-created in the Haven
  key-item setup; data block already present, fieldUseFunc = ItemUseOutOfBattle_CannotUse
  stub — the island-select field effect is future systems work) — REUSED, NOT redefined.
  2 new key items 901-902 (both importance 1, CannotUse, placeholder icons):
  ITEM_SEAL_SHARD_AETHERON 901 ("Seal Shard", icon RedOrb — Stormveil Electric/Flying Mega
  trigger stub), ITEM_CASS_DOCUMENTS 902 ("Cass Documents", icon Powder/EnergyPowder —
  lore/examine key item, the Ashenveil evacuation records Cass carried since Gildhaven).
- Convergence (FINAL ISLAND, 2026-06-14): 1 new key item ITEM_WARDENS_RESEARCH 903
  ("Warden Research", importance 1, CannotUse, POCKET_KEY_ITEMS, icon Powder/EnergyPowder
  — Sollis gives it at the gathering; lore/examine, the Warden's complete research /
  the third option fully documented). NO Seal Shard (no Mega on Convergence).
  **Next free item ID: 904.** Adding items is cheap — no hard ceiling nearby.

### Build cost of this refactor
EWRAM 86.43% -> 86.45% (+52 bytes), ROM 78.91% -> 79.00%. Compiles exit 0.

---

## Tennyson destination-select system (boat travel, 2026-06-12)

The single way the player sails between islands. Replaces the old per-harbor
direct-warp Tennyson scripts. Zero RAM cost (pure script + multichoice tables).

### Where it lives
- **Shared script:** `data/scripts/pelagios_boat.inc` (included from
  data/event_scripts.s right after pelagios_speaker_names.inc). Holds the entry
  point `Pelagios_EventScript_BoardTennyson`, the per-tier menu branches, the
  per-island sail handlers (warps), and all shared narration text.
- **Multichoice lists:** `src/data/script_menu.h` — `MultichoiceList_BoatSloop`
  / `_BoatBrigantine` / `_BoatGalleon`, registered in `sMultichoiceLists[]`.
- **Menu constants:** `include/constants/script_menu.h` — `MULTI_BOAT_SLOOP`,
  `MULTI_BOAT_BRIGANTINE`, `MULTI_BOAT_GALLEON` (appended to the MULTI_ enum).

### Multichoice strategy: one fixed list per boat tier
Rather than per-harbor lists or dynamic multichoice, there are **3 static lists,
one per VAR_BOAT_TIER** (0x4100). Island order is identical across tiers so each
higher tier just APPENDS destinations; the last entry is always CANCEL.
- Tier 0/1 (Sloop) -> `MULTI_BOAT_SLOOP`: Haven Isle, Ironhold (tier 0 routes
  here too as a safety net; it should never reach a shared menu).
- Tier 2 (Brigantine) -> `MULTI_BOAT_BRIGANTINE`: + Sirocco Isle, Emberveil.
- Tier 3 (Galleon) -> `MULTI_BOAT_GALLEON`: + Schism (North), Schism (South),
  Thalvern, Gildhaven. NOTE Schism is the Split island with TWO ports, so it occupies
  TWO menu entries ("SCHISM (NORTH)" -> Frostmark, "SCHISM (SOUTH)" -> Venomquay).
  Galleon dispatch indices: 0 Haven, 1 Ironhold, 2 Sirocco, 3 Emberveil,
  4 Schism North, 5 Schism South, 6 Thalvern, 7 Gildhaven, 8 Cancel.
`Pelagios_EventScript_BoardTennyson` switches on VAR_BOAT_TIER to pick the list,
then switches on VAR_RESULT (the chosen index) to a sail handler. B-press returns
MULTI_B_PRESSED (127), which falls through to the CANCEL handler.

### Same-island handling: "You're already moored here."
Each harbor's *_EventScript_Tennyson sets **VAR_TEMP_1** to its island id
(PELAGIOS_ISLAND_* in pelagios_boat.inc) before `goto`-ing the shared entry.
The sail handler compares VAR_TEMP_1 to its own island; if equal it shows
"You're already moored here." instead of warping. (Per-harbor menu omission was
rejected — it would mean 4x3 list variants. A single marker var is cleaner.)
VAR_TEMP_1 is safe: it is set fresh at boarding, read once, and clears on warp.
Note Emberveil's CalderaRuins also uses VAR_TEMP_1 as a panel counter, but that
is a different map/session — no overlap with the boarding interaction.

### Stub islands (Galleon tier)
Schism (North), Schism (South), Thalvern, Gildhaven are SELECTABLE so the menu
grows naturally, but have no port yet. They route to
`Pelagios_EventScript_SailNoChart` -> "The charts don't show a path there yet."
- Schism has DEDICATED stub handlers `Pelagios_EventScript_SailToSchismNorth` /
  `_SailToSchismSouth` (each currently `goto SailNoChart`) and TWO island ids:
  `PELAGIOS_ISLAND_SCHISM` (4, Frostmark/north) and `PELAGIOS_ISLAND_SCHISM_SOUTH`
  (7, Venomquay/south). Map-builder swap-in instructions (warp + same-island guard)
  are commented inline in pelagios_boat.inc above the two stub handlers — wire each
  port's *_EventScript_Tennyson to set VAR_TEMP_1 to the matching id.

### Arrival tiles (walkable, verified against layout collision bits)
- Haven Isle Harbor: **(13,9)** — sand in front of the berth.
- Ironhold GatemarkPort: **(10,14)** — on the pier; walking north crosses the
  arrival coord triggers at row 11.
- Sirocco DustmouthPort: **(10,14)** — identical pier geometry to Ironhold; walking
  north crosses the VAR_SIROCCO_PROGRESS==0 arrival triggers at (9-10,11).
- Emberveil AshmouthPort: **(10,14)** — arrival is an ON_FRAME table
  (VAR_EMBERVEIL_PROGRESS==0), fires anywhere on the map, so any walkable tile
  works; (10,14) chosen for consistency.

### Adding a future island to the menu
1. Give it a `PELAGIOS_ISLAND_*` id in pelagios_boat.inc.
2. Replace its `Pelagios_EventScript_SailNoChart` dispatch case with a real
   `Pelagios_EventScript_SailTo<Island>` (warp to that port's arrival tile + the
   same-island guard). Keep the dispatch index in sync with its position in the
   matching `MultichoiceList_Boat*` in src/data/script_menu.h.
3. In that island's harbor scripts.inc, write *_EventScript_Tennyson that does
   `lockall` / `setvar VAR_TEMP_1, PELAGIOS_ISLAND_<this>` / optional intro msgbox
   / `goto Pelagios_EventScript_BoardTennyson`, and point its boarding bg_events
   at it. (Boarding scripts ALWAYS hang off sign bg_events, never the SS_TIDAL
   object — see Known Issues harbor rules.)

### bg_events untouched
Only the script BODIES behind the existing *_EventScript_Tennyson labels were
rewritten. The sign-type boarding bg_events (and the SS_TIDAL decoration objects)
were not moved, so the generators (build_mapjson.py / build_ironhold_mapjson.py /
build_sirocco_mapjson.py / the Emberveil generator) stay in sync — they still
point at the same label names.

---

## Storm Compass field-use wiring (2026-06-14, gmake exit 0)

ITEM_STORM_COMPASS (880, Aetheron, replaces Fly) "Use" from the bag now opens the
island-select destination menu from ANY map. Pure C-side field-use registration; the
menu SCRIPT (Pelagios_EventScript_StormCompass in data/scripts/pelagios_boat.inc) was
already written and correct.

### What was changed (3 files)
- **src/item_use.c:** added `void ItemUseOutOfBattle_StormCompass(u8 taskId)` +
  `static void Task_UseStormCompass(u8 taskId)`, mirroring **ItemUseOutOfBattle_PokemonBoxLink**
  exactly:
  ```c
  void ItemUseOutOfBattle_StormCompass(u8 taskId) {
      sItemUseOnFieldCB = Task_UseStormCompass;
      SetUpItemUseOnFieldCallback(taskId);   // closes bag, returns to field
  }
  static void Task_UseStormCompass(u8 taskId) {
      ScriptContext_SetupScript(Pelagios_EventScript_StormCompass);
      DestroyTask(taskId);
  }
  ```
  Plus the forward decl `static void Task_UseStormCompass(u8);` and the extern script
  symbol `extern const u8 Pelagios_EventScript_StormCompass[];` (it lives in
  data/scripts/pelagios_boat.inc, NOT event_scripts.h, so it's declared locally in item_use.c).
- **include/item_use.h:** added `void ItemUseOutOfBattle_StormCompass(u8 taskId);`.
- **src/data/items.h:** ITEM_STORM_COMPASS `.fieldUseFunc` changed
  `ItemUseOutOfBattle_CannotUse` -> `ItemUseOutOfBattle_StormCompass`. Other fields
  left as-is (correct for a field key item: importance 1, POCKET_KEY_ITEMS,
  type ITEM_USE_BAG_MENU).

### Why this model + map-independence
`SetUpItemUseOnFieldCallback` only fades the bag out and returns control to the field,
then runs the callback — it imposes **NO** map-type / surf-state / harbor / field-condition
gate (unlike Surf/Fly, which check CanUseSurf/etc.). So the compass opens the menu
unconditionally from any outdoor, cave, or surf tile. The script's same-island guard is
disarmed from the field via VAR_TEMP_1 = 255 sentinel (no PELAGIOS_ISLAND_* uses 255), so
every destination is selectable. Possession is the gate (player only has the item after
Aetheron) — NO FLAG_STORM_COMPASS_OBTAINED check added. Convergence remains gated inside
the script by FLAG_AETHERON_RESOLVED (boat menu untouched).

### Link / verification
Build exit 0, zero new warnings (only the benign RWX LOAD-segment note). ELF symbols
resolved: ItemUseOutOfBattle_StormCompass (T), Task_UseStormCompass (t),
Pelagios_EventScript_StormCompass (D). EWRAM 86.46% / IWRAM 86.63% / ROM 80.14%
(26,890,068 B) — RAM unchanged, ROM +~0.5 KB (the small function).

### In-emulator check still needed (could NOT run mGBA)
With the Storm Compass in the bag, open BAG -> KEY ITEMS -> Storm Compass -> USE on:
(1) an outdoor map, (2) a cave/interior map, (3) while SURFING. In all three the
island-select menu must open; pick a visited island and confirm the warp fires (and that
Convergence is offered only after FLAG_AETHERON_RESOLVED). Also confirm CANCEL / B-press
returns to the field cleanly (no soft-lock).

---

## Game Systems

### Following Pokémon (overworld follower) — ENABLED globally 2026-06-14
- **Enabled via expansion config**, single define flipped in `include/config/overworld.h`:
  `OW_FOLLOWERS_ENABLED` **FALSE -> TRUE**. Its prerequisite `OW_POKEMON_OBJECT_EVENTS`
  was already TRUE (no change). No other follower defines touched (BOBBING/POKEBALLS/
  SCRIPT_MOVEMENT keep their shipped TRUE defaults; B_FLAG_FOLLOWERS_DISABLED stays 0 =
  always on). There is NO `OW_FOLLOWERS_POKEMON` define in this expansion — the single
  `OW_FOLLOWERS_ENABLED` toggle is the documented enable.
- **PER-MAP MECHANISM = GLOBAL, NO PER-MAP JSON FIELD.** Verified: `struct MapHeader`
  (include/global.fieldmap.h) has no follower bit; map.json only has allow_cycling/
  allow_escaping/allow_running siblings (no allow_followers); tools/mapjson/mapjson.cpp
  has no follower field. Follower spawning is gated entirely at runtime in
  `UpdateFollowingPokemon()` (src/event_object_movement.c ~2354) by the global config +
  the B_FLAG_FOLLOWERS_DISABLED / FLAG_TEMP_HIDE_FOLLOWER flags — NOT by map data.
  **Consequence: enabling the config is sufficient for ALL maps; ZERO per-map edits and
  ZERO generator (tools/pelagios/build_*_mapjson.py) edits were needed or made.**
- **Indoor / large-sprite safety (build-time):** no hard assert. Indoors,
  UpdateFollowingPokemon() simply removes the follower if its gfx is larger than 32x32
  (mapType == MAP_TYPE_INDOOR && oam size > ST_OAM_SIZE_2). Water/surf and warp behavior
  is engine-handled at runtime.
- **RAM cost = effectively ZERO** (the follower object-event slot is statically allocated
  in gObjectEvents[] regardless of the toggle). Measured by relinking with the feature
  OFF then ON (both runs reached the linker memory-region report):
  EWRAM 86.46% -> 86.46% (+0 B), IWRAM 86.63% -> 86.63% (+0 B), ROM 79.95% -> 79.95%
  (+544 B). Far below the 95% gate — runtime-budget HARD RULE satisfied.
- **UNTESTED in mGBA:** in-emulator follower behavior on water/surf, through warps, and on
  indoor maps is NOT verified (build-time only — no emulator run was done). The expansion's
  follower note ("additional scripting may be required") still applies; Pelagios scripts
  were NOT modified for follower support.
- **BUILD-BLOCKER (pre-existing, NOT caused by this change — owner: script-writer):**
  flipping the config forced a full `data/event_scripts.s` rebuild, which surfaced a latent
  bug in the UNTRACKED `data/maps/Primalis_VerdantLanding/scripts.inc`: `playse SE_M_GROWL`
  references a sound symbol that does NOT exist in include/constants/songs.h (link fails:
  "undefined reference to SE_M_GROWL"). PROVEN pre-existing: reverting the follower toggle
  and force-rebuilding event_scripts reproduces the identical error. Fix is a one-line
  script edit (e.g. SE_M_GROWL -> SE_M_ROAR / SE_M_UPROAR) — OUT OF SYSTEMS-ENGINEER SCOPE
  (no .inc/script edits allowed by this task). The follower config change itself links
  cleanly; the tree cannot reach `gmake exit 0` until this Primalis script symbol is fixed.

### Badge System: Narrative-only (DECIDED 2026-06-12)
Gyms set FLAG_[ISLAND]_GYM[N]_CLEAR for progression gating. Badge fanfare plays for
feedback. No FLAG_BADGE*_GET beyond the vanilla 8. Future: custom Island Journal UI
replaces Trainer Card post-launch.

The engine provides exactly 8 badge flags (FLAG_BADGE01_GET..08, NUM_BADGES == 8),
now fully consumed. Pelagios does NOT expand the badge-flag system. Policy comment
block lives in include/constants/flags.h after the badge defs. All gym GATING keys
off per-island GYM*_CLEAR flags or VAR_[ISLAND]_PROGRESS — never off FLAG_BADGE*_GET —
so the 8-flag cap does not affect gating.

Concrete badge-flag usage map (audited 2026-06-12, zero policy violations found):
| Island | Gym leaders | Engine badge flag |
|---|---|---|
| Haven | (none — no gyms) | — (BADGE01 reserved, unwired) |
| Ironhold | Petra / Forge / Rook / Sever | BADGE02 / 03 / 04 / 05 |
| Sirocco | Silt / Crag / Miria | BADGE06 / 07 / 08 (last engine slot) |
| Sirocco | Dagan | narrative-only (GYM4_CLEAR) |
| Emberveil | Cinder / Slag / Vex / Solace | narrative-only (GYM1..4_CLEAR) |
| Schism+ | (future) | narrative-only by this policy |

Files: 7 `setflag FLAG_BADGE*_GET` total — Ironhold (IronholdCity 02, Armory_Interior 03,
SummitFortress_Interior1 04, Interior2 05), Sirocco (MiradenOasis 06+07,
BuriedCity_Interior1 08). Everything else plays fanfare + "received the X BADGE" text
with NO badge flag set.

---

## Naming Conventions

### Map Names
Format: `IslandName_LocationName`
Examples:
- `HavenIsle_TidemarkVillage`
- `HavenIsle_SollisLab`
- `Ironhold_GatemarkPort`
- `Ironhold_IronholdCity`

### Map Groups
Format: `MAP_GROUP_ISLANDNAME`
Examples:
- `MAP_GROUP_HAVEN_ISLE`
- `MAP_GROUP_IRONHOLD`
- `MAP_GROUP_SIROCCO`

### Flags
Format: `FLAG_ISLANDNAME_DESCRIPTION` or `FLAG_STORY_DESCRIPTION`
Examples:
- `FLAG_HAVEN_ISLE_COMPLETE`
- `FLAG_STARTER_CHOSEN`
- `FLAG_CASS_DEFECTED`

### Variables
Format: `VAR_DESCRIPTION`
Examples:
- `VAR_CASS_RELATIONSHIP`
- `VAR_BOAT_TIER`

### Trainers
Format: `TRAINER_CLASS_ISLANDNAME_NUMBER`
Examples:
- `TRAINER_YOUNGSTER_HAVEN_1`
- `TRAINER_FISHERMAN_HAVEN_1`

### Items (custom)
Format: `ITEM_DESCRIPTIVE_NAME`
Examples:
- `ITEM_WARDENS_JOURNAL`
- `ITEM_NAVIGATORS_LOG`
- `ITEM_SEAL_SHARD_IRONHOLD`

---

## Dialogue Style Guidelines
- Tone: Pokémon game warmth with more emotional depth — not childish, not grimdark
- NPCs should feel like real people with opinions, not just information dispensers
- Subtle worldbuilding — the Covenant's presence should feel slightly off even early
- The Warden's death is recent — NPCs who knew them should reflect that quietly
- Haven Isle should feel warm and safe before everything goes wrong
- Avoid exposition dumps — spread lore across multiple NPCs and interactions

### Speaker nameboxes (MANDATORY for all new island scripts — done for Haven Isle + Ironhold 2026-06-11)

The expansion's namebox feature (include/config/name_box.h, src/field_name_box.c)
shows a small name window above the dialogue frame. It is enabled by default
(no config change needed; OW_FLAG_SUPPRESS_NAME_BOX 0 means never suppressed).
Drive it from scripts with the `setspeaker` macro (asm/macros/event.inc):

```asm
	setspeaker Pelagios_Speaker_Sollis   @ next message shows a SOLLIS namebox
	msgbox SomeText, MSGBOX_DEFAULT
	setspeaker 0                         @ clears it (narration mid-conversation)
	msgbox SomeNarration, MSGBOX_DEFAULT
```

Rules:

- **Name strings** live in `data/scripts/pelagios_speaker_names.inc`
  (included from data/event_scripts.s). Add new characters there as
  `Pelagios_Speaker_Name:: .string "NAME$"`. Names are ALL CAPS and must fit
  8 tiles / 64 px in FONT_SMALL (about 10-11 characters).
- Do NOT use the C-side `SP_NAME_*` enum in scripts — enums are invisible to
  the assembler. Pass a `Pelagios_Speaker_*` label, or literal `0` to clear.
- The buffered speaker **persists across consecutive msgboxes** while the
  message box stays open. `closemessage`, script end, battles, warps, and
  menus all clear it (HideFieldMessageBox / InitStandardTextBoxWindows).
  So: `setspeaker` before every speaker CHANGE, `setspeaker 0` before
  narration shown in the same open box; nothing needed after closemessage.
- **What gets a namebox:** lines that are one character's direct speech —
  story characters, gym leaders, named trainers' post-battle text, and
  unnamed NPCs under a role name (GUARD, SAILOR, ELDER, INNKEEPER…).
- **What does NOT:** narration/system text (signs, item finds, "The ground
  shudders!", the "…Your POKéNAV buzzes." line), yes/no prompts phrased as
  narration ("Tell him you understand?"), and prose that quotes a speaker in
  third person — the Resistance Hideout's quoted-within-narration style is
  its own attribution, so it deliberately has no nameboxes.
- **No double attribution:** a message with a namebox must NOT also start
  with an inline `NAME:` prefix. Texts that mixed narration and speech were
  split into separate msgboxes — do the same in new scripts.
- **Trainer battles:** `setspeaker` before a *talk-initiated* `trainerbattle`
  nameboxes the intro speech (it goes through ShowFieldMessage). Sight
  trainers MUST keep `trainerbattle` as the script's first command, so their
  intro gets no namebox — put `setspeaker` between `trainerbattle_single`
  and the post-battle msgbox instead. In-battle defeat text never shows a
  namebox (battle UI). Start gym victory scripts with the leader's
  `setspeaker` (or none if they open with narration) — never rely on
  pre-battle state; the field reload after battle resets the speaker.
- **giveitem / badge-fanfare messages are narration:** `setspeaker 0`
  before them, re-set the speaker after.
- **Scripted PokéNav calls:** buzz line as plain narration msgbox, then
  `setspeaker` the caller for the spoken part (namebox renders normally;
  real match calls even get their own PokéNav-styled namebox gfx).

---

## Current Build Status

### ✅ Completed
- Base pokeemerald-expansion fork compiles successfully
- pokeemerald.gba builds and boots in mGBA
- devkitARM toolchain installed and configured
- Porymap installed and pointed at repo
- All Pelagios flags (flags.h 0x4A7–0x4D8), vars (vars.h 0x40F7–0x40FE), key items
  (items.h 874–883 + src/data/items.h), MAPSEC entries (region_map_sections.json)
- Berry tree IDs 90–91 (include/constants/berry.h)
- gMapGroup_HavenIsle registered in map_groups.json (16 maps). NOTE: stub groups
  for future islands deliberately skipped — empty map groups can't be expressed
  in the generated groups.inc; add each island's group with its first real map.
- 8 new layouts composed (data/layouts/HavenIsle_*/map.bin + border.bin) via
  tools/pelagios/build_layouts.py; entries appended to layouts.json.
  Interiors reuse vanilla layouts (Brendan's house 1F/2F, Birch lab, HOUSE1/2,
  POKEMON_CENTER_1F/2F, MART; ruins = AlteringCave/StevensRoom copies + ladder)
- All 16 map.json files generated via tools/pelagios/build_mapjson.py
  (warps, connections, NPCs, triggers — regenerate via the script, don't hand-edit)
- All 16 scripts.inc written: opening sequence (mom 2F call → mom 1F dialogue →
  Cass intercept trigger → lab ON_FRAME cutscene), full starter selection
  (3 loose starters, yes/no + nickname, Cass takes counter-type, Sollis gives
  WARDENS_JOURNAL), TENNYSON_KEY via house PC, village exit blockers until
  starter chosen, ruins disturbance (camera shake + PokéNav message), Ralts
  flee scene, ruins inscription (sets FLAG_CIPHER_1_FOUND), Warden explanation +
  Sloop unlock (gives NAVIGATORS_LOG), all NPC dialogue, mart, nurse,
  trainer scripts for Jay & Old Bren

### 🔄 In Progress — Haven Isle (ALL WIRING DONE, BUILDS CLEAN as of 2026-06-11)
All five wiring steps are complete and the full build passes with exit 0:
1. [x] 16 `.include "data/maps/HavenIsle_*/scripts.inc"` lines in data/event_scripts.s
2. [x] Heal locations HEAL_LOCATION_TIDEMARK_VILLAGE +
   HEAL_LOCATION_HAVEN_ISLE_PLAYER_HOUSE_2F in src/data/heal_locations.json
3. [x] Trainers TRAINER_YOUNGSTER_HAVEN_1 (855) + TRAINER_FISHERMAN_HAVEN_1 (856)
   in include/constants/opponents.h (TRAINERS_COUNT_EMERALD now 857) +
   src/data/trainers.party (Jay: Rattata L5/Pidgey L5; Old Bren: Magikarp L4/Marill L6)
4. [x] New game spawn: src/new_game.c WarpToTruck now warps to
   MAP_HAVEN_ISLE_PLAYER_HOUSE_2F (x4, y3). New-game init
   (EventScript_ResetAllMapFlags in data/scripts/new_game.inc) sets
   FLAG_HIDE_RUINS_RALTS, FLAG_SYS_B_DASH, setrespawn
   HEAL_LOCATION_HAVEN_ISLE_PLAYER_HOUSE_2F, and plants the two Route 1 Oran trees.
5. [x] Wild encounters for MAP_HAVEN_ISLE_ROUTE1 / ROUTE2 in
   src/data/wild_encounters.json (Route 1: grass + surf; Route 2: grass + surf +
   fishing incl. Old/Good/Super Rod with rare Totodile).
Only in-emulator verification remains (see Compilation checklist).

### Opening flow state machine (reference for future scripting)
- VAR_PELAGIOS_INTRO_STATE: 0=new game, 1=mom called (2F), 2=mom talked,
  3=Cass intercept done, 4=lab cutscene done/choosing, 5=starter+journal done
- VAR_HAVEN_RUINS_STATE: 0=village exits blocked (no starter), 1=armed
  (disturbance plays on entering Interior1), 2=mid-sequence (transient,
  Ralts scene chains on the next frame), 3=ruins scene done
- VAR_PELAGIOS_STARTER: 1=Totodile, 2=Chimchar, 3=Rowlet

### 🔄 In Progress — Ironhold (CONSTANTS DONE, BUILDS CLEAN as of 2026-06-11)
Systems/constants layer complete (gmake exit 0). Maps and scripts still pending.
- Flags (include/constants/flags.h, 0x4D9-0x4E2): FLAG_IRONHOLD_ARRIVED 0x4D9,
  FLAG_IRONHOLD_GYM1_CLEAR 0x4DA, _GYM2_CLEAR 0x4DB, _GYM3_CLEAR 0x4DC,
  _GYM4_CLEAR 0x4DD, FLAG_IRONHOLD_RESISTANCE_MET 0x4DE, _SEAL_FOUND 0x4DF,
  FLAG_IRONHOLD_CIPHER_FOUND 0x4E0, FLAG_GRAPPLE_HOOK_OBTAINED 0x4E1,
  FLAG_DOCUMENT_FRAGMENT_OBTAINED 0x4E2, FLAG_HIDE_IRONHOLD_SEVER_GATE 0x4E3
  (gate-Sever cameo hide flag, added 2026-06-11). (FLAG_IRONHOLD_RESOLVED reused
  existing 0x4AC.) Next free story flag: 0x4E4.
- Var (include/constants/vars.h): VAR_IRONHOLD_PROGRESS 0x40FF (0=arrived..5=resolved).
  VAR_SEVER_RELATIONSHIP confirmed pre-existing (0x40FA). NOTE: 0x40FF was the LAST
  free general var slot — VARS_END is 0x40FF. No more vars without a saveblock-impacting
  VARS_END expansion.
- Items: ITEM_DOCUMENT_FRAGMENT = 884 (new, in items.h + src/data/items.h, icon
  gItemIcon_Powder / gItemIconPalette_EnergyPowder placeholder). ITEM_GRAPPLE_HOOK
  (876) already existed from Haven Isle setup. ITEM_SEAL_SHARD_IRONHOLD added as a
  #define alias to existing ITEM_SEAL_SHARD_1 (882) — no new ID consumed.
- Trainers (include/constants/opponents.h, 857-863; src/data/trainers.party):
  TRAINER_OFFICER_IRONHOLD_1 857 (Mace: Growlithe L12/Machoke L14),
  TRAINER_WORKER_IRONHOLD_1 858 (Finn: Aron L11 x2/Lairon L13),
  TRAINER_HIKER_IRONHOLD_1 859 (Crag: Graveler L16/Onix L17/Steelix L18),
  TRAINER_COVENANT_IRONHOLD_1 860 (Venn: Riolu/Meditite L15/Lucario L17),
  TRAINER_COVENANT_IRONHOLD_2 861 (Hale: Bronzor L16/Aron/Pawniard L17),
  TRAINER_COVENANT_IRONHOLD_3 862 (Sorn: Bisharp/Lucario L22/Scizor L24),
  TRAINER_COVENANT_IRONHOLD_4 863 (Vael: Metagross/Aegislash L26/Bisharp L28).
  Classes are placeholders (Gentleman/Hiker/Cooltrainer M) - vanilla has no
  Officer/Worker/Covenant class. TRAINERS_COUNT_EMERALD bumped 857 -> 864.
- ⚠️ TRAINER FLAG SPACE NOW FULLY CONSUMED. TRAINERS_COUNT_EMERALD (864) ==
  MAX_TRAINERS_COUNT_EMERALD (864). Trainer defeat flags occupy 0x500-0x85F,
  immediately followed by SYSTEM_FLAGS at 0x860. ZERO trainer slots remain.
  The 4 Ironhold GYM LEADERS (Petra, Forge, Rook, Sever) have NO trainer IDs and
  CANNOT be added without expanding MAX_TRAINERS_COUNT_EMERALD, which collides with
  SYSTEM_FLAGS and requires a flag-space refactor. THIS IS BLOCKING for gym battles.
  Decision deferred to user — do NOT improvise the refactor.
- SKIPPED: Ironhold map group registration (gMapGroup_Ironhold). Empty map groups
  can't be expressed in generated groups.inc (same as Haven Isle). The map-builder
  agent must register MAP_GROUP_IRONHOLD together with its first real map.
- UPDATE 2026-06-11: gym-leader flag-space blocker RESOLVED. TRAINER_LEADER_IRONHOLD_PETRA
  (864), _FORGE (865), _ROOK (866), _SEVER (867) now exist in opponents.h + trainers.party.
  Map-builder wired their NPC scripts (TODO placeholders) accordingly.

### ✅ Sirocco Isle — CONSTANTS (systems-engineer, 2026-06-12, gmake exit 0)
Systems/constants layer complete. (Maps and scripts now ALSO complete — see the
"Sirocco MAPS" and "Sirocco SCRIPTS" sections below.)
- Flags (include/constants/flags.h, 0x4E4-0x4EE, 11 story flags): FLAG_SIROCCO_ARRIVED
  0x4E4, GYM1_CLEAR 0x4E5, GYM2_CLEAR 0x4E6, GYM3_CLEAR 0x4E7, GYM4_CLEAR 0x4E8,
  DAGAN_ESCAPED 0x4E9, DEX_MET 0x4EA, BURIED_CITY_FOUND 0x4EB, SEAL_FOUND 0x4EC,
  CIPHER_FOUND 0x4ED, NORTH_TERRITORY_OPEN 0x4EE (extra gating flag I added for the
  GiltClawTerritory block; the brief's list has no name for it). FLAG_SIROCCO_RESOLVED
  reused existing 0x4AD. Hidden item: FLAG_HIDDEN_ITEM_SIROCCO_BERRY = 0x267 (Rawst
  Berry, Route 1). ⚠️ Story block 0x4A7-0x4EF is now nearly full — only 0x4EF free.
  Emberveil and all later islands need a new "Pelagios story block 2" past 0x4F0.
- Vars: NONE added. VAR_SIROCCO_PROGRESS (0x4101) and VAR_DAGAN_RELATIONSHIP (0x40FB)
  both already existed from the capacity refactor / initial setup. States documented
  in SIROCCO_BRIEF.md (0=not arrived .. 7=resolved). Reserved spares 0x410A-0x410F
  untouched.
- Items: ITEM_SEAL_SHARD_SIROCCO = 885 (new, items.h + src/data/items.h, name "Seal
  Shard", icon gItemIcon_BlueOrb / gItemIconPalette_BlueOrb placeholder, key item,
  importance 1, CannotUse — stub only, not awarded yet). No field-traversal items on
  Sirocco (ITEM_GRAPPLE_HOOK from Ironhold reused for rubble if needed).
- Trainers (include/constants/opponents.h, 873-887; src/data/trainers.party). Parties
  verbatim from SIROCCO_BRIEF.md. Generic: TRAINER_GILTCLAW_SIROCCO_1 873 (Rael),
  TRAINER_TRAVELER_SIROCCO_1 874 (Yoss), GILTCLAW_2 875 (Wex), _3 876 (Pell),
  _4 877 (Sera), _5 878 (Voss), _6 879 (Mave), _7 880 (Thorn), _8 881 (Fen),
  _9 882 (Rusk), _10 883 (Lace). Gym leaders: TRAINER_LEADER_SIROCCO_SILT 884
  (Ground, Hiker pic), _CRAG 885 (Rock, Hiker pic), _MIRIA 886 (Ground/Rock,
  Cooltrainer F pic), _DAGAN 887 (Ground/Rock boss, Rich Boy pic per brief).
  Pre-evolution substitutions ("Garchomp (Gabite)" etc.) use the parenthetical species
  at the listed level. TRAINERS_COUNT_EMERALD bumped 873 -> 888. 136 slots free.
- SKIPPED: Sirocco map group registration (gMapGroup_Sirocco). Empty map groups can't
  be expressed in generated groups.inc (established pattern — same as Haven/Ironhold).
  The map-builder agent must register MAP_GROUP_SIROCCO with its first real map.

### ✅ Completed — Sirocco MAPS (map-builder, 2026-06-12, gmake exit 0)
All 18 Sirocco maps built, registered, and compiling cleanly (full build exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.19%). Built via
tools/pelagios/build_sirocco_layouts.py (8 composed outdoor layouts) +
build_sirocco_mapjson.py (all 18 map.json, with a post-pass that recomputes every
Sirocco<->Sirocco connection offset from the actual layout openings) +
build_sirocco_scripts_stubs.py (placeholder scripts.inc). Regenerate via those
scripts; do NOT hand-edit map.bin/map.json/scripts.inc.

NEW composed outdoor layouts (General/Mauville desert pair, metatiles sampled
verbatim from LAYOUT_ROUTE111: sand 0x3121, deep-sand wild-encounter tile 0x3251,
rock walls 0x0471/0x0473, water 0x1170, shore 0x1179, oasis grass 0x3001):
- LAYOUT_SIROCCO_DUSTMOUTH_PORT 20x18 - run-down port, south sea + central dock for
  the Tennyson (Brigantine), north gate to DesertRoute1.
- LAYOUT_SIROCCO_DESERT_ROUTE_1 22x40 - long route, dried-riverbed rock scar down the
  middle (with a crossing gap), deep-sand encounter patches, north->Oasis south->Port
  east->DexCamp. Hidden Rawst Berry bush marker at (5,36)/item (5,35).
- LAYOUT_SIROCCO_MIRADEN_OASIS 28x24 - town around a shrunken oasis (grass ring + small
  water core), gym/center/shop signs, north exit to GiltClawTerritory.
- LAYOUT_SIROCCO_DEX_CAMP 16x14 - small camp, west exit only -> DesertRoute1.
- LAYOUT_SIROCCO_DESERT_ROUTE_2 24x18 - shorter/harder route, sand columns, west->Oasis
  east->BuriedCity_Exterior.
- LAYOUT_SIROCCO_BURIED_CITY_EXTERIOR 22x20 - excavation site, exposed stone ring with a
  doorway gap (warp down to Interior1), west->Route2, deep-sand encounter patches.
- LAYOUT_SIROCCO_GILT_CLAW_TERRITORY 20x22 - branded-post corridor, south->Oasis
  north->DaganPalace_Exterior.
- LAYOUT_SIROCCO_DAGAN_PALACE_EXTERIOR 18x16 - ostentatious gate with two fountains
  (water tiles), south->GiltClawTerritory, north door -> Interior1.

REUSED vanilla layouts (interiors, zero new binary):
- DustmouthPort_Inn = POKEMON_CENTER_1F (lobby/heal); _Inn_Interior = POKEMON_CENTER_2F
  (rooms, with a stairs warp down to the black market).
- DustmouthPort_BlackMarket = SEALED_CHAMBER_OUTER_ROOM (hidden basement).
- MiradenOasis_PokemonCenter = POKEMON_CENTER_1F; _Shop = MART.
- BuriedCity_Interior1 = VICTORY_ROAD_1F (Gym 3, Miria); _Interior2 =
  SEALED_CHAMBER_OUTER_ROOM (mural/cipher room); _SealChamber = SEALED_CHAMBER_INNER_ROOM
  (Xerath + Gilt Claw apparatus).
- DaganPalace_Interior1 = MOSSDEEP_CITY_SPACE_CENTER_1F; _Interior2 =
  MOSSDEEP_CITY_SPACE_CENTER_2F (Gym 4, Dagan + post-battle Miria).

Battle scenes: gym battle maps (BuriedCity_Interior1, DaganPalace_Interior2) =
MAP_BATTLE_SCENE_GYM; all others MAP_BATTLE_SCENE_NORMAL (no _CAVE/_BUILDING scene
constants exist). Weather: outdoor routes WEATHER_SANDSTORM, MiradenOasis WEATHER_SUNNY,
all interiors/BuriedCity/SealChamber WEATHER_NONE (per brief).

Connections (offsets recomputed from layout openings, all reciprocal):
  DustmouthPort.up<->DesertRoute1.down ; DesertRoute1.up<->MiradenOasis.down ;
  DesertRoute1.right<->DexCamp.left (east branch) ;
  MiradenOasis.right<->DesertRoute2.left ; DesertRoute2.right<->BuriedCityExterior.left ;
  MiradenOasis.up<->GiltClawTerritory.down ;
  GiltClawTerritory.up<->DaganPalaceExterior.down.
  BuriedCity Interior1/2/SealChamber and DaganPalace Interior1/2 link by WARPS, not
  connections (the brief's deeper gating is handled narratively + via VAR triggers).

Wild encounters (src/data/wild_encounters.json, 12-slot land tables, rate 20):
  gSiroccoDesertRoute1 (Sandshrew/Sandile 40 / Trapinch 30 / Cacnea 20 / Hippopotas ~9 /
  Sigilyph 1), gSiroccoDesertRoute2 (Sandile40/Vibrava30/Skorupi20/Silicobra10),
  gSiroccoBuriedCityExterior (Sandile50/Baltoy30/Vibrava20),
  gSiroccoBuriedCityInterior1 (Baltoy50/Claydol30/Golurk20),
  gSiroccoGiltClawTerritory (Sandaconda50/Krokorok30/Gabite20). No water/fishing tables
  (brief defines none). DEVIATION: the brief's "Sigilyph 5% daytime mirage replacing a
  Cacnea slot" became a single flat 1% rarest slot (slot 11) - these tables have no
  time-of-day variants. Pre-evolution substitutions ("Flygon (Vibrava)" etc.) use the
  parenthetical species so the levels match.

Heal location: HEAL_LOCATION_SIROCCO_DUSTMOUTH_PORT (src/data/heal_locations.json),
respawn at DustmouthPort_Inn via LOCALID_SIROCCO_INNKEEPER.

MAPSECs (already added by systems-engineer in region_map_sections.json +
region_map_entries.h): MAPSEC_SIROCCO_DUSTMOUTH_PORT, _DESERT_ROUTE_1, _MIRADEN_OASIS,
_DESERT_ROUTE_2, _BURIED_CITY, _DAGAN_PALACE (5 MAPSECs cover all 18 maps).

Hidden item: Rawst Berry as a bg_event type "hidden_item" at DesertRoute1 (5,35),
item ITEM_RAWST_BERRY, flag FLAG_HIDDEN_ITEM_SIROCCO_BERRY (0x267), with a dried-bush
sign marker at (5,36).

Harbor/boat: the SS_TIDAL (Tennyson, Brigantine) at DustmouthPort is pure decoration
(script 0x0, elevation 1, offshore at (12,15)); boarding runs through 5 sign bg_events
on the pier-edge/hull tiles - NEVER a boarding script on the ship object (per harbor
rules; matches HavenIsle/Ironhold). Xerath in the SealChamber is the same pattern
(SS_TIDAL decoration, script 0x0, sealed/non-interactable in main story).

GYM LEADERS are talk-initiated overworld NPCs (TRAINER_TYPE_NONE) so their scripts can
be flag-gated (trainer-see requires trainerbattle as the first command): Silt + Crag in
MiradenOasis, Miria in BuriedCity_Interior1, Dagan in DaganPalace_Interior2. The 10
generic Gilt Claw trainers + Traveler Yoss are sight trainers (TRAINER_TYPE_NORMAL).
North-territory gate (blocked until Gym 2): MiradenOasis NorthBlocker NPC uses
FLAG_SIROCCO_NORTH_TERRITORY_OPEN as a hide flag + VAR_SIROCCO_PROGRESS coord triggers
(values 1/2) as a fallback bounce. Post-battle Miria object in DaganPalace_Interior2 is
hidden behind FLAG_SIROCCO_DAGAN_ESCAPED.

DEVIATIONS / NOTES for next agents:
- Sigilyph daytime-mirage encounter flattened to a 1% slot (see Wild encounters above).
- Gym 1/2 puzzles (sandstorm-maze, boulder-push) and Gym 3 stone-tablet puzzle are NOT
  built as map geometry - the reused vanilla interiors have no movable boulders/tablets.
  Either the script-writer simplifies them to talk-gated battles or a future map pass
  adds Strength-boulder objects. Non-blocking.
- North-territory blocker is BOTH an NPC (hide flag) and VAR coord triggers as a belt-
  and-braces gate; script-writer should make the NPC's bounce dialogue authoritative and
  can drop the coord fallback if redundant.
- Custom outdoor layouts are blocky desert rectangles (no building-sprite stamps); warp
  tiles sit on plain sand. Review in Porymap and dress with building/door metatiles later.
- Black-market access: Inn_Interior has a stairs warp (10,4) down to the basement; the
  BlackMarketDoor NPC in the Inn lobby is flavor/hint only (script-writer decides whether
  it also warps or just points upstairs). No hidden-door tile mechanic was built.

### ✅ Completed — Sirocco SCRIPTS (script-writer, 2026-06-12, gmake exit 0)
All 18 Sirocco scripts.inc fully implemented - zero TODO placeholders remain.
(About 60% was written by a prior cut-off script-writer session and verified intact:
DustmouthPort + Inn/Interior/BlackMarket, DesertRoute1/2, MiradenOasis incl. Silt+Crag,
DexCamp, BuriedCity_Exterior, PokemonCenter, Shop, and all Sirocco speaker names in
pelagios_speaker_names.inc. This session wrote the back half: BuriedCity_Interior1/2,
SealChamber, GiltClawTerritory, DaganPalace x3, plus a `setspeaker` consistency fix on
Silt/Crag pre-battle and the Emberveil link-blocker fix noted under Known Issues.)
Highlights, grouped by system:
- Arrival (DustmouthPort, coord trigger at progress 0): dock enforcer "water tax"
  routine, camera pan north to the dry hills. FLAG_SIROCCO_ARRIVED + progress 1.
- Gym gating: Crag refuses until GYM1_CLEAR; north gate to GiltClawTerritory blocked by
  NorthBlocker NPC (hide flag FLAG_SIROCCO_NORTH_TERRITORY_OPEN) + coord-trigger bounce
  armed at progress 1/2 (both kept - belt and braces); SealChamber apparatus stonewalls
  until FLAG_SIROCCO_SEAL_FOUND. All four leaders talk-initiated (TRAINER_TYPE_NONE),
  each with `setspeaker` before trainerbattle_single (Petra precedent).
- BADGES: Silt FLAG_BADGE06_GET (Dust), Crag FLAG_BADGE07_GET (Stone), Miria
  FLAG_BADGE08_GET (Gilt). ⚠️ ENGINE BADGE FLAGS NOW EXHAUSTED (no FLAG_BADGE09 exists,
  NUM_BADGES = 8). Dagan's MIRE BADGE is NARRATIVE-ONLY: fanfare + "received" text play
  but no badge flag is set (FLAG_SIROCCO_GYM4_CLEAR is the real state). All later-island
  badges must do the same unless the systems engineer expands the badge system.
- TM substitutions: Silt TM Earthquake (as briefed), Crag TM Rock Tomb (no Stone Edge TM
  exists - lampshaded), Miria TM Dragon Claw (as briefed), Dagan TM THIEF (no Crunch TM
  exists; Thief is Dark-type like Crunch and in character - lampshaded). Dagan's TM uses
  checkitemspace + call (NOT ShowBagIsFull) so a full bag can never abort the escape.
- Dex (DexCamp): pre-Gym1 nervous brush-off + camp-shut first-entry trigger; full intro
  after GYM1_CLEAR sets FLAG_SIROCCO_DEX_MET; recurring "Warden panel" dialogue with a
  checkitem ITEM_WARDENS_JOURNAL recognition branch; cipher-found and resolved variants.
  Kept warm/safe/alive per brief (his death is Thalvern's, not foreshadowed).
- BuriedCity discovery (Exterior coord triggers, once via flag guard): camera pan across
  the excavation; Dex's pinned note if met, anonymous dig markers if not.
  FLAG_SIROCCO_BURIED_CITY_FOUND.
- CIPHER DECISION: Haven = cipher 1, Ironhold = cipher 2, so the Interior2 central mural
  is cipher 3 - FLAG_CIPHER_3_FOUND + FLAG_SIROCCO_CIPHER_FOUND set together at the
  mural, followed by the Warden's Journal "vault, not settlement" decode text (brief
  verbatim). Vision = white-flash sequence + placeholder SPECIES_REGIROCK cry for
  Xerath. Side murals are flavor lore (city alive / drought + sealing).
- SealChamber: discovery cutscene at progress 5 (camera pan, crude Gilt Claw pump rig
  vs Covenant lattices contrast, "someone built a machine to do this deliberately") ->
  FLAG_SIROCCO_SEAL_FOUND + progress 6. Apparatus (YESNO) plays shutdown + seal
  reinforcement (white fade, rig dies, ShakeCamera, Regirock-cry placeholder) ->
  FLAG_SIROCCO_RESOLVED + progress 7, Sollis PokéNav call incl. the brief's "DAGAN was
  operating independently?" seed line, then the GALLEON CHECK: VAR_BOAT_TIER = 3 only if
  FLAG_EMBERVEIL_RESOLVED already set, else "Emberveil still burns" text. ⚠️ The mirror
  branch (FLAG_SIROCCO_RESOLVED -> tier 3) MUST live in Emberveil's SealChamber
  resolution - still a stub at time of writing. ITEM_SEAL_SHARD_SIROCCO deliberately
  NOT given (stub per brief).
- Dagan escape scene (Interior2): talk-initiated battle -> stands/straightens narration,
  exit speech with TWO optional response beats feeding VAR_DAGAN_RELATIONSHIP
  (addvar +1 each; 0 = said nothing, 1 = engaged once, 2 = engaged both), badge toss +
  TM, window beat, side-door exit under fadescreen (removeobject +
  FLAG_SIROCCO_DAGAN_ESCAPED - his object's hide flag; NO permanent-defeat flag, he
  reappears on Gildhaven), then Miria enters (FLAG_TEMP_1-hidden cameo object:
  ON_TRANSITION re-hides her every load, the scene clearflag+addobjects her) with the
  "He always has a boat ready" line -> FLAG_SIROCCO_GYM4_CLEAR + progress 5.
  Defensive fallback: if Dagan is somehow defeated-but-present, one wry line then the
  same shared escape tail.
- PROGRESS-VAR SAFETY (nonlinear island): Miria is reachable before Crag (the dig opens
  after Gym 1), so her victory advances progress 4 ONLY via call_if_eq progress==3 -
  an unconditional set would disarm the north-gate triggers without Gym 2. Crag sets 3
  from 2 linearly; the Dagan escape (5) is the choke point and sets unconditionally.
- Tennyson (DustmouthPort bg_events): chained YESNO sail-back to HavenIsle_Harbor
  (13,9) or Ironhold_GatemarkPort (10,14); post-resolution flavor variant.
- Trainers: all 11 generics (873-883) wired with brief dialogue verbatim (sight
  trainers: trainerbattle first command, setspeaker only before the post-battle
  msgbox). Post-resolution NPC variants throughout (child's stick, old woman, water
  distributors, merchant, innkeeper, Silt's "old paths", Crag joins Dex's dig, Miria
  catalogues for herself, info broker).
- Healing: Dustmouth Inn = innkeeper YESNO heal + ON_TRANSITION setrespawn
  HEAL_LOCATION_SIROCCO_DUSTMOUTH_PORT; Miraden PokemonCenter = standard nurse (also
  respawns at the Inn - the island's only heal location).

### Sirocco flow state machine (authoritative)
- VAR_SIROCCO_PROGRESS: 0=not arrived, 1=arrived (dock scene done), 2=Silt beaten,
  3=Crag beaten (north gate opens), 4=Miria beaten (only set if it was exactly 3),
  5=Dagan beaten + escaped (SealChamber discovery armed), 6=seal/siphon discovered
  (FLAG_SIROCCO_SEAL_FOUND), 7=seal reinforced / resolved (FLAG_SIROCCO_RESOLVED).
  NOTE: the brief lists "6 = Seal reinforced, 7 = Resolved"; implemented as
  6=discovered / 7=reinforced+resolved to match the Ironhold pattern and the
  map-builder's progress-5 discovery triggers.
- VAR_DAGAN_RELATIONSHIP: 0=said nothing, 1=engaged one of his two questions,
  2=engaged both (he marks the player as genuinely interesting - Gildhaven fuel).
- VAR_TEMP_0 (DexCamp only): one-per-session camp-shut narration before Gym 1.
- FLAG_TEMP_1 (DaganPalace_Interior2 only): hides the Miria cameo object; re-set by
  ON_TRANSITION every load (temp flags clear on warp = object visible by default).

### Sirocco deferred / known limitations (script side)
- Dagan's MIRE BADGE sets no engine badge flag (badge flags exhausted at 08) - badge
  case UI will show 8 badges max. Flagged for systems-engineer if it ever matters.
- Gym puzzles (Silt sandstorm maze, Crag boulder push, Miria tablet push) simplified to
  straight talk-gated battles - the reused vanilla interiors have no puzzle objects
  (same simplification as Ironhold's Rook).
- The oasis "water level visibly rises" ripple effect is narration + post-resolution
  NPC dialogue only (no tile animation built; brief itself hedges this).
- Sequence break tolerated: a player who beats Miria before Crag keeps progress at 2;
  everything still resolves (4 is cosmetic; 5 is the choke point). Crag can never
  regress progress (only sets 3 when lower).
- Dagan's side-door exit is a fadescreen + removeobject (no walk path on the reused
  Mossdeep 2F layout that's guaranteed collision-free).
- If the player's bag is full at Dagan's TM beat, the TM is silently forfeited (no
  re-claim NPC) - rare and non-blocking, same policy as Sever.

### ✅ Completed — Ironhold MAPS (map-builder, 2026-06-11, gmake exit 0)
All 17 maps built, registered, and compiling cleanly. Built via
tools/pelagios/build_ironhold.py (layouts) + build_ironhold_mapjson.py (map.json),
mirroring the Haven Isle tooling. Regenerate via those scripts; do not hand-edit
map.bin/map.json.

NEW composed outdoor layouts (General/Slateport stone-port aesthetic; General/Cave
for rocky/cave) — sampled vanilla metatiles, collision/elevation inherited:
- LAYOUT_IRONHOLD_GATEMARK_PORT 20x18 (General/Slateport) - stone port, south sea +
  central dock for the Tennyson, north gate to OuterDistrict.
- LAYOUT_IRONHOLD_OUTER_DISTRICT 24x20 (General/Slateport) - scrubland grass patches
  (wild encounters), graffiti wall, west Grapple-gated rubble to MountainPass.
- LAYOUT_IRONHOLD_IRONHOLD_CITY 28x24 (General/Slateport) - fortress town, iron-gate
  pillars, building warp tiles + signs. Petra (Gym 1) is an in-overworld NPC at the
  north end (no separate gym map - keeps the brief's 17-map count).
- LAYOUT_IRONHOLD_MOUNTAIN_PASS 20x34 (General/Cave) - winding rocky route, cave warp.
- LAYOUT_IRONHOLD_SUMMIT_APPROACH 22x20 (General/Cave) - rocky vista, WEATHER_SHADE.
- LAYOUT_IRONHOLD_SUMMIT_FORTRESS_EXTERIOR 18x16 (General/Slateport) - stone gate.

REUSED vanilla layouts (interiors, zero new binary):
- GatemarkPort_Inn = LAYOUT_POKEMON_CENTER_1F (lobby/heal); _Inn_Interior = POKEMON_CENTER_2F
- IronholdCity_PokemonCenter = POKEMON_CENTER_1F; _PokeMart = MART
- IronholdCity_Armory = HOUSE1 (foyer); _Armory_Interior = DEWFORD_TOWN_GYM (Gym 2, Forge)
- ResistanceHideout = SEALED_CHAMBER_INNER_ROOM
- SummitFortress_Interior1 = VICTORY_ROAD_1F (Gym 3, Rook)
- SummitFortress_Interior2 = SEALED_CHAMBER_OUTER_ROOM (Gym 4, Sever)
- SummitFortress_SealChamber = SEALED_CHAMBER_INNER_ROOM (Ferrath + siphon apparatus)

Battle scenes: gym battle maps (Armory_Interior, Interior1, Interior2) =
MAP_BATTLE_SCENE_GYM; all others MAP_BATTLE_SCENE_NORMAL. Summit maps use WEATHER_SHADE.

Connections (offsets validated to align openings; topology is a hub at OuterDistrict):
  GatemarkPort.up<->OuterDistrict.down ; OuterDistrict.up<->IronholdCity.down ;
  OuterDistrict.left<->MountainPass.right (Grapple-gated) ; MountainPass.up<->SummitApproach.down ;
  SummitApproach.up<->SummitFortress_Exterior.down. Summit reached via MountainPass+Cave
  (cave warps link MountainPass<->SummitApproach), NOT a direct City->Summit link
  (the brief's City "north blocked until Gym3" is handled narratively, not as a connection).

Wild encounters (src/data/wild_encounters.json, 12-slot land tables, fixed rates):
  gIronholdOuterDistrict (Machop40/Aron30/Meditite20/Riolu10),
  gIronholdMountainPass (Aron40/GeodudeAlola30/Bronzor20/Beldum10),
  gIronholdMountainPassCave (Aron50/Bronzor30/Pawniard15/Beldum5),
  gIronholdSummitApproach (Aron50/Pawniard30/FarfetchdGalar20). No water/fishing tables
  (brief defines none for Ironhold).

Heal location: HEAL_LOCATION_IRONHOLD_GATEMARK_PORT (src/data/heal_locations.json),
respawn at GatemarkPort_Inn via LOCALID_IRONHOLD_INNKEEPER.

MAPSECs added (region_map_sections.json + region_map_entries.h):
  MAPSEC_IRONHOLD_GATEMARK_PORT, _OUTER_DISTRICT, _CITY, _MOUNTAIN_PASS, _SUMMIT.

DEVIATIONS / NOTES for next agents:
- RESOLVED 2026-06-11 (systems-engineer): Ironhold hidden items + Sever gate flag.
  - FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE = FLAG_HIDDEN_ITEMS_START + 0x71 (= 0x265,
    reclaimed FLAG_UNUSED_0x265) and FLAG_HIDDEN_ITEM_IRONHOLD_IRON =
    FLAG_HIDDEN_ITEMS_START + 0x72 (= 0x266, reclaimed FLAG_UNUSED_0x266), defined
    in include/constants/flags.h right after FLAG_HIDDEN_ITEM_ROUTE_105_BIG_PEARL.
    These MUST live in the hidden-items range: asm/macros/map.inc bg_hidden_item_event
    hard-errors (.error) if flag < FLAG_HIDDEN_ITEMS_START. Do NOT put hidden-item
    flags in the 0x4xx Pelagios story block.
  - Placed as bg_events type "hidden_item": ITEM_ANTIDOTE at
    Ironhold_OuterDistrict (3,9) (adjacent to the Grapple-gated rubble at 2,9-2,10),
    ITEM_IRON at Ironhold_MountainPass (9,22) (next to the RockPile sign at 8,22).
  - FLAG_HIDE_IRONHOLD_SEVER_GATE now exists at 0x4E3 (replaced FLAG_UNUSED_0x4E3).
    The gate-Sever object in Ironhold_SummitFortress_Exterior uses it as its hide
    flag (FLAG_UNUSED_0x020 stand-in removed). script-writer: toggle this flag after
    both summit gate guards fall to reveal the Sever cameo.
  - All three changes were made in tools/pelagios/build_ironhold_mapjson.py and the
    map.json files regenerated, so the generator and output stay in sync (no hand
    edits that a regen would clobber).
- Ferrath (SealChamber) is always-visible (flag 0) - sealed/non-interactable in main story.
- Custom outdoor layouts are blocky stone rectangles (no building-sprite stamps); warp
  tiles sit on plain ground. Review in Porymap and dress up with building/door metatiles later.
- Ferrath (SealChamber) is always-visible (flag 0) - sealed/non-interactable in main story.
- Custom outdoor layouts are blocky stone rectangles (no building-sprite stamps); warp
  tiles sit on plain ground. Review in Porymap and dress up with building/door metatiles later.

### ✅ Completed — Ironhold SCRIPTS (script-writer, 2026-06-11, gmake exit 0)
All 17 Ironhold scripts.inc are fully implemented (zero TODO placeholders remain).
Highlights, grouped by system:
- Arrival (GatemarkPort): trigger cutscene at the dock head (guard marches down,
  Navigator's Log check - missing log turns the player back, log present = uneasy
  "Warden's log. You may pass." + camera pan to the glowing summit). Sets
  FLAG_IRONHOLD_ARRIVED + VAR_IRONHOLD_PROGRESS=1.
- Gym gating: Forge refuses battle until FLAG_IRONHOLD_GYM1_CLEAR (Armory apprentice
  foreshadows it); Sever refuses until FLAG_IRONHOLD_GYM3_CLEAR; SealChamber mid-room
  barrier triggers bounce the player until progress 5. All four leaders are
  talk-initiated (TRAINER_TYPE_NONE in map.json - trainer-see requires trainerbattle
  as the FIRST script command, incompatible with gating).
- Badges: Petra FLAG_BADGE02_GET, Forge 03, Rook 04, Sever 05 (badge 1 reserved for a
  future island per the "badge 2 overall" numbering in IRONHOLD_BRIEF.md).
- TM substitutions (briefed TMs don't exist in the vanilla TM set): Petra TM_BULK_UP
  (as briefed), Forge TM_STEEL_WING (for Iron Defense), Rook TM_BRICK_BREAK (for Close
  Combat), Sever TM_IRON_TAIL (for Meteor Mash). Dialogue lampshades the substitutions.
- Grapple Hook: Forge victory gives ITEM_GRAPPLE_HOOK + FLAG_GRAPPLE_HOOK_OBTAINED +
  progress=3 BEFORE the TM (so a full bag can never skip the key item). Rubble gate is
  script-side: OuterDistrict coord triggers (progress 0/1/2) bounce the player with a
  "too heavy" message (defensive checkitem branch plays a clear scene if the hook is
  somehow held while armed); at progress>=3 the triggers disarm and the pass is open.
- Resistance Hideout: ON_FRAME (VAR_TEMP_0) first-entry scene - leader approaches,
  recognizes the Warden's kid, gives ITEM_DOCUMENT_FRAGMENT; sets
  FLAG_DOCUMENT_FRAGMENT_OBTAINED + FLAG_IRONHOLD_RESISTANCE_MET. Pre-Gym1 entry gets
  stonewalled ("Wrong basement, friend."). The City HQ guard object now uses
  FLAG_IRONHOLD_GYM1_CLEAR as its hide flag (rotated north after Petra falls), opening
  the hideout door per the brief.
- Sever gate cameo (SummitFortress_Exterior): ON_TRANSITION manages
  FLAG_HIDE_IRONHOLD_SEVER_GATE (hidden until BOTH elite guards defeated; the second
  victory script reveals him with addobject; talking plays the cameo then he walks into
  the fortress and the flag re-sets permanently; also hidden once GYM4 clear).
- Sever key scene (Interior2): wordless badge handover, fade-reposition to the chamber
  door (setobjectxy + copyobjectxytoperm - no walk path the player could block),
  unlock, his one line, then YESNO choice: YES "I understand." -> VAR_SEVER_RELATIONSHIP=1
  (Respected); NO (say nothing; folds in the brief's "That's not enough.") -> stays 0.
  Sets FLAG_IRONHOLD_GYM4_CLEAR + progress=5. He never speaks again on the island and
  his object hide flag is FLAG_IRONHOLD_RESOLVED (gone after resolution, per brief).
- SealChamber: discovery cutscene at progress 5 (camera pan across the siphon to
  Ferrath, ShakeCamera, "draining something that shouldn't be drained") -> sets
  FLAG_IRONHOLD_SEAL_FOUND + progress=6. Apparatus interaction (YESNO) plays the
  reinforcement (white fade, machinery dies, placeholder SPECIES_REGISTEEL cry for
  Ferrath) -> FLAG_IRONHOLD_RESOLVED + FLAG_IRONHOLD_CIPHER_FOUND + FLAG_CIPHER_2_FOUND,
  progress=7, VAR_BOAT_TIER=2, journal entry text, Sollis PokeNav call, Brigantine
  unlock text. ITEM_SEAL_SHARD_IRONHOLD deliberately NOT given (stub per brief).
- CIPHER DECISION: IRONHOLD_BRIEF.md calls this "cipher 1 of 9", but Haven Isle's
  ruins inscription already sets FLAG_CIPHER_1_FOUND. Ironhold's is therefore treated
  as cipher 2 (FLAG_CIPHER_2_FOUND + FLAG_IRONHOLD_CIPHER_FOUND, both set at the
  apparatus). The MountainPass_Cave inscription is flavor-only cross-island lore
  (no cipher flag), echoing Haven's "WHILE ONE LOCK HOLDS" text.
- Tennyson: boarding now routes through the SHARED tier-gated destination menu
  (see "Tennyson destination-select system" below). The old per-harbor YESNO
  warps were replaced 2026-06-12; the brigantine-refit flavor survives as an
  intro msgbox before the menu.
- Trainers: all 7 generic (857-863) + 4 leaders (864-867) wired with brief dialogue.
  ADDED 5 summit garrison trainers 868-872 (TRAINER_COVENANT_IRONHOLD_5..9: Bryn,
  Karst, Ostin, Wrenna, Senna) in opponents.h + trainers.party for the
  Patrol2/Patrol3/StationedA/StationedB/EliteGuard2 NPCs that had no IDs.
  TRAINERS_COUNT_EMERALD 868 -> 873 (151 slots free).
- Healing: GatemarkPort Inn = innkeeper YESNO heal (Common_EventScript_OutOfCenterPartyHeal)
  plus ON_TRANSITION setrespawn HEAL_LOCATION_IRONHOLD_GATEMARK_PORT. City PokemonCenter =
  standard nurse (also respawns at the Inn - the island's only heal location).
- Generator (tools/pelagios/build_ironhold_mapjson.py) fixes, all regenerated:
  coord triggers now use VAR_IRONHOLD_PROGRESS (the old FLAG_* "var" fields could
  NEVER fire - VarGet of a flag ID returns the ID itself); leaders TRAINER_TYPE_NONE;
  HQ guard hide flag; Interior2 Sever hide flag; SealChamber DoorSealed triggers added
  (new label IronholdSealChamber_EventScript_DoorSealed).
- NAMEBOXES (2026-06-11, gmake exit 0): speaker nameboxes retrofitted across ALL
  Haven Isle + Ironhold dialogue via `setspeaker` + data/scripts/pelagios_speaker_names.inc
  (included from event_scripts.s). Inline "NAME:" prefixes removed; mixed
  narration+speech texts split into separate msgboxes. Full pattern + rules:
  Dialogue Style Guidelines > Speaker nameboxes (above). Deliberate exceptions:
  Resistance Hideout (quoted-narration style keeps no nameboxes), Forge's battle
  intro (opens with narration), Sorn's post-battle line (mostly narration),
  sight-trainer intros (engine constraint), nurse common scripts (shared vanilla).

### Ironhold flow state machine (authoritative; vars.h comment is stale)
- VAR_IRONHOLD_PROGRESS: 0=not arrived, 1=arrived (dock scene done), 2=Petra beaten,
  3=Forge beaten + Grapple Hook (rubble disarms), 4=Rook beaten, 5=Sever beaten
  (chamber open), 6=siphon discovered (FLAG_IRONHOLD_SEAL_FOUND), 7=seal reinforced /
  island resolved (FLAG_IRONHOLD_RESOLVED, VAR_BOAT_TIER=2).
  NOTE: include/constants/vars.h still comments "0=arrived..5=resolved" - that comment
  is stale (script-writer may not edit vars.h); THIS mapping is the implemented one.
- VAR_SEVER_RELATIONSHIP: 0=Unacknowledged (default / said nothing), 1=Respected
  ("I understand." after his post-battle line).
- VAR_TEMP_0 (ResistanceHideout only): ON_FRAME first-entry scene disarm for the
  current map session; permanence comes from FLAG_IRONHOLD_RESISTANCE_MET.

### Ironhold deferred / known limitations (script side)
- Rubble has no one-time clearing animation on the normal path: once the hook is
  obtained the triggers simply disarm. A dedicated "rubble cleared" flag (0x4E4 free)
  or a C field effect would enable a proper one-shot clear scene - deferred
  (C field effects are systems-engineer domain).
- The brief's second Grapple Hook application ("SummitFortress gate mechanism") has no
  corresponding map object/trigger - skipped.
- Rook's "locked doors opened in sequence" gym puzzle simplified to the two stationed
  trainers (Victory Road layout has no door objects).
- Gym TMs given after flags/key items with Common_EventScript_ShowBagIsFull fallback
  (Petra/Forge/Rook) or silently skipped (Sever - his key scene must not abort);
  no re-claim flags exist, so a full bag forfeits the TM. Rare and non-blocking.
- Forge does not physically reappear in IronholdCity post-resolution (no such object);
  his post-resolution dialogue plays at his anvil in the Armory instead.
- New-game init does not touch Ironhold state: progress var defaults 0,
  FLAG_HIDE_IRONHOLD_SEVER_GATE is re-asserted by the map's ON_TRANSITION every load.

### ✅ Completed — Emberveil MAPS (map-builder, 2026-06-12, gmake exit 0)
All 15 maps built, registered, and compiling cleanly. Built via
tools/pelagios/build_emberveil.py (layouts) + build_emberveil_mapjson.py (map.json)
+ build_emberveil_scripts_stubs.py (stub scripts.inc). Regenerate via those scripts;
do NOT hand-edit map.bin/map.json. EWRAM 86.45% / IWRAM 86.63% / ROM 79.19%.

NEW composed outdoor layouts (General/Lavaridge volcanic aesthetic; metatiles sampled
verbatim from Mt Chimney so collision/elevation are correct):
- LAYOUT_EMBERVEIL_ASHMOUTH_PORT 20x18 - volcanic stone port, south sea + central dock
  for the Tennyson, north exit -> LavaRoute1.
- LAYOUT_EMBERVEIL_LAVA_ROUTE1 20x28 - lava-field route, tall-grass encounter patches,
  decorative (non-gating) lava channels, 2 Rawst berry trees + 2 hidden Rawst berries.
- LAYOUT_EMBERVEIL_ASH_FIELDS 24x20 - open ash plains, broad encounter grass, dead-tree
  pillars (examine via signs).
- LAYOUT_EMBERVEIL_CINDERHOLD_CITY 28x24 - volcanic city, south gate -> AshFields, east
  gate -> LavaRoute2. Gym1 (Cinder) + Gym2 (Slag) are in-overworld NPCs (no separate gym
  maps - keeps the 15-map count; mirrors Ironhold's Petra pattern).
- LAYOUT_EMBERVEIL_LAVA_ROUTE2 24x18 - narrow route, Lava-Boots-gated lava wall leaving a
  2-tile corridor at x=8-9.
- LAYOUT_EMBERVEIL_CALDERA_APPROACH 22x22 - caldera ring, Gym3 (Vex), Lava-Boots-gated
  lava band sealing the north ruins stairway (corridor x=10-11), warp up to CalderaRuins.
- LAYOUT_EMBERVEIL_CALDERA_RUINS 20x24 - ancient stone chambers, 3 Path-B panel signs
  (panel3 in a walled north alcove = Warden's notes), warps down/up.
- LAYOUT_EMBERVEIL_VOLCANO_ASCENT 18x30 - steep final approach, lava channels, WEATHER_DROUGHT.
- LAYOUT_EMBERVEIL_VOLCANO_SUMMIT 20x20 - lava-ring ritual platform (walkable centre),
  Solace + the seal, WEATHER_DROUGHT, warp down into SealChamber.

REUSED vanilla layouts (interiors, zero new binary):
- AshmouthPort_Inn = POKEMON_CENTER_1F (lobby/heal); _Inn_Interior = POKEMON_CENTER_2F
- CinderholdCity_PokemonCenter = POKEMON_CENTER_1F
- CinderholdCity_TempleHall (foyer) = HOUSE2; _TempleHall_Interior (grand ceremonial
  hall, Solace's early location + central-flame vision sign) = SEALED_CHAMBER_OUTER_ROOM
- SealChamber = SEALED_CHAMBER_INNER_ROOM (Pyrath decoration + seal apparatus, mirrors
  Ironhold's SealChamber exactly: Pyrath SS_TIDAL script 0x0 at (10,5), apparatus
  CABLE_CAR at (10,8), discovery triggers row 14, warp back at (10,19)).

Battle scenes: all MAP_BATTLE_SCENE_NORMAL (gyms are in-overworld NPCs, no gym maps).
Weather: SUNNY outdoors, DROUGHT on VolcanoAscent + VolcanoSummit, NONE indoors + SealChamber.

Connections (offsets validated reciprocal; topology is the linear chain from the brief):
  AshmouthPort.up<->LavaRoute1.down (0) ; LavaRoute1.up<->AshFields.down (-2/+2) ;
  AshFields.up<->CinderholdCity.down (-2/+2) ; CinderholdCity.right<->LavaRoute2.left (3/-3) ;
  LavaRoute2.right<->CalderaApproach.left (-2/+2) ; VolcanoAscent.up<->VolcanoSummit.down (-1/+1).
  CalderaApproach->CalderaRuins->VolcanoAscent are WARPS (steep stairways), not connections.

Wild encounters (src/data/wild_encounters.json, 12-slot land tables, rate 20, brief species):
  gEmberveilLavaRoute1 (Slugma40/Numel30/Houndour20/Magby10),
  gEmberveilAshFields (Numel40/Houndour30/Torkoal20/Litwick10),
  gEmberveilLavaRoute2 (Camerupt40/Houndoom30/Turtonator20/Larvesta10),
  gEmberveilCalderaApproach (Camerupt40/Turtonator30/Magmar20/Larvesta10),
  gEmberveilCalderaRuins (Litwick~50/Lampent~30/Heatran~20, low-level rare Heatran).
  No water/fishing tables (brief defines none). No encounters in port/city/temple/summit/
  ascent/sealchamber (no table = no spawns).

Heal location: HEAL_LOCATION_EMBERVEIL_ASHMOUTH_PORT, respawn at AshmouthPort_Inn via
LOCALID_EMBERVEIL_INNKEEPER (mirrors Ironhold).

MAPSECs added (region_map_sections.h enum + .json + region_map_entries.h):
  MAPSEC_EMBERVEIL_ASHMOUTH_PORT, _LAVA_ROUTE1, _LAVA_ROUTE2, _ASH_FIELDS,
  _CINDERHOLD_CITY, _CALDERA, _VOLCANO (7 total).

DEVIATIONS / NOTES for the script-writer (pelagios-script-writer is NEXT):
- All 15 scripts.inc are STUBS (build_emberveil_scripts_stubs.py): each defines its
  <Map>_MapScripts:: (.byte 0, empty table) plus a minimal placeholder per referenced
  EventScript label (talk NPCs/signs = lockall/releaseall/end; coord triggers = bare end).
  Every label is tagged "@ TODO (script-writer): implement ...". Replace with real content;
  the stub generator will NOT clobber a scripts.inc once it contains a .string line.
- LAVA-BOOTS GATES are coord triggers comparing VAR_EMBERVEIL_PROGRESS (NOT flags - VarGet
  of a flag returns the flag id). Armed for progress 2 (Gym2 not done -> no Lava Boots):
  LavaRoute2 EmberveilLavaRoute2_EventScript_LavaGate at (10,8)/(10,9);
  CalderaApproach EmberveilCalderaApproach_EventScript_LavaGate at (10,8)/(11,8).
  CinderholdCity east gate (EmberveilCinderholdCity_EventScript_CalderaGate) armed for
  progress 1 AND 2 at (26,11)/(26,12). Add more progress-value duplicate triggers if you
  need finer gating (Ironhold pattern: one trigger row per blocked progress value).
- GYM LEADERS are talk-initiated NPCs (TRAINER_TYPE_NONE), so their scripts can be flag-gated
  (trainer-see requires trainerbattle as the FIRST command). Cinder + Slag live in
  CinderholdCity (LOCALID_EMBERVEIL_CINDER at the south gate 14,20; LOCALID_EMBERVEIL_SLAG
  at the east lava works 21,14). Vex is at CalderaApproach (LOCALID_EMBERVEIL_VEX 10,7).
  Solace appears TWICE: LOCALID_EMBERVEIL_SOLACE_TEMPLE in TempleHall_Interior (hide flag
  FLAG_EMBERVEIL_GYM2_CLEAR - vanishes after Gym2) and LOCALID_EMBERVEIL_SOLACE_SUMMIT on
  the VolcanoSummit platform (always visible; gate her pre-battle scene by progress). The
  SolaceScene triggers (EmberveilVolcanoSummit_EventScript_SolaceScene, (9,8)/(10,8)) fire
  at progress 4 (Gym3 done). Trainer IDs 896-899 (Cinder/Slag/Vex/Solace) already exist.
- SOLACE SPRITE: brief asks for LADY placeholder; OBJ_EVENT_GFX_LADY does not exist in the
  expansion - used OBJ_EVENT_GFX_WOMAN_5 for both Solace objects. Swap to a custom sprite later.
- Path-B panels: 3 signs in CalderaRuins (Panel1 (7,7), Panel2 (12,7), Panel3 (9,4) in the
  north alcove). The brief wants them examined in order via a LOCAL STEP COUNTER (not flags);
  Panel3 gives ITEM_WARDEN_NOTES + sets FLAG_WARDEN_NOTES_EMBERVEIL.
- Tennyson: SS_TIDAL decoration (script 0x0) at AshmouthPort (12,15); boarding via 5
  sign-type bg_events on the pier edge/south end (all -> EmberveilAshmouthPort_EventScript_Tennyson).
  NEVER put the boarding script on the ship object (per the harbor rule). Sail-back-to-Haven
  warp target should be a walkable Haven Harbor tile (Haven uses 13,9). Inbound from Haven:
  Haven's Tennyson currently warps to Ironhold; the boat-tier-3 routing to Emberveil is a
  boat-transition-system concern (not built) - the arrival cutscene + FLAG_EMBERVEIL_ARRIVED /
  VAR_EMBERVEIL_PROGRESS=1 fire from the AshmouthPort arrival coord trigger at (9,11)/(10,11).
- MUSIC placeholders: MUS_SLATEPORT (port), MUS_LILYCOVE (city), MUS_ROUTE110 (routes),
  MUS_MT_PYRE (caldera/ruins/ascent), MUS_MT_CHIMNEY (summit), MUS_LILYCOVE_MUSEUM (temple
  foyer), MUS_SEALED_CHAMBER (seal chamber). Swap to themed tracks later.
- Custom outdoor layouts are blocky volcanic rectangles (lava walls/channels, ash grass,
  stone walls); warp tiles sit on plain ground. Review in Porymap and dress with
  building/door/cliff metatiles later (same caveat as Ironhold).
- Hidden Rawst berries are bg_events type hidden_item with FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1
  (0x268) / BERRY2 (0x269) at LavaRoute1 (3,6)/(16,21) - in the hidden-items range, as required.

UNRELATED FIX made to unblock linking (parallel Sirocco build was broken): the Sirocco
map.json (and tools/pelagios/build_sirocco_mapjson.py) referenced MUS_ROUTE111, which is NOT
a real song symbol - the whole tree failed to link. Replaced all 6 with MUS_ROUTE110
(generator + the 6 generated map.json kept in sync). This was pre-existing breakage from the
Sirocco map build, not Emberveil work; flagging it here for the Sirocco owner.

### ✅ Completed — Emberveil SCRIPTS (script-writer, 2026-06-12, gmake exit 0)
All 15 Emberveil scripts.inc fully implemented - zero TODO placeholders remain.
(Most were written by an earlier session that was cut off mid-run; this session
verified all 15 for consistency, charmap compliance, namebox rules, and trigger
wiring, then wrote the one missing piece: the SealChamber discovery + resolution.)
- Arrival (AshmouthPort): ON_FRAME at VAR_EMBERVEIL_PROGRESS==0 (NOT coord triggers -
  the west port is fully walkable and the future boat-warp position is unknown).
  Greeter approach, camera pan up to the volcano, FLAG_EMBERVEIL_ARRIVED + progress 1.
- Gyms: all four leaders talk-initiated (TRAINER_TYPE_NONE). Badges are NARRATIVE-ONLY
  (engine badge flags exhausted at FLAG_BADGE08_GET): fanfare + "received the X BADGE"
  text + island GYM_CLEAR flag only. Slag gated on GYM1_CLEAR; Solace's scene on
  progress 4. TM substitutions (lampshaded in dialogue): Cinder TM_FLAMETHROWER (as
  briefed), Slag TM_EARTHQUAKE (no Earth Power TM), Vex TM_AERIAL_ACE (no Brave Bird
  TM), Solace TM_FIRE_BLAST (as briefed, no bag-full branch - key scene must not abort).
- Lava Boots: Slag victory gives ITEM_LAVA_BOOTS + FLAG_LAVA_BOOTS_OBTAINED +
  progress=3 BEFORE the TM (full bag can never skip it). Gates are script-side
  (Ironhold rubble pattern): city east gate triggers (progress 0-2) bounce west;
  LavaRoute2 (10,8)/(10,9) + CalderaApproach (10,8)/(11,8) lava-gate triggers
  (progress 0-2) bounce with heat text, defensive checkitem ITEM_LAVA_BOOTS branch
  plays a crossing line if the boots are somehow held while armed. At progress 3 all
  disarm; CalderaApproach re-arms the same tiles as the Vex intercept (progress 3 only).
- Warden's Notes (Path B unlock): CalderaRuins panels via VAR_TEMP_1 local step
  counter (resets on map load = "in one visit" per brief): Panel1 0->1, Panel2 1->2,
  Panel3 at 2 opens the alcove - ITEM_WARDEN_NOTES + FLAG_WARDEN_NOTES_EMBERVEIL.
  Partial cipher translations on panels 1-2 gated on FLAG_CIPHER_2_FOUND.
- Summit (Path A): Solace ritual scene -> battle (trainerbattle_no_intro) -> serene
  defeat -> "Was I wrong?" YESNO: YES = FLAG_SOLACE_TOLD_TRUTH, NO ("about the
  method") = FLAG_SOLACE_ALT_ENDING -> badge, GYM4_CLEAR, progress 5.
- Summit (Path B): notes prompt -> decline retreats the player 2 tiles north (trigger
  re-arms, repeatable); accept = commune ritual (white fade, GROUDON cry placeholder
  for Pyrath, seal stabilizes), NO battle, GYM4_CLEAR + FLAG_EMBERVEIL_PATH_B,
  progress 5. Battle loss on Path A: whiteout, progress stays 4, scene re-fires.
- SealChamber: discovery triggers (row 14, progress==5) play the camera-pan reveal -
  Pyrath text is path-split (restless/dimmed on A, calm/aware on B) - sets
  FLAG_EMBERVEIL_SEAL_FOUND + progress 6. Apparatus (YESNO) plays the reinforcement
  (no siphon here - the ancient monitoring station mends honest wear): sets
  FLAG_EMBERVEIL_RESOLVED + FLAG_EMBERVEIL_CIPHER_FOUND + FLAG_CIPHER_4_FOUND,
  progress 7, journal cipher-4 entry (Dorne tease per brief), Solace PokeNav line
  (path-split), Sollis PokeNav call (path-split replies; Path A version seeds "your
  parent spent years on another approach" - Maren knows more than she says), then the
  Galleon check. ITEM_SEAL_SHARD_EMBERVEIL deliberately NOT given (stub per brief).
- CIPHER DECISION: Haven=1, Ironhold=2, Sirocco=3 (BuriedCity mural), Emberveil=4
  (FLAG_CIPHER_4_FOUND + FLAG_EMBERVEIL_CIPHER_FOUND, both at the apparatus). The
  brief consistently says cipher 4 - no discrepancy this island.
- GALLEON CHECK (order-independent): apparatus resolution does
  goto_if_set FLAG_SIROCCO_RESOLVED -> VAR_BOAT_TIER = 3 + refit text, else
  "Sirocco's sands still restless" text. Sirocco's SealChamber has the verified
  mirror branch (checks FLAG_EMBERVEIL_RESOLVED). Tier is only ever SET to 3 here,
  never lowered - either island may resolve second.
- Path A/B post-resolution NPC variants on EVERY recurring NPC: port greeter, sailor,
  both inn travelers, innkeeper, Cinder ("questions don't matter" / next historian
  beat), Vex (silent watch narration / pure relief), Slag (path-independent per
  brief), historian, doubting NPC (leaving / "right to wonder"), cult member, child,
  temple attendant, both worshippers, Solace at the summit (ResolvedA / ResolvedB).
- TempleHall central flame vision: repeatable white-fade triptych (sealing witnessed,
  first ritual, generations keeping a lock turned). Solace temple cameo hidden by
  FLAG_EMBERVEIL_GYM2_CLEAR (she moves to the summit).
- Tennyson (AshmouthPort): 5 bg_events on the pier (ship object decorative, per the
  harbor rule), sail back to MAP_HAVEN_ISLE_HARBOR (13,9), with resolved /
  Sirocco-pending / Galleon-refit text variants.
- Speakers: NO new entries needed - all Emberveil speakers (Solace, Cinder, Slag,
  Vex, Sera, Bren, Mira, Rem, Voss, Tane, Horne, Cael, Devotee, Worshipper,
  Attendant, Historian + role names) already existed in pelagios_speaker_names.inc.

### Emberveil flow state machine (authoritative)
- VAR_EMBERVEIL_PROGRESS: 0=not arrived (AshmouthPort arrival ON_FRAME armed; city
  east gate + both lava gates armed at 0-2), 1=arrived, 2=Cinder beaten, 3=Slag
  beaten + Lava Boots (east gate + lava gates disarm; Vex intercept arms),
  4=Vex beaten (intercept disarms; summit SolaceScene triggers (9,3)/(10,3) arm),
  5=Solace confronted, Path A or B (summit triggers disarm; SealChamber discovery
  triggers row 14 arm), 6=seal found (discovery played; apparatus offer live),
  7=resolved (FLAG_EMBERVEIL_RESOLVED; Galleon if Sirocco also resolved).
- Path determinant at progress 4 entry: FLAG_WARDEN_NOTES_EMBERVEIL set -> Path B
  prompt; unset -> Path A. FLAG_EMBERVEIL_PATH_B is set only by Path B acceptance
  and drives every later A/B text branch.
- VAR_TEMP_1 (CalderaRuins only): panel step counter (0->1->2), resets on map load.

### Emberveil deferred / known limitations (script side)
- REACHABILITY: RESOLVED 2026-06-12. The shared Tennyson destination menu is now
  built (see "Tennyson destination-select system" below). At Brigantine tier
  (VAR_BOAT_TIER 2, set by Ironhold's SealChamber) the menu lists Sirocco Isle and
  Emberveil, so both islands are reachable from any harbor without a debug warp.
- Path B is permanently unavailable once Path A completes (notes found afterward are
  lore-only via Panel3's normal discovery) - by design per the brief ("if found
  before confronting Solace").
- Solace's Final Island appearance/death (Path A) and post-game letter (Path B) are
  future-island wiring - nothing set up beyond the flags.
- Cinder's gym brazier puzzle and Slag's lava-valve puzzle simplified away (leaders
  are in-overworld NPCs, no gym maps - established Ironhold/Petra pattern).
- Pyrath cry placeholder = SPECIES_GROUDON (summit commune + seal chamber).
- The brief's exterior "volcano rumbles then quiets" resolution cutaway is rendered
  as prose inside the SealChamber (player is a mile underground; no camera cut).

### ✅ Schism Isle — CONSTANTS (systems-engineer, 2026-06-13, gmake exit 0)
Systems/constants layer complete. Maps and scripts NOT started (map-builder is next).
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.53% (26,685,900 B).
- **Flags (16 story flags + 2 hidden-item flags):** Block 2 0x49D-0x4A6 filled
  ENTIRELY (10): FLAG_SCHISM_ARRIVED 0x49D, GYM1_CLEAR 0x49E, GYM2_CLEAR 0x49F,
  GYM3_CLEAR 0x4A0, GYM4_CLEAR 0x4A1, FLAG_EIRA_SCAR_PASS 0x4A2,
  FLAG_DRENN_SCAR_PASS 0x4A3, FLAG_SCAR_RUINS_FOUND 0x4A4,
  FLAG_EIRA_CEASEFIRE_WILLING 0x4A5, FLAG_DRENN_CEASEFIRE_WILLING 0x4A6.
  The other 6 went into NEW "STORY BLOCK 3" = scattered FLAG_UNUSED slots (no
  contiguous run remains below 0x500): FLAG_SCHISM_SEAL_NORTH_FOUND 0x468,
  FLAG_SCHISM_SEAL_SOUTH_FOUND 0x470, FLAG_SCHISM_SEAL_NORTH_DONE 0x472,
  FLAG_SCHISM_SEAL_SOUTH_DONE 0x479, FLAG_SCHISM_CEASEFIRE 0x4F9,
  FLAG_SCHISM_CIPHER_FOUND 0x4FA. **REUSED existing flags (not redefined):**
  FLAG_SCHISM_RESOLVED 0x4AF, FLAG_DRENN_ALIVE 0x4BB, FLAG_CIPHER_5_FOUND 0x4C1.
  Hidden items: FLAG_HIDDEN_ITEM_SCHISM_1 0x26A (ice side),
  FLAG_HIDDEN_ITEM_SCHISM_2 0x26B (the Scar) — both in the hidden-items range.
  ⚠️ After Schism, only 0x4FF (1 scattered slot) remains below 0x500. Thalvern/
  Gildhaven need a fresh story-flag region — see "STORY BLOCK 3" subsection.
- **Vars:** ONE new var VAR_SCHISM_CEASEFIRE_PROGRESS 0x410A (was a reserved spare;
  0=none/1=skeptical/2=willing/3=done). VAR_SCHISM_PROGRESS 0x4103 pre-existed
  (0=not arrived .. 7=resolved per brief). 5 reserved spares 0x410B-0x410F remain.
- **Items (888-891):** ITEM_SCAR_PASS_ICE 888, ITEM_SCAR_PASS_POISON 889,
  ITEM_SEAL_SHARD_GLACITH 890, ITEM_SEAL_SHARD_TOXARA 891 (all key items, importance 1,
  CannotUse, placeholder icons: passes = Powder/EnergyPowder, shards = BlueOrb). Seal
  Shards are stubs (not awarded yet). Names charmap-safe (no em dashes/straight quotes).
- **Trainers (900-909):** 6 faction generics + 4 gym leaders, parties from
  SCHISM_BRIEF.md verbatim. TRAINER_ICEFACTION_SCHISM_1 900 (Heln), _2 901 (Vera),
  TRAINER_POISONFACTION_SCHISM_1 902 (Osk), _2 903 (Fen), TRAINER_ICEFACTION_SCHISM_3
  904 (Rael), TRAINER_POISONFACTION_SCHISM_3 905 (Siv); TRAINER_LEADER_SCHISM_SLEET
  906 (Ice, Youngster pic), _EIRA 907 (Ice, Cooltrainer F), _MURK 908 (Poison/Ghost,
  Cooltrainer M), _DRENN 909 (Poison/Dragon, Scientist FRLG pic). TRAINERS_COUNT_EMERALD
  900 -> 910 (114 free). Alolan Grimer = `Grimer-Alola` in the party (parser ->
  SPECIES_GRIMER_ALOLA).
- **BADGES: narrative-only** (DECIDED region-wide). All 4 Schism gyms set only
  FLAG_SCHISM_GYM*_CLEAR. No FLAG_BADGE*_GET (exhausted at 08). Gating uses
  GYM*_CLEAR / VAR_SCHISM_PROGRESS.
- **Boat menu — DUAL Schism ports:** MultichoiceList_BoatGalleon now has TWO Schism
  entries ("SCHISM (NORTH)" / "SCHISM (SOUTH)"). pelagios_boat.inc has two stub
  handlers Pelagios_EventScript_SailToSchismNorth/South (each `goto SailNoChart` for
  now) + a second island id PELAGIOS_ISLAND_SCHISM_SOUTH (7). Map-builder swap-in
  instructions (warp targets MAP_SCHISM_FROSTMARK_PORT / MAP_SCHISM_VENOMQUAY_PORT,
  same-island guards, VAR_TEMP_1 wiring) are commented inline in pelagios_boat.inc.
- **DEFERRED to map-builder:** (1) gMapGroup_Schism / MAP_GROUP_SCHISM registration in
  map_groups.json — an empty map group can't be expressed in generated groups.inc
  (established Haven/Ironhold/Sirocco/Emberveil pattern); register with the first real
  Schism map. (2) MAPSEC_SCHISM_* entries (region_map_sections.h/.json +
  region_map_entries.h) — these are tightly coupled to map x/y coverage and the brief's
  systems-engineer checklist omits them; add when building the maps. (3) Swapping the
  two boat stub handlers to real warps once the ports exist. (4) Heal locations for both
  port Inns (add both to IsLastHealLocationPlayerHouse if they respawn to a house — they
  are Inns, so they respawn at the Inn like Ironhold/Sirocco/Emberveil; no house-heal
  special-case needed).

### ✅ Thalvern Isle — CONSTANTS (systems-engineer, 2026-06-13, gmake exit 0)
Systems/constants layer complete. Maps and scripts NOT started (map-builder is next).
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.67% (26,733,076 B).
- **Flags (10 new, STORY BLOCK 4 0x26C-0x275):** FLAG_THALVERN_ARRIVED 0x26C,
  GYM1_CLEAR 0x26D (Tide), GYM2_CLEAR 0x26E (Psalm), GYM3_CLEAR 0x26F (Lens),
  FLAG_THALVERN_DEX_MET 0x270, FLAG_THALVERN_LENS_DEFECTED 0x271,
  FLAG_THALVERN_THRONE_CHOICE 0x272, FLAG_THALVERN_SEAL_FOUND 0x273,
  FLAG_THALVERN_CIPHER_FOUND 0x274, FLAG_NUMA_VESS_CONFRONTED 0x275. These renamed the
  BLOCK 4 START boundary placeholder + the next 9 FLAG_UNUSED slots (collision-verified:
  one def each, zero external refs). **REUSED pre-allocated flags (NOT redefined):**
  FLAG_THALVERN_RESOLVED 0x4B0, FLAG_DEX_ALIVE 0x4BA (both pre-existed in the 0x4xx
  blocks). **CIPHER decision:** Thalvern is cipher 6. FLAG_CIPHER_6_FOUND 0x4C2 ALREADY
  EXISTS (the CIPHER_1-9 set is pre-allocated) — REUSED, not added. FLAG_THALVERN_CIPHER_FOUND
  0x274 is the island-local flag; scripts set both at the Throne Room cipher.
  **Hidden items: NONE** — the brief lists no Thalvern hidden items, so zero hidden-item
  flags consumed (FLAG_HIDDEN_ITEMS_START is 0x1F4; the bg_hidden_item_event macro hard-
  errors below it, and the last contiguous vanilla-capable slot 0x264 remains free for a
  future island; BLOCK 4 0x26C+ is technically >= 0x1F4 so legal for hidden items if ever
  needed, but none were placed here).
- **Vars:** NONE added. VAR_THALVERN_PROGRESS 0x4104 pre-existed (states 0=not arrived ..
  7=resolved per brief). No relationship/state var needed (FLAG_DEX_ALIVE carries the one
  branching choice). **5 reserved spares 0x410B-0x410F remain.**
- **Items (892-894):** ITEM_COVENANT_ACCESS_CARD 892 (Lens gives it; bypasses CovenantSite
  blockade; icon Powder/EnergyPowder), ITEM_SEAL_SHARD_THALVERN 893 (**THE Feraligatr Mega
  Evolution trigger** per the story bible — icon BlueOrb/BlueOrb; the actual Mega-Evolution
  wiring is FUTURE systems work, the item + placeholder is enough now), ITEM_DEX_NOTES 894
  (Dex's decoded notes, PATH B only; icon Powder/EnergyPowder). All key items, importance 1,
  CannotUse, names charmap-safe. **Next free item ID: 895.** NOTE: ITEM_SONAR_LENS (878,
  Dive replacement) ALREADY EXISTED from the Haven key-item batch — NOT re-added.
- **Trainers (910-916, 7 total — Thalvern has ONLY 3 gyms, not 4):** parties from
  THALVERN_BRIEF.md verbatim. Generics: TRAINER_SCHOLAR_THALVERN_1 910 (Wren, Scientist FRLG
  pic), TRAINER_COVENANT_THALVERN_1 911 (Holt, Scientist FRLG), _2 912 (Sael, Scientist FRLG
  F), _3 913 (Cael, Scientist FRLG). Gym leaders: TRAINER_LEADER_THALVERN_TIDE 914 (Water,
  Fisherman pic per brief), _PSALM 915 (Psychic, Cooltrainer F), _LENS 916 (Water/Psychic,
  Cooltrainer M per brief sprite). Wishiwashi listed as bare `Wishiwashi` (School form is
  in-battle). TRAINERS_COUNT_EMERALD 910 -> 917 (**107 free**).
- **BADGES: narrative-only** (DECIDED region-wide). All 3 Thalvern gyms set only
  FLAG_THALVERN_GYM*_CLEAR. No FLAG_BADGE*_GET (exhausted at 08). Gating uses GYM*_CLEAR /
  VAR_THALVERN_PROGRESS.
- **MAPSECs added (6, covering 13 maps):** MAPSEC_THALVERN_TIDESPIRE_PORT,
  _FLOATING_MARKET, _COASTAL_ROUTE, _SUBMERGED_RUINS, _COVENANT_SITE, _THRONE_ROOM in
  region_map_sections.h (enum) + .json + region_map_entries.h (placeholder x/y 0,0 1x1 —
  map-builder positions them on the region map). Done in the constants pass (Sirocco/
  Emberveil precedent).
- **DEFERRED to map-builder:** (1) gMapGroup_Thalvern / MAP_GROUP_THALVERN registration in
  map_groups.json — an empty map group can't be expressed in generated groups.inc
  (established Haven/Ironhold/Sirocco/Emberveil/Schism pattern); register WITH the first
  real Thalvern map. The brief's "map group stub" checklist item is satisfied by this
  deferral (adding an empty group breaks the build). (2) Replace the Galleon boat-menu
  Thalvern stub case in pelagios_boat.inc with a real SailToThalvern handler once
  TidespirePort exists. (3) Heal location for TidespirePort Inn. (4) Region-map x/y
  positions for the 6 MAPSECs.

### ✅ Primalis Isle — CONSTANTS (systems-engineer, 2026-06-14, gmake exit 0)
Systems/constants layer complete. Maps and scripts NOT started (map-builder is next).
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.90% (26,810,440 B). **ZERO new vars.**
- **Flags (13 new, STORY BLOCK 4 0x282-0x28E):** FLAG_PRIMALIS_ARRIVED 0x282,
  GYM1_CLEAR 0x283 (Fern), GYM2_CLEAR 0x284 (Scale), GYM3_CLEAR 0x285 (Thorn),
  GYM4_CLEAR 0x286 (Elder Mako), FLAG_PRIMALIS_TRUST_EARNED 0x287,
  FLAG_PRIMALIS_ORAL_HISTORY_HEARD 0x288, FLAG_PRIMALIS_LENS_MET 0x289,
  FLAG_PRIMALIS_RUINS_FOUND 0x28A, FLAG_PRIMALIS_SEAL_FOUND 0x28B,
  FLAG_PRIMALIS_CIPHER_FOUND 0x28C, FLAG_BEAST_WHISTLE_OBTAINED 0x28D,
  FLAG_PRIMALIS_TOKEN_GIVEN 0x28E. Renamed 13 contiguous FLAG_UNUSED slots (one def each,
  zero external refs). **REUSED pre-allocated (NOT redefined):** FLAG_PRIMALIS_RESOLVED 0x4B2.
  **CIPHER decision:** Primalis is cipher 8 (consistent with the brief). FLAG_CIPHER_8_FOUND
  0x4C4 ALREADY EXISTS (CIPHER_1-9 pre-allocated) — REUSED, not added. FLAG_PRIMALIS_CIPHER_FOUND
  0x28C is the island-local flag; scripts set both at the Heartwood cipher.
  **Hidden items: NONE** — the brief lists no Primalis bg_event hidden items (Beast-Whistle
  undergrowth tiles are field-effect obstacles, not hidden items), so zero hidden-item flags.
  **BLOCK 4 free after Primalis: 0x28F-0x2BB (45 flags)** for Ashenveil/Aetheron/Convergence.
- **Vars: NONE added — explicitly confirmed no new var required (brief says "No additional
  vars needed — 3 spares preserved").** VAR_PRIMALIS_PROGRESS 0x4106 PRE-EXISTED (states
  0=not arrived, 1=arrived, 2=Gym1 Fern, 3=Gym2 Scale + Mako accessible, 4=Gym3 Thorn,
  5=Gym4 Mako + oral history, 6=seal reinforced, 7=resolved). The Mako-trust mechanic folds
  into progress states + FLAG_PRIMALIS_TRUST_EARNED — no counter var. **NO spare consumed;
  genuinely-free spares remain 0x410D/0x410E/0x410F (3).** (0x410B/0x410C are USED by Thalvern
  scripts.) **I did NOT need any new var and am flagging nothing on the var front.**
- **Items (897-898):** ITEM_BEAST_WHISTLE PRE-EXISTED at **879** ("Beast Whistle", icon
  SilphScope — replaces Cut/Sweet Scent; batch-created in the Haven key-item setup) — REUSED,
  NOT redefined. NEW: ITEM_PRIMALIS_TOKEN 897 ("Primal Token", icon ContestPass — Elder Mako's
  carved bone token, proof of Zoan trust, used on the Final Island; key item importance 1
  CannotUse), ITEM_SEAL_SHARD_PRIMALIS 898 ("Seal Shard", icon RedOrb — Grass/Dragon
  seal-shard STUB, not awarded; per brief a plain stub, NOT a Mega trigger). Both in
  include/constants/items.h enum + src/data/items.h struct, charmap-safe names.
  **Next free item ID: 899.**
- **Trainers (925-932, 8 total — 4 Zoan guardians + 4 gym leaders):** parties from
  PRIMALIS_BRIEF.md verbatim. Zoan guardians: TRAINER_ZOAN_PRIMALIS_1 925 (Sera, Cooltrainer F:
  Tropius 46/Budew 47/Roserade 49), _2 926 (Vex, Cooltrainer M: Bagon 47/Flygon 48/Haxorus 50),
  _3 927 (Rael, Cooltrainer M: Exeggcute 49/Tangrowth 50/Kommo-o 52), _4 928 (Cael, Cooltrainer
  M: Goomy 50/Goodra 51/Dragapult 53). Gym leaders: TRAINER_LEADER_PRIMALIS_FERN 929 (Grass,
  Cooltrainer F pic: Tropius 48/Tsareena 49/Rillaboom 50/Decidueye 52), _SCALE 930 (Dragon,
  Cooltrainer M: Haxorus 51/Flygon 52/Drampa 53/Kommo-o 55), _THORN 931 (Grass/Dragon,
  Cooltrainer M: Decidueye 53/Kommo-o 54/Kartana 55/Dragapult 57), _MAKO 932 (Elder Mako,
  Dragon/Water boss, **Hiker pic** — no shark/elder pic exists in the expansion, Hiker chosen
  for a rugged ancient look; Garchomp 55/Sharpedo 56/Kommo-o 57/Dragapult 58/Goodra 60, with
  4x Full Restore). TRAINERS_COUNT_EMERALD 925 -> **933** (**91 free**).
- **BADGES: narrative-only** (DECIDED region-wide). All 4 Primalis gyms set only
  FLAG_PRIMALIS_GYM*_CLEAR. No FLAG_BADGE*_GET (exhausted at 08). Gating uses GYM*_CLEAR /
  VAR_PRIMALIS_PROGRESS.
- **MAPSECs: UNBLOCKED 2026-06-14 — the u8 MAPSEC ceiling is RESOLVED (see "MAPSEC CEILING …
  RESOLVED" under Known Issues).** mapsec_u8_t/metloc_u8_t widened to u16, stored Pokémon
  metLocation repacked to a 9-bit field (ceiling now 508; MAPSEC_NONE still 255 so 253 slots
  free). Kanto was NOT removed (its FRLG content is live).
- MAPSECs added (7): VerdantLanding, JungleRoute1,
  JungleInterior, ZoanVillage, JungleRoute2,
  AncientRuinsCamp, Heartwood — ceiling cleared. Added 2026-06-14 to
  region_map_sections.json (data source of truth) + region_map_entries.h (placeholder
  0,0,1,1 coords — map-builder positions them with the maps); region_map_sections.h
  regenerates from the .json via the Inja template region_map_sections.constants.json.txt.
  MAPSEC_NONE shifted 255 -> 262; METLOC_* sentinels intact at 0x1FD-0x1FF. gmake exit 0.
- **DEFERRED to map-builder:** (1) gMapGroup_Primalis / MAP_GROUP_PRIMALIS registration in
  map_groups.json — an empty map group breaks generated groups.inc (established pattern);
  register WITH the first real Primalis map. The brief's "map group stub" checklist item is
  satisfied by this deferral. (2) Replace the Galleon boat-menu Ashenveil/Primalis stub case
  in pelagios_boat.inc with a real SailToPrimalis handler once VerdantLanding exists.
  (3) Heal location for VerdantLanding Inn. (4) Beast-Whistle obstacle tiles on Route1/Route2
  + the field effect (script-writer/systems). (5) Wild-encounter tables (JungleRoute1/Interior/
  Route2/Heartwood per brief). [(6) Primalis MAPSECs — DONE 2026-06-14, see above.]

### ✅ Convergence — CONSTANTS (systems-engineer, 2026-06-14, gmake exit 0) — FINAL ISLAND
Convergence is the FINAL ISLAND — pure story, three endings, no gyms/badges, no generic
trainers, no inn/heal, no wild encounters, no boat registration (Storm Compass handles
return travel). Systems/constants layer complete. Maps and scripts NOT started. Build clean:
EWRAM 86.46% / IWRAM 86.63% / ROM 80.13% (26,888,116 B). **ZERO new vars, 3 var spares
(0x410D/E/F) PRESERVED.**
- **Flags (8 NEW, STORY BLOCK 4 0x29F-0x2A6):** FLAG_CONVERGENCE_ARRIVED 0x29F,
  FLAG_CONVERGENCE_GATHERING_SEEN 0x2A0, FLAG_SOLLIS_CONFESSION_HEARD 0x2A1,
  FLAG_DORNE_FINAL_BATTLE_DONE 0x2A2, FLAG_ENDING_STOP_PLAYED 0x2A3,
  FLAG_ENDING_HELP_PLAYED 0x2A4, FLAG_ENDING_TRUE_PLAYED 0x2A5,
  FLAG_CONVERGENCE_COMPLETE 0x2A6. Renamed 8 contiguous FLAG_UNUSED slots (collision-
  verified: one def each in flags.h, zero external refs). **REUSED pre-allocated (NOT
  redefined):** FLAG_DORNE_CHOICE_STOP 0x4B5 / HELP 0x4B6 / DEFER 0x4B7,
  FLAG_TRUE_ENDING_UNLOCKED 0x4B8, FLAG_CASS_DEFECTED 0x4BC (the ending-branch determinants
  set on Ashenveil/elsewhere). **No FLAG_CONVERGENCE_RESOLVED exists** — the pre-allocated
  RESOLVED set ends at FLAG_AETHERON_RESOLVED 0x4B4; per the brief FLAG_CONVERGENCE_COMPLETE
  0x2A6 IS the "island resolved / any ending played" flag (brief naming followed; noted as a
  RESOLVED-equivalent). NO new cipher (CIPHER_1-9 complete; True Ending checks
  FLAG_TRUE_ENDING_UNLOCKED). NO hidden items.
- **Vars: NONE added.** VAR_CONVERGENCE_PROGRESS 0x4109 PRE-EXISTED (states 0=not arrived,
  1=arrived+gathering, 2=temple entered, 3=inner sanctum, 4=kill switch chamber, 5=ending
  played). Spares 0x410D/0x410E/0x410F untouched (3 preserved). The brief confirmed no new
  var needed.
- **Items (903):** ITEM_WARDENS_RESEARCH 903 ("Warden Research", importance 1,
  ItemUseOutOfBattle_CannotUse, POCKET_KEY_ITEMS, examine-only; icon gItemIcon_Powder /
  gItemIconPalette_EnergyPowder — copied from ITEM_CASS_DOCUMENTS, a known-good neighboring
  pair). Sollis gives it at the gathering scene. In include/constants/items.h enum +
  src/data/items.h struct, name/description charmap-safe (ASCII hyphens, no straight quotes).
  **Next free item ID: 904.**
- **Trainers (940):** TRAINER_LEADER_DORNE 940 — the game's FINAL battle (Ending 1 / STOP
  path only). Party VERBATIM from CONVERGENCE_BRIEF.md: Bisharp 65 / Hydreigon 66 / Weavile
  67 / Aegislash 68 / Garchomp 69 / Kyurem 70. (Brief's Lv.70 legendary placeholder was
  "Zekrom or Kyurem" — Kyurem picked because Voss already uses Zekrom Lv.64.) Pic: Magma
  Leader Maxie (no custom pic; a commanding-antagonist placeholder per the brief's "fitting
  placeholder" guidance — like the Ironhold/Sirocco leaders). Class Leader, Male, 4x Full
  Restore, Smart Switching AI. TRAINERS_COUNT_EMERALD 940 -> **941** (**83 free**). NO
  generic trainers, NO gym leaders beyond Dorne.
- **BADGES:** N/A — Convergence has no gyms (no badge flags touched).
- **MAP GROUP — DEFERRED to map-builder:** gMapGroup_Convergence / MAP_GROUP_CONVERGENCE is
  NOT registered (an empty map group can't be expressed in generated groups.inc — established
  pattern; register WITH the first real Convergence map). **6 maps** eventual count:
  Convergence_Approach, _AncientCapital_Exterior, _Interior1, _Interior2, _KillSwitchChamber,
  _Epilogue. NO heal location, NO wild encounters, NO boat-menu entry (Storm Compass returns).
- **FLAGGED (pre-existing, not mine):** an UNTRACKED Aetheron script
  (data/maps/Aetheron_CovenantInstallation_Interior/scripts.inc) produces a benign scaninc
  warning "scripts.inc:349 unexpected EOF in string" during the dependency scan. Build still
  links and emits pokeemerald.gba (gmake exit 0). It's a script file (script-writer domain),
  pre-existing, and non-fatal — left untouched.

### ✅ Aetheron Isle — CONSTANTS (systems-engineer, 2026-06-14, gmake exit 0)
Aetheron is the SKY ISLAND (Electric/Flying): reached via the Knock Up Stream + ITEM_SEA_CHART
(FLAG_SEA_CHART_FOUND), NOT by boat tier. Cass defects here. Systems/constants layer complete.
Maps and scripts NOT started (map-builder is next). Build clean: EWRAM 86.46% / IWRAM 86.63% /
ROM 80.08% (26,868,920 B). **ZERO new vars, ZERO var spares consumed.**
- **Flags (8 NEW, STORY BLOCK 4 0x297-0x29E):** FLAG_AETHERON_ARRIVED 0x297,
  FLAG_AETHERON_GYM1_CLEAR 0x298 (Gale), FLAG_AETHERON_GYM2_CLEAR 0x299 (Arc),
  FLAG_AETHERON_GYM3_CLEAR 0x29A (Voss), FLAG_AETHERON_CASS_SEEN 0x29B,
  FLAG_AETHERON_INSTALLATION_FOUND 0x29C, FLAG_AETHERON_SEAL_FOUND 0x29D,
  FLAG_STORM_COMPASS_OBTAINED 0x29E. Renamed 8 contiguous FLAG_UNUSED slots (one def each,
  zero external refs), allocated from 0x297 (right after Ashenveil's 0x296).
- **REUSED pre-allocated, verified existing — NOT redefined:** FLAG_AETHERON_RESOLVED **0x4B4**
  (pre-allocated in the per-island RESOLVED block 0x4B0-0x4B4 — REUSED, not re-added),
  FLAG_CASS_DEFECTED **0x4BC** (ALREADY existed from the original Cass-storyline constants
  setup — REUSED, no duplicate #define). **CIPHER decision: NO new cipher.** The CIPHER_1-9
  set is COMPLETE (Ashenveil took cipher 9 / FLAG_CIPHER_9_FOUND 0x4C5); AETHERON_BRIEF.md
  explicitly states "Cipher 9 already found on Ashenveil — no new cipher here." No
  FLAG_CIPHER_10_FOUND was invented. **Hidden items: NONE** (the Storm Compass / Seal Shard /
  Cass Documents are scripted object/bag pickups, not hidden-item flags).
  **BLOCK 4 free after Aetheron: 0x29F-0x2BB (29 flags)** for Convergence/future.
- **Vars: NONE added — confirmed.** VAR_AETHERON_PROGRESS **0x4108** PRE-EXISTED
  (0=not arrived, 1=arrived, 2=Gym1, 3=Gym2, 4=Gym3+Cass defection, 5=seal reinforced,
  6=resolved). VAR_CASS_RELATIONSHIP **0x40F7** PRE-EXISTED (0=Distant..3=Devoted; set to 3
  on the defection scene). **NO spare consumed; genuinely-free spares remain
  0x410D/0x410E/0x410F (3).**
- **Items (901-902):** ITEM_STORM_COMPASS PRE-EXISTED at **880** ("Storm Compass", icon
  TownMap, replaces Fly / island-select fast travel; batch-created in the Haven key-item setup;
  data block already present, fieldUseFunc = ItemUseOutOfBattle_CannotUse stub — the
  island-select field effect is FUTURE systems work) — REUSED, NOT redefined. NEW:
  ITEM_SEAL_SHARD_AETHERON 901 ("Seal Shard", icon RedOrb — Stormveil Electric/Flying Mega
  trigger stub; not awarded yet beyond the script), ITEM_CASS_DOCUMENTS 902 ("Cass Documents",
  icon Powder/EnergyPowder — lore/examine key item, the Ashenveil evacuation records Cass
  carried since Gildhaven). Both importance 1, CannotUse, charmap-safe names, in
  include/constants/items.h enum + src/data/items.h struct. **Next free item ID: 903.**
- **Trainers (7 NEW, 933-939):** Community guardians TRAINER_GUARDIAN_AETHERON_1 933 (Sael,
  Cooltrainer F), _2 934 (Renn, Cooltrainer M); Covenant officers TRAINER_COVENANT_AETHERON_1
  935 (Mael, Cooltrainer M), _2 936 (Sera, Cooltrainer F); gym leaders (Pic Leader)
  TRAINER_LEADER_AETHERON_GALE 937 (Flying, Cooltrainer F), _ARC 938 (Electric, Cooltrainer M),
  _VOSS 939 (Electric/Steel, Cooltrainer M). Parties verbatim from AETHERON_BRIEF.md; Voss's
  4th mon is Zekrom Lv.64 (the brief's "captured legendary" — Zekrom exists in the expansion so
  the Electivire Lv.64 substitute was NOT needed) and Voss carries 4x Full Restore.
  TRAINERS_COUNT_EMERALD bumped **933 -> 940** (84 free). **BADGES NARRATIVE-ONLY** (region-wide
  policy): the 3 gyms set only FLAG_AETHERON_GYM*_CLEAR, NO FLAG_BADGE*_GET (engine badge flags
  were exhausted at FLAG_BADGE08_GET).
- **MAPSECs added (6):** MAPSEC_AETHERON_KNOCK_UP_STREAM, _CLOUD_LANDING, _SKY_ROUTE,
  _AETHER_VILLAGE, _COVENANT_INSTALLATION, _STORM_PEAK — the 11 maps condense to 6 MAPSECs.
  Added to region_map_sections.json (data source of truth) + region_map_entries.h (placeholder
  0,0,1,1 coords — map-builder positions them with the maps); region_map_sections.h regenerates
  from the .json via the Inja template. **MAPSEC_NONE shifted 267 -> 273** (still far below the
  508 ceiling; METLOC_* sentinels intact at 0x1FD-0x1FF).
- **BOAT MENU — Aetheron verified + Convergence ADDED (two activation gates):**
  - **Aetheron (verified, pre-existing from the Ashenveil pass):** PELAGIOS_ISLAND_AETHERON id
    (10); "AETHERON" entry at **index 10** of MultichoiceList_BoatGalleon; Galleon dispatch
    **case 10 -> Pelagios_EventScript_SailToAetheron**, which does
    `goto_if_unset FLAG_SEA_CHART_FOUND, Pelagios_EventScript_NoSeaChart` then falls through to
    SailNoChart (Aetheron maps don't exist yet). The FLAG_SEA_CHART_FOUND gate is INTACT — not
    broken. Map-builder swaps the post-chart branch to a real MAP_AETHERON_* warp later.
  - **Convergence (NEW this pass — the Ashenveil pass did NOT add it):** added
    PELAGIOS_ISLAND_CONVERGENCE id (11); new "CONVERGENCE" entry at **index 11** of
    MultichoiceList_BoatGalleon; Galleon dispatch **case 11 ->
    Pelagios_EventScript_SailToConvergence**, which does
    `goto_if_unset FLAG_AETHERON_RESOLVED, Pelagios_EventScript_ConvergenceClosed` (shows new
    Pelagios_Text_ConvergenceClosed "the way there is not yet open") then falls through to
    SailNoChart (Convergence maps don't exist yet). **The final island unlocks only after
    FLAG_AETHERON_RESOLVED is set** (Aetheron resolved / Cass defected). Map-builder swaps the
    post-resolve branch to a real MAP_CONVERGENCE_* warp later.
  - Cancel pushed from index 11 to **index 12** in the Galleon list — **list indices and
    dispatch cases verified in sync** (0=Haven, 1=Ironhold, 2=Sirocco, 3=Emberveil,
    4=Schism-N, 5=Schism-S, 6=Thalvern, 7=Gildhaven, 8=Primalis, 9=Ashenveil, 10=Aetheron,
    11=Convergence, 12=Cancel).
- **DEFERRED to map-builder:** (1) gMapGroup_Aetheron / MAP_GROUP_AETHERON registration in
  map_groups.json — an empty map group breaks generated groups.inc (established pattern);
  register WITH the first real Aetheron map (the 11-map group stub). The brief's "map group
  stub (11 maps)" checklist item is satisfied by this deferral + the 6 MAPSECs already added.
  (2) Swap the Aetheron boat-menu post-chart branch to a real SailToAetheron warp once
  MAP_AETHERON_<port> exists, and the Convergence post-resolve branch once MAP_CONVERGENCE_*
  exists. (3) Storm Compass island-select field effect (replaces Fly) is FUTURE systems work
  — the item is a CannotUse stub for now. (4) Heal location (CloudLanding Inn), wild-encounter
  tables (SkyRoute / StormPeak), WEATHER_THUNDERSTORM on outdoor maps, Covenant control panel
  bg_event — all map-builder/script-writer.

### ✅ Ashenveil Isle — CONSTANTS (systems-engineer, 2026-06-14, gmake exit 0)
Ashenveil is the DEAD ISLAND: NO gym leaders, NO trainers, NO inn/heal — pure narrative.
Systems/constants layer complete. Maps and scripts NOT started (map-builder is next).
Build clean: EWRAM 86.46% / IWRAM 86.63% / ROM 80.01% (26,846,760 B). **ZERO new vars,
ZERO new trainers, ZERO var spares consumed.**
- **Flags (8 new, STORY BLOCK 4 0x28F-0x296):** FLAG_ASHENVEIL_ARRIVED 0x28F,
  FLAG_ASHENVEIL_OUTPOST_MET 0x290, FLAG_PHANTOM_LANTERN_OBTAINED 0x291,
  FLAG_ASHENVEIL_COVENANT_DOCS_FOUND 0x292, FLAG_ASHENVEIL_DORNE_MET 0x293,
  FLAG_SEA_CHART_FOUND 0x294, FLAG_MORTHAS_ENCOUNTERED 0x295,
  FLAG_ASHENVEIL_CIPHER_FOUND 0x296. Renamed 8 contiguous FLAG_UNUSED slots (one def each,
  zero external refs). Allocated from 0x28F (picking up right after Primalis's 0x28E).
- **REUSED pre-allocated, verified existing — NOT redefined:** FLAG_DORNE_CHOICE_STOP 0x4B5,
  FLAG_DORNE_CHOICE_HELP 0x4B6, FLAG_DORNE_CHOICE_DEFER 0x4B7 (all three ALREADY existed from
  the original constants setup — reused, no duplicate #define), FLAG_ASHENVEIL_VISITED 0x4B3,
  FLAG_TRUE_ENDING_UNLOCKED 0x4B8. **CIPHER decision:** Ashenveil is cipher 9 (the brief's
  final journal entry). FLAG_CIPHER_9_FOUND 0x4C5 ALREADY EXISTS (CIPHER_1-9 pre-allocated)
  — REUSED, not added; FLAG_ASHENVEIL_CIPHER_FOUND 0x296 is the island-local pair (scripts
  set both at the Morthas cipher), matching the Primalis/Gildhaven convention.
  **Hidden items: NONE** (brief lists no bg_event hidden items; the Sea Chart / Seal Shard /
  documents are scripted bg_event/object pickups, not hidden-item flags).
  **BLOCK 4 free after Ashenveil: 0x297-0x2BB (37 flags)** for Aetheron/Convergence.
- **Vars: NONE added — confirmed.** VAR_ASHENVEIL_PROGRESS 0x4107 PRE-EXISTED
  (0=not arrived, 1=arrived, 2=lantern obtained, 3=dead city reached, 4=Dorne met,
  5=sea chart found, 6=Morthas encountered, 7=visited/complete). VAR_DORNE_RELATIONSHIP 0x40F8
  PRE-EXISTED (set to max on the DORNE_CHOICE_HELP branch). **NO spare consumed; genuinely-free
  spares remain 0x410D/0x410E/0x410F (3).**
- **Items (899-900):** ITEM_PHANTOM_LANTERN PRE-EXISTED at **881** ("Phantom Lantern", replaces
  Defog/Flash in caves; batch-created in the Haven key-item setup) — REUSED, NOT redefined.
  NEW: ITEM_SEA_CHART 899 ("Sea Chart", icon TownMap — found in MorthasGrove (Cass left it);
  FLAG_SEA_CHART_FOUND activates Aetheron in the boat menu; key item importance 1 CannotUse),
  ITEM_SEAL_SHARD_ASHENVEIL 900 ("Seal Shard", icon RedOrb — THE Decidueye Mega Evolution
  trigger per the story bible; Mega wiring is future systems work; stub, not awarded). Both in
  include/constants/items.h enum + src/data/items.h struct, charmap-safe names.
  **Next free item ID: 901.**
- **Trainers: NONE.** Ashenveil has no gyms and no trainers (per brief). opponents.h and
  trainers.party were NOT touched. TRAINERS_COUNT_EMERALD stays **933** (91 free). Badges:
  N/A (no gyms).
- **MAPSECs added (5):** MAPSEC_ASHENVEIL_GREYPORT (covers Greyport + Outpost + Interior),
  _ASH_FIELDS, _DEAD_CITY (covers DeadCity_Exterior + Ruins1 + Ruins2), _MEMORIAL,
  _MORTHAS_GROVE — the 9 maps condense to 5 MAPSECs (Sirocco pattern). Added to
  region_map_sections.json (data source of truth) + region_map_entries.h (placeholder 0,0,1,1
  coords — map-builder positions them with the maps); region_map_sections.h regenerates from
  the .json via the Inja template. **MAPSEC_NONE shifted 262 -> 267** (still far below the
  508 ceiling; METLOC_* sentinels intact at 0x1FD-0x1FF). gmake exit 0.
- **BOAT MENU — NEW sea-chart activation path (different from every prior island):**
  - **Ashenveil:** added PELAGIOS_ISLAND_ASHENVEIL id (9) in pelagios_boat.inc, a new
    "ASHENVEIL" entry to MultichoiceList_BoatGalleon (src/data/script_menu.h) at **index 9**,
    Galleon dispatch **case 9 -> Pelagios_EventScript_SailToAshenveil**, which routes to
    SailNoChart for now (its maps don't exist). Normal Galleon entry, no special gate.
  - **Aetheron:** added PELAGIOS_ISLAND_AETHERON id (10), "AETHERON" entry at **index 10**,
    Galleon dispatch **case 10 -> Pelagios_EventScript_SailToAetheron**. Aetheron is NOT gated
    by boat tier — it is ALWAYS in the static Galleon list (architecture (i), no dynamic list),
    but its handler does `goto_if_unset FLAG_SEA_CHART_FOUND, Pelagios_EventScript_NoSeaChart`
    (shows new Pelagios_Text_NoSeaChart "you have no chart for that heading") and otherwise
    falls through to SailNoChart (Aetheron maps don't exist yet). When the player finds
    ITEM_SEA_CHART in MorthasGrove (FLAG_SEA_CHART_FOUND set), the no-chart refusal lifts.
  - Cancel pushed from index 9 to **index 11** in the Galleon list — list indices and dispatch
    cases verified in sync (0=Haven … 8=Primalis, 9=Ashenveil, 10=Aetheron, 11=Cancel).
- **DEFERRED to map-builder:** (1) gMapGroup_Ashenveil / MAP_GROUP_ASHENVEIL registration in
  map_groups.json — an empty map group breaks generated groups.inc (established pattern);
  register WITH the first real Ashenveil map. The brief's "map group stub" checklist item is
  satisfied by this deferral + the 5 MAPSECs already added. (2) Swap the Ashenveil boat-menu
  stub case to a real SailToAshenveil warp once MAP_ASHENVEIL_GREYPORT exists, and swap the
  Aetheron post-chart branch to a real warp once MAP_AETHERON_* exists. (3) NO heal location
  (intentional — the island has no inn). (4) Phantom Lantern obstacle tiles + field effect in
  AshFields1. (5) Wild-encounter tables (AshFields1 / DeadCity_Exterior / MorthasGrove).
  (6) Per-map dusk/dark palette + WEATHER_FOG_HORIZONTAL. NOTE (RESOLVED 2026-06-14): the
  Primalis-style map-header `region_map_section` truncation warning is FIXED — the emitter
  (tools/mapjson/mapjson.cpp) now writes `.2byte` for the region section, matching the u16
  struct field. See "MAP-HEADER MAPSEC WIDTH FIX" under SAVE-COMPATIBILITY BREAK #2. It was
  NOT harmless: it also silently misaligned the header tail (weather/type/battle-scene) on
  every map; all headers were regenerated, build clean, zero warnings.

### ✅ Gildhaven Isle — CONSTANTS (systems-engineer, 2026-06-13, gmake exit 0)
Systems/constants layer complete. Maps and scripts NOT started (map-builder is next).
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.79% (26,774,668 B). NO new var needed.
- **Flags (12 new, STORY BLOCK 4 0x276-0x281):** FLAG_GILDHAVEN_ARRIVED 0x276,
  GYM1_CLEAR 0x277 (Glint), GYM2_CLEAR 0x278 (Shade), GYM3_CLEAR 0x279 (Lace),
  GYM4_CLEAR 0x27A (Serel), FLAG_GILDHAVEN_DAGAN_MET 0x27B, FLAG_GILDHAVEN_LACE_TALKED 0x27C,
  FLAG_GILDHAVEN_MANOR_ACCESS 0x27D, FLAG_GILDHAVEN_COVENANT_MAP_SEEN 0x27E,
  FLAG_GILDHAVEN_SEAL_FOUND 0x27F, FLAG_GILDHAVEN_CIPHER_FOUND 0x280,
  FLAG_CASS_GILDHAVEN_SEEN 0x281. These renamed 12 contiguous FLAG_UNUSED slots (one def
  each, zero external refs). **REUSED pre-allocated flags (NOT redefined):**
  FLAG_GILDHAVEN_RESOLVED 0x4B1 (pre-existed in the 0x4xx blocks). **CIPHER decision:**
  Gildhaven is cipher 7. FLAG_CIPHER_7_FOUND 0x4C3 ALREADY EXISTS (CIPHER_1-9 pre-allocated)
  — REUSED, not added. FLAG_GILDHAVEN_CIPHER_FOUND 0x280 is the island-local flag; scripts
  set both at the SealChamber cipher. **Hidden items: NONE** — the brief lists no Gildhaven
  hidden items (wild-encounter tables in BlackMarket/SealChamber are map-builder work, not
  bg_event hidden items), so zero hidden-item flags consumed.
- **Vars: NONE added — explicitly confirmed no new var required.** VAR_GILDHAVEN_PROGRESS
  0x4105 pre-existed (states 0=not arrived, 1=arrived, 2=Glint, 3=Shade, 4=Lace, 5=Serel,
  6=seal reinforced, 7=resolved). The corruption mechanic is PURELY progress-gated
  (progress 0-2 = normal NPC dialogue, 3+ = corrupted variant) — it folds into
  VAR_GILDHAVEN_PROGRESS states, needing no counter var. Dagan rides the existing
  VAR_DAGAN_RELATIONSHIP 0x40FB. **NO spare consumed; genuinely-free spares remain
  0x410D/0x410E/0x410F (3).** (0x410B/0x410C are USED by Thalvern scripts.)
- **Items (895-896):** ITEM_VANE_MANOR_KEY 895 ("Manor Key", icon BasementKey/OldKey —
  Lace gives it post-Gym3; unlocks VaneManor + the Exchange passage), ITEM_SEAL_SHARD_GILDHAVEN
  896 ("Seal Shard", icon RedOrb/RedOrb — Fairy/Dark seal-shard STUB, not awarded yet; per
  the brief it MAY be the Serel-Mawile Mega trigger if Mega is implemented, but Mega wiring
  is FUTURE work — item + placeholder is enough now). Both key items, importance 1, CannotUse,
  names charmap-safe (in include/constants/items.h enum + src/data/items.h struct).
  **Next free item ID: 897.**
- **Trainers (917-924, 8 total — Gildhaven has FOUR gyms):** parties from GILDHAVEN_BRIEF.md
  verbatim. Generics (Exchange/Elite guards): TRAINER_GUARD_GILDHAVEN_1 917 (Rael, Cooltrainer
  M), _2 918 (Sera, Cooltrainer F), _3 919 (Morn, Cooltrainer M), _4 920 (Daven, Cooltrainer
  M). Gym leaders: TRAINER_LEADER_GILDHAVEN_GLINT 921 (Fairy, Cooltrainer M pic), _SHADE 922
  (Dark, Cooltrainer M), _LACE 923 (Fairy/Dark, Cooltrainer F per character note), _SEREL 924
  (Fairy/Dark boss, Rich Boy pic per brief). Serel's Mawile Lv.55 is a plain Mawile (brief
  says "Mega, if Mega system implemented" — Mega not wired). TRAINERS_COUNT_EMERALD
  917 -> 925 (**99 free**).
- **BADGES: narrative-only** (DECIDED region-wide). All 4 Gildhaven gyms set only
  FLAG_GILDHAVEN_GYM*_CLEAR. No FLAG_BADGE*_GET (exhausted at 08). Gating uses GYM*_CLEAR /
  VAR_GILDHAVEN_PROGRESS.
- **MAPSECs added (7, covering 13 maps):** MAPSEC_GILDHAVEN_GOLDPORT_HARBOR,
  _MERCHANT_DISTRICT, _BLACK_MARKET, _NOBLE_QUARTER, _VANE_MANOR, _THE_EXCHANGE,
  _SEAL_CHAMBER in region_map_sections.h (enum, before MAPSEC_NONE) + .json +
  region_map_entries.h (placeholder x/y 0,0 1x1 — map-builder positions them). Done in the
  constants pass (Sirocco/Emberveil/Thalvern precedent).
- **DEFERRED to map-builder:** (1) gMapGroup_Gildhaven / MAP_GROUP_GILDHAVEN registration in
  map_groups.json — an empty map group can't be expressed in generated groups.inc
  (established pattern); register WITH the first real Gildhaven map. The brief's "map group
  stub" checklist item is satisfied by this deferral (adding an empty group breaks the build).
  (2) Replace the Galleon boat-menu Gildhaven stub case in pelagios_boat.inc with a real
  SailToGildhaven handler once GoldportHarbor exists. (3) Heal location for GoldportHarbor Inn.
  (4) Region-map x/y positions for the 7 MAPSECs. (5) Wild-encounter tables (BlackMarket:
  Zorua/Snubbull/Mawile; SealChamber: Carbink/Togepi/Ralts) — map-builder/wild_encounters.json.
  (6) Cass glimpse NPC + Covenant map bg_event — map-builder/script-writer.

### ✅ Completed — Schism MAPS (map-builder, 2026-06-13, gmake exit 0)
All 21 Schism maps built, registered, and compiling cleanly (full build exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.57%, 26,699,828 B). Built via
tools/pelagios/build_schism.py (9 composed outdoor layouts) +
build_schism_mapjson.py (all 21 map.json) + build_schism_scripts_stubs.py (stub
scripts.inc). Regenerate via those scripts; do NOT hand-edit map.bin/map.json.
Audit on entry: ZERO Schism map-side work existed (no maps/layouts/generators/
encounters/heal/MAPSEC); only the CONSTANTS layer (flags/vars/items 888-891/
trainers 900-909 incl. Sleet/Eira/Murk/Drenn/dual boat stubs) was present and the
baseline build was clean.

TILESETS / WEATHER per half (per the user's spec):
- NORTH/ice (FrostmarkPort, FrozenTundra, IceCity, IceCave): General +
  gTileset_PelagiosIce, WEATHER_SNOW.
- SOUTH/poison (VenomquayPort, ToxicSwamp, PoisonCity): General +
  gTileset_PelagiosPoison, WEATHER_RAIN.
- MIDDLE (TheScar, ScarRuins): General + gTileset_Cave (grey/ash), WEATHER_NONE.
- Both SealChambers: SEALED_CHAMBER_INNER_ROOM, WEATHER_NONE (North=Glacith,
  South=Toxara; SS_TIDAL decoration + CABLE_CAR apparatus, mirrors Ironhold/Emberveil).
NOTE on custom tilesets: PelagiosIce/Poison are flat-fill (one terrain metatile);
collision/elevation are baked into the LAYOUT WORD (secondary metatile local id L =
global 512+L; word = id | collision<<10 | elevation<<12), exactly like Emberveil
baked collision into General/Lavaridge words. Outdoor layouts are blocky terrain
rectangles - review/dress in Porymap with edge/cliff metatiles later (same caveat as
every prior island).

9 NEW composed outdoor layouts: SCHISM_FROSTMARK_PORT 20x18, SCHISM_FROZEN_TUNDRA
20x32, SCHISM_ICE_CITY 28x24, SCHISM_ICE_CAVE 20x20, SCHISM_THE_SCAR 28x20,
SCHISM_SCAR_RUINS 20x22, SCHISM_VENOMQUAY_PORT 20x18, SCHISM_TOXIC_SWAMP 20x32,
SCHISM_POISON_CITY 28x24.

12 REUSED vanilla layouts (zero new binary): Inn lobbies = POKEMON_CENTER_1F (x2),
Inn rooms = POKEMON_CENTER_2F (x2), City PokeCenters = POKEMON_CENTER_1F (x2),
Barracks/Lab foyers = HOUSE2 (x2), both SealChambers = SEALED_CHAMBER_INNER_ROOM.

TWO SPECIAL INTERIORS:
- IceCity_Barracks_Interior (Gym 1 / Sleet ice-slide puzzle) = vanilla
  SHOAL_CAVE_LOW_TIDE_ICE_ROOM. The custom PelagiosIce tileset has NO MB_ICE
  behavior metatiles, so a slide puzzle CANNOT be built from it; Shoal's ice room
  has real MB_ICE/cracked-ice tiles + a built-in slide puzzle. Sliding is pure
  engine behavior - no triggers needed. Warps at the bottom entrance (12-13,28),
  Sleet at the far top-left (3,2) past the ice, MAP_BATTLE_SCENE_GYM. TODO
  (script-writer): a puzzle reset/hint sign exists at (12,24) if wanted.
- PoisonCity_Laboratory_Interior (Gym 3 Murk lower / Gym 4 Drenn upper) =
  vanilla AQUA_HIDEOUT_1F. Its elev-1 platform catwalks over water read as toxic-vat
  platforms (Murk + Researcher Siv placed at elevation=1 on the lower floor); the
  elev-3 north area reads as Drenn's upper office (Drenn at elev 3, "visible across
  the floor pre-Gym3"). ONE map conveys both levels via the elevation split - no
  second map. Warps at the bottom corridor (12-13,27), MAP_BATTLE_SCENE_GYM.

CONNECTIONS (all reciprocal, offsets recomputed from layout openings):
  FrostmarkPort.up<->FrozenTundra.down (0/0) ;
  FrozenTundra.up<->IceCity.down (-4/4) ; FrozenTundra.right<->TheScar.left (6/-6) ;
  IceCity.right<->IceCave.left (2/-2) ; TheScar.right<->ToxicSwamp.left (-6/6) ;
  VenomquayPort.up<->ToxicSwamp.down (0/0) ; ToxicSwamp.up<->PoisonCity.down (-4/4).
  WARPS (not connections): IceCave->SealChamber_North ; TheScar->ScarRuins ;
  PoisonCity west cave-mouth (1,11)->SealChamber_South (the brief's "poison cave" is
  condensed into this warp - there is no separate PoisonCave map, keeping the count
  at exactly 21).

THE SCAR FACTION BLOCKERS: ice-faction soldier object
  (LOCALID_SCHISM_TUNDRA_SCAR_GUARD) at the FrozenTundra east opening (17,15), and a
  poison-faction guard (LOCALID_SCHISM_SWAMP_SCAR_GUARD) at the ToxicSwamp west
  opening (2,15). Both are blocker OBJECTS with TODO scripts. NO flag-as-var coord
  trigger gate was placed (coord-event var-compare runs VarGet, which returns the
  flag id itself for a flag id and can never fire - CLAUDE.md Known Issues). The
  script-writer gates Scar access on FLAG_EIRA_SCAR_PASS / FLAG_DRENN_SCAR_PASS via
  the guard objects (toggle hide/position), and wires the ceasefire meeting via the
  VAR_SCHISM_CEASEFIRE_PROGRESS==2 coord triggers already placed at TheScar (13-14,11).

WILD ENCOUNTERS (src/data/wild_encounters.json, 12-slot land tables, rate 20, brief
species): gSchismFrozenTundra (Swinub/Snorunt/Cubchoo/Sneasel ~40/30/20/10),
gSchismIceCave (Bergmite/Cryogonal/Jynx ~50/30/20), gSchismTheScar (Sableye/Mawile
50/50), gSchismToxicSwamp (Grimer-Alolan/Croagunk/Mareanie/Foongus ~40/30/20/10).
Used SPECIES_GRIMER_ALOLA (not _ALOLAN). No water/fishing tables (brief defines none);
no tables in ports/cities/barracks/lab/ScarRuins/SealChambers.

HEAL LOCATIONS: HEAL_LOCATION_SCHISM_FROSTMARK_PORT (respawn FrostmarkPort_Inn via
LOCALID_SCHISM_FROSTMARK_INNKEEPER) AND HEAL_LOCATION_SCHISM_VENOMQUAY_PORT (respawn
VenomquayPort_Inn via LOCALID_SCHISM_VENOMQUAY_INNKEEPER). **DEVIATION from the
SCHISM_BRIEF / this session's prompt:** neither was added to
IsLastHealLocationPlayerHouse() in src/heal_location.c. That function TRUE-routes
whiteout to the mom/prof heal path; FALSE-routes to the NURSE path. Pelagios Inns
reuse LAYOUT_POKEMON_CENTER_1F with a NURSE-gfx innkeeper at the nurse spot (7,2), so
the nurse path is correct - exactly why Ironhold/Sirocco/Emberveil Inns are NOT in
that function. The Haven house-heal special-case exists only because it respawns to a
nurse-less HOUSE. Adding the Inns would WRONGLY trigger the mom-heal. The constants
handoff (above) anticipated this ("they are Inns, so they respawn at the Inn... no
house-heal special-case needed"). Verified against Haven's handling.

MAPSECs (8, all 21 maps): MAPSEC_SCHISM_FROSTMARK_PORT, _FROZEN_TUNDRA, _ICE_CITY,
_ICE_CAVE, _THE_SCAR, _VENOMQUAY_PORT, _TOXIC_SWAMP, _POISON_CITY (enum in
region_map_sections.h + .json + region_map_entries.h, placed after EMBERVEIL_VOLCANO).

HIDDEN ITEMS (bg_event type hidden_item, in the hidden-items range as required):
ITEM_NEVER_MELT_ICE at FrozenTundra (4,7) flag FLAG_HIDDEN_ITEM_SCHISM_1 (0x26A);
ITEM_MAX_REVIVE at TheScar (8,14) flag FLAG_HIDDEN_ITEM_SCHISM_2 (0x26B).

BOAT STUB SWAP DONE (data/scripts/pelagios_boat.inc): Pelagios_EventScript_SailToSchismNorth
warps MAP_SCHISM_FROSTMARK_PORT (10,14) and SailToSchismSouth warps
MAP_SCHISM_VENOMQUAY_PORT (10,14), each with the
`goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM[_SOUTH], Pelagios_EventScript_AlreadyHere`
same-island guard + Pelagios_Text_CastOff narration. Both arrival tiles (10,14)
verified walkable against the compiled layouts (the project's port convention, matching
Ironhold/Sirocco/Emberveil). Each port's *_EventScript_Tennyson was given minimal
boarding wiring (lockall / setvar VAR_TEMP_1 / goto Pelagios_EventScript_BoardTennyson) -
boat-system wiring, written into the otherwise-stubbed port scripts.inc, mirroring the
hand-written Emberveil Tennyson. Galleon multichoice list indices 4/5 (SCHISM NORTH/
SOUTH) match the dispatch cases 4/5. Schism is the third Galleon island; tier stays 3.

DEVIATIONS / NOTES for pelagios-script-writer (NEXT):
- All 21 scripts.inc are STUBS (every EventScript label tagged
  "@ TODO (script-writer): implement ..."), EXCEPT the two port Tennyson labels which
  carry real boarding wiring (see boat swap above - leave those alone or only add intro
  flavor before the goto). The stub generator won't clobber a scripts.inc once it has a
  .string line.
- Gym leaders are talk-initiated NPCs (TRAINER_TYPE_NONE): Sleet
  (Barracks_Interior 3,2), Eira (IceCity 14,6), Murk (Lab_Interior 10,21 elev1), Drenn
  (Lab_Interior 12,3). Trainer IDs 906-909 exist. Eira/leaders gate by talking, not sight.
- Sleet/Murk/Drenn/Eira ceasefire cameos: TheScar has two ceasefire-actor objects
  (LOCALID_SCHISM_SCAR_EIRA 5,9 + LOCALID_SCHISM_SCAR_DRENN 22,9) hidden behind
  FLAG_SCHISM_CEASEFIRE; the script-writer reveals/animates them for the meeting cutscene
  (CeasefireScene triggers at 13-14,11 fire at VAR_SCHISM_CEASEFIRE_PROGRESS==2).
- SealChamber discovery triggers: North at VAR_SCHISM_PROGRESS==3 (row 14), South at
  VAR_SCHISM_PROGRESS==5 (row 14). The dual-seal completion / resolution / Ashenveil-gate
  check (all three Galleon islands resolved) is script-writer logic per SCHISM_BRIEF.
- Cipher 5: ScarRuins central pedestal sign (9,11) -> FLAG_SCHISM_CIPHER_FOUND +
  FLAG_CIPHER_5_FOUND (verify FLAG_CIPHER_5_FOUND exists; Haven=1, Ironhold=2, Sirocco=3,
  Emberveil=4, so Schism's ScarRuins = cipher 5).
- Badges are NARRATIVE-ONLY (engine badge flags exhausted at 08): Sleet/Eira/Murk/Drenn
  set only FLAG_SCHISM_GYM1..4_CLEAR + fanfare + "received the X BADGE" text.
- Music is placeholder (MUS_LILYCOVE cities, MUS_ROUTE110 routes, MUS_MT_PYRE Scar,
  MUS_SEALED_CHAMBER caves/chambers, MUS_SLATEPORT ports). Sprites placeholder
  (Eira=COOLTRAINER_F, Drenn/Murk/Siv=SCIENTIST_1/COOLTRAINER_M, Sleet=YOUNGSTER).
- Pre-existing bug fixed during this session: OBJ_EVENT_GFX_SCIENTIST_M does NOT exist
  (briefs say "SCIENTIST_M placeholder"); used OBJ_EVENT_GFX_SCIENTIST_1 (generator +
  regenerated maps). Recorded in agent memory.

### ✅ Completed — Schism SCRIPTS (script-writer, 2026-06-13, gmake exit 0)
All 21 Schism scripts.inc fully implemented - zero TODO placeholders remain (the two
PokemonCenter maps are intentionally text-free: standard nurse via
Common_EventScript_PkmnCenterNurse). Build clean: EWRAM 86.45% / IWRAM 86.63% /
ROM 79.66% (26,730,676 B, +~31 KB scripts). The agent's stream dropped during its final
report; this section was reconstructed and verified by static audit (zero TODOs, clean
link, all branch logic traced) by the orchestrator.

CONSTANT RECONCILIATION: NONE NEEDED. Every flag the task named already existed from the
constants pass - FLAG_EIRA_SCAR_PASS 0x4A2, FLAG_DRENN_SCAR_PASS 0x4A3,
FLAG_SCAR_RUINS_FOUND 0x4A4, FLAG_EIRA_CEASEFIRE_WILLING 0x4A5,
FLAG_DRENN_CEASEFIRE_WILLING 0x4A6, FLAG_DRENN_ALIVE 0x4BB, FLAG_CIPHER_5_FOUND - so the
scripts reference real flags throughout (no item-checkitem substitution was required).

- DUAL ARRIVAL: both FrostmarkPort (north) and VenomquayPort (south) set
  FLAG_SCHISM_ARRIVED + VAR_SCHISM_PROGRESS=1 on arrival. Either port is a valid first
  landing (Galleon menu offers both).
- SCAR GATES: TheScar's two faction blockers gate the FrozenTundra and ToxicSwamp roads.
  North road guard (FrozenTundra) keys off FLAG_EIRA_SCAR_PASS (granted in IceCity by
  Eira); south road guard (ToxicSwamp) off FLAG_DRENN_SCAR_PASS (granted in
  PoisonCity_Laboratory_Interior by Drenn). IceCave's Rael also keys off the ice pass.
- SCARRUINS: discovery + unified white-flash vision -> FLAG_SCAR_RUINS_FOUND. Central
  pedestal resolves cipher 5: FLAG_CIPHER_5_FOUND + FLAG_SCHISM_CIPHER_FOUND + Warden's
  Journal decode text (Haven=1, Ironhold=2, Sirocco=3, Emberveil=4, Schism=5).
- CEASEFIRE WILLINGNESS: Eira and Drenn each set FLAG_EIRA/DRENN_CEASEFIRE_WILLING in
  their post-battle dialogue, gated on FLAG_SCAR_RUINS_FOUND. A shared check
  (CheckBothWilling) arms the TheScar meeting only once BOTH are willing.
- CEASEFIRE MEETING (the island's keystone scene, TheScar): the scene sets
  FLAG_SCHISM_CEASEFIRE up front so the two leader objects (whose hide flag IS
  FLAG_SCHISM_CEASEFIRE) addobject into view. Choreography per the user's spec - both
  walk in from opposite sides, a long deliberate pause, EIRA SPEAKS FIRST
  (SchismTheScar_Text_CeasefireEira), DRENN RESPONDS (CeasefireDrenn), a silence beat
  (CeasefireSilence), neither moves toward the other, both leave separately. No music
  cue. ON_TRANSITION (Schism_TheScar_OnTransition_ShowLeaders) keeps them visible after.
- DUAL-SEAL RESOLUTION (ORDER-INDEPENDENT, deadlock-proof): each SealChamber apparatus
  sets its own *_SEAL_NORTH_DONE / *_SEAL_SOUTH_DONE then `goto
  Schism_EventScript_CheckDualSeals` (the shared check lives in SealChamber_North's
  scripts.inc; the South chamber cross-file gotos it - link-verified). CheckDualSeals
  `goto_if_unset` BOTH done-flags -> OneSealOnly bounce, so resolution only fires when
  the SECOND seal is reinforced regardless of order. It then branches on
  FLAG_SCHISM_CEASEFIRE: set -> ResolutionPeaceful, unset -> ResolutionFailure. Both set
  FLAG_SCHISM_RESOLVED. VAR_TEMP_2 records which chamber was second (0=north,1=south)
  for failure flavor.
- CEASEFIRE FAILURE + DRENN DEATH: if the seals are completed with FLAG_SCHISM_CEASEFIRE
  unset, ResolutionFailure plays the failure path; Drenn's death scene clears
  FLAG_DRENN_ALIVE (default-set; the failure path is the only thing that unsets it).
  Two flavor variants via VAR_TEMP_2 (which side was sealed last).
- ASHENVEIL UNLOCK (STUBBED): resolution checks `goto_if_unset FLAG_THALVERN_RESOLVED`
  and `goto_if_unset FLAG_GILDHAVEN_RESOLVED` -> AshenveilNotYet placeholder. Both
  forward-flags exist (link-clean); the real Ashenveil sea-chart wiring is future work
  when all three Galleon islands are resolved. Order-independent like the Sirocco/
  Emberveil Galleon mirror, so whichever of the three resolves last opens the way.
- BADGES narrative-only (Sleet/Eira/Murk/Drenn set FLAG_SCHISM_GYM1..4_CLEAR + fanfare +
  text, no FLAG_BADGE*). VAR_SCHISM_CEASEFIRE_PROGRESS tracks the player's Drenn-choice
  responses (willingness scale). Speakers EIRA/DRENN/SLEET/MURK (+ faction roles) added
  to pelagios_speaker_names.inc; setspeaker used throughout.

### Schism flow state machine (authoritative)
- VAR_SCHISM_PROGRESS: 0=not arrived, 1=arrived (either port), then advances through the
  ice/poison gym chain and Scar access; the two SealChamber discovery triggers arm at
  the north/south progress values the maps set (North discovery armed ~progress==3,
  South ~progress==5 per the MAPS section). Resolution is the dual-seal choke point, not
  a single linear value - both halves must reach their seal independently.
- FLAG_SCHISM_CEASEFIRE is the success/failure determinant: achieving it BEFORE both
  seals are reinforced yields the peaceful resolution; reinforcing both seals while it's
  unset yields ResolutionFailure + Drenn's death. The willingness flags
  (FLAG_EIRA/DRENN_CEASEFIRE_WILLING) gate whether the meeting can happen at all.

### Schism deferred / known limitations (script side)
- Ashenveil unlock is a stub (Thalvern/Gildhaven unbuilt) - resolving Schism does not
  yet open any new destination; the boat menu still lists those two as no-chart stubs.
- Gym puzzle flavor (ice-slide barracks, toxic-vat lab) is map geometry only; scripts
  treat the leaders as talk-gated battles (Ironhold/Sirocco precedent).
- Placeholder legendary cries: SPECIES_REGIDRAGO (Toxara, south) and the north chamber's
  Glacith cry are stand-ins; sprites/music remain the map-pass placeholders.
- ⚠️ Story-flag space: only 0x4FF remains below the 0x500 trainer-flag wall. The
  systems-engineer MUST open a fresh story-flag region before Thalvern/Gildhaven.

### ✅ Completed — Thalvern MAPS (map-builder, 2026-06-13, gmake exit 0)
All 13 Thalvern maps built, registered, compiling cleanly (full build exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.71%, +~14.5 KB). Built via
tools/pelagios/build_thalvern.py (11 composed layouts + idempotent merge into
layouts.json) + build_thalvern_mapjson.py (all 13 map.json) +
build_thalvern_scripts_stubs.py (stub scripts.inc; the port Tennyson body is REAL
boat wiring, not a stub). Regenerate via those scripts; do NOT hand-edit map.bin/
map.json/scripts.inc. gMapGroup_Thalvern registered (map_groups.json group_order +
map list), 13 `.include` lines in event_scripts.s.

THE 13 MAPS — tileset / weather / layout source:
| Map | Tileset (primary+secondary) | Weather | Layout |
|---|---|---|---|
| TidespirePort | General + PelagiosUnderwater | RAIN | composed (20x18) |
| TidespirePort_Inn | (vanilla POKEMON_CENTER_1F) | NONE | reused |
| TidespirePort_Inn_Interior | (vanilla POKEMON_CENTER_2F) | NONE | reused |
| FloatingMarket | General + PelagiosUnderwater | RAIN | composed (28x24) |
| DexCamp | General + PelagiosUnderwater | RAIN | composed (16x14) |
| CoastalRoute | General + PelagiosUnderwater | **FOG_HORIZONTAL** | composed (20x34) |
| SubmergedRuins_Exterior | General + PelagiosUnderwater | RAIN | composed (24x20) |
| SubmergedRuins_Interior1 | General + PelagiosUnderwater | RAIN | composed (22x22, platform puzzle) |
| SubmergedRuins_Interior2 | General + PelagiosUnderwater | RAIN | composed (22x20, mural) |
| SubmergedRuins_Interior3 | General + PelagiosUnderwater | RAIN | composed (18x16, choice) |
| ThroneRoom | General + PelagiosUnderwater | NONE | composed (**28x40, vast**) |
| CovenantSite | General + PelagiosUnderwater | NONE | composed (22x18) |
| DeepApproach | General + PelagiosUnderwater | NONE | composed (18x28) |

WEATHER NOTE: the brief names "WEATHER_FOG_1" which is NOT a real symbol. Used
**WEATHER_FOG_HORIZONTAL** (=6, the vanilla fog used by Cave of Origin) on
CoastalRoute. All outdoor maps WEATHER_RAIN; ThroneRoom/CovenantSite/DeepApproach
WEATHER_NONE; submerged interiors WEATHER_RAIN (light) per brief. (DeepApproach is
the Covenant approach so it inherits the CovenantSite WEATHER_NONE, not rain — minor
deviation for tonal consistency with the indoor-feeling final stretch.)

TILESET: ALL Thalvern maps that aren't reused vanilla interiors pair
gTileset_General (primary) + gTileset_PelagiosUnderwater (secondary). The brief only
required Underwater on the SubmergedRuins maps + ThroneRoom; I extended it to the
surface maps too (Port/Market/DexCamp/CoastalRoute/Exterior/CovenantSite/DeepApproach)
because the drowned watery look fits the whole Sunken Kingdom and avoids a second
custom palette. Metatiles are flat-fill (secondary id 1, collision/elevation baked
into the layout word, Schism/Emberveil method). Hand-tune richer metatiles (cliffs,
water edges, building stamps, the throne) in Porymap later — current maps are blocky.

SUBMERGEDRUINS_INTERIOR1 PLATFORM PUZZLE (composed, NOT a vanilla reuse): the whole
floor starts as deep water (U_WATER, collision 1, elev 0). Walkable stone platforms
(U_GROUND, collision 0, elev 3) are stamped as stepping-stone islands with narrow
central connectors forming ONE solvable path: south entry apron (8-13,18-20) ->
centre platforms up the x10-11 spine -> north dais (7-14,1-3) where Psalm (Gym 2)
stands. West/east spur platforms are dead-ends over the flood (future Dive / wild-
water flavor). The periodic-submerge tile animation + any Dive/SONAR_LENS logic is
engine + script-writer triggers — the elevation/collision split geometry is baked in.
(Chose a custom underwater layout over reusing AQUA_HIDEOUT_1F: gives a cleaner,
purpose-shaped platform path; documented in agent memory.)

THRONEROOM DESIGN (28x40, the largest Pelagios room so far): a drowned cathedral.
Enter from the south door (warp lands 13/14,38), walk a long central processional
spine (x13-14 kept clear the full 40 tiles) up a wide stone nave (x8-19) flanked by
FLOODED outer aisles (x1-6 west, x21-26 east, all impassable deep water). Twin
colonnades of solid pillars line the nave (rows every 3 tiles) for monumental scale,
with flanking flooded basins partway up to break the walk. At the far north: a raised
throne dais (9-18,2-6) with the throne block (player faces it from the south at
13-14,2-3), the seal apparatus (CABLE_CAR at 13,8) before it, Pelagios (SS_TIDAL
decoration, script 0x0, elev 1, at 13,4 — sealed/non-interactable per the SealChamber
pattern), and Dex (hidden behind FLAG_TEMP_1, for PATH B). The throne sequence fires
from a coord trigger row at y34 (progress 5) on the central spine. All seal-apparatus
/ confrontation tiles validated walkable.

COASTALROUTE VISION TRIGGERS (TODO handoff): 3 coord triggers on the central walkway
tiles ((10,28)/(9,18)/(10,7)) for the atmospheric vision-flash patches. Placed with
**var VAR_TEMP_2 / value 0** as placeholders — the script-writer rewires the gating
(brief wants one-shot-per-tile local flags + a screen flash + a unique ancient-text
line each). A separate inscription sign sits at (8,16). The 2 route trainers (Scholar
Wren, Researcher Holt) are sight trainers.

DEXCAMP NOTE BG_EVENTS (TODO handoff): research notes as 3 examine signs
(ResearchNotes1 5,4 / ResearchNotes2 10,4 / EquipmentCrate 13,8) + Dex NPC (8,5) +
a Lens-at-camp cameo (11,6) hidden behind FLAG_THALVERN_LENS_DEFECTED.

HEAL LOCATION: HEAL_LOCATION_THALVERN_TIDESPIRE_PORT (src/data/heal_locations.json),
respawn at TidespirePort_Inn (4,8 on the port) via LOCALID_THALVERN_TIDESPIRE_INNKEEPER
(NURSE-gfx innkeeper at the nurse's 7,2 spot). **NOT added to
IsLastHealLocationPlayerHouse()** in src/heal_location.c — confirmed correct per the
Schism precedent: that function is ONLY for nurse-less player-house respawns; the Inn's
nurse-gfx innkeeper takes the normal nurse whiteout path. Only Haven Isle Player House
2F belongs in that function.

BOAT STUB SWAP: pelagios_boat.inc Galleon menu **case 6** (THALVERN, index 6 in
MultichoiceList_BoatGalleon) swapped from SailNoChart to a real
**Pelagios_EventScript_SailToThalvern** (same-island guard vs PELAGIOS_ISLAND_THALVERN
[=5] -> Pelagios_EventScript_AlreadyHere; else cast-off + warp MAP_THALVERN_TIDESPIRE_PORT
**10,14**). Arrival tile (10,14) = the central stone dock, verified walkable (col 0);
the arrival coord triggers sit at (9,13)/(10,13) one tile north so the landing tile
itself is not a trigger (warp-landing rule). TidespirePort's *_EventScript_Tennyson is
real boat wiring (setvar VAR_TEMP_1, PELAGIOS_ISLAND_THALVERN -> goto BoardTennyson);
boarding hangs off 5 pier sign bg_events; the SS_TIDAL (12,15, script 0x0, elev 1) is
pure decoration.

WILD ENCOUNTERS (src/data/wild_encounters.json, all **water_mons** per the brief's
surf/flooded/deep-water framing; 5 slots, rates 60/30/5/4/1):
  gThalvernCoastalRoute (Staryu/Chinchou/Frillish x2/Inkay, L40-44),
  gThalvernSubmergedRuinsInterior1 (Slowpoke/Beheeyem/Wynaut x2/Slowpoke, L44-48),
  gThalvernSubmergedRuinsInterior2 (Frillish/Slowbro/Claydol x2/Frillish, L46-50).
  DEVIATION: the brief's 40/30/20/10 splits don't map cleanly onto the 5-slot water
  rates; approximated (e.g. Frillish doubled into slots 2-3 for the 20% band). As with
  Schism/Emberveil composed layouts, the flat-fill secondary metatiles carry NORMAL
  behavior, so these tables only fire where the engine sees surf/water behavior — the
  flooded sections need Dive/Surf-behavior metatiles, a Porymap follow-up. Tables are
  attached per the established precedent.

CONNECTIONS (all reciprocal, offsets validated negated):
  TidespirePort.up(-4)<->FloatingMarket.down(+4) ;
  FloatingMarket.up(+6)<->DexCamp.down(-6) ;
  FloatingMarket.right(-5)<->CoastalRoute.left(+5) ;
  CoastalRoute.up(-2)<->SubmergedRuins_Exterior.down(+2) ;
  CovenantSite.down(+2)<->DeepApproach.up(-2).
  The ruins chain (Exterior->Interior1->Interior2->Interior3->ThroneRoom) and the
  Covenant route (CovenantSite->DeepApproach->Interior3) link by WARPS, not connections
  (Interior3 has a dedicated DeepApproach-landing warp pair at 8/9,14).

MAPSEC POSITIONS (region_map_entries.h): the 6 Thalvern MAPSECs were 0,0 placeholders
(same as every prior Pelagios island — the Hoenn region map has no Pelagios space).
I gave them a distinct non-overlapping cluster (TidespirePort 3,12 / FloatingMarket
3,11 / CoastalRoute 4,11 / SubmergedRuins 5,11 / CovenantSite 5,12 / ThroneRoom 6,12)
so they don't stack. They don't display in normal play (Pelagios uses boat travel, not
Fly); cosmetic only. (Deviation from the 0,0 precedent — harmless; revert to 0,0 if a
real Pelagios region map is ever authored.)

DEVIATIONS / NOTES for pelagios-script-writer (NEXT):
- All 13 scripts.inc are STUBS (every EventScript label tagged "@ TODO (script-writer):
  implement ...") EXCEPT the real TidespirePort Tennyson boarding body. Replace with
  real content per THALVERN_BRIEF.md (Key Scripts + NPC Dialogue + Gym Leaders sections).
  The stub generator won't clobber a scripts.inc once it has a .string line.
- GYM LEADERS are talk-initiated overworld NPCs (TRAINER_TYPE_NONE, IDs 914-916):
  Tide (FloatingMarket 8,8, outside his warehouse), Psalm (Interior1 north dais 10,2),
  Lens (Exterior 11,9 — also a defection cameo in Interior2 13,6 hidden behind
  FLAG_THALVERN_GYM3_CLEAR, and at DexCamp 11,6 behind FLAG_THALVERN_LENS_DEFECTED).
  Set FLAG_THALVERN_GYM[1-3]_CLEAR; badges narrative-only (engine slots exhausted).
- THE CHOICE (Interior3): coord triggers at (8,3)/(9,3) (progress 4) -> ChoiceScene;
  Lens (8,8) delivers the warning, Dex (9,12, hidden FLAG_TEMP_1) is "right behind".
  FLAG_DEX_ALIVE = TRUE on "I'll go" (player first, PATH A); stays FALSE on "Let Dex
  go" (PATH B). PATH B gives ITEM_DEX_NOTES (894) at the Throne sequence.
- THRONE ROOM (progress 5): trigger row y34 -> ThroneSequence (PATH A recovery vs PATH
  B Dex collapse). Apparatus (13,8) reinforces the seal -> FLAG_THALVERN_RESOLVED +
  FLAG_THALVERN_CIPHER_FOUND + FLAG_CIPHER_6_FOUND, ITEM_SEAL_SHARD_THALVERN (893,
  Feraligatr Mega, currently a stub — give it here). Pelagios cry = pick a placeholder
  species (e.g. SPECIES_KYOGRE). Inscription sign (10,7) = cipher 6 lore.
- ASHENVEIL 3-ISLAND UNLOCK: after FLAG_THALVERN_RESOLVED, check Schism + Thalvern +
  Gildhaven all resolved -> unlock Ashenveil in the boat menu (boat-menu wiring not yet
  built for Ashenveil; leave the check + a TODO). Galleon tier is already set (3) from
  the parallel islands, so no VAR_BOAT_TIER change here.
- NUMA VESS (CovenantSite 10,6): refuses to battle, then leaves; FLAG_NUMA_VESS_CONFRONTED.
  Accessible after Gym 3 via the Lens's ITEM_COVENANT_ACCESS_CARD (892) — the script-
  writer gates DeepApproach/CovenantSite entry on it.
- Interior2 east-passage Blocker (16,7) faces the doorway gap at (17,7-8) into the side
  pocket (18-20,6-9); gate it on Covenant access. Central mural (10,6) = the extended
  vision; side murals (5,6)/(15,6) = flavor.
- Add Tide, Psalm, Lens, Dex, Numa Vess to pelagios_speaker_names.inc.

### ✅ Completed — Thalvern SCRIPTS (script-writer, 2026-06-13, gmake exit 0)
All 13 Thalvern scripts.inc fully implemented — zero TODO placeholders remain.
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.79% (26,772,012 B).

CONSTANT RECONCILIATIONS (all verified against headers, nothing invented):
- Caller name: brief's resolution PokeNav says "Solaris"; CLAUDE.md + every other
  island use Pelagios_Speaker_Sollis (Professor Maren Sollis). Used SOLLIS for
  consistency — "Solaris" treated as a brief typo.
- PATH DETERMINANT carrier: needed a persistent flag/var to record the Interior3
  choice that the ThroneRoom reads, WITHOUT setting FLAG_DEX_ALIVE early. Used the
  pre-existing reserved spare VAR_PELAGIOS_RESERVED_0x410B (0=unset, 1=PATH A player-
  first, 2=PATH B Dex-first). No vars.h edit — it already existed as a reserved spare.
- Day counter: VAR_PELAGIOS_RESERVED_0x410C (the second reserved spare), advanced
  1->2->3 across the three recovery beats. Both reserved spares are transient/single-
  use here; future islands may reuse them freely (they reset / are read once).
- Gym TMs: NO Surf TM (Surf is HM03) and NO Scald TM exist in the 50-TM vanilla set.
  Tide -> ITEM_TM_WATER_PULSE (only offensive Water TM, "Surf equiv"), Psalm ->
  ITEM_TM_PSYCHIC (exact), Lens -> ITEM_TM_RAIN_DANCE (Water support, for "Scald").
  All three lampshaded in dialogue (Sirocco/Emberveil/Schism precedent).
- Pelagios cry placeholder = SPECIES_KYOGRE (Water legendary) in all throne/seal beats.
- ITEM_WARDENS_JOURNAL (874) confirmed as the exact journal item the Dex checkitem
  branch uses. ITEM_SONAR_LENS (878, Dive replacement) already existed — not given
  on Thalvern (no Dive-gating built into the reused interiors; deferred).

PATH A / PATH B — branch determinant + mutual exclusivity:
- The Interior3 choice scene (coord trigger (8/9,3), VAR_THALVERN_PROGRESS==4) sets
  VAR_PELAGIOS_RESERVED_0x410B to 1 (YES, "I'll go in.") or 2 (NO, "Let Dex go
  ahead."), sets FLAG_THALVERN_THRONE_CHOICE + FLAG_THALVERN_SEAL_FOUND, advances
  progress to 5. Dex appears as a FLAG_TEMP_1 cameo for the scene, then is removed.
- The ThroneRoom sequence (coord trigger (13/14,34), VAR_THALVERN_PROGRESS==5)
  branches: `goto_if_eq VAR_PELAGIOS_RESERVED_0x410B, 2 -> PathB`, else PathA. ONE
  var, ONE branch -> exactly one path can ever play. Both paths end at progress 6
  with the seal apparatus armed.
- FLAG_DEX_ALIVE is set in EXACTLY ONE place: inside ThalvernThroneRoom_EventScript_
  PathA, after the three-day recovery completes (player wakes). It is never set,
  cleared, or referenced anywhere in PATH B. Confirmed by grep: the only `setflag
  FLAG_DEX_ALIVE` is in PathA's wake beat. PATH B leaves it FALSE.

THREE-DAY RECOVERY (most complex sequence): rendered IN-PLACE over a held black
screen inside PathA (cannot continue a script after warp+waitstate; DexCamp has no
bed/recovery hooks and map.json is generator-owned, so no actual relocation). After
collapse (fadescreen FADE_TO_BLACK), VAR_PELAGIOS_RESERVED_0x410C counts 1->2->3;
each value `call`s RecoveryDay which `goto_if_eq`s to that day's beat (scene
narration + Dex's day line) then returns. Day 1 "you've been out a day", Day 2 "Psalm
says it's normal. Ish.", Day 3 "the Lens brought food. We're all here." Player wakes,
brief "What did you see?" exchange + Dex's "they saw this too, they knew" line, then
SET FLAG_DEX_ALIVE, fadescreen FADE_FROM_BLACK back into the ThroneRoom (player never
moved), progress 6. Then the seal apparatus reinforces.

CIPHER 6 (both paths): ThalvernThroneRoom_EventScript_Cipher6 is a shared `call`
(guarded by FLAG_THALVERN_CIPHER_FOUND) invoked by BOTH PathA (after the parent-flash
vision beat) and PathB (after Dex's collapse, from his scattered decoded notes). Sets
FLAG_CIPHER_6_FOUND + FLAG_THALVERN_CIPHER_FOUND, same journal text verbatim incl. the
encoded "I saw my successor in the vision" section that ties to PATH A's parent flash.
Per the brief the cipher resolves in the Throne Room (NOT the Interior2 mural — that
mural is the standalone "longest vision so far", no cipher flag).

SEAL REINFORCEMENT / RESOLUTION (apparatus at (13,8), armed progress>=6): Pelagios
ACKNOWLEDGMENT only (presence/recognition, fadescreen + Kyogre cry — explicitly NOT a
vision, distinct from the mural/throne visions), the ruins stop sinking (narration),
PokeNav call to Sollis with TWO variants on FLAG_DEX_ALIVE (alive: "they weren't being
poetic", tell Dex I'm glad; dead: "Dex... use his notes, that's all he'd want"). Sets
FLAG_THALVERN_RESOLVED + progress 7, awards ITEM_SEAL_SHARD_THALVERN (Feraligatr Mega
trigger) LAST so a full bag can't block resolution. VAR_BOAT_TIER untouched (already
Galleon).

ASHENVEIL 3-ISLAND STUB (order-independent, mirrors Schism_EventScript_AshenveilCheck):
after resolution, ThalvernThroneRoom_EventScript_AshenveilCheck does
`goto_if_unset FLAG_SCHISM_RESOLVED -> NotYet` / `goto_if_unset FLAG_GILDHAVEN_RESOLVED
-> NotYet` -> "way to the dead island stays closed" text; if all three set -> "open at
last" text. Gildhaven isn't built so this is the standard stub. Schism's mirror already
checks FLAG_THALVERN_RESOLVED; whichever of the three resolves last triggers the open
text. No VAR_BOAT_TIER change (Ashenveil uses a sea-chart, future work).

LENS DEFECTION cameo (3-stage, Sirocco/Schism cameo pattern): Exterior Lens = Gym 3
battle (gated GYM2_CLEAR), gives Depth Badge + TM Rain Dance, sets GYM3_CLEAR +
progress 4, then removed every load via Exterior ON_TRANSITION (after GYM3_CLEAR) —
he points the player INSIDE. Interior2 Lens (hide flag FLAG_THALVERN_GYM3_CLEAR =
visible after Gym 3) is the quiet defection: gives ITEM_COVENANT_ACCESS_CARD (892),
sets FLAG_THALVERN_LENS_DEFECTED, walks off + removeobject; Interior2 ON_TRANSITION
removes him after defection. DexCamp Lens (hide flag FLAG_THALVERN_LENS_DEFECTED =
appears after defecting) is his permanent home, with alive/dead post-resolution text.

NUMA VESS (CovenantSite (10,6)): pure dialogue scene, NO trainerbattle — she refuses
("I don't dirty my hands. I file reports..."), sets FLAG_NUMA_VESS_CONFRONTED, walks
off (set_invisible), CovenantSite ON_TRANSITION removes her permanently after.

BADGES: NARRATIVE-ONLY for all 3 (Tide/Psalm/Lens) — fanfare + "received the X Badge"
text + FLAG_THALVERN_GYM*_CLEAR only, NO FLAG_BADGE*_GET (engine slots exhausted at 08).

VISION PATCHES (CoastalRoute): the 3 coord triggers all gate VAR_TEMP_2==0 (never set
on this map = always armed); each patch self-guards with its own FLAG_TEMP bit
(FLAG_TEMP_2/3/4) so it plays once per map visit (session-local, per the brief's "local
flag"). setspeaker 0 throughout (narration). Different ancient-text line per tile.

KEY DEX RULE HONORED: no Dex line on the island foreshadows his death — he is warm,
brilliant, optimistic throughout (first-meeting / met / post-mural / pre-throne /
resolved-alive variants). Death (PATH B) is quiet: he simply lies down and doesn't
wake; Psalm's "he'll live, won't wake up" diagnosis is wrong (he never wakes).

DEX MULTI-STAGE (DexCamp): pre-met (+ ITEM_WARDENS_JOURNAL checkitem variant) ->
FLAG_THALVERN_DEX_MET; met; post-mural (cipher found); pre-throne (Gym3 clear);
resolved-alive. DexCamp ON_TRANSITION removeobjects the Dex object when resolved-AND-
dead (his map.json object has no hide flag), so his absence is permanent on PATH B;
research-notes signs and the DexCamp Lens cameo also have alive/dead post-res variants.

DEFERRED / KNOWN LIMITATIONS (script side):
- Recovery is in-place montage, not a literal warp to a DexCamp bed (engine + map.json
  constraints). Functionally identical; player sees only black + day text.
- Gym puzzles (Tide currents, Psalm/Interior1 platform-flood, Lens) are simplified to
  talk-gated battles — the reused vanilla interiors have no current/platform objects.
- Interior2 east-passage Blocker is flavor only (checkitem ITEM_COVENANT_ACCESS_CARD
  branch); the real Covenant gate is narrative + the DeepApproach sight trainers.
- Full-bag at the Seal Shard / Dex Notes beats: key items always fit; no bag-full
  branch needed. Gym TMs use the goto_if_eq VAR_RESULT FALSE bag-full fallback.
- VAR_PELAGIOS_RESERVED_0x410B/0x410C are consumed transiently by Thalvern; document
  before a later island reuses them mid-Thalvern-playthrough (non-issue post-resolution).

### Thalvern flow state machine (authoritative)
- VAR_THALVERN_PROGRESS: 0=not arrived, 1=arrived (dock scene done; arrival is a
  coord trigger at (9/10,13) gating progress==0), 2=Tide beaten, 3=Psalm beaten,
  4=Lens beaten (Gym 3; Interior3 choice trigger armed), 5=throne choice made / Throne
  Room entered (ThroneRoom sequence trigger armed), 6=throne sequence done either path
  (seal apparatus armed), 7=resolved (FLAG_THALVERN_RESOLVED, Seal Shard given).
  Linear island — each gym sets the next value unconditionally (no nonlinear guard
  needed, unlike Sirocco/Schism).
- PATH determinant: VAR_PELAGIOS_RESERVED_0x410B (1=PATH A player-first, 2=PATH B
  Dex-first), set at the Interior3 choice, read by the ThroneRoom. Mutually exclusive.
- FLAG_DEX_ALIVE: set in EXACTLY ONE place — PathA's recovery wake beat. Never touched
  on PATH B. TRUE = Dex survives (DexCamp object stays; alive post-res variants); FALSE
  = Dex dead (DexCamp object removed by ON_TRANSITION; dead post-res variants).
- VAR_PELAGIOS_RESERVED_0x410C: PATH A three-day recovery day counter (1->2->3).
- FLAG_TEMP_1: Dex cameo hide flag on Interior3 + ThroneRoom (re-set by each map's
  ON_TRANSITION). FLAG_TEMP_2/3/4: CoastalRoute per-patch once-guards. FLAG_TEMP_5:
  Interior2 mural once-guard.

### ✅ Completed — Gildhaven MAPS (map-builder, 2026-06-14, gmake exit 0)
All 13 Gildhaven maps built, registered, compiling cleanly (full build exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.81%, 26,781,160 B). Built via
tools/pelagios/build_gildhaven.py (4 composed outdoor layouts + idempotent merge into
layouts.json) + build_gildhaven_mapjson.py (all 13 map.json) +
build_gildhaven_scripts_stubs.py (stub scripts.inc; the port Tennyson body is REAL boat
wiring, not a stub). Regenerate via those scripts; do NOT hand-edit map.bin/map.json/
scripts.inc. gMapGroup_Gildhaven registered (group 81; map_groups.json group_order + map
list), 13 `.include` lines in event_scripts.s.

SECONDARY TILESET: gTileset_Lilycove (paired with gTileset_General primary) for ALL
composed outdoor maps. Chosen over gTileset_Slateport for the wealthy/glittering big-city
look — Lilycove has ornate fancy paving (0x3124) and richer building fronts that read as a
gold-trimmed trading port; Slateport's market-stall tiles suit a working port, not a
wealthy one. Outdoor metatile words sampled verbatim from LAYOUT_LILYCOVE_CITY (a
vanilla General+Lilycove pairing) so collision/elevation come for free. Gildhaven is
ENTIRELY URBAN — no custom Pelagios tileset.

THE 13 MAPS — secondary tileset / weather / layout source:
| Map | Secondary tileset | Weather | Layout |
|---|---|---|---|
| GoldportHarbor | Lilycove | SUNNY | composed (20x18) |
| GoldportHarbor_Inn | (vanilla POKEMON_CENTER_1F) | NONE | reused |
| GoldportHarbor_Inn_Interior | (vanilla POKEMON_CENTER_2F) | NONE | reused |
| MerchantDistrict | Lilycove | SUNNY | composed (28x24) |
| MerchantDistrict_PokemonCenter | (vanilla POKEMON_CENTER_1F) | NONE | reused |
| BlackMarket | (vanilla SEALED_CHAMBER_OUTER_ROOM) | NONE | reused (hidden den) |
| NobleQuarter | Lilycove | SUNNY | composed (24x20) |
| NobleQuarter_VaneManor | (vanilla MOSSDEEP_CITY_SPACE_CENTER_1F) | NONE | reused |
| NobleQuarter_VaneManor_Interior | (vanilla MOSSDEEP_CITY_SPACE_CENTER_2F) | NONE | reused |
| TheExchange_Exterior | Lilycove | SUNNY | composed (20x16) |
| TheExchange_Interior1 | (vanilla LILYCOVE_MUSEUM_1F) | NONE | reused |
| TheExchange_Interior2 | (vanilla LILYCOVE_MUSEUM_2F) | NONE | reused (Gym 4) |
| TheExchange_SealChamber | (vanilla SEALED_CHAMBER_INNER_ROOM) | NONE | reused |

INTERIOR REUSE NOTE: the brief suggested the FRLG Pokemon Mansion for VaneManor, but the
FRLG LAYOUT_POKEMON_MANSION_* binaries DO NOT link into the Emerald build (their layout
data is only emitted for FRLG-region maps — link failed with "undefined reference to
PokemonMansion_1F_Layout"). Swapped to MOSSDEEP_CITY_SPACE_CENTER_1F/2F (the multi-room
wealthy palace interior used by Sirocco's DaganPalace) — warp coords mirror that precedent
(foyer entry (7,9)/(8,9), up-stair (13,1); study down-stair (13,1)). RULE for future
islands: prefer Hoenn-region vanilla interiors; FRLG-region layouts (Pokemon Mansion,
Celadon Dept Store, Kanto gyms) won't link.

SEALCHAMBER URBAN-vs-ANCIENT CONTRAST: TheExchange_SealChamber reuses
SEALED_CHAMBER_INNER_ROOM (ancient stone) — deliberately at odds with the gilt Lilycove
Exchange floors above it, per the brief's key visual beat. Mirrors the established
Ironhold/Schism/Thalvern SealChamber pattern EXACTLY: Mirath as SS_TIDAL decoration
(script 0x0) at (10,5), seal-reinforcement apparatus CABLE_CAR at (10,8), discovery
coord triggers on row 14 (cols 9-11, VAR_GILDHAVEN_PROGRESS==5), warp back to Interior2 at
(10,19)/(11,19). NOTE per brief: Gildhaven's seal leaks NATURALLY (no Covenant siphon) —
the apparatus is the player's reinforcement point, NOT machinery to disable first. That
distinction is DIALOGUE (script-writer), not geometry — the apparatus object is identical
to other islands; the script must NOT add a "disable the siphon" step.

KEY NPC / bg_event PLACEMENTS (all TODO stubs for the script-writer):
- Cass GLIMPSE at GoldportHarbor: object GFX RIVAL_MAY_NORMAL at (9,2), hide flag
  FLAG_CASS_GILDHAVEN_SEEN, label GildhavenGoldportHarbor_EventScript_CassGlimpse. Arrival
  coord triggers (9,13)/(10,13) at VAR_GILDHAVEN_PROGRESS==0 fire the arrival cutscene +
  Cass walk-off. Script-writer: walk Cass off and set FLAG_CASS_GILDHAVEN_SEEN so she hides.
- Cass NOBLE-QUARTER one-time scene: second RIVAL_MAY object at NobleQuarter (11,5), same
  hide flag, label ..._EventScript_CassWarning; coord triggers (11,8)/(12,8) (placeholder
  var VAR_TEMP_2) label ..._EventScript_CassScene. Script-writer rewires the once-guard
  (FLAG_CASS_GILDHAVEN_SEEN) and the "Leave. Please." beat.
- Dagan in BlackMarket: RICH_BOY object at (10,13), HIDE FLAG = FLAG_GILDHAVEN_GYM1_CLEAR
  (so he only appears after Gym 1, per brief), label GildhavenBlackMarket_EventScript_Dagan.
  Also a flavor Dagan-in-cafe object at MerchantDistrict (20,8)
  (..._EventScript_DaganCafe) — waves, won't engage until Gym 1. VAR_DAGAN_RELATIONSHIP
  increments at the BlackMarket meeting (script-writer).
- Covenant MAP bg_event: TheExchange_Interior2 sign at (11,3),
  GildhavenTheExchangeInterior2_EventScript_CovenantMap (script-writer sets
  FLAG_GILDHAVEN_COVENANT_MAP_SEEN + the yield-map lore reveal).
- Lace Vane: TWO objects — MerchantDistrict (23,4) watching the noble-quarter entrance
  (..._EventScript_LaceWatching, won't speak until Gym 2) AND NobleQuarter (18,9) as the
  Gym 3 leader (..._EventScript_Lace). A third hidden cameo at TheExchange_Interior2 (14,5)
  (hide flag FLAG_TEMP_1) for the Serel post-battle moment (..._EventScript_LacePostBattle).
- ALL 4 GYM LEADERS are TRAINER_TYPE_NONE (talk-initiated so the script-writer can
  flag-gate): Glint MerchantDistrict (13,11), Shade MerchantDistrict (5,16), Lace
  NobleQuarter (18,9), Serel TheExchange_Interior2 (11,5). Serel also has a flavor
  "watching from the upper level" object in Interior1 (10,2).
- 4 Exchange GUARD trainers (sight, TRAINER_TYPE_NORMAL): Rael+Sera at Exterior (7,6)/(12,6),
  Morn+Daven at Interior1 (6,8)/(14,8). GFX GENTLEMAN (no GUARD gfx exists).
- The hidden BlackMarket entrance: MerchantDistrict warp at (5,7) (walkable, below the
  silk-stall wall block) + a hint sign at (7,7) (..._EventScript_SilkStall). The
  VaneManor->Exchange hidden passage is the warp at VaneManor_Interior (2,8) <->
  TheExchange_Exterior (1,14) (two-way).

HEAL LOCATION: HEAL_LOCATION_GILDHAVEN_GOLDPORT_HARBOR (src/data/heal_locations.json),
respawn at GoldportHarbor_Inn via LOCALID_GILDHAVEN_HARBOR_INNKEEPER, on-map respawn tile
(4,8) (mirrors Thalvern). NOT added to IsLastHealLocationPlayerHouse() — that function is
only for nurse-less player-house respawns (Schism/Thalvern precedent); the Inn uses a
nurse-graphics innkeeper and takes the normal nurse path automatically.

BOAT STUB SWAP: data/scripts/pelagios_boat.inc case 7 (MULTI_BOAT_GALLEON index 7,
"GILDHAVEN" — the last island entry before CANCEL) swapped from SailNoChart to the real
Pelagios_EventScript_SailToGildhaven (same-island guard goto_if_eq VAR_TEMP_1,
PELAGIOS_ISLAND_GILDHAVEN -> AlreadyHere; warp MAP_GILDHAVEN_GOLDPORT_HARBOR, 10, 14).
PELAGIOS_ISLAND_GILDHAVEN (=6) already existed. ARRIVAL TILE (10,14) verified walkable
(word 0x3124, col 0) — the central stone dock. GoldportHarbor's *_EventScript_Tennyson
sets VAR_TEMP_1 = PELAGIOS_ISLAND_GILDHAVEN (the stub generator wrote this real wiring).
Boarding via 4 sign bg_events on the dock spine ((8,13)/(8,14)/(11,13)/(11,14)); the
SS_TIDAL at (10,15) is pure decoration (script 0x0), never the boarding object.

WILD ENCOUNTERS (src/data/wild_encounters.json, 12-slot land tables, rate 8 — low, urban
hidden areas only, no outdoor routes): gGildhavenBlackMarket (Zorua x6 / Snubbull x4 /
Mawile x2, ~Lv30-37) and gGildhavenSealChamber (Carbink x6 / Togepi x4 / Ralts x2,
~Lv48-53 — Ralts is the Haven-Isle-ruins callback the brief wants). NO tables on the two
harbors, MerchantDistrict, NobleQuarter, VaneManor, or Exchange floors 1-2 (no entry =
no spawns). NO hidden items placed anywhere on Gildhaven (brief: none).

MAPSECs: the 7 Gildhaven MAPSECs (GOLDPORT_HARBOR / MERCHANT_DISTRICT / BLACK_MARKET /
NOBLE_QUARTER / VANE_MANOR / THE_EXCHANGE / SEAL_CHAMBER) remain UNPOSITIONED (null x/y),
matching EVERY prior Pelagios island — no island has region-map coords yet (the custom
world map / nautical chart is a Not-Started item). Position all Pelagios MAPSECs together
when that custom map is built; fabricating Hoenn coords now would conflict with it.

CONNECTIONS (reciprocal, validated against compiled collision bits): only
GoldportHarbor.up(-4) <-> MerchantDistrict.down(+4) is a true map connection (gaps align:
Harbor north cols 9-10 == Merchant south cols 13-14). Everything else links by WARP
(urban arches/doors): MerchantDistrict<->NobleQuarter (NE arch, MD warp2 (25,2) <->
NobleQuarter warp2/3 (11,18)/(12,18)); MerchantDistrict->BlackMarket (alley); NobleQuarter
->VaneManor (gated post-Gym3); VaneManor_Interior->TheExchange_Exterior (hidden passage,
two-way); Exchange Exterior->Interior1->Interior2->SealChamber (stairs).

DEVIATIONS / NOTES for the script-writer (pelagios-script-writer is NEXT):
- All 13 scripts.inc are STUBS (every EventScript label tagged "@ TODO (script-writer):
  implement ..."; the Tennyson body is the one exception — real boat wiring). The stub
  generator won't clobber a scripts.inc once it has a .string line.
- Gym puzzles (Glint's buy/sell-stall gates, Shade's reduced-visibility maze) are NOT built
  as map geometry — reused/blocky layouts have no such mechanics. Simplify to talk-gated
  battles (established Petra/Cinder/Tide pattern) or add objects in a later pass.
- CORRUPTION MECHANIC is dialogue-only: MerchantDistrict/NobleQuarter NPCs branch on
  VAR_GILDHAVEN_PROGRESS (0-2 normal, 3+ corrupted) — no map/geometry support needed.
- Vane Manor is locked until post-Gym3: gate the NobleQuarter manor warp (10,5)/(13,5) on
  FLAG_GILDHAVEN_MANOR_ACCESS / ITEM_VANE_MANOR_KEY (Lace gives both post-battle). No
  physical lock object built — script-side bounce, mirror the Ironhold rubble pattern.
- The SealChamber hidden-stairs warp (Interior2 warp2 at (11,1)) should be gated until
  Serel is defeated (FLAG_GILDHAVEN_GYM4_CLEAR) — script-side, no geometry lock.
- Composed outdoor layouts are blocky Lilycove rectangles (fancy paving + wall blocks); warp
  tiles sit on plain paving. Review in Porymap and dress with building/door/stall metatiles
  later (same caveat as every prior island).
- MUSIC placeholders: MUS_SLATEPORT (port), MUS_LILYCOVE (city/market), MUS_LILYCOVE_MUSEUM
  (noble quarter), MUS_PETALBURG (manor), MUS_GAME_CORNER (Exchange + black market),
  MUS_SEALED_CHAMBER (seal chamber). Swap to themed tracks later.

### ✅ Completed — Gildhaven SCRIPTS (script-writer, 2026-06-14, gmake exit 0)
All 13 Gildhaven scripts.inc fully implemented — zero TODO placeholders remain.
Build clean: EWRAM 86.45% / IWRAM 86.63% / ROM 79.89% (26,807,912 B).

SPEAKERS ADDED (pelagios_speaker_names.inc): Glint, Shade, Serel ("SEREL VANE", 10
chars), plus role names Official, Resident, Butler. Cass / Dagan / Lace already existed
(deduped — not re-added). Guard role reused for the 4 Exchange guard trainers (their
names live in trainer data; post-battle lines use the GUARD namebox). Broker / Merchant /
Innkeeper / Traveler / Vendor / Child / Woman / Officer / Clerk all pre-existed.

VAR_GILDHAVEN_PROGRESS flow (authoritative): 0=not arrived, 1=arrived (harbor cutscene
done), 2=Glint beaten (Gym1), 3=Shade beaten (Gym2 — CORRUPTION variants switch ON at
>=3), 4=Lace beaten (Gym3 — manor access + key), 5=Serel beaten (Gym4 — SealChamber
discovery triggers row 14 arm), 6=seal found (discovery played; apparatus offer live),
7=resolved (FLAG_GILDHAVEN_RESOLVED). All gym victories set the EXACT next progress value
(linear island, no nonlinear guard needed). Discovery-at-5 / resolved-at-7 mirrors every
prior SealChamber.

CORRUPTION MECHANIC: NO new var. NPC scripts compare VAR_GILDHAVEN_PROGRESS and
`goto_if_ge ...Corrupt` at >=3 (Harbor official+sailor, Inn traveler, MerchantDistrict
vendor+child, NobleQuarter resident). Each also has a FLAG_GILDHAVEN_RESOLVED post-state
branch (checked FIRST). Pure atmospheric — no gameplay effect.

TWO CASS SCENES — shared-flag guard:
- Scene 1 (harbor glimpse): part of the Arrival coord trigger (progress 0 -> 1, naturally
  once-only). It does NOT set FLAG_CASS_GILDHAVEN_SEEN. Its object
  (LOCALID_GILDHAVEN_CASS_HARBOR, map.json hide flag FLAG_CASS_GILDHAVEN_SEEN which the
  harbor never sets) is walked off + removeobject in-scene, AND a GoldportHarbor
  ON_TRANSITION `removeobject`s it whenever progress != 0 (so it never respawns on
  re-entry). KEY: harbor reserves NOTHING of FLAG_CASS_GILDHAVEN_SEEN.
- Scene 2 (NobleQuarter "Leave. Please."): coord triggers (11,8)/(12,8) arm on
  VAR_TEMP_2==0 (transient — armed every entry). The scene is guarded
  `goto_if_set FLAG_CASS_GILDHAVEN_SEEN -> done`, sets that flag at the end (+ removeobject
  her cameo). Her object's map.json hide flag IS FLAG_CASS_GILDHAVEN_SEEN, so she is gone
  on every later entry. Result: each scene fires exactly once and they cannot conflict,
  because only scene 2 ever touches the shared flag.

DAGAN (BlackMarket) — continuity: object hide flag in map.json is FLAG_GILDHAVEN_GYM1_CLEAR,
but the engine HIDES on flag-SET, which is the WRONG direction (brief wants him to APPEAR
post-Gym1). Fixed WITHOUT a map.json edit via a BlackMarket ON_TRANSITION that drives
visibility explicitly: GYM1 unset -> removeobject; GYM1 set -> addobject (overrides the
spawn flag). First meeting = extended monologue with TWO yes/no question beats; each YES
calls a +1 bump on VAR_DAGAN_RELATIONSHIP (capped at 3 — it carries 0-2 from Sirocco; the
var is a 0-3 comedy scale). Sets FLAG_GILDHAVEN_DAGAN_MET. Subsequent/warm/resolved
variants gate on VAR_DAGAN_RELATIONSHIP>=2 and FLAG_GILDHAVEN_RESOLVED; resolved line sets
up the postcard reference (no Ashenveil object — pure dialogue, per brief).

SEREL/LACE post-battle choreography (Interior2): Serel victory -> post speech -> Serel
walks to window -> Lace cameo revealed (clearflag FLAG_TEMP_1 + addobject
LOCALID_GILDHAVEN_LACE_EXCHANGE) walks in from the side door -> SILENT look (face turns
only, no dialogue) -> Serel walks off + removeobject (ruined, not arrested) -> Lace turns
to player, "It matters." speech -> badge fanfare + FLAG_GILDHAVEN_GYM4_CLEAR + progress 5 +
TM -> Lace withdraws + removeobject. Lace cameo uses the FLAG_TEMP_1 ON_TRANSITION re-hide
pattern (temp flags clear on warp; transition re-sets it so she is invisible by default).

NO-SIPHON SEALCHAMBER (brief-explicit): the discovery + apparatus mirror the established
structure MINUS the siphon-disable step. Mirath leaks NATURALLY through honest old stone —
no Covenant machinery to tear loose. Apparatus = a single direct reinforcement (white
fade, GARDEVOIR cry placeholder for Mirath, the wanting in the air simply stops; the
city's prosperity begins dissolving — described, not shown). Ancient-stone-vs-gilt-Exchange
contrast emphasized. Discovery sets FLAG_GILDHAVEN_SEAL_FOUND + progress 6; apparatus sets
FLAG_GILDHAVEN_RESOLVED + progress 7.

RALTS CALLBACK = CUTSCENE (decided): no Ralts object exists in the SealChamber map.json and
the brief frames it as a fleeting appearance, so it is a PURE one-time cutscene folded into
the discovery scene (SE_PIN + RALTS cry placeholder + narration: a Ralts drawn up by the
leaking energy, scatters into the stone — explicit callback to the Haven Isle ruins). NOT a
catchable battle. Fires once (inside discovery, which is progress-5-gated and self-disarms
to 6). The 20%-Ralts wild-table slot the map-builder added in SealChamber is separate and
untouched.

CIPHER 7 (Inscription sign, SealChamber): FLAG_CIPHER_7_FOUND + FLAG_GILDHAVEN_CIPHER_FOUND
set together; full journal text incl. the encoded "LACE VANE is twelve years old" passage
verbatim from the brief. Re-examine variant.

COVENANT MAP (Interior2 bg_event (11,3)): 3-msgbox reveal of the full yield map (every
island rated), sets FLAG_GILDHAVEN_COVENANT_MAP_SEEN. The InfoBroker (BlackMarket) reacts
to that flag.

THREE-ISLAND ASHENVEIL GATE — VERIFIED CONSISTENT across all three islands. Gildhaven's
resolution does goto_if_unset FLAG_SCHISM_RESOLVED / FLAG_THALVERN_RESOLVED -> NotYet, else
the "all three stilled" branch (narration + Sollis "the dead island… be careful" — NO warp,
NO boat-tier change; Ashenveil sea-chart is future work; clean real handler so only one
branch needs wiring when Ashenveil exists). Confirmed Schism (SealChamber_North,
order-independent, BOTH dual-port chambers funnel through CheckDualSeals->Resolution->
AshenveilCheck) and Thalvern (ThroneRoom) each check the OTHER two RESOLVED flags — so
whichever of the three resolves LAST fires the opens branch. No discrepancy. The gate can
now ACTUALLY fire (all three islands exist).

SOLLIS CONFESSION (resolution PokeNav): she reveals she has held a copy of the Covenant
yield map for EIGHT YEARS (the parent found it), apologizes — another layer of "Maren knows
more than she says," per her CLAUDE.md characterization. Brief's "Solaris" treated as a typo
for SOLLIS (consistent with every prior island).

SEAL SHARD DECISION: ITEM_SEAL_SHARD_GILDHAVEN (896) is NOT given — pure stub per the brief
(Mirath is Fairy/Dark, not in the Mega-shard list; matches the no-give precedent on most
islands; Thalvern/Schism that DO give are the Mega ones). Documented; trivially addable
later AFTER progression if wanted (key item, always full-bag-safe).

BADGES NARRATIVE-ONLY (all 4): engine badge flags exhausted at FLAG_BADGE08_GET, so Glint
(Gilt) / Shade (Shadow) / Lace (Vane) / Serel (Exchange) play fanfare + "{PLAYER} received
the X BADGE" text + island GYM*_CLEAR flag only — NO FLAG_BADGE*_GET.

TM SUBSTITUTIONS (none of the brief's Fairy/Dark TMs exist in the vanilla 50-TM set; each
lampshaded in dialogue): Glint Dazzling Gleam -> ITEM_TM_LIGHT_SCREEN (no Fairy TM exists);
Shade Crunch -> ITEM_TM_THIEF (Dark, same predatory spirit); Lace Play Rough ->
ITEM_TM_TORMENT (Dark, no Play Rough TM); Serel Moonblast -> ITEM_TM_SHADOW_BALL (powerful
special). Key items/flags/badge always given BEFORE the TM (full-bag-safe); Glint/Shade use
Common_EventScript_ShowBagIsFull fallback, Lace/Serel use a skip-label so their key
scene/key item never aborts on a full bag.

CONSTANT RECONCILIATIONS (all verified, nothing invented): flags 0x276-0x281 + 0x4B1 +
0x4C3 all present as specified; VAR_GILDHAVEN_PROGRESS 0x4105 + VAR_DAGAN_RELATIONSHIP
0x40FB present; ITEM_VANE_MANOR_KEY 895 + ITEM_SEAL_SHARD_GILDHAVEN 896 present; journal
item is ITEM_WARDENS_JOURNAL (not referenced by a checkitem this island — no journal-
recognition branch was needed in Gildhaven, unlike Sirocco's Dex); trainers 917-924 present;
HEAL_LOCATION_GILDHAVEN_GOLDPORT_HARBOR present. Localids taken from each map.json
(LOCALID_GILDHAVEN_CASS_HARBOR / _CASS_NOBLE / _DAGAN / _SEREL / _LACE_EXCHANGE / _CITY_NURSE
etc.). NO permanent var spare consumed (0x410D/E/F still free); only VAR_TEMP_1 (Lace cameo,
pre-existing map.json hide flag) + VAR_TEMP_2 (noble Cass trigger, pre-existing map.json) +
VAR_TEMP_1 boarding var (boat) used — all transient. NO new flags/vars/items allocated.

DEVIATIONS / DEFERRED (script side):
- Gym puzzles (Glint stall-buying, Shade reduced-visibility) simplified to talk-gated
  battles — the reused vanilla interiors / in-overworld NPC leaders have no puzzle objects
  (established Ironhold/Sirocco pattern).
- VaneManor front-door warp (NobleQuarter 10/13,5) is NOT script-gated; the Exchange
  PASSAGE is gated narratively via FLAG_GILDHAVEN_MANOR_ACCESS (Butler turns the player
  away from the study + HiddenPassage sign reads "sealed" until access). The
  VaneManor_Interior->Exchange warp tile (2,8) is ungated in map.json (no coord trigger
  there); flow holds because Lace grants access at the same beat she points the player
  down. A future map pass could add a coord-trigger hard gate if playtest shows a sequence
  break.
- Mirath cry placeholder = SPECIES_GARDEVOIR (apparatus); Ralts callback uses SPECIES_RALTS.
- The "city shimmer fades" exterior effect is narration inside the SealChamber (player is
  underground; no camera cut), matching prior islands.

### Gildhaven flow state machine (authoritative)
- VAR_GILDHAVEN_PROGRESS: 0..7 as above (linear; each gym sets the next value exactly).
- VAR_DAGAN_RELATIONSHIP (0-3): carries from Sirocco; +1 per engaged BlackMarket question
  (2 questions), capped at 3. Drives Dagan's subsequent/warm variants (>=2).
- FLAG_CASS_GILDHAVEN_SEEN: set ONLY by the NobleQuarter scene (scene 2). Harbor glimpse
  (scene 1) is once-only via the arrival progress trigger + ON_TRANSITION removeobject.
- VAR_TEMP_1 (Interior2 only): Lace post-battle cameo hide flag, re-set by ON_TRANSITION.
- VAR_TEMP_2 (NobleQuarter only): Cass-scene coord trigger value (transient; flag-guarded).

### ✅ Completed — Aetheron MAPS (map-builder, 2026-06-14, gmake exit 0)
All 11 Aetheron (the SKY ISLAND, Electric/Flying) maps built, registered, compiling
cleanly. EWRAM 86.46% / IWRAM 86.63% / ROM 80.10% (26,876,020 B). Built via
tools/pelagios/build_aetheron.py (6 new layouts) + build_aetheron_mapjson.py (all 11
map.json) + build_aetheron_scripts_stubs.py (stub scripts.inc) + build_aetheron_wild.py
(2 wild tables). Regenerate via those scripts; do NOT hand-edit map.bin/map.json.

THE 11 MAPS (layout / tileset / weather):
1. Aetheron_KnockUpStream — NEW LAYOUT_AETHERON_KNOCK_UP_STREAM (16x14), General+Slateport,
   WEATHER_RAIN. SCRIPTED ASCENT cutscene map only: tiny cloud platform framed by VOID.
   NO NPCs, NO wild, NO warps back. One-way forward warp -> CloudLanding.
2. Aetheron_CloudLanding — NEW LAYOUT_AETHERON_CLOUD_LANDING (20x18), General+Slateport,
   WEATHER_RAIN_THUNDERSTORM. Cloud-pier dock, Tennyson decoration + 4 boarding bg_events,
   inn warp, dockhand + community NPCs. up<->SkyRoute.
3. Aetheron_CloudLanding_Inn — REUSE LAYOUT_POKEMON_CENTER_1F, NONE. Innkeeper (nurse gfx,
   heal). Warps out + upstairs.
4. Aetheron_CloudLanding_Inn_Interior — REUSE LAYOUT_POKEMON_CENTER_2F, NONE. One scholar NPC.
5. Aetheron_SkyRoute — NEW LAYOUT_AETHERON_SKY_ROUTE (18x32), General+Slateport,
   RAIN_THUNDERSTORM. Narrow cloud-path framed by VOID both sides (vertigo). 2 Guardian
   sight trainers (933/934). Storm-patch wild grass. down<->CloudLanding, up<->AetherVillage.
6. Aetheron_AetherVillage — NEW LAYOUT_AETHERON_AETHER_VILLAGE (24x20), General+Slateport,
   RAIN_THUNDERSTORM. Low aerodynamic buildings. Gym1 Gale (937) + Gym2 Arc (938) talk NPCs.
   FIRST CASS SIGHTING object. PC warp. Installation-view sign. down<->SkyRoute, right<->CovenantInstallation.
7. Aetheron_AetherVillage_PokemonCenter — REUSE LAYOUT_POKEMON_CENTER_1F, NONE. Nurse.
8. Aetheron_CovenantInstallation — NEW LAYOUT_AETHERON_COVENANT_INSTALLATION (22x18),
   General+**Mauville** (angular industrial = feels WRONG on a sky island), RAIN_THUNDERSTORM.
   2 Covenant-officer sight trainers (935 + 1 interior). left<->AetherVillage, warp->Interior.
9. Aetheron_CovenantInstallation_Interior — REUSE LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F,
   NONE, MAP_BATTLE_SCENE_GYM. Gym3 Voss (939) talk NPC. Control-panel bg_event
   (FLAG_AETHERON_INSTALLATION_FOUND). THE CASS DEFECTION 3-character tableau (see below).
   3 warps: out (x2) + back stairs (13,1) -> StormPeak.
10. Aetheron_StormPeak — NEW LAYOUT_AETHERON_STORM_PEAK (18x20), General+Slateport,
    RAIN_THUNDERSTORM. Wind-scoured summit, extraction-apparatus rig (sign bg_event),
    post-defection Cass object. warp down -> Installation_Interior, warp (12,4) -> SealChamber.
11. Aetheron_SealChamber — REUSE LAYOUT_SEALED_CHAMBER_INNER_ROOM, NONE. The EYE OF THE
    STORM (bright + still, tonal OPPOSITE of Ashenveil's MorthasGrove). Stormveil (SS_TIDAL
    decoration), extraction apparatus (CABLE_CAR object), Cass-in-chamber object. STORM
    COMPASS as a SCRIPTED examine bg_event on the apparatus wreckage (gives ITEM_STORM_COMPASS
    + FLAG_STORM_COMPASS_OBTAINED — NOT a hidden item). Discovery triggers row 14. Warp back.

ARRIVAL TOPOLOGY (built): boat Pelagios_EventScript_SailToAetheron --warp--> 
MAP_AETHERON_KNOCK_UP_STREAM at (8,11) [verified walkable]; the ascent cutscene fires from
the (8,11)/(8,10) coord triggers (progress 0); the ascent script ends by warping forward
(KnockUpStream warp 0 -> CloudLanding warp index 1 = the dedicated arrival tile (8,13), a
plain settlement-ground tile WEST of the boarding pier so it never re-fires during play).
KnockUpStream has NO warp back. CloudLanding's Tennyson sail-back (4 bg_events) sets
VAR_TEMP_1 = PELAGIOS_ISLAND_AETHERON and gotos the shared boat menu.

BOAT STUB SWAP: replaced SailToAetheron's `goto Pelagios_EventScript_SailNoChart` with
the same-island guard + `warp MAP_AETHERON_KNOCK_UP_STREAM, 8, 11`. The
`goto_if_unset FLAG_SEA_CHART_FOUND` gate is UNTOUCHED (Sea Chart still gates Aetheron).
Convergence handler (FLAG_AETHERON_RESOLVED gate) left untouched.

WEATHER NOTE: the brief's "WEATHER_THUNDERSTORM" does NOT exist. The real symbol is
**WEATHER_RAIN_THUNDERSTORM** (5) — used on all storm maps. KnockUpStream = WEATHER_RAIN
(3). Interiors + SealChamber = WEATHER_NONE.

TWO+ CASS OBJECTS:
- (a) AetherVillage SIGHTING: object flag = FLAG_AETHERON_CASS_SEEN (VISIBLE until the
  sighting scene sets the flag, then vanishes). First-entry coord triggers at progress 1.
- (b) CovenantInstallation_Interior DEFECTION: Cass object at the north DOORWAY (13,2),
  hidden by FLAG_AETHERON_GYM3_CLEAR (appears after Voss is beaten). 3-character tableau:
  Voss at (8,5), Cass at (13,2), player on the open floor between — collision-clear,
  verified. The control panel sign is at (11,3).
- (c) StormPeak post-defection Cass (9,16, faces up) + (d) SealChamber Cass (12,9): both
  use FLAG_AETHERON_CASS_SEEN as a placeholder hide flag; the defection/seal scripts manage
  her real "walks alongside" visibility at runtime (script-writer owns runtime visibility).

GYM PLACEMENTS (all talk-initiated TRAINER_TYPE_NONE so scripts can flag-gate):
Gale (937) AetherVillage (11,8); Arc (938) AetherVillage (13,12); Voss (939)
CovenantInstallation_Interior (8,5).

HEAL: HEAL_LOCATION_AETHERON_CLOUD_LANDING -> CloudLanding_Inn via LOCALID_AETHERON_INNKEEPER
(nurse-gfx innkeeper). DECISION: NOT added to IsLastHealLocationPlayerHouse() — nurse-gfx
innkeeper takes the standard nurse path (per the heal-routing rule; Inns never go in that fn).

WILD TABLES (2, src/data/wild_encounters.json): gAetheronSkyRoute (Emolga40/Swablu30/
Togetic20/Jolteon10, lv53-56), gAetheronStormPeak (Electabuzz40/Manectric30/Vikavolt20/
Xurkitree10, lv56-59). NO encounters elsewhere; no water/fishing. NO hidden items anywhere.

MAPSEC MAPPING (6 pre-existing MAPSECs cover all 11 maps): KNOCK_UP_STREAM (KnockUpStream),
CLOUD_LANDING (CloudLanding + Inn + Inn_Interior), SKY_ROUTE (SkyRoute), AETHER_VILLAGE
(AetherVillage + PokemonCenter), COVENANT_INSTALLATION (Installation + Interior), STORM_PEAK
(StormPeak + SealChamber). Aetheron MAPSEC values are 0x10B-0x110 (> 0xFF) — the documented
harmless "value 0x10X truncated to 0xX" header.inc warning applies (same as Primalis/Ashenveil;
maps build + function, region-map naming may be slightly off until the field is widened).

CONNECTIONS (offsets computed for >=2-tile overlap, all reciprocal): CloudLanding.up(0)<->
SkyRoute.down(0); SkyRoute.up(-4)<->AetherVillage.down(4); AetherVillage.right(1)<->
CovenantInstallation.left(-1). All other links are WARPS (KnockUpStream->CloudLanding one-way;
inn/PC/installation interiors; StormPeak<->Installation_Interior; StormPeak<->SealChamber).

CLOUD/SKY AESTHETIC LIMITATION: the cloud surface is approximated with the light General
ground metatile (0x3001) + deep-water VOID (0x1170) framing the map edges as the open-sky
abyss. TRUE cloud-white/luminous palette is a Porymap per-map palette pass — maps currently
read green-ground-on-General. The Installation's Mauville secondary gives the deliberately
angular/wrong industrial look. SealChamber (reused SEALED_CHAMBER_INNER_ROOM) is meant to
read bright + still vs the storm. All NEW outdoor layouts are blocky rectangles; dress with
cloud/edge/building metatiles in Porymap later (same caveat as every prior island).

DEVIATIONS / NOTES for the script-writer (pelagios-script-writer is NEXT):
- All 11 scripts.inc are STUBS (every EventScript label tagged "@ TODO (script-writer):
  implement ..."). EXCEPTION: AetheronCloudLanding_EventScript_Tennyson is REAL boat wiring.
- KnockUpStream ascent: AetheronKnockUpStream_EventScript_Ascent (triggers (8,11)/(8,10),
  progress 0) must play the ascent cutscene THEN warp the player forward (warp index 0 ->
  CloudLanding arrival tile). Set FLAG_AETHERON_ARRIVED + VAR_AETHERON_PROGRESS=1.
- First Cass sighting: AetheronAetherVillage_EventScript_CassSightingScene (triggers (11,17)/
  (12,17), progress 1) shows + removeobjects the LOCALID_AETHERON_CASS_VILLAGE object and sets
  FLAG_AETHERON_CASS_SEEN (its hide flag).
- DEFECTION: AetheronCovenantInstallationInterior_EventScript_CassDefection — full
  choreography per AETHERON_BRIEF.md (stop music, long pauses, FLAG_CASS_DEFECTED,
  VAR_CASS_RELATIONSHIP=3). Cass object appears via FLAG_AETHERON_GYM3_CLEAR.
- Lava-boots-style gating is NOT needed here (no key-item field gate on Aetheron) — gym
  progression is narrative. Lightning-strike FX on StormPeak = script/field-effect TODO.
- Storm Compass: AetheronSealChamber_EventScript_StormCompass (sign (11,8)) gives
  ITEM_STORM_COMPASS + FLAG_STORM_COMPASS_OBTAINED post-seal. The seal reinforcement +
  resolution (FLAG_AETHERON_SEAL_FOUND/_RESOLVED, ITEM_SEAL_SHARD_AETHERON, Convergence
  unlock) is the script-writer's. Stormveil cry placeholder TBD (no Stormveil species).
- Badges narrative-only (engine badge flags exhausted) — Gale/Arc/Voss set only
  FLAG_AETHERON_GYM1/2/3_CLEAR + fanfare/text. Add Gale/Arc/Voss/Cass(Aetheron) speakers.

### ✅ Completed — Aetheron SCRIPTS (script-writer, 2026-06-14, gmake exit 0)
All 11 Aetheron scripts.inc fully implemented — ZERO TODO placeholders remain. This is
the SKY ISLAND and holds the CASS DEFECTION (the rival-arc emotional turn). Audit at
session start: all 11 were TODO stubs (no prior partial session). EWRAM 86.46% /
IWRAM 86.63% / ROM 80.14% (26,889,556 B, +~13.5 KB scripts).

SPEAKERS (data/scripts/pelagios_speaker_names.inc): added GALE, ARC, DOCKHAND,
TECHNICIAN. Pelagios_Speaker_Voss + Pelagios_Speaker_Cass ALREADY existed (Voss reuses
the "VOSS" string from the Emberveil block; Cass is the rival — never duplicated).

VAR_AETHERON_PROGRESS flow (AUTHORITATIVE, as built — reconciles brief + map-builder
triggers): 0=not arrived, 1=arrived (KnockUpStream ascent done), 2=Gale beaten,
3=Arc beaten, 4=Voss beaten + Cass defection done, 5=StormPeak crested (arms SealChamber
discovery), 6=seal found + reinforced/resolved. (Brief said "5=seal reinforced, 6=resolved";
implemented as 5=at-chamber-discovery / 6=resolved to match the map-builder's progress==5
discovery trigger and progress==4 StormPeak arrival trigger.)

- KNOCK UP STREAM (AetheronKnockUpStream_EventScript_Ascent, coord triggers (8,11)/(8,10)
  at progress 0): rapid fade-to/from-white speed-line ascent narration + a glimpse of Cass's
  Covenant vessel rising alongside, then one-way `warp MAP_AETHERON_CLOUD_LANDING, 8, 13`.
  Sets FLAG_AETHERON_ARRIVED + progress 1. (The boat warps the player IN at (8,11) = the
  trigger tile, so the ascent fires immediately on arrival.)
- CLOUDLANDING: dockhand ("First outsiders in three years… You're not Covenant") + community
  member, both with RESOLVED variants. The (9,12)/(10,12) progress==0 Arrival coord trigger is
  a SAFE NO-OP (progress is already 1 before the player lands here, so it can never fire) — the
  arrival atmosphere lives in the dockhand's first-talk. Inn = innkeeper YESNO heal +
  ON_TRANSITION setrespawn HEAL_LOCATION_AETHERON_CLOUD_LANDING; Inn_Interior = scholar
  (Arc-left-the-village lore).
- SKYROUTE: Guardian Sael (933) + Renn (934) sight trainers, brief dialogue verbatim
  (trainerbattle first command, setspeaker only before post-battle).
- AETHERVILLAGE: Gale (Gym 1, talk-initiated, TM Aerial Ace [no Air Slash TM — lampshaded],
  narrative-only STORM BADGE, progress 2); Arc (Gym 2, gated on GYM1_CLEAR, community-defector
  preamble, "hands a schematic" = LORE FLAVOR ONLY no item, TM Thunderbolt, VOLT BADGE,
  progress 3); FIRST CASS SIGHTING (coord triggers (11,17)/(12,17) at progress 1, fires ONCE):
  Cass at (20,5) faces the player, a long look, then walks 2 north into the alley +
  `removeobject` + `setflag FLAG_AETHERON_CASS_SEEN` (her hide flag) — no dialogue, narration
  only; installation-view sign. Child + defector NPC dialogue lives via the gym leaders / signs.
  PokemonCenter = standard nurse + respawn.
- COVENANTINSTALLATION (exterior): Officer Mael (935) sight trainer; outer-gate officer
  talk-flavor (no battle, RESOLVED variant); Approach coord trigger (progress==2 only) =
  facility atmosphere narration.
- COVENANTINSTALLATION_INTERIOR: Voss (Gym 3, gated on GYM2_CLEAR, full pre/post verbatim,
  TM Thunder, COMMAND BADGE) whose post-battle dialogue ENDS by looking to the door and
  `goto`s straight into the DEFECTION (no free-roam between). Officer Sera (936) sight
  trainer. Control panel (sign (11,3)) = full yield-log readout VERBATIM, sets
  FLAG_AETHERON_INSTALLATION_FOUND. ON_LOAD removes Voss once GYM3_CLEAR set; ON_TRANSITION
  removes the defection-Cass object while GYM3_CLEAR is unset (her hide flag is GYM3_CLEAR
  which is CLEAR pre-fight, so the engine would otherwise draw her early).
- THE CASS DEFECTION (AetheronCovenantInstallationInterior_EventScript_CassDefection):
  EXACT timing honored — `fadeoutbgm 4` (MANDATORY silence) → Voss "Agent Vell. Stop them."
  → `delay 60` (the silence beat, NON-NEGOTIABLE, NOT shortened) → Cass "Commander." →
  the documents speech (847/400/401, verbatim, paragraph-split) → "There isn't one." →
  Voss cut off ("Agent Vell-") → Cass "I'm done." → Cass steps in (walk_down x2) → "{PLAYER}.
  I'm sorry it took me this long." → "Can I come with you?" → `fadedefaultbgm` (music resumes)
  → player YES (one-way "Of course.") → Cass "Okay." → `delay 20` → "Let's go." → Voss exits
  QUIETLY (plain walk-off + removeobject, no speech). Sets FLAG_AETHERON_GYM3_CLEAR +
  FLAG_CASS_DEFECTED + **setvar VAR_CASS_RELATIONSHIP, 3 (HARD SET, not addvar)** + progress 4.
  Cass is revealed mid-scene via clearflag GYM3_CLEAR + addobject, removed at the end.
  Full-bag note: VOSS's TM is given before the defection with the result cached in VAR_TEMP_2;
  if the bag was full a single forfeit line plays at the very end (the scene NEVER aborts).
- STORMPEAK: ON_TRANSITION force-spawns the post-defection Cass object (addobject when
  FLAG_CASS_DEFECTED — does NOT clear the shared CASS_SEEN hide flag, avoiding a cross-map
  un-hide). Cass-talk = "Let's just get it done." Arrival trigger (progress==4) = crest
  narration → setvar progress 5 (arms SealChamber discovery). Apparatus sign = flavor
  examine (the real reinforcement is in the chamber) with RESOLVED variant.
- SEALCHAMBER (two-character climax): ON_TRANSITION force-spawns Cass. Discovery triggers
  (10,14)/(9,14) at progress==5 = the silent eye-of-the-storm reveal, Stormveil cry
  placeholder SPECIES_ZAPDOS (no Stormveil species), sets FLAG_AETHERON_SEAL_FOUND + progress 6.
  Apparatus (CABLE_CAR, YESNO) = player + Cass reinforce TOGETHER, Stormveil answers, the
  storm shifts, the extraction core self-destructs, then `goto`s the Storm Compass giver:
  giveitem ITEM_STORM_COMPASS (full-bag-safe) + setflag FLAG_STORM_COMPASS_OBTAINED + setflag
  FLAG_AETHERON_RESOLVED + giveitem ITEM_SEAL_SHARD_AETHERON (stub) → POST-RESOLUTION CASS
  (verbatim: "Is this what it felt like?…" → player "Convergence." → Cass "Right." (delay 20)
  "Together then.") → SOLLIS POKeNAV CALL (verbatim, uses `--` two hyphens NOT em dash:
  "And -- is that Cass?… Good. I'm glad she's with you. Come home soon. Both of you.") →
  close line stating the way to Convergence is open. The StormCompass sign (11,8) is the same
  giver with pre-reinforcement / already-have flavor branches. NO new cipher (cipher 9 was
  on Ashenveil — brief confirms none here).
- STORM COMPASS FIELD EFFECT: SCRIPT written in data/scripts/pelagios_boat.inc as
  **Pelagios_EventScript_StormCompass** — `lockall` + `setvar VAR_TEMP_1, 255` (sentinel so the
  sail handlers' same-island guard never matches from the field) + MULTI_BOAT_GALLEON
  multichoice + dispatch to the existing Pelagios_EventScript_SailTo* handlers (works from ANY
  map; Convergence stays gated by FLAG_AETHERON_RESOLVED). ✅ C HANDOFF RESOLVED
  (systems-engineer, 2026-06-14): the item's bag "Use" is now wired. See "Storm Compass
  field-use wiring" below.
- CONVERGENCE GATE VERIFIED: Pelagios_EventScript_SailToConvergence still does
  `goto_if_unset FLAG_AETHERON_RESOLVED, Pelagios_EventScript_ConvergenceClosed` (boat menu
  UNTOUCHED). The SealChamber resolution sets FLAG_AETHERON_RESOLVED; the Sollis call is the
  in-world signal Convergence is reachable. The Storm Compass menu lists Convergence (case 11)
  under the same gate.
- GYM/TM DECISIONS: all narrative-only badges (no FLAG_BADGE*_GET; engine cap exhausted). TMs:
  Gale = TM Aerial Ace (no Air Slash TM exists — lampshaded), Arc = TM Thunderbolt (as briefed),
  Voss = TM Thunder (as briefed). Voss's Zekrom party slot is systems-engineer's data (parties
  already exist); scripts don't touch parties.
- CONSTANT RECONCILIATIONS: all flags/vars/items confirmed by grep (0x297-0x29E story flags,
  FLAG_AETHERON_RESOLVED 0x4B4, FLAG_CASS_DEFECTED 0x4BC, VAR_AETHERON_PROGRESS 0x4108,
  VAR_CASS_RELATIONSHIP 0x40F7, ITEM_STORM_COMPASS 880, ITEM_SEAL_SHARD_AETHERON 901). NOTHING
  invented. ITEM_CASS_DOCUMENTS (902) exists but is NOT given in scripts — the documents are
  narrated in the defection rather than handed over as a bag item (a lore item with no examine
  hook was unnecessary; the defection text carries the 847/400/401 content). NO permanent var
  spare consumed (0x410D/E/F untouched; only VAR_TEMP_1/VAR_TEMP_2 used transiently).
- DEFERRED / DEVIATIONS: (1) Storm Compass C bag-use registration — DONE 2026-06-14 (see
  "Storm Compass field-use wiring" below). (2) The shared
  SailTo* handlers play the boat "cast off" text even when reached from the air via the compass
  — acceptable flavor, swap if it ever matters. (3) StormPeak lightning-strike FX is narration
  only (no C field effect). (4) ITEM_CASS_DOCUMENTS unused (narrated instead). (5) Gym-puzzle
  geometry not built (leaders are in-overworld NPCs — established Petra/Ironhold pattern).
  (6) Stormveil cry = SPECIES_ZAPDOS placeholder.

### ✅ Completed — Ashenveil MAPS (map-builder, 2026-06-14, gmake exit 0)
All 9 Ashenveil (the DEAD ISLAND) maps built, registered, compiling cleanly.
EWRAM 86.46% / IWRAM 86.63% / ROM 80.03% (26,854,372 B, +~8 KB over the Primalis
baseline). Built via tools/pelagios/build_ashenveil.py (5 ash/ruin outdoor layouts)
+ build_ashenveil_mapjson.py (9 map.json) + build_ashenveil_scripts_stubs.py (stub
scripts.inc; Greyport Tennyson is REAL boat wiring) + build_ashenveil_wild.py (3 Ghost
tables). Regenerate via those scripts; NEVER hand-edit map.bin/map.json/scripts.inc.

ASHENVEIL IS UNLIKE EVERY PRIOR ISLAND — hard constraints honored:
- NO HEAL LOCATION. No inn, no nurse. Nothing added to src/data/heal_locations.json.
  IsLastHealLocationPlayerHouse() UNTOUCHED (verified: grep -ic ashenveil = 0 in both).
- NO TRAINERS, NO GYM LEADERS. Zero trainer/gym objects (grep TRAINER_TYPE_NORMAL = NONE).
  TRAINERS_COUNT unchanged (no constants added by this pass).
- WILD encounters in ONLY 3 maps (Ghost tables, see below). No other map has a table.
- The ONLY living NPC is Orin (Greyport). Every other "NPC" is an examine bg_event (sign)
  or a script-managed object (Dorne, Morthas). No talk-characters, no sight trainers.
- NO bg_hidden_item events anywhere (the Sea Chart is a SCRIPTED examine bg_event).

THE 9 MAPS (tileset / weather / layout):
1. Greyport — General+Slateport / WEATHER_FOG_HORIZONTAL / NEW LAYOUT_ASHENVEIL_GREYPORT
   (20x18). MAP_TYPE_TOWN. Tiny ruined Galleon port. Orin NPC at outpost entrance (7,8,
   talk, TODO script). SS_TIDAL Tennyson decoration (script 0x0, elev 1, (12,15)); 4
   boarding sign bg_events on the dock (9-10,14-15) -> AshenveilGreyport_EventScript_Tennyson
   (REAL wiring: sets VAR_TEMP_1 = PELAGIOS_ISLAND_ASHENVEIL). Arrival coord triggers
   (9-10,12) at VAR_ASHENVEIL_PROGRESS==0. Warp (5,7) into the outpost. up<->AshFields1.
2. Greyport_Outpost — REUSE LAYOUT_HOUSE2 / WEATHER_NONE / MAP_TYPE_INDOOR. 2 examine
   signs: research notes (2,1), Phantom-Lantern shelf (6,1, script-writer gives the Lantern
   + FLAG_PHANTOM_LANTERN_OBTAINED). Warps: out (3-4,7), up into Interior (8,2 — moved off
   the HOUSE2 wall row to a walkable floor tile).
3. Greyport_Outpost_Interior — REUSE LAYOUT_HOUSE2 / WEATHER_NONE / INDOOR. 1 examine sign:
   half-finished warm tea cup (5,1, "Cass was here"). Warp back (8-9,2).
4. AshFields1 — General+Slateport / FOG / NEW LAYOUT_ASHENVEIL_ASH_FIELDS1 (22x32).
   MAP_TYPE_ROUTE. Ghost wild (gAshenveilAshFields1). 4 environmental examine signs:
   ChildShoe (5,13), DoorHandle (16,13), StreetSign "RIVER LANE" (6,20), Footprints (15,20).
   PHANTOM-LANTERN ash-gates: impassable ASHWALL rows (y=15) seal TWO OPTIONAL reward nooks
   (W cols 2-4, E cols 17-19) — the MAIN N-S spine (cols 10-11) stays fully open, so the
   player can ALWAYS reach DeadCity even if a gate is mid-implementation (SOFTLOCK-SAFE).
   3 TODO LanternGate coord triggers (progress==2) front the nooks. up<->DeadCity, down<->Greyport.
5. DeadCity_Exterior — General+Slateport / FOG / NEW LAYOUT_ASHENVEIL_DEAD_CITY_EXTERIOR
   (32x30, the LARGE map — scale is the point). MAP_TYPE_CITY. Grid of ruined building
   blocks + ash streets. Ghost wild (gAshenveilDeadCityExterior). NO NPCs. Warps down into
   Ruins1 (5,7) + Ruins2 (13,7). up<->TheMemorial, down<->AshFields1. Central spine (15-16) open.
6. DeadCity_Ruins1 — REUSE LAYOUT_GRANITE_CAVE_B1F / WEATHER_NONE / MAP_TYPE_UNDERGROUND.
   1 examine sign: the official sanitized Covenant report (25,11). Warps out (25,13)+(4,21).
7. DeadCity_Ruins2 — REUSE LAYOUT_GRANITE_CAVE_B2F / WEATHER_NONE / UNDERGROUND. 1 examine
   sign: the REAL Covenant documents (29,11, Cass's copy on the floor; script-writer sets
   FLAG_ASHENVEIL_COVENANT_DOCS_FOUND). Warps out (29,13)+(28,21).
8. TheMemorial — General+Slateport / FOG / NEW LAYOUT_ASHENVEIL_THE_MEMORIAL (20x18).
   MAP_TYPE_ROUTE. A SMALL GARDEN — the ONLY GREEN on the island (General grass patch amid
   ash). 4 grave-marker examine signs (6,5)/(9,5)/(12,5)/(14,5); the rightmost (14,5,
   GraveNewest) is Vael Jr. DORNE: a script-managed object at (14,6) with hide flag
   FLAG_ASHENVEIL_DORNE_MET (PRESENT until the scene plays, GONE after). Central tableau
   space (8-11, 8-11) kept clear/walkable for the 2-NPC Dorne scene. 2 DorneScene coord
   triggers (progress==3) at (9-10,11). NO wild. down<->DeadCity, right<->MorthasGrove.
9. MorthasGrove — General+Slateport / FOG / NEW LAYOUT_ASHENVEIL_MORTHAS_GROVE (22x22),
   the DARKEST map. MAP_TYPE_ROUTE. Cathedral of dead-tree trunks (WALL clumps) ringing a
   dark central pool (WATER (9-12,9-12) = Morthas's seal). Morthas overworld object at (10,8)
   with hide flag FLAG_MORTHAS_ENCOUNTERED (VISIBLE while unset, GONE once set — exactly the
   map-builder's flag-field idiom). Sea Chart: a SCRIPTED examine sign at (15,16) near the
   tree line (script-writer gives ITEM_SEA_CHART + FLAG_SEA_CHART_FOUND — NOT a hidden item,
   no flag slot consumed). 2 MorthasApproach coord triggers (progress==5) at (10-11,13).
   Ghost wild pre-encounter (gAshenveilMorthasGrove). left<->TheMemorial.

CONNECTIONS (all reciprocal, offsets computed from compiled collision gaps):
  Greyport.up(-1)<->AshFields1.down(+1) ; AshFields1.up(-5)<->DeadCity.down(+5) ;
  DeadCity.up(+6)<->TheMemorial.down(-6) ; TheMemorial.right(-2)<->MorthasGrove.left(+2).
  Ruins1/Ruins2 reached by WARP from DeadCity_Exterior (not connections).

WILD ENCOUNTERS (src/data/wild_encounters.json, 12-slot land tables, rate 20; 3 maps only):
  gAshenveilAshFields1   — Gastly/Shuppet/Duskull/Honedge (lv48-51)
  gAshenveilDeadCityExterior — Misdreavus/Sableye/Runerigus/Cursola (lv50-53)
  gAshenveilMorthasGrove — Phantump/Spiritomb/Dragapult (lv52-55, pre-encounter)
  Distributed via build_ashenveil_wild.py (greedy slot fill approximating the brief's
  40/30/20/10 split across the canonical Emerald slot weights). All species verified to exist.

BOAT WIRING: data/scripts/pelagios_boat.inc — Pelagios_EventScript_SailToAshenveil swapped
from SailNoChart to a REAL handler (same-island guard goto_if_eq VAR_TEMP_1,
PELAGIOS_ISLAND_ASHENVEIL -> AlreadyHere; warp MAP_ASHENVEIL_GREYPORT 10,14 — verified
walkable). Aetheron handler LEFT AS-IS (its FLAG_SEA_CHART_FOUND gate stays; Aetheron maps
don't exist yet).

MAPSEC mapping (9 maps -> 5 MAPSECs, already registered by systems-engineer):
  Greyport/Outpost/Interior -> MAPSEC_ASHENVEIL_GREYPORT ; AshFields1 -> _ASH_FIELDS ;
  DeadCity Exterior/Ruins1/Ruins2 -> _DEAD_CITY ; TheMemorial -> _MEMORIAL ;
  MorthasGrove -> _MORTHAS_GROVE.

DUSK-PALETTE LIMITATION (documented): "permanent dusk" is approximated with grey
Slateport stone metatiles + fog weather. TRUE dusk palette tinting needs a Porymap
per-map darkened-palette pass (or a C map-load tint) — out of map-builder scope. Maps
read grey-but-not-dark until then; MorthasGrove is meant to be the darkest.

KNOWN WARNING — RESOLVED 2026-06-14 (systems-engineer): the header.inc line-12
"value 0x10X truncated to 0xX" warning (Ashenveil 0x106-0x10A, Primalis 0x100-0x105) is
FIXED. Root cause was the map-header ASM emitter tools/mapjson/mapjson.cpp still writing
`.byte` for the region section after mapsec_u8_t was widened to u16 — both truncating
MAPSEC>=256 AND silently misaligning every map's weather/type/battle-scene byte. Emitter
now writes `.2byte`; all 1074 headers regenerated; build clean, zero warnings. Full detail:
"MAP-HEADER MAPSEC WIDTH FIX" under SAVE-COMPATIBILITY BREAK #2. ROM-only, no new save break.

DEVIATIONS / NOTES for the script-writer (pelagios-script-writer is NEXT):
- All 9 scripts.inc are STUBS (every EventScript label tagged "@ TODO (script-writer):
  implement ..."), EXCEPT AshenveilGreyport_EventScript_Tennyson (real boat wiring).
  The stub generator won't clobber a scripts.inc once it has a .string line.
- The Phantom-Lantern ash-gates only seal OPTIONAL nooks; you can wire the field-effect
  clear at leisure without risking main-path softlock. If the brief wants a Lantern-gated
  MAIN chokepoint, add an ASHWALL band across cols 10-11 in build_ashenveil.py and a
  matching coord trigger — but the Lantern is obtained at the Greyport outpost (progress 2),
  so a main gate is also safe. Document whichever you choose.
- Dorne is a script-managed object (hide flag FLAG_ASHENVEIL_DORNE_MET). The DorneScene
  coord trigger fires at progress==3; your scene should set the flag on the player's choice
  (FLAG_DORNE_CHOICE_STOP/HELP/DEFER) so Dorne is removed afterward. The newest grave
  (GraveNewest, examinable after he leaves) is at (14,5), beside his spawn (14,6).
- Morthas's hide flag is FLAG_MORTHAS_ENCOUNTERED — set it after the 5-turn endurance
  sequence and the object vanishes automatically (map-builder set the flag field).
- The Sea Chart stone is a normal examine sign (15,16); give ITEM_SEA_CHART +
  FLAG_SEA_CHART_FOUND there (no hidden-item flag needed). FLAG_SEA_CHART_FOUND is the
  Aetheron unlock in the boat menu.
- Cipher 9 / FLAG_TRUE_ENDING_UNLOCKED / FLAG_ASHENVEIL_RESOLVED have NO dedicated map
  object yet — the brief implies they resolve at/after the Morthas encounter in the grove.
  Wire them into the MorthasApproach/Morthas resolution scripts, or add an apparatus object
  to MorthasGrove if you prefer a discrete seal-focus (Morthas IS the focus per the brief).
- MUSIC placeholders (swap later): Greyport MUS_ABNORMAL_WEATHER, fields MUS_ROUTE120,
  DeadCity/ruins MUS_MT_PYRE, memorial MUS_MT_PYRE_EXTERIOR, grove MUS_SEALED_CHAMBER.
- SPRITE placeholders: Orin OBJ_EVENT_GFX_SCIENTIST_1, Dorne OBJ_EVENT_GFX_MAN_4,
  Morthas OBJ_EVENT_GFX_SS_TIDAL (the standard sealed-legendary decoration).
- Custom outdoor layouts are blocky grey ash/ruin rectangles; review in Porymap and dress
  with ruined-building/cliff/dusk metatiles later (same caveat as every prior island).

### ✅ Completed — Ashenveil SCRIPTS (script-writer, 2026-06-14, gmake exit 0)
All 9 Ashenveil scripts.inc are fully implemented — ZERO TODO placeholders across the
island. A prior script-writer session wrote maps 1-7 (Greyport, Greyport_Outpost,
Greyport_Outpost_Interior, AshFields1, DeadCity_Ruins1, DeadCity_Ruins2; DeadCity_Exterior
is intentionally event-free) then dropped. THIS session wrote the final TWO — the game's
climax: Ashenveil_TheMemorial + Ashenveil_MorthasGrove. EWRAM 86.46% / IWRAM 86.63% /
ROM 80.06% (26,864,372 B).

THE DORNE SCENE (TheMemorial, DorneScene coord trigger, VAR_ASHENVEIL_PROGRESS==3):
- Reveals the FLAG_ASHENVEIL_DORNE_MET-hidden Dorne object via clearflag + addobject;
  after he leaves, removeobject + re-setflag so he is permanently gone on future loads.
- Full truth sequence (setspeaker Pelagios_Speaker_Dorne, split into beats): the Covenant
  let the island die (847 / boats for 400 / falsified report), the seal-siphoning cause,
  the ancient kill switch, the Warden's-death truth, and the carry line VERBATIM in its
  own msgbox ("I didn't kill your parent. But I knew it was coming and I didn't warn
  them. That's what I carry."), then the offer.
- 3-WAY CHOICE via two chained MSGBOX_YESNO (no C multichoice list — Schism/Drenn pattern):
  "I'll stop you." -> FLAG_DORNE_CHOICE_STOP ; "I'll help you." -> FLAG_DORNE_CHOICE_HELP ;
  decline both -> FLAG_DORNE_CHOICE_DEFER. Exactly one flag set per run.
- VAR_DORNE_RELATIONSHIP (0=Adversarial..3=Reluctant Respect): HELP -> 3 (maximum, per
  brief); STOP -> raised to 2 only if currently below 2 (never lowers a higher prior
  value — "I still respect it"); DEFER -> unchanged.
- Daughter lines VERBATIM ("Her name was Vael. Same as mine. Her mother's idea. She was
  four years old."), then a QUIET exit: plain applymovement walk-off (down/right) +
  removeobject. NO music sting, NO camera, NO fanfare. He just leaves.
- Sets FLAG_ASHENVEIL_DORNE_MET + FLAG_ASHENVEIL_VISITED, advances progress 3 -> 4.
- Talk fallback (AshenveilTheMemorial_EventScript_Dorne): defensive short line only;
  the scene is trigger-only and Dorne is removed afterward.
- Graves: Grave1/2/3 quiet examine lines; GraveNewest ENDS EXACTLY on "She would have
  been twenty-three." — nothing follows.

THE MORTHAS ENCOUNTER (MorthasGrove, MorthasApproach coord trigger, progress==5):
- 5-turn endurance loop (NOT a battle). Counter = VAR_TEMP_1 (NO permanent var spare
  consumed; 0x410D/E/F untouched). Each turn: impressionistic vision msgbox +
  MSGBOX_YESNO "Endure it?". NO/turn-away -> resets VAR_TEMP_1, steps the player back,
  releaseall (progress stays 5 so re-entering re-arms the trigger for a retry). YES ->
  addvar; at 5 -> Morthas settles.
- FINAL VISION IN SILENCE: BGM stopped with `fadeoutbgm 4`, the brief's verbatim
  six-seconds vision shown across 3 msgboxes paced with `delay` (incl. a `delay 90`
  stillness beat), then music restored with `fadedefaultbgm`. The silence is real.
- Resolution ordering: set FLAG_MORTHAS_ENCOUNTERED + FLAG_ASHENVEIL_VISITED + progress
  (6 then 7) BEFORE the full-bag-safe giveitem ITEM_SEAL_SHARD_ASHENVEIL, so a full bag
  can never strand the resolution. removeobject hides Morthas.
- CIPHER 9 is shown LAST as the emotional climax: sets FLAG_CIPHER_9_FOUND +
  FLAG_ASHENVEIL_CIPHER_FOUND + FLAG_TRUE_ENDING_UNLOCKED, then the full journal entry
  VERBATIM, ENDING EXACTLY on "I love you. I should have said that more. / I love you."
  — NOTHING follows it.
- Talk fallback (AshenveilMorthasGrove_EventScript_Morthas): defensive still-pool line.

SEA CHART (MorthasGrove SeaChartStone bg_event): examine -> sets FLAG_SEA_CHART_FOUND
(BEFORE the full-bag-safe giveitem ITEM_SEA_CHART) + advances progress to 5 if behind,
includes Cass's note VERBATIM with TWO HYPHENS ("In case something happens to me. -- C"),
and a beat telling the player Aetheron (the sky island) is now reachable. AETHERON GATE
VERIFIED: data/scripts/pelagios_boat.inc Pelagios_EventScript_SailToAetheron does
`goto_if_unset FLAG_SEA_CHART_FOUND` — the chart flips the gate (boat menu NOT edited).
Until Aetheron's port is built the menu falls through to SailNoChart ("no port yet")
even with the chart — expected/correct.

CONSTANT RECONCILIATION: FLAG_ASHENVEIL_RESOLVED does NOT exist in flags.h (grep = 0).
The MAPS handoff note suggested wiring it; instead the island's resolution flag is
FLAG_ASHENVEIL_VISITED (0x4B3), set in both the Dorne scene and the Morthas resolution.
No RESOLVED flag is set anywhere — do not add one.

DEFERRED / KNOWN LIMITATIONS (script side):
- 3-way choice is two YESNO prompts, not a one-screen 3-option menu (C multichoice lists
  are systems-engineer domain). Reads naturally.
- Dorne's walk-off is a short applymovement then removeobject (no guaranteed
  collision-free exit path on the composed memorial layout) — matches "no dramatic exit".
- Morthas cry: none played (the encounter is silence-themed; no placeholder cry, unlike
  prior seal chambers — intentional).
- ITEM_SEAL_SHARD_ASHENVEIL given as the Decidueye Mega trigger stub (Mega wiring is
  future systems work).

### ✅ Completed — Primalis MAPS (orchestrator inline, 2026-06-14, gmake exit 0)
All 13 Primalis maps built, registered, compiling cleanly. EWRAM 86.46% / IWRAM 86.63% /
ROM 79.94% (26,824,648 B, +~13 KB). Built via tools/pelagios/build_primalis.py (7 jungle
layouts) + build_primalis_mapjson.py (13 map.json) + build_primalis_scripts_stubs.py
(stub scripts.inc; the VerdantLanding Tennyson is REAL boat wiring). Regenerate via those
scripts; don't hand-edit map.bin/map.json. (Built inline by the orchestrator after two
map-builder agent sessions dropped early leaving only a partial layouts generator.)

TILESETS: every outdoor map pairs gTileset_General (primary) + gTileset_Fortree
(secondary, the vanilla treehouse/jungle-canopy set — closest dense-jungle match).
Interiors reuse vanilla layouts (zero new binary): Inn/PokeCenter = POKEMON_CENTER_1F/2F;
ElderHall/_Interior = MOSSDEEP_CITY_SPACE_CENTER_1F/2F (foyer + upper oral-history
chamber, two facing elder NPCs); Heartwood_SealChamber = SEALED_CHAMBER_INNER_ROOM.

WEATHER: WEATHER_FOG_HORIZONTAL on ALL outdoor jungle maps (VerdantLanding, both routes,
JungleInterior, ZoanVillage, AncientRuinsCamp) — the brief's "dense fog". ⚠️ DEVIATION:
the task brief asked WEATHER_FOGGY for ZoanVillage but that constant does NOT exist
(6=FOG_HORIZONTAL, 9=FOG_DIAGONAL unused, 22=FOG aggregate); used FOG_HORIZONTAL. TheHeartwood
+ SealChamber + all interiors = WEATHER_NONE.

THE 13 MAPS (tileset / weather / layout):
- VerdantLanding (Fortree/FOG, composed 20x18) — Galleon dock + Tennyson; Inn warp.
- VerdantLanding_Inn / _Inn_Interior (POKEMON_CENTER_1F/2F, NONE) — heal + the Lens.
- JungleRoute1 (Fortree/FOG, 20x32) — optional Beast-Whistle nook (see below).
- JungleInterior (Fortree/FOG, 26x24) — river hub.
- ZoanVillage (Fortree/FOG, 28x24) — Fern (Gym1) + Scale (Gym2) talk-NPCs; 2 warps.
- ZoanVillage_PokemonCenter (POKEMON_CENTER_1F, NONE) — nurse.
- ZoanVillage_ElderHall / _Interior (MOSSDEEP_SPACE_CENTER_1F/2F, NONE) — Mako (Gym4) +
  second elder facing him for the oral-history scene.
- JungleRoute2 (Fortree/FOG, 20x30) — Thorn (Gym3) talk-NPC; north-exit Beast gate.
- AncientRuinsCamp (Fortree/FOG, 24x20) — central altar bg_event (cipher 8 / RUINS_FOUND).
- TheHeartwood (Fortree/NONE, 28x34) — THE vast ancient sanctuary: long processional nave,
  twin tree colonnades, root-knot centerpiece; warp up into the SealChamber.
- Heartwood_SealChamber (SEALED_CHAMBER_INNER_ROOM, NONE) — Verdath SS_TIDAL decoration
  (10,5), apparatus CABLE_CAR (10,8), discovery triggers row 14 (progress 5), warp back
  (10,19) — identical to the other islands' seal rooms.

⚠️ BEAST-WHISTLE GATE DESIGN (important — I diverged from the raw layout generator to
avoid a softlock): the Whistle is obtained POST-GYM4, so it CANNOT gate early main paths.
The original generator put full-width impassable bands across BOTH routes' main paths,
which would have softlocked the player before Gym 1. Fixed:
- JungleRoute1's undergrowth now seals only an OPTIONAL NE reward nook (main spine fully
  open — verified x9-10 passable the whole length).
- JungleRoute2's undergrowth seals the NORTH EXIT (the AncientRuinsCamp/Heartwood approach)
  — correct, since the player reaches the Heartwood only after the Whistle; Gym 3 (Thorn)
  is met SOUTH of the seal and stays reachable (verified spine open y3-28, row2 sealed).
Both seals have TODO coord triggers (PrimalisJungleRoute1/2_EventScript_BeastGate) for the
script-writer to wire to the Beast-Whistle field effect (they compare VAR_PRIMALIS_PROGRESS).

CONNECTIONS (all reciprocal, offsets negated — verified): VerdantLanding.up<->JungleRoute1
.down (0); JungleRoute1.up<->JungleInterior.down (-3/3); JungleInterior.up<->ZoanVillage
.down (-1/1); ZoanVillage.right<->JungleRoute2.left (-3/3); JungleRoute2.up<->AncientRuins
Camp.down (-2/2); AncientRuinsCamp.up<->TheHeartwood.down (-2/2). TheHeartwood->SealChamber
is a WARP. (JungleInterior's layout has an unused east gap — harmless dead-end, no softlock.)

GYM LEADERS are talk-initiated (TRAINER_TYPE_NONE) so the script-writer can flag-gate them:
Fern/Scale in ZoanVillage, Thorn on JungleRoute2, Mako in ElderHall_Interior (+ a Heartwood
cameo). 5 Zoan-guardian sight trainers placed (IDs 925-928 — note only 4 trainer IDs exist,
so the script-writer should map the 5th object to one of them or drop it; map.json gives each
its own LOCALID + TODO label). The Lens appears at the Inn AND the AncientRuinsCamp/Heartwood
(hide flag FLAG_PRIMALIS_LENS_MET / FLAG_PRIMALIS_RUINS_FOUND).

WILD TABLES (12-slot land, rate 20, brief species, 5/4/2/1 split; Heartwood 6/4/2):
gPrimalisJungleRoute1 (Tropius/Budew/Goomy/Bagon), gPrimalisJungleInterior (Tangela/
Exeggcute/Jangmo-o/Dratini — "river only" flattened to the rare slot), gPrimalisJungleRoute2
(Vibrava/Steenee/Jangmo-o/Kartana), gPrimalisTheHeartwood (Zarude/Drampa/Goomy). No tables
in VerdantLanding/ZoanVillage/ElderHall/AncientRuinsCamp/SealChamber (per brief).

HEAL LOCATION: HEAL_LOCATION_PRIMALIS_VERDANT_LANDING -> respawn at VerdantLanding_Inn via
LOCALID_PRIMALIS_VERDANT_INNKEEPER (nurse-gfx). NOT in IsLastHealLocationPlayerHouse()
(correct — nurse-gfx innkeeper takes the normal nurse path; Schism/Thalvern/Gildhaven precedent).

BOAT MENU: ⚠️ Primalis was NOT in the Galleon menu, so I ADDED it: new
PELAGIOS_ISLAND_PRIMALIS = 8 (id 7 was SCHISM_SOUTH); "PRIMALIS" appended to
MultichoiceList_BoatGalleon (src/data/script_menu.h) at index 8 (before Cancel, now index 9);
dispatch `case 8, Pelagios_EventScript_SailToPrimalis` added; real SailToPrimalis handler
warps to MAP_PRIMALIS_VERDANT_LANDING (10,14) with the same-island guard. VerdantLanding's
Tennyson sets VAR_TEMP_1 = PELAGIOS_ISLAND_PRIMALIS. Menu index and dispatch case verified aligned.

NO HIDDEN ITEMS (per brief). MAPSEC positions left at placeholder 0,0 (no region map yet —
all islands do this). Sprite placeholders: no Zoan/shark gfx exist — Fern=LASS, Scale=MAN_5,
Thorn/Mako=HIKER, Zoan trainers=CAMPER/PICNICKER, Lens=SCIENTIST_1. Outdoor layouts are blocky
jungle rectangles (Fortree canopy detail is Porymap hand-tuning later — established caveat).

### ✅ Completed — Key-item bag display audit (systems-engineer, 2026-06-14, gmake exit 0)
Audited the bag/key-items display for all 26 Pelagios key items (IDs 874-898 + the
ITEM_SEAL_SHARD_IRONHOLD alias). Build clean: EWRAM 86.46% / IWRAM 86.63% / ROM 79.95%
(26,825,352 B, +704 B from two slightly longer description strings; ZERO RAM change).

SCOPE DECISION — NO custom BW-style bag UI built (deliberately deferred). pokeemerald-
expansion ALREADY ships the FRLG/expansion bag layout, which is the high-value win a
BW-style mod would chase: WIN_DESCRIPTION (src/item_menu.c sDefaultBagWindows) is a large
14x6-tile box bottom-left, the item list is on the right, a dedicated item-icon area sits
above the description, and descriptions auto-wrap (WrapFontIdToFit). The vanilla-Emerald
cramped 1-line description box is NOT what this codebase uses. A full bespoke UI (bigger
icon sprite, prominent USE button) is a multi-hour C rewrite of the bag window/sprite
system for marginal gain over what's already there — flagged as POST-LAUNCH polish, folded
under the existing "Island Journal UI" deferral. Implemented the simpler, high-value pass
instead (per the task's own >2h guard).

VERIFIED for every Pelagios key item (all PASS):
- pocket = POCKET_KEY_ITEMS (all 26). importance = 1 (all 26 — they can't be tossed,
  correct for key items).
- type = ITEM_USE_BAG_MENU + fieldUseFunc = ItemUseOutOfBattle_CannotUse (all 26). This is
  COHERENT with the "key item field effects = Not Started" state: the key-items pocket
  context menu (sContextMenuItems_KeyItemsPocket in src/item_menu.c) always shows USE /
  REGISTER / GIVE / CANCEL, and pressing USE routes through ItemMenu_UseOutOfBattle ->
  the item's fieldUseFunc -> ItemUseOutOfBattle_CannotUse, which shows the standard
  "can't use that here" (Dad's-advice) message. So USE appears but politely no-ops until
  the field-effect system (HM-replacement) is built. NO field-effect C functions were
  invented (out of scope; Not Started).
- description = complete, non-placeholder 3-line text fitting the box (all 26 — no
  "?????" / blank entries existed; the constants passes had already written real copy).
- iconPic / iconPalette = real gItemIcon_* / gItemIconPalette_* symbols (all verified
  present in src/data/graphics/items.h). Placeholder icon CHOICES are acceptable per
  CLAUDE.md (swap to custom art later); none reference a missing symbol.

CHANGES MADE (src/data/items.h, descriptions only — no struct/pocket/importance edits):
- ITEM_SEAL_SHARD_GILDHAVEN (896): was "from PELAGIOS's seal. / It resonates faintly."
  -> "from MIRATH's seal. / It glints like gold." (Gildhaven's sealed legendary is
  MIRATH, Fairy/Dark — confirmed in Gildhaven_TheExchange_SealChamber/scripts.inc; the
  generic Pelagios text was wrong/copied).
- ITEM_SEAL_SHARD_PRIMALIS (898): was "from PELAGIOS's seal. / It resonates faintly."
  -> "from VERDATH's seal. / It pulses like a heart." (Primalis's sealed legendary is
  VERDATH, Grass/Dragon — confirmed in PRIMALIS_BRIEF.md).
  NOTE: ITEM_SEAL_SHARD_THALVERN (893) keeps "from PELAGIOS's seal." — CORRECT, Thalvern's
  seal IS Pelagios (Thalvern_ThroneRoom/scripts.inc, Kyogre cry placeholder). Not changed.

DEFERRED (intentional):
- Full BW-style custom bag UI (post-launch polish; see above + Island Journal UI deferral).
- Key-item field effects (HM-replacement USE behavior) — remains "Not Started"; the USE
  button correctly no-ops until that system exists. When built, each traversal key item's
  fieldUseFunc changes from ItemUseOutOfBattle_CannotUse to its real field-effect function.

### ✅ Completed — Primalis SCRIPTS (script-writer, 2026-06-14, gmake exit 0)
All 13 Primalis scripts.inc fully implemented — zero TODO placeholders remain. Build clean
(EWRAM 86.46% / IWRAM 86.63% / ROM 80.01%, +~20 KB). Speakers added (dedup-checked) to
pelagios_speaker_names.inc: Mako ("ELDER MAKO"), Fern, Scale, Guardian, Villager. REUSED
existing labels (NOT redefined): Thorn, Elder, Historian, Attendant, Lens ("THE LENS"), Dex,
Sera, Vex, Rael, Cael, Innkeeper, Child, Sollis.

5TH-TRAINER RESOLUTION (option b, cleanest): the maps placed FIVE Zoan sight-trainer objects
but only 4 ids exist (925-928). Converted LOCALID_PRIMALIS_ZOAN5 (JungleRoute2) to a PLAIN
talk NPC in the GENERATOR (tools/pelagios/build_primalis_mapjson.py: trainer_type
TRAINER_TYPE_NONE, sight 0) and regenerated JungleRoute2/map.json (build stayed clean). It is
now a guardian-villager with battle-flavor/lore dialogue (no trainerbattle). The 4 remaining
sight trainers map 1:1 to DISTINCT ids by ID-order (NOT brief map-placement, which the
map-builder deviated from): ZOAN1=TRAINER_ZOAN_PRIMALIS_1 SERA (Route1), ZOAN2=_2 VEX
(JungleInterior), ZOAN3=_3 RAEL (ZoanVillage), ZOAN4=_4 CAEL (ZoanVillage). Verified distinct.

CONSTANT RECONCILIATIONS (all grepped against the real headers, nothing invented):
- FLAG_DEX_ALIVE is the correct flag name (0x4BA) for the Lens "Dex alive/gone" branch.
- FLAG_PRIMALIS_RESOLVED 0x4B2, FLAG_CIPHER_8_FOUND 0x4C4, FLAG_PRIMALIS_CIPHER_FOUND 0x28C —
  all pre-existing, reused.
- ITEM_BEAST_WHISTLE 879, ITEM_PRIMALIS_TOKEN 897 — given in the oral-history scene.
  ITEM_SEAL_SHARD_PRIMALIS 898 deliberately NOT given (stub per brief).
- TM substitutions (no Leaf Storm/Dragon Dance/Power Whip/Outrage TM in the 50-set, each
  lampshaded): Fern Leaf Storm -> ITEM_TM_GIGA_DRAIN; Scale Dragon Dance -> ITEM_TM_BULK_UP;
  Thorn Power Whip -> ITEM_TM_BULLET_SEED; Mako Outrage -> ITEM_TM_DRAGON_CLAW.
- Verdath cry placeholder = SPECIES_RAYQUAZA (ancient Dragon legendary).
- SE: SE_M_GROWL / SE_M_GROWTH do NOT exist; used SE_M_GRASSWHISTLE (jungle ambient / whistle),
  SE_SUDOWOODO_SHAKE (root stirring), SE_M_SUPERSONIC (reinforce flash).
- NO var spare consumed (confirmed): scene state uses VAR_TEMP_0 (Heartwood ambient) /
  VAR_TEMP_1 (boat boarding only). Spares 0x410D/E/F untouched.

KEY SCENES:
- ARRIVAL (VerdantLanding): coord triggers (9,12)/(10,12) at progress 0; jungle SE, wary Zoan
  greeter approaches and challenges; FLAG_PRIMALIS_ARRIVED + progress 1. Greeter has post-Lens
  + resolved variants.
- LENS INN (VerdantLanding_Inn lobby — the Lens object lives there, NOT the Inn_Interior):
  FLAG_DEX_ALIVE branch (warm/forward if alive; one quiet "DEX would have loved this" beat then
  moves on if gone — NEVER foreshadows his death). Sets FLAG_PRIMALIS_LENS_MET. Inn ON_TRANSITION
  setrespawn HEAL_LOCATION_PRIMALIS_VERDANT_LANDING + innkeeper YESNO heal.
- BEAST-WHISTLE GATES (Route1 optional NE nook, Route2 north exit): checkitem ITEM_BEAST_WHISTLE
  -> if held, "you clear the growth" pass; else "undergrowth too dense" + bounce 1 tile south.
  Keys off the ITEM, not the trigger's progress value (works the moment the Whistle is obtained).
  DEFERRED: the actual tile-removal field effect (overgrow is physically impassable in the
  layout) is systems work — a known limitation, same as Ironhold's rubble having no clear
  animation. The bounce-message trigger IS the gate and works without it.
- GYMS (all talk-initiated, TRAINER_TYPE_NONE): Fern (ZoanVillage, ->progress 2), Scale
  (ZoanVillage, gated on Gym1, ->3), Thorn (JungleRoute2, gated on Gym2, ->4), Mako
  (ElderHall_Interior, gated on Gym3, ->5 via the oral-history victory). BADGES NARRATIVE-ONLY
  (fanfare + "received the X BADGE" text, NO FLAG_BADGE*_GET — engine cap exhausted). Each gym
  gives its key item/flag BEFORE the TM (full-bag-safe).
- MAKO FIRST MEETING (post-Gym2, progress 3): suspicious; parent sat here 12 years ago, wept,
  left without explaining; sends player to Thorn. Sets FLAG_PRIMALIS_TRUST_EARNED (here meaning
  "the conversation began"; nothing downstream needs it set only post-Gym4). Plays once; an
  AwaitThorn line repeats until Gym3.
- ORAL HISTORY (Mako Gym4 victory — the narrative pivot, paced with delays): reveals the KILL
  SWITCH EXISTS and Haven Isle is the central node, Wardens are its caretakers; the
  parent-weeping detail; the unanswerable "What did your parent DO with this?". Gives
  ITEM_BEAST_WHISTLE (+FLAG_BEAST_WHISTLE_OBTAINED) and ITEM_PRIMALIS_TOKEN (+FLAG_PRIMALIS_TOKEN_GIVEN),
  BOTH via checkitemspace+call_if_eq (full-bag-safe), story flags set AFTER, TM (Dragon Claw)
  LAST via a local TMFull bail (NOT ShowBagIsFull mid-scene). Sets FLAG_PRIMALIS_GYM4_CLEAR +
  FLAG_PRIMALIS_ORAL_HISTORY_HEARD + progress 5.
- HEARTWOOD (TheHeartwood): ON_TRANSITION shows/hides the Mako+Lens escort objects by
  FLAG_PRIMALIS_RUINS_FOUND (wrong-direction map.json hide flag — Gildhaven pattern). ON_FRAME
  (VAR_TEMP_0, progress>=5) atmospheric reaction: the jungle goes quiet, the root-knot turns a
  face toward the player. Mako/Lens/root-knot have seal-found + resolved variants.
- ANCIENTRUINSCAMP altar: "We did not make this..." inscription -> FLAG_PRIMALIS_RUINS_FOUND
  (gates the Heartwood escort). Camp Lens object also shown/hidden via ON_TRANSITION by
  FLAG_PRIMALIS_LENS_MET (same wrong-direction fix).
- SEAL REINFORCEMENT (Heartwood_SealChamber): discovery triggers row 14 (progress 5) ->
  FLAG_PRIMALIS_SEAL_FOUND (Verdath = SPECIES_RAYQUAZA cry). Apparatus (no siphon — honest
  ancient wear) reinforces directly: white flash, fully-transformed Zoan begin to recover
  (narration), then Mako's ONE line EXACTLY "The Root remembers us." with NOTHING after it in
  that beat. Sets FLAG_PRIMALIS_RESOLVED + progress 7.
- RESOLUTION Sollis call: she has known about the kill switch since before the player was born
  (parent told her; argued for a year), ending on EXACTLY "Come home when you can. There are
  things I should tell you in person." NO boat-tier change (Galleon is the cap); Primalis is
  NOT an Ashenveil gate, so NO three-island check here.
- CIPHER 8 (SealChamber inscription, per the island pattern — NOT the altar): full Warden's
  Journal entry incl. the "third option" and Dorne ("...I do not think I will find that way in
  time."), encoded sections rendered as "(encoded)" stage directions. Sets FLAG_CIPHER_8_FOUND
  + FLAG_PRIMALIS_CIPHER_FOUND.
- ZOAN RECOVERY (ZoanVillage Villager object = the fully-transformed member): pre-resolution
  examine-only narration ("they look at you with recognition... just stuck", no dialogue/namebox);
  POST-resolution the gut-punch "I am still in here. I am still in here." with a delay BEFORE
  (delay 30) and AFTER (delay 40 + a hand-finding-yours narration msgbox). Child + all recurring
  NPCs (greeter, lodger, Fern/Scale/Thorn defeated, attendant, Elder2, both Lens locations,
  Zoan5 villager) have post-resolution variants.

### Primalis flow state machine (authoritative)
- VAR_PRIMALIS_PROGRESS: 0=not arrived (arrival triggers armed), 1=arrived, 2=Fern beaten,
  3=Scale beaten (Mako first meeting available), 4=Thorn beaten (Mako Gym4 available),
  5=Mako beaten + oral history (Beast Whistle + Token given; SealChamber discovery armed),
  6=seal found (apparatus offer live), 7=resolved (FLAG_PRIMALIS_RESOLVED).
- Trust/history gating: Mako's state machine routes on FLAG_PRIMALIS_GYM4_CLEAR /
  FLAG_PRIMALIS_GYM3_CLEAR / FLAG_PRIMALIS_TRUST_EARNED / progress<3. TRUST_EARNED set at the
  first meeting (post-Gym2); ORAL_HISTORY_HEARD set at the Gym4 victory.
- Beast-Whistle gate keys off checkitem ITEM_BEAST_WHISTLE (not progress).
- VAR_TEMP_0 (Heartwood only): one-per-session ambient-reaction disarm.

### Primalis deferred / known limitations (script side)
- Beast-Whistle tile-removal FIELD EFFECT is deferred to systems (overgrow is physically
  impassable in the layout; the bounce-message trigger is the working gate). Same class as
  Ironhold's rubble.
- Gym puzzles (none built — leaders are in-overworld talk NPCs, Ironhold/Petra precedent).
- If the player's bag is full at a gym TM beat, the TM is forfeited (no re-claim NPC) — rare,
  non-blocking, established policy. The oral-history key items are full-bag-safe; only the
  trailing Dragon Claw TM can be lost.
- Verdath / Zoan-recovery / Mako line use placeholder cry SPECIES_RAYQUAZA and no custom sprite.

### ⏳ Not Started
- Sirocco Isle: COMPLETE (constants + maps + scripts, 2026-06-12) — only in-emulator
  playtest pending; see Completed — Sirocco SCRIPTS
- Emberveil: COMPLETE (constants + maps + scripts, 2026-06-12) — only in-emulator
  playtest pending; see Completed — Emberveil SCRIPTS (reachability caveat there)
- Schism Isle: COMPLETE (constants + maps + scripts, 2026-06-13) — only in-emulator
  playtest pending; see Completed — Schism SCRIPTS
- Thalvern: COMPLETE (constants + maps + scripts, 2026-06-13) — only in-emulator
  playtest pending; see Completed — Thalvern SCRIPTS
- Gildhaven: COMPLETE (constants + maps + scripts, 2026-06-14) — only in-emulator
  playtest pending; see Completed — Gildhaven SCRIPTS. The 3-island Ashenveil gate is now
  live (Schism + Thalvern + Gildhaven all check each other; whichever resolves last opens it)
- Primalis: CONSTANTS + MAPS DONE (2026-06-14) — scripts NOT started (script-writer is
  next; see "✅ Completed — Primalis MAPS" + "Primalis Isle — CONSTANTS")
- Ashenveil
- Aetheron
- Convergence
- Custom sprites (using placeholders throughout)
- Custom legendary Pokémon
- ~~Boat transition system~~ DONE 2026-06-12 (tier-gated Tennyson destination menu)
- World map / nautical chart
- Key item field effects
- Mega Evolution system (Seal Shards)
- Z-Move distribution
- Difficulty modes
- Quest/mission tracker UI
- **Island Journal UI (POST-LAUNCH, not for current development)** — a custom UI
  replacing the vanilla Trainer Card badge display. Tracks per-island gym clears
  (FLAG_[ISLAND]_GYM[N]_CLEAR) and seal shards collected, rather than the engine's
  8-badge counter. Required because Pelagios adopted narrative-only badges
  region-wide (see Game Systems > Badge System): the Trainer Card counter tops out
  at 8 and won't reflect Emberveil-and-later badges. Deferred to post-launch; do NOT
  build during current island development.

---

## Haven Isle — Task Checklist
Work through these in order. Check off as completed.

### Constants & Data
- [x] Add all flags to include/constants/flags.h
- [x] Add all variables to include/constants/vars.h
- [x] Add all key items to include/constants/items.h
- [x] Add key item data to src/data/items.h
- [x] Add island map group to map_groups.json (stubs for empty islands not possible)

### Maps (exteriors are buildings inside TidemarkVillage; interiors are own maps)
- [x] HavenIsle_Harbor
- [x] HavenIsle_TidemarkVillage (contains lab/houses/PC/mart exteriors + warps)
- [x] HavenIsle_SollisLab (interior)
- [x] HavenIsle_PlayerHouse_1F / HavenIsle_PlayerHouse_2F
- [x] HavenIsle_CassHouse (interior)
- [x] HavenIsle_ElderHouse (interior)
- [x] HavenIsle_PokemonCenter_1F / _2F, HavenIsle_Mart
- [x] HavenIsle_Route1
- [x] HavenIsle_AncientRuinsExterior
- [x] HavenIsle_AncientRuinsInterior1
- [x] HavenIsle_AncientRuinsInterior2
- [x] HavenIsle_Route2
- [x] HavenIsle_FishingDocks

### Scripts
- [x] Opening sequence (player wakes up, mom calls, goes to lab)
- [x] Starter selection scene (three Pokémon loose in lab)
- [x] Warden's Journal key item script
- [x] Ancient Ruins disturbance trigger (screen shake + Ralts flee)
- [x] Sollis explanation of Warden role
- [x] Haven Isle complete / Sloop unlock
- [x] All NPC dialogue (sailor, old woman, child, elder, fisherman)
- [x] Trainer battle scripts written (trainer DATA still needed — see In Progress)

### Wild Encounters
- [x] Route 1 grass encounters (Rattata/Yungoos 40%, Pidgey/Taillow 30%, Mareep ~14%, Gastly ~6%)
- [x] Route 1 surf encounters (Marill 60%, Azumarill 40%)
- [x] Route 2 grass encounters (Yungoos ~49%, Taillow 30%, Ralts ~21%)
- [x] Route 2 surf encounters (Marill 70%, Horsea 30%)
- [x] Route 2 fishing encounters (Old Rod: Magikarp/Horsea; Good Rod: Horsea 60/Totodile 40; Super Rod filled too)

### Compilation
- [x] Full compile passes with zero errors (gmake exit 0, 2026-06-11)
- [ ] Game boots to opening sequence in mGBA (untested — needs in-emulator run)
- [ ] Starter selection works correctly (untested in-emulator)
- [ ] Can navigate all Haven Isle maps (untested in-emulator)

---

## Session Handoff Protocol
At the end of each session, update this file:
1. Move completed items from the checklist to ✅ Completed
2. Note any known issues or compilation warnings
3. Write a one-line "Next session starts here:" note

### ✅ Completed — Convergence MAPS (map-builder, 2026-06-14, gmake exit 0)
ALL 6 Convergence (FINAL ISLAND) maps built, registered, compiling cleanly. Build clean:
EWRAM 86.46% / IWRAM 86.63% / ROM 80.16% (26,898,136 B, +~8 KB). Built via
tools/pelagios/build_convergence.py (4 composed layouts + idempotent merge into layouts.json)
+ build_convergence_mapjson.py (6 map.json) + build_convergence_scripts_stubs.py (stub
scripts.inc — NO Tennyson exception on this island). Regenerate via those scripts; do NOT
hand-edit map.bin/map.json/scripts.inc. gMapGroup_Convergence registered (map group 85;
map_groups.json group_order + map list), 6 `.include` lines in event_scripts.s.

THE 6 MAPS — layout / tileset / weather (ALL maps WEATHER_NONE per brief):
| Map | Layout | Tileset (primary+secondary) | Source |
|---|---|---|---|
| Approach | LAYOUT_CONVERGENCE_APPROACH (24x20) | General + Slateport | composed (shore gathering) |
| AncientCapital_Exterior | LAYOUT_CONVERGENCE_ANCIENT_CAPITAL_EXTERIOR (36x34) | General + Slateport | composed (LARGE ruined city) |
| AncientCapital_Interior1 | LAYOUT_SEALED_CHAMBER_OUTER_ROOM (21x23) | General + Cave | REUSED vanilla (temple outer / murals) |
| AncientCapital_Interior2 | LAYOUT_SEALED_CHAMBER_INNER_ROOM (21x23) | General + Cave | REUSED vanilla (inner sanctum) |
| KillSwitchChamber | LAYOUT_CONVERGENCE_KILL_SWITCH_CHAMBER (26x40) | General + Cave | composed (VAST, deepest-ancient) |
| Epilogue | LAYOUT_CONVERGENCE_EPILOGUE (24x20) | General + Slateport | composed (flag-gated roster) |

TILESET SUBSTITUTION (documented): there is NO gTileset_Ruins in this project. The brief's
"ancient secondary / Ruins" intent is served by **gTileset_Slateport** (grey ancient stone =
the Ashenveil dead-city precedent) for the outdoor capital/approach/epilogue, and the **Sealed
Chamber family (gTileset_Cave)** for the temple interiors + the KillSwitchChamber (the deepest /
most ancient aesthetic available). Outdoor words sampled verbatim from LAYOUT_SLATEPORT_CITY
(General/Slateport: STONE 0x3001, WALL 0x04C6, WALL2 0x04C7, WATER 0x1170, shore 0x1179); cave
words from LAYOUT_VICTORY_ROAD_1F (General/Cave: floor 0x4211 col0 elev4, wall 0x0611 col1).
Flat-fill terrain; richer metatiles (temple facade, cliffs, mixed-era detail) are a Porymap
hand-tuning pass later (established caveat every island).

CONNECTIONS (reciprocal, offsets validated against compiled collision bits):
  Approach.up(-6) <-> AncientCapital_Exterior.down(+6). That is the ONLY map connection.
  Everything else links by WARP: Exterior <-> Interior1 (north door); Interior1 <-> Interior2
  (SEALED_CHAMBER outer/inner door pattern); Interior2 -> KillSwitchChamber (forward warp at the
  inner-room north walkable band (10,5)/(11,5)). The KillSwitchChamber has arrival warps at the
  south spine (12/13,37) pointing back to Interior2 ONLY as the landing target — the brief's
  "NO return" is honored by the ending coord trigger (progress 3) firing on the first step in,
  so the player never freely walks back; the ending sequence warps to the Epilogue (script-writer).
  The Epilogue is a TERMINAL map (NO warps); the ending sequence warps the player IN, post-credits
  the Storm Compass handles all return travel.

ARRIVAL (boat menu swap-in, data/scripts/pelagios_boat.inc): Pelagios_EventScript_SailToConvergence
(already FLAG_AETHERON_RESOLVED-gated, pre-existing) had its post-gate `goto SailNoChart` stub
REPLACED with the real same-island guard (vs PELAGIOS_ISLAND_CONVERGENCE = 11, pre-existing) +
cast-off + `warp MAP_CONVERGENCE_APPROACH, 11, 13`. Arrival tile (11,13) = the jetty foot, verified
walkable; the gathering coord triggers sit at row 11 (two tiles north), so the landing tile is NOT
a trigger tile (warp-landing rule). NO Tennyson/SS_TIDAL boarding object exists on the island and
there is NO return warp anywhere — Storm Compass is the return (brief-mandated).

RETURNING-NPC ROSTER + VISIBILITY (AncientCapital_Exterior, all TODO-stub objects):
  - Eira (north, 17,3): spawn `flag` = FLAG_SCHISM_CEASEFIRE. INVERSE-FLAG PROBLEM — the engine
    HIDES an object when its flag is SET, but the brief wants Eira PRESENT only if
    FLAG_SCHISM_CEASEFIRE. The Exterior ON_TRANSITION (script-writer) is AUTHORITATIVE: addobject
    when ceasefire SET / removeobject when unset (Gildhaven Dagan inverse-flag precedent).
  - Drenn (south, 17,30): spawn `flag` = FLAG_DRENN_ALIVE — SAME inverse-flag problem. Exterior
    ON_TRANSITION must addobject when FLAG_DRENN_ALIVE SET / removeobject when unset.
  - Mako (east, 31,15), Lace (west, 3,15), The Lens (central, 18,17), Dagan (leaning east, 24,20):
    always present (flag '0').
  - Dex is NOT placed as a Convergence object (brief says he arrives if FLAG_DEX_ALIVE) — the
    script-writer may addobject a Dex cameo in the gathering/epilogue if wanted; left as an option.
  Approach scene roster: Dorne (always, 11,9), Sollis (script-managed entrance, spawn flag
  FLAG_CONVERGENCE_GATHERING_SEEN), Cass (always, 13,10), + 2 decorative Covenant-fleet SS_TIDAL
  ships offshore (elev 1, script 0x0, bg detail only — NOT boarding objects).
  Epilogue roster: the FULL Ending-3 cast (Sollis/Cass/Dorne/Eira/Mako/Lens/Drenn/Lace/Dagan) is
  placed, all defaulting hidden behind FLAG_CONVERGENCE_COMPLETE; the script-writer's ON_TRANSITION
  reveals the correct per-ending subset (Ending 1 = Sollis on a Haven-warm stage; Ending 2 =
  Dorne + open-sea legendaries; Ending 3 = everyone).

MURAL bg_events (Interior1): 8 examine signs, one per wall section (NW/N1/N2/NE on the north wall
faced from the south; W1/W2 on the west wall, E1/E2 on the east wall) — all TODO mural-lore stubs.
The west/east mural signs sit on impassable wall tiles (correct — faced from the adjacent floor).
KILL-SWITCH event objects: Interior2 has a "kill switch visible ahead" examine sign (10,4, faced
north) + a FinalChoice coord trigger (10/11,6, progress 2). KillSwitchChamber has the mechanism
itself as a huge SS_TIDAL decoration object (12,4, elev 1, script TODO) on the raised north dais,
+ Dorne (14,7) + Cass (11,9), + the Ending coord trigger (12/13,34, progress 3).

ENDING / EPILOGUE STRUCTURE (geometry only; all branching is script-writer): three endings all
play in the KillSwitchChamber off the single progress-3 Ending trigger (branch on
FLAG_DORNE_CHOICE_STOP/HELP + FLAG_TRUE_ENDING_UNLOCKED). Dorne's final battle (Ending 1 only,
TRAINER_LEADER_DORNE 940) is talk/script-initiated (no sight trainer). Each ending's epilogue
warps the player into the single Convergence_Epilogue map; the Credits coord trigger (11/12,12,
progress 5) rolls credits. VAR_CONVERGENCE_PROGRESS: 0 not arrived, 1 arrived+gathering, 2 temple
entered, 3 inner sanctum / kill-switch chamber, 4 (reserved), 5 ending played.

MAPSECs (3, covering all 6 maps): MAPSEC_CONVERGENCE_APPROACH (273, also used by Epilogue),
MAPSEC_CONVERGENCE_ANCIENT_CAPITAL (274, the Exterior + both interiors),
MAPSEC_CONVERGENCE_KILL_SWITCH (275). Added to region_map_sections.json (name-only, no x/y —
placeholder, matching every prior Pelagios island; the custom nautical chart is future work);
region_map_sections.h enum + region_map_entries.h auto-regenerate from the .json. Values >0xFF
emit the SAME benign header.byte truncation warning as Primalis/Ashenveil/Aetheron (region-map
naming only; maps build + function — NOT a map-builder bug).

NO heal location, NO inn, NO wild encounters, NO trainers/gym objects, NO hidden items on
Convergence (all brief-mandated; IsLastHealLocationPlayerHouse untouched).

PRE-EXISTING BUILD BREAKAGE FIXED (unrelated to Convergence, was blocking the whole tree):
src/data/heal_locations.json was TRUNCATED mid-write at the Primalis entry (a prior session
crashed: `"map": "MAP_PRIM` with an unescaped newline, file ended at 429 lines). This produced a
JSONPROC_ERROR before any Convergence work could link. REPAIRED: completed the Primalis heal
location (HEAL_LOCATION_PRIMALIS_VERDANT_LANDING -> MAP_PRIMALIS_VERDANT_LANDING_INN via
LOCALID_PRIMALIS_VERDANT_INNKEEPER, tile 4,8) AND added the missing Aetheron heal location
(HEAL_LOCATION_AETHERON_CLOUD_LANDING -> MAP_AETHERON_CLOUD_LANDING_INN via LOCALID_AETHERON_INNKEEPER,
tile 4,8) — the latter was referenced by Aetheron's PokemonCenter/Inn ON_TRANSITION setrespawn
calls but never made it into the file before the crash. Both are nurse-gfx-innkeeper Inns, so
NEITHER is added to IsLastHealLocationPlayerHouse() (correct — Schism/Thalvern/Gildhaven precedent).
Flag for Primalis/Aetheron owners: verify these two heal entries match intent in playtest.

DEVIATIONS / NOTES for pelagios-script-writer (NEXT):
- All 6 scripts.inc are STUBS (every EventScript label tagged "@ TODO (script-writer): implement
  ..."). NO boat/Tennyson wiring on this island. The stub generator won't clobber a scripts.inc
  once it has a .string line.
- Composed outdoor layouts are blocky grey-stone / cave rectangles; warp tiles sit on plain
  ground. Review in Porymap and dress with temple/cliff/mixed-era metatiles later (every-island
  caveat). The KillSwitchChamber is intentionally the most monumental (26x40 nave + dais).
- MUSIC placeholders: MUS_ABNORMAL_WEATHER (Approach/fleet), MUS_SEALED_CHAMBER (capital +
  temple interiors + kill switch), MUS_LITTLEROOT (Epilogue, Haven-warm). Swap to themed tracks.
- SPRITE placeholders (no Convergence-specific gfx): Dorne=MAN_4, Sollis=WOMAN_3, Cass=RIVAL_MAY_
  NORMAL, Eira=WOMAN_5, Mako=HIKER, Drenn=SCIENTIST_1, Lace=WOMAN_2, Lens=SCIENTIST_1, Dagan=
  RICH_BOY, kill switch + fleet ships = SS_TIDAL. Swap to custom sprites later.
- Add Convergence speaker names (Dorne/Sollis/Cass already exist from prior islands; verify
  Eira/Mako/Drenn/Lace/Lens/Dagan are present in pelagios_speaker_names.inc — most are).

### ✅ Completed — Convergence SCRIPTS (script-writer, 2026-06-14, gmake exit 0) — FINAL ISLAND
ALL 6 Convergence scripts.inc fully implemented — zero TODO placeholders across all 6 maps.
A PRIOR session implemented 4 of the 6 (Approach, AncientCapital_Exterior, Interior1, Interior2 —
left UNTOUCHED this pass). THIS session wrote the last two: **KillSwitchChamber** (the three-ending
resolution + Dorne's final battle) and **Epilogue** (the per-ending epilogue + the final line +
credits). Build clean: EWRAM 86.46% / IWRAM 86.63% / ROM 80.21% (26,914,296 B).

ROUTING (matches the already-written Interior1/Interior2): Interior1's Dorne confrontation sets
FLAG_DORNE_CHOICE_STOP or _HELP (DEFER resolves into one of those; the "true way" posture sets
_HELP so Dorne walks in WITH the player). Interior2 advances VAR_CONVERGENCE_PROGRESS to 3 and
warps the player into the KillSwitchChamber (warp 0, south spine 12/13,37). The player walks north
and crosses the Ending coord trigger at (12/13,34) at progress==3 — that fires
ConvergenceKillSwitch_EventScript_Ending, the single entry point for all three endings.

THREE-ENDING RESOLUTION (KillSwitchChamber) — TRUE checked STRICTLY FIRST:
  goto_if_set FLAG_TRUE_ENDING_UNLOCKED -> EndingTrue   (OVERRIDES STOP/HELP/DEFER)
  else goto_if_set FLAG_DORNE_CHOICE_STOP -> EndingStop
  else -> EndingHelp
- ENDING 1 (STOP): Dorne walks to the switch; FINAL BATTLE via trainerbattle_no_intro
  TRAINER_LEADER_DORNE (940); post-battle EXACTLY "You fight like your parent.\pThey would have
  done the same thing."; delay 40; Dorne walks off quietly (removeobject, no sting/camera); fleet
  withdraws narration; setflag FLAG_DORNE_FINAL_BATTLE_DONE + FLAG_ENDING_STOP_PLAYED.
- ENDING 2 (HELP): joint activation; EIGHT legendary cries in sequence (delay 25 between):
  KYOGRE (Thalvern/Feraligatr), GROUDON (Emberveil/Infernape), RAYQUAZA (Primalis/Verdath/
  Decidueye), REGICE (Schism N/Glacith), REGIDRAGO (Schism S/Toxara), ZAPDOS (Aetheron/Stormveil),
  LUGIA (Pelagios — no prior cry, placeholder), GIRATINA (Morthas — no prior cry, placeholder) —
  the first six reuse each island seal chamber's exact placeholder cry; network-collapse narration;
  Dorne "I thought this would feel different." / Cass "Does it feel wrong?" / Dorne "No. Just
  quieter than I expected."; setflag FLAG_ENDING_HELP_PLAYED.
- ENDING 3 (TRUE, only if FLAG_TRUE_ENDING_UNLOCKED): third-function (OPEN) reveal; player explains;
  Dorne reads, delay 60, EXACTLY "Your parent was right.\pI was going to burn everything down when I
  could have just opened the door."; Warden's-touch narration (player must touch); fog-clearing
  dissolution; fleet leaves one by one; setflag FLAG_ENDING_TRUE_PLAYED.
- Shared tail: setvar VAR_CONVERGENCE_PROGRESS 5, fadescreen, warp MAP_CONVERGENCE_EPILOGUE 11,16.

EPILOGUE (single map, branches on FLAG_ENDING_*_PLAYED via the progress==5 credits trigger at
(11/12,12); the player warps in at (11,16) and walks north into it): TRUE checked first, then HELP,
else STOP.
- EPILOGUE 1 (STOP): Haven Isle, Sollis EXACTLY "You stopped him. The seals are stable.\pIs that
  enough?"; player silent (narration); walks to harbor; credits.
- EPILOGUE 2 (HELP): legendaries free in the distance; Dorne/Cass/Dorne "different/wrong/quieter"
  exchange; credits.
- EPILOGUE 3 (TRUE): the FULL returning cast, each a verbatim final line — Sollis, Cass, Dorne,
  Eira (nods once, NO dialogue — narration only, no namebox), Mako, The Lens, Dex (ONLY if
  FLAG_DEX_ALIVE), Drenn (ONLY if FLAG_DRENN_ALIVE), Lace, Dagan (FUNNY, three VAR_DAGAN_
  RELATIONSHIP variants 0/1/2, ALL ending on EXACTLY "It's the most interesting I've felt in
  years."), then THE FINAL LINE OF THE GAME — Cass EXACTLY "{PLAYER}. Come on.\pLet's go home." —
  NOTHING follows it; credits roll immediately.

CREDITS MECHANISM: vanilla `special GameClear` (the EverGrandeCity Hall-of-Fame end path) after
`fadescreenspeed FADE_TO_BLACK, 24`. It heals the party, records the Hall of Fame, rolls the FRLG
credits, sets FLAG_SYS_GAME_CLEAR, and returns the player to their last heal location for post-game.
FLAG_CONVERGENCE_COMPLETE is set IMMEDIATELY BEFORE GameClear (persists). The Epilogue's named NPC
objects are spawn-gated behind FLAG_CONVERGENCE_COMPLETE, so they are HIDDEN during the cutscene
(which is told entirely via narration/speech — no object movement needed) and APPEAR for post-game
flavor talk once the flag is set. Each post-game NPC has a short flavor line.

CONSTANT RECONCILIATION (all verified to EXIST — nothing invented):
  FLAG_DORNE_FINAL_BATTLE_DONE (0x2A2) EXISTS — used as the Ending-1 post-battle marker.
  FLAG_CONVERGENCE_COMPLETE (0x2A6) EXISTS — set before GameClear.
  FLAG_ENDING_STOP/HELP/TRUE_PLAYED (0x2A3/4/5), FLAG_TRUE_ENDING_UNLOCKED (0x4B8),
  FLAG_DORNE_CHOICE_STOP/HELP (0x4B5/6), FLAG_DEX_ALIVE (0x4BA), FLAG_DRENN_ALIVE (0x4BB),
  TRAINER_LEADER_DORNE (940), VAR_DAGAN_RELATIONSHIP (0x40FB) — all present. No new flags/vars/
  items/trainers/speakers added. NO var spare consumed (only VAR_CONVERGENCE_PROGRESS + transient
  VAR_TEMP_1; 0x410D/E/F untouched).

DEFERRED / LIMITATIONS (script side):
- The cutscene is fully narrative (no mid-scene object choreography) because the Epilogue roster is
  spawn-gated behind FLAG_CONVERGENCE_COMPLETE (not yet set during the scene). Post-game the roster
  appears for flavor talk. A future pass could stage walk-on choreography by force-spawning via
  addobject if richer blocking is wanted.
- Pelagios + Morthas legendary cries use SPECIES_LUGIA / SPECIES_GIRATINA placeholders (no prior
  seal-chamber cry existed for the two final-island legendaries). Swap when custom cries exist.
- Dorne's final-battle party is the systems-engineer's TRAINER_LEADER_DORNE (940) data — unchanged.
- GameClear sets the vanilla continue-warp to the Littleroot/Pelagios house; post-credits return is
  the player's last heal location. Storm Compass (bag) handles all post-game island travel — player
  is NOT locked.

**Next session starts here (THE GAME IS SCRIPT-COMPLETE, 2026-06-14):** ALL 11 ISLANDS + ALL 3
ENDINGS ARE SCRIPT-COMPLETE. Convergence SCRIPTS DONE (script-writer, gmake exit 0, EWRAM 86.46% /
IWRAM 86.63% / ROM 80.21% / 26,914,296 B). The last two Convergence maps (KillSwitchChamber +
Epilogue) are implemented; zero TODO placeholders remain across the entire project's scripts. The
main-story script pass for Pokemon Pelagios is FINISHED. REMAINING WORK is no longer scripting:
(1) a full in-emulator playtest pass of all 11 islands + all 3 endings + credits + post-game; (2)
polish — custom sprites, music, legendary species/cries, the deferred per-island Porymap map-
dressing pass; the Storm Compass is already wired for post-game travel. See "✅ Completed —
Convergence SCRIPTS" above for the three-ending structure, the TRUE-first gate, the Dorne final
battle, the legendary-cry list, the exact final line, and the `special GameClear` credits mechanism.

**[ARCHIVED] Prior pointer (Convergence MAPS DONE, 2026-06-14):** CONVERGENCE (FINAL ISLAND)
MAPS DONE (map-builder, gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.16% / 26,898,136 B).
All 6 maps built/registered (gMapGroup_Convergence map group 85 + 6 .include lines + 4 composed
layouts + 3 MAPSECs + boat-menu arrival warp swap-in); compiling cleanly. Full detail +
script-writer handoff: "✅ Completed — Convergence MAPS" section above. Also fixed a pre-existing
heal_locations.json truncation that was blocking the whole build (Primalis + Aetheron heal
locations restored — see that section).
**NEXT: pelagios-script-writer for Convergence scripts** — the brief calls this the MOST IMPORTANT
script pass; take more time than any prior island. All 6 scripts.inc are STUBS. Implement per
CONVERGENCE_BRIEF.md: the gathering scene (Approach, progress 0 trigger -> Dorne+Sollis reunion +
Sollis confession + ITEM_WARDENS_RESEARCH 903 + all returning NPCs take positions ->
FLAG_CONVERGENCE_ARRIVED + FLAG_CONVERGENCE_GATHERING_SEEN + progress 1); all returning NPC
dialogue (relationship-var-aware: VAR_DORNE/CASS/MAREN_RELATIONSHIP); Interior1 mural examinations
(full lore, hidden sections if FLAG_TRUE_ENDING_UNLOCKED); Interior2 Dorne confrontation
(FLAG_DORNE_CHOICE_STOP/HELP branch); the THREE endings in the KillSwitchChamber (Ending 1 = Dorne
battle TRAINER_LEADER_DORNE 940 — the "Your parent would be proud of you. I'm not sure if that
helps." line MUST NOT change; Ending 2 = help Dorne / legendary cries; Ending 3 = FLAG_TRUE_ENDING_
UNLOCKED only, player+Dorne activate the third function); per-ending Epilogue (ON_TRANSITION reveals
the flag-gated roster) + Credits trigger (progress 5). Ending 3's LAST line is Cass: "{PLAYER}. Come
on. Let's go home." — MUST NOT change, nothing after it. Dagan funny in all three. Wire the
Exterior ON_TRANSITION for the Eira (FLAG_SCHISM_CEASEFIRE) + Drenn (FLAG_DRENN_ALIVE) INVERSE-flag
visibility (addobject/removeobject — Gildhaven Dagan precedent; the spawn flags alone are wrong-
direction). Use setspeaker nameboxes throughout.
After scripts: pelagios-build-debugger does the final verification pass + marks the GAME COMPLETE.

**[ARCHIVED] Prior pointer (Convergence CONSTANTS DONE, 2026-06-14):** CONVERGENCE (FINAL
ISLAND) CONSTANTS DONE (systems-engineer, gmake exit 0, EWRAM 86.46% / IWRAM 86.63% /
ROM 80.13% / 26,888,116 B). All Convergence constants allocated — full detail in
"✅ Convergence — CONSTANTS" above. Summary: 8 story flags BLOCK 4 0x29F-0x2A6
(ARRIVED/GATHERING_SEEN/SOLLIS_CONFESSION_HEARD/DORNE_FINAL_BATTLE_DONE/ENDING_STOP_PLAYED/
ENDING_HELP_PLAYED/ENDING_TRUE_PLAYED/CONVERGENCE_COMPLETE); REUSED FLAG_DORNE_CHOICE_*
(0x4B5-0x4B7) + FLAG_TRUE_ENDING_UNLOCKED (0x4B8) + FLAG_CASS_DEFECTED (0x4BC); NO
FLAG_CONVERGENCE_RESOLVED exists (FLAG_CONVERGENCE_COMPLETE 0x2A6 is the resolved-equivalent).
ITEM_WARDENS_RESEARCH 903 (next free 904). TRAINER_LEADER_DORNE 940 (party VERBATIM from brief:
Bisharp 65 / Hydreigon 66 / Weavile 67 / Aegislash 68 / Garchomp 69 / Kyurem 70; Pic Magma
Leader Maxie placeholder), TRAINERS_COUNT_EMERALD now 941 (83 free). ZERO new vars, 3 spares
(0x410D/E/F) preserved. BLOCK 4 free 0x2A7-0x2BB (21 flags) — island set COMPLETE.
**NEXT: pelagios-map-builder for Convergence maps** — build the **6 maps** (Convergence_Approach,
_AncientCapital_Exterior, _Interior1, _Interior2, _KillSwitchChamber, _Epilogue); register
gMapGroup_Convergence WITH the first map (NOT pre-registered — empty groups break groups.inc);
**NO heal location, NO inn, NO wild encounters, NO boat-menu entry** (Storm Compass handles
return travel — the SailToConvergence boat path is already gated by FLAG_AETHERON_RESOLVED).
Dorne is the ONLY trainer (TRAINER_TYPE_NONE talk-initiated so the script-writer can gate the
final battle on FLAG_DORNE_CHOICE_STOP). Then script-writer (the brief calls it the MOST
IMPORTANT script pass — gathering scene, 3 endings, all returning NPCs). FLAGGED: pre-existing
benign scaninc warning on the UNTRACKED Aetheron CovenantInstallation_Interior scripts.inc:349
(script-writer domain; non-fatal, build still exits 0). Also still pending from Aetheron: the
Storm Compass C bag-use registration (handoff block in data/scripts/pelagios_boat.inc).

**Prior pointer (Aetheron SCRIPTS, 2026-06-14):** AETHERON ISLE SCRIPTS DONE
(script-writer, gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.14% / 26,889,556 B).
All 11 Aetheron (SKY ISLAND) scripts.inc fully written — ZERO TODO placeholders — full detail
in "✅ Completed — Aetheron SCRIPTS" above. The CASS DEFECTION is implemented with the EXACT
timing (fadeoutbgm 4 silence / delay 60 before Cass's first word / "Okay."(delay 20)"Let's go."
/ fadedefaultbgm resume / VAR_CASS_RELATIONSHIP HARD-SET to 3 / FLAG_CASS_DEFECTED). The first
Cass sighting fires once; the two-character seal reinforcement gives the Storm Compass + Seal
Shard + sets FLAG_AETHERON_RESOLVED; the Sollis call signals Convergence is open. Storm Compass
field-effect SCRIPT (Pelagios_EventScript_StormCompass) is written + compiles; its bag-"Use"
C registration is the ONLY remaining wire (SYSTEMS-ENGINEER: mirror ItemUseOutOfBattle_PokemonBoxLink
in src/item_use.c + set ITEM_STORM_COMPASS.fieldUseFunc — full handoff block is commented above
the script in data/scripts/pelagios_boat.inc). Convergence gate verified intact
(SailToConvergence goto_if_unset FLAG_AETHERON_RESOLVED). No permanent var spare consumed.
Speakers GALE/ARC/DOCKHAND/TECHNICIAN added (Voss/Cass already existed). ITEM_CASS_DOCUMENTS
(902) exists but is narrated in the defection rather than given as a bag item.
REMAINING AETHERON WORK: in-emulator playtest (ascent cutscene + forward warp, dockhand arrival,
once-only Cass sighting, Gale/Arc/Voss gyms + TMs, control-panel yield log,
THE DEFECTION — verify the delay-60 silence + music stop/resume + VAR_CASS_RELATIONSHIP=3 +
FLAG_CASS_DEFECTED, StormPeak Cass walking alongside, two-character seal reinforcement +
Storm Compass/Seal Shard, post-resolution Cass + Sollis call, Storm Compass bag menu once the
C wire lands). **NEXT/FINAL ISLAND: Convergence** — gated by FLAG_AETHERON_RESOLVED; do the
constants pass first (systems-engineer: story block 4 free 0x29F-0x2BB; trainer slots 940+ free;
next item 903; then swap SailToConvergence's post-resolve branch to a real warp once
MAP_CONVERGENCE_* exists). Also pending: the Storm Compass C bag-use registration above.

**Prior pointer (Aetheron constants, 2026-06-14):** AETHERON ISLE CONSTANTS DONE
(systems-engineer, gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.08% / 26,868,920 B).
Full detail in "✅ Aetheron Isle — CONSTANTS" above. Summary: 8 new story flags 0x297-0x29E
(ARRIVED/GYM1-3_CLEAR/CASS_SEEN/INSTALLATION_FOUND/SEAL_FOUND/STORM_COMPASS_OBTAINED);
FLAG_AETHERON_RESOLVED 0x4B4 + FLAG_CASS_DEFECTED 0x4BC REUSED (both pre-existed); NO new
cipher (CIPHER set complete at 9); ITEM_STORM_COMPASS 880 REUSED + 2 new items
ITEM_SEAL_SHARD_AETHERON 901 / ITEM_CASS_DOCUMENTS 902 (next free 903); 7 new trainers 933-939
incl. gym leaders Gale 937 / Arc 938 / Voss 939 (narrative-only badges; Voss runs Zekrom),
TRAINERS_COUNT_EMERALD 940; 6 MAPSECs (MAPSEC_NONE now 273); boat menu Aetheron SEA_CHART gate
VERIFIED + Convergence ADDED with FLAG_AETHERON_RESOLVED gate (Galleon list/dispatch in sync,
Cancel at index 12); ZERO new vars, 3 spares preserved. **NEXT: pelagios-map-builder builds
the 11 Aetheron maps** (KnockUpStream scripted-only/no-warps-back, CloudLanding cloud-pier,
SkyRoute open-sky, AetherVillage, CovenantInstallation+Interior, StormPeak Cass-NPC space,
SealChamber two-character space; WEATHER_THUNDERSTORM outdoors; wild tables SkyRoute/StormPeak;
heal location CloudLanding Inn) and registers gMapGroup_Aetheron / MAP_GROUP_AETHERON WITH the
first map (deferred by constants pass). Pair the SECONDARY tileset with the General/sky palette
per brief. Then swap the boat-menu Aetheron + Convergence stub branches to real warps.

---

**Prior pointer (debug, 2026-06-14):** IRONHOLD "no buildings" VISUAL BUG
DIAGNOSED + PARTLY FIXED (gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.07%). Root
cause = UNDRESSED LAYOUTS, not a tileset/render bug: Ironhold outdoor map.bin contained
ONLY primary-tileset ground/wall/water/sign IDs (<512) — no building metatiles were ever
placed, and Slateport (secondary) was unused. Verified ZERO out-of-range metatile IDs on
Ironhold AND on the cross-island spot-check (Sirocco/Schism/Emberveil all share the same
undressed state, all in-range). FIXED: (1) dock-guard + Covenant patrols OBJ_EVENT_GFX_MAN_3
/HIKER -> BLACK_BELT (uniformed); (2) a minimal dressing pass — stamped verbatim Slateport
building blocks (door tiles aligned to the existing warps) onto GatemarkPort/OuterDistrict/
IronholdCity, validated all warp doors stay walkable. Also fixed an UNRELATED pre-existing
build blocker: a truncated src/data/heal_locations.json (in-flight Primalis entry) was
breaking the whole build — restored the complete Primalis entry. FULL writeup in Known
Issues "Ironhold 'no buildings' visual diagnosis + dressing pass". DEFERRED: a richer
town-dressing pass for ALL islands (Ironhold is still sparse; Sirocco/Schism/Emberveil
undressed) is map-builder content work. User must in-emulator verify buildings render +
doors enter (I cannot run mGBA). Aetheron pointer below still stands.

**Prior pointer (Ashenveil):** ASHENVEIL FULLY COMPLETE (constants + maps + scripts,
2026-06-14, gmake exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 80.06%, 26,864,372 B). The
final two scripts (THE climax of the game) are done: TheMemorial (the Dorne scene + 3-way
choice) and MorthasGrove (the 5-turn silent-vision endurance + Cipher 9 / true ending +
Sea Chart). ZERO TODO placeholders across all 9 Ashenveil scripts.inc. Full implementation
map in "✅ Completed — Ashenveil SCRIPTS" above. Key facts: Dorne 3 choice flags via 2
chained YESNO, VAR_DORNE_RELATIONSHIP HELP=3/STOP>=2/DEFER unchanged, quiet walk-off exit;
Morthas counter on VAR_TEMP_1 (NO permanent spare), `fadeoutbgm 4` -> vision -> `fadedefaultbgm`
for the silence; Cipher 9 ends on the two "I love you" lines (unchanged); FLAG_ASHENVEIL_VISITED
is the resolution flag (FLAG_ASHENVEIL_RESOLVED does NOT exist); Sea Chart sets
FLAG_SEA_CHART_FOUND which the boat menu's SailToAetheron already gates on (boat menu untouched).
ONLY remaining Ashenveil work is in-emulator playtest (arrival silence, Orin/Lantern, AshFields
gate, Ruins docs, BOTH-then-DEFER Dorne branches + relationship var, Morthas flee-and-retry +
the silent final vision, Sea Chart -> Aetheron-listed, Cipher 9 / true-ending flags).

NEXT ISLAND: AETHERON (Sky Island) — gated by FLAG_SEA_CHART_FOUND (NOT boat tier). Start
with the systems-engineer CONSTANTS pass (STORY BLOCK 4 free after Ashenveil: 0x297-0x2BB,
37 flags; next item id 901; ZERO var spares to consume — only 0x410D/E/F reserved remain).
Then map-builder, then script-writer. Aetheron is where Cass DEFECTS and rejoins the player;
the boat menu's SailToAetheron currently falls through to SailNoChart ("no port yet") until
Aetheron's port map exists — replace that stub case with a real SailToAetheron warp once the
port is built. After Aetheron: Convergence (the Final Island) — where FLAG_TRUE_ENDING_UNLOCKED
and the three FLAG_DORNE_CHOICE_* branches pay off.

**Prior pointer (Ashenveil maps):** ASHENVEIL MAPS DONE (map-builder, 2026-06-14, gmake exit 0,
EWRAM 86.46% / IWRAM 86.63% / ROM 80.03%, 26,854,372 B). All 9 Ashenveil (DEAD ISLAND) maps
built/registered (gMapGroup_Ashenveil + 9 .include lines + 5 new layouts + 4 reused vanilla
interiors + 3 Ghost wild tables + boat stub swapped to a real warp). NO heal location, NO
trainers, NO hidden items — IsLastHealLocationPlayerHouse() untouched. Full detail in
"✅ Completed — Ashenveil MAPS" above.

**Prior pointer (Ashenveil constants):** ASHENVEIL CONSTANTS DONE (systems-engineer,
2026-06-14, gmake exit 0, ROM 80.01%). Full detail in
"✅ Ashenveil Isle — CONSTANTS" above. Allocated: 8 flags BLOCK 4 0x28F-0x296 (reused
FLAG_DORNE_CHOICE_STOP/HELP/DEFER 0x4B5-0x4B7 which ALREADY existed, FLAG_ASHENVEIL_VISITED
0x4B3, FLAG_TRUE_ENDING_UNLOCKED 0x4B8, cipher 9 FLAG_CIPHER_9_FOUND 0x4C5 pre-allocated);
2 new items ITEM_SEA_CHART 899 + ITEM_SEAL_SHARD_ASHENVEIL 900 (Decidueye Mega trigger), with
ITEM_PHANTOM_LANTERN 881 REUSED; 5 MAPSECs (MAPSEC_NONE 262 -> 267); ZERO new vars/spares;
TRAINERS_COUNT_EMERALD unchanged at 933. NEW boat-menu sea-chart activation path: Ashenveil is
Galleon menu index 9 (case 9 -> SailToAshenveil -> SailNoChart stub); Aetheron is index 10
(case 10 -> SailToAetheron), ALWAYS listed but `goto_if_unset FLAG_SEA_CHART_FOUND` shows
"no chart for that heading" until the Sea Chart is found — then falls through to SailNoChart
until Aetheron maps exist. **BLOCK 4 free after Ashenveil: 0x297-0x2BB (37 flags); next item
id 901.**

NEXT: ASHENVEIL MAPS (pelagios-map-builder). 9 maps, NO trainers, NO heal location, all maps
WEATHER_FOG_HORIZONTAL + dusk/dark palette (MorthasGrove darkest). Maps:
Ashenveil_Greyport, _Greyport_Outpost, _Greyport_Outpost_Interior, _AshFields1,
_DeadCity_Exterior, _DeadCity_Ruins1, _DeadCity_Ruins2, _TheMemorial, _MorthasGrove. Register
gMapGroup_Ashenveil WITH the first real map (deferred by systems-engineer). 3 wild tables
(AshFields1 / DeadCity_Exterior / MorthasGrove pre-encounter — ghost mons per brief). Phantom
Lantern obstacle tiles + environmental bg_events (shoe/handle/sign/footprints) in AshFields1,
grave markers in TheMemorial, sea-chart stone in MorthasGrove. Swap the Ashenveil boat stub
(SailToAshenveil) to a real warp once Greyport's arrival tile is verified walkable. The 5
Ashenveil MAPSECs are already in region_map_sections.json + region_map_entries.h (position
them). NOTE (RESOLVED 2026-06-14): the map-header MAPSEC>=0x100 truncation warning is FIXED
(mapjson.cpp emits `.2byte` for the region section now). See "MAP-HEADER MAPSEC WIDTH FIX".

**Prior pointer (Primalis maps):** PRIMALIS MAPS DONE (orchestrator inline, 2026-06-14, gmake
exit 0, EWRAM 86.46% / IWRAM 86.63% / ROM 79.94%). All 13 Primalis maps built, registered
(gMapGroup_Primalis + 13 .include lines + 7 layouts + 4 wild tables + heal location + boat-menu
entry), compiling cleanly. Full detail + script-writer handoff: "✅ Completed — Primalis MAPS"
section. Built inline by the orchestrator after two map-builder agent sessions dropped early.
**NEXT: pelagios-script-writer for Primalis scripts** — all 13 scripts.inc are STUBS (every
EventScript label "@ TODO (script-writer)") EXCEPT the VerdantLanding Tennyson (real boat wiring).
Implement per PRIMALIS_BRIEF.md: arrival cutscene (FLAG_PRIMALIS_ARRIVED + progress 1); the Lens
inn meeting (+ Dex-alive variant); Fern/Scale/Thorn/Mako gym leaders (talk-initiated, IDs 929-932,
narrative-only badges); Mako trust mechanic + first meeting (post-Gym2) + oral-history scene
(post-Gym4, two facing elders in ElderHall_Interior); ITEM_BEAST_WHISTLE handoff (post-Gym4) +
ITEM_PRIMALIS_TOKEN; the TWO Beast-Whistle gate field effects (PrimalisJungleRoute1/2_EventScript_
BeastGate — Route1 optional nook, Route2 north-exit Heartwood gate; both compare VAR_PRIMALIS_
PROGRESS); AncientRuinsCamp altar + cipher 8 (FLAG_CIPHER_8_FOUND/FLAG_PRIMALIS_CIPHER_FOUND);
Heartwood seal reinforcement + Zoan recovery -> FLAG_PRIMALIS_RESOLVED; all NPC/trainer dialogue.
VAR_PRIMALIS_PROGRESS: 0 not arrived,1 arrived,2 Gym1,3 Gym2+Mako,4 Gym3,5 Gym4+oral history,
6 seal reinforced,7 resolved. ⚠️ Only 4 Zoan trainer IDs (925-928) exist but 5 Zoan sight-trainer
objects were placed — map the 5th to an existing ID or make it a non-trainer NPC. Use setspeaker
nameboxes (add FERN/SCALE/THORN/MAKO + Zoan role names; the LENS already exists from Thalvern).
NOTE: Primalis is the 8th island and the LAST of the Galleon set before Ashenveil; the 3-island
Ashenveil gate (Schism+Thalvern+Gildhaven) is already wired on those three islands.

(Prior pointer) PRIMALIS CONSTANTS DONE (systems-engineer, 2026-06-14). See
"✅ Primalis Isle — CONSTANTS" above. Allocated: 13 flags BLOCK 4 0x282-0x28E (reused
FLAG_PRIMALIS_RESOLVED 0x4B2 + cipher 8 FLAG_CIPHER_8_FOUND 0x4C4); items 897 ITEM_PRIMALIS_
TOKEN + 898 ITEM_SEAL_SHARD_PRIMALIS (ITEM_BEAST_WHISTLE 879 PRE-EXISTED, reused); trainers
925-932 (4 Zoan guardians 925-928 + 4 leaders Fern 929 / Scale 930 / Thorn 931 / Mako 932),
TRAINERS_COUNT_EMERALD now 933. ZERO new vars (VAR_PRIMALIS_PROGRESS 0x4106 pre-existed;
brief confirms no extras; spares 0x410D/E/F untouched). Narrative-only badges. NO hidden items.

✅ MAPSEC u8 CEILING — RESOLVED 2026-06-14 (was the blocker flagged with Primalis constants).
mapsec_u8_t/metloc_u8_t widened u8 -> u16; stored metLocation is a 9-bit bitfield (ceiling 508,
253 free); sentinels at 0x1FD/E/F; Kanto kept. Full detail: "MAPSEC CEILING … RESOLVED" + the
SAVE-COMPATIBILITY BREAK #2 section. Primalis (and all later islands) can add MAPSECs normally.

NEXT (map-builder): build the 13 Primalis maps per PRIMALIS_BRIEF.md (gMapGroup_Primalis
registered WITH the first map; jungle tileset Fortree-or-closest; Beast-Whistle obstacle tiles
on Route1/Route2; 4 wild tables; VerdantLanding Inn heal location; swap the Galleon boat-menu
stub to a real SailToPrimalis warp). **MAPSECs: now UNBLOCKED — add the 7 Primalis MAPSECs to
region_map_sections.json (regenerate region_map_entries.h) like any earlier island.**
Trainer/flag/item constants are all in place and compiling. For Ashenveil/later constants:
BLOCK 4 free 0x28F-0x2BB
(45 flags), next item ID 899, trainer slots 91 free, var spares 0x410D/0x410E/0x410F (3) only.
Cipher 9 is the next (last) cipher (Haven=1 .. Gildhaven=7, Primalis=8).

Other open work: the 8-island in-emulator playtest backlog (Haven/Ironhold/Sirocco/
Emberveil/Schism/Thalvern/Gildhaven, all untested in mGBA). Gildhaven playtest route to
verify: harbor arrival + Cass glimpse (once-only, removeobject, no respawn), noble-quarter
Cass "Leave. Please." (once-only), Glint->BlackMarket Dagan appears (ON_TRANSITION) +
relationship bumps, Shade gate + corruption variants kicking in at progress 3, Lace gym +
manor key/access + passage, Exchange guards (4 sight trainers) + Covenant map reveal, Serel
gym + the Serel/Lace silent-look choreography (object pathing on the Lilycove-Museum-2F
layout — watch the side-door walk), SealChamber discovery + Ralts cutscene, direct
apparatus reinforcement (no siphon step), cipher 7, Sollis confession, and the Ashenveil
gate text (resolve Gildhaven last to see the "opens" branch).
PRIOR pointer (Gildhaven constants / Thalvern / Schism / maps / tilesets) below.

**Prior pointer (Schism):** SCHISM ISLE COMPLETE (constants + maps + scripts, 2026-06-13,
gmake exit 0). All 21 Schism scripts.inc fully written - zero TODO placeholders. See
"✅ Completed — Schism SCRIPTS" + "Schism flow state machine" (order-independent dual-seal
resolution; ceasefire meeting + failure/Drenn-death branches; cipher 5; stubbed Ashenveil
3-island gate). Schism in-emulator playtest still pending: dual-port arrival, Scar gates,
ScarRuins vision + cipher 5, ceasefire meeting AND failure branches, both SealChambers in
EITHER order, all 4 gyms, the boat menu sail loop.
PRIOR pointer (Schism maps / constants / tilesets) below.

**Prior pointer (tilesets):** CUSTOM PELAGIOS TILESETS DONE (2026-06-12, gmake exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.52%, +~53 KB). Six secondary tilesets created from
the converted Essentials art and registered in graphics.h / metatiles.h / headers.h /
include/tilesets.h: gTileset_PelagiosCoastal (413 tiles), _Desert (336), _Volcanic (294),
_Ice (255), _Poison (197), _Underwater (229). All flat-fill metatiles, terrain palette in
secondary slot 6, compile/link/Porymap-loadable, NOT yet used by any map. Generator:
tools/pelagios/build_tilesets.py (regenerate-only; clobbers Porymap edits). Full format +
extension guide: "Custom Pelagios Tilesets" section above Reference Maps. NEXT for
map-builder: when building island maps, pair these as the SECONDARY tileset (e.g. Sirocco
maps -> gTileset_PelagiosDesert, Emberveil -> _Volcanic, Schism ice/poison -> _Ice/_Poison,
Thalvern -> _Underwater, Haven coastal -> _Coastal). Hand-tune richer metatiles in Porymap
once a tileset is paired with real maps. ISLAND WORK pointer below still stands.

**Prior pointer (4-island verify):** 4-ISLAND COMPILE-VERIFICATION PASS DONE (2026-06-12,
gmake exit 0, EWRAM 86.45% / IWRAM 86.63% / ROM 79.36%). All four built islands
(Haven, Ironhold, Sirocco, Emberveil) + the shared boat menu now COMPILE-VERIFIED.
Forced rebuild of event_scripts.s + heal_location.c surfaced ZERO warnings (only the
benign "LOAD segment with RWX permissions" linker note, normal for GBA). Static
analysis found NO build-breaking errors and NO logic bugs to fix — in particular the
critical Galleon mirror check is ALREADY correctly implemented on BOTH sides
(Sirocco SealChamber line 115 goto_if_set FLAG_EMBERVEIL_RESOLVED; Emberveil
SealChamber line 170 goto_if_set FLAG_SIROCCO_RESOLVED -> both -> VAR_BOAT_TIER 3),
so whichever island resolves SECOND unlocks the Galleon. See "4-Island Compile
Verification (2026-06-12)" under Known Issues for the full pass/fail checklist.
Emberveil was CLAUDE.md-flagged as possibly incomplete (parallel session) — verified
FULLY scripted (15 maps registered, zero TODO/FIXME, Path A/B mutually exclusive,
all cipher/resolution/lava-boots wiring correct).
ONLY remaining work is the same in-emulator playtest backlog (all 4 islands + boat
menu untested in mGBA) plus the next island.
NEXT ISLAND: Schism Isle constants (story block 2 free: 0x49D-0x4A6, 10 flags;
trainer slots 900-1023 free; next item 888) — then replace the Schism stub case in
pelagios_boat.inc with a real sail handler. **BEFORE Schism: systems engineer must
decide badge-flag expansion (see "BADGE FLAGS — engine cap reached" below).**

---

**Prior pointer (Sirocco):** Sirocco SCRIPTS DONE (2026-06-12, gmake exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.34%). All 18 Sirocco scripts.inc fully written -
zero TODO placeholders remain. See "Completed - Sirocco SCRIPTS" + "Sirocco flow state
machine" above (progress var 0-7, Miria progress guard, narrative-only Mire Badge,
cipher 3 at the Interior2 mural, Dagan escape + VAR_DAGAN_RELATIONSHIP 0-2, conditional
Galleon check). REMAINING SIROCCO WORK: in-emulator playtest (arrival cutscene, gym
chain, north-gate bounce/open, Dex branches incl. journal recognition, mural vision +
cipher, Dagan escape BOTH response branches, SealChamber discovery + resolution, Galleon
text branch, post-resolution NPC variants, Tennyson sail-backs). CRITICAL HANDOFF to
whoever finishes Emberveil scripts: Emberveil_SealChamber is still a stub and its
resolution MUST mirror the Galleon check (goto_if_set FLAG_SIROCCO_RESOLVED ->
VAR_BOAT_TIER = 3) or the Galleon can never unlock when Emberveil is resolved second.
Also note: engine badge flags are exhausted (Miria took FLAG_BADGE08_GET, the last one);
Emberveil's four badges must be narrative-only like Dagan's Mire Badge. I also fixed a
dangling Emberveil label (EmberveilVolcanoSummit_Text_PathBDecline) that was breaking
the whole tree's link - see Known Issues.

---

**Prior pointer (Emberveil maps):** Emberveil MAPS DONE (2026-06-12, gmake exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.19%). All 15 maps built/registered via
tools/pelagios/build_emberveil.py + build_emberveil_mapjson.py +
build_emberveil_scripts_stubs.py (gMapGroup_Emberveil registered, 9 new General/Lavaridge
outdoor layouts, 6 reused vanilla interiors, 7 MAPSECs, heal location, 5 wild tables,
all connections reciprocal + every event tile validated walkable/in-bounds/no-overlap).
Full details + script-writer handoff notes in "Completed - Emberveil MAPS" above.
HANDOFF TO pelagios-script-writer: all 15 scripts.inc are STUBS (every EventScript label
tagged "@ TODO (script-writer): implement ..."); replace with real content per
EMBERVEIL_BRIEF.md (arrival cutscene + volcano pan, Cinder/Slag/Vex/Solace gym leaders
[talk-initiated, IDs 896-899], Lava Boots handoff + the 3 VAR_EMBERVEIL_PROGRESS lava/caldera
gates, Path-A vs Path-B Solace scenes via the 3 CalderaRuins panel signs + step counter,
SealChamber resolution + cipher 4 + conditional Galleon check vs FLAG_SIROCCO_RESOLVED, all
NPC/trainer dialogue). Use setspeaker nameboxes per the Dialogue Style Guidelines. The stub
generator won't clobber a scripts.inc once it has a .string line. NOTE: I also fixed a
pre-existing Sirocco link blocker (MUS_ROUTE111 -> MUS_ROUTE110 in 6 Sirocco map.json +
their generator) so the tree links; flag to the Sirocco owner.

---

**Prior pointer (Ironhold):** Ironhold SCRIPTS DONE (2026-06-11, gmake exit 0,
EWRAM 86.45% / IWRAM 86.63% / ROM 79.09%). All 17 scripts.inc fully written - zero
TODO placeholders remain. See "Completed - Ironhold SCRIPTS" + "Ironhold flow state
machine" above for the full implementation map (progress var 0-7, gym gating, Grapple
Hook rubble, resistance hideout, Sever cameo + key scene, SealChamber resolution,
cipher-2 decision, Brigantine unlock, 5 new trainers 868-872).
NEXT: in-emulator playtest of BOTH islands in mGBA. Ironhold route to verify:
dock arrival cutscene + Navigator's Log check (try with/without log via debug),
Petra -> HQ guard vanishes -> hideout ON_FRAME scene + Document Fragment, Forge
gating + Grapple Hook handoff, OuterDistrict rubble bounce (before) / open (after),
mountain route trainers, both elite guards -> Sever gate cameo appears/leaves,
Rook -> Sever gating + key scene (test BOTH choice branches; check
VAR_SEVER_RELATIONSHIP), SealChamber barrier bounce (progress 3/4), discovery
cutscene, apparatus resolution (flags/boat tier/journal/PokeNav), post-resolution
NPC variants (Petra/Forge/Elder/citizen/fisherman/traveler), Tennyson sail-back
warp to HavenIsle_Harbor (13,11) - VERIFY that tile is walkable, and Sever absent
from Interior2 after resolution. Haven Isle playtest checklist still pending too
(see Compilation section). After playtest: start Sirocco Isle / Emberveil.

---

## Known Issues

### Ironhold "no buildings" visual diagnosis + dressing pass (2026-06-14, gmake exit 0)
SYMPTOM (mGBA): Ironhold outdoor maps showed only flat ground (no buildings); gate
guard sprite looked wrong; scripts fired fine.

DEFINITIVE ROOT CAUSE = **hypothesis (a): UNDRESSED LAYOUTS, not a render/tileset bug.**
Decoded every Ironhold outdoor map.bin (metatile_id = word & 0x3FF): the layouts only
ever contained ground (0x001), wall (0x0C6), tall-grass (0x00D), water (0x170/0x189) and
sign (0x003) IDs — **ALL < 512 (i.e. all from the PRIMARY gTileset_General); the paired
SECONDARY gTileset_Slateport was essentially unused, and NO building metatiles were ever
placed.** build_ironhold.py only stamps ground/wall/water/sign/grass (S_GROUND/S_WALL/
S_SIGN/S_WATER/S_TALL palette — no building stamps). So "flat ground" was the
expected-but-undressed state. NO out-of-range IDs anywhere (rules out hypothesis b);
tileset pairing is correct (rules out c). A tileset swap would NOT have added buildings.

FIXES APPLIED (generators + regenerate; never hand-edited the outputs):
1. GUARD SPRITE (real bug — wrong placeholder): dock guard was OBJ_EVENT_GFX_MAN_3 (a
   civilian). Changed to **OBJ_EVENT_GFX_BLACK_BELT** (uniformed/martial, fits the
   Steel/Fighting occupied-island aesthetic) in build_ironhold_mapjson.py. Also swapped
   the generic Covenant rank-and-file guards/patrols from OBJ_EVENT_GFX_HIKER ->
   BLACK_BELT: DockGuard, SoldierPatrolA/B, HQGuard, SoldierVenn, Patrol2, Patrol3,
   EliteGuard2. Named officer placeholders (Forge, Crag, EliteSorn/Vael, ScoutHale,
   StationedA/B interiors) left as-is (deliberate character placeholders, not the
   "wrong guard" symptom). All gfx constants verified present in event_objects.h.
2. MINIMAL SAFE DRESSING PASS (build_ironhold.py): added BLD_A (5x6, has a door) and
   BLD_B (7x5, decorative facade) building stamps copied VERBATIM as full metatile words
   (id|collision|elevation) out of LAYOUT_SLATEPORT_CITY — so every tile is guaranteed
   valid for the General/Slateport pair AND internally tile-consistent (no risk of
   creating the hypothesis-b out-of-range bug). Buildings stamped so each BLD_A DOOR tile
   lands exactly on an existing warp coord and rises NORTH of the warp (door enterable
   from the south): GatemarkPort inn @ warp(4,8); IronholdCity @ warps (6,11),(20,11),
   (20,17) + a door-frame patch at (23,11) + 2 decorative BLD_B halls; a decorative BLD_B
   in OuterDistrict. IronholdCity sign tiles moved to plaza ground LEFT of each door (old
   sign tiles sat directly above the warp = now inside the building facade): (6,10)->
   (3,11), (20,10)->(17,11), (20,16)->(17,17) — updated in BOTH the layout S_SIGN tiles
   AND the mapjson sign() bg events so they stay in sync. VALIDATED post-regen: zero
   out-of-range IDs (max 0x395 = 512+405, within Slateport's 406 metatiles), all 4 city +
   1 port warp doors collision=0 AND the approach tile south of each door is walkable ->
   every building is enterable. This is a MINIMAL improvement (a few recognizable
   buildings around warps), NOT a full town pass — the maps still read as sparse. A richer
   Porymap/generator dressing pass (more buildings, paths, cliffs, door anims) remains
   DEFERRED map-builder content work.

CROSS-ISLAND SPOT-CHECK (same metatile-ID decode): Sirocco (sec=gTileset_Mauville),
Schism (sec=gTileset_PelagiosIce/Poison + Cave), Emberveil (sec=gTileset_Lavaridge) —
**ALL have ZERO out-of-range metatile IDs (no render bug anywhere), and ALL share the
same (a) undressed-layout state** (each outdoor layout uses only 0-2 secondary IDs;
mostly ground/wall/water). So the "flat ground" content gap is REGION-WIDE by generator
design. Only Ironhold got the dressing pass this session; Sirocco/Schism/Emberveil
dressing is the same deferred map-builder task. (NOTE: Schism layouts now exist in
data/layouts even though the island list still labels Schism "Not Started".)

UNRELATED PRE-EXISTING BLOCKER FIXED (was breaking the WHOLE build, not Ironhold): an
in-flight uncommitted session left src/data/heal_locations.json TRUNCATED mid-token at
HEAL_LOCATION_PRIMALIS_VERDANT_LANDING ("MAP_PRIM<EOF>), causing JSONPROC_ERROR. The
Primalis_VerdantLanding maps/scripts/innkeeper already exist and a script
(Primalis_VerdantLanding_Inn_OnTransition) setrespawns that heal location, so I RESTORED
a complete valid entry (map MAP_PRIMALIS_VERDANT_LANDING, respawn _INN, npc
LOCALID_PRIMALIS_VERDANT_INNKEEPER) rather than deleting it — preserving the in-flight
Schism/Thalvern/Gildhaven/Primalis heal-location additions. Flag to whoever owns that
work: confirm the Primalis heal coords (placeholder x4,y8) and finish those islands.

IN-EMULATOR VERIFICATION THE USER MUST DO (I CANNOT run mGBA — fixes proven only by
metatile-ID-range analysis + gfx-constant existence):
- Ironhold GatemarkPort/OuterDistrict/IronholdCity now show building structures (roofs/
  walls) instead of bare ground; the dock guard is a black-belt sprite, not a civilian.
- Walk INTO each city building door (warps (6,11),(20,11),(20,17)) and the port inn door
  (4,8): you should enter the interior, not bump a wall. Confirm doors reachable from S.
- Read the relocated city signs (now LEFT of each door at (3,11),(17,11),(17,17)).
- Confirm Covenant patrols/guards across Ironhold render as black-belts.
RUNTIME BUDGET after this pass: EWRAM 86.46% / IWRAM 86.63% (unchanged — map/tileset data
is ROM) / ROM 80.07% (up from 79.52%; includes the building metatiles + the now-buildable
in-flight Schism/Gildhaven/Primalis heal content).

### BOOT FREEZE (black screen + player only) — ROOT CAUSE + FIX (2026-06-14)
SYMPTOM: fresh ROM, after the naming/intro, the game froze on a BLACK SCREEN with only
the player sprite visible — the map tilemap/tileset never loaded. Player suspected the
`.2byte` MAPSEC map-header fix.

ROOT CAUSE — **map-header 4-byte MISALIGNMENT introduced by the (correct) `.2byte` fix,
not the `.2byte` itself.** The `.2byte` change is structurally right and PROVEN correct:
asm byte-offset table == `struct MapHeader` offsets field-for-field (offsetof on a
GBA-faithful 4-byte-pointer model):
| field | struct | asm | field | struct | asm |
|---|---|---|---|---|---|
| mapLayout | 0x00 | 0x00 | cave(requires_flash) | 0x16 | 0x16 |
| events | 0x04 | 0x04 | weather | 0x17 | 0x17 |
| mapScripts | 0x08 | 0x08 | mapType | 0x18 | 0x18 |
| connections | 0x0C | 0x0C | floorNumber | 0x19 | 0x19 |
| music | 0x10 | 0x10 | filler_19 | 0x1A | 0x1A |
| mapLayoutId | 0x12 | 0x12 | flags bitfield | 0x1B | 0x1B |
| regionMapSectionId(.2byte) | 0x14 | 0x14 | battleType | 0x1C | 0x1C |
sizeof(MapHeader)=0x20; map_header_flags macro emits exactly 1 byte (0x1B). All correct.

The bug was SECONDARY: headers are emitted back-to-back in headers.inc with **NO `.align`
and NO trailing pad** (mapjson.cpp emitted only the label + fields). Pre-`.2byte` each
header body was **0x1C bytes = 28 = a multiple of 4**, so consecutive headers stayed
4-byte aligned by luck. The `.2byte` widening made each body **0x1D bytes (29, ODD)**, so
every header after the first drifted to a non-4-aligned address (e.g. SlateportCity was at
0x...11d, MauvilleCity 0x...13a, the SPAWN map HavenIsle_PlayerHouse_2F at 0x...cb3 — odd).
C derefs `mapHeader->mapLayout` (pointer at 0x00) with a 32-bit LDR; on ARM7TDMI an
**unaligned LDR rotates the word** (no fault) → garbage layout pointer → tileset/tilemap
never loads → black screen, player still drawn. (This is purely an alignment-of-the-label
problem; the field LAYOUT inside the header was always correct.)

FIX (tools/mapjson/mapjson.cpp, ROM-only, no save break): emit `\t.align 2\n` (2^2 =
4-byte align in GAS/ARM) immediately BEFORE each `<MapName>:` header label, so every header
starts 4-byte aligned regardless of the odd 0x1D body size. Did NOT revert `.2byte` / the
u16 mapsec field (that would re-break MAPSEC>=256). Regenerated ALL headers from clean:
`rm -f tools/mapjson/mapjson` + `find data/maps -name header.inc -delete` +
`find build -name header.inc -delete` + `gmake`. VERIFIED in the linked ELF: every map
header now 4-byte aligned (558 checked, 0 misaligned; e.g. PetalburgCity 0x...100,
Slateport 0x...120, Mauville 0x...140, HavenIsle_PlayerHouse_2F 0x...2e0 — all end 0/4/8/c,
0x20 apart). Build exit 0, ZERO truncation/narrowing warnings (only benign RWX LOAD note).
Linker: EWRAM 86.46% / IWRAM 86.63% / ROM 80.07%.

RULED OUT (all PASS, not the cause): spawn warp (new_game.c WarpToTruck →
MAP_HAVEN_ISLE_PLAYER_HOUSE_2F 4,3); spawn layout (reuses valid vanilla
LAYOUT_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F w/ real tilesets); spawn ON_FRAME script
(MomCalls: lockall/msgbox/releaseall — no hang); EventScript_ResetAllMapFlags (setflag
only). The freeze was entirely the header misalignment.

IN-EMULATOR VERIFICATION STILL REQUIRED (could not run mGBA — static fix only): boot a
FRESH ROM → name screen → confirm it loads into the Haven Isle bedroom with **tiles AND
player visible** (not a black screen), the mom-calls msgbox fires, and you can walk. Then
spot-check Primalis/Ashenveil maps still show correct weather + map-name popup (the
original `.2byte` realignment).

RULE: any future raw-asm MapHeader emitter MUST (a) emit `.2byte` for region_map_section
AND (b) emit `.align 2` before the header label. The 0x1D odd body size makes per-header
4-byte alignment mandatory; do NOT rely on the body size being a multiple of 4.

### Ashenveil completion marker: VISITED, not RESOLVED (note for Convergence)
Ashenveil has NO `FLAG_ASHENVEIL_RESOLVED` flag (it does not exist — the Dead Island has
no seal-reinforce "resolution" like the other islands). Its slot in the per-island
RESOLVED block (0x4B0-0x4B4) is intentionally named **`FLAG_ASHENVEIL_VISITED` (0x4B3)**,
which the Ashenveil scripts set as the completion marker (alongside `FLAG_TRUE_ENDING_UNLOCKED`
and the `FLAG_DORNE_CHOICE_*` flags). **Convergence (and any final-island / true-ending logic)
must check `FLAG_ASHENVEIL_VISITED`, NOT `FLAG_ASHENVEIL_RESOLVED`** — the latter is undefined
and would fail to assemble. (The other islands keep `FLAG_<ISLAND>_RESOLVED`: Thalvern 0x4B0,
Gildhaven 0x4B1, Primalis 0x4B2, Aetheron 0x4B4.)

### MAPSEC CEILING (u8 limit) — RESOLVED 2026-06-14 (Option A, widen; Kanto kept)
**FIXED.** The RegionMapSecId enum hit its u8 ceiling at Gildhaven (MAPSEC_NONE = 255, the
max a `mapsec_u8_t` held), which blocked Primalis and all later islands from adding region-map
sections. Cleared by widening + a metLocation repack (full save-break writeup: "SAVE-COMPATIBILITY
BREAK #2 — MAPSEC widening" in Constant Space Layout above).

What was done:
- `mapsec_u8_t`/`metloc_u8_t` (include/gametypes.h) widened **u8 -> u16** — handler vars/params
  hold ids past 255 now. This also fixed the old src/landmark.c terminator overflow (its
  `mapsec_u8_t mapSection` is u16 now, so `{MAPSEC_NONE,…}` no longer truncates).
- The **stored** metLocation became a **9-bit bitfield** in `struct PokemonSubstruct3`
  (include/pokemon.h), reclaiming the former `unused_0B:1`. The substruct stays EXACTLY 12 bytes,
  so **BoxPokemon did NOT grow** — required, because a wider BoxPokemon overflows the PC box
  storage save sector (the `PokemonStorageFreeSpace` static-assert in src/save.c — this is what
  ruled out a full-u16 stored field; the original "just widen the substruct to u16" plan would
  not link).
- METLOC_* sentinels moved to **0x1FD/0x1FE/0x1FF** (top of the 9-bit range) so real MAPSEC ids
  never collide with them (src/data/region_map/region_map_sections.constants.json.txt — the .h
  regenerates from it).
- **New ceiling = 508 real MAPSECs** (sentinels 509-511). MAPSEC_NONE is currently 255, so
  **253 MAPSEC slots are free** — ample for Primalis (7) + Ashenveil/Aetheron/Convergence.

Why **Kanto was NOT removed** (the old Option B): the merged FRLG region is fully compiled and
LIVE — ~400 FRLG `*_Frlg/map.json` set Kanto MAPSECs as their region_map_section, and
src/region_map.c + src/regions.c hardcode the Kanto/Sevii region-map layouts. Deleting the 105
Kanto MAPSEC enum entries would break all of that. The brief itself said to leave any Kanto
MAPSEC that retained FRLG data references — they ALL are referenced — so removal was rejected as
unnecessary (the u16 widening alone clears the ceiling) and far too risky.

Primalis MAPSECs can now be added normally — by the map-builder with the first Primalis map, or a
quick constants follow-up. All OTHER Primalis constants (flags/items/trainers) were already
allocated and compiling.

### 4-Island Compile Verification (2026-06-12, gmake exit 0)
Full static + compile verification pass over Haven, Ironhold, Sirocco, Emberveil +
the shared boat menu. Build clean; forced rebuild of event_scripts.s + heal_location.c
showed ZERO warnings (only the benign RWX LOAD-segment linker note). NO fixes were
needed — every item below PASSED as found.

BOAT SYSTEM (data/scripts/pelagios_boat.inc):
- [PASS] Compiles, all labels resolve (build links; no dangling gotos/text refs).
- [PASS] VAR_BOAT_TIER gating: switch routes tier 3->Galleon, 2->Brigantine,
  0&1->Sloop (tier-0 defensively shows Sloop list; Haven's pre-Sloop boarding
  handles the true tier-0 case before this menu is ever reached).
- [PASS] All 4 harbors set VAR_TEMP_1 to the correct island id and reach
  Pelagios_EventScript_BoardTennyson; boarding bg_events (3 at Haven, 5 each at
  Ironhold/Sirocco/Emberveil) all point at the harbor's *_EventScript_Tennyson.
- [PASS] Arrival tiles walkable (collision=0 verified against compiled map.bin):
  Haven (13,9) elev3, Ironhold/Sirocco/Emberveil (10,14) elev3.
- [PASS] "Already moored here" fires for the current island at each harbor
  (each SailTo handler checks VAR_TEMP_1 == its own island id).
- [PASS] Galleon stubs Schism/Thalvern/Gildhaven -> SailNoChart "charts don't
  show a path there yet".
- [PASS] Multichoice registration: enum MULTI_BOAT_SLOOP/BRIGANTINE/GALLEON order
  (include/constants/script_menu.h) matches table indices (src/data/script_menu.h)
  and list contents match the switch-case order exactly.

HAVEN: [PASS] Sollis resolution sets FLAG_HAVEN_ISLE_COMPLETE + FLAG_BOAT_SLOOP_UNLOCKED
  + VAR_BOAT_TIER 1. [PASS] 16/16 .include lines. [PASS] Heal locations TIDEMARK_VILLAGE
  + PLAYER_HOUSE_2F registered. [PASS] IsLastHealLocationPlayerHouse() includes
  HEAL_LOCATION_HAVEN_ISLE_PLAYER_HOUSE_2F.

IRONHOLD: [PASS] 17/17 maps registered + included. [PASS] SealChamber sets
  FLAG_IRONHOLD_RESOLVED + VAR_BOAT_TIER 2 + FLAG_CIPHER_2_FOUND + FLAG_IRONHOLD_CIPHER_FOUND.
  [PASS] VAR_SEVER_RELATIONSHIP=1 on YES, stays 0 on NO. [PASS] Grapple gating uses
  FLAG_GRAPPLE_HOOK_OBTAINED / VAR_IRONHOLD_PROGRESS<3. [PASS] HEAL_LOCATION_IRONHOLD_GATEMARK_PORT
  registered.

SIROCCO: [PASS] 18/18 maps registered, zero TODO. [PASS] Dagan escape sets
  FLAG_SIROCCO_DAGAN_ESCAPED and his RICH_BOY object's hide flag IS that flag.
  [PASS] Resolution sets FLAG_SIROCCO_RESOLVED + conditional Galleon (VAR_BOAT_TIER 3
  only if FLAG_EMBERVEIL_RESOLVED). [PASS] Mural (Interior2) sets FLAG_CIPHER_3_FOUND +
  FLAG_SIROCCO_CIPHER_FOUND. [PASS] Dex gated by FLAG_SIROCCO_GYM1_CLEAR, sets
  FLAG_SIROCCO_DEX_MET. [PASS] VAR_DAGAN_RELATIONSHIP increments via 2x addvar (0/1/2).
  [PASS] HEAL_LOCATION_SIROCCO_DUSTMOUTH_PORT registered.

EMBERVEIL (verified FULLY scripted — CLAUDE.md's "parallel session, may be stale"
  caveat is now cleared): [PASS] 15/15 maps registered, zero TODO/FIXME. [PASS] Path B
  gated on FLAG_WARDEN_NOTES_EMBERVEIL at VolcanoSummit; Path A is "PATH_B unset" so the
  two are mutually exclusive by construction (no FLAG_EMBERVEIL_PATH_A exists or is
  needed; Path A's sub-branches FLAG_SOLACE_TOLD_TRUTH vs FLAG_SOLACE_ALT_ENDING are
  likewise exclusive). [PASS] Resolution sets FLAG_EMBERVEIL_RESOLVED and the SYMMETRIC
  Galleon check (goto_if_set FLAG_SIROCCO_RESOLVED -> VAR_BOAT_TIER 3) — the critical
  handoff was ALREADY correctly implemented; no fix required. [PASS] Lava-boots gate on
  LavaRoute2 + CalderaApproach via checkitem ITEM_LAVA_BOOTS. [PASS] Cipher 4 sets
  FLAG_CIPHER_4_FOUND (0x4C0, exists) + FLAG_EMBERVEIL_CIPHER_FOUND. [PASS]
  HEAL_LOCATION_EMBERVEIL_ASHMOUTH_PORT registered.

SPEAKER NAMES: [PASS] All Pelagios_Speaker_* labels referenced across all 4 islands +
  boat menu are defined in pelagios_speaker_names.inc; no undefined refs, no orphan
  definitions (clean both directions). No double-attribution (setspeaker + inline NAME:
  prefix) found in the newest scripts (Sirocco/Emberveil).

### BADGE FLAGS — engine cap reached; DECIDED 2026-06-12 (narrative-only, region-wide)
Engine provides FLAG_BADGE01_GET..FLAG_BADGE08_GET (8 only, NUM_BADGES == 8), now FULLY
CONSUMED. **DECISION (user, 2026-06-12): adopt narrative-only badges region-wide** — see
"Game Systems > Badge System" below for the formal policy. Option (b) was chosen; the
badge-flag system is NOT expanded. Existing FLAG_BADGE*_GET usage up through 08 stays
as-is; Emberveil-and-later gyms set only FLAG_*_GYM*_CLEAR. No script changes were needed
(audit 2026-06-12 found zero policy violations: no gating checks a FLAG_BADGE*; all gating
uses GYM*_CLEAR / progress vars; Haven has no gyms). Policy comment block added to
include/constants/flags.h after NUM_BADGES.

Concrete badge-flag usage map (audited 2026-06-12):
- Haven: NO gyms (verified — zero badge/gym references). BADGE01 left reserved/unwired
  for a future island per IRONHOLD_BRIEF.md numbering.
- Ironhold: FLAG_BADGE02_GET (Petra, IronholdCity), 03 (Forge, Armory_Interior),
  04 (Rook, SummitFortress_Interior1), 05 (Sever, SummitFortress_Interior2).
- Sirocco: FLAG_BADGE06_GET (Silt, MiradenOasis), 07 (Crag, MiradenOasis),
  08 (Miria, BuriedCity_Interior1) — **08 is the LAST engine slot, consumed.**
  Dagan (DaganPalace_Interior2): NARRATIVE-ONLY (fanfare + text, no badge flag;
  FLAG_SIROCCO_GYM4_CLEAR is the real state).
- Emberveil: ALL FOUR (Cinder/Slag/Vex/Solace) NARRATIVE-ONLY — set only
  FLAG_EMBERVEIL_GYM1..4_CLEAR, no FLAG_BADGE*_GET. Fanfare + "received the X BADGE"
  text play for feedback.
- Schism and all later islands: narrative-only by this policy (GYM*_CLEAR only).
Consequence: vanilla Trainer Card badge counter tops out at 8 and won't reflect
Emberveil+ badges — the post-launch Island Journal UI (see Not Started) will replace it.

### Resolved (2026-06-12 — tree-wide link failure from in-flight Emberveil scripts)
- Baseline `gmake` before the Sirocco script session failed to link:
  `undefined reference to EmberveilVolcanoSummit_Text_PathBDecline` (a parallel
  Emberveil script session referenced the label but never defined the string).
  Fixed by adding the missing text in data/maps/Emberveil_VolcanoSummit/scripts.inc
  (player declines to show the Warden's notes mid-ritual; tone-matched to the
  surrounding Path B scene). Emberveil's owner should review the line and note that
  Emberveil_SealChamber scripts are still TODO stubs.
- RULE for parallel sessions: always run a baseline gmake BEFORE writing, so new
  errors are attributable; fix or minimally patch other-island link breakage first.

- 2026-06-11 end-of-session build verification: incremental gmake exit 0,
  pokeemerald.gba produced. Linker memory: EWRAM 86.43%, IWRAM 86.63%,
  ROM 78.91% (26,477,104 B / 32 MB). All JSON parses, all 16 HavenIsle
  scripts.inc included, ROUTE1/ROUTE2 encounters + both HAVEN trainers +
  new_game PLAYER_HOUSE_2F warp all present. No fixes needed.

### Resolved (2026-06-11 — Tennyson interaction, second pass: bg_event boarding)
- Even with the object on the shore-edge tile, interaction stayed unreliable.
  Decisive evidence: every vanilla SS_TIDAL object (Slateport/Lilycove Harbor)
  has script 0x0 - the engine NEVER makes the giant ship sprite itself
  interactable. The 96x40 sprite draws centered over ~6x2.5 tiles (hence
  "walking inside the sprite") while interaction targets only its 1-tile anchor.
- Fix (vanilla-consistent): SS_TIDAL object is now pure decoration (script 0x0,
  anchor (13,11), elevation 1, just offshore). Boarding runs through three
  sign-type bg_events on the shore-edge water tiles (12,10)-(14,10) pointing at
  HavenIsleHarbor_EventScript_Tennyson - stand on the sand row 9 anywhere along
  the hull, face down, press A. Changes in build_mapjson.py, regenerated.
- RULE for all future harbors: never put the boarding script on the SS_TIDAL
  object itself; use bg_events or a pier tile/NPC. Ironhold GatemarkPort had
  the same bug-in-waiting (boarding script on its SS_TIDAL at the pier end) -
  fixed the same way: ship moved to (12,15) as decoration (script 0x0), five
  boarding bg_events along the pier's east edge and south end
  ((11,13)-(11,15), (10,16), (9,16)); changes in build_ironhold_mapjson.py.

### Resolved (2026-06-11 — Tennyson not interactable / sailing to Ironhold)
- Root cause was physical, not flags: the SS_TIDAL object sat at (13,12),
  elevation 1 - two tiles into open water - while the player can only stand on
  the shore (elevation 3), so a face-interaction was impossible. Flags
  (FLAG_BOAT_SLOOP_UNLOCKED, FLAG_HAVEN_ISLE_COMPLETE) were being set correctly
  by SollisWardenTalk. Boat moved to (13,10), the shore-edge tile directly
  south of the sand at (13,9), with elevation 0 so interaction works across the
  land/water elevation split (changed in build_mapjson.py, regenerated).
- The "Set sail?" YES branch still pointed at the pre-Ironhold "to be
  continued" stub. Now warps to MAP_IRONHOLD_GATEMARK_PORT (10,14) - on the
  Gatemark pier beside the Tennyson, so walking north crosses the arrival
  triggers at (9-10,11) and the dock cutscene fires naturally.
- VAR_BOAT_TIER was never set to 1 by Haven's resolution (the var was added in
  the later capacity refactor). SollisWardenTalk now sets it; the sail script
  has a never-downgrade safety net (sets tier 1 only if still 0).
- Ironhold->Haven return warp landed the player on open water at (13,11);
  fixed to the sand at (13,9), in front of the berth.

### Resolved (2026-06-11 — Ancient Ruins visual/placement bugs)
- Ralts object sat on wall metatile 0x612 at (16,10), clipped into geometry AND
  off-screen from the entry tile when the ON_FRAME scene played. Moved to
  (16,19) - verified floor (0x3211) in the entrance chamber, on-screen from the
  player's entry at (18,22). Flee movement rewritten (2 west, 2 north, every
  step verified walkable). Coordinate lives in tools/pelagios/build_mapjson.py.
- "Sand tile at the interior entrance": the exterior mouth (0x0A7, the Rusturf
  cave-entrance metatile) and the interior exit (0x23C + 0x658-0x65A cave mouth)
  were already correct; the seam was Altering Cave's decorative dirt patch
  (0x3201) beside the entrance, inherited by our layout copy. build_layouts.py
  now flattens all 0x3201 to cave floor 0x3211 in Interior1; bins regenerated.
- Verification is scripted: tile assertions (Ralts tile, flee path, no 0x3201
  left, ladder/entrance intact) ran against the regenerated bin before compile.

### Resolved (2026-06-11 — Cass intercept scene rework)
- Reworked TidemarkVillage_EventScript_CassIntercept to the vanilla
  rival-intercept pattern: single coord trigger on the doormat tile (5,8) the
  door-exit step lands on; lockall; player force-walked 2 tiles south to (5,10);
  exclamation + Cass walks (8,10)->(6,10) and faces the player; player faces
  back; msgbox dialogue only after all movement completes (waitmovement 0 after
  every applymovement); then Cass runs east to the lab doormat and despawns;
  state advances to VAR_PELAGIOS_INTRO_STATE=3.
- Old woman NPC moved to (3,11) with up/down-only wander so she can never
  occupy (5,10) and desync the intercept blocking the player's forced walk.
- Trigger/object changes made in tools/pelagios/build_mapjson.py and map.json
  regenerated (never hand-edit those files).

### Resolved (2026-06-11 — Ancient Ruins playtest bugs)
- BUG 1+3 (shared root cause): the ruins disturbance coord triggers on
  AncientRuinsExterior fired at VAR_HAVEN_RUINS_STATE==0, but starter selection
  sets the var to 1 - so post-starter the disturbance could never fire (and the
  Ralts ON_FRAME, gated on var==2, never played); pre-starter/stale saves fired
  it on the row OUTSIDE the entrance. Fix: removed the exterior coord triggers
  entirely (generator updated + map.json regenerated). Both scenes now run as a
  chained ON_FRAME table on HavenIsle_AncientRuinsInterior1: var==1 -> camera
  shake disturbance (sets 2) -> var==2 -> Ralts flee scene + Sollis PokéNav call
  (sets 3 + FLAG_HAVEN_RUINS_VISITED). Guaranteed to fire inside the ruins,
  fires exactly once, and recovers from a save stuck at state 2.
- BUG 2 (not a bug): no wild encounters in the ruins is INTENTIONAL per the
  brief ("No wild Pokémon in ruins. Scripted Ralts appearance only"). There is
  deliberately no wild_encounters.json entry for either interior map; the
  AlteringCave-derived floor only spawns encounters when a table exists.
- RULE: straight double-quote `"` is NOT in charmap.txt - use curly “ ” in
  .string text (they map to B1/B2). Same family of issue as the em-dash rule.

### Resolved (2026-06-11 — whiteout freeze at Haven Isle player house)
- Losing a battle with respawn set to the player's house froze the game on the
  nurse's "We hope you excel!" line. Root cause was twofold:
  1. src/heal_location.c IsLastHealLocationPlayerHouse() only listed the
     Littleroot/Pallet houses, so Haven Isle whiteouts ran the NURSE script
     (EventScript_AfterWhiteOutHeal), whose
     `applymovement VAR_LAST_TALKED, Movement_PkmnCenterNurse_Bow` +
     `waitmovement 0` hangs in a map with no nurse. Fixed by adding
     HEAL_LOCATION_HAVEN_ISLE_PLAYER_HOUSE_2F to the check.
  2. EventScript_AfterWhiteOutMomHeal hardcoded LOCALID_PLAYERS_HOUSE_1F_MOM
     (Littleroot's mom), which would hang the same way on the Haven map. Fixed
     to `applymovement VAR_LAST_TALKED` - the respawn system
     (SetWhiteoutRespawnWarpAndHealerNPC) already stores the healer NPC there.
  RULE for future islands: any new respawn-to-house heal location must be added
  to IsLastHealLocationPlayerHouse(), and respawn scripts must address the
  healer via VAR_LAST_TALKED, never a hardcoded LOCALID.

### Resolved (2026-06-11 — Haven Isle first clean compile)
- Custom key-item icons referenced non-existent gfx symbols in src/data/items.h.
  Fixed to valid placeholder symbols: ITEM_BEAST_WHISTLE AquaScope→SilphScope,
  ITEM_PHANTOM_LANTERN FlashOrb→FlameOrb, ITEM_TENNYSON_KEY TmCase→TMCase
  (correct casing). These are placeholder icons only; swap to custom art later.
- Em dash (U+2014) is NOT in charmap.txt and broke the script preproc. Replaced
  with ASCII hyphen `-` across 6 HavenIsle scripts.inc files (Harbor, SollisLab,
  AncientRuinsExterior, AncientRuinsInterior1, ElderHouse, FishingDocks).
  RULE: never use em/en dashes in .string dialogue — use `-`. `é` and `…` are fine
  (they exist in charmap).

### Open (needs in-emulator verification, not compile-blocking)
- Heal location HEAL_LOCATION_HAVEN_ISLE_PLAYER_HOUSE_2F respawns to PlayerHouse_1F
  via LOCALID_HAVEN_MOM with explicit respawn_x/y 5,5 — verify those coords land on
  a valid walkable tile in-emulator (set to match the 1F mom NPC position).
- New-game intro still plays the vanilla Birch speech/gender select before warping
  to the Haven Isle bedroom — acceptable placeholder; re-skin to Sollis later.
- Exterior maps were composed programmatically (tools/pelagios/build_layouts.py)
  from vanilla tile stamps — review in Porymap for visual seams (knoll edges on
  AncientRuinsExterior, sand/grass transitions on Harbor/Route2/Docks).

---

## ROM Budget (audited 2026-06-11)

**The 32 MB limit is a GBA hardware cap and cannot be raised.** The cartridge
address window (0x08000000-0x09FFFFFF) is 32 MB; ld_script_modern.ld already
maps `ROM (rx): ORIGIN = 0x8000000, LENGTH = 32M`. There is no config to expand.

**Current usage: 26.4 MB / 32 MB (78.91%) — but ~78.9% was the BASELINE before
Pelagios.** Haven Isle (8 layouts, 16 maps, all scripts/dialogue/encounters/
trainers) added well under 0.1 MB. Maps and scripts are nearly free.

What actually consumes the ROM (from pokeemerald.map):
- data/sound_data.o ~9.9 MB (music/SFX, incl. merged FRLG soundtrack)
- src/pokemon.o ~5.8 MB (all-gen species data; sprites already smol-compressed)
- everything else is small (event_scripts 0.96 MB total for ALL vanilla+FRLG+Pelagios)

**Projection:** 9 remaining islands at even 3x Haven Isle's content ≈ ~2 MB
total vs ~6.7 MB free. Map/script content for the whole game fits comfortably.
The real budget risks are future CUSTOM assets (music tracks, large sprite sets).

**Space levers if headroom is ever needed (in order of payoff):**
1. Disable unused Pokémon generations in include/config/species_enabled.h
   (P_GEN_x_POKEMON) once the final Pelagios dex is locked — multi-MB savings.
   WARNING: changes saveblock dex flags → requires new save file.
2. Strip the merged FRLG content (maps/music/tilesets unused by Pelagios) —
   several MB incl. a chunk of sound_data, but invasive; only as last resort.
3. Pokémon sprites are ALREADY smol-compressed (INCGFX .smol) — no win left there.

Do NOT pre-emptively disable generations now; wild encounter tables and story
mons span many gens and the dex isn't final.

---

## Reference Maps
When creating new maps always reference these existing maps for format:
- `data/maps/PetalburgCity/map.json` — town format
- `data/maps/Route101/map.json` — route format
- `data/maps/BirchsLab/map.json` — lab interior format
- `data/maps/PlayerHouseF1/map.json` — house interior format

---

## Custom Pelagios Tilesets (created 2026-06-12, gmake exit 0)

Six custom SECONDARY tilesets assembled from the GBA-converted Essentials source
PNGs in `graphics/tilesets/pelagios_source/`. NOT yet used by any map — they exist,
compile, link, and are valid/loadable for future map work and Porymap. Generated by
`tools/pelagios/build_tilesets.py` (rerun to regenerate; never hand-edit the .bin/png).

| Tileset (`gTileset_*`) | Data dir (`data/tilesets/secondary/`) | Source theme | Tiles | Metatiles | Default behaviors |
|---|---|---|---|---|---|
| PelagiosCoastal   | pelagios_coastal   | haven_isle_coastal (HGSS Beach/Sea/Waves, Sand shore) | 413 | 413 | sand=NORMAL, sea=OCEAN_WATER, deep=DEEP_WATER |
| PelagiosDesert    | pelagios_desert    | sirocco_desert (Sand, Sandy Dirt, Gravel, Dirt)       | 336 | 336 | sand=DEEP_SAND, dirt/gravel=NORMAL, lt.grass=TALL_GRASS |
| PelagiosVolcanic  | pelagios_volcanic  | emberveil_volcanic (Magma Lava/Water, Red cave)       | 294 | 294 | lava/water=DEEP_WATER, cave floors=CAVE |
| PelagiosIce       | pelagios_ice       | schism_ice (Snow/White cave, ice border, path)        | 255 | 255 | cave/snow=CAVE, white path=NORMAL |
| PelagiosPoison    | pelagios_poison    | schism_poison (Blue/Green/Brown cave floor, mud)      | 197 | 197 | floors=CAVE, mud=DEEP_WATER |
| PelagiosUnderwater| pelagios_underwater| thalvern_underwater (Underwater dark, Seaweed, Sea deep)| 229 | 229 | all=DEEP_WATER |

### Format / pipeline (matches this repo's C-based tileset system)
- Each tileset dir holds: `tiles.png` (4bpp indexed, 16 tiles wide, <=15 colors +
  transparent index 0), `palettes/00.pal..15.pal` (JASC-PAL), `metatiles.bin`,
  `metatile_attributes.bin`.
- Registered in FOUR places (all in the Emerald `!IS_FRLG` section, before the FRLG
  block — DO NOT place new tileset entries inside the `#if IS_FRLG` / `#else` tail):
  1. `src/data/tilesets/graphics.h` — `gTilesetTiles_*` (INCGFX_U32 .4bpp.fastSmol
     with `-num_tiles N -Wnum_tiles`) + `gTilesetPalettes_*[][16]` (16 INCGFX_U16 .gbapal).
  2. `src/data/tilesets/metatiles.h` — `gMetatiles_*` + `gMetatileAttributes_*` (INCBIN_U16).
  3. `src/data/tilesets/headers.h` — `const struct Tileset gTileset_*` (isCompressed=TRUE,
     isSecondary=TRUE, callback=NULL).
  4. `include/tilesets.h` — `extern const struct Tileset gTileset_*;`
- No `.mk` / graphics_file_rules entry needed: the INCGFX preproc auto-builds PNG->4bpp
  on the fly. Porymap auto-discovers tilesets from these C files (no porymap json edit).

### GBA constraints honored (read before extending)
- Secondary tile budget = NUM_TILES_TOTAL(1024) - NUM_TILES_IN_PRIMARY(512) = **512 tiles**.
  All six are <=413 (generator caps at MAX_TILES=480). A secondary tileset's own tiles
  occupy local ids 0..N-1 (engine offsets them to 512+ at runtime); metatiles may also
  reference primary tiles 0-511, but these stand-alone tilesets only use their own.
- 16 colors/palette (15 + transparent index 0). The real terrain palette lives in
  **secondary palette slot 6** (NUM_PALS_IN_PRIMARY=6); slots 0-5 are overwritten by
  whatever PRIMARY tileset a map pairs this with, slots 7-12 are zero-filled/free.
  ALL metatiles reference palette slot 6.
- metatiles.bin = 8 u16/metatile (4 bottom-layer + 4 top-layer). u16 =
  tileid(bits0-9) | hflip(10) | vflip(11) | palnum(12-15). Metatiles here are flat
  2x2 fills: bottom = 4 copies of one tile @ pal 6, top = blank tile 0 (transparent),
  layer type = NORMAL. Metatile 0 is fully blank by convention.
- metatile_attributes.bin = 2 bytes/metatile (porymap metatile_attributes_size=2):
  behavior = bits0-7 (MB_* value), layer type = bits12-15.

### Extending in Porymap (future map work)
- These are usable as the SECONDARY tileset of any map (pair with a primary like
  gTileset_General). Pick the island-appropriate one when building Pelagios island maps.
- Each tile currently has its own flat metatile; in Porymap you can build richer
  metatiles (edges, cliffs, multi-tile structures, top-layer overlays) by combining the
  existing tiles, adjusting palette assignment (keep to slot 6 or add real palettes to
  slots 7-12), and tuning per-metatile behavior/collision/elevation. Re-running
  `build_tilesets.py` REGENERATES from scratch and will clobber Porymap edits — once a
  tileset is hand-tuned in Porymap, stop regenerating it (or re-curate the generator first).
- To add/curate tiles, edit the `TILESETS` dict in `tools/pelagios/build_tilesets.py`
  (per-source behavior + max-tiles-to-sample), rerun, and update `-num_tiles N` in
  graphics.h if the tile count changes.
- Source art is full HGSS/Essentials autotile blob sheets (thousands of redundant
  tiles, e.g. coastal sources total ~29k tiles). The generator extracts a curated,
  deduplicated, capped representative subset — it does NOT (and cannot) load whole sheets
  within the 512-tile budget. For specific tiles not captured, raise that source's cap
  in the dict or add the source PNG.