# GILDHAVEN — ISLAND BRIEF
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Haven Isle through Schism complete ✅
- VAR_BOAT_TIER = 3 (Galleon) ✅
- VAR_GILDHAVEN_PROGRESS exists in vars.h ✅
- STORY BLOCK 4 opened (0x26C–0x2BB) ✅
- Dagan Mire established from Sirocco ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Gildhaven |
| Theme | Merchant island — glittering surface, rotten underneath, artificial prosperity |
| Primary types | Fairy / Dark |
| Battle terrain | Clear weather, indoor arenas, neon-lit battle backgrounds |
| Legendary | Mirath (Fairy/Dark) — sealed beneath the financial district |
| Boat required | Galleon |
| Parallel islands | Schism Isle, Thalvern (any order) |
| Key return | Dagan Mire reappears here from Sirocco |

---

## Narrative Summary

Gildhaven is the wealthiest island in Pelagios — a glittering trading port
where everything is for sale. Its prosperity is artificial: Mirath's leaking
seal energy has been subtly driving the population toward obsession with
wealth and status for generations. People near the seal become increasingly
unable to think about anything except accumulation.

**Dagan Mire is back.** He escaped Sirocco, identified that Gildhaven's
legendary is the source of the city's prosperity, and has been selling
this information to the highest bidder. The Covenant wants the legendary.
Several merchant lords want to control access to the seal's energy. Dagan
just wants to retire comfortably.

**The corruption mechanic:** The longer the player stays in the financial
district without reinforcing the seal, the more NPCs they encounter are
obsessed with money — their dialogue shifts subtly from normal to
increasingly single-minded. This is atmospheric only (no gameplay penalty)
but creates a creeping wrongness.

**Key story beats:**
1. Arrive at Goldport harbor — immediately feels
   wealthier and slightly off
2. Dagan spotted in the crowd — he's here, he's fine,
   he's already working an angle
3. Defeat Gym 1 (Glint) — street-level trader
4. Discover the black market — optional area,
   good items, Dagan's base of operations
5. Defeat Gym 2 (Shade) — black market enforcer
6. Encounter Lace Vane — Serel Vane's daughter,
   conflicted about her father
7. Defeat Gym 3 (Lace Vane) — the moral midpoint
8. Dagan provides intel on Serel's location:
   "He's in the Exchange basement. I know because
    I designed his security system. I can walk you
    through it. For a reasonable fee."
9. Infiltrate the Exchange — Gildhaven's financial HQ
10. Defeat Gym 4 (Serel Vane)
11. Reinforce Mirath's seal — Gildhaven's economy
    immediately starts destabilizing
12. Lace quietly thanks the player
13. Warden's Journal cipher 7 found
14. FLAG_GILDHAVEN_RESOLVED set

**Cass appears briefly here** — operating on Covenant orders,
clearly uncomfortable. Warns the player to leave without explaining
why. Player finds out why on Dead Island (Ashenveil).

---

## Map Group

```
MAP_GROUP_GILDHAVEN:
  - Gildhaven_GoldportHarbor
  - Gildhaven_GoldportHarbor_Inn
  - Gildhaven_GoldportHarbor_Inn_Interior
  - Gildhaven_MerchantDistrict
  - Gildhaven_MerchantDistrict_PokemonCenter
  - Gildhaven_BlackMarket
  - Gildhaven_NobleQuarter
  - Gildhaven_NobleQuarter_VaneManor
  - Gildhaven_NobleQuarter_VaneManor_Interior
  - Gildhaven_TheExchange_Exterior
  - Gildhaven_TheExchange_Interior1
  - Gildhaven_TheExchange_Interior2
  - Gildhaven_TheExchange_SealChamber
```

---

## Map Descriptions

### Gildhaven_GoldportHarbor
- Entry point — immediately impressive
- Gold-trimmed architecture, spotless docks
- Covenant ships visible in the harbor (ominous)
- Cass is visible in the distance when player arrives —
  brief glimpse, then gone (scripted NPC that
  disappears after player takes 5 steps)
