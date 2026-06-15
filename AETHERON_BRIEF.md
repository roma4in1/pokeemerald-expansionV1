# AETHERON — ISLAND BRIEF (SKY ISLAND)
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
- Ashenveil complete ✅
- ITEM_SEA_CHART obtained ✅
- FLAG_SEA_CHART_FOUND set ✅
- Aetheron unlocked in boat menu ✅
- VAR_AETHERON_PROGRESS exists in vars.h ✅
- STORY BLOCK 4 available ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Aetheron |
| Theme | Sky Island — above the clouds, electric storms, ancient observatory |
| Primary types | Electric / Flying |
| Battle terrain | Electric Terrain, permanent thunderstorm |
| Legendary | Stormveil (Electric/Flying) — sky guardian, commands the Knock Up Stream |
| Access | Via Knock Up Stream scripted sequence (sea chart required) |
| Key item obtained | ITEM_STORM_COMPASS (replaces Fly — fast travel between islands) |
| Critical scene | Cass defects here. Joins player for Convergence. |

---

## Narrative Summary

Aetheron floats above the ocean, accessible only via the Knock Up Stream —
a massive upward current of air and water that the sea chart identifies.
The island exists above the clouds, permanently storm-swept, home to the
Stormveil legendary and a small community that has lived above the world
for generations.

**The Covenant has a military installation here** — they've been using
Stormveil's electrical output to power their operations across the region.
This is the most direct evidence of Covenant exploitation the player has
seen since Gildhaven's yield map.

**Cass is here, on Covenant orders.** But Cass has been carrying the
Ashenveil documents since Gildhaven — the proof of what the Covenant did
to that island. On Aetheron, Cass makes the decision.

**The defection scene:**
After the player defeats Gym 3 (Covenant Commander), Cass appears.
Cass has been given orders to stop the player.
Cass looks at the player for a long moment.
Then Cass steps aside.
"I found the documents. On Ashenveil.
 I know what they did.
 I'm done."
Cass hands the player the Ashenveil documents (lore item).
Cass and the player enter the SealChamber together.
From this point forward, Cass travels with the player.

**Key story beats:**
1. Knock Up Stream sequence — scripted ascent
2. Arrive at Aetheron — above the clouds, electric storms
3. Aetheron community NPCs — they know about the world below,
   choose to stay above it
4. Defeat Gym 1 (Gale) — community elder
5. Discover Covenant installation — scale of exploitation revealed
6. Defeat Gym 2 (Arc) — Covenant electrical engineer
7. Cass spotted — brief sighting, doesn't engage yet
8. Defeat Gym 3 (Commander Voss) — Covenant military commander
9. Cass defection scene — the moment the whole game built to
10. Enter SealChamber with Cass
11. Reinforce Stormveil's seal
12. ITEM_STORM_COMPASS obtained
13. Cass and player descend together — Convergence next
14. Cipher 9 already found on Ashenveil — no new cipher here
15. FLAG_AETHERON_RESOLVED set
16. FLAG_CASS_DEFECTED set

---

## Map Group

```
MAP_GROUP_AETHERON:
  - Aetheron_KnockUpStream
  - Aetheron_CloudLanding
  - Aetheron_CloudLanding_Inn
  - Aetheron_CloudLanding_Inn_Interior
  - Aetheron_SkyRoute
  - Aetheron_AetherVillage
  - Aetheron_AetherVillage_PokemonCenter
  - Aetheron_CovenantInstallation
  - Aetheron_CovenantInstallation_Interior
  - Aetheron_StormPeak
  - Aetheron_SealChamber
```

---

## Map Descriptions

### Aetheron_KnockUpStream
- The ascent sequence — not a navigable map
- Scripted cutscene only: the Tennyson caught in
  the massive upward current, spinning, rising
