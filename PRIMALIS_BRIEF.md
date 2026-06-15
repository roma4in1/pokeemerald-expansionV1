# PRIMALIS — ISLAND BRIEF (BEAST ISLAND)
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
Before starting this island:
- Schism, Thalvern, Gildhaven all complete ✅
- FLAG_SCHISM_RESOLVED, FLAG_THALVERN_RESOLVED,
  FLAG_GILDHAVEN_RESOLVED all set ✅
- Ashenveil unlocked in boat menu ✅
- VAR_PRIMALIS_PROGRESS exists in vars.h ✅
- STORY BLOCK 4 available (0x282–0x2BB) ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Primalis |
| Theme | Beast Island — jungle, primal, Zoan guardian community |
| Primary types | Grass / Dragon |
| Battle terrain | Grassy Terrain, dense fog, jungle battle backgrounds |
| Legendary | Verdath (Grass/Dragon) — oldest sealed legendary, predates the civilization |
| Boat required | Galleon |
| Access | Unlocked after Schism + Thalvern + Gildhaven resolved |
| Key item obtained | ITEM_BEAST_WHISTLE |
| Key character | Elder Mako — gives player carved token for Final Island |

---

## Narrative Summary

Primalis is the oldest island in Pelagios — its jungle predates the ancient
civilization. The legendary Verdath is so old that the jungle itself is an
expression of its power — the island literally grew around the sealed legendary
over millennia.

A community of Zoan users have lived on Primalis for generations, drawn by
Verdath's energy. They are the island's guardians. A faction has started
transforming completely and can't change back — Verdath's energy is
destabilizing their transformations as the seal weakens.

The Lens reappears here, following Dex's research leads.

Elder Mako is the community leader — part-shark transformation, ancient,
deeply suspicious of outsiders. Earning his trust is the island's central
mechanic. He reveals the oral history: the kill switch exists, Haven Isle
is the central node.

ITEM_BEAST_WHISTLE obtained here — replaces Cut/Sweet Scent.

Key story beats:
1. Arrive at Verdant Landing
2. The Lens is at the inn — gives context
3. Defeat Gym 1 (Fern) — young Zoan guardian
4. Navigate dense jungle
5. Defeat Gym 2 (Scale) — mid-rank guardian
6. Elder Mako accessible — first meeting, suspicious
7. Defeat Gym 3 (Thorn) on JungleRoute2
8. Elder Mako reveals oral history (post-Gym4)
9. Defeat Gym 4 (Elder Mako)
10. Receive ITEM_BEAST_WHISTLE + carved token
11. Reinforce Verdath's seal in the Heartwood
12. Fully-transformed Zoan members begin recovering
13. Cipher 8 found
14. FLAG_PRIMALIS_RESOLVED set

---

## Map Group

```
MAP_GROUP_PRIMALIS:
  - Primalis_VerdantLanding
  - Primalis_VerdantLanding_Inn
  - Primalis_VerdantLanding_Inn_Interior
  - Primalis_JungleRoute1
  - Primalis_JungleInterior
  - Primalis_ZoanVillage
  - Primalis_ZoanVillage_PokemonCenter
  - Primalis_ZoanVillage_ElderHall
  - Primalis_ZoanVillage_ElderHall_Interior
  - Primalis_JungleRoute2
  - Primalis_AncientRuinsCamp
  - Primalis_TheHeartwood
  - Primalis_Heartwood_SealChamber
```

---

## Gym Leaders

### Gym 1 — Fern
- Location: Training area, ZoanVillage edge
- Type: Grass — part-plant Zoan
- Badge: Verdant Badge (narrative-only)
- Level range: 48-52
- Party: Tropius 48, Tsareena 49, Rillaboom 50, Decidueye 52
- Pre-battle: "You want to go deeper. Elder Mako said
  outsiders aren't welcome. But he also said to
  evaluate anyone who made it this far.
  I'm evaluating."
- Post-battle: "You move like you know what you're
  protecting. Elder Mako will see you. Beat Scale first."
- Gives: Verdant Badge, TM for Leaf Storm

### Gym 2 — Scale
- Location: Village center, ZoanVillage
- Type: Dragon — part-dragon Zoan
- Badge: Scale Badge (narrative-only)
- Level range: 51-55
- Party: Haxorus 51, Flygon 52, Drampa 53, Kommo-o 55
- Pre-battle: "The fully-transformed ones — you saw them.
  They were friends. Whatever is happening started
  three months ago. Same time the forest felt wrong.
  If you understand why, tell me after."
- Post-battle: "Elder Mako's hall. He'll see you now.
  He won't trust you yet. That comes after."
- Gives: Scale Badge, TM for Dragon Dance

