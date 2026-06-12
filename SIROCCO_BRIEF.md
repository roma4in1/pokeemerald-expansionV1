# SIROCCO ISLE — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle complete and compiling cleanly ✅
- Ironhold complete and compiling cleanly ✅
- VAR_BOAT_TIER = 2 (Brigantine) must be set ✅
- VAR_SIROCCO_PROGRESS exists in vars.h ✅
- Capacity refactor complete (trainer slots 1024, var range extended) ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Sirocco Isle |
| Theme | Desert noir — lawless, hot, crime syndicate controls water |
| Primary types | Ground / Rock |
| Battle terrain | Sandstorm weather, Harsh Sun, desert battle backgrounds |
| Legendary | Xerath (Ground/Rock) — sealed beneath the Buried City |
| Boat required | Brigantine |
| Galleon unlocked | After BOTH FLAG_SIROCCO_RESOLVED and FLAG_EMBERVEIL_RESOLVED are set |
| Parallel island | Emberveil (can be done in either order) |

---

## Narrative Summary

Sirocco Isle is a desert island controlled by **The Gilt Claw** — a crime
syndicate that has monopolized the island's water supply. Their leader,
**Dagan Mire**, has discovered that the legendary Pokémon Xerath sealed
beneath the ancient Buried City is the island's true water source. He is
deliberately weakening the seal to accelerate the drought, planning to
control all remaining water when the oasis runs dry.

The player arrives to find a desperate population, an encroaching desert,
and a crime lord who is refreshingly honest about his motives.

**Key story beats:**
1. Player arrives at Dustmouth — dusty shantytown, clearly desperate
2. Player learns the oasis is shrinking, the Gilt Claw controls water
3. Defeat Gym 1 (Silt) — local guide turned outlaw
4. Discover the Buried City through archaeologist Dex's camp
5. Defeat Gym 2 (Crag) — former archaeologist funding digs via crime
6. Enter the Buried City — first sight of Xerath's seal weakening
7. Defeat Gym 3 (Miria) — Dagan's lieutenant, efficient and cold
8. Confront Dagan at his palace — defeat him
9. Dagan escapes — explicitly escapes, this is intentional
10. Reinforce Xerath's seal — oasis stabilizes
11. Warden's Journal cipher 3 found in Buried City ruins
12. FLAG_SIROCCO_RESOLVED set — contributes to Galleon unlock

**Dagan escapes** — he reappears on Gildhaven later. Do NOT set any
"Dagan defeated permanently" flag. He escapes cleanly after the battle.

---

## Map Group

```
MAP_GROUP_SIROCCO:
  - Sirocco_DustmouthPort
  - Sirocco_DustmouthPort_Inn
  - Sirocco_DustmouthPort_Inn_Interior
  - Sirocco_DustmouthPort_BlackMarket
  - Sirocco_DesertRoute1
  - Sirocco_MiradenOasis
  - Sirocco_MiradenOasis_PokemonCenter
  - Sirocco_MiradenOasis_Shop
  - Sirocco_DexCamp
  - Sirocco_DesertRoute2
  - Sirocco_BuriedCity_Exterior
  - Sirocco_BuriedCity_Interior1
  - Sirocco_BuriedCity_Interior2
  - Sirocco_BuriedCity_SealChamber
  - Sirocco_GiltClawTerritory
  - Sirocco_DaganPalace_Exterior
  - Sirocco_DaganPalace_Interior1
  - Sirocco_DaganPalace_Interior2
```

---

## Map Descriptions

### Sirocco_DustmouthPort
- Entry point from sea via Brigantine
- Ramshackle port town — wooden scaffolding, cracked stone, dust everywhere
- Gilt Claw presence immediately visible — branded water barrels,
  enforcers watching the docks
- Key NPCs:
  - Dock enforcer (Gilt Claw grunt, doesn't block but makes
    the player feel watched — menacing dialogue)
  - Desperate local who explains the water situation quietly
  - Merchant selling supplies at inflated prices
    ("Gilt Claw tariff — not my idea")
  - Sailor who mentions Dex's camp to the east
- Inn available (Ironhold_DustmouthPort_Inn)
- Black market accessible via hidden door in inn basement
  (Sirocco_DustmouthPort_BlackMarket — optional lore area)
- Gilt Claw water distribution point visible — NPCs queuing
- Sign at north exit: "Miraden Oasis — 2 days walk. Gilt Claw
  Water Route — permit required."
- Connects north to Sirocco_DesertRoute1
- Tileset: port but run-down, sandy, desert-adjacent

### Sirocco_DustmouthPort_Inn / Interior
- Innkeeper NPC — exhausted, clearly paying Gilt Claw protection
- Two traveler NPCs with rumors:
  - "The oasis used to be twice this size ten years ago"
  - "There's something under the sand near the old ruins.
     Dagan's people won't let anyone dig there."