- Key NPCs:
  - Harbor official (efficient, slightly cold):
    "Welcome to Gildhaven. Dock fees are 500 credits.
     We accept all currencies."
    (No actual payment mechanic — just flavor)
  - Sailor who has been here too long:
    "I came to deliver cargo six months ago.
     I keep finding reasons to stay.
     I'm not sure the reasons are real."
- Inn available
- Connects to MerchantDistrict
- Weather: WEATHER_SUNNY (artificial optimism)

### Gildhaven_GoldportHarbor_Inn / Interior
- Most expensive-looking inn so far
- Innkeeper: professionally warm, slightly hollow
- Traveler NPC who has lost track of time:
  "I've been here... three months? Four?
   The city has a way of keeping you.
   I should leave. I will leave. Tomorrow."

### Gildhaven_MerchantDistrict
- Main city area — market stalls, shops, crowds
- Everything slightly too polished, slightly too eager
- Gym 1 entrance (Glint's gym — a converted market stall
  that expanded into a battle arena)
- Gym 2 entrance (Shade's gym — hidden, in an alley)
- Key NPCs:
  - Street vendor who is transparently overcharging:
    "Only the best goods. Genuine artifacts from the
     Sunken Kingdom. [pause] All authentic."
  - Child who has absorbed the island's values:
    "My dad says everything has a price.
     Even the things that don't look like they do."
  - Dagan spotted in a café — waves cheerfully,
    won't engage until after Gym 1
  - Lace Vane visible near the noble quarter entrance —
    watching her father's manor, won't speak until
    after Gym 2
- Connects: GoldportHarbor, BlackMarket, NobleQuarter
- Weather: WEATHER_SUNNY

### Gildhaven_BlackMarket
- Hidden area — accessible via alley behind a specific
  market stall (examine the stall from behind)
- Dagan's base of operations
- Vendors selling items at fair prices:
  (contrast with MerchantDistrict overpricing —
   the black market is actually more honest)
- Dagan is here after Gym 1 is cleared:
  "Ah. You found me. I'm impressed.
   Took the Gilt Claw three weeks to find this place.
   Sit down. I have information you want."
- Information broker NPC who knows about the seal:
  "The Exchange basement has been off-limits for
   forty years. Vane's grandfather built it.
   Nobody asks why a financial building needs
   a basement that deep."
- No wild Pokémon

### Gildhaven_NobleQuarter
- Wealthy residential area — Serel Vane's manor visible
- Lace Vane is here — post-Gym2, she speaks to player
- Gym 3 entrance (Lace's gym — a garden arena at the
  manor's edge, improvised but elegant)
- The Covenant's local presence visible —
  Covenant officers in the noble quarter
- Brief Cass appearance: emerges from the manor,
  sees player, freezes, says "Leave. Please."
  Then walks away quickly (scripted, no battle)
- Connects to MerchantDistrict

### Gildhaven_NobleQuarter_VaneManor / Interior
- Serel Vane's manor — locked until post-Gym3
- Lace gives player access after her gym battle
- Interior: wealth on display, clearly affected by
  Mirath's energy — everything is about acquisition
- Examine study: Covenant documents visible —
  Serel has been in contact with the Covenant about
  Mirath for years
- Connects to TheExchange_Exterior (hidden passage)

### Gildhaven_TheExchange_Exterior
- Gildhaven's financial headquarters — imposing building
- Two trainer battles at the entrance (Serel's elite guards)
- Connects to Interior1
- Weather: WEATHER_SUNNY

### Gildhaven_TheExchange_Interior1
- Grand hall — intimidating wealth
- More trainer battles
- Serel Vane visible on an upper level — watching
- Connects to Interior2

### Gildhaven_TheExchange_Interior2
- Upper floors — Serel's personal space
- Gym 4 location (Serel Vane)
- A map of Pelagios on the wall — every island marked
  with estimated legendary energy values
- Examine the map: it's the Covenant's exploitation
  plan — each seal rated by energy output
- After defeating Serel: hidden stairs to SealChamber

### Gildhaven_TheExchange_SealChamber
- Beneath the Exchange — Mirath's seal
- The oldest-looking room in Gildhaven —
  ancient architecture completely at odds with
  the building above it
- Mirath barely visible — a shimmering Fairy/Dark form
- No Covenant machinery here — the seal leaks naturally,
  no siphoning apparatus needed (Mirath's energy
  seeps upward on its own)
- Player reinforces the seal
- Immediate effect: the shimmering quality of the
  city above begins to fade
- FLAG_GILDHAVEN_RESOLVED set here

---

## Gym Leaders

### Gym 1 — Glint
- Location: Converted market stall, MerchantDistrict
- Type specialist: Fairy
- Badge: Gilt Badge 1 (narrative-only)
- Level range: 44-48
- Party:
  - Sylveon Lv.44
  - Clefable Lv.45
  - Ribombee Lv.46
  - Togekiss Lv.48
- Gym puzzle: navigate market stalls — buy/sell
  items to unlock gates (flavor mechanic, no real
  currency — just interact with stalls in order)
- Pre-battle dialogue:
  "Everyone who comes to Gildhaven wants something.
   You want to go deeper into the city.
   I want to know if you're worth letting through.
   Simple transaction. Let's see."
- Post-battle dialogue:
  "Good. The black market is three alleys back,
   behind the silk stall. Tell them Glint sent you.
   And find Dagan — he's been asking about you."
- Gives: Gilt Badge 1, TM for Dazzling Gleam

### Gym 2 — Shade
- Location: Hidden alley gym, MerchantDistrict
- Type specialist: Dark
- Badge: Shadow Badge (narrative-only)
- Level range: 46-50
- Party:
  - Weavile Lv.46
  - Tyranitar Lv.47
  - Bisharp Lv.48
  - Hydreigon Lv.50
- Gym puzzle: navigate in reduced visibility —
  some lights are out, certain paths only visible
  when standing in specific spots
- Pre-battle dialogue:
  "I work for whoever pays.
   Right now that's Vane.
   Tomorrow — who knows.
   Beat me and I'll consider it a job offer."
- Post-battle dialogue:
  "Vane's daughter is in the noble quarter.
   She's been watching the manor for a week.
   She's not watching it because she loves her father.
   Talk to her before you go further."
- Gives: Shadow Badge, TM for Crunch

### Gym 3 — Lace Vane
- Location: Garden arena, NobleQuarter edge
- Type: Fairy/Dark mix
- Badge: Vane Badge (narrative-only)
- Level range: 48-52
- Party:
  - Mawile Lv.48
  - Morgrem Lv.49
  - Hatterene Lv.50
  - Grimmsnarl Lv.52
- Pre-battle dialogue:
  "My father built his fortune on this island.
   He believes in it. In Gildhaven. In what it
   represents.
   I used to believe in him.
   I'm going to battle you because I need to
   understand something about myself.
   Whether I'm still protecting him or letting go."
- Post-battle dialogue:
  "...Letting go. I think.
   Here — the manor key. The passage to the Exchange
   is through his study. I found it last year.
   I never used it. I'm using it now."
  (Gives VaneManor access + passage to Exchange)
- Post-battle (quiet, to herself, player overhears):
  "I wonder if he ever actually loved this place.
   Or just what it gave him."
- Gives: Vane Badge, TM for Play Rough
- POST-BATTLE: VaneManor access unlocked

### Gym 4 — Merchant Lord Serel Vane
- Location: TheExchange_Interior2
- Type: Fairy/Dark (powerful, deliberately composed team)
- Badge: Exchange Badge (narrative-only)
- Level range: 50-55
- Party:
  - Hatterene Lv.50
  - Grimmsnarl Lv.51
  - Togekiss Lv.52
  - Hydreigon Lv.53
  - Mawile (Mega, if Mega system implemented) Lv.55
- Pre-battle dialogue:
  "I've been expecting someone like you for years.
   The Warden's child, following the seals.
   I want you to understand something before we battle:
   I am not the villain of this story.
   I am a man who found a resource and used it.
   The same as every city ever built.
   The same as every civilization that came before.
   They sealed Mirath to keep it from people like me.
   I simply found it anyway."
- Post-battle dialogue:
  "You've won.
   [pause — genuinely processing this]
   The seal will hold now. The city will change.
   You're going to watch forty years of accumulated
   prosperity dissolve in a season.
   I hope the people who live here appreciate
   what you've done for them.
   [pause]
   I don't think they will."
  (Serel leaves — not arrested, just ruined)
- Gives: Exchange Badge, TM for Moonblast

---

## Key Characters

### Dagan Mire (returning from Sirocco)
- Sprite: Use RICH_BOY placeholder
- He's in the BlackMarket after Gym 1
- Tone: same as Sirocco — charming, pragmatic,
  genuinely helpful when it benefits him
- He provides the Exchange layout (for a "fee"
  that the script waves away as already paid —
  he doesn't actually take anything)
- He will NOT help for free — he says he won't —
  but when pressed on what he wants, he says:
  "Honestly? To watch you pull this off.
   I've been in this city three months.
   I am very bored."
- VAR_DAGAN_RELATIONSHIP increments here
- Post-resolution: Dagan sends a postcard
  (referenced in dialogue from another NPC)

### Lace Vane
- Sprite: Use YOUNGSTER_F placeholder
  (she's young, trying to look older and harder)
- Gym 3 leader and moral turning point
- She's not evil — she's trapped by family loyalty
- Post-resolution: she appears on later islands
  briefly as a traveling merchant (doing it honestly
  this time, without the legendary's artificial boost)
- On Final Island: she's running supplies to
  resistance fighters in the ancient capital

### Serel Vane
- Sprite: Use RICH_BOY placeholder (older version)
- He's been affected by Mirath for so long the
  obsession IS his personality now
- He's not deluded — he knows exactly what he's doing
- He just genuinely doesn't think it's wrong
- Post-resolution: Serel Vane is ruined —
  his wealth was entirely dependent on Mirath's energy
- He doesn't appear on Final Island

### Cass (brief appearance)
- Two scripted appearances:
  1. Harbor arrival: glimpse, then gone
  2. Noble quarter: tells player to leave, then goes
- Cass is on Covenant orders here — uncomfortable
- This is a seeds-of-doubt moment for Cass
- Do NOT have Cass speak more than necessary

---

## New Flags Required

Draw from STORY BLOCK 4 (0x26C–0x2BB):

```c
FLAG_GILDHAVEN_ARRIVED
FLAG_GILDHAVEN_GYM1_CLEAR        // Glint defeated
FLAG_GILDHAVEN_GYM2_CLEAR        // Shade defeated
FLAG_GILDHAVEN_GYM3_CLEAR        // Lace defeated
FLAG_GILDHAVEN_GYM4_CLEAR        // Serel defeated
FLAG_GILDHAVEN_DAGAN_MET         // Dagan met in BlackMarket
FLAG_GILDHAVEN_LACE_TALKED       // Lace spoke post-battle
FLAG_GILDHAVEN_MANOR_ACCESS      // VaneManor access granted
FLAG_GILDHAVEN_COVENANT_MAP_SEEN // Player examined Covenant map
FLAG_GILDHAVEN_SEAL_FOUND        // SealChamber entered
FLAG_GILDHAVEN_RESOLVED          // Seal reinforced
FLAG_GILDHAVEN_CIPHER_FOUND      // Cipher 7 collected
FLAG_CASS_GILDHAVEN_SEEN         // Cass appearance logged
```

---

## New Variables Required

Confirm VAR_GILDHAVEN_PROGRESS exists (0x4105).
Confirm VAR_DAGAN_RELATIONSHIP exists.

VAR_GILDHAVEN_PROGRESS states:
- 0 = not arrived
- 1 = arrived
- 2 = Gym 1 cleared (Glint)
- 3 = Gym 2 cleared (Shade)
- 4 = Gym 3 cleared (Lace)
- 5 = Gym 4 cleared (Serel)
- 6 = Seal reinforced
- 7 = Resolved

---

## New Items Required

```c
ITEM_VANE_MANOR_KEY     // Given by Lace post-battle
ITEM_SEAL_SHARD_GILDHAVEN  // Fairy/Dark energy fragment — stub
```

---

## Wild Pokémon Encounters

Gildhaven has no outdoor routes with wild encounters —
it's an entirely urban island. Wild Pokémon appear
only in specific hidden areas:

### Gildhaven_BlackMarket (hidden area encounters)
```
Common (50%):   Zorua
Uncommon (30%): Snubbull
Rare (20%):     Mawile
```

### Gildhaven_TheExchange_SealChamber
```
Common (50%):   Carbink
Uncommon (30%): Togepi
Rare (20%):     Ralts (Fairy variant encounter —
                 drawn to Mirath's energy,
                 callback to Haven Isle ruins)
```

No wild encounters in:
- Both harbors, MerchantDistrict, NobleQuarter
- VaneManor, Exchange floors 1-2

---

## Trainer Data

### TRAINER_GUARD_GILDHAVEN_1
- Name: "Exchange Guard Rael"
- Location: TheExchange_Exterior
- Party: Mawile Lv.48, Togekiss Lv.49, Grimmsnarl Lv.51
- Pre-battle: "The Exchange is private property.
  Appointment required."
- Post-battle: "...Your appointment has been approved."

### TRAINER_GUARD_GILDHAVEN_2
- Name: "Exchange Guard Sera"
- Location: TheExchange_Exterior
- Party: Hatterene Lv.49, Hydreigon Lv.50, Weavile Lv.51
- Pre-battle: "Mr. Vane doesn't receive unannounced visitors."
- Post-battle: "He'll receive you."

### TRAINER_GUARD_GILDHAVEN_3
- Name: "Elite Guard Morn"
- Location: TheExchange_Interior1
- Party: Tyranitar Lv.50, Bisharp Lv.51,
         Grimmsnarl Lv.52, Hatterene Lv.53
- Pre-battle: "You've gotten further than anyone expected.
  Mr. Vane is impressed. He asked me to make it hurt."
- Post-battle: "He'll be less impressed when I report this."

### TRAINER_GUARD_GILDHAVEN_4
- Name: "Elite Guard Daven"
- Location: TheExchange_Interior1
- Party: Hydreigon Lv.51, Togekiss Lv.52,
         Mawile Lv.52, Weavile Lv.53
- Pre-battle: "Final checkpoint. Mr. Vane is upstairs."
- Post-battle: "He's expecting you now."

---

## NPC Dialogue Guidelines

**Corruption mechanic — dialogue should subtly shift:**
NPCs in MerchantDistrict and NobleQuarter have
two dialogue variants — normal and corrupted.
Use VAR_GILDHAVEN_PROGRESS to switch:
Progress 0-2: normal dialogue
Progress 3+: subtle obsession creeping in
(The player being deeper in the island = closer
to the seal = stronger effect)

**GoldportHarbor official (normal):**
"Welcome to Gildhaven. The finest trading port
 in the archipelago. Dock fees apply."

**GoldportHarbor sailor (too long):**
"I came to deliver cargo six months ago.
 I keep finding reasons to stay.
 The city has a way of keeping you.
 Good opportunities. Always good opportunities.
 I should leave. I will leave.
 Tomorrow."

**MerchantDistrict street vendor:**
"Only the best goods. Genuine artifacts from the
 Sunken Kingdom. [pause] All authentic.
 Very reasonable prices for genuine artifacts.
 Did I mention they're genuine?"

**MerchantDistrict child:**
"My dad says everything has a price.
 Even the things that don't look like they do.
 Even people.
 [pause — child processes this]
 Is that right? That seems wrong."

**MerchantDistrict (corrupted variant, progress 3+):**
"The market was good today. Better than yesterday.
 Tomorrow will be better.
 [without being asked]
 I made four hundred credits today.
 I need five hundred tomorrow."

**Dagan in BlackMarket (first meeting):**
"Ah. The Warden's child. You found me.
 I'm impressed — it took the Gilt Claw three weeks.
 Sit down. I have information and you have
 the only interesting situation on this island.
 I've been here three months.
 I am very bored.
 [pause]
 Here's what I know: Vane's grandfather built
 the Exchange directly above something old.
 Something old that makes people want things.
 I think you know what that something is.
 The passage is through the manor study.
 His daughter knows about it.
 She's never used it.
 [pause]
 I want to watch you pull this off.
 That's my price. Is that acceptable?"

**Dagan (subsequent visits):**
"Still here. Still bored. How's progress?
 [player updates him]
 Excellent. This is the most entertainment
 I've had since Sirocco.
 Did the oasis come back, by the way?
 I heard it came back. Good.
 Good for Miraden."

**Lace Vane (pre-battle):**
"My father built his fortune on this island.
 He believes in it. In what it represents.
 I used to believe in him.
 I'm going to battle you because I need to
 understand something about myself."

**Lace Vane (post-battle, quiet):**
"I wonder if he ever actually loved this place.
 Or just what it gave him.
 [to player, not herself now]
 He was a good father when I was small.
 Before the Exchange. Before all of this.
 I don't know when it changed.
 I don't think he knows either."

**Serel Vane (pre-battle):**
"I've been expecting someone like you for years.
 I want you to understand something before we battle:
 I am not the villain of this story.
 I am a man who found a resource and used it.
 The same as every city ever built."

**Cass (noble quarter, brief):**
"[sees player — freezes]
 Leave. Please.
 [player: why?]
 Because I'm here and you're here and
 if anyone connects those two things —
 [stops herself]
 Just leave. Please."
(Walks away quickly — FLAG_CASS_GILDHAVEN_SEEN set)

**Exchange map (examine):**
"A map of the entire Pelagios archipelago.
 Each island has a notation —
 Haven Isle: legacy node, low yield.
 Ironhold: military site, contested access.
 Sirocco: depleted, write-off.
 Emberveil: volatile, monitoring required.
 Schism: unstable twin system, high risk.
 Thalvern: primary target, extraction pending.
 Gildhaven: current site, active management.
 [...]
 This is the Covenant's plan. Written out.
 Every legendary. Every island.
 Every yield estimate."
FLAG_GILDHAVEN_COVENANT_MAP_SEEN set

**Post-resolution NPCs:**

MerchantDistrict vendor (confused):
"I... had the most extraordinary morning.
 I sat down with my ledger and I looked at the numbers
 and I couldn't remember why they mattered.
 An hour ago I would have said that was impossible.
 Now I'm sitting here wondering what else I want.
 Besides numbers. Besides more.
 I'm not sure. It's been a long time."

MerchantDistrict child:
"Dad sat down at breakfast and started crying.
 He said he didn't know why.
 I think I know why.
 I think something lifted."

Sailor who stayed too long:
"I'm leaving today. Actually today.
 I've been saying tomorrow for months.
 Something changed this morning.
 I remembered where I was going."

---

## Key Scripts

### Arrival Script
- Player docks at Goldport
- Cass glimpse: Cass NPC appears 5 tiles away,
  turns, walks off — player can't catch them
- Harbor official dialogue
- FLAG_GILDHAVEN_ARRIVED set
- VAR_GILDHAVEN_PROGRESS = 1

### Cass Noble Quarter Appearance
- Triggered when player enters NobleQuarter
  for the first time
- Cass emerges from VaneManor
- Sees player
- Brief dialogue (see above)
- Walks back inside quickly
- FLAG_CASS_GILDHAVEN_SEEN set
- This scene fires ONCE — never repeats

### Dagan BlackMarket Meeting
- Triggered on first entry to BlackMarket
  after FLAG_GILDHAVEN_GYM1_CLEAR
- Extended Dagan dialogue
- VAR_DAGAN_RELATIONSHIP increments
- FLAG_GILDHAVEN_DAGAN_MET set

### Exchange Covenant Map Scene
- Triggered by examining map in Interior2
- Extended text reveal of Covenant plan
- FLAG_GILDHAVEN_COVENANT_MAP_SEEN set
- This is a significant lore moment — give it weight

### Serel Post-Battle Scene
- Serel delivers post-battle speech
- Walks to window, looks out at city
- Lace enters from a side door — brief moment
  between Lace and Serel, no dialogue, just a look
- Serel leaves
- Lace to player:
  "Thank you.
   [pause]
   I don't know if he ever actually loved this place.
   Or just what it gave him.
   I don't know if that matters.
   [pause — decides it does matter]
   It matters."
- FLAG_GILDHAVEN_GYM4_CLEAR set

### SealChamber Sequence
- Player enters the ancient chamber beneath the Exchange
- The contrast with the building above is stark —
  ancient stone, silence, no gold anywhere
- Mirath barely visible — a shimmering presence
- No machinery — the seal leaks naturally
  (this is different from all other islands)
- Player reinforces the seal directly — no apparatus
  to disable first
- As the seal stabilizes: the shimmer fades
- Brief outdoor effect (described not shown):
  "Above you, in the city that was built over this
   chamber, forty years of artificial prosperity
   begins to dissolve."
- FLAG_GILDHAVEN_SEAL_FOUND set
- FLAG_GILDHAVEN_RESOLVED set

### Island Resolution Script
- PokéNav call to Solaris:
  "Gildhaven readings just stabilized. Mirath's
   seal is holding. What did you find?"
  Player: [describes]
  Solaris: "The Covenant's yield map. You found it.
   [pause]
   Your parent found a copy of that map eight years ago.
   I have it. I've had it.
   I should have shown you from the beginning.
   I'm sorry."
  (Another layer of Solaris's knowledge revealed)
- Check if Schism + Thalvern + Gildhaven all resolved:
  If yes: Ashenveil unlocks in boat menu
          Brief scene: boat menu adds "Ashenveil"
          Solaris: "The dead island. Your parent
           went there once and wouldn't talk about
           it for a month. Be careful."

### Cipher 7 (Warden's Journal)
- Found when examining SealChamber inscriptions
- Journal entry:
  "Gildhaven is the most honest dishonest place
   I've ever been. Everyone knows the prosperity
   is wrong. Nobody says it. It's the island's
   open secret — like a family that knows one
   member is sick but pretends at dinner that
   everything is fine.
   Mirath doesn't force anyone to want things.
   It just amplifies what's already there.
   That's the terrifying part.
   [encoded] ...Lace Vane is twelve years old.
   She brought me tea and asked me what I was
   researching. I didn't tell her.
   I hope by the time this matters she's grown
   into someone better than her father.
   It's possible. It seemed possible."

---

## Battle Terrain Setup

All Gildhaven maps:
```json
"weather": "WEATHER_SUNNY"
```

TheExchange interiors and SealChamber:
```json
"weather": "WEATHER_NONE"
```

Battle backgrounds:
- Outdoor/MerchantDistrict: town battle background
- Exchange interiors: building battle background
- SealChamber: cave/ancient battle background

---

## Gildhaven — Task Checklist

### pelagios-systems-engineer (first)
- [ ] Add all Gildhaven flags to flags.h (BLOCK 4)
- [ ] Confirm VAR_GILDHAVEN_PROGRESS exists
- [ ] Confirm VAR_DAGAN_RELATIONSHIP exists
- [ ] Add ITEM_VANE_MANOR_KEY
- [ ] Add ITEM_SEAL_SHARD_GILDHAVEN stub
- [ ] Add all trainer entries + 4 gym leaders
      (Glint, Shade, Lace, Serel)
- [ ] Add Gildhaven map group stub
- [ ] Compile and fix errors

### pelagios-map-builder (second)
- [ ] Gildhaven_GoldportHarbor + Inn + Interior
- [ ] Gildhaven_MerchantDistrict
- [ ] Gildhaven_MerchantDistrict_PokemonCenter
- [ ] Gildhaven_BlackMarket
- [ ] Gildhaven_NobleQuarter
- [ ] Gildhaven_NobleQuarter_VaneManor + Interior
- [ ] Gildhaven_TheExchange_Exterior
- [ ] Gildhaven_TheExchange_Interior1
- [ ] Gildhaven_TheExchange_Interior2
- [ ] Gildhaven_TheExchange_SealChamber
- [ ] Wild encounter tables (BlackMarket, SealChamber)
- [ ] Heal location (GoldportHarbor Inn)
- [ ] Cass glimpse NPC placement at harbor
      (object event that walks off on arrival trigger)
- [ ] Covenant map object in Interior2
      (bg_event examine trigger)
- [ ] Use gTileset_General (town/building) throughout
      No custom tileset needed — Gildhaven is urban
- [ ] Compile after every 3-4 maps

### pelagios-script-writer (third)
- [ ] Arrival script + Cass harbor glimpse
- [ ] Cass noble quarter appearance (one-time scene)
- [ ] Dagan BlackMarket meeting + relationship increment
- [ ] Corruption dialogue variants (progress-gated)
- [ ] Exchange Covenant map examination
- [ ] Serel post-battle scene + Lace moment
- [ ] SealChamber sequence (no apparatus — direct seal)
- [ ] Island resolution + Ashenveil unlock check
- [ ] Cipher 7 unlock
- [ ] All gym leader dialogue (Glint, Shade, Lace, Serel)
- [ ] Lace post-battle extended dialogue
- [ ] Dagan dialogue variants (first meeting + subsequent)
- [ ] All NPC dialogue including corruption variants
- [ ] All trainer dialogue
- [ ] Post-resolution NPC variants
- [ ] Ralts encounter in SealChamber (callback to Haven)
      — scripted one-time encounter, not wild table
- [ ] Add Glint, Shade, Lace, Serel, Dagan (Gildhaven
      variant) to pelagios_speaker_names.inc

### pelagios-build-debugger (last)
- [ ] Full compile clean
- [ ] Verify Cass harbor glimpse fires once only
- [ ] Verify Cass noble quarter fires once only
- [ ] Verify corruption dialogue variants trigger
      correctly on VAR_GILDHAVEN_PROGRESS
- [ ] Verify Dagan meeting gated by GYM1_CLEAR
- [ ] Verify Lace gives manor access post-battle
- [ ] Verify Covenant map scene sets flag correctly
- [ ] Verify SealChamber seal reinforcement works
      (no apparatus step — different from other islands)
- [ ] Verify FLAG_GILDHAVEN_RESOLVED triggers
      Ashenveil unlock check correctly
- [ ] Verify Ralts callback encounter fires correctly
- [ ] Verify cipher 7 flag sets correctly
- [ ] Update CLAUDE.md — mark Gildhaven complete

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
GILDHAVEN_BRIEF.md. Schism is complete and compiling
cleanly. Implement all Gildhaven constants — flags
from STORY BLOCK 4, items including ITEM_VANE_MANOR_KEY
and ITEM_SEAL_SHARD_GILDHAVEN stub, trainer entries
including four gym leaders (Glint, Shade, Lace, Serel),
and map group stub — following the checklist in
GILDHAVEN_BRIEF.md. Compile and fix errors.
Do not build maps or scripts.
Note: Thalvern systems engineer may be running in
parallel — check current trainer IDs before assigning
new IDs to avoid collisions.
```

---

*This brief covers Gildhaven only.
Build Thalvern in parallel using THALVERN_BRIEF.md.
Both plus Schism must resolve before Ashenveil unlocks.*
