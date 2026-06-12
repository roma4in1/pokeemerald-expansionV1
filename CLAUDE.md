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
| EWRAM | **86.45%** (226,632 B / 256 KB) | hard 100% | 2026-06-12, 4-island compile-verify pass |
| IWRAM | **86.63%** (28,388 B / 32 KB) | hard 100% | 2026-06-12, 4-island compile-verify pass |
| ROM | **79.36%** (26,628,268 B / 32 MB) | hard 100% (GBA cap, see ROM Budget) | 2026-06-12, 4-island compile-verify pass |

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

### Trainer flags
- `MAX_TRAINERS_COUNT_EMERALD`: was 864, now **1024** (include/constants/opponents.h)
- Trainer defeat flags occupy **0x500 - 0x8FF** (was 0x500-0x85F)
- `TRAINERS_COUNT_EMERALD`: **900** (Emberveil added 888-899)
- **Free trainer headroom: 1024 - 900 = 124 slots** for future islands
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
- **Free in block 2: 0x49D-0x4A6 (10 flags)** for future islands. After block 2 fills,
  the next overflow must find another contiguous FLAG_UNUSED run (never collide with
  trainer flags 0x500-0x8FF or SYSTEM_FLAGS 0x900+).

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
  Next free hidden-item slot: +0x76 (0x26A, was FLAG_UNUSED_0x26A).

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
  - VAR_PELAGIOS_RESERVED_0x410A through 0x410F (6 reserved spare slots)
- Beyond 0x410F requires another VARS_END bump (saveblock impact) — STOP and flag.
- Emberveil added NO new vars: VAR_EMBERVEIL_PROGRESS 0x4102 pre-existed (states
  0=not arrived .. 7=resolved). The 6 reserved spares 0x410A-0x410F remain free.

### Items (Pelagios key items, include/constants/items.h + src/data/items.h)
- Vanilla items run to 873. Pelagios key items 874-885 (Haven/Ironhold/Sirocco batch).
- Emberveil: ITEM_LAVA_BOOTS 877 already existed (Haven key-item batch; data matches
  brief, icon gItemIcon_Bicycle placeholder — unchanged). NEW: ITEM_WARDEN_NOTES 886
  (icon Powder/EnergyPowder), ITEM_SEAL_SHARD_EMBERVEIL 887 (icon RedOrb, Infernape
  Mega stub). **Next free item ID: 888.** ITEM_SEAL_SHARD_IRONHOLD stays a #define
  alias to ITEM_SEAL_SHARD_1 (882). Adding items is cheap — no hard ceiling nearby.

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
- Tier 3 (Galleon) -> `MULTI_BOAT_GALLEON`: + Schism Isle, Thalvern, Gildhaven.
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
Schism / Thalvern / Gildhaven are SELECTABLE so the menu grows naturally, but
have no port yet. They route to `Pelagios_EventScript_SailNoChart` ->
"The charts don't show a path there yet."

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

## Game Systems

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

### ⏳ Not Started
- Sirocco Isle: COMPLETE (constants + maps + scripts, 2026-06-12) — only in-emulator
  playtest pending; see Completed — Sirocco SCRIPTS
- Emberveil: COMPLETE (constants + maps + scripts, 2026-06-12) — only in-emulator
  playtest pending; see Completed — Emberveil SCRIPTS (reachability caveat there)
- Schism Isle
- Thalvern
- Gildhaven
- Primalis
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

**Next session starts here:** 4-ISLAND COMPILE-VERIFICATION PASS DONE (2026-06-12,
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