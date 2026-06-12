# EMBERVEIL — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle complete and compiling cleanly ✅
- Ironhold complete and compiling cleanly ✅
- VAR_BOAT_TIER = 2 (Brigantine) must be set ✅
- VAR_EMBERVEIL_PROGRESS exists in vars.h ✅
- Capacity refactor complete ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Emberveil |
| Theme | Volcanic island — dangerous beauty, fire cult, willing sacrifice |
| Primary types | Fire / Ground |
| Battle terrain | Harsh Sun, ash weather effect, volcanic battle backgrounds |
| Legendary | Pyrath (Fire/Ground) — sealed inside the volcano's core |
| Boat required | Brigantine |
| Galleon unlocked | After BOTH FLAG_SIROCCO_RESOLVED and FLAG_EMBERVEIL_RESOLVED are set |
| Parallel island | Sirocco Isle (can be done in either order) |

---

## Narrative Summary

Emberveil is a volcanic island of stunning, dangerous beauty. Its people
are fierce and independent — they've survived on an active volcano for
generations and are proud of it.

A cult called **The Ember Circle** has formed around the volcano, led by
**High Ember Solace** — a genuinely kind, deeply devout elder who has led
her community for thirty years. The cult has been performing rituals that
they believe will usher in a paradise through Pyrath's release. They don't
understand that the rituals are weakening the seal — or that Pyrath's
release means the volcano erupts and kills everyone.

**This is the morally complex island.** Solace is not a villain. Her followers
love her. The player must convince her she's wrong without destroying her
or her community.

**Two resolution paths exist — the player's choice matters:**

**Path A (Default — only defeating Solace):**
- Player defeats Solace, reinforces seal by force
- Cult fragments — followers lose faith, community fractures
- Solace survives but broken
- She appears on Final Island and dies in Covenant crossfire
- This is the "efficient but costly" resolution

**Path B (True resolution — finding the commune ritual):**
- Player finds the Warden's hidden notes in the caldera ruins
- Notes describe a way to let people safely commune with
  a sealed legendary without weakening the seal
- Player presents this to Solace before the final battle
- Solace agrees to try — the ritual works
- Community stays intact, Solace understands what she's
  been maintaining all along
- She sends a letter post-game — she's okay
- This is the "right" resolution, rewarded but not forced

**Key story beats:**
1. Player arrives at Ashmouth — volcanic port, fire everywhere
2. Cult presence is visible but not oppressive — they're
   a community, not an army
3. Defeat Gym 1 (Cinder) — young initiate with doubts
4. Climb through lava fields to Cinderhold (main city)
5. Defeat Gym 2 (Slag) — non-cult worker who loves fire Pokémon
6. Discover the caldera approach — feel the seal's instability
7. Defeat Gym 3 (Vex) — cult enforcer protecting caldera
8. Optional: find Warden's notes in caldera ruins (Path B unlock)
9. Confront High Ember Solace at the volcano summit
10. Battle Solace — Path A or Path B resolution
11. Reinforce Pyrath's seal
12. Warden's Journal cipher 4 found
13. FLAG_EMBERVEIL_RESOLVED set

---

## Map Group

```
MAP_GROUP_EMBERVEIL:
  - Emberveil_AshmouthPort
  - Emberveil_AshmouthPort_Inn
  - Emberveil_AshmouthPort_Inn_Interior
  - Emberveil_LavaRoute1
  - Emberveil_AshFields
  - Emberveil_CinderholdCity
  - Emberveil_CinderholdCity_PokemonCenter
  - Emberveil_CinderholdCity_TempleHall
  - Emberveil_CinderholdCity_TempleHall_Interior
  - Emberveil_LavaRoute2
  - Emberveil_CalderaApproach
  - Emberveil_CalderaRuins
  - Emberveil_VolcanoAscent
  - Emberveil_VolcanoSummit
  - Emberveil_SealChamber
```

---

## Map Descriptions

### Emberveil_AshmouthPort
- Entry point from sea via Brigantine
- Small port — everything is slightly singed, ash falls like snow
- Warm orange light from the volcano visible at all times
- Cult presence visible but welcoming — they greet visitors
  as potential converts, not threats
