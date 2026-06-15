# SCHISM ISLE — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle, Ironhold, Sirocco, Emberveil complete ✅
- VAR_BOAT_TIER = 3 (Galleon) must be set ✅
- VAR_SCHISM_PROGRESS exists in vars.h ✅
- Capacity refactor complete ✅
- Custom tilesets registered ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Schism Isle |
| Theme | Split island — cold war between Ice and Poison factions |
| Primary types | Ice (north) / Poison (south) |
| Battle terrain | Snow (north), Acid Rain custom weather (south), none in the Scar |
| Legendaries | Glacith (Ice/Steel) north + Toxara (Poison/Dragon) south — twin legendaries |
| Boat required | Galleon |
| Next tier unlocked | None — Schism is one of three parallel Galleon islands |
| Parallel islands | Thalvern, Gildhaven (any order with Schism) |

---

## Narrative Summary

Schism Isle was once a unified island with one community. Twenty years ago
two Devil Fruit users — one Ice Logia, one Poison Logia — clashed in a
battle that permanently scarred the landscape. The north half froze. The
south half became toxic swampland. The middle zone — the Scar — is a
devastated no-man's-land where nothing lives.

Both factions occupy their respective halves, locked in a cold war. Each
believes the other side's legendary is the problem. Both are actively
weakening the opposing seal. Neither realizes that the twin legendaries
are linked — destroying one destabilizes the other, triggering both.

**Dorne has been here.** He left deliberately misleading information with
both factions encouraging them to break the seals. This is the first
direct evidence of his active plan in the game.

**The player must:**
1. Navigate both halves of the island
2. Defeat gym leaders on each side
3. Achieve a ceasefire between the factions
4. Reinforce both seals simultaneously

**Two resolution outcomes:**

**Ceasefire achieved:**
- Both Eira and Drenn survive
- Eira brings fighters to Final Island
- Post-game: Eira and Drenn stand at the Scar border together
- FLAG_DRENN_ALIVE set

**Ceasefire failed (Drenn dies):**
- Eira's ice faction launches final assault
- Drenn fights to the last, dies from wounds
- Eira comes to Final Island alone, heavier
- FLAG_DRENN_ALIVE not set

The ceasefire is achievable by showing both factions the unified ruins
in the Scar — evidence that they were once one community.

---

## Map Group

```
MAP_GROUP_SCHISM:
  - Schism_FrostmarkPort
  - Schism_FrostmarkPort_Inn
  - Schism_FrostmarkPort_Inn_Interior
  - Schism_FrozenTundra
  - Schism_IceCity
  - Schism_IceCity_PokemonCenter
  - Schism_IceCity_Barracks
  - Schism_IceCity_Barracks_Interior
  - Schism_IceCave
  - Schism_TheScar
  - Schism_ScarRuins
  - Schism_VenomquayPort
  - Schism_VenomquayPort_Inn
  - Schism_VenomquayPort_Inn_Interior
  - Schism_ToxicSwamp
  - Schism_PoisonCity
  - Schism_PoisonCity_PokemonCenter
  - Schism_PoisonCity_Laboratory
  - Schism_PoisonCity_Laboratory_Interior
  - Schism_SealChamber_North
  - Schism_SealChamber_South
```

---

## Map Descriptions

### Schism_FrostmarkPort
- Entry point from sea — ice-covered docks, snow falling
- Ice faction controlled — soldiers visible immediately
- Key NPCs:
  - Ice faction soldier (not hostile, just watchful)
  - Sailor who explains the island's split history briefly
  - Merchant selling cold-resistance items
- Inn available
- Sign: "Frostmark — Ice Territory. Venomquay access prohibited."
- Connects north to FrozenTundra
- Tileset: gTileset_PelagiosIce + gTileset_General
- Weather: WEATHER_SNOW

### Schism_FrostmarkPort_Inn / Interior
- Innkeeper NPC — ice faction member, pragmatic
- Traveler NPC who came to study the island:
  "I found documents suggesting this island had one
   name once. Nobody here will tell me what it was."

### Schism_FrozenTundra
- Route north from Frostmark through frozen landscape
- Snow weather, icy terrain
- Two trainer encounters (ice faction scouts)
- Frozen lake — can cross on ice (no field move needed,
  just passable ice tiles)
- Wild Pokémon in snow patches
- Connects south to FrostmarkPort
- Connects north to IceCity
- Connects east to TheScar (blocked until player has
  spoken to Eira — she gives a Scar pass)
- Tileset: gTileset_PelagiosIce
- Weather: WEATHER_SNOW

### Schism_IceCity
- Ice faction's main settlement — fortress aesthetic,
  everything built to survive permanent winter