### Gym 3 — Thorn
- Location: JungleRoute2 (encounters player on route)
- Type: Grass/Dragon mix
- Badge: Thorn Badge (narrative-only)
- Level range: 53-57
- Party: Decidueye 53, Kommo-o 54, Kartana 55, Dragapult 57
- Pre-battle: "Elder Mako told me about you. He said
  you might be the one the Root has been waiting for.
  He also said to make sure. I'm making sure."
- Post-battle: "You're sure. The Heartwood is past
  the ruins camp. Elder Mako will go with you."
- Gives: Thorn Badge, TM for Power Whip

### Gym 4 — Elder Mako
- Location: ZoanVillage_ElderHall_Interior
- Type: Dragon/Water (shark transformation)
- Badge: Elder Badge (narrative-only)
- Level range: 55-60
- Party: Garchomp 55, Sharpedo 56, Kommo-o 57,
         Dragapult 58, Goodra 60
- Pre-battle: "Our oral history says a guardian will
  come from outside. They will not understand at first.
  But they will learn. I need to know if you are
  that guardian or just someone who fights well."
- Post-battle: Extended scene — gives ITEM_BEAST_WHISTLE,
  ITEM_PRIMALIS_TOKEN, reveals oral history about
  kill switch and Haven Isle. Mentions player's parent
  sat here twelve years ago, wept, left without
  explaining.
- Gives: Elder Badge, TM for Outrage

---

## New Flags Required

Draw from STORY BLOCK 4 (0x282–0x2BB):

```c
FLAG_PRIMALIS_ARRIVED
FLAG_PRIMALIS_GYM1_CLEAR
FLAG_PRIMALIS_GYM2_CLEAR
FLAG_PRIMALIS_GYM3_CLEAR
FLAG_PRIMALIS_GYM4_CLEAR
FLAG_PRIMALIS_TRUST_EARNED
FLAG_PRIMALIS_ORAL_HISTORY_HEARD
FLAG_PRIMALIS_LENS_MET
FLAG_PRIMALIS_RUINS_FOUND
FLAG_PRIMALIS_SEAL_FOUND
FLAG_PRIMALIS_RESOLVED
FLAG_PRIMALIS_CIPHER_FOUND
FLAG_BEAST_WHISTLE_OBTAINED
FLAG_PRIMALIS_TOKEN_GIVEN
```

---

## New Variables Required

Confirm VAR_PRIMALIS_PROGRESS exists (0x4106).
No additional vars needed — 3 spares preserved.

VAR_PRIMALIS_PROGRESS: 0=not arrived, 1=arrived,
2=Gym1, 3=Gym2+Mako accessible, 4=Gym3,
5=Gym4+oral history, 6=seal reinforced, 7=resolved

---

## New Items Required

```c
ITEM_BEAST_WHISTLE    // Replaces Cut/Sweet Scent
                      // Clears dense undergrowth
ITEM_PRIMALIS_TOKEN   // "Elder Mako's mark. Proof of
                      //  trust from the Zoan community."
ITEM_SEAL_SHARD_PRIMALIS  // Grass/Dragon stub
```

---

## Wild Pokémon Encounters

### JungleRoute1
Common: Tropius (40%), Budew (30%)
Uncommon: Goomy (20%)
Rare: Bagon (10%)

### JungleInterior
Common: Tangela (40%), Exeggcute (30%)
Uncommon: Jangmo-o (20%)
Rare: Dratini — river only (10%)

### JungleRoute2
Common: Vibrava (40%), Steenee (30%)
Uncommon: Jangmo-o (20%)
Rare: Kartana (10%)

### TheHeartwood
Common: Zarude (50%)
Uncommon: Drampa (30%)
Rare: Goomy (20%)

No encounters in: VerdantLanding, ZoanVillage,
ElderHall, AncientRuinsCamp, SealChamber

---

## Trainer Data

TRAINER_ZOAN_PRIMALIS_1: Guardian Sera — JungleRoute1
Party: Tropius 46, Budew 47, Roserade 49
Pre: "You're going deeper. I need to know you
can handle what's deeper."
Post: "You can. Keep going."

TRAINER_ZOAN_PRIMALIS_2: Guardian Vex — JungleRoute1
Party: Bagon 47, Flygon 48, Haxorus 50
Pre: "The jungle tests everyone who enters.
I'm part of the test."
Post: "You passed this part."

TRAINER_ZOAN_PRIMALIS_3: Guardian Rael — JungleInterior
Party: Exeggcute 49, Tangrowth 50, Kommo-o 52
Pre: "The village is close. The elder doesn't want
visitors. I enforce that."
Post: "The elder will decide for himself."

TRAINER_ZOAN_PRIMALIS_4: Guardian Cael — JungleInterior
Party: Goomy 50, Goodra 51, Dragapult 53
Pre: "Four of us guard the approaches.
You've beaten three."
Post: "Welcome to Primalis."

---

## Key NPC Dialogue

