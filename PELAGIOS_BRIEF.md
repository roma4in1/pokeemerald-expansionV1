# POKÉMON PELAGIOS — CLAUDE CODE DEVELOPMENT BRIEF

## PROJECT OVERVIEW

This is a Pokémon GBA ROM hack called **Pokémon Pelagios** built on top of
`pokeemerald-expansion` (forked from `rh-hideout/pokeemerald-expansion`).
The repo is already cloned, compiles successfully, and produces a working
`pokeemerald.gba`. Your job is to build the game's content on top of this
base.

**Engine:** pokeemerald-expansion (GBA, Emerald base)
**Build command:** `gmake -j$(sysctl -n hw.ncpu)`
**Map editor:** Porymap (installed, points to this repo)
**Scripting:** Poryscript preferred where available, fallback to raw inc scripts

---

## WORLD STRUCTURE

The region is an **archipelago** navigated by boat. Island progression is
semi-linear gated by boat upgrades.

```
Haven Isle (Home Island — fixed start)
    ↓ Sloop unlocked
Ironhold (Occupied Island)
    ↓ Brigantine unlocked
Sirocco Isle (Sand) + Emberveil (Volcanic)  ← any order
    ↓ Galleon unlocked
Schism Isle (Split Ice/Poison) + Thalvern (Sunken Kingdom) + Gildhaven (Merchant) ← any order
    ↓
Ashenveil (Dead Island) ← unlocked after above three
    ↓ Sea chart found here
Aetheron (Sky Island) ← accessed via Knock Up Stream scripted sequence
    ↓
Convergence (Final Island — fixed end)
```

---

## HAVEN ISLE — FULL SPECIFICATION

### Geography & Maps Needed

```
Haven Isle Harbor
    ↓
Tidemark Village
    ├── Professor Sollis's Lab (interior)
    ├── Player's House (interior)
    └── Cass's House (interior)
    ↓
Coastal Route 1 (north, leads to ruins)
    ↓
Ancient Ruins (small dungeon, 2 rooms)
    ↓ (dead end, returns to Route 1)

Coastal Route 2 (south of village, fishing docks)
    ↓
Fishing Docks (small area, one NPC)
```