- Screen effects: rapid flashes, speed lines,
  Cass visible briefly in a Covenant vessel
  ascending alongside (doesn't acknowledge player)
- Fades to white — arrives at CloudLanding
- No NPCs, no wild Pokémon, no warps back
- Weather: WEATHER_RAIN + lightning

### Aetheron_CloudLanding
- The island's base — literally on top of clouds
- The clouds are solid ground here — white,
  slightly luminous, electric crackling visible
- The Tennyson docks on a cloud-pier
- Community members visible — calm despite the
  permanent storm above
- Inn available
- Key NPCs:
  - Cloud-pier dockhand: "You came up the Stream.
    First outsiders in three years."
  - Community member: "We can see the whole world
    from here. We chose not to be part of it."
- Connects to SkyRoute
- Weather: WEATHER_THUNDERSTORM
- Tileset: gTileset_General (cloud/sky palette)

### Aetheron_CloudLanding_Inn / Interior
- Small inn — community-run
- Innkeeper: "The storm never stops. You stop
  noticing after a while."
- Scholar NPC: "The Covenant came two years ago.
  Said they needed access to the peak.
  We said no. They came anyway."

### Aetheron_SkyRoute
- Route between CloudLanding and AetherVillage
- Open sky on both sides — vertigo-inducing
- Electric Terrain visible as crackling ground
- Wild Pokémon in storm patches
- Two trainer encounters (community guardians)
- Connects CloudLanding to AetherVillage
- Weather: WEATHER_THUNDERSTORM

### Aetheron_AetherVillage
- Main community settlement
- Built low against the wind — everything
  aerodynamic, nothing tall
- Gym 1 here (Gale — community elder)
- Gym 2 here (Arc — captured Covenant engineer
  who stayed when the installation was built)
- Covenant Installation visible on the horizon —
  angular, wrong, out of place
- Key NPCs:
  - Village elder (pre-battle with Gale)
  - Children who have never been below the clouds:
    "What's the ground like? Is it soft?"
  - Covenant defector NPC who stayed:
    "I was a technician. I helped build the
     installation. I couldn't leave after I saw
     what it was doing to Stormveil."
- Connects SkyRoute, StormPeak approach,
  CovenantInstallation
- Weather: WEATHER_THUNDERSTORM

### Aetheron_CovenantInstallation / Interior
- The Covenant's power extraction facility
- Angular architecture — wrong for this island
- Gym 3 (Commander Voss) is inside
- Examine control panels: readouts showing
  energy extraction from Stormveil's seal —
  numbers, dates, yield projections
  (mirrors the yield map from Gildhaven but
  more detailed, more damning)
- After Gym 3: Cass defection scene fires here
  or just outside — see Scripts section
- Connects to AetherVillage and StormPeak
- Weather: WEATHER_NONE (interior)

### Aetheron_StormPeak
- The highest point — directly below the storm
- Stormveil's seal is here
- Lightning strikes around the player constantly
  (visual only, no damage)
- The Covenant's extraction apparatus is here —
  largest and most sophisticated seen yet
- After Cass defection: Cass walks alongside
  player on this map
- Connects CovenantInstallation to SealChamber
- Weather: WEATHER_THUNDERSTORM

### Aetheron_SealChamber
- Stormveil's seal — eye of the storm
- Here the storm is completely silent
- Stormveil vast, present, electric
- Player and Cass reinforce the seal together
- The apparatus is destroyed in the process
- ITEM_STORM_COMPASS found here — it was
  part of the seal apparatus, now freed
- FLAG_AETHERON_RESOLVED set
- FLAG_CASS_DEFECTED set
- Weather: WEATHER_NONE

---

## Gym Leaders

### Gym 1 — Gale
- Location: AetherVillage
- Type: Flying
- Badge: Storm Badge (narrative-only)
- Level range: 55-59
- Party:
  - Togekiss Lv.55
  - Skarmory Lv.56
  - Talonflame Lv.57
  - Corviknight Lv.59
- Pre-battle:
  "You came up the Stream. That alone tells me
   something about you.
   The Covenant came up the Stream too.
   The difference between you matters.
   Show me the difference."
- Post-battle:
  "There it is. The Installation is east.
   You'll need to go through it to reach the peak.
   Commander Voss won't step aside.
   But there's someone up there who might."
- Gives: Storm Badge, TM for Air Slash

### Gym 2 — Arc
- Location: AetherVillage (Covenant defector)
- Type: Electric
- Badge: Volt Badge (narrative-only)
- Level range: 57-61
- Party:
  - Jolteon Lv.57
  - Magnezone Lv.58
  - Vikavolt Lv.59
  - Xurkitree Lv.61
- Pre-battle:
  "I built the installation. I know every wire,
   every extraction point, every yield number.
   I also know what it's doing to Stormveil.
   I stayed because someone should stay and know.
   Beat me. Then I'll tell you everything
   you need to know to shut it down."
- Post-battle:
  "The main extraction core is on the peak.
   The apparatus has a manual override —
   it'll destroy itself if triggered from inside.
   Voss knows this. He'll stop you.
   [hands player a schematic]
   In case you need it."
- Gives: Volt Badge, TM for Thunderbolt

### Gym 3 — Commander Voss
- Location: CovenantInstallation_Interior
- Type: Electric/Steel (military precision)
- Badge: Command Badge (narrative-only)
- Level range: 59-64
- Party:
  - Magnezone Lv.59
  - Aegislash Lv.60
  - Metagross Lv.61
  - Zekrom Lv.64
    (Covenant captured legendary — not THE
     legendary, a different one; use as
     placeholder if Zekrom not available,
     substitute Electivire Lv.64)
- Pre-battle:
  "Warden's child. I've read the file.
   You've been very busy.
   Eight islands. Eight seals reinforced.
   Do you understand what you're undoing?
   The Covenant has kept this region stable
   for forty years by managing the seal network.
   You're dismantling forty years of work
   because a dead man left you a journal.
   I can't let you proceed."
- Post-battle:
  "...You're stronger than the file suggested.
   [stands, straightens uniform]
   I've done my duty. What happens next
   is above my authority.
   [looks toward the door]
   Though I think you'll find the path forward
   has already been decided for you."
  [Cass is in the doorway]
- Gives: Command Badge, TM for Thunder

---

## The Cass Defection Scene

Most important scene on Aetheron. Write carefully.

After Commander Voss's post-battle dialogue,
Cass appears in the doorway of the installation.

Cass has been on Covenant orders the entire time.
Cass was supposed to stop the player here.

Cass looks at Voss. Then at the player.
A long pause. No music (stop music).

Cass: "Commander."
Voss: "Agent Vell. Stop them."
Cass: [doesn't move]

Another pause.

Cass: "I found documents on Ashenveil.
 The evacuation records.
 The real ones.
 847 people. 400 boat capacity.
 401 survivors recorded.
 [pause]
 I've been carrying them since Gildhaven.
 I kept thinking there would be an explanation.
 [pause]
 There isn't one."

Voss: "Agent Vell—"
Cass: "I'm done."

Cass steps fully into the room.
Faces the player.

Cass: "[Player name].
 I'm sorry it took me this long.
 [pause]
 Can I come with you?"

Player: [any response — yes/okay/of course]
(No choice menu — the answer is always yes)

Cass: "Okay.
 [beat]
 Let's go."

Voss leaves — doesn't fight again, doesn't
dramatically exit. Just turns and walks out.
Leaves them to it.

Music resumes.

FLAG_CASS_DEFECTED set
VAR_CASS_RELATIONSHIP set to 3 (maximum)
regardless of previous value

After this scene: Cass follows as an NPC on
StormPeak and SealChamber maps. Not a follower
Pokémon — a second overworld character walking
alongside. She doesn't speak again until after
the seal is reinforced.

---

## New Flags Required (STORY BLOCK 4)

```c
FLAG_AETHERON_ARRIVED
FLAG_AETHERON_GYM1_CLEAR        // Gale defeated
FLAG_AETHERON_GYM2_CLEAR        // Arc defeated
FLAG_AETHERON_GYM3_CLEAR        // Voss defeated
FLAG_AETHERON_CASS_SEEN         // First Cass sighting
FLAG_AETHERON_INSTALLATION_FOUND // Installation discovered
FLAG_AETHERON_SEAL_FOUND        // SealChamber entered
FLAG_AETHERON_RESOLVED          // Seal reinforced
FLAG_CASS_DEFECTED              // Cass defection scene played
FLAG_STORM_COMPASS_OBTAINED     // Item received
```

---

## New Variables Required

Confirm VAR_AETHERON_PROGRESS exists (0x4108).
Confirm VAR_CASS_RELATIONSHIP exists.
No new vars needed. 3 spares preserved.

VAR_AETHERON_PROGRESS:
0=not arrived, 1=arrived, 2=Gym1, 3=Gym2,
4=Gym3+Cass defection, 5=seal reinforced,
6=resolved

---

## New Items Required

```c
ITEM_STORM_COMPASS   // Replaces Fly
                     // "An ancient compass that
                     //  resonates with island
                     //  harbors. Allows instant
                     //  travel between visited
                     //  ports."
                     // Opens island selection menu
                     // when used from bag

ITEM_SEAL_SHARD_AETHERON  // Electric/Flying stub
                           // "Crystallized storm
                           //  energy from Stormveil's
                           //  seal. Crackles faintly."

ITEM_CASS_DOCUMENTS  // The Ashenveil real documents
                     // "A copy of the Covenant's
                     //  internal memo. Cass kept
                     //  this since Gildhaven."
                     // Examine for lore text only
```

---

## Wild Pokémon Encounters

### Aetheron_SkyRoute
```
Common (40%):   Emolga
Common (30%):   Swablu
Uncommon (20%): Togetic
Rare (10%):     Jolteon
```

### Aetheron_StormPeak
```
Common (40%):   Electabuzz
Common (30%):   Manectric
Uncommon (20%): Vikavolt
Rare (10%):     Xurkitree
```

No encounters: CloudLanding, both inns,
AetherVillage, CovenantInstallation, SealChamber

---

## Trainer Data

### TRAINER_GUARDIAN_AETHERON_1
- Name: "Guardian Sael"
- Location: SkyRoute
- Party: Togetic Lv.53, Skarmory Lv.54,
         Talonflame Lv.56
- Pre: "The sky tests everyone who walks it.
  You're being tested now."
- Post: "You pass. Continue."

### TRAINER_GUARDIAN_AETHERON_2
- Name: "Guardian Renn"
- Location: SkyRoute
- Party: Emolga Lv.54, Manectric Lv.55,
         Vikavolt Lv.57
- Pre: "We've kept this island hidden for
  generations. The Covenant found it anyway.
  At least you came up the honest way."
- Post: "The village is ahead. Gale is waiting."

### TRAINER_COVENANT_AETHERON_1
- Name: "Covenant Officer Mael"
- Location: CovenantInstallation
- Party: Magneton Lv.55, Klang Lv.56,
         Magnezone Lv.58
- Pre: "Installation is restricted.
  Covenant personnel only."
- Post: "...Breach logged."

### TRAINER_COVENANT_AETHERON_2
- Name: "Covenant Officer Sera"
- Location: CovenantInstallation_Interior
- Party: Aegislash Lv.57, Metagross Lv.58,
         Magnezone Lv.59
- Pre: "Commander Voss is expecting you.
  He's not pleased."
- Post: "He'll be less pleased when I report this."

---

## NPC Dialogue Guidelines

**CloudLanding dockhand:**
"You came up the Stream. First outsiders in
 three years. The last ones were Covenant.
 [looks at player carefully]
 You're not Covenant. Come in."

**CloudLanding community member:**
"We can see the whole world from here.
 The islands. The sea. The Covenant ships.
 We chose not to be part of it.
 [pause]
 The Covenant didn't ask us before they came."

**Inn innkeeper:**
"The storm never stops. You stop noticing
 after a while. It becomes like breathing.
 The silence would be worse now, I think."

**Inn scholar:**
"The Covenant came two years ago. Said they
 needed access to the peak for research.
 We said no. They came anyway.
 That's when Arc left the village.
 He said he wanted to watch from the inside."

**AetherVillage child:**
"What's the ground like? Is it soft?
 Dad says it's brown. I thought brown was
 just a color but he says it's a texture too.
 I want to see brown."

**AetherVillage Covenant defector (Arc, pre-battle):**
"I was a technician. Installation team.
 I helped run the cables, calibrate the extractors.
 I knew what we were doing to Stormveil.
 I told myself the yield numbers justified it.
 [pause]
 The yield numbers are very large.
 The justification got harder every month."

**CovenantInstallation control panel (examine):**
"STORMVEIL EXTRACTION ARRAY — YIELD LOG
 Month 1: 847 terawatt-hours
 Month 6: 1,204 TWh (seal degradation: 12%)
 Month 12: 1,891 TWh (seal degradation: 31%)
 Month 18: 2,847 TWh (seal degradation: 53%)
 Month 24: [REDACTED] (seal degradation: 71%)
 PROJECTED SEAL FAILURE: [REDACTED]
 NOTE: Yield increase corresponds directly
 to seal degradation rate. Extraction is
 accelerating. Do not distribute externally."
FLAG_AETHERON_INSTALLATION_FOUND set

**Post-defection Cass (StormPeak, walking alongside):**
[No dialogue until after seal reinforcement]
[If examined/talked to: "Let's just get it done."]

**Post-resolution Cass (after seal reinforced):**
"Is this what it felt like? All the others?
 [player: yes/nods/something]
 I've been on the other side of this for two years.
 Watching the yield numbers go up.
 Telling myself it was stable management.
 [pause]
 What do we do now?"
Player: "Convergence."
Cass: "Right.
 [beat]
 Together then."

**Post-resolution community members:**

Dockhand:
"The storm changed this morning.
 Still here. But different.
 Lighter, somehow.
 Is that what you did up there?"

AetherVillage child:
"Dad says the crackling stopped for a minute.
 Just one minute.
 He says that's never happened before.
 I was asleep. I missed it.
 That seems unfair."

---

## Key Scripts

### Knock Up Stream Sequence
- Triggered when player uses ITEM_SEA_CHART
  at sea (from boat menu or harbor)
- Scripted cutscene: screen effects, rapid
  ascent, Cass's Covenant vessel visible
  briefly alongside (no interaction)
- Fades to white → arrives at CloudLanding
- FLAG_AETHERON_ARRIVED set
- VAR_AETHERON_PROGRESS = 1
- This replaces the normal sail sequence —
  Aetheron can only be reached this way

### First Cass Sighting
- Triggered when player enters AetherVillage
  for the first time
- Cass visible across the village square —
  Covenant uniform, sees player, looks away
  deliberately, walks into an alley
- No dialogue — just a sighting
- FLAG_AETHERON_CASS_SEEN set
- One-time only

### CovenantInstallation Discovery
- Player examines control panel
- Extended readout text (yield log above)
- FLAG_AETHERON_INSTALLATION_FOUND set

### Cass Defection Scene
Full implementation in "The Cass Defection Scene"
section above.
- lockall
- Stop music (silence is critical)
- Extended choreography — pauses, positioning
- Music resumes after "Let's go."
- FLAG_CASS_DEFECTED set
- VAR_CASS_RELATIONSHIP = 3

### Seal Reinforcement (with Cass)
- Player and Cass approach seal together
- Cass NPC walks alongside on StormPeak
- SealChamber: both characters present
- Player interacts with seal
- Stormveil responds — the storm above calms
  slightly (weather effect change)
- Extraction apparatus destroys itself
- ITEM_STORM_COMPASS found in the wreckage
- Brief scene: player and Cass stand in the
  sudden stillness
- Cass's post-resolution lines (above)
- FLAG_AETHERON_SEAL_FOUND set
- FLAG_AETHERON_RESOLVED set
- ITEM_SEAL_SHARD_AETHERON given

### Island Resolution
- PokéNav call to Sollis:
  "Aetheron readings stabilized. Stormveil's
   seal is holding. And — is that Cass?
   [pause]
   Good. I'm glad she's with you.
   Come home soon. Both of you."
- Storm Compass field effect now active
  (fast travel between all visited island
   harbors — replaces Fly)
- Convergence appears in boat menu

### Storm Compass Field Effect
- When used from bag:
  Opens island selection menu showing all
  visited islands (all harbors unlocked)
  Player selects destination → instant travel
  Same menu as Tennyson boat but from the air
  Works on any map (field use)

---

## Battle Terrain Setup

All Aetheron outdoor maps:
```json
"weather": "WEATHER_THUNDERSTORM"
```

With ELECTRIC_TERRAIN battle effect.

CovenantInstallation: WEATHER_NONE
SealChamber: WEATHER_NONE (then WEATHER_NONE
after seal — the storm doesn't disappear,
just changes character)

Battle backgrounds:
- Sky/outdoor: sky battle background
- Installation: building/tech background
- SealChamber: special electric background

---

## Task Checklist

### pelagios-systems-engineer
- [ ] Aetheron flags in flags.h (BLOCK 4)
- [ ] ITEM_STORM_COMPASS, ITEM_SEAL_SHARD_AETHERON,
      ITEM_CASS_DOCUMENTS
- [ ] 4 trainers + 3 gym leaders (Gale, Arc, Voss)
- [ ] Map group stub (11 maps)
- [ ] Convergence stub in boat menu
      (activates after FLAG_AETHERON_RESOLVED)
- [ ] Storm Compass field effect stub in items.h
- [ ] Compile clean

### pelagios-map-builder
- [ ] All 11 Aetheron maps
- [ ] KnockUpStream — scripted only, no warps back
- [ ] CloudLanding cloud-pier aesthetic
- [ ] SkyRoute — open sky both sides
- [ ] CovenantInstallation — angular, wrong aesthetic
- [ ] StormPeak — Cass NPC placement stub
- [ ] SealChamber — two-character scene space
- [ ] Wild encounters (SkyRoute, StormPeak)
- [ ] Heal location (CloudLanding Inn)
- [ ] WEATHER_THUNDERSTORM for all outdoor maps
- [ ] Covenant control panel as bg_event
- [ ] Compile after every 3 maps

### pelagios-script-writer
- [ ] Knock Up Stream ascent sequence
- [ ] Arrival + CloudLanding atmosphere
- [ ] First Cass sighting (AetherVillage)
- [ ] All gym leader scripts (Gale, Arc, Voss)
- [ ] CovenantInstallation panel examination
- [ ] CASS DEFECTION SCENE — full implementation
      Stop music / long pauses / choreography /
      FLAG_CASS_DEFECTED / VAR_CASS_RELATIONSHIP=3
- [ ] StormPeak with Cass NPC walking alongside
- [ ] Seal reinforcement (two-character scene)
- [ ] Post-resolution Cass dialogue
- [ ] Island resolution + Storm Compass unlock
- [ ] Storm Compass field effect
- [ ] All NPC dialogue per guidelines
- [ ] All trainer dialogue
- [ ] Post-resolution NPC variants
- [ ] Add Gale, Arc, Voss, Cass (Aetheron variant)
      to pelagios_speaker_names.inc

### pelagios-build-debugger
- [ ] Full compile clean
- [ ] Verify Cass defection flags set correctly
- [ ] Verify VAR_CASS_RELATIONSHIP = 3
- [ ] Verify Storm Compass field effect works
- [ ] Verify Convergence in boat menu after resolved
- [ ] Verify KnockUpStream sequence fires correctly
- [ ] Update CLAUDE.md

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
AETHERON_BRIEF.md. Ashenveil is complete and
compiling cleanly. Implement all Aetheron constants
— flags from STORY BLOCK 4, items including
ITEM_STORM_COMPASS (replaces Fly, field use opens
island selection menu), ITEM_SEAL_SHARD_AETHERON,
and ITEM_CASS_DOCUMENTS (lore item), trainer entries
including three gym leaders (Gale, Arc, Voss) plus
four generic trainers, map group stub (11 maps), and
Convergence stub in boat menu that activates after
FLAG_AETHERON_RESOLVED. Confirm VAR_AETHERON_PROGRESS
exists (0x4108) and VAR_CASS_RELATIONSHIP exists —
reuse both, do not add new vars. Compile and fix
errors. Do not build maps or scripts.
```

---

*After Aetheron: Convergence (Final Island).
The end of the game.*