- Eira's command post is here
- Key buildings:
  - PokéCenter
  - Barracks (Gym 1 — Sleet's location)
  - Eira's command room (locked until Gym 1 cleared)
- Key NPCs:
  - Sleet (Gym 1, in barracks)
  - Eira (in command room post-Gym1)
  - Ice faction soldiers with varying dialogue
  - One NPC who remembers the unified island:
    "My grandmother told me about the old name.
     I don't repeat it. It makes the commanders
     uncomfortable."
- Gym 2 (Eira) is in the command room
- Connects south to FrozenTundra
- Connects east into IceCave (leads to SealChamber_North)
- Tileset: gTileset_PelagiosIce + gTileset_General
- Weather: WEATHER_SNOW

### Schism_IceCity_Barracks / Interior
- Military barracks — gym 1 location (Sleet)
- Ice training facility aesthetic
- Gym puzzle: slide on ice tiles to reach Sleet
  (classic ice gym sliding puzzle — 4x4 grid)
- Sleet at the far end

### Schism_IceCave
- Cave connecting IceCity to SealChamber_North
- Short linear path
- Wild Pokémon (cave encounters)
- Ancient inscription on cave wall — same script as
  other islands, cross-island lore connection
- Connects west to IceCity, east to SealChamber_North
- Tileset: gTileset_PelagiosIce (cave variant)

### Schism_TheScar
- The devastated middle zone — no man's land
- No weather — the energies cancel each other out
- Eerie stillness, grey ash, dead landscape
- Only Sableye and Mawile visible as wild encounters
- Ancient ruins of the unified community partially visible
- Key: Examine the central ruin structure →
  vision of the island before the split
  (same ancient civilization script)
- Dorne's footprints visible in the ash (examine for
  lore text — he was here recently)
- The unified ruins are the key to the ceasefire:
  FLAG_SCAR_RUINS_FOUND when player examines them
- Connects west to FrozenTundra (ice side access)
- Connects east to ToxicSwamp (poison side access)
- Connects south to ScarRuins (deeper ruins)
- No weather (WEATHER_NONE)
- Tileset: gTileset_General (devastated, grey)

### Schism_ScarRuins
- Deeper ruins beneath the Scar
- Most intact ancient civilization site so far
- Murals showing Glacith and Toxara being sealed
  together by the same civilization
- Ancient text: the two legendaries were worshipped
  as counterparts — Glacith preserves, Toxara cleanses
  Together they maintained the island's balance