Inn Lens (first meeting, with DEX flag variant):
"You made it. I've been here two weeks. The
community calls the legendary 'the Root.' Their oral
history is the clearest ancient civilization account
I've found. [if DEX dead: 'Dex would have loved this.'
Said quietly, moves on. if DEX alive: said warmly.]"

ZoanVillage fully-transformed member:
[No dialogue — just watches]
[Examine: "They look at you with recognition.
Something in their eyes is still there. Just stuck."]

Elder Mako first meeting (post-Gym2, pre-Gym4):
"Your parent sat in this hall twelve years ago.
I told them what I know. They wept. Then left
without explaining. I have wondered since what
they did with it. Prove yourself to Thorn.
Then we talk."

Oral history scene (post-Gym4):
Mako reveals: kill switch exists, Haven Isle is
the central node, ancient civilization built it
as a failsafe, Wardens are its caretakers.
"What did your parent do with this knowledge?"
Player cannot answer yet.
Mako: "Find out. Before someone else uses it."

AncientRuinsCamp altar:
"We did not make this. We found this. We chose
to stay. We chose to remember. We chose to guard
what we did not make because it was worth guarding."

Post-resolution fully-transformed member (recovering):
"I... [struggles] ...I remember. I remember who I
was. I'm still in here. I'm still in here."

Post-resolution Sollis call:
"Did you speak to Elder Mako?"
Player: "He told me about the kill switch. Haven Isle."
Sollis: [long pause] "I know. I've known since before
you were born. Your parent told me. We argued about
it for a year. Come home when you can. There are
things I should tell you in person. Things I should
have told you at the start."

---

## Cipher 8 Content

"Mako told me everything. I wept. The kill switch
was built by people who loved this world enough to
burn it down rather than let it be weaponized.
I understand that impulse. But there is a third option.
I found it. [encoded] ...Dorne doesn't know about
the third option. If he did, none of this would be
necessary. I need to find a way to tell him that
doesn't end with one of us dead. [encoded] ...I
don't think I'll find that way in time."

---

## Battle Terrain Setup

Outdoor routes: WEATHER_FOG_HORIZONTAL, Grassy Terrain
ZoanVillage: WEATHER_FOGGY
Heartwood/SealChamber: WEATHER_NONE

---

## Task Checklist

### pelagios-systems-engineer
- [ ] Primalis flags in flags.h (BLOCK 4)
- [ ] ITEM_BEAST_WHISTLE, ITEM_PRIMALIS_TOKEN,
      ITEM_SEAL_SHARD_PRIMALIS
- [ ] 4 gym leaders + 4 generic trainers
- [ ] Map group stub
- [ ] Confirm no var spares consumed
- [ ] Compile clean

### pelagios-map-builder
- [ ] All 13 maps
- [ ] Beast Whistle obstacle tiles on Route1/Route2
- [ ] Wild encounter tables (4 areas)
- [ ] Heal location (VerdantLanding Inn)
- [ ] Swap boat stub to real warp
- [ ] Dense jungle tileset (Fortree or closest)
- [ ] Compile after every 3 maps

### pelagios-script-writer
- [ ] Arrival script
- [ ] Beast Whistle field effect
- [ ] Lens inn meeting (DEX_ALIVE variant)
- [ ] Mako first meeting (post-Gym2)
- [ ] Oral history scene (post-Gym4, extended)
- [ ] Beast Whistle + token handoff
- [ ] Heartwood sequence
- [ ] Seal reinforcement + Zoan recovery
- [ ] Mako's line: "The Root remembers us."
- [ ] Resolution + Sollis cipher reveal
- [ ] Cipher 8
- [ ] All gym leader/NPC/trainer dialogue
- [ ] Post-resolution fully-transformed recovery scene
- [ ] Add all speakers to pelagios_speaker_names.inc

### pelagios-build-debugger
- [ ] Full compile clean
- [ ] Verify Beast Whistle field effect
- [ ] Verify oral history fires post-Gym4
- [ ] Verify trust flags set correctly
- [ ] Verify cipher 8 sets correctly
- [ ] Update CLAUDE.md

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
PRIMALIS_BRIEF.md. Thalvern and Gildhaven are complete
and compiling cleanly. Implement all Primalis constants
— flags from STORY BLOCK 4, items including
ITEM_BEAST_WHISTLE (replaces Cut/Sweet Scent),
ITEM_PRIMALIS_TOKEN (carved token for Final Island),
and ITEM_SEAL_SHARD_PRIMALIS stub, trainer entries
including four gym leaders (Fern, Scale, Thorn, Mako),
and map group stub. Do not consume var spares unless
absolutely necessary — confirm no additional vars needed
before adding any. Compile and fix errors.
Do not build maps or scripts.
```

---

*After Primalis: Ashenveil is next — heaviest narrative
island. Build sequentially, not in parallel.*