- Key NPCs:
  - Cult greeter ("Welcome to Emberveil. Pyrath's warmth
    be with you." — earnest, not threatening)
  - Sailor who talks about the volcano's recent activity:
    "She's been rumbling more than usual. The cult says
     it's Pyrath stirring. I say it's a problem."
  - Merchant selling fire-resistant items (Rawst Berries,
    Lum Berries, Charcoal)
- Inn available
- Connects north to LavaRoute1
- Tileset: port but volcanic, dark stone, ember glow effects

### Emberveil_AshmouthPort_Inn / Interior
- Innkeeper NPC — cult member but hospitable
- Traveler NPC who came to see the volcano:
  "It's beautiful. Terrifying. Both at once. I understand
   why people worship it."
- Another traveler who is less comfortable:
  "The ash is getting thicker. I'm leaving tomorrow."

### Emberveil_LavaRoute1
- Route north from Ashmouth through lava fields
- Harsh Sun weather
- Ash particles in the air (visual effect)
- Dramatic — lava flows visible but not traversable yet
  (no Lava Boots yet — those come from this island)
- Two trainer encounters (young cult initiates)
- Berry trees: 2x Rawst Berry
- Connects south to AshmouthPort, north to AshFields
- Tileset: volcanic route, dark rock, lava channels

### Emberveil_AshFields
- Transitional area — open ash plains
- The devastation here is beautiful — grey ash, orange sky
- Dead trees preserved in ash (examine for lore — they
  show this area was forest once, before an eruption)
- One trainer encounter (Gym 1 gatekeeper — cult initiate)
- Wild Pokémon encounters
- Connects south to LavaRoute1, north to CinderholdCity
- Connects east to CalderaApproach (blocked until Gym 2)
- Tileset: ash plains, grey, orange sky

### Emberveil_CinderholdCity
- Main city — built into the caldera's outer slope
- Architecturally impressive — the cult built this city
  over generations, and it shows
- Fire everywhere but controlled — lanterns, hearths,
  torch-lit streets
- Key buildings:
  - PokéCenter
  - Temple Hall (the cult's main gathering space — also Gym 2)
  - Various houses with cult-member NPCs
- Key NPCs:
  - Cinder (post-Gym1, here in the city)
  - Slag (near the Temple Hall exterior)
  - Various cult members with varying levels of devotion
  - One quietly doubting NPC: "I believe in Pyrath.
    I'm just not sure about the timing."
  - Elder cult historian (optional lore about the cult's
    founding 200 years ago)
- Gym 1 is at the city's south entrance (converted gatehouse)
- Gym 2 (Slag) is in a lava works building at city's east
- Connects south to AshFields
- Connects east to LavaRoute2 (to CalderaApproach, blocked until Gym 2)
- Tileset: volcanic city, dark stone, orange lanterns

### Emberveil_CinderholdCity_TempleHall / Interior
- The cult's main ceremonial space
- Grand interior — fire-lit, impressive scale
- Used for optional lore scenes, not a gym
- Examine the central flame: vision flash — brief image of
  Pyrath being sealed, cult founders witnessing it,
  them beginning the rituals to maintain what they thought
  was communion (actually seal maintenance)
- High Ember Solace is here initially before the player
  advances far enough for her to move to the summit

### Emberveil_LavaRoute2
- Route east of Cinderhold toward the caldera
- Harder terrain — narrow paths, lava flows close to the route
- Three trainer encounters (cult enforcers — they're
  protecting the caldera approach, not malicious)
- Connects west to CinderholdCity, east to CalderaApproach

### Emberveil_CalderaApproach
- The caldera's outer ring — dramatic, oppressive heat
- The seal instability is physically noticeable here —
  wild Pokémon are agitated, unusual species present
- Gym 3 (Vex) is here — a cult enforcer stationed to
  prevent unauthorized caldera entry
- After defeating Vex: CalderaRuins accessible
- Connects west to LavaRoute2
- Connects into CalderaRuins

### Emberveil_CalderaRuins
- Ancient stone structures built into the caldera wall
- Pre-cult construction — the civilization that sealed
  Pyrath built these as monitoring stations
- Examine various ruins for cross-island lore
- **PATH B UNLOCK:** Hidden alcove contains the Warden's
  field notes (FLAG_WARDEN_NOTES_EMBERVEIL)
  — notes describe the commune ritual
  — requires examining three specific ruin panels in order
  — if found before confronting Solace: Path B becomes available
- Connects south to CalderaApproach
- Connects up to VolcanoAscent

### Emberveil_VolcanoAscent
- The volcano's upper slope — dramatic final approach
- Weather intensifies here: Harsh Sun + ash storm
- Two trainer battles (High Ember Solace's closest disciples)
- Connects down to CalderaRuins, up to VolcanoSummit

### Emberveil_VolcanoSummit
- The summit — Pyrath's seal is here, visible as a massive
  glowing formation in the rock
- High Ember Solace is here, performing the ritual
- The ritual is clearly having an effect — the seal pulses
  visibly with each cycle
- Pre-battle scene plays before the player can approach
  (see Scripts section)
- Gym 4 location (High Ember Solace)
- Path A or Path B resolution determined here
- Connects down to VolcanoAscent
- Connects into SealChamber (after resolution)

### Emberveil_SealChamber
- Beneath the summit — Pyrath's actual seal
- Pyrath visible: massive, fire-type legendary, currently
  weakened and restless but not free
- After Solace scene resolves: player reinforces the seal
- FLAG_EMBERVEIL_RESOLVED set here
- Warden's Journal cipher 4 found here

---

## Geography & Connections

```
[Sea / Brigantine docked]
        ↓
Emberveil_AshmouthPort
        ↓
Emberveil_LavaRoute1
        ↓
Emberveil_AshFields
        ↓
Emberveil_CinderholdCity
  ├── Gym 1 (Cinder) — south entrance
  ├── Gym 2 (Slag) — east lava works
  └── TempleHall (lore, Solace early location)
        ↓ east [blocked until Gym 2]
Emberveil_LavaRoute2
        ↓
Emberveil_CalderaApproach (Gym 3 — Vex)
        ↓
Emberveil_CalderaRuins [Path B notes hidden here]
        ↓
Emberveil_VolcanoAscent
        ↓
Emberveil_VolcanoSummit (Gym 4 — Solace)
        ↓
Emberveil_SealChamber
```

---

## Gym Leaders

### Gym 1 — Cinder
- Location: Converted gatehouse, south entrance of CinderholdCity
- Type specialist: Fire
- Badge: Ember Badge (Badge 1 of Emberveil, Badge 6 overall
  if Sirocco done first / Badge 10 if after Sirocco)
- NOTE: Badge numbering depends on player order.
  Use a generic badge slot — the game tracks gym clears
  by flag, not badge number order.
- Level range: 22-26
- Party:
  - Arcanine Lv.22
  - Rapidash Lv.24
  - Ninetales Lv.26
- Gym puzzle: light braziers in correct sequence to open
  the path — wrong sequence resets and spawns a trainer
- Pre-battle dialogue:
  "I became an initiate because the fire felt true.
   Pyrath's warmth, the High Ember's teachings — I believed.
   I still believe. I just — have questions lately.
   The volcano's been louder. The High Ember says that's
   a blessing. It doesn't feel like a blessing.
   Beat me. I need to think."
- Post-battle dialogue:
  "When you find the High Ember — and you will —
   ask her what happens to the island if Pyrath wakes.
   Actually ask her. See what she says."
- Gives: Ember Badge, TM for Flamethrower

### Gym 2 — Slag
- Location: Lava works building, east CinderholdCity
- Type specialist: Fire/Ground mix
- Badge: Forge Badge (Badge 2 of Emberveil)
- Level range: 28-32
- Party:
  - Magcargo Lv.28
  - Camerupt Lv.29
  - Torkoal Lv.30
  - Heatran Lv.32
- Gym puzzle: redirect lava flows by operating valves —
  clear a path through the lava works facility
- Pre-battle dialogue:
  "I'm not in the cult. Never was.
   I just work the lava fields — best fuel source
   on any island I've been to.
   I'll battle you because you look like you need
   the practice for whatever's up that mountain.
   Don't take it personally."
- Post-battle dialogue:
  "Here. Lava Boots. Military salvage, came through
   the port last month. You'll need them on the caldera.
   And — the High Ember is a good person.
   Good people can be wrong about things that matter."
- Gives: Forge Badge, TM for Earth Power, ITEM_LAVA_BOOTS
- POST-BATTLE: ITEM_LAVA_BOOTS given here
  FLAG_LAVA_BOOTS_OBTAINED set

### Gym 3 — Vex
- Location: Emberveil_CalderaApproach
- Type: Fire/Flying mix (fast, aggressive team)
- Badge: Caldera Badge (Badge 3 of Emberveil)
- Level range: 33-37
- Party:
  - Talonflame Lv.33
  - Darmanitan Lv.34
  - Salazzle Lv.35
  - Charizard Lv.37
- Pre-battle dialogue:
  "The caldera is sacred. The High Ember's instructions
   are clear: no outsiders past this point.
   I follow the High Ember."
- Post-battle dialogue:
  "...Go. I won't stop you.
   But if you hurt her — if you hurt this community —
   I will find you."
- Gives: Caldera Badge, TM for Brave Bird

### Gym 4 — High Ember Solace
- Location: Emberveil_VolcanoSummit
- Type: Fire (pure, elegant team — she's been doing this
  for thirty years and every choice is deliberate)
- Badge: Pyrath Badge (Badge 4 of Emberveil)
- Level range: 40-45
- Party:
  - Volcarona Lv.40
  - Chandelure Lv.41
  - Heatran Lv.42
  - Magmortar Lv.43
  - Arcanine Lv.45
- **PRE-BATTLE SCENE (Path A — no Warden's notes found):**
  Solace is performing the ritual. Doesn't stop when
  player arrives.
  "You've come to stop the ritual. I know.
   The Warden's child — yes, I know who you are.
   Your parent visited this summit years ago.
   They stood where you're standing.
   They didn't stop me either.
   They sat with me for an hour and then left.
   I've wondered about that for six years.
   I won't stop the ritual. Pyrath's awakening will
   purify this island. I have seen it.
   If you want to stop me, you'll have to battle me."
- **PRE-BATTLE SCENE (Path B — Warden's notes found):**
  Player shows Solace the notes before the ritual completes.
  Solace stops. Reads them. Long pause.
  "These are your parent's notes.
   A way to commune with Pyrath without...
   without weakening the seal.
   (pause)
   I've been performing the ritual for thirty years.
   My mother performed it before me. Her mother before her.
   We thought we were calling to Pyrath.
   We were — we were maintaining a seal.
   (long pause)
   Show me. Show me how the commune ritual works.
   If it's real — if your parent found a real path —
   I want to walk it. Not the one that kills my people."
  PATH B: No battle. Solace performs the commune ritual
  with the player. The seal stabilizes. Pyrath responds —
  a moment of genuine connection, neither imprisoned nor
  free, simply acknowledged.
- **POST-BATTLE DIALOGUE (Path A):**
  Solace accepts defeat with complete serenity.
  "You're stronger. That's clear.
   Do what you came to do.
   (pause)
   Was I wrong? About all of it?"
  Player choice:
  - "Yes. The volcano would have erupted." →
    Solace closes her eyes. Says nothing more.
    (FLAG_SOLACE_TOLD_TRUTH set)
  - "About the method. Not about Pyrath." →
    Solace opens her eyes. Something shifts.
    "Then perhaps there's more to understand."
    (FLAG_SOLACE_ALT_ENDING set — partial, not full Path B)
- **POST-BATTLE DIALOGUE (Path B — after commune ritual):**
  "Thirty years. My mother's lifetime. Her mother's lifetime.
   All of it — not wrong. Just incomplete.
   We were maintaining something sacred.
   We just didn't know what.
   (pause, looking at the seal)
   Pyrath heard us. All those years, Pyrath heard us.
   That's enough. That's more than enough."
- Gives: Pyrath Badge, TM for Fire Blast

---

## Key Characters

### High Ember Solace
- Sprite: Use LADY placeholder until custom sprite ready
- Overworld: starts in TempleHall_Interior, moves to
  VolcanoSummit after FLAG_EMBERVEIL_GYM2_CLEAR
- Tone: serene, genuinely kind, absolutely convinced
- She is NOT a villain — write her with full dignity
- Her faith is real and her community loves her
- Post-resolution (Path A): she stays on Emberveil,
  fragmented, her community fractured
  She appears on Final Island and dies there
- Post-resolution (Path B): she stays on Emberveil,
  her community intact, now understanding their true role
  She sends a post-game letter — she is okay
  She does NOT appear on Final Island

### Cinder (Gym 1)
- Sprite: Use YOUNGSTER placeholder
- The doubter — his doubt should feel real, not weak
- He's not leaving the cult, he's questioning within it
- Post-resolution (Path A): his doubt deepens to crisis
- Post-resolution (Path B): his questions get answers,
  he becomes the next community historian

### Vex (Gym 3)
- Sprite: Use COOLTRAINER_M placeholder
- Post-resolution: stays at CalderaApproach
- Path A: watching the community fracture in silence
- Path B: first person Solace tells about the commune ritual
  His reaction is pure relief

### Slag (Gym 2)
- Sprite: Use WORKER placeholder
- Not cult, never was, doesn't change regardless of path
- Gives ITEM_LAVA_BOOTS post-battle always

---

## New Flags Required

Add to include/constants/flags.h:

```c
// Emberveil progression
FLAG_EMBERVEIL_ARRIVED
FLAG_EMBERVEIL_GYM1_CLEAR         // Cinder defeated
FLAG_EMBERVEIL_GYM2_CLEAR         // Slag defeated
FLAG_EMBERVEIL_GYM3_CLEAR         // Vex defeated
FLAG_EMBERVEIL_GYM4_CLEAR         // Solace defeated/convinced
FLAG_LAVA_BOOTS_OBTAINED          // ITEM_LAVA_BOOTS received
FLAG_WARDEN_NOTES_EMBERVEIL       // Hidden notes found in CalderaRuins
FLAG_EMBERVEIL_PATH_B             // True resolution achieved
FLAG_EMBERVEIL_SEAL_FOUND         // Player entered SealChamber
FLAG_EMBERVEIL_RESOLVED           // Seal reinforced, island complete
FLAG_EMBERVEIL_CIPHER_FOUND       // Warden's Journal cipher 4 collected
FLAG_SOLACE_TOLD_TRUTH            // Player told Solace the truth (Path A)
FLAG_SOLACE_ALT_ENDING            // Player's middle-ground response (Path A partial)
FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY1 // Rawst Berry Route 1
FLAG_HIDDEN_ITEM_EMBERVEIL_BERRY2 // Rawst Berry Route 1 (second)
```

---

## New Variables Required

Confirm VAR_EMBERVEIL_PROGRESS exists (capacity refactor).

VAR_EMBERVEIL_PROGRESS states:
- 0 = not arrived
- 1 = arrived at Ashmouth
- 2 = Gym 1 cleared (Cinder)
- 3 = Gym 2 cleared (Slag) + Lava Boots obtained
- 4 = Gym 3 cleared (Vex)
- 5 = Solace confronted (Path A or B)
- 6 = Seal reinforced
- 7 = Resolved

---

## New Items Required

Add to include/constants/items.h and src/data/items.h:

| Constant | Name | Description | Type |
|---|---|---|---|
| ITEM_LAVA_BOOTS | Lava Boots | "Fireproof boots. Walk across ash fields and lava rock." | Key Item |
| ITEM_WARDEN_NOTES | Warden's Field Notes | "Your parent's handwritten research notes. A diagram shows a ritual — 'communion without breaking'." | Key Item |
| ITEM_SEAL_SHARD_EMBERVEIL | Emberveil Seal Shard | "A fragment of crystallized legendary energy from Emberveil's volcano. Infernape resonates with it." | Key Item — Infernape Mega Evolution trigger — stub until Mega system built |

---

## Wild Pokémon Encounters

### Emberveil_LavaRoute1 (Rocky/ash patches)
```
Common (40%):   Slugma
Common (30%):   Numel
Uncommon (20%): Houndour
Rare (10%):     Magby
```

### Emberveil_AshFields (Ash patches)
```
Common (40%):   Numel
Common (30%):   Houndour
Uncommon (20%): Torkoal
Rare (10%):     Litwick
```

### Emberveil_LavaRoute2 (Rocky patches)
```
Common (40%):   Camerupt (Numel)
Common (30%):   Houndoom (Houndour)
Uncommon (20%): Turtonator
Rare (10%):     Larvesta
```

### Emberveil_CalderaApproach (Volcanic rock)
```
Common (40%):   Camerupt
Common (30%):   Turtonator
Uncommon (20%): Magmar
Rare (10%):     Volcarona (Larvesta)
```

### Emberveil_CalderaRuins (Ash/rock)
```
Common (50%):   Litwick
Uncommon (30%): Chandelure (Lampent)
Rare (20%):     Heatran (rare encounter, low level — 
                 it's drawn to the seal)
```

No wild encounters in:
- AshmouthPort
- CinderholdCity
- TempleHall
- VolcanoAscent
- VolcanoSummit
- SealChamber

---

## Trainer Data

### TRAINER_CULTMEMBER_EMBERVEIL_1
- Name: "Initiate Sera"
- Location: Emberveil_LavaRoute1
- Party: Slugma Lv.20, Houndour Lv.21
- Pre-battle: "Pyrath's warmth protects the faithful!
  Let me show you what devotion looks like."
- Post-battle: "...Devotion also means accepting defeat
  with grace. Pyrath be with you."

### TRAINER_CULTMEMBER_EMBERVEIL_2
- Name: "Initiate Bren"
- Location: Emberveil_LavaRoute1
- Party: Numel Lv.20, Slugma Lv.21, Torkoal Lv.22
- Pre-battle: "The High Ember says the awakening is near.
  I want to be ready when it comes."
- Post-battle: "I'll be readier next time."

### TRAINER_CULTMEMBER_EMBERVEIL_3
- Name: "Devotee Mira" (different from Miria on Sirocco)
- Location: Emberveil_AshFields
- Party: Houndoom Lv.23, Numel Lv.23, Camerupt Lv.25
- Pre-battle: "You're heading toward the city. The High Ember
  will want to know about visitors."
- Post-battle: "She'll know about you anyway. She always does."

### TRAINER_CULTMEMBER_EMBERVEIL_4
- Name: "Acolyte Rem"
- Location: Emberveil_LavaRoute2
- Party: Darmanitan Lv.27, Talonflame Lv.28, Salazzle Lv.29
- Pre-battle: "The caldera is the High Ember's domain.
  Outsiders don't belong there."
- Post-battle: "You belong there about as much as anyone."

### TRAINER_CULTMEMBER_EMBERVEIL_5
- Name: "Acolyte Voss"
- Location: Emberveil_LavaRoute2
- Party: Charizard Lv.29, Heatran Lv.31
- Pre-battle: "I've trained here my whole life.
  The heat doesn't touch me anymore."
- Post-battle: "You run hotter than I expected."

### TRAINER_CULTMEMBER_EMBERVEIL_6
- Name: "Acolyte Tane"
- Location: Emberveil_LavaRoute2
- Party: Magmortar Lv.30, Turtonator Lv.31, Volcarona Lv.33
- Pre-battle: "Three of us guard this route.
  You won't get through all three."
- Post-battle: "Apparently you will."

### TRAINER_CULTMEMBER_EMBERVEIL_7
- Name: "Elder Disciple Horne"
- Location: Emberveil_VolcanoAscent
- Party: Volcarona Lv.36, Heatran Lv.37, Chandelure Lv.38
- Pre-battle: "I've served the High Ember for forty years.
  I won't let you interrupt the culmination of her work."
- Post-battle: "...Go. If you can beat me,
  perhaps Pyrath has a reason for your being here."

### TRAINER_CULTMEMBER_EMBERVEIL_8
- Name: "Elder Disciple Cael"
- Location: Emberveil_VolcanoAscent
- Party: Arcanine Lv.37, Charizard Lv.38, Volcarona Lv.39
- Pre-battle: "The summit is sacred. Turn back."
- Post-battle: "I misjudged you. Go."

---

## NPC Dialogue Guidelines

**Ashmouth cult greeter:**
"Welcome to Emberveil. Pyrath's warmth be with you.
 The High Ember's teachings have sustained this island
 for two centuries. We hope you'll feel that warmth
 during your visit."
(Sincere, not creepy — this is a functioning community)

**Ashmouth sailor:**
"She's been rumbling more than usual. The cult says
 it's Pyrath stirring. I say it's a problem.
 I've been docked here three weeks and the ash is
 twice as thick as when I arrived."

**Ashmouth merchant:**
"Fire-resistance supplies. Best stock on any island —
 we need them. Rawst Berries grow wild here.
 Charcoal's a local product — Emberveil export.
 We do good business as long as the mountain
 stays where it is."

**Inn traveler (beautiful):**
"It's beautiful. Terrifying. Both at once. I understand
 why people worship it. I don't understand why they'd
 want to wake it up."

**Inn traveler (leaving):**
"The ash is getting thicker. I'm leaving tomorrow.
 The cult says that's Pyrath's breath. I prefer
 my lungs less blessed."

**CinderholdCity doubting NPC:**
"I believe in Pyrath. I'm just not sure about the timing.
 The High Ember says the awakening is imminent.
 Imminent has meant different things over the years.
 This time it feels — different."

**CinderholdCity cult historian:**
"The Ember Circle was founded two hundred years ago
 by the survivors of a smaller eruption. They believed
 Pyrath had spared them for a purpose — to prepare
 for Pyrath's full awakening.
 Every High Ember since has refined the ritual.
 Solace has brought it further than anyone."

**TempleHall central flame (examine):**
[Vision: ancient figures sealing Pyrath, the first
ritual being performed, generations passing, each
generation believing they're calling to their god
when they're actually maintaining a seal]
"The flame has burned here for two centuries.
 It has never gone out."

**CalderaRuins panel 1 (examine):**
"[ancient script — same as Haven Isle and Ironhold]"
(If player has decoded enough ciphers, partial translation:
"...the seal requires maintenance through resonance...
 the ritual of communion sustains without breaking...")

**CalderaRuins panel 2 (examine):**
"[ancient script]"
(Partial: "...those who tend the flame tend the seal...
 the Warden need not be present if the community...")

**CalderaRuins panel 3 (examine — PATH B TRIGGER):**
"[Warden's field notes tucked into a stone alcove]"
"These are in your parent's handwriting.
 Recent — within the last few years.
 A diagram, carefully drawn. A ritual. Notes in the margin:
 'Communion without breaking. The cult is doing it wrong
 but they're doing something. This is the correction.'"
FLAG_WARDEN_NOTES_EMBERVEIL set
ITEM_WARDEN_NOTES given to player

**Post-resolution NPCs (Path A):**

Cinder: "She's still here. She hasn't left the summit.
  I don't know what to say to her.
  I had questions and now the questions don't matter
  because the thing I was questioning is gone."

Vex: (says nothing — just watches the community)

CinderholdCity doubting NPC: "I'm leaving. Not because
  I stopped believing. Because I don't know what I
  believe anymore and I need somewhere quiet to find out."

**Post-resolution NPCs (Path B):**

Cinder: "She told me this morning. About the seal.
  About what the ritual actually does.
  I cried. I'm not ashamed of that.
  We've been keeping something alive for two centuries
  without knowing it. That's not less sacred.
  That's more."

Vex: "I was ready to fight you to the end.
  Then Solace came down from the summit and explained.
  And I understood why your parent sat with her
  instead of stopping her.
  They were waiting for the notes to be found."

---

## Key Scripts

### Arrival Script
- Player docks at Ashmouth
- Cult greeter approaches — friendly, not blocking
- Camera pans up to show volcano in background
- Ash particle effect begins (subtle throughout island)
- FLAG_EMBERVEIL_ARRIVED set
- VAR_EMBERVEIL_PROGRESS = 1

### Lava Boots Handoff (Slag post-battle)
- After Slag battle ends
- Slag dialogue plays (see Gym 2 section)
- ITEM_LAVA_BOOTS given to player
- FLAG_LAVA_BOOTS_OBTAINED set
- LavaRoute2 lava obstacles now passable
- CalderaApproach ash fields now traversable

### Lava Boots Field Effect
- When player approaches lava obstacle tiles and presses A:
  Check for ITEM_LAVA_BOOTS
  - Present: brief boot animation, path opens
  - Absent: "The ground is too hot to cross safely."
- Applies to: CalderaApproach ash fields,
  CalderaRuins narrow paths, VolcanoAscent lava channels

### Warden's Notes Discovery (Path B Unlock)
- Triggered by examining CalderaRuins panel 3
  (only reachable after examining panels 1 and 2 first —
   use a local step counter, not flags)
- Screen dims briefly
- Player finds notes tucked in alcove
- Dialogue plays (see CalderaRuins panel 3 above)
- ITEM_WARDEN_NOTES added to bag
- FLAG_WARDEN_NOTES_EMBERVEIL set

### Solace Pre-Battle Scene (Path A)
- Triggered on entering VolcanoSummit
  without FLAG_WARDEN_NOTES_EMBERVEIL
- Solace performing ritual — camera shows the seal
  pulsing in rhythm with the ritual
- Solace dialogue (see Gym 4 section — Path A version)
- Battle begins

### Solace Pre-Battle Scene (Path B)
- Triggered on entering VolcanoSummit
  WITH FLAG_WARDEN_NOTES_EMBERVEIL
- Solace performing ritual
- Player uses ITEM_WARDEN_NOTES from bag
  (or: prompted automatically if in bag — "Use the notes?")
- Solace stops ritual to read
- Extended dialogue scene (see Gym 4 section — Path B version)
- Commune ritual sequence:
  Screen goes warm amber
  Pyrath's form visible through the seal — massive, aware
  A moment of stillness — Pyrath acknowledges without breaking
  Screen returns to normal
  The seal stabilizes visibly
- No battle — FLAG_EMBERVEIL_GYM4_CLEAR set without battle
- FLAG_EMBERVEIL_PATH_B set

### Island Resolution Script
- Triggered after seal reinforcement in SealChamber
- Brief cutscene: volcano visible from outside rumbles
  then quiets — the instability resolving
- Player contacts Solace via PokéNav (or speaks to her
  if still present at summit — Path B she stays):
  Path A Solace: (quiet) "It's done then."
  Path B Solace: "The island is breathing normally again.
   I can feel it. Pyrath is... calm."
- Player contacts Maren Sollis via PokéNav:
  "Emberveil readings just stabilized. The seal is holding.
   What happened up there?"
  Player: [describes resolution]
  Maren: "The commune ritual. Your parent found that notation
   years ago. I didn't know if it would work."
  (Another hint that Maren knows more than she says)
- FLAG_EMBERVEIL_RESOLVED set
- Check if FLAG_SIROCCO_RESOLVED is also set:
  - If yes: VAR_BOAT_TIER = 3, Galleon unlocked,
    refit dialogue plays
  - If no: dialogue acknowledges Emberveil complete but
    notes Sirocco still needs attention

### Warden's Journal Cipher 4
- Triggered in SealChamber after resolution
- Journal entry unlocks:
  "The commune ritual works. I tested it here — Pyrath
   acknowledged me. Not freedom. Not imprisonment.
   Something in between that I don't have a word for yet.
   The cult has been sustaining the seal for two centuries
   without knowing it. If someone finds my notes in time,
   that community survives. If not — I hope the player
   who comes after me is kind about how they do what
   has to be done. [encoded] ...Dorne called again.
   He found Emberveil. He's moving faster than I thought."

---

## Battle Terrain Setup

All outdoor Emberveil maps:
```json
"weather": "WEATHER_SUNNY"
```

AshFields and CalderaApproach:
```json
"weather": "WEATHER_SUNNY"
```
With ash particle sprite overlay (coord event)

VolcanoAscent and VolcanoSummit:
```json
"weather": "WEATHER_DROUGHT"
```

SealChamber:
```json
"weather": "WEATHER_NONE"
```

Battle backgrounds:
- Outdoor routes: volcanic/grass battle background
- CalderaApproach/Ruins: cave battle background
- VolcanoSummit: special fire battle background
- SealChamber: cave battle background

---

## Emberveil — Task Checklist

### pelagios-systems-engineer (first) — DONE 2026-06-12
- [x] Add all Emberveil flags to include/constants/flags.h
  (story block 2 opened at 0x493-0x4A6; Emberveil claims 0x493-0x49C + ARRIVED 0x4EF;
   hidden-item berries 0x268/0x269; RESOLVED 0x4AE / SOLACE_ALT_ENDING 0x4B9 reused)
- [x] Confirm VAR_EMBERVEIL_PROGRESS exists (capacity refactor) — 0x4102, no new vars
- [x] Add ITEM_LAVA_BOOTS to items.h and src/data/items.h — already existed (877)
- [x] Add ITEM_WARDEN_NOTES to items.h and src/data/items.h — 886
- [x] Add ITEM_SEAL_SHARD_EMBERVEIL stub to items.h — 887
- [x] Add all trainer entries to trainers.party
  (TRAINER_CULTMEMBER_EMBERVEIL_1 through _8 = 888-895 + 4 gym leaders 896-899)
- [x] Add gym leader trainer entries:
  - Cinder 896 (Pic Youngster)
  - Slag 897 (Pic Hiker — no Worker pic in expansion)
  - Vex 898 (Pic Cooltrainer M)
  - Solace 899 (Pic Lady)
- [~] Add Emberveil map group stub to map_groups.json — SKIPPED: empty map groups
  cannot be expressed in the generated groups.inc. The map-builder registers
  MAP_GROUP_EMBERVEIL when it creates the island's first real map (Emberveil_AshmouthPort).
- [x] Compile and fix errors — gmake exit 0 (EWRAM 86.45%, ROM 79.12%)

### pelagios-map-builder (second) — DONE 2026-06-12 (gmake exit 0)
- [x] Emberveil_AshmouthPort
- [x] Emberveil_AshmouthPort_Inn + Interior
- [x] Emberveil_LavaRoute1
- [x] Emberveil_AshFields
- [x] Emberveil_CinderholdCity
- [x] Emberveil_CinderholdCity_PokemonCenter
- [x] Emberveil_CinderholdCity_TempleHall + Interior
- [x] Emberveil_LavaRoute2
- [x] Emberveil_CalderaApproach
- [x] Emberveil_CalderaRuins
- [x] Emberveil_VolcanoAscent
- [x] Emberveil_VolcanoSummit
- [x] Emberveil_SealChamber
- [x] Wild encounter tables (all 5 outdoor areas)
- [x] Heal location (AshmouthPort Inn)
- [x] Volcanic outdoor tileset = General/Lavaridge (Mt Chimney sampled); SealChamber +
     TempleHall interior reuse vanilla Cave/Sealed-Chamber layouts. NOTE: caldera areas
     use the same General/Lavaridge volcanic look (not a separate Cave tileset) for
     visual continuity; SealChamber is the only Cave-tileset map.
- [x] Lava obstacle tiles on LavaRoute2 and CalderaApproach (impassable lava walls leaving
     gated corridors; coord triggers compare VAR_EMBERVEIL_PROGRESS - scripter wires the
     ITEM_LAVA_BOOTS field check / clear)
- [x] Compile clean (full build exit 0; also fixed a pre-existing Sirocco MUS_ROUTE111
     link blocker to let the tree link)

### pelagios-script-writer (third) — DONE 2026-06-12
(gmake exit 0; full implementation record in CLAUDE.md "Completed — Emberveil SCRIPTS")
- [x] Arrival script + volcano camera pan (ON_FRAME at progress 0, not coord triggers)
- [x] Lava Boots handoff (Slag post-battle, key item BEFORE the TM)
- [x] Lava Boots field effect on obstacle tiles (script-side coord triggers,
      progress 0-2 armed, checkitem defensive branch - no C code needed)
- [x] Warden's Notes discovery (panel sequence via VAR_TEMP_1 local step counter)
- [x] Solace pre-battle scene Path A
- [x] Solace pre-battle scene Path B (commune ritual sequence; decline retreats + re-arms)
- [x] Solace post-battle dialogue + player choice (Path A: TOLD_TRUTH / ALT_ENDING)
- [x] Path B resolution (no battle variant; FLAG_EMBERVEIL_PATH_B)
- [x] SealChamber resolution script (discovery progress 5->6, apparatus 6->7)
- [x] Island resolution + conditional Galleon check (mirrors Sirocco's; tier never lowered)
- [x] Warden's Journal cipher 4 unlock (FLAG_CIPHER_4_FOUND + FLAG_EMBERVEIL_CIPHER_FOUND)
- [x] All gym leader pre/post battle dialogue (badges narrative-only - engine badge
      flags exhausted; TM subs lampshaded: Earthquake for Earth Power, Aerial Ace
      for Brave Bird)
- [x] All NPC dialogue per guidelines above
- [x] All trainer pre/post battle dialogue (cult members 1-8, sight-initiated)
- [x] Path A post-resolution NPC dialogue variants
- [x] Path B post-resolution NPC dialogue variants
- [x] TempleHall central flame vision trigger (repeatable)
- [x] CalderaRuins panel examine scripts (3 panels, translations gated on cipher 2)

### pelagios-build-debugger (last)
- [ ] Full compile passes with zero errors
- [ ] Verify Lava Boots field effect fires correctly
- [ ] Verify Path B triggers only with FLAG_WARDEN_NOTES_EMBERVEIL
- [ ] Verify Path A and Path B set correct flags
- [ ] Verify FLAG_EMBERVEIL_RESOLVED triggers conditional
      Galleon check correctly
- [ ] Verify cipher 4 flag sets correctly
- [ ] Verify commune ritual scene plays correctly (Path B)
- [ ] Update CLAUDE.md — mark Emberveil complete

---

## Prompt to Start

Give this to pelagios-systems-engineer:

```
Read CLAUDE.md and EMBERVEIL_BRIEF.md. Ironhold is complete
and compiling cleanly. Your task: implement all Emberveil
constants — flags, variables, items including ITEM_LAVA_BOOTS
and ITEM_WARDEN_NOTES, trainer entries including four gym
leaders (Cinder, Slag, Vex, Solace), and map group
registration — following the task checklist in EMBERVEIL_BRIEF.md
under "pelagios-systems-engineer". Compile after completing
constants and fix any errors. Do not build maps or scripts.
Note: Sirocco Isle systems engineer may be running in parallel
— check current trainer IDs in trainers.party before assigning
new IDs to avoid collisions.
```

---

*This brief covers Emberveil only.
Build Sirocco in parallel using SIROCCO_BRIEF.md.
Both must be resolved before Galleon unlocks.*
*Check trainer ID collisions if both systems engineers
run simultaneously.*
