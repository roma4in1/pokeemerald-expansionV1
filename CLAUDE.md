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
| EWRAM | **86.45%** (226,632 B / 256 KB) | hard 100% | 2026-06-11, after Ironhold scripts |
| IWRAM | **86.63%** (28,388 B / 32 KB) | hard 100% | 2026-06-11, after Ironhold scripts |
| ROM | **79.09%** (26,538,000 B / 32 MB) | hard 100% (GBA cap, see ROM Budget) | 2026-06-11, after Ironhold scripts |

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
- `TRAINERS_COUNT_EMERALD`: 868 (Ironhold gym leaders are 864-867)
- **Free trainer headroom: 1024 - 868 = 156 slots** for future islands
- Ironhold gym leaders: TRAINER_LEADER_IRONHOLD_PETRA 864, FORGE 865, ROOK 866,
  SEVER 867 (parties from IRONHOLD_BRIEF.md, in src/data/trainers.party)

### System flags
- `SYSTEM_FLAGS` base: was 0x860, now **0x900** (derived from TRAINER_FLAGS_END + 1,
  so it floats automatically with MAX_TRAINERS_COUNT — never hardcode it)
- All FLAG_SYS_*/badge/landmark/daily flags are SYSTEM_FLAGS-relative; they shifted
  up transparently. DAILY_FLAGS and FLAGS_COUNT grew by 0xA0 (160).
- `FLAGS_COUNT`: was ~0x960, now **~0xA00** (still far below SPECIAL_FLAGS_START 0x4000)

### Story flags (Pelagios)
- Pelagios story block: 0x4A7 onward. Ironhold flags end at 0x4E3
  (FLAG_HIDE_IRONHOLD_SEVER_GATE = 0x4E3, gate-Sever cameo hide flag).
- Free: 0x4E4-0x4EF (12 flags) before vanilla flags resume at 0x4F0.

### Hidden-item flags (Pelagios)
- Hidden-item flags MUST be in the hidden-items range (>= FLAG_HIDDEN_ITEMS_START,
  which is 0x1F4 for the Emerald/Hoenn map block). The bg_hidden_item_event macro
  (asm/macros/map.inc) hard-errors if flag < FLAG_HIDDEN_ITEMS_START — never put a
  hidden-item flag in the 0x4xx story block.
- Vanilla Emerald hidden items occupy FLAG_HIDDEN_ITEMS_START + 0x00..0x6F.
  Pelagios claims +0x71 onward (reusing the trailing FLAG_UNUSED_0x265+ slots):
  - FLAG_HIDDEN_ITEM_IRONHOLD_ANTIDOTE = +0x71 (0x265)
  - FLAG_HIDDEN_ITEM_IRONHOLD_IRON     = +0x72 (0x266)
  Next free hidden-item slot: +0x73 (0x267, was FLAG_UNUSED_0x267).

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

### Build cost of this refactor
EWRAM 86.43% -> 86.45% (+52 bytes), ROM 78.91% -> 79.00%. Compiles exit 0.

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
- Tennyson: sail back to MAP_HAVEN_ISLE_HARBOR (13,11) any time via YESNO warp;
  post-resolution shows the brigantine refit text. Haven->Ironhold travel still goes
  through Haven's "to be continued" Tennyson (boat transition system not built).
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

### ⏳ Not Started
- Sirocco Isle
- Emberveil
- Schism Isle
- Thalvern
- Gildhaven
- Primalis
- Ashenveil
- Aetheron
- Convergence
- Custom sprites (using placeholders throughout)
- Custom legendary Pokémon
- Boat transition system
- World map / nautical chart
- Key item field effects
- Mega Evolution system (Seal Shards)
- Z-Move distribution
- Difficulty modes
- Quest/mission tracker UI

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

**Next session starts here:** Ironhold SCRIPTS DONE (2026-06-11, gmake exit 0,
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