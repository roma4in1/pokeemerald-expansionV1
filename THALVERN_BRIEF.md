# THALVERN — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle through Schism complete ✅
- VAR_BOAT_TIER = 3 (Galleon) ✅
- VAR_THALVERN_PROGRESS exists in vars.h ✅
- STORY BLOCK 4 opened (0x26C–0x2BB) ✅
- FLAG_DEX_ALIVE concept established in story bible ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Thalvern (The Sunken Kingdom) |
| Theme | Ancient ruins slowly drowning — archaeology, psychic visions, deep sea |
| Primary types | Water / Psychic |
| Battle terrain | Rain, fog, underwater battle backgrounds |
| Legendary | Pelagios (Water/Psychic) — the ancient civilization's guardian deity |
| Boat required | Galleon |
| Parallel islands | Schism Isle, Gildhaven (any order) |
| Key death | Dex the archaeologist dies here — major emotional beat |

---

## Narrative Summary

Thalvern is a partially submerged island — the ancient civilization's
most significant site. Its ruins are sinking faster every year as
Pelagios's seal weakens. The legendary is broadcasting psychic memories
of the ancient civilization to anyone sensitive enough to receive them,
and some locals have been driven half-mad by the visions.

Two factions are racing to find the Throne Room (deepest point, legendary
site):
- **The Covenant's Archaeological Division** led by Director Numa Vess,
  who wants to capture Pelagios for the Covenant
- **Independent scholars** led by Dex, following the same leads as the
  player, piecing together the ancient civilization's true history

**The critical choice: does Dex reach the Throne Room first?**

**If player goes ahead of Dex (FLAG_DEX_ALIVE = TRUE):**
- Player experiences the full psychic vision instead of Dex
- Player is incapacitated for three in-game days (scripted rest)
- Dex stays at player's side during recovery
- Dex survives Thalvern
- His presence on later islands has more emotional weight
- His death on Ashenveil (future island) hits harder

**If player lets Dex go first (FLAG_DEX_ALIVE = FALSE):**
- Dex reaches the Throne Room, experiences full vision
- Dex never wakes up — found unconscious, decoded notes in hand
- His decoded notes become the player's research tool
- His sister is found on Dead Island (future island) — player can
  tell her or not
- This is the default path if player doesn't intervene

**The choice is presented before the Throne Room:**
An NPC (The Lens — questioning Covenant junior archaeologist) warns
the player that whoever goes first will bear the full vision's weight.
Player chooses: "I'll go" or "Let Dex go ahead."

**Key story beats:**
1. Arrive at Tidespire floating market — immediately feels different,
   psychic energy visible
2. NPCs with vision symptoms — some locals receiving fragments
3. Defeat Gym 1 (Tide) — local fisherman
4. Find Dex's research camp — he's been here for weeks
5. Navigate partially submerged ruins
6. Defeat Gym 2 (Psalm) — scholar who stayed to study the visions
7. Covenant presence intensifies — Director Numa Vess arrives
8. Defeat Gym 3 (Lens) — Covenant junior archaeologist doubting orders
9. Race/approach to Throne Room — The Lens warns about vision weight
10. CHOICE: player goes first, or Dex goes first
11. Throne Room sequence — vision of ancient civilization's final days
12. Resolution: reinforce Pelagios's seal
13. Warden's Journal cipher 6 found
14. FLAG_THALVERN_RESOLVED set

---

## Map Group

```
MAP_GROUP_THALVERN:
  - Thalvern_TidespirePort
  - Thalvern_TidespirePort_Inn
  - Thalvern_TidespirePort_Inn_Interior
  - Thalvern_FloatingMarket
  - Thalvern_CoastalRoute
  - Thalvern_DexCamp
  - Thalvern_SubmergedRuins_Exterior
  - Thalvern_SubmergedRuins_Interior1
  - Thalvern_SubmergedRuins_Interior2
  - Thalvern_SubmergedRuins_Interior3
  - Thalvern_CovenantSite
  - Thalvern_DeepApproach
  - Thalvern_ThroneRoom
```

