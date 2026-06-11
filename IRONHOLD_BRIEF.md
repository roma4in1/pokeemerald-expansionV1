# IRONHOLD — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle must be fully built and compiling cleanly ✅
- ITEM_NAVIGATORS_LOG must exist in items.h ✅
- VAR_BOAT_TIER must exist in vars.h ✅
- FLAG_HAVEN_ISLE_COMPLETE must exist in flags.h ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Ironhold |
| Theme | Occupied Island — military garrison, political tension |
| Primary types | Steel / Fighting |
| Battle terrain | Clear weather, stone/fortress battle backgrounds |
| Legendary | Ferrath (Steel/Fighting) — sealed inside the Summit Fortress |
| Boat required | Sloop (unlocked at end of Haven Isle) |
| Brigantine unlocked | After FLAG_IRONHOLD_RESOLVED is set |

---

## Map Group

```
MAP_GROUP_IRONHOLD:
  - Ironhold_GatemarkPort
  - Ironhold_GatemarkPort_Inn
  - Ironhold_GatemarkPort_Inn_Interior
  - Ironhold_OuterDistrict
  - Ironhold_IronholdCity
  - Ironhold_IronholdCity_Armory
  - Ironhold_IronholdCity_Armory_Interior
  - Ironhold_IronholdCity_PokemonCenter
  - Ironhold_IronholdCity_PokeMart
  - Ironhold_ResistanceHideout
  - Ironhold_MountainPass
  - Ironhold_MountainPass_Cave
  - Ironhold_SummitApproach
  - Ironhold_SummitFortress_Exterior
  - Ironhold_SummitFortress_Interior1
  - Ironhold_SummitFortress_Interior2
  - Ironhold_SummitFortress_SealChamber
```

---

## Map Descriptions

### Ironhold_GatemarkPort
- Entry point from sea via Sloop
- Covenant checkpoint at the dock — guard NPCs check the player's
  Navigator's Log before allowing entry