### Haven Isle Harbor
- Entry point from sea (late game return visits)
- The Tennyson (player's boat) is docked here — overworld boat sprite
- One sailor NPC who gives sailing tips
- Sign: "Tidemark Village — 1 ahead"
- Connects north to Tidemark Village
- Tileset: nautical/port, water to the south

### Tidemark Village
- Small fishing village, warm and safe feeling
- Key buildings:
  - **Professor Sollis's Lab** — slightly larger house, sign outside
  - **Player's House** — smallest house, top-left of village
  - **Cass's House** — next to player's house
  - **PokéCenter** — center of village
  - **PokéMart** — next to PokéCenter
  - **Village Elder's House** — bottom-right, optional lore NPC
- NPCs:
  - Fisherman on dock south of village
  - Child running around
  - Old woman tending garden
  - Sailor looking at the sea
- Connects: south to Haven Isle Harbor, north to Coastal Route 1,
  east to Coastal Route 2
- Tileset: Littleroot/Pallet aesthetic but coastal

### Professor Sollis's Lab (Interior)
- Standard lab layout but with nautical decor
- Three Pokémon loose in the room: Totodile (by the door), Chimchar
  (on a high bookshelf), Rowlet (sitting on a journal on the desk)
- Professor Sollis sprite: use FEMALE_SCIENTIST placeholder for now
- The Warden's old journal is on the desk (key item trigger)
- Bookshelves, research tables, a map of Pelagios on the wall
- Script triggers:
  - First entry: intro cutscene, professor dialogue
  - Desk journal: gives WARDENS_JOURNAL key item after starter chosen
  - Each Pokémon: brief flavour dialogue if interacted with before choosing

### Player's House (Interior)
- Small: bed, PC, bookshelf, mom NPC (use MOM placeholder)
- Mom dialogue changes based on story progress flags
- PC: standard healing PC
- Game opens here — player wakes up, mom calls them downstairs

### Coastal Route 1
- Medium length route running north from village
- Wild Pokémon: Rattata/Yungoos (common), Pidgey/Taillow (common),
  Mareep (uncommon), Marill (water's edge, uncommon)
- Leads to Ancient Ruins entrance
- One trainer: youngster near the middle
- Berry trees: 2x Oran Berry
- Tileset: route with coastal cliffs to the east, grass patches

### Ancient Ruins (Exterior + Interior)
- Exterior: overgrown stone entrance, crumbling walls
- Interior Room 1: puzzle room, broken tiles, faint glow effect on
  centre tile (script trigger — reacts to legendary energy on game open)
- Interior Room 2: deeper chamber, ancient inscription on wall
  (examine for lore text about the seal network)
- No wild Pokémon in ruins
- Script: on first visit, brief screen flash + Ralts appears and
  flees (foreshadowing, not catchable yet)
- Tileset: cave/ruins mix

### Coastal Route 2
- Short route east of village leading to fishing docks
- Water route with Marill, Totodile (rare), Horsea (rare) via fishing
- One fisherman trainer
- Tileset: coastal/beach

### Fishing Docks
- Tiny map, 2 NPCs
- Old fisherman gives the player an Old Rod
- Other NPC gives fishing tips
- Connects back to Coastal Route 2

---

## KEY ITEMS TO DEFINE

Add these to the items system:

| Item | Description | Obtained |
|------|-------------|---------|
| WARDENS_JOURNAL | "Your parent's weathered journal. Some entries are encoded." | Professor's lab, start |
| NAVIGATORS_LOG | "An ancient sailing chart. Lets you traverse open water." | Haven Isle, after first story beat |
| GRAPPLE_HOOK | "Military-grade equipment. Traverse crumbling walls and barriers." | Ironhold |
| LAVA_BOOTS | "Fireproof boots. Walk across ash fields and lava rock." | Emberveil |
| SONAR_LENS | "Ancient technology from the Sunken Kingdom. Allows deep diving." | Thalvern |
| BEAST_WHISTLE | "A carved bone whistle. Jungle creatures clear paths at its call." | Primalis |
| STORM_COMPASS | "Navigates between any visited island harbor instantly." | Aetheron |
| PHANTOM_LANTERN | "Reveals hidden paths in fog and darkness." | Ashenveil |
| SEAL_SHARD_1 | "A fragment of crystallized legendary energy from Ironhold." | Ironhold |
| TENNYSON_KEY | "The key to the Tennyson. Your parent's boat." | Player's house, game start |

HM moves are REPLACED by these key items. Do not use HM moves for
field traversal. The NavigatorsLog replaces Surf, GrappleHook replaces
Strength/Rock Smash, etc.

---

## STARTER POKÉMON SETUP

The player chooses from three starters in Professor Sollis's lab.
All three are loose in the room, not in Pokéballs on a table.

| Starter | Type | Position in lab |
|---------|------|----------------|
| Totodile | Water | Near the door, runs at player |
| Chimchar | Fire | On the highest bookshelf |
| Rowlet | Grass/Flying | On the desk, sitting on journal |

**Script flow:**
1. Player enters lab
2. Sollis dialogue: "I've been trying to figure out which one to give
   you for six months. Apparently they've already decided."
3. Player approaches each Pokémon for flavour text
4. Choice menu appears when player interacts with chosen Pokémon
5. Confirmation, nickname prompt
6. Rival Cass receives the counter-type off-screen (mentioned in dialogue)
7. Sollis gives WARDENS_JOURNAL
8. Player receives TENNYSON_KEY from their house PC

**Rival counter-type logic:**
- Player picks Totodile → Cass gets Chimchar
- Player picks Chimchar → Cass gets Rowlet  
- Player picks Rowlet → Cass gets Totodile

---

## STORY FLAGS & VARIABLES TO ESTABLISH

Set up these flags from the start so all future scripting is consistent:

```c
// Story progression flags
FLAG_STARTER_CHOSEN
FLAG_JOURNAL_OBTAINED
FLAG_HAVEN_RUINS_VISITED
FLAG_HAVEN_ISLE_COMPLETE
FLAG_BOAT_SLOOP_UNLOCKED

// Relationship variables (0-3 scale)
VAR_CASS_RELATIONSHIP      // 0=Distant, 1=Competitive, 2=Trusting, 3=Devoted
VAR_DORNE_RELATIONSHIP     // 0=Adversarial, 1=Wary, 2=Conflicted, 3=Reluctant Respect
VAR_MAREN_RELATIONSHIP     // 0=Distant, 1=Strained, 2=Honest, 3=Reconciled
VAR_SEVER_RELATIONSHIP     // 0=Unacknowledged, 1=Respected
VAR_DAGAN_RELATIONSHIP     // 0-3 comedy scale

// Island resolution flags
FLAG_IRONHOLD_RESOLVED
FLAG_SIROCCO_RESOLVED
FLAG_EMBERVEIL_RESOLVED
FLAG_SCHISM_RESOLVED
FLAG_THALVERN_RESOLVED
FLAG_GILDHAVEN_RESOLVED
FLAG_PRIMALIS_RESOLVED
FLAG_ASHENVEIL_VISITED
FLAG_AETHERON_RESOLVED

// Choice flags
FLAG_DORNE_CHOICE_STOP      // Player chose to stop Dorne
FLAG_DORNE_CHOICE_HELP      // Player chose to help Dorne
FLAG_DORNE_CHOICE_DEFER     // Player chose "I need time"
FLAG_TRUE_ENDING_UNLOCKED   // Player decoded full journal
FLAG_SOLACE_ALT_ENDING      // Player found the commune ritual
FLAG_DEX_ALIVE              // Player went ahead of Dex in Thalvern
FLAG_DRENN_ALIVE            // Ceasefire achieved on Schism Isle
FLAG_CASS_DEFECTED          // Cass defected on Aetheron

// Journal cipher progress
FLAG_CIPHER_1_FOUND through FLAG_CIPHER_9_FOUND

// Memory Shard collection
FLAG_MEMORY_SHARD_1 through FLAG_MEMORY_SHARD_10
```

---

## MAP GROUPS STRUCTURE

Add a new map group for each island. Start with HAVEN_ISLE:

```
MAP_GROUP_HAVEN_ISLE:
  - HavenIsle_Harbor
  - HavenIsle_TidemarkVillage
  - HavenIsle_SollisLab
  - HavenIsle_SollisLab_1F  (if split floors needed)
  - HavenIsle_PlayerHouse
  - HavenIsle_PlayerHouse_1F
  - HavenIsle_CassHouse
  - HavenIsle_Route1
  - HavenIsle_AncientRuins_Exterior
  - HavenIsle_AncientRuins_Interior1
  - HavenIsle_AncientRuins_Interior2
  - HavenIsle_Route2
  - HavenIsle_FishingDocks
```

Future groups to add (stubs only for now):
```
MAP_GROUP_IRONHOLD
MAP_GROUP_SIROCCO
MAP_GROUP_EMBERVEIL
MAP_GROUP_SCHISM
MAP_GROUP_THALVERN
MAP_GROUP_GILDHAVEN
MAP_GROUP_PRIMALIS
MAP_GROUP_ASHENVEIL
MAP_GROUP_AETHERON
MAP_GROUP_CONVERGENCE
```

---

## WILD POKÉMON ENCOUNTERS

### Haven Isle encounters (use existing encounter tables format):

**Coastal Route 1 (Grass):**
```
Common (40%):  Rattata, Yungoos
Common (30%):  Pidgey, Taillow  
Uncommon (20%): Mareep
Rare (10%):    Gastly (night only)
```

**Coastal Route 1 (Surfing):**
```
Common (60%): Marill
Uncommon (40%): Azumarill
```

**Coastal Route 2 (Grass):**
```
Common (50%): Yungoos
Common (30%): Taillow
Uncommon (20%): Ralts
```

**Coastal Route 2 (Surfing):**
```
Common (70%): Marill
Uncommon (30%): Horsea
```

**Coastal Route 2 (Fishing - Old Rod):**
```
Common (70%): Magikarpw
Uncommon (30%): Horsea
```

**Coastal Route 2 (Fishing - Good Rod):**
```
Common (60%): Horsea
Uncommon (40%): Totodile (rare find, not starter)
```

**Ancient Ruins:**
No wild encounters. Scripted Ralts appearance only.

---

## OPENING SEQUENCE SCRIPT

The game should open as follows:

1. **Fade in** — Player's bedroom, morning light
2. **Mom calls** from downstairs (text box, no movement)
3. Player gains control, walks downstairs
4. Mom dialogue: brief, warm, mentions Professor Sollis wants to see
   the player, mentions the Tennyson hasn't been touched since
   the Warden died
5. Player exits house
6. **Cass intercepts** outside — brief enthusiastic dialogue, says
   they're headed to Sollis's lab too, races the player there
7. Player arrives at lab — Cass is already inside talking to Sollis
8. **Starter selection sequence** (described above)
9. After selection — Sollis gives WARDENS_JOURNAL, brief dialogue
   about the Warden
10. Cass leaves first, player can explore village before leaving
11. **First seal disturbance trigger** — when player approaches
    Ancient Ruins entrance on Route 1, brief screen shake + flash,
    wild Pokémon scatter animation, Sollis calls on PokéNav asking
    player to investigate
12. Player enters ruins, completes ruins sequence
13. Returns to Sollis — she explains what a Warden is (first time
    the word is used), tells player about Ironhold disturbance
14. **Haven Isle complete flag set**, Sloop upgrade dialogue,
    boat becomes usable at harbor

---

## TRAINER DATA — HAVEN ISLE

```
TRAINER_YOUNGSTER_HAVEN_1:
  Name: "Jay"
  Party: Rattata (Lv.5), Pidgey (Lv.5)
  Pre-battle: "Hey! You're the Warden's kid, right? 
               Let's see if you inherited the talent!"
  Post-battle: "Whoa! Okay, okay. You're legit."

TRAINER_FISHERMAN_HAVEN_1:
  Name: "Old Bren"
  Party: Magikarp (Lv.4), Marill (Lv.6)
  Pre-battle: "These waters've been strange lately. 
               Fish acting weird. Battle me while I think."
  Post-battle: "Strange times. Your parent used to 
               say the same thing, years back."
```

---

## NPC DIALOGUE — HAVEN ISLE

Write dialogue that:
- Establishes the world as a seafaring archipelago
- References the Warden's recent death subtly (players will pick up
  on the somber undertone)
- Makes Haven Isle feel lived-in and warm before everything goes wrong
- Plants early seeds of the Covenant's presence (a sailor mentions
  "Covenant ships passing more often lately")