---

## Map Descriptions

### Thalvern_TidespirePort
- Entry point from sea
- Built on stilts above the water — the island's original
  ground level is meters below
- Psychic energy visible as faint shimmer on the water
- Key NPCs:
  - Fisherman who has stopped fishing: "The fish left.
    Something down there changed. I hear things now.
    Not sounds — images. Quick ones."
  - Porter who warns about the ruins:
    "Don't go into the deep ruins without a guide.
     People get lost in the visions."
  - Merchant selling water-breathing items and
    psychic-resist berries (Persim, Lum)
- Inn available
- Connects to FloatingMarket
- Tileset: gTileset_PelagiosUnderwater + gTileset_General
- Weather: WEATHER_RAIN

### Thalvern_TidespirePort_Inn / Interior
- Innkeeper who sees visions occasionally:
  "I saw a city last night. Not this one.
   Bigger. Whole. Then the water came."
- Scholar NPC who came to study the phenomenon:
  "The visions are fragments. They're not random —
   they're Pelagios broadcasting. It's trying to
   tell us something."

### Thalvern_FloatingMarket
- The island's main settlement — a network of
  connected floating platforms and stilt buildings
- Psychic energy stronger here — closer to the ruins
- Key buildings:
  - PokéCenter
  - Gym 1 entrance (Tide's gym — a converted warehouse)
  - Gym 2 entrance (Psalm's study)
  - Dex's research camp (Thalvern_DexCamp — adjacent)
- Key NPCs:
  - Child drawing pictures of the visions they're seeing
    (disturbing but innocent)
  - Elder who has been experiencing visions for years:
    "It gets easier. You stop being frightened of
     the dead city. You start trying to understand it."
  - Numa Vess visible on a balcony (distant, watching)
    — NPC object, not interactable yet
- Connects to TidespirePort, CoastalRoute, DexCamp
- Weather: WEATHER_RAIN

### Thalvern_DexCamp
- Dex's research camp on the floating market's edge
- Equipment everywhere, notebooks stacked high
- Dex is here — excited, slightly wild-eyed:
  "You found me! Or I found you. One of those.
   I've decoded three inscription panels this week.
   The same civilization as Haven Isle and Sirocco.
   And Thalvern is their CENTER. This is where
   they were based. The seal here is the oldest.
   Pelagios predates the civilization — they built
   around it."
- If player has Warden's Journal:
  "Your parent's cipher. I've been trying to crack
   the same system for two years. Between your journal
   and my panels — I think we can decode everything.
   If we live long enough."
- Research notes on table — examine for lore
- Dex mentions The Lens: "Covenant archaeologist.
  Young. He's been slipping me information. I think
  he's having second thoughts about who he works for."
- No wild Pokémon

### Thalvern_CoastalRoute
- Route between FloatingMarket and SubmergedRuins
- Partially flooded — elevated walkways
- Two trainer encounters (Covenant researcher, scholar)
- Vision patches: step on certain tiles → brief
  flash of ancient architecture (coord events,
  purely visual, no game effect)
- Connects FloatingMarket to SubmergedRuins_Exterior
- Weather: WEATHER_RAIN + FOG

### Thalvern_SubmergedRuins_Exterior
- Ancient stone structures, water level rising
- The scale is remarkable — this was a major center
- Covenant researchers visible in background (NPCs)
- Gym 3 (Lens) is here — he's stationed outside
  ostensibly to control access
- Entrance to Interior1 visible
- Examine any exposed inscription for lore
- Connects to CoastalRoute, Interior1
- Weather: WEATHER_RAIN

### Thalvern_SubmergedRuins_Interior1
- First chamber — partially flooded floor
- Water Pokémon visible in the flooded sections
- Gym 2 (Psalm) is here — she's been studying a
  specific inscription for weeks
- Gym puzzle: navigate between raised stone platforms
  over flooded sections, some platforms submerge
  periodically (use tile animations)
- Connects to Exterior, Interior2

### Thalvern_SubmergedRuins_Interior2
- Deeper chamber — more intact
- Covenant presence: researchers taking notes,
  one blocking a side passage
- Ancient murals showing Pelagios — vast, serene,
  clearly the civilization's central figure
- Examine central mural: extended vision flash —
  clearest vision yet, showing the civilization
  at its height, then its final days, then the
  sealing of Pelagios
- The Lens is here (if Gym 3 already defeated) —
  he's defecting quietly, gives player a keycard
  to bypass the Covenant blockade
- Connects to Interior1, Interior3

### Thalvern_SubmergedRuins_Interior3
- Pre-Throne Room chamber
- The Lens meets player here (if not met earlier)
- WARNING SCENE: The Lens tells player about the
  vision weight — this is the choice point
  "Whoever goes in first takes the full vision.
   Dex is right behind you.
   One of you goes first. That person —
   I don't know what it does to them."
- CHOICE: "I'll go in." / "Let Dex go ahead."
- Connects to Interior2, ThroneRoom

### Thalvern_ThroneRoom
- Deepest chamber — Pelagios's seal
- Pelagios barely visible — enormous, ancient, aware
- The psychic energy is overwhelming here
- **PATH A (Player goes first):**
  Screen goes white — extended vision sequence
  (see Scripts section)
  Player collapses — three-day recovery scripted
  Dex finds player, stays with them
  FLAG_DEX_ALIVE = TRUE
- **PATH B (Dex goes first):**
  Dex enters ahead
  A moment of silence — then Dex collapses
  Player finds him — decoded notes in hand
  He never wakes up
  FLAG_DEX_ALIVE stays FALSE
- After resolution: Pelagios's seal reinforced
- Cipher 6 found in the ancient inscriptions
- No wild Pokémon

### Thalvern_CovenantSite
- Covenant's temporary base of operations
- Director Numa Vess is here (not a gym leader —
  a story antagonist who flees rather than fights)
- Covenant researchers
- Covenant documents visible — examine for lore
  about the Covenant's true plans for Pelagios
- Optional confrontation: player can face Numa Vess
  She refuses to battle: "I don't dirty my hands.
  I file reports. I commission studies. I wait."
  Then leaves — not defeated, just uninterested
- Accessible after Gym 3 (Lens gives access)
- Connects to DeepApproach

### Thalvern_DeepApproach
- Final route to Throne Room
- Covenant blockade (researchers as trainers)
- Connects CovenantSite to Interior3

---

## Gym Leaders

### Gym 1 — Tide
- Location: Converted warehouse, FloatingMarket
- Type specialist: Water
- Badge: Tide Badge (narrative-only)
- Level range: 44-48
- Party:
  - Gyarados Lv.44
  - Clawitzer Lv.45
  - Wishiwashi (School) Lv.46
  - Milotic Lv.48
- Gym puzzle: navigate waterlogged warehouse floor —
  currents push player in set directions
- Pre-battle dialogue:
  "I was a fisherman for thirty years.
   Then the fish left and the visions started.
   Now I guard this market because someone has to.
   You want to go into the ruins.
   Everyone wants to go into the ruins.
   Show me you can handle what's in there."
- Post-battle dialogue:
  "The visions get louder the deeper you go.
   Find the scholar — Psalm. She's been down
   there longer than anyone and she's still sane.
   Mostly."
- Gives: Tide Badge, TM for Surf equivalent

### Gym 2 — Psalm
- Location: SubmergedRuins_Interior1
- Type specialist: Psychic
- Badge: Vision Badge (narrative-only)
- Level range: 47-51
- Party:
  - Gardevoir Lv.47
  - Reuniclus Lv.48
  - Orbeetle Lv.49
  - Slowking Lv.51
- Pre-battle dialogue:
  "I came to study the visions academically.
   Then I started having them myself.
   Now I'm not sure where the study ends
   and the experience begins.
   Battle me. It helps me stay present."
- Post-battle dialogue:
  "The Throne Room. That's what you want.
   The visions there are complete — not fragments.
   I went once. Came back changed.
   Take someone you trust.
   Don't go alone."
- Gives: Vision Badge, TM for Psychic

### Gym 3 — The Lens
- Location: SubmergedRuins_Exterior
- Type: Water/Psychic mix
- Badge: Depth Badge (narrative-only)
- Level range: 49-53
- Party:
  - Slowbro Lv.49
  - Lanturn Lv.50
  - Malamar Lv.51
  - Starmie Lv.53
- Pre-battle dialogue:
  "I'm stationed here to control access.
   Covenant orders. I've been controlling access
   for three weeks and in that time I've read
   every inscription Dex decoded.
   I want you to get to the Throne Room.
   Beat me so I can say I tried to stop you."
- Post-battle dialogue:
  "The Covenant wants Pelagios as a weapon.
   I've seen the containment specs.
   Here — my access card. Get to the Throne Room.
   Reinforce the seal before Numa Vess does."
  (Gives Covenant access card — key item for
   bypassing CovenantSite blockade)
  (The Lens defects after this — joins Dex's camp)
- Gives: Depth Badge, TM for Scald

---

## Key Characters

### Dex (Archaeologist) — MAJOR CHARACTER
- Sprite: Use SCIENTIST_M placeholder
- This is Dex's final island — he dies here
  (or survives if player goes first)
- He should feel warm, brilliant, alive
- Do NOT foreshadow his death in his dialogue
- He's optimistic and excited — the worst time to die
- If FLAG_DEX_ALIVE = TRUE: he appears on later islands
  continuing the research
- If FLAG_DEX_ALIVE = FALSE: his decoded notes are the
  player's most important research tool going forward
- His death (Path B) should be quiet, not dramatic —
  he's simply not there when the player looks for him

### Director Numa Vess
- Sprite: Use COOLTRAINER_F placeholder
- Bureaucratic villain — files reports, commissions
  studies, never gets her hands dirty
- She's genuinely interested in history and willing
  to destroy it for institutional benefit
- She does NOT battle the player — she's not a gym leader
- She leaves before the player can confront her meaningfully
- Post-resolution: she files a report about the failed
  containment. The Covenant notes it. She continues.

### The Lens
- Sprite: Use COOLTRAINER_M placeholder
- Gym 3 and defector
- He's been slipping Dex information for weeks
- Post-Thalvern: joins Dex's research camp
  (if Dex alive) or continues Dex's work alone
  (if Dex died)
- Appears on Beast Island (Primalis) following leads

---

## New Flags Required

Draw from STORY BLOCK 4 (0x26C–0x2BB):

```c
FLAG_THALVERN_ARRIVED
FLAG_THALVERN_GYM1_CLEAR        // Tide defeated
FLAG_THALVERN_GYM2_CLEAR        // Psalm defeated
FLAG_THALVERN_GYM3_CLEAR        // Lens defeated
FLAG_THALVERN_DEX_MET           // Player spoke to Dex
FLAG_THALVERN_LENS_DEFECTED     // Lens gave access card
FLAG_THALVERN_THRONE_CHOICE     // Choice scene triggered
FLAG_THALVERN_SEAL_FOUND        // ThroneRoom entered
FLAG_THALVERN_RESOLVED          // Seal reinforced
FLAG_THALVERN_CIPHER_FOUND      // Cipher 6 collected
FLAG_DEX_ALIVE                  // Player went first (TRUE = Dex survives)
FLAG_NUMA_VESS_CONFRONTED       // Optional Numa Vess scene
```

---

## New Variables Required

Confirm VAR_THALVERN_PROGRESS exists (0x4104).

VAR_THALVERN_PROGRESS states:
- 0 = not arrived
- 1 = arrived
- 2 = Gym 1 cleared (Tide)
- 3 = Gym 2 cleared (Psalm)
- 4 = Gym 3 cleared (Lens)
- 5 = Throne Room entered
- 6 = Seal reinforced
- 7 = Resolved

---

## New Items Required

```c
ITEM_COVENANT_ACCESS_CARD  // Given by Lens post-battle
                           // Bypasses CovenantSite blockade
ITEM_SEAL_SHARD_THALVERN   // Feraligatr Mega Evolution trigger
                           // Awarded after resolution
                           // This is THE Feraligatr Seal Shard
                           // from the story bible
ITEM_DEX_NOTES             // Dex's decoded notes (PATH B only)
                           // Key item, gives lore text when used
```

---

## Wild Pokémon Encounters

### Thalvern_CoastalRoute (Surfing/water patches)
```
Common (40%):   Staryu
Common (30%):   Chinchou
Uncommon (20%): Frillish
Rare (10%):     Inkay
```

### Thalvern_SubmergedRuins_Interior1 (Flooded sections)
```
Common (50%):   Slowpoke
Uncommon (30%): Beheeyem
Rare (20%):     Wynaut
```

### Thalvern_SubmergedRuins_Interior2 (Deep water)
```
Common (50%):   Frillish
Uncommon (30%): Slowbro
Rare (20%):     Claydol
```

No wild encounters in:
- Both ports, FloatingMarket, DexCamp
- Interior3, ThroneRoom
- CovenantSite, DeepApproach

---

## Trainer Data

### TRAINER_SCHOLAR_THALVERN_1
- Name: "Scholar Wren"
- Location: Thalvern_CoastalRoute
- Party: Slowpoke Lv.42, Staryu Lv.43, Starmie Lv.45
- Pre-battle: "I came here to study the visions.
  Now I'm having them. Science requires participation,
  I suppose."
- Post-battle: "The deeper you go, the stronger they get.
  Take something grounding with you."

### TRAINER_COVENANT_THALVERN_1
- Name: "Researcher Holt"
- Location: Thalvern_CoastalRoute
- Party: Slowbro Lv.43, Lanturn Lv.44, Starmie Lv.46
- Pre-battle: "Covenant Archaeological Division.
  This site is under research jurisdiction."
- Post-battle: "...Jurisdiction apparently disputed."

### TRAINER_COVENANT_THALVERN_2
- Name: "Researcher Sael"
- Location: Thalvern_DeepApproach
- Party: Malamar Lv.46, Slowking Lv.47, Beheeyem Lv.48
- Pre-battle: "Director Vess has classified this approach.
  No unauthorized personnel."
- Post-battle: "I'll note your authorization in my report."

### TRAINER_COVENANT_THALVERN_3
- Name: "Senior Researcher Cael"
- Location: Thalvern_DeepApproach
- Party: Starmie Lv.47, Reuniclus Lv.48, Slowking Lv.50
- Pre-battle: "Last checkpoint before the Throne Room.
  You'll need more than strength to handle what's inside."
- Post-battle: "Apparently strength is enough.
  Don't say I didn't warn you."

---

## NPC Dialogue Guidelines

**Tidespire fisherman (stopped fishing):**
"The fish left three months ago.
 Same time the visions started.
 I see the old city sometimes, when I'm on the water.
 Full of people. Going about their day.
 Then I blink and it's just ruins."

**Tidespire porter:**
"Don't go into the deep ruins without a guide.
 People get lost in the visions.
 Not physically — they come back.
 But something in their eyes is different after."

**FloatingMarket child drawing:**
[Child is drawing detailed architectural sketches
 of buildings they have never seen]
"I keep dreaming the same city.
 Can you draw? I can't get the towers right."

**FloatingMarket elder:**
"It gets easier. You stop being frightened of
 the dead city. You start trying to understand it.
 I've been seeing it for eleven years.
 I know their faces now. Some of them.
 I don't know their names."

**Inn innkeeper:**
"I saw a city last night. Not this one.
 Bigger. Whole. Busy.
 I woke up sad in a way I couldn't explain.
 Grief for people who died before my grandparents
 were born. Is that strange?"

**Inn scholar:**
"The visions are fragments. They're not random —
 they're Pelagios broadcasting. It's trying to
 tell us something before the ruins sink completely.
 A message in a bottle, except the bottle is
 a legendary Pokémon and the ocean is time."

**Dex (first meeting):**
"You found me! The Warden's child — yes, I know
 who you are. Your parent and I worked together
 years ago. They were better at the cipher than me.
 I've decoded three panels this week.
 Same civilization as Haven Isle, Sirocco, all of it.
 And Thalvern is their CENTER.
 Pelagios predates the civilization entirely —
 they built their entire culture around maintaining
 its seal. Two thousand years of civilization
 devoted to a single act of care.
 Remarkable, isn't it?"

**Dex (if player has Warden's Journal):**
"Is that — your parent's journal?
 The cipher. May I see it?
 I've been trying to crack the same system —
 [examines it, excited]
 This is it. This is the key.
 Together we can decode everything.
 All of it. The whole story."
 (Dex gains dialogue variants throughout island
  as he processes the journal information)

**Dex (pre-Throne Room, PATH A setup):**
"The Throne Room. I've been trying to reach it
 for weeks. The Covenant keeps blocking me.
 Now the Lens has given us access.
 I should go first — I've been preparing for this.
 But something in the Psalm's eyes when she came
 back — I don't know. I don't know."

**The Lens (pre-battle):**
"I'm stationed here to control access.
 Covenant orders. I've been controlling access
 for three weeks and in that time I've read
 every inscription Dex decoded.
 I want you to get to the Throne Room.
 Beat me so I can say I tried to stop you."

**The Lens (post-defection, at Dex's camp):**
"I worked for the Covenant for four years.
 I believed in institutional research.
 Then I read what Dex found and I understood
 that institutions don't care about what they find —
 only what they can use.
 I care what I find.
 That distinction cost me my career.
 I think it was the right trade."

**Numa Vess (optional confrontation):**
"You're the Warden's child.
 I know your file.
 I'm not going to fight you — that's not how I work.
 I file reports. I commission studies.
 I waited three years for this site to become
 accessible. I can wait longer.
 The seal will weaken eventually. Everything does."
 (She leaves — unhurried, unimpressed)

**Post-resolution NPCs:**

Tidespire fisherman:
"The visions stopped this morning. Just — stopped.
 I almost miss them.
 Is that strange? Grieving for something
 that was making you mad?"

FloatingMarket elder:
"The faces are gone. Eleven years of faces —
 gone this morning.
 I hope they know it's peaceful now."

FloatingMarket child:
"I finished the drawing.
 [Shows a complete, accurate architectural sketch
  of the ancient capital — which no one has seen]
 I don't know why I drew this.
 I don't think I'll draw it again."

---

## Key Scripts

### Arrival Script
- Player docks at Tidespire
- Psychic shimmer effect on water (palette cycle)
- Fisherman approaches immediately — first vision hint
- FLAG_THALVERN_ARRIVED set
- VAR_THALVERN_PROGRESS = 1

### Vision Patches (CoastalRoute)
- Coord events on specific tiles
- Brief screen flash + single line of ancient text
- No game effect — purely atmospheric
- Different text on each tile
- Only fires once per tile (local flag tracking)

### Dex Meeting Script
- First entry to DexCamp
- Extended Dex dialogue (enthusiasm, research update)
- Journal reaction if ITEM_WARDENS_JOURNAL in bag
- FLAG_THALVERN_DEX_MET set

### Interior2 Central Mural Vision
- Triggered by examining central mural
- Extended vision sequence — longest so far
- Shows: ancient civilization at height, Pelagios
  as central figure, the final days, the sealing
- After vision: player can move again
- "You saw it too. Everyone who touches that mural
   sees it." (Psalm, if present)

### The Choice Scene (Interior3)
- The Lens delivers warning about vision weight
- Player choice:
  "I'll go in." → FLAG_DEX_ALIVE set TRUE,
    player goes first
  "Let Dex go ahead." → Dex goes first,
    FLAG_DEX_ALIVE stays FALSE

### ThroneRoom — PATH A (Player goes first)
- Player enters ThroneRoom
- Extended vision sequence (longest in game):
  The complete memory of the ancient civilization
  Their final meeting, the decision to seal Pelagios
  Pelagios's perspective — awareness, patience, grief
  The Warden network being established
  A flash of the player's parent — young, at this
  same spot, seeing the same thing
- Screen goes dark — player collapses
- Three-day recovery:
  Scene: player in makeshift bed at DexCamp
  Dex at player's side — worried, present
  Day 1: "You've been out a day. Take your time."
  Day 2: "Psalm says the visions are normal. Ish."
  Day 3: "The Lens brought food. We're all here."
  Player wakes — brief dialogue with Dex
  "What did you see?"
  Player: [optional response, any choice]
  Dex: "I think I understand now why your parent
   sat with Solace instead of stopping her.
   They saw this too. They knew."
- FLAG_DEX_ALIVE = TRUE
- Return to ThroneRoom to reinforce seal

### ThroneRoom — PATH B (Dex goes first)
- Dex enters ahead of player
- Brief silence — a moment of nothing
- Then: Dex collapses
- Player finds him — decoded notes scattered around him
- The notes are fully decoded — everything Dex spent
  years trying to understand, completed in one moment
- Brief scene: player cannot wake him
- Psalm arrives: "This is what I was afraid of.
  He'll live. But he won't wake up. I've seen it once
  before — someone who stayed too long in the vision."
  (Psalm is wrong — Dex doesn't wake up)
- ITEM_DEX_NOTES added to player's bag
- FLAG_DEX_ALIVE stays FALSE
- Reinforce seal after scene

### Seal Reinforcement and Resolution
- Player interacts with Pelagios's seal
- Pelagios responds — brief psychic acknowledgment
  (not vision — just presence, awareness, recognition)
- The ruins stop sinking — a temporary stabilization
- PokéNav call to Solaris:
  "Thalvern readings stabilized. Pelagios's seal
   is holding. What did you find in the Throne Room?"
  Player: [describes]
  Solaris: [reacts differently based on FLAG_DEX_ALIVE]
  DEX ALIVE: "The vision. Your parent wrote about it.
   I thought they were being poetic.
   They weren't being poetic."
  DEX GONE: "Dex. I knew him. We all knew him.
   His notes — use them. That's what he'd want.
   That's all he'd want."
- FLAG_THALVERN_RESOLVED set
- ITEM_SEAL_SHARD_THALVERN given (Feraligatr Mega item)
- Check Ashenveil unlock: if Schism + Thalvern +
  Gildhaven all resolved → Ashenveil unlocks in
  boat menu

### Cipher 6 (Warden's Journal)
- Found during ThroneRoom experience (both paths)
- Journal entry unlocks:
  PATH A: "I saw it. The whole thing. I understand
   now why the Wardens before me never wrote this
   down — there aren't words for the last day.
   There are only faces.
   Pelagios remembered every one of them.
   It still does.
   [encoded] ...I saw my successor in the vision.
   Young. Determined. I didn't know it was a
   vision at the time. I thought it was a dream.
   I hope they're ready. I hope I prepared enough."
  PATH B: Same entry, same text — the decoded notes
   Dex left behind include this cipher fragment

---

## Battle Terrain Setup

All Thalvern outdoor maps:
```json
"weather": "WEATHER_RAIN"
```

CoastalRoute adds fog:
```json
"weather": "WEATHER_FOG_1"
```

SubmergedRuins interiors: WEATHER_RAIN (light)
ThroneRoom: WEATHER_NONE
CovenantSite: WEATHER_NONE

Battle backgrounds:
- Outdoor maps: rain/water background
- SubmergedRuins: underwater/cave background
- ThroneRoom: special deep-sea background

---

## Thalvern — Task Checklist

### pelagios-systems-engineer (first)
- [ ] Add all Thalvern flags to flags.h (BLOCK 4)
- [ ] Confirm VAR_THALVERN_PROGRESS exists
- [ ] Add ITEM_COVENANT_ACCESS_CARD
- [ ] Add ITEM_SEAL_SHARD_THALVERN (Feraligatr Mega)
- [ ] Add ITEM_DEX_NOTES
- [ ] Add all trainer entries + 3 gym leaders
      (Tide, Psalm, Lens)
- [ ] Add Thalvern map group stub
- [ ] Compile and fix errors

### pelagios-map-builder (second)
- [ ] Thalvern_TidespirePort + Inn + Interior
- [ ] Thalvern_FloatingMarket
- [ ] Thalvern_DexCamp
- [ ] Thalvern_CoastalRoute (vision patch tiles)
- [ ] Thalvern_SubmergedRuins_Exterior
- [ ] Thalvern_SubmergedRuins_Interior1
      (flooded platform puzzle)
- [ ] Thalvern_SubmergedRuins_Interior2
- [ ] Thalvern_SubmergedRuins_Interior3
- [ ] Thalvern_ThroneRoom
- [ ] Thalvern_CovenantSite
- [ ] Thalvern_DeepApproach
- [ ] Wild encounter tables (CoastalRoute, Interior1, Interior2)
- [ ] Heal location (TidespirePort Inn)
- [ ] Use gTileset_PelagiosUnderwater for submerged maps
- [ ] Compile after every 3-4 maps

### pelagios-script-writer (third)
- [ ] Arrival script + psychic shimmer effect
- [ ] Vision patch coord events (CoastalRoute)
- [ ] Dex meeting script (with journal reaction variant)
- [ ] Interior2 mural vision sequence
- [ ] The choice scene (Interior3)
- [ ] ThroneRoom PATH A (player first, recovery sequence)
- [ ] ThroneRoom PATH B (Dex first, death scene)
- [ ] Seal reinforcement + Pelagios acknowledgment
- [ ] Resolution script + Ashenveil check + Seal Shard
- [ ] Cipher 6 (both path variants)
- [ ] Numa Vess optional confrontation
- [ ] All gym leader dialogue (Tide, Psalm, Lens)
- [ ] Lens defection + access card handoff
- [ ] All NPC dialogue per guidelines
- [ ] All trainer dialogue
- [ ] Post-resolution NPC variants (both Dex paths)
- [ ] Dex multi-stage dialogue across island
- [ ] Add Tide, Psalm, Lens, Dex, Numa Vess to
      pelagios_speaker_names.inc

### pelagios-build-debugger (last)
- [ ] Full compile clean
- [ ] Verify FLAG_DEX_ALIVE sets correctly on PATH A
- [ ] Verify PATH B Dex scene + ITEM_DEX_NOTES
- [ ] Verify ITEM_SEAL_SHARD_THALVERN awarded
- [ ] Verify Ashenveil unlock check fires correctly
- [ ] Verify cipher 6 sets correctly
- [ ] Update CLAUDE.md

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
THALVERN_BRIEF.md. Schism is complete and compiling
cleanly. Implement all Thalvern constants — flags
from STORY BLOCK 4, items including ITEM_SEAL_SHARD_THALVERN
(the Feraligatr Mega Evolution trigger), ITEM_COVENANT_ACCESS_CARD,
and ITEM_DEX_NOTES, trainer entries including three gym leaders
(Tide, Psalm, Lens), and map group stub — following the
checklist in THALVERN_BRIEF.md. Compile and fix errors.
Do not build maps or scripts.
```

---

*This brief covers Thalvern only.
Build Gildhaven in parallel using GILDHAVEN_BRIEF.md.
Both plus Schism must resolve before Ashenveil unlocks.*
