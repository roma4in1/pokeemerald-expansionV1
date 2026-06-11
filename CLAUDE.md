# Pokémon Pelagios — Claude Code Project Context

## Project Overview
GBA ROM hack built on **pokeemerald-expansion** (Emerald decomp base).
Full design specification: `PELAGIOS_BRIEF.md`
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

### 🔄 In Progress
- Haven Isle (current task)

### ⏳ Not Started
- Ironhold
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
- [ ] Add all flags to include/constants/flags.h
- [ ] Add all variables to include/constants/vars.h
- [ ] Add all key items to include/constants/items.h
- [ ] Add key item data to src/data/items.h
- [ ] Add island map group stubs to map_groups.json

### Maps
- [ ] HavenIsle_Harbor
- [ ] HavenIsle_TidemarkVillage
- [ ] HavenIsle_SollisLab (exterior connects to village)
- [ ] HavenIsle_SollisLab_Interior
- [ ] HavenIsle_PlayerHouse (exterior connects to village)
- [ ] HavenIsle_PlayerHouse_Interior
- [ ] HavenIsle_CassHouse (exterior connects to village)
- [ ] HavenIsle_CassHouse_Interior
- [ ] HavenIsle_Route1
- [ ] HavenIsle_AncientRuins_Exterior
- [ ] HavenIsle_AncientRuins_Interior1
- [ ] HavenIsle_AncientRuins_Interior2
- [ ] HavenIsle_Route2
- [ ] HavenIsle_FishingDocks

### Scripts
- [ ] Opening sequence (player wakes up, mom calls, goes to lab)
- [ ] Starter selection scene (three Pokémon loose in lab)
- [ ] Warden's Journal key item script
- [ ] Ancient Ruins disturbance trigger (screen shake + Ralts flee)
- [ ] Sollis explanation of Warden role
- [ ] Haven Isle complete / Sloop unlock
- [ ] All NPC dialogue (sailor, old woman, child, elder, fisherman)
- [ ] Trainer battles (Youngster Jay, Fisherman Old Bren)

### Wild Encounters
- [ ] Route 1 grass encounters
- [ ] Route 1 surf encounters
- [ ] Route 2 grass encounters
- [ ] Route 2 surf encounters
- [ ] Route 2 fishing encounters (Old Rod + Good Rod)

### Compilation
- [ ] Full compile passes with zero errors
- [ ] Game boots to opening sequence in mGBA
- [ ] Starter selection works correctly
- [ ] Can navigate all Haven Isle maps

---

## Session Handoff Protocol
At the end of each session, update this file:
1. Move completed items from the checklist to ✅ Completed
2. Note any known issues or compilation warnings
3. Write a one-line "Next session starts here:" note

**Next session starts here:** Build Haven Isle — start with flags.h,
vars.h, items.h constants, then map files, then scripts.

---

## Known Issues
None yet — project is at baseline.

---

## Reference Maps
When creating new maps always reference these existing maps for format:
- `data/maps/PetalburgCity/map.json` — town format
- `data/maps/Route101/map.json` — route format
- `data/maps/BirchsLab/map.json` — lab interior format
- `data/maps/PlayerHouseF1/map.json` — house interior format