### Sirocco_DustmouthPort_BlackMarket
- Hidden basement under the inn
- Three vendor NPCs selling items at normal prices
  (contrast with overworld overpriced shops — player reward
  for exploring)
- Information broker NPC who hints at Dagan's real plan:
  "He's not just selling water. He's waiting for something.
   Like he knows something will run out."
- No wild Pokémon

### Sirocco_DesertRoute1
- Long desert route from Dustmouth north to Miraden Oasis
- Sandstorm weather active
- Harsh Sun in open areas
- Dried riverbed visible — the old water course that fed Dustmouth
- Two trainer encounters (Gilt Claw grunt, wandering traveler)
- Hidden item: Rawst Berry (dried bush)
- Connects south to DustmouthPort, north to MiradenOasis
- East branch leads to DexCamp (accessible after Gym 1)
- Tileset: desert, sandy, cracked earth, sparse dead vegetation

### Sirocco_MiradenOasis
- The main settlement — built around the shrinking oasis
- Noticeably more prosperous than Dustmouth but clearly anxious
- The oasis is smaller than the dried ring of earth around it shows
  it used to be — visual storytelling, no NPC needs to say it
- Key buildings:
  - PokéCenter
  - Shop (Gilt Claw branded, normal prices here — Dagan is
    smart enough to keep the main town functional)
  - Gym 1 entrance (Silt's gym — a converted water warehouse)
  - Gym 2 entrance (Crag's gym — an excavation site building)
  - Elder's house (optional lore)
- Key NPCs:
  - Child playing near the oasis edge — "Mum says don't go
    past the wet part. The wet part keeps getting smaller."
  - Old woman who remembers the oasis at full size
  - Gilt Claw water distributor — efficient, not cruel,
    just doing a job
  - Scholar who mentions the Buried City legends
- Connects south to DesertRoute1
- Connects east to DesertRoute2 (leads to DexCamp and BuriedCity)
- Connects north to GiltClawTerritory (blocked until Gym 2 cleared)
- Tileset: town built around water, palm trees, sandstone

### Sirocco_DexCamp
- Small outdoor camp east of DesertRoute1
- Dex's excavation camp — tarps, equipment, notebooks everywhere
- Dex NPC is here — enthusiastic, slightly chaotic energy
- First meeting with Dex:
  "Oh! A visitor who isn't Gilt Claw. Wonderful.
   You see this sand? Underneath it? A whole city.
   They sealed something down there and the Gilt Claw
   doesn't want anyone to find out what."
- Dex gives player access to BuriedCity_Exterior after
  FLAG_SIROCCO_GYM1_CLEAR is set
- Research notes on table (examine for lore about ancient
  civilization — same civilization as Haven Isle ruins)
- No wild Pokémon at camp
- Connects west to DesertRoute1

### Sirocco_DesertRoute2
- Route from MiradenOasis east toward the Buried City
- Shorter than Route 1 but harder — stronger wild Pokémon
- One trainer (Gilt Claw enforcer blocking BuriedCity access
  until Gym 1 is cleared — Dex's doing, he vouches for player)
- Sand columns visible in background — geological features
- Connects west to MiradenOasis, east to BuriedCity_Exterior

### Sirocco_BuriedCity_Exterior
- Excavation site — partially uncovered ancient stonework
- The scale is impressive — this was once a major city
- Dex's dig markers visible throughout
- Two Gilt Claw grunt trainer battles here (they're trying
  to shut down the excavation)
- Entrance to Interior1 visible — ancient stone doorway
- Examine any exposed wall for ancient script
  (same script as Haven Isle ruins and Ironhold cave — third
  cross-island connection, players who've been paying attention
  will recognize it now)
- Connects west to DesertRoute2
- Connects down into BuriedCity_Interior1

### Sirocco_BuriedCity_Interior1
- First chamber — large, partially excavated
- Sandy floor with stone tiles emerging from the sand
- Baltoy and Claydol present as wild encounters
  (they're drawn to the seal energy)
- Gym 3 location (Miria — Dagan's lieutenant)
- Gym puzzle: push ancient stone tablets into correct
  positions to open the path — matches the dungeon aesthetic
- Miria is at the far end, cataloguing the ruins for Dagan
- After defeating Miria: door to Interior2 opens

### Sirocco_BuriedCity_Interior2
- Deeper chamber — more intact than Interior1
- Ancient murals on the walls (examine for lore about
  Xerath and the ancient civilization)
- The air is drier here — the seal's effect
- No gym battle here — this is pure exploration/story
- Connects deeper to SealChamber
- Examine central mural: vision flash — brief image of
  Xerath at full power, the ancient civilization worshipping it,
  then the sealing ceremony
- Cipher fragment visible on the far wall (FLAG_CIPHER_3_FOUND
  after examining — Warden's Journal cipher 3 of 9)

### Sirocco_BuriedCity_SealChamber
- Deepest room — Xerath's seal
- Machinery visible — more primitive than Ironhold's,
  cruder, clearly Gilt Claw construction not Covenant
- Xerath barely visible — a massive form beneath the stone
- Script trigger on entry: discovery cutscene (see Scripts)
- Player interacts with Gilt Claw apparatus to disable it
- Then interacts with seal to reinforce it
- Oasis water level begins rising (described in post-resolution
  NPC dialogue, not shown directly)
- FLAG_SIROCCO_RESOLVED set here
- No wild Pokémon in SealChamber

### Sirocco_GiltClawTerritory
- Route north of Miraden — the Gilt Claw's controlled zone
- Visually distinct: branded posts, enforcer patrols
- Two trainer battles (elite Gilt Claw enforcers)
- Connects south to MiradenOasis
- Connects north to DaganPalace_Exterior
- Blocked until FLAG_SIROCCO_GYM2_CLEAR

### Sirocco_DaganPalace_Exterior
- Dagan's compound — ostentatious, clearly expensive,
  deliberately contrasted with Dustmouth's desperation
- Fountains running (he has water to spare — the point)
- Two grunt trainer battles at the gate
- Connects south to GiltClawTerritory
- Connects into DaganPalace_Interior1

### Sirocco_DaganPalace_Interior1
- Entry hall — guards, wealth on display
- Two more trainer battles
- Connects deeper to Interior2

### Sirocco_DaganPalace_Interior2
- Dagan's main hall — where the player confronts him
- Gym 4 location (Dagan Mire)
- Dagan is sitting comfortably, not alarmed by the player's arrival
- Post-battle: Dagan escapes — see Scripts section
- No wild Pokémon in palace

---

## Geography & Connections

```
[Sea / Brigantine docked]
        ↓
Sirocco_DustmouthPort
  └── BlackMarket (hidden)
        ↓
Sirocco_DesertRoute1 ──east──→ Sirocco_DexCamp
        ↓
Sirocco_MiradenOasis
  ├── Gym 1 (Silt)
  ├── Gym 2 (Crag)
  └── [North blocked until Gym 2]
        ↓ east
Sirocco_DesertRoute2
        ↓
Sirocco_BuriedCity_Exterior
        ↓
Sirocco_BuriedCity_Interior1 (Gym 3 — Miria)
        ↓
Sirocco_BuriedCity_Interior2
        ↓
Sirocco_BuriedCity_SealChamber
        ↑ (separate path)
Sirocco_GiltClawTerritory [blocked until Gym 2]
        ↓
Sirocco_DaganPalace_Exterior
        ↓
Sirocco_DaganPalace_Interior1
        ↓
Sirocco_DaganPalace_Interior2 (Gym 4 — Dagan)
```

---

## Gym Leaders

### Gym 1 — Silt
- Location: Converted water warehouse, MiradenOasis
- Type specialist: Ground
- Badge: Dust Badge (Badge 1 of Sirocco, Badge 6 overall)
- Level range: 22-26
- Party:
  - Hippowdon Lv.22
  - Excadrill Lv.24
  - Dugtrio Lv.24
  - Garchomp (Gabite) Lv.26
- Gym puzzle: sandstorm reduces visibility — navigate via
  landmark tiles, wrong path loops back to start
- Pre-battle dialogue:
  "I used to guide people across this desert for a living.
   Knew every dune, every safe path.
   Then Dagan bought the water rights and the paths
   stopped mattering. Now I work for him.
   Beat me and maybe I'll remember what I used to be."
- Post-battle dialogue:
  "...Go east. Find the archaeologist's camp.
   Don't let Dagan's people see you heading there."
- Gives: Dust Badge, TM for Earthquake

### Gym 2 — Crag
- Location: Excavation site building, MiradenOasis
- Type specialist: Rock
- Badge: Stone Badge (Badge 2 of Sirocco, Badge 7 overall)
- Level range: 27-31
- Party:
  - Gigalith Lv.27
  - Barbaracle Lv.28
  - Tyranitar Lv.29
  - Rhyperior Lv.31
- Gym puzzle: boulder pushing — boulders block excavation
  equipment, push in correct order to reach Crag
- Pre-battle dialogue:
  "I needed funding for the dig. Dagan provided it.
   I told myself I wasn't responsible for what he did
   with the information.
   Funny how that works, isn't it."
- Post-battle dialogue:
  "The north territory. Dagan's compound.
   I've given him everything he asked for.
   I don't know what's under that seal but I know
   it's not his to take."
- Gives: Stone Badge, TM for Stone Edge

### Gym 3 — Miria
- Location: Sirocco_BuriedCity_Interior1
- Type: Ground/Rock mix
- Badge: Gilt Badge (Badge 3 of Sirocco, Badge 8 overall)
- Level range: 33-37
- Party:
  - Garchomp Lv.33
  - Rhyperior Lv.34
  - Sandaconda Lv.35
  - Flygon Lv.37
- Pre-battle dialogue:
  "Dagan said someone would get this far eventually.
   He said to evaluate whether you were worth his time.
   Defeat me and he'll see you himself."
- Post-battle dialogue:
  "He'll be impressed. He respects competence.
   Just — don't expect him to change his plans.
   He never does."
- Gives: Gilt Badge, TM for Dragon Claw

### Gym 4 — Dagan Mire
- Location: Sirocco_DaganPalace_Interior2
- Type: Ground/Rock (mixed, powerful)
- Badge: Mire Badge (Badge 4 of Sirocco, Badge 9 overall)
- Level range: 38-43
- Party:
  - Krookodile Lv.38
  - Flygon Lv.39
  - Tyranitar Lv.41
  - Excadrill Lv.41
  - Garchomp Lv.43
- Pre-battle dialogue:
  "The Warden's child. I've been wondering when you'd show up.
   You want to know something funny? I'm not hiding anything.
   The seal is weakening, the water will run out, and I'll
   control what's left. It's not complicated.
   I just got here first."
- Post-battle dialogue:
  "Well. That's that then."
  (Stands, straightens jacket)
  "I'm not going to fight about it. You won, fair enough.
   But I want you to know — someone else would have done
   exactly what I did. I was just honest about it."
  (Walks to window, looks out)
  "The oasis will come back now, I suppose.
   Good for Miraden. I'll find another opportunity."
  (Escapes — scripted exit, does not stay)
- Gives: Mire Badge, TM for Crunch
- NOTE: Dagan ESCAPES after this scene. He does not get
  arrested or defeated permanently. Set no permanent defeat flag.
- VAR_DAGAN_RELATIONSHIP increments based on player's
  optional dialogue responses during his speech

---

## Key Characters

### Dagan Mire
- Sprite: Use RICH_BOY placeholder until custom sprite ready
- Overworld: present only in DaganPalace_Interior2
- Dialogue tone: charming, self-aware, never defensive
- He finds the player genuinely interesting
- He is NOT redeemed — he just leaves
- Reappears on Gildhaven (referenced in future brief)

### Dex (Archaeologist)
- Sprite: Use SCIENTIST_M placeholder
- Overworld: present at DexCamp throughout
- First major recurring NPC — treat him warmly
- He is excitable, brilliant, slightly disorganized
- His research notes reference the same ancient civilization
  the Haven Isle ruins pointed to
- CRITICAL: Dex survives Sirocco Isle. His death occurs
  on Thalvern — do not foreshadow his death here.
  He should feel safe, optimistic, and alive.

### Miria (Gym 3)
- Sprite: Use COOLTRAINER_F placeholder
- She is efficient, not cruel
- She genuinely believes in Dagan's competence if not his ethics
- Post-resolution she stays in the Buried City continuing
  to catalogue the ruins — now for herself, not Dagan

### Silt (Gym 1)
- Sprite: Use HIKER placeholder
- Post-resolution he goes back to guiding people across the desert
- Optional post-resolution dialogue: "I remembered what the
  old paths feel like. Turns out they're still there."

---

## New Flags Required

Add to include/constants/flags.h:

```c
// Sirocco progression
FLAG_SIROCCO_ARRIVED
FLAG_SIROCCO_GYM1_CLEAR          // Silt defeated
FLAG_SIROCCO_GYM2_CLEAR          // Crag defeated
FLAG_SIROCCO_GYM3_CLEAR          // Miria defeated
FLAG_SIROCCO_GYM4_CLEAR          // Dagan defeated (battle only)
FLAG_SIROCCO_DAGAN_ESCAPED        // Dagan's escape scene played
FLAG_SIROCCO_DEX_MET              // Player spoke to Dex at camp
FLAG_SIROCCO_BURIED_CITY_FOUND    // Player entered BuriedCity
FLAG_SIROCCO_SEAL_FOUND           // Player found the Gilt Claw apparatus
FLAG_SIROCCO_RESOLVED             // Seal reinforced, island complete
FLAG_SIROCCO_CIPHER_FOUND         // Warden's Journal cipher 3 collected
FLAG_HIDDEN_ITEM_SIROCCO_BERRY    // Rawst Berry on Route 1
```

---

## New Variables Required

Confirm VAR_SIROCCO_PROGRESS exists (added in capacity refactor).
Confirm VAR_DAGAN_RELATIONSHIP exists (should be from initial setup).

VAR_SIROCCO_PROGRESS states:
- 0 = not arrived
- 1 = arrived at Dustmouth
- 2 = Gym 1 cleared (Silt)
- 3 = Gym 2 cleared (Crag) — north territory unlocked
- 4 = Gym 3 cleared (Miria)
- 5 = Dagan defeated
- 6 = Seal reinforced
- 7 = Resolved

---

## New Items Required

Add to include/constants/items.h and src/data/items.h:

| Constant | Name | Description | Type |
|---|---|---|---|
| ITEM_SEAL_SHARD_SIROCCO | Sirocco Seal Shard | "A fragment of crystallized legendary energy from Sirocco's Buried City." | Key Item — stub only, not awarded yet |

Note: No new field-traversal key items on Sirocco.
ITEM_GRAPPLE_HOOK (from Ironhold) is used to clear
rubble at BuriedCity_Exterior entrance if desired —
use existing item, no new item needed.

---

## Wild Pokémon Encounters

### Sirocco_DesertRoute1 (Sand patches)
```
Common (40%):   Sandshrew / Sandile
Common (30%):   Trapinch
Uncommon (20%): Cacnea
Rare (10%):     Hippopotas
```

### Sirocco_DesertRoute1 (Daytime only — mirage effect)
```
Very Rare (5%): Sigilyph
(replaces one Cacnea slot during daytime hours)
```

### Sirocco_DesertRoute2 (Sand patches)
```
Common (40%):   Sandile
Common (30%):   Vibrava
Uncommon (20%): Skorupi
Rare (10%):     Silicobra
```

### Sirocco_BuriedCity_Exterior (Sand patches)
```
Common (50%):   Sandile
Uncommon (30%): Baltoy
Rare (20%):     Flygon (Vibrava)
```

### Sirocco_BuriedCity_Interior1 (Dust patches)
```
Common (50%):   Baltoy
Uncommon (30%): Claydol
Rare (20%):     Golurk
```

### Sirocco_GiltClawTerritory (Sand)
```
Common (50%):   Sandaconda
Uncommon (30%): Krookodile (Krokorok)
Rare (20%):     Garchomp (Gabite)
```

No wild encounters in:
- DustmouthPort
- MiradenOasis
- DexCamp
- BuriedCity_Interior2
- BuriedCity_SealChamber
- DaganPalace (any floor)

---

## Trainer Data

### TRAINER_GILTCLAW_SIROCCO_1
- Name: "Enforcer Rael"
- Location: Sirocco_DesertRoute1
- Party: Sandile Lv.20, Krokorok Lv.22
- Pre-battle: "Gilt Claw business. Move along or move through me."
- Post-battle: "...Fine. The boss will hear about this."

### TRAINER_TRAVELER_SIROCCO_1
- Name: "Traveler Yoss"
- Location: Sirocco_DesertRoute1
- Party: Marill Lv.19, Azumarill Lv.21
- Pre-battle: "I'm trying to get to Miraden before my water runs out.
  Battle me — it'll distract me from the heat."
- Post-battle: "Good fight. Want some water? I have exactly enough
  for both of us if we're careful."

### TRAINER_GILTCLAW_SIROCCO_2
- Name: "Enforcer Wex"
- Location: Sirocco_DesertRoute2
- Party: Krokorok Lv.25, Sandaconda Lv.26, Flygon (Vibrava) Lv.27
- Pre-battle: "The ruins are Gilt Claw property.
  Dagan's orders. Nobody goes past here without clearance."
- Post-battle: "You have clearance now, I suppose."

### TRAINER_GILTCLAW_SIROCCO_3
- Name: "Grunt Pell"
- Location: Sirocco_BuriedCity_Exterior
- Party: Sandile Lv.26, Sandile Lv.26, Krokorok Lv.28
- Pre-battle: "Nobody's supposed to be down here.
  Dagan's not going to like this."
- Post-battle: "Going to like it even less now."

### TRAINER_GILTCLAW_SIROCCO_4
- Name: "Grunt Sera"
- Location: Sirocco_BuriedCity_Exterior
- Party: Trapinch Lv.25, Vibrava Lv.27, Flygon Lv.29
- Pre-battle: "I don't get paid enough for this."
- Post-battle: "I definitely don't get paid enough for this."

### TRAINER_GILTCLAW_SIROCCO_5
- Name: "Elite Enforcer Voss"
- Location: Sirocco_GiltClawTerritory
- Party: Garchomp (Gabite) Lv.30, Krookodile Lv.31,
         Tyranitar Lv.33
- Pre-battle: "You've gotten further than anyone expected.
  Dagan said that would happen. He said to make it hurt anyway."
- Post-battle: "He also said to let you through when you did.
  He wants to meet you."

### TRAINER_GILTCLAW_SIROCCO_6
- Name: "Elite Enforcer Mave"
- Location: Sirocco_GiltClawTerritory
- Party: Flygon Lv.31, Sandaconda Lv.32, Excadrill Lv.33
- Pre-battle: "Last line before the palace. Make it count."
- Post-battle: "Go ahead. He's expecting you. He's always
  expecting people."

### TRAINER_GILTCLAW_SIROCCO_7
- Name: "Palace Guard Thorn"
- Location: Sirocco_DaganPalace_Exterior
- Party: Krookodile Lv.33, Garchomp (Gabite) Lv.34,
         Tyranitar Lv.35
- Pre-battle: "The palace is private property."
- Post-battle: "...The door's open."

### TRAINER_GILTCLAW_SIROCCO_8
- Name: "Palace Guard Fen"
- Location: Sirocco_DaganPalace_Exterior
- Party: Excadrill Lv.34, Sandaconda Lv.34, Flygon Lv.36
- Pre-battle: "I've seen Dagan handle situations like this before.
  He always comes out ahead. I believe in him."
- Post-battle: "He's going to be fine. He always is."

### TRAINER_GILTCLAW_SIROCCO_9
- Name: "Inner Guard Rusk"
- Location: Sirocco_DaganPalace_Interior1
- Party: Garchomp Lv.35, Krookodile Lv.36, Tyranitar Lv.37
- Pre-battle: "You really walked in here. Incredible."
- Post-battle: "And now you're walking further in. More incredible."

### TRAINER_GILTCLAW_SIROCCO_10
- Name: "Inner Guard Lace" (different character from Gildhaven's Lace)
- Location: Sirocco_DaganPalace_Interior1
- Party: Flygon Lv.36, Excadrill Lv.37, Garchomp Lv.38
- Pre-battle: "Dagan says you're interesting. I'll see about that."
- Post-battle: "...Okay. You're interesting."

---

## NPC Dialogue Guidelines

**Dustmouth dock enforcer:**
"Gilt Claw port. Water tax is three hundred for non-members.
 Don't have it? The harbor's that way."
(If player tries to pay — no mechanic, just flavor)
"Just kidding. We let travelers through.
 Dagan says a dry mouth tells no tales."
(Ominous laugh, steps aside)

**Dustmouth desperate local (whisper):**
"Don't drink the free water they give out.
 It's fine — I think it's fine — but there's never quite
 enough. There's never quite enough of anything anymore."

**Dustmouth inflated merchant:**
"Gilt Claw Supply Fee. Twenty percent over standard.
 Not negotiable. Dagan's price, not mine.
 I'd leave if I knew where to go."

**Dustmouth sailor (mentions Dex):**
"Some scholar set up a dig camp east of the main route.
 Gilt Claw's been trying to shut him down for months.
 He keeps finding things they don't want found."

**Inn traveler (oasis size):**
"My grandmother described the oasis as a lake.
 My mother called it a pond.
 I call it a puddle.
 My daughter — if I have one — what will she call it?"

**Inn traveler (Buried City):**
"Whatever's under those ruins, Dagan found it before
 the archaeologists did. That's the only explanation
 for why he's so confident the water won't matter soon."

**MiradenOasis child:**
"Mum says don't go past the wet part.
 The wet part keeps getting smaller.
 I measure it every morning with a stick.
 Today it was smaller again."

**MiradenOasis old woman:**
"I've lived here seventy years. The oasis used to
 take ten minutes to walk around. Now it takes two.
 Something is drinking it and it isn't us."

**MiradenOasis Gilt Claw water distributor:**
"Daily ration. Don't argue about the amount.
 Don't ask what happens when it runs out.
 Those aren't questions I'm allowed to answer."

**MiradenOasis scholar:**
"The Buried City is thousands of years old.
 Whatever civilization built it had water in abundance.
 Makes you wonder what they were protecting it from."

**MiradenOasis elder:**
"Before the Gilt Claw, before Dagan, before any of this —
 there was a story the old people told about the desert.
 That the sand remembers what grew here once.
 That something in the deep rock is keeping it alive.
 Dagan found the something."

**DexCamp Dex (first meeting):**
"Oh! A visitor who isn't Gilt Claw. Wonderful.
 You see this sand? Underneath it? A whole city.
 They sealed something down there and the Gilt Claw
 doesn't want anyone to find out what.
 I've found three inscription panels in a week.
 Same script as some ruins I documented years ago
 on a place called Haven Isle. Remarkable, isn't it?
 Same civilization, thousands of miles apart.
 What were they building and why did they seal it?"

**DexCamp Dex (subsequent visits):**
"Found another panel today. The script mentions
 something called a Warden. Someone who maintained
 a network of some kind. Does that mean anything to you?"
(If player has ITEM_WARDENS_JOURNAL): Dex reacts with
recognition — "Wait. That journal. Can I see it?
 I've been trying to decode this script for months and
 your journal uses the same cipher structure. Extraordinary."

**Post-resolution NPCs:**

Silt: "I remembered what the old paths feel like.
  Turns out they're still there."

Crag: "I'm going back to archaeology. Properly this time.
  Dex said I could work his camp. I said yes before
  he finished the sentence."

MiradenOasis child: "The wet part got bigger!
  I measured it this morning. It got bigger by half a stick!"

Old woman: "The oasis is breathing again.
  I didn't think I'd live to see it."

---

## Key Scripts

### Arrival Script
- Player docks at Dustmouth
- Dock enforcer dialogue plays (flavor, no hard block)
- Camera pans to show the shrunken oasis in the distance
- FLAG_SIROCCO_ARRIVED set
- VAR_SIROCCO_PROGRESS = 1

### Dex Meeting Script
- Triggered on first entry to DexCamp
- Requires FLAG_SIROCCO_GYM1_CLEAR (Silt vouches for player)
- If no flag: Dex's tent is closed, sign says "No visitors —
  Gilt Claw watching"
- With flag: full Dex introduction scene
- FLAG_SIROCCO_DEX_MET set
- BuriedCity_Exterior access unlocked

### BuriedCity Discovery Script
- Triggered on first entry to BuriedCity_Exterior
- Brief cutscene: player looks at the scale of the excavation
- Dex appears alongside (if met) and narrates briefly
- FLAG_SIROCCO_BURIED_CITY_FOUND set

### BuriedCity_Interior2 Mural Vision
- Triggered by examining central mural
- Screen flash — brief vision of Xerath and the sealing ceremony
- No dialogue — purely visual
- Cipher fragment unlocked: FLAG_CIPHER_3_FOUND set
  (Warden's Journal cipher 3 of 9)
- FLAG_SIROCCO_CIPHER_FOUND set

### SealChamber Discovery Cutscene
- First entry to SealChamber
- Camera pans across Gilt Claw apparatus
- Xerath's form barely visible beneath the stone
- Dialogue: "This is what's been draining the oasis.
  Not a drought. Not climate. Someone built a machine
  to do this deliberately."
- FLAG_SIROCCO_SEAL_FOUND set

### Dagan Escape Scene (Post-Battle)
- After defeating Dagan in battle
- Dagan delivers his post-battle speech (see Gym 4 section)
- Walks to a side door — scripted exit
- Door closes behind him
- Miria enters from main door, says:
  "He told me this would happen eventually.
   He had a boat ready. He always has a boat ready."
- FLAG_SIROCCO_DAGAN_ESCAPED set
- FLAG_SIROCCO_GYM4_CLEAR set
- Player can now access BuriedCity_SealChamber
  (Miria unblocks the path — "Might as well fix what he broke")

### Island Resolution Script
- Triggered after seal reinforcement in SealChamber
- Brief cutscene: the oasis water level visibly rises
  (ripple effect on oasis tiles)
- Player contacts Sollis via PokéNav
- Sollis: "The Sirocco readings just stabilized.
  What happened?"
- Player: "A crime lord was draining the legendary's seal
  for water control. He got away."
- Sollis: (pause) "The Covenant's work is usually more
  subtle than that. Dagan was operating independently?"
- (This line plants a seed — Sollis knows more about
  Covenant methodology than she should)
- FLAG_SIROCCO_RESOLVED set
- Check if FLAG_EMBERVEIL_RESOLVED is also set:
  - If yes: VAR_BOAT_TIER = 3 (Galleon unlocked),
    Galleon refit dialogue plays
  - If no: dialogue acknowledges Sirocco complete but
    notes Emberveil still needs attention

### Warden's Journal Cipher 3
- Triggered when FLAG_CIPHER_3_FOUND is set
- Journal entry unlocks — partially decoded text:
  "[encoded] ...the Buried City was not built as
   a settlement. It was built as a vault. The seal
   beneath it predates the civilization that built
   the city above it. They built the city to protect
   the seal. [encoded] ...Dex found a panel I missed
   years ago. He doesn't know what it means yet.
   I do. [encoded]"

---

## Battle Terrain Setup

All outdoor Sirocco maps:
```json
"weather": "WEATHER_SANDSTORM"
```

MiradenOasis (sheltered by terrain):
```json
"weather": "WEATHER_SUNNY"
```

BuriedCity interiors and SealChamber:
```json
"weather": "WEATHER_NONE"
```

Battle backgrounds:
- Outdoor routes: desert battle background
- BuriedCity interiors: cave battle background
- DaganPalace: building battle background

---

## Sirocco Isle — Task Checklist

### pelagios-systems-engineer (first)
- [x] Add all Sirocco flags to include/constants/flags.h (0x4E4-0x4EE; hidden item 0x267)
- [x] Confirm VAR_SIROCCO_PROGRESS exists (capacity refactor) — 0x4101, not re-added
- [x] Confirm VAR_DAGAN_RELATIONSHIP exists — 0x40FB, not re-added
- [x] Add ITEM_SEAL_SHARD_SIROCCO stub to items.h — ID 885 (+ src/data/items.h)
- [x] Add all trainer entries to trainers.party
  (TRAINER_GILTCLAW_SIROCCO_1-10 + TRAVELER_1 + 4 gym leaders, IDs 873-887)
- [x] Add gym leader trainer entries:
  - Silt — TRAINER_LEADER_SIROCCO_SILT 884
  - Crag — TRAINER_LEADER_SIROCCO_CRAG 885
  - Miria — TRAINER_LEADER_SIROCCO_MIRIA 886
  - Dagan — TRAINER_LEADER_SIROCCO_DAGAN 887
- [~] Add Sirocco map group stub to map_groups.json — SKIPPED (empty map groups can't
  be expressed in generated groups.inc; map-builder registers MAP_GROUP_SIROCCO with
  the first real map, same established pattern as Haven/Ironhold)
- [x] Compile and fix errors — gmake exit 0, no errors

### pelagios-map-builder (second)
- [x] Sirocco_DustmouthPort
- [x] Sirocco_DustmouthPort_Inn + Interior
- [x] Sirocco_DustmouthPort_BlackMarket
- [x] Sirocco_DesertRoute1
- [x] Sirocco_MiradenOasis
- [x] Sirocco_MiradenOasis_PokemonCenter
- [x] Sirocco_MiradenOasis_Shop
- [x] Sirocco_DexCamp
- [x] Sirocco_DesertRoute2
- [x] Sirocco_BuriedCity_Exterior
- [x] Sirocco_BuriedCity_Interior1
- [x] Sirocco_BuriedCity_Interior2
- [x] Sirocco_BuriedCity_SealChamber
- [x] Sirocco_GiltClawTerritory
- [x] Sirocco_DaganPalace_Exterior
- [x] Sirocco_DaganPalace_Interior1
- [x] Sirocco_DaganPalace_Interior2
- [x] Wild encounter tables (all 5 outdoor areas)
- [x] Heal location (DustmouthPort Inn)
- [x] Reference: use Desert tileset for outdoor maps,
     Cave tileset for BuriedCity interiors,
     Building tileset for palace interiors
- [x] Compile after every 4-5 maps

### pelagios-script-writer (third) — DONE 2026-06-12, gmake exit 0
- [x] Arrival script + camera pan
- [x] Dex meeting script (flag-gated)
- [x] BuriedCity discovery script
- [x] BuriedCity_Interior2 mural vision + cipher unlock (cipher 3: FLAG_CIPHER_3_FOUND)
- [x] SealChamber discovery cutscene
- [x] Dagan escape scene (post-battle scripted exit, VAR_DAGAN_RELATIONSHIP 0-2)
- [x] Island resolution script + conditional Galleon check (Sirocco side; Emberveil's
      SealChamber must mirror it)
- [x] Warden's Journal cipher 3 unlock (at the mural, brief text verbatim)
- [x] All gym leader pre/post battle dialogue (TM subs: Rock Tomb for Stone Edge,
      Thief for Crunch; Mire Badge narrative-only - badge flags end at BADGE08)
- [x] All NPC dialogue per guidelines above
- [x] All trainer pre/post battle dialogue (brief verbatim)
- [x] Post-resolution NPC dialogue variants
- [x] Dex recurring dialogue variants (with/without journal)

### pelagios-build-debugger (last)
- [ ] Full compile passes with zero errors
- [ ] Verify Dagan escape scene plays correctly
- [ ] Verify FLAG_SIROCCO_RESOLVED triggers conditional
      Galleon check correctly
- [ ] Verify Dex dialogue variants fire on journal possession
- [ ] Verify cipher 3 flag sets correctly
- [ ] Update CLAUDE.md — mark Sirocco complete

---

## Prompt to Start

Give this to pelagios-systems-engineer:

```
Read CLAUDE.md and SIROCCO_BRIEF.md. Ironhold is complete
and compiling cleanly. Your task: implement all Sirocco
constants — flags, variables, items, trainer entries including
four gym leaders (Silt, Crag, Miria, Dagan), and map group
registration — following the task checklist in SIROCCO_BRIEF.md
under "pelagios-systems-engineer". Compile after completing
constants and fix any errors. Do not build maps or scripts.
```

---

*This brief covers Sirocco Isle only.
Build Emberveil in parallel using EMBERVEIL_BRIEF.md.
Both must be resolved before Galleon unlocks.*