- Cipher 5 found here (Warden's Journal)
- Connects north back to TheScar
- No wild encounters
- Tileset: gTileset_General (cave/ruins)

### Schism_VenomquayPort
- Southern entry point — toxic green water, corroded docks
- Poison faction controlled
- Key NPCs:
  - Poison faction guard (suspicious of outsiders)
  - Fisherman who notices the fish are dying:
    "The water's been wrong for months. Getting worse."
  - Merchant selling antidotes and remedies
- Inn available
- Sign: "Venomquay — Poison Territory. Frostmark
   access prohibited."
- Connects north to ToxicSwamp
- NOTE: Player arrives here if they choose to approach
  from the south. The boat destination menu offers both
  Frostmark and Venomquay as Schism options once
  VAR_BOAT_TIER = 3. Player can start from either side.
- Tileset: gTileset_PelagiosPoison + gTileset_General
- Weather: WEATHER_RAIN (acid rain variant)

### Schism_VenomquayPort_Inn / Interior
- Innkeeper who doubles as information broker
- Traveler NPC: "I tried to cross to the ice side once.
  The Scar stopped me. Something in the middle doesn't
  want you there."

### Schism_ToxicSwamp
- Route north from Venomquay through toxic swampland
- Acid rain weather
- Elevated wooden walkways over toxic water
- Wild Pokémon in toxic patches
- Two trainer encounters (poison faction members)
- Connects south to VenomquayPort
- Connects north to PoisonCity
- Connects west to TheScar (blocked until player has
  spoken to Drenn — he gives a Scar pass)
- Tileset: gTileset_PelagiosPoison
- Weather: WEATHER_RAIN

### Schism_PoisonCity
- Poison faction's settlement — industrial, corroded,
  functional in a grimy way
- Drenn's laboratory is here
- Key buildings:
  - PokéCenter
  - Laboratory (Gym 3 — Murk's location + Drenn's office)
  - Various faction housing
- Key NPCs:
  - Murk (Gym 3, in laboratory lower floor)
  - Drenn (in laboratory upper office post-Gym3)
  - Poison faction members
  - One elderly NPC painting the island from memory:
    "I paint it the way it looked before. Green and white
     together. Nobody buys these paintings."
- Gym 4 (Drenn) is in the laboratory upper floor
- Connects south to ToxicSwamp
- Connects west into poison cave (leads to SealChamber_South)
- Tileset: gTileset_PelagiosPoison + gTileset_General
- Weather: WEATHER_RAIN

### Schism_PoisonCity_Laboratory / Interior
- Industrial laboratory aesthetic
- Gym 3 lower floor (Murk) and Gym 4 upper floor (Drenn)
- Gym puzzle: navigate around toxic vats that damage
  on contact — use raised platforms
- Drenn visible in upper office through glass (pre-Gym3)
- After Gym 3: elevator/stairs to upper floor

### Schism_SealChamber_North
- Glacith's seal — deep inside the IceCave
- Glacith barely visible — massive ice/steel form
- Covenant-style machinery visible but cruder —
  both factions have been weakening each other's seals
  with improvised equipment
- Script trigger: discovery cutscene
- Reinforce seal here as part of resolution
- No wild encounters
- Tileset: gTileset_PelagiosIce

### Schism_SealChamber_South
- Toxara's seal — accessible from PoisonCity cave
- Mirror of North chamber but poison aesthetic
- Toxara visible — sinuous, dragon-poison form
- Same machinery, same weakening
- Script trigger: discovery cutscene (mirrors north)
- Reinforce seal here as part of resolution
- No wild encounters
- Tileset: gTileset_PelagiosPoison

---

## Geography & Connections

```
[Sea — Galleon docked at either port]

NORTH (Ice Side):           SOUTH (Poison Side):
Schism_FrostmarkPort        Schism_VenomquayPort
        ↓                           ↓
Schism_FrozenTundra         Schism_ToxicSwamp
        ↓                           ↓
Schism_IceCity              Schism_PoisonCity
  ├── Barracks (Gym 1)        ├── Laboratory (Gym 3+4)
  └── [east → IceCave]        └── [west → poison cave]
        ↓                           ↓
Schism_IceCave          [poison cave → SealChamber_South]
        ↓
Schism_SealChamber_North

         BOTH SIDES CONNECT TO:
              Schism_TheScar
                    ↓
              Schism_ScarRuins
```

Player must navigate both sides. The Scar is accessible
from both FrozenTundra and ToxicSwamp once faction leaders
grant access.

---

## Gym Leaders

### Gym 1 — Sleet (Ice faction)
- Location: Schism_IceCity_Barracks_Interior
- Type specialist: Ice
- Badge: Frost Badge (narrative-only, Badge 10 overall)
- Level range: 40-44
- Party:
  - Sneasel Lv.40
  - Weavile Lv.41
  - Beartic Lv.42
  - Froslass Lv.44
- Gym puzzle: ice sliding puzzle (4x4 grid)
- Pre-battle dialogue:
  "Young. Traveling alone. From the outside.
   Either very brave or very stupid.
   We'll find out which."
- Post-battle dialogue:
  "Brave then. Eira will see you.
   Don't waste her time — she has very little
   patience for people who don't get to the point."
- Gives: Frost Badge (narrative), TM for Blizzard

### Gym 2 — Frost Commander Eira
- Location: Schism_IceCity command room
- Type: Ice (fast, clinical team)
- Badge: Commander Badge (narrative-only, Badge 11)
- Level range: 45-50
- Party:
  - Weavile Lv.45
  - Mamoswine Lv.46
  - Glaceon Lv.47
  - Froslass Lv.48
  - Aurorus Lv.50
- Pre-battle dialogue:
  "The Warden's child. I've read the intelligence reports.
   Your parent visited this island seven years ago.
   They sat with me for two hours and said almost nothing.
   Then they left.
   I've been trying to understand that visit ever since.
   Defeat me and perhaps you'll explain it."
- Post-battle dialogue:
  "...You fight like someone who knows what they're
   protecting. That's rarer than you'd think.
   The Scar pass. Take it.
   Find out what's in the middle.
   Then come back and tell me what my parent
   never did."
- Gives: Commander Badge (narrative), TM for Ice Beam
- POST-BATTLE: Scar pass given — FLAG_EIRA_SCAR_PASS set
- VAR_SCHISM_PROGRESS advances

### Gym 3 — Murk (Poison faction)
- Location: Schism_PoisonCity_Laboratory_Interior (lower)
- Type: Poison/Ghost mix
- Badge: Venom Badge (narrative-only, Badge 12)
- Level range: 40-44
- Party:
  - Gengar Lv.40
  - Toxicroak Lv.41
  - Crobat Lv.42
  - Dragalge Lv.44
- Gym puzzle: navigate toxic vat platforms
- Pre-battle dialogue:
  "Outsider. Interesting.
   The last outsider who came through here worked
   for someone called Dorne. Left some documents.
   Drenn found them very convincing.
   I found them suspicious.
   Beat me and you can tell Drenn why."
- Post-battle dialogue:
  "Drenn is upstairs. He's been studying those
   documents for three weeks.
   Whatever Dorne told him — be ready to argue
   against it. Drenn is brilliant and wrong
   in equal measure."
- Gives: Venom Badge (narrative), TM for Sludge Bomb

### Gym 4 — Venom Lord Cassius Drenn
- Location: Schism_PoisonCity_Laboratory_Interior (upper)
- Type: Poison/Dragon
- Badge: Drenn Badge (narrative-only, Badge 13)
- Level range: 46-52
- Party:
  - Dragalge Lv.46
  - Toxapex Lv.47
  - Salazzle Lv.48
  - Naganadel Lv.50
  - Goodra Lv.52
- Pre-battle dialogue:
  "Dorne was here. Did you know that?
   Left me evidence that the ice faction has been
   deliberately destabilizing our seal for years.
   Proof, he called it.
   I believe him. I've seen what they've done
   to our waters. I've watched our people sicken.
   You're going to tell me Dorne was lying.
   I'd like to see you try."
- Post-battle dialogue (before ceasefire decision):
  "...You're strong. Fine.
   Say what you came to say."
  PLAYER CHOICE:
  Option A: "Dorne manipulated both sides."
  → Drenn is skeptical but listening
  → VAR_SCHISM_CEASEFIRE_PROGRESS = 1
  Option B: "The Scar has proof. Come see it."
  → Drenn agrees to visit the Scar (requires
    FLAG_SCAR_RUINS_FOUND)
  → VAR_SCHISM_CEASEFIRE_PROGRESS = 2
  Option C: Say nothing / leave
  → Drenn remains convinced by Dorne's documents
  → Ceasefire becomes harder but not impossible
- Gives: Drenn Badge (narrative), TM for Dragon Pulse
- POST-BATTLE: Scar pass given from south side
  FLAG_DRENN_SCAR_PASS set

---

## Ceasefire System

The ceasefire is the island's central mechanic.
It requires:

1. Player defeats both Eira (Gym 2) and Drenn (Gym 4)
2. Player finds the ScarRuins and sets FLAG_SCAR_RUINS_FOUND
3. Player speaks to both leaders about the Scar

**Ceasefire dialogue triggers:**
- Speak to Eira after finding ScarRuins:
  She agrees to meet at the Scar border
  FLAG_EIRA_CEASEFIRE_WILLING set
- Speak to Drenn after finding ScarRuins
  (requires VAR_SCHISM_CEASEFIRE_PROGRESS >= 1):
  He agrees to meet (reluctantly)
  FLAG_DRENN_CEASEFIRE_WILLING set

**The Ceasefire Meeting (TheScar — scripted):**
- Triggered when both flags are set and player
  enters TheScar
- Eira and Drenn appear on opposite sides of the Scar
- Player walks between them
- Both examine the unified ruins
- Neither speaks for a long moment
- Eira: "This was one place."
- Drenn: "Yes."
- Another silence
- Neither shakes hands
- They both stop attacking the seals
- FLAG_SCHISM_CEASEFIRE set
- VAR_SCHISM_PROGRESS = 6

**If ceasefire fails:**
- If player seals north without ceasefire:
  Eira's faction attacks south
  Drenn fights them, is mortally wounded
  Player finds him after — brief scene
  FLAG_DRENN_ALIVE not set
- If player seals south without ceasefire:
  Mirror — Drenn's faction retaliates
  (Eira survives either way — she's more cautious)

---

## Key Characters

### Frost Commander Eira
- Sprite: Use COOLTRAINER_F placeholder
- Overworld: IceCity command room (post-Gym1)
  TheScar (ceasefire scene)
- Pragmatic, cold, has been fighting this war her
  whole life. Respects strength above all.
- She does NOT trust easily — the ceasefire costs her
- Post-resolution: stays on Schism, watches the border
- Final Island: arrives with ice faction fighters
  if ceasefire achieved

### Venom Lord Cassius Drenn
- Sprite: Use SCIENTIST_M placeholder
  (he's a brilliant paranoid researcher, not a warrior)
- Overworld: PoisonCity laboratory upper floor
  TheScar (ceasefire scene, if achieved)
- Paranoid, brilliant, has been fighting this war his
  whole life. Keeps a painting of the unified island.
- If ceasefire achieved: survives, doesn't forgive
  Eira, but stops fighting
- If ceasefire fails: dies defending his city
- NOTE: His Dragalge has been with him since childhood —
  mention this in his pre-battle setup text

### Murk (Gym 3)
- Sprite: Use COOLTRAINER_M placeholder
- The one who noticed Dorne's documents were suspicious
- Post-resolution: if ceasefire, becomes the faction's
  first contact with the ice side

### Sleet (Gym 1)
- Sprite: Use YOUNGSTER placeholder (young soldier)
- Youngest gym leader in the game
- Post-resolution: if ceasefire, he's the one who
  actually crosses the Scar first — curiosity over fear

---

## New Flags Required

Add to include/constants/flags.h:

```c
// Schism progression
FLAG_SCHISM_ARRIVED
FLAG_SCHISM_GYM1_CLEAR           // Sleet defeated
FLAG_SCHISM_GYM2_CLEAR           // Eira defeated
FLAG_SCHISM_GYM3_CLEAR           // Murk defeated
FLAG_SCHISM_GYM4_CLEAR           // Drenn defeated
FLAG_EIRA_SCAR_PASS              // Eira gave Scar access
FLAG_DRENN_SCAR_PASS             // Drenn gave Scar access
FLAG_SCAR_RUINS_FOUND            // Player found unified ruins
FLAG_EIRA_CEASEFIRE_WILLING      // Eira agreed to meet
FLAG_DRENN_CEASEFIRE_WILLING     // Drenn agreed to meet
FLAG_SCHISM_CEASEFIRE            // Ceasefire achieved
FLAG_DRENN_ALIVE                 // Drenn survived (ceasefire)
FLAG_SCHISM_SEAL_NORTH_FOUND     // North SealChamber discovered
FLAG_SCHISM_SEAL_SOUTH_FOUND     // South SealChamber discovered
FLAG_SCHISM_SEAL_NORTH_DONE      // Glacith seal reinforced
FLAG_SCHISM_SEAL_SOUTH_DONE      // Toxara seal reinforced
FLAG_SCHISM_RESOLVED             // Both seals reinforced
FLAG_SCHISM_CIPHER_FOUND         // Cipher 5 collected
FLAG_HIDDEN_ITEM_SCHISM_1        // Hidden item ice side
FLAG_HIDDEN_ITEM_SCHISM_2        // Hidden item scar
```

---

## New Variables Required

Confirm VAR_SCHISM_PROGRESS exists (capacity refactor).
Add:

```c
VAR_SCHISM_CEASEFIRE_PROGRESS  // 0=none, 1=skeptical, 2=willing, 3=done
```

VAR_SCHISM_PROGRESS states:
- 0 = not arrived
- 1 = arrived (at either port)
- 2 = Gym 1 cleared (Sleet)
- 3 = Gym 2 cleared (Eira) + Scar pass
- 4 = Gym 3 cleared (Murk)
- 5 = Gym 4 cleared (Drenn) + south Scar pass
- 6 = Ceasefire achieved OR seals reinforced unilaterally
- 7 = Resolved

---

## New Items Required

Add to include/constants/items.h and src/data/items.h:

| Constant | Name | Description | Type |
|---|---|---|---|
| ITEM_SCAR_PASS_ICE | Ice Faction Pass | "Eira's mark. Grants passage through the Scar from the north." | Key Item |
| ITEM_SCAR_PASS_POISON | Poison Faction Pass | "Drenn's mark. Grants passage through the Scar from the south." | Key Item |
| ITEM_SEAL_SHARD_GLACITH | Glacith Seal Shard | "Crystallized energy from Glacith's seal. Ice-cold to the touch." | Key Item stub |
| ITEM_SEAL_SHARD_TOXARA | Toxara Seal Shard | "Crystallized energy from Toxara's seal. Faintly corrosive." | Key Item stub |

---

## Wild Pokémon Encounters

### Schism_FrozenTundra (Snow patches)
```
Common (40%):   Swinub
Common (30%):   Snorunt
Uncommon (20%): Cubchoo
Rare (10%):     Sneasel
```

### Schism_IceCave
```
Common (50%):   Bergmite
Uncommon (30%): Cryogonal
Rare (20%):     Jynx
```

### Schism_TheScar (Rare encounters only)
```
Common (50%):   Sableye
Uncommon (50%): Mawile
(No other encounters — only these two types
 survive in the neutral zone)
```

### Schism_ToxicSwamp (Toxic patches)
```
Common (40%):   Grimer (Alolan)
Common (30%):   Croagunk
Uncommon (20%): Mareanie
Rare (10%):     Foongus
```

### Schism_ScarRuins
```
No wild encounters — pure story area
```

No wild encounters in:
- Both ports
- Both cities
- Both laboratories/barracks
- Both SealChambers

---

## Trainer Data

### TRAINER_ICEFACTION_SCHISM_1
- Name: "Scout Heln"
- Location: Schism_FrozenTundra
- Party: Sneasel Lv.38, Swinub Lv.39
- Pre-battle: "Ice faction territory. State your business."
- Post-battle: "...Pass through. Carefully."

### TRAINER_ICEFACTION_SCHISM_2
- Name: "Scout Vera"
- Location: Schism_FrozenTundra
- Party: Cubchoo Lv.38, Snorunt Lv.39, Beartic Lv.41
- Pre-battle: "The cold doesn't bother us. Does it bother you?"
- Post-battle: "Apparently not. Interesting."

### TRAINER_POISONFACTION_SCHISM_1
- Name: "Wader Osk"
- Location: Schism_ToxicSwamp
- Party: Croagunk Lv.38, Grimer Lv.39
- Pre-battle: "Don't drink the water. Don't touch the water.
  Actually, just battle me and then keep moving."
- Post-battle: "Smart. Keep moving."

### TRAINER_POISONFACTION_SCHISM_2
- Name: "Wader Fen"
- Location: Schism_ToxicSwamp
- Party: Mareanie Lv.39, Toxicroak Lv.40, Dragalge Lv.42
- Pre-battle: "You smell like the ice side. Or maybe
  that's just outside. Hard to tell anymore."
- Post-battle: "You're tougher than you smell."

### TRAINER_ICEFACTION_SCHISM_3
- Name: "Guard Rael"
- Location: Schism_IceCave
- Party: Bergmite Lv.40, Cryogonal Lv.41, Weavile Lv.43
- Pre-battle: "Cave's restricted. Commander's orders."
- Post-battle: "You have the pass. Go ahead."

### TRAINER_POISONFACTION_SCHISM_3
- Name: "Researcher Siv"
- Location: Schism_PoisonCity_Laboratory_Interior
- Party: Gengar Lv.40, Crobat Lv.41, Toxapex Lv.43
- Pre-battle: "I'm running experiments. You're interrupting.
  Defeat me and I'll at least be interrupted by someone capable."
- Post-battle: "Noted. Drenn's upstairs. Good luck."

---

## NPC Dialogue Guidelines

**Frostmark sailor:**
"Split island. Has been since before I was born.
 Used to be one port — or so the old charts say.
 Now there's two, and neither talks to the other.
 I do deliveries to both. Don't tell either."

**Frostmark merchant:**
"Cold-resistance gear. Best investment you'll make here.
 The south side has the same shop but selling antidotes.
 I know because I'm the same person.
 Different coat. Don't tell Eira."
(Same character runs both faction shops — comic detail)

**Frostmark Inn traveler:**
"I found documents suggesting this island had one
 name once. Nobody here will tell me what it was.
 I think they know. I think remembering hurts."

**IceCity soldier who remembers:**
"My grandmother told me about the old name.
 I don't repeat it. It makes the commanders
 uncomfortable.
 She said the island used to smell like pine and
 salt at the same time. I've never smelled that."

**IceCity NPC (uncomfortable about saluting):**
"My dad says don't salute the Covenant ships when
 they pass. Eira says we don't answer to the Covenant.
 I salute anyway. It seems safer."

**Venomquay fisherman:**
"The fish are dying. Not just fewer — wrong.
 Colors are off. Behavior's off.
 Something in the deep water changed months ago.
 The faction says it's the ice side's fault.
 The fish don't care whose fault it is."

**PoisonCity elderly painter:**
"I paint it the way it looked before. Green and white
 together. Nobody buys these paintings.
 This one's the harbor. One harbor. One name.
 I'll paint until I can't anymore."

**TheScar — examine Dorne's footprints:**
"Fresh tracks in the ash. Recently made.
 Someone was standing here, looking at the ruins.
 Then they walked toward the poison side.
 The tracks are precise. Purposeful.
 Whoever this was, they weren't lost."

**TheScar — examine central ruins:**
[Vision flash — the island before the split,
 unified community, both legendaries visible
 in the distance, coexisting]
"The ruins show a single settlement.
 The same architecture as both cities — because
 they were built by the same people."
FLAG_SCAR_RUINS_FOUND set

**ScarRuins murals:**
"The murals show two Pokémon sealed together —
 one of ice, one of poison. Not enemies.
 Counterparts. The inscription reads:
 [ancient script — partially decoded with enough ciphers]
 '...Glacith preserves. Toxara cleanses.
  Neither alone. Both together.
  The island breathes between them...'"

**Post-ceasefire NPCs:**

IceCity soldier who remembered:
"Sleet crossed the Scar this morning.
 Nobody stopped him.
 I don't know what that means yet.
 I think it means something."

PoisonCity painter:
"Someone bought a painting today.
 Ice faction uniform. Young.
 He didn't say anything. Just paid and left.
 I think I'll paint faster now."

Frostmark sailor/merchant:
"I can use one coat now.
 Do you have any idea how much that saves me?"

---

## Key Scripts

### Dual Arrival Script
- Schism has TWO arrival ports in the boat menu
- Both Frostmark and Venomquay appear as options
  when VAR_BOAT_TIER = 3
- Update pelagios_boat.inc to offer both ports:
  "Schism Isle (North)" and "Schism Isle (South)"
- Each arrival sets FLAG_SCHISM_ARRIVED and
  VAR_SCHISM_PROGRESS = 1

### Scar Access Scripts
- TheScar connection from FrozenTundra:
  Check FLAG_EIRA_SCAR_PASS
  If absent: ice faction soldier blocks path
  "The Scar is restricted. Commander's orders."
  If present: soldier steps aside silently
- TheScar connection from ToxicSwamp:
  Check FLAG_DRENN_SCAR_PASS
  Same pattern, poison faction guard

### ScarRuins Discovery
- Triggered by examining central ruin structure
  in TheScar
- Screen flash — vision of unified island
- Dialogue as above
- FLAG_SCAR_RUINS_FOUND set
- New dialogue options unlock with Eira and Drenn

### Ceasefire Meeting (TheScar — scripted cutscene)
- Triggered when FLAG_EIRA_CEASEFIRE_WILLING and
  FLAG_DRENN_CEASEFIRE_WILLING both set and player
  enters TheScar
- lockall
- Eira walks in from west (ice side)
- Drenn walks in from east (poison side)
- Player stands between them
- Long pause (waitmovement, no dialogue)
- Both look at the ruins
- Eira speaks (one line)
- Drenn responds (one line)
- Another pause
- Neither shakes hands
- Both turn and walk back to their respective sides
- releaseall
- FLAG_SCHISM_CEASEFIRE set
- VAR_SCHISM_PROGRESS = 6
- VAR_SCHISM_CEASEFIRE_PROGRESS = 3

### North SealChamber Discovery
- First entry to Schism_SealChamber_North
- Glacith barely visible through the ice
- Faction machinery visible — crude but effective
- Player interacts with machinery to disable it
- Player interacts with seal to reinforce
- FLAG_SCHISM_SEAL_NORTH_DONE set
- Check if FLAG_SCHISM_SEAL_SOUTH_DONE also set:
  If yes: resolution triggers
  If no: "One seal stable. The other still weakens."

### South SealChamber Discovery
- Mirror of north — Toxara, poison aesthetic
- Same mechanic
- FLAG_SCHISM_SEAL_SOUTH_DONE set
- Check if FLAG_SCHISM_SEAL_NORTH_DONE also set

### Ceasefire Failure (if seals reinforced without ceasefire)
- If FLAG_SCHISM_SEAL_NORTH_DONE set WITHOUT
  FLAG_SCHISM_CEASEFIRE:
  Trigger Eira's assault on south
  Brief cutscene: sounds of conflict from south
  Player finds Drenn wounded in PoisonCity
  Drenn: "You should have talked faster."
  He dies. FLAG_DRENN_ALIVE stays unset.
- If FLAG_SCHISM_SEAL_SOUTH_DONE set WITHOUT
  FLAG_SCHISM_CEASEFIRE:
  Mirror — Drenn's retaliation
  Eira survives (she's more defensively positioned)

### Island Resolution Script
- Both seals reinforced — FLAG_SCHISM_RESOLVED set
- Brief cutscene: both halves of the island visible,
  weather shifts — north stays cold, south stays damp,
  but the Scar begins to show the faintest green
- PokéNav call to Sollis:
  "Schism readings just stabilized — both seals.
   Simultaneously. How did you manage that?"
  Player: [describes ceasefire or unilateral action]
  Sollis reacts differently based on outcome:
  Ceasefire: "Your parent tried that once. Couldn't
   get them to the table. You did."
  Unilateral: "The seals are stable. That's what
   matters. I hope the cost was worth it."
- FLAG_SCHISM_RESOLVED set
- Check if Thalvern and Gildhaven also resolved:
  Galleon tier stays 3 — no new boat needed
  (Ashenveil unlocks after all three resolved)

### Warden's Journal Cipher 5
- Found in ScarRuins examination
- Journal entry:
  "The island had a name before the split.
   I found it in the unified ruins.
   I'm not writing it here — if someone reads
   this journal before Schism is healed,
   knowing the name won't help them.
   If someone reads this after —
   ask Eira. She knows. She just doesn't say.
   [encoded] ...Dorne was here six months ago.
   He spoke to both factions. I found his
   documents in Drenn's office — copies.
   He's been feeding both sides the same lie
   in different directions. Brilliant.
   Terrifying. [encoded]"

---

## Battle Terrain Setup

Schism_FrostmarkPort, FrozenTundra, IceCity, IceCave:
```json
"weather": "WEATHER_SNOW"
```

Schism_VenomquayPort, ToxicSwamp, PoisonCity:
```json
"weather": "WEATHER_RAIN"
```

Schism_TheScar, ScarRuins:
```json
"weather": "WEATHER_NONE"
```

SealChambers: WEATHER_NONE

Battle backgrounds:
- Ice maps: snow/ice battle background
- Poison maps: cave/swamp battle background
- Scar: plain/ash battle background
- SealChambers: cave battle background

---

## Schism Isle — Task Checklist

### pelagios-systems-engineer (first)
- [ ] Add all Schism flags to include/constants/flags.h
- [ ] Add VAR_SCHISM_CEASEFIRE_PROGRESS to vars.h
- [ ] Confirm VAR_SCHISM_PROGRESS exists
- [ ] Add ITEM_SCAR_PASS_ICE and ITEM_SCAR_PASS_POISON
- [ ] Add ITEM_SEAL_SHARD_GLACITH and ITEM_SEAL_SHARD_TOXARA stubs
- [ ] Add all trainer entries to trainers.party
  (6 generic trainers + 4 gym leaders)
- [ ] Add gym leader entries:
  - Sleet (next available ID)
  - Eira
  - Murk
  - Drenn
- [ ] Update pelagios_boat.inc to offer dual Schism ports
  ("Schism Isle (North)" → FrostmarkPort,
   "Schism Isle (South)" → VenomquayPort)
- [ ] Add Schism map group stub to map_groups.json
- [ ] Compile and fix errors

### pelagios-map-builder (second) — DONE 2026-06-13 (gmake exit 0)
- [x] Schism_FrostmarkPort
- [x] Schism_FrostmarkPort_Inn + Interior
- [x] Schism_FrozenTundra
- [x] Schism_IceCity
- [x] Schism_IceCity_PokemonCenter
- [x] Schism_IceCity_Barracks + Interior (ice sliding puzzle = SHOAL_CAVE_LOW_TIDE_ICE_ROOM)
- [x] Schism_IceCave
- [x] Schism_TheScar
- [x] Schism_ScarRuins
- [x] Schism_VenomquayPort
- [x] Schism_VenomquayPort_Inn + Interior
- [x] Schism_ToxicSwamp
- [x] Schism_PoisonCity
- [x] Schism_PoisonCity_PokemonCenter
- [x] Schism_PoisonCity_Laboratory + Interior (two-level = AQUA_HIDEOUT_1F, elev split)
- [x] Schism_SealChamber_North
- [x] Schism_SealChamber_South
- [x] Wild encounter tables (FrozenTundra, IceCave,
      TheScar, ToxicSwamp)
- [x] Heal locations (FrostmarkPort Inn, VenomquayPort Inn).
      DEVIATION: NOT added to IsLastHealLocationPlayerHouse — Inns reuse the
      Poke-Center layout + nurse-gfx innkeeper, so the nurse whiteout path is correct;
      adding them would wrongly trigger the mom-heal (see CLAUDE.md Completed — Schism MAPS).
- [x] Use gTileset_PelagiosIce for north maps
- [x] Use gTileset_PelagiosPoison for south maps
- [x] Use gTileset_General (+Cave) for TheScar/ScarRuins
- [x] Scar access blockers (soldiers) on both
      FrozenTundra→TheScar and ToxicSwamp→TheScar
      connections (blocker objects + TODO scripts; no flag-as-var coord gate)
- [x] Compile after every 4-5 maps
- [x] BONUS: dual boat stubs swapped to real warps (FrostmarkPort/VenomquayPort 10,14)

### pelagios-script-writer (third)
- [ ] Dual arrival scripts (both ports)
- [ ] Scar access gate scripts (both sides)
- [ ] ScarRuins discovery + vision flash
- [ ] Ceasefire meeting cutscene (TheScar)
- [ ] Ceasefire failure script (both variants)
- [ ] North SealChamber discovery + reinforcement
- [ ] South SealChamber discovery + reinforcement
- [ ] Dual seal completion check
- [ ] Island resolution + Ashenveil unlock check
- [ ] Cipher 5 unlock
- [ ] Eira post-battle dialogue + Scar pass handoff
- [ ] Drenn post-battle dialogue + choice system
- [ ] Drenn ceasefire willing trigger
- [ ] Eira ceasefire willing trigger
- [ ] All gym leader pre/post battle dialogue
- [ ] All NPC dialogue per guidelines above
- [ ] All trainer dialogue
- [ ] Post-ceasefire NPC dialogue variants
- [ ] Post-failure NPC dialogue variants
- [ ] Drenn death scene (ceasefire failure)
- [ ] Use setspeaker for all named NPCs
- [ ] Add Eira, Drenn, Sleet, Murk to
      pelagios_speaker_names.inc

### pelagios-build-debugger (last)
- [ ] Full compile passes with zero errors
- [ ] Verify dual Schism ports in boat menu
- [ ] Verify Scar gates work from both sides
- [ ] Verify ceasefire triggers correctly
- [ ] Verify ceasefire failure triggers correctly
- [ ] Verify dual seal completion check
- [ ] Verify FLAG_SCHISM_RESOLVED sets correctly
- [ ] Verify cipher 5 flag sets correctly
- [ ] Verify both heal locations in
      IsLastHealLocationPlayerHouse
- [ ] Update CLAUDE.md — mark Schism complete

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
SCHISM_BRIEF.md. Emberveil is complete and compiling
cleanly. Implement all Schism constants — flags,
variables including VAR_SCHISM_CEASEFIRE_PROGRESS,
items including dual Scar passes and two Seal Shard
stubs, trainer entries including four gym leaders
(Sleet, Eira, Murk, Drenn), dual Schism port entries
in pelagios_boat.inc, and map group registration —
following the task checklist in SCHISM_BRIEF.md under
pelagios-systems-engineer. Compile after completing
constants and fix any errors. Do not build maps or
scripts.
```

---

*This brief covers Schism Isle only.
Build Thalvern and Gildhaven in parallel using
their respective briefs once Schism constants are in.
All three must be resolved before Ashenveil unlocks.*