Key NPCs:
- **Sailor at harbor:** references Covenant ships, uneasy tone
- **Old woman in garden:** knew the Warden personally, warm, sad
- **Child running around:** pure energy, no plot, comic relief
- **Village Elder:** optional lore, explains Haven Isle's history
  briefly, hints the ruins are older than anyone knows
- **Fisherman on dock:** gives Old Rod, mentions fish behaving strangely

---

## TECHNICAL NOTES

- Match ALL file formats exactly to existing maps in the project
- Use tilesets already present in the repo — do not reference
  tilesets that don't exist
- All scripts should use the scripting format consistent with
  the rest of pokeemerald-expansion
- Register all new maps in map_groups.json AND the appropriate
  header files
- All new flags and variables must be added to
  include/constants/flags.h and include/constants/vars.h
- New items must be added to include/constants/items.h and
  src/data/items.h with appropriate data
- After generating all files, verify the project still compiles
  with: gmake -j$(sysctl -n hw.ncpu)
- Fix any compilation errors before finishing

---

## WHAT TO BUILD FIRST (PRIORITY ORDER)

1. Flag and variable constants (flags.h, vars.h)
2. Key item definitions (items.h)
3. Map group stubs for all islands (map_groups.json)
4. Haven Isle map files (all maps listed above)
5. Opening sequence script
6. Starter selection script
7. Wild encounter tables for Haven Isle routes
8. Trainer data
9. NPC dialogue scripts
10. Compile and fix errors

---

## STYLE GUIDELINES

- Dialogue should feel like a Pokémon game in tone but with more
  emotional depth — not childish, not grimdark
- NPC names should fit a nautical/adventure world
- Map dimensions should feel proportional — don't make maps too
  large (tedious to traverse) or too small (claustrophobic)
- Tidemark Village should feel comparable in size to Littleroot Town
- Routes should feel comparable to early Hoenn routes
- The Ancient Ruins should feel slightly eerie even before anything
  happens there

---

*This brief covers Haven Isle only. Subsequent islands will be
briefed separately once Haven Isle compiles and is playable.*

# Pokemon Pelagios

GBA ROM hack built on pokeemerald-expansion.
Full spec: PELAGIOS_BRIEF.md
Build command: gmake -j$(sysctl -n hw.ncpu)
Output: pokeemerald.gba

Always compile and fix errors after each major change.