- The Tennyson docks here on arrival
- Small port town, grey stone buildings, Covenant flags everywhere
- Key NPCs:
  - Dock guard (blocks entry without Navigator's Log check)
  - Sailor who recognizes the Tennyson — knew the Warden
  - Merchant selling basic supplies
  - Resistance contact disguised as a fisherman (gives hint
    about the underground after player defeats Gym 1)
- Inn available for healing (Ironhold_GatemarkPort_Inn)
- Sign at north exit: "Ironhold City — Covenant Garrison HQ"
- Connects north to Ironhold_OuterDistrict
- Tileset: port/harbor but darker, more industrial than Haven Isle

### Ironhold_GatemarkPort_Inn / Interior
- Standard inn layout
- Innkeeper NPC — nervous, clearly watched by Covenant
- One traveler NPC who mentions "strange readings" near the summit

### Ironhold_OuterDistrict
- Transition area between port and city
- Outer ring of Ironhold — working class, less Covenant presence
- Wild Pokémon visible in patches of scrubland at the edges
- Two trainer encounters here (Officer Jenny variant, worker)
- Resistance graffiti visible on one wall (examine for lore text)
- Hidden item: Antidote in rubble pile
- Connects south to GatemarkPort, north to IronholdCity
- Connects west to MountainPass entrance (blocked by Grapple Hook
  rubble until ITEM_GRAPPLE_HOOK is obtained)
- Tileset: route/town hybrid, stone walls, industrial

### Ironhold_IronholdCity
- Main city — Covenant garrison HQ, most NPCs, all key buildings
- Covenant soldiers patrol in pairs on set movement scripts
- Key buildings:
  - Armory (Gym 2 — Forge's location)
  - PokéCenter
  - PokéMart (Covenant-branded, slightly overpriced)
  - Commander Sever's HQ (locked until post-Gym 3)
  - Resistance contact house (looks ordinary, hidden basement)
- Key NPCs:
  - Covenant soldier (blocks HQ entrance)
  - Ironhold citizen who whispers about the summit
  - Elder who remembers Ironhold before the occupation
  - Child who salutes every passing soldier (uncomfortable detail)
  - Covenant officer who recognizes the Warden's name — goes quiet
- Gym 1 (Petra) is in a converted town hall at the north end
- Connects south to OuterDistrict
- Connects north to SummitApproach (blocked until Gym 3 cleared)
- Tileset: town but fortress-aesthetic, grey stone, iron gates

### Ironhold_IronholdCity_Armory / Interior
- Gym 2 location (Forge)
- Exterior: large stone building with forge smoke
- Interior: workshop aesthetic, anvils, steel equipment
- Gym puzzle: navigate around cooling metal frames that block paths
- Forge is at the back, working at an anvil when player arrives

### Ironhold_ResistanceHideout
- Hidden entrance behind a bookshelf in the resistance contact's house
- Small basement room — maps on the wall, documents, a radio
- Three resistance members NPCs
- Key scene: resistance leader gives player the first piece of
  evidence about Covenant seal exploitation (a document fragment)
- Accessible after defeating Gym 1 and speaking to disguised
  fisherman at the port
- No wild Pokémon
- Tileset: cave/interior hybrid, dim lighting

### Ironhold_MountainPass
- Route connecting IronholdCity to the Summit
- Blocked initially by collapsed rocks (requires ITEM_GRAPPLE_HOOK)
- ITEM_GRAPPLE_HOOK is obtained from Forge after defeating Gym 2
- Medium length route, rocky terrain, cliff edges
- Two trainer encounters (hiker, Covenant patrol)
- Wild Pokémon in rocky patches
- Hidden item: Iron (hidden in rock pile)
- Connects east to IronholdCity, north to SummitApproach

### Ironhold_MountainPass_Cave
- Short cave cutting through the mountain
- No puzzle — linear path
- Wild Pokémon (cave encounters, Aron common)
- One trainer inside (Covenant scout)
- Examine cave wall for ancient inscription — same script as
  Haven Isle ruins, first cross-island lore connection

### Ironhold_SummitApproach
- Final outdoor area before the fortress
- Dramatic vista — player can see the whole island below
- Covenant's strongest patrol trainers here (3 trainers)
- Weather shifts to overcast/stormy here
- Seal energy visible as faint glow from fortress ahead
- Connects south to MountainPass, north to SummitFortress_Exterior

### Ironhold_SummitFortress_Exterior
- Imposing fortress gate
- Two Covenant elite guards as trainer battles before entry
- Commander Sever appears here after player defeats both guards —
  brief dialogue, steps aside, says nothing more
- Connects south to SummitApproach
- Connects north into SummitFortress_Interior1

### Ironhold_SummitFortress_Interior1
- First floor of fortress — barracks, training hall
- Gym 3 location (Rook — Sever's lieutenant)
- Gym puzzle: locked doors opened by defeating stationed soldiers
  in sequence
- Rook is in the central training hall
- After defeating Rook, door to Interior2 unlocks

### Ironhold_SummitFortress_Interior2
- Upper floor — command level
- Gym 4 location (Commander Holt Sever)
- More austere than Interior1 — Sever's personal space
- A map of the whole Pelagios region on the wall
  (examine for worldbuilding text, region overview)
- After defeating Sever — key scene plays (see Scripts section)
- Door to SealChamber unlocks

### Ironhold_SummitFortress_SealChamber
- Deepest room — where Ferrath is sealed
- Ancient architecture visible beneath the fortress construction
- Machinery the Covenant built to siphon seal energy
- Ferrath visible but not interactable during main story
- Script trigger at chamber entrance: discovery cutscene,
  player sees the siphoning apparatus, seal glowing weakly
- Player interacts with apparatus to reinforce the seal
- FLAG_IRONHOLD_RESOLVED set here
- Warden's Journal entry unlocks here (cipher 1 of 9)
- No wild Pokémon in this room

---

## Geography & Connections

```
[Sea / Tennyson docked]
        ↓
Ironhold_GatemarkPort
        ↓
Ironhold_OuterDistrict ←→ [MountainPass blocked until Grapple Hook]
        ↓
Ironhold_IronholdCity
  ├── Armory (Gym 2)
  ├── ResistanceHideout (hidden)
  └── [North blocked until Gym 3]
        ↓
Ironhold_SummitApproach
        ↓
Ironhold_SummitFortress_Exterior
        ↓
Ironhold_SummitFortress_Interior1 (Gym 3)
        ↓
Ironhold_SummitFortress_Interior2 (Gym 4 — Sever)
        ↓
Ironhold_SummitFortress_SealChamber
```

---

## Gym Leaders

### Gym 1 — Petra
- Location: Converted town hall, north end of IronholdCity
- Type specialist: Fighting
- Badge: Iron Badge (Badge 1 of Ironhold, Badge 2 overall)
- Level range: 14-17
- Party:
  - Machop Lv.14
  - Hariyama Lv.15
  - Pangoro Lv.17
- Gym puzzle: weighted pressure plates that must be stepped on
  in correct order to unlock Petra's barrier
- Pre-battle dialogue:
  "You want to pass through Ironhold? Fine. Show me what you've got.
   I don't care who your parent was — earn it."
- Post-battle dialogue:
  "Not bad. Honest answer? I'm glad someone's pushing back.
   Just... be careful who you say that to around here."
- Gives: Iron Badge, TM for Bulk Up

### Gym 2 — Forge
- Location: Ironhold Armory
- Type specialist: Steel
- Badge: Steel Badge (Badge 2 of Ironhold, Badge 3 overall)
- Level range: 18-22
- Party:
  - Steelix Lv.18
  - Magnezone Lv.20
  - Klefki Lv.20
  - Bisharp Lv.22
- Post-battle reward: gives player ITEM_GRAPPLE_HOOK
- Pre-battle dialogue:
  "I make weapons for the Covenant. I don't ask what they do with them.
   That used to feel like enough. Beat me and maybe I'll think harder."
- Post-battle dialogue:
  "Take this. Military salvage — they'll never miss one.
   The mountain pass north of the city. There's more up there
   than the Covenant wants people to see."
- Gives: Steel Badge, TM for Iron Defense, ITEM_GRAPPLE_HOOK

### Gym 3 — Rook
- Location: SummitFortress_Interior1
- Type: Fighting/Steel mix
- Badge: Garrison Badge (Badge 3 of Ironhold, Badge 4 overall)
- Level range: 24-28
- Party:
  - Lucario Lv.24
  - Aegislash Lv.26
  - Gallade Lv.26
  - Scizor Lv.28
- Pre-battle dialogue:
  "Commander Sever said to let you through if you beat me.
   I don't understand why. I'm going to follow orders anyway."
- Post-battle dialogue:
  "...He said you'd get this far. He said to tell you:
   'The report exists. It always has.'"
- Gives: Garrison Badge, TM for Close Combat

### Gym 4 — Commander Holt Sever
- Location: SummitFortress_Interior2
- Type: Steel/Fighting mix (mixed team)
- Badge: Covenant Badge (Badge 4 of Ironhold, Badge 5 overall)
- Level range: 30-35
- Party:
  - Bisharp Lv.30
  - Excadrill Lv.31
  - Conkeldurr Lv.32
  - Lucario Lv.33
  - Metagross Lv.35
- Pre-battle dialogue:
  "I know why you're here. I know what you'll find in that chamber.
   I've known for three years. Defeat me, and I won't stop you."
- Post-battle dialogue (key scene — see Scripts):
  Sever stands down. Gives no badge speech. Opens the SealChamber
  door. Says one line:
  "Your parent came here six months ago. I told them the same
   thing I'm telling you now: I know. I won't act. I'm sorry."
  Then steps aside and says nothing more.
- Gives: Covenant Badge, TM for Meteor Mash
- NOTE: Sever's VAR_SEVER_RELATIONSHIP increments to RESPECTED
  if player selects dialogue option "I understand" after his
  post-battle speech. Stays UNACKNOWLEDGED if player says nothing
  or selects "That's not enough."

---

## Key Characters

### Commander Holt Sever
- Sprite: Use COOLTRAINER_M placeholder until custom sprite ready
- Overworld: patrols SummitFortress_Interior2
- Is present at fortress exterior gate before player enters
  (brief dialogue, steps aside)
- Does NOT appear on Ironhold after FLAG_IRONHOLD_RESOLVED
  (he has gone quiet — filed no report)
- Reappears in later islands as a background presence
  (see PELAGIOS_BRIEF.md for his full arc)

### Resistance Leader (no name — referred to as "the contact")
- Sprite: Use YOUNGSTER placeholder until custom sprite ready
- Only appears in ResistanceHideout
- Gives player the document fragment (key item: ITEM_DOCUMENT_FRAGMENT)
- Dialogue reflects fear but determination
- References the Warden: "Your parent was here. They took a copy
  of this document. We never heard from them again."

### Forge (Gym 2)
- Sprite: Use COOLTRAINER_M placeholder
- Overworld: working at anvil in Armory interior
- Gives ITEM_GRAPPLE_HOOK post-battle
- Reappears as background NPC in IronholdCity after resolution
  (now idle, no longer working — something changed in him)

---

## New Flags Required

Add to include/constants/flags.h:

```c
// Ironhold progression
FLAG_IRONHOLD_ARRIVED           // Player first arrived at Ironhold
FLAG_IRONHOLD_GYM1_CLEAR        // Petra defeated
FLAG_IRONHOLD_GYM2_CLEAR        // Forge defeated
FLAG_IRONHOLD_GYM3_CLEAR        // Rook defeated
FLAG_IRONHOLD_GYM4_CLEAR        // Sever defeated
FLAG_IRONHOLD_RESISTANCE_MET    // Player found the hideout
FLAG_IRONHOLD_SEAL_FOUND        // Player discovered the siphon apparatus
FLAG_IRONHOLD_RESOLVED          // Seal reinforced, island complete
FLAG_IRONHOLD_CIPHER_FOUND      // Warden's Journal cipher 1 collected
FLAG_GRAPPLE_HOOK_OBTAINED      // ITEM_GRAPPLE_HOOK received from Forge
FLAG_DOCUMENT_FRAGMENT_OBTAINED // Resistance document received
```

---

## New Variables Required

Add to include/constants/vars.h:

```c
VAR_IRONHOLD_PROGRESS   // 0=arrived, 1=gym1, 2=gym2, 3=gym3, 4=sever, 5=resolved
```

Note: VAR_SEVER_RELATIONSHIP already defined in Haven Isle constants.
Confirm it exists before adding again.

---

## New Items Required

Add to include/constants/items.h and src/data/items.h:

| Constant | Name | Description | Type |
|---|---|---|---|
| ITEM_GRAPPLE_HOOK | Grapple Hook | "Military-grade equipment. Traverse crumbling walls and barriers." | Key Item |
| ITEM_DOCUMENT_FRAGMENT | Document Fragment | "A partial Covenant report. Some words are redacted. The ones that aren't are damning." | Key Item |
| ITEM_SEAL_SHARD_IRONHOLD | Ironhold Seal Shard | "A fragment of crystallized legendary energy from Ironhold's summit." | Key Item — NOT OBTAINABLE YET (stub only, obtained in future update) |

---

## Wild Pokémon Encounters

### Ironhold_OuterDistrict (Grass patches)
```
Common (40%):   Machop
Common (30%):   Aron
Uncommon (20%): Meditite
Rare (10%):     Riolu
```

### Ironhold_MountainPass (Rocky terrain)
```
Common (40%):   Aron
Common (30%):   Geodude (Alolan)
Uncommon (20%): Bronzor
Rare (10%):     Beldum
```

### Ironhold_MountainPass_Cave
```
Common (50%):   Aron
Common (30%):   Bronzor
Uncommon (15%): Pawniard
Rare (5%):      Beldum
```

### Ironhold_SummitApproach (Rocky patches)
```
Common (50%):   Aron
Uncommon (30%): Pawniard
Rare (20%):     Galarian Farfetch'd
```

No wild encounters in:
- GatemarkPort
- IronholdCity
- ResistanceHideout
- SummitFortress (any floor)
- SealChamber

---

## Trainer Data

### TRAINER_OFFICER_IRONHOLD_1
- Name: "Officer Mace"
- Location: Ironhold_OuterDistrict
- Party: Growlithe Lv.12, Machoke Lv.14
- Pre-battle: "Halt. Covenant regulation 7: all travelers must
  be assessed for hostile intent. Prepare to be assessed."
- Post-battle: "...Assessment complete. You may pass.
  Don't make me regret it."

### TRAINER_WORKER_IRONHOLD_1
- Name: "Worker Finn"
- Location: Ironhold_OuterDistrict
- Party: Aron Lv.11, Aron Lv.11, Lairon Lv.13
- Pre-battle: "I just work the forges. I don't want trouble.
  But I can't just let strangers walk past either."
- Post-battle: "You're strong. The Warden's kid, right?
  Someone said you'd come eventually."

### TRAINER_HIKER_IRONHOLD_1
- Name: "Hiker Crag"
- Location: Ironhold_MountainPass
- Party: Graveler Lv.16, Onix Lv.17, Steelix Lv.18
- Pre-battle: "The mountain doesn't care who you are.
  Neither do I. Let's go."
- Post-battle: "The summit's changed. Something up there
  feels wrong. Has for months."

### TRAINER_COVENANT_IRONHOLD_1
- Name: "Soldier Venn"
- Location: Ironhold_MountainPass
- Party: Riolu Lv.15, Meditite Lv.15, Lucario Lv.17
- Pre-battle: "Unauthorized personnel beyond this point.
  Covenant standing order — turn back or be escorted."
- Post-battle: "How... Commander Sever said no one could
  get past here. I need to file a report."

### TRAINER_COVENANT_IRONHOLD_2
- Name: "Scout Hale"
- Location: Ironhold_MountainPass_Cave
- Party: Bronzor Lv.16, Aron Lv.17, Pawniard Lv.17
- Pre-battle: "The cave's restricted. I don't make the rules."
- Post-battle: "Fine. Go. I never saw you."

### TRAINER_COVENANT_IRONHOLD_3
- Name: "Elite Guard Sorn"
- Location: Ironhold_SummitApproach
- Party: Bisharp Lv.22, Lucario Lv.22, Scizor Lv.24
- Pre-battle: "Final checkpoint. Last chance to turn around."
- Post-battle: "..."
  (Says nothing after losing. Steps aside in silence.)

### TRAINER_COVENANT_IRONHOLD_4
- Name: "Elite Guard Vael" (not the villain — different character,
  common Covenant name, deliberate subtle detail)
- Location: Ironhold_SummitFortress_Exterior
- Party: Metagross Lv.26, Aegislash Lv.26, Bisharp Lv.28
- Pre-battle: "The Commander doesn't want visitors."
- Post-battle: "...Go ahead. He's expecting you."

---

## NPC Dialogue Guidelines

**Gatemark Port Sailor (recognizes Tennyson):**
"That boat. I haven't seen her in port since... well.
 Your parent kept her in good shape. You sailing her alone?"
(If player says yes): "Good. She's a strong vessel.
 Ironhold's a harder harbor than Haven Isle. Keep your eyes open."

**Innkeeper:**
"Room's yours. I'd say enjoy your stay but —
 well. It's Ironhold. Get some rest."

**Traveler at inn:**
"I came here for trade. The Covenant's doubled the checkpoint
 fees since last year. And the summit — something's making
 the Steel-types aggressive up there. Nobody official will say why."

**Resistance graffiti (examine):**
[The words are partially scratched out but readable:]
"THE SUMMIT FEEDS ON SOMETHING IT SHOULDN'T"

**IronholdCity elder:**
"This city had a different name before the Covenant arrived.
 We don't use it anymore. It's easier that way.
 Your parent knew the old name. I wonder if they told you."

**Child saluting soldiers:**
"One day I'm going to be a Covenant soldier!
 They get to carry real weapons!"
(Pause) "...My dad says that's not something to want.
 But he says it quietly."

**Covenant officer who recognizes Warden's name:**
"Warden? I... that name is in our records.
 I shouldn't say anything else."
(Turns away, won't speak again)

**IronholdCity citizen (whisper):**
"Don't look at the summit at night. The glow started
 three months ago. Covenant says it's equipment testing.
 Equipment doesn't pulse like that."

**Post-resolution NPCs (after FLAG_IRONHOLD_RESOLVED):**
- Petra: "The siphon's offline. I can feel it — the Pokémon
  in the city are calmer already. Whatever you did up there..."
- Forge: (Now idle, staring at his hands) "I made tools
  for that machine. Didn't ask what it was for.
  I should have asked."
- Elder: "Something changed on the summit today.
  The old name for this city was Ferrath's Rest.
  Now you know why."

---

## Key Scripts

### Arrival Script (first time player docks at GatemarkPort)
- Dock guard demands to see Navigator's Log
- Check for ITEM_NAVIGATORS_LOG — if missing, turn player back
  with dialogue: "No sailing papers, no entry. Covenant law."
- If present: guard examines it, pauses, waves player through
  with clear discomfort — "Warden's log. You may pass."
- FLAG_IRONHOLD_ARRIVED set
- Brief cutscene: camera pans up to show summit fortress

### Resistance Hideout Discovery
- Triggered by interacting with bookshelf in contact's house
- Requires FLAG_IRONHOLD_GYM1_CLEAR
- If flag not set: bookshelf is just a bookshelf
- If flag set: bookshelf moves, stairs revealed
- On first entry: resistance leader dialogue + ITEM_DOCUMENT_FRAGMENT given
- FLAG_IRONHOLD_RESISTANCE_MET set
- FLAG_DOCUMENT_FRAGMENT_OBTAINED set

### Forge Post-Battle (Grapple Hook handoff)
- After battle ends, Forge puts down his hammer
- Dialogue plays (see trainer data above)
- ITEM_GRAPPLE_HOOK given to player
- FLAG_GRAPPLE_HOOK_OBTAINED set
- MountainPass rubble now clears when player approaches it

### Grapple Hook Field Effect
- When player stands adjacent to crumbled rock barriers and
  presses A: check for ITEM_GRAPPLE_HOOK
- If present: brief animation, rocks clear, path opens
- If absent: "The rocks are too heavy to move."
- Applies to: MountainPass entrance rubble,
  SummitFortress gate mechanism

### Sever Post-Battle (Key Scene)
- Battle ends
- Sever does not give badge speech
- Walks to SealChamber door, unlocks it
- Delivers one line (see Gym 4 section above)
- CHOICE POINT: player selects response
  - "I understand." → VAR_SEVER_RELATIONSHIP = RESPECTED (1)
  - [Say nothing / walk past] → VAR_SEVER_RELATIONSHIP stays 0
- Sever does not speak again on this island
- FLAG_IRONHOLD_GYM4_CLEAR set

### SealChamber Discovery Cutscene
- Triggered on first entry to SealChamber
- Screen dims slightly
- Camera pans across the Covenant machinery
- Ferrath visible in the background — weakened, sealed
- Player approaches apparatus
- Screen flash + brief rumble effect
- Dialogue: "The machinery is draining something.
  Something that shouldn't be drained."
- Player interacts with apparatus: reinforcement sequence
  (screen flash, machinery powers down, Ferrath's glow stabilizes)
- FLAG_IRONHOLD_SEAL_FOUND set

### Island Resolution Script
- Triggered after SealChamber seal reinforcement
- Brief cutscene: the glow from the summit dims to normal
- Warden's Journal entry unlocks — encoded page lights up
  (cipher 1 of 9 collected)
- FLAG_IRONHOLD_CIPHER_FOUND set
- FLAG_IRONHOLD_RESOLVED set
- VAR_BOAT_TIER increments to 2 (Brigantine unlocked)
- Dialogue: player contacts Sollis via PokéNav
- Sollis: "The readings from Ironhold just changed.
  What did you find up there?"
- Player: "The Covenant was using the seal. Draining it."
- Sollis: (long pause) "...Come back to Haven Isle when you can.
  There are things I should have told you earlier."
- Brigantine now available at GatemarkPort dock

### Warden's Journal Entry (Ironhold Cipher)
- Triggered after FLAG_IRONHOLD_RESOLVED
- In Warden's Journal key item, new encoded section unlocks
- Decoded text (requires no cipher yet — this one is partially
  legible, the others need ciphers collected):
  "Sever knows. He won't act but he knows. I don't blame him.
   What can one person do inside a system that large?
   I used to think I knew the answer to that question."

---

## Battle Terrain Setup

All Ironhold maps use clear weather by default.
Gym battles use: no terrain effect, stone/fortress battle background.
SummitApproach: overcast weather (WEATHER_CLOUDS).
SealChamber: no weather, dim lighting effect via palette.

Set in each map.json:
```json
"battle_scene": "BATTLE_SCENE_BUILDING"
```
For summit maps:
```json
"battle_scene": "BATTLE_SCENE_CAVE"
```

---

## Ironhold — Task Checklist

Work through in this order. Agents handle their domains:

### pelagios-systems-engineer (first) — DONE 2026-06-11, build exit 0
- [x] Add all Ironhold flags to include/constants/flags.h (0x4D9-0x4E2)
- [x] Add VAR_IRONHOLD_PROGRESS to include/constants/vars.h (0x40FF, last free slot)
- [x] Confirm VAR_SEVER_RELATIONSHIP exists (from Haven Isle) — present at 0x40FA
- [x] Add ITEM_GRAPPLE_HOOK to items.h and src/data/items.h — already existed (876)
- [x] Add ITEM_DOCUMENT_FRAGMENT to items.h and src/data/items.h (884)
- [x] Add ITEM_SEAL_SHARD_IRONHOLD stub to items.h — alias of ITEM_SEAL_SHARD_1 (882)
- [x] Add all 7 generic trainer entries to trainers.party (857-863)
- [ ] Add Ironhold map group stub to map_groups.json — SKIPPED: empty groups can't
      be expressed in groups.inc. Map-builder registers MAP_GROUP_IRONHOLD with map 1.
- ⚠️ BLOCKER: 4 gym leaders cannot get trainer IDs — trainer flag space FULL
      (TRAINERS_COUNT == MAX == 864). Needs flag-space refactor before gym scripting.

### pelagios-map-builder (second)
- [ ] Ironhold_GatemarkPort
- [ ] Ironhold_GatemarkPort_Inn
- [ ] Ironhold_GatemarkPort_Inn_Interior
- [ ] Ironhold_OuterDistrict
- [ ] Ironhold_IronholdCity
- [ ] Ironhold_IronholdCity_Armory
- [ ] Ironhold_IronholdCity_Armory_Interior
- [ ] Ironhold_IronholdCity_PokemonCenter
- [ ] Ironhold_IronholdCity_PokeMart
- [ ] Ironhold_ResistanceHideout
- [ ] Ironhold_MountainPass
- [ ] Ironhold_MountainPass_Cave
- [ ] Ironhold_SummitApproach
- [ ] Ironhold_SummitFortress_Exterior
- [ ] Ironhold_SummitFortress_Interior1
- [ ] Ironhold_SummitFortress_Interior2
- [ ] Ironhold_SummitFortress_SealChamber
- [ ] Wild encounter tables (OuterDistrict, MountainPass,
      MountainPass_Cave, SummitApproach)
- [ ] Heal location (GatemarkPort Inn)

### pelagios-script-writer (third)
- [ ] Arrival script (Navigator's Log check, cutscene)
- [ ] Resistance Hideout discovery script
- [ ] Forge post-battle Grapple Hook handoff script
- [ ] Grapple Hook field effect script
- [ ] Sever post-battle key scene + relationship choice
- [ ] SealChamber discovery cutscene
- [ ] Island resolution script + Brigantine unlock
- [ ] Warden's Journal cipher 1 unlock
- [ ] All gym leader pre/post battle dialogue
- [ ] All NPC dialogue (sailor, innkeeper, traveler, elder,
      child, officer, citizen, post-resolution NPCs)
- [ ] All trainer pre/post battle dialogue

### pelagios-build-debugger (last)
- [ ] Full compile passes with zero errors
- [ ] Verify Navigator's Log check works at port entry
- [ ] Verify Grapple Hook clears MountainPass rubble
- [ ] Verify Sever relationship variable sets correctly
- [ ] Verify FLAG_IRONHOLD_RESOLVED triggers Brigantine unlock
- [ ] Update CLAUDE.md — mark Ironhold complete

---

## Prompt to Start

Give this to the first agent (pelagios-systems-engineer):

```
Read CLAUDE.md and IRONHOLD_BRIEF.md. Haven Isle is complete
and compiling cleanly. Your task: implement all Ironhold
constants — flags, variables, items, trainer stubs, and map
group registration — following the task checklist in
IRONHOLD_BRIEF.md under "pelagios-systems-engineer".
Compile after completing constants and fix any errors.
Do not build maps or scripts — that is the next agent's job.
```

---

*This brief covers Ironhold only.
Next island brief: Sirocco Isle and Emberveil (any order).*
