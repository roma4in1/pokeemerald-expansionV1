# ASHENVEIL — ISLAND BRIEF (DEAD ISLAND)
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
- Schism + Thalvern + Gildhaven all complete ✅
- Ashenveil unlocked in boat menu ✅
- VAR_ASHENVEIL_PROGRESS exists ✅
- STORY BLOCK 4 available ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Ashenveil (Dead Island) |
| Theme | Grief, guilt, history, silence. No active crisis. Pure story. |
| Types | Ghost / Dark |
| Terrain | Fog, permanent dusk, ghost battle backgrounds |
| Legendary | Morthas (Ghost/Dark) — memory and loss, endurance battle |
| Boat required | Galleon |
| Key items | ITEM_PHANTOM_LANTERN, ITEM_SEA_CHART, ITEM_SEAL_SHARD_ASHENVEIL |
| Critical scenes | Dorne appears. Full truth. Major story choice. Cipher 9. |

---

## Narrative Summary

Ashenveil is dead. Twenty years ago the Covenant deliberately botched
the evacuation — 847 people, 400 boat capacity, 401 survivors recorded,
446 unaccounted for in a file that closes without explanation.

The island is empty except for researcher Orin (sole outpost occupant)
and Dorne, who visits his daughter's grave here.

Cass was here before the player — sent by the Covenant to find and
destroy the sea chart leading to Aetheron. Cass found it, found the
real Covenant documents, hid the sea chart, kept the documents, left
a copy in the ruins in case something happens.

Dorne tells the player everything at the memorial. The player chooses.

No gym leaders. No trainers. No inn. The island's challenge is entirely
narrative. The sea chart is the reward.

Key beats:
1. Greyport — Orin gives ITEM_PHANTOM_LANTERN
2. Ash fields — hidden paths, environmental details
3. Dead city — official report (ruins1), real documents (ruins2)
4. The Memorial — DORNE SCENE + player choice
5. MorthasGrove — find sea chart (Cass left it)
6. Morthas endurance encounter (5 turns)
7. Final vision — six seconds, no music
8. ITEM_SEAL_SHARD_ASHENVEIL + Cipher 9
9. FLAG_ASHENVEIL_VISITED + Aetheron unlocks

---

## Map Group

```
MAP_GROUP_ASHENVEIL:
  - Ashenveil_Greyport
  - Ashenveil_Greyport_Outpost
  - Ashenveil_Greyport_Outpost_Interior
  - Ashenveil_AshFields1
  - Ashenveil_DeadCity_Exterior
  - Ashenveil_DeadCity_Ruins1
  - Ashenveil_DeadCity_Ruins2
  - Ashenveil_TheMemorial
  - Ashenveil_MorthasGrove
```

---

## Map Descriptions

### Greyport
- Tiny ruined port, permanent dusk, ash falls
- Small research outpost is the only structure
- Orin NPC — sole occupant
- No inn — player cannot heal here (intentional)
- Connects to AshFields1
- Weather: WEATHER_FOG_HORIZONTAL + dusk palette

### Greyport Outpost / Interior
- Orin's research notes (examine — the math:
  847 people, 400 capacity, no follow-up report)
- ITEM_PHANTOM_LANTERN given here
- Environmental detail: half-finished cup of tea,
  still faintly warm (Cass was here)

### AshFields1
- Hidden paths — blocked without Phantom Lantern
- Environmental bg_events:
  - Child's shoe (red)
  - Door handle (brass, polished)
  - Street sign (RIVER LANE)
  - Dorne's footprints (recent, heading to memorial)
- Wild Ghost Pokémon
- Connects Greyport to DeadCity_Exterior
- Weather: WEATHER_FOG_HORIZONTAL

### DeadCity_Exterior
- Full ruined city — scale is striking
- Ghost Pokémon
- Cass's tracks visible (went to memorial,
  then MorthasGrove, then back)
- Connects to Ruins1, Ruins2, TheMemorial

### DeadCity_Ruins1
- Official Covenant evacuation report (sanitized):
  "geological instability / all residents accounted for
   / casualties: none"

### DeadCity_Ruins2
- Real Covenant documents — Cass's copy:
  "seal-siphoning caused instability / evacuation
   deliberately under-provisioned / decision made at
   highest level / classify and suppress"
- FLAG_ASHENVEIL_COVENANT_DOCS_FOUND set

### TheMemorial
- Small garden, only green on the island
- Someone maintained it for twenty years (Dorne)
- Simple grave markers — one newer than others
- DORNE SCENE fires on entry (see below)
- After Dorne leaves: examine newest marker:
  "VAEL DORNE (JUNIOR) — Born on this island.
   She would have been twenty-three."

### MorthasGrove
- Cathedral of ash-preserved dead trees
- Dark pool at center — Morthas's seal
- Examine stone near tree line → ITEM_SEA_CHART:
  "A waterproof case. Inside: a sea chart.
   'The stream rises here.' An X on the open sea.
   Below, different handwriting:
   'In case something happens to me. — C'"
- Morthas encounter triggers approaching the pool
- Wild Pokémon (pre-encounter only)

---

## THE DORNE SCENE (TheMemorial)

Most important scene in the game. Write with care.

Dorne at his daughter's grave when player arrives.
He was expecting this. He turns and speaks.

Content (in order):
1. What the Covenant did — deliberate evacuation
   failure, covered-up death count, suppressed reports
2. What the kill switch is — the ancient civilization's
   failsafe against exactly what the Covenant is doing
3. The truth about the player's parent:
   "Your parent didn't die because of me.
    We disagreed. They refused. I respected that.
    Three months later they were dead. The Covenant
    intercepted their message about stopping my plan.
    A living Warden who knew what they knew was more
    dangerous to the Covenant than I was.
    I didn't kill your parent.
    But I knew it was coming and I didn't warn them.
    That's what I carry."
4. His offer:
   "I'm going to use the kill switch.
    Either you're there to make sure I do it right
    or you're there to stop me.
    Either way ends the Covenant's ability to exploit.
    The difference is what happens to the legendaries."

Player choice:
- "I'll stop you." → FLAG_DORNE_CHOICE_STOP
  Dorne: "I know. That's what your parent would
  have done. I still respect it."

- "I'll help you." → FLAG_DORNE_CHOICE_HELP
  Dorne: "Good. The kill switch requires a Warden's
  activation. I couldn't have used it alone."
  VAR_DORNE_RELATIONSHIP = maximum

- "I need time." → FLAG_DORNE_CHOICE_DEFER
  Dorne: "Take it. I'll be at Haven Isle.
  I've waited twenty years. I can wait longer."

After choice — Dorne's final lines:
"Her name was Vael. Same as mine. Her mother's idea.
 She was four years old."
He leaves. No dramatic exit. Just leaves.

---

## MORTHAS ENCOUNTER

Not a standard battle. Scripted endurance sequence.

5-turn sequence:
- Each turn: vision text (impressionistic memory of
  the island's destruction)
- Player choice each turn: "Endure." / "Turn away."
- "Turn away" = fled, restart
- 5 endured = Morthas settles

Final vision (turn 5 aftermath):
Stop music completely. Show text:
"You see the island before it died.
 Not ruins. A city. Morning light.
 People going about their day — a baker opening
 a stall, two children arguing over something
 small and important, a fisherman checking his nets.
 A woman hanging laundry and humming a song
 you don't recognize.
 None of them know what is coming.
 The vision lasts six seconds.
 There is no music.
 Then it's over."
Resume music.

After encounter:
- ITEM_SEAL_SHARD_ASHENVEIL given
  (Decidueye Mega Evolution trigger)
- FLAG_MORTHAS_ENCOUNTERED set

---

## New Flags Required (STORY BLOCK 4)

```c
FLAG_ASHENVEIL_ARRIVED
FLAG_ASHENVEIL_OUTPOST_MET
FLAG_PHANTOM_LANTERN_OBTAINED
FLAG_ASHENVEIL_COVENANT_DOCS_FOUND
FLAG_ASHENVEIL_DORNE_MET
FLAG_DORNE_CHOICE_STOP
FLAG_DORNE_CHOICE_HELP
FLAG_DORNE_CHOICE_DEFER
FLAG_SEA_CHART_FOUND
FLAG_MORTHAS_ENCOUNTERED
FLAG_ASHENVEIL_VISITED
FLAG_ASHENVEIL_CIPHER_FOUND
```

Note: FLAG_DORNE_CHOICE_* should already exist
from initial constants setup. Reuse, don't duplicate.

---

## New Variables Required

Confirm VAR_ASHENVEIL_PROGRESS exists (0x4107).
Confirm VAR_DORNE_RELATIONSHIP exists.
No new vars needed. 3 spares preserved.

VAR_ASHENVEIL_PROGRESS:
0=not arrived, 1=arrived, 2=lantern obtained,
3=dead city reached, 4=Dorne met, 5=sea chart found,
6=Morthas encountered, 7=visited/complete

---

## New Items Required

```c
ITEM_PHANTOM_LANTERN   // Replaces Defog/Flash in caves
                       // "Reveals hidden paths in fog
                       //  and darkness."

ITEM_SEA_CHART         // "A chart showing a location in
                       //  the open sea. A notation reads:
                       //  'The stream rises here.'"
                       // Activates Aetheron in boat menu

ITEM_SEAL_SHARD_ASHENVEIL  // Decidueye Mega trigger
                            // "Crystallized memory from
                            //  Morthas's seal. Cold and still."
```

---

## Wild Pokémon

### AshFields1
Gastly (40%), Shuppet (30%), Duskull (20%), Honedge (10%)

### DeadCity_Exterior
Misdreavus (40%), Sableye (30%), Runerigus (20%),
Cursola (10%)

### MorthasGrove (pre-encounter)
Phantump (50%), Spiritomb (30%), Dragapult (20%)

No encounters: Greyport, Outpost, Ruins1/2,
TheMemorial, MorthasGrove post-encounter

---

## NPC Dialogue

### Orin (sole researcher)
"You shouldn't be here. Nobody comes here.
 [looks at player] But you're here. And you have
 that look. Like you already know why they said
 it was uninhabitable. Come inside. I'll give
 you the lantern."

### Orin (on staying)
"Why do I stay? Because someone should.
 Because 847 people lived here and the official
 record says they all evacuated safely and they
 didn't and someone should stay and know that.
 Even if knowing doesn't change anything."

### Orin (Cass detail)
"Someone else was here two days ago. Young.
 Covenant uniform. Took something from the
 memorial area. Left something in the grove.
 Didn't speak to me. Just worked and left.
 They looked troubled."

### AshFields examine scripts
Child's shoe: "A small shoe. Red. The kind that
 wears out fast because children run."

Door handle: "A brass handle. Someone polished
 this regularly. Force of habit."

Street sign: "RIVER LANE. There is no river here.
 There wasn't one when this was built either.
 Someone named it hopefully."

Dorne footprints: "Fresh tracks in the ash.
 Heading toward the memorial. Purposeful. Not lost."

### Official report (Ruins1)
"ASHENVEIL INCIDENT — CLASSIFIED
 Cause: unexpected geological instability.
 Evacuation: completed, all residents accounted for.
 Casualties: none.
 Status: uninhabitable, access restricted."

### Real documents (Ruins2)
"ASHENVEIL — INTERNAL MEMO
 The geological instability was caused by our
 seal-siphoning apparatus. The evacuation boats
 were deliberately under-provisioned. The decision
 was made at the highest level. The survivors
 who reached the receiving port have been relocated
 and the incident classified. The Warden's field
 reports from this site have been suppressed.
 This memo is classified at highest level.
 Destroy after reading."
[It was not destroyed. Copied by Cass. Left here.]

### Post-Morthas atmosphere
The grove is still. The ash falls. Morthas's form
is barely visible in the pool — present but settled.

---

## Cipher 9 Content (Final Journal Entry)

This is the emotional climax of the entire game.

"If you're reading this, you decoded everything.
 That means you're ready.
 The kill switch has a third option.
 Dorne doesn't know. I found it in Thalvern's
 Throne Room — in the vision.
 The civilization built the kill switch to destroy
 the legendary network. But they also built a
 second function. A release.
 Not destruction. Liberation.
 If the seals are reinforced — all of them, stable —
 the kill switch doesn't destroy. It frees.
 The legendaries remain in the world but the seal
 network dissolves. No one can exploit them again.
 No one can use the kill switch against them again.
 I couldn't tell Dorne. He would have used it
 before the seals were ready.
 I couldn't tell anyone.
 I'm telling you now.
 The Covenant is moving faster than I expected.
 I don't have time to do this myself.
 I'm sorry I'm leaving it to you.
 I love you. I should have said that more.
 I love you."

After cipher 9:
- FLAG_CIPHER_9_FOUND set
- FLAG_TRUE_ENDING_UNLOCKED set
  (enables the third ending option at Final Island)

---

## Battle Terrain

All maps: WEATHER_FOG_HORIZONTAL + dark/dusk palette
MorthasGrove: darkest palette in the game

---

## Task Checklist

### pelagios-systems-engineer
- [ ] Ashenveil flags in flags.h (BLOCK 4)
      Check FLAG_DORNE_CHOICE_* already exist —
      reuse, do not duplicate
- [ ] ITEM_PHANTOM_LANTERN, ITEM_SEA_CHART,
      ITEM_SEAL_SHARD_ASHENVEIL
- [ ] NO trainer entries
- [ ] NO gym leaders
- [ ] Map group stub
- [ ] Aetheron boat menu stub (activates via
      FLAG_SEA_CHART_FOUND not boat tier)
- [ ] No var spares consumed
- [ ] Compile clean

### pelagios-map-builder
- [ ] All 9 maps
- [ ] Phantom Lantern obstacle tiles in AshFields1
- [ ] Environmental bg_events in AshFields1
      (shoe, handle, sign, footprints)
- [ ] Grave markers in TheMemorial as bg_events
- [ ] Sea chart stone in MorthasGrove as bg_event
- [ ] Wild encounter tables (3 areas)
- [ ] NO heal location
- [ ] Dusk/dark palette for all maps
- [ ] Swap Ashenveil boat stub to real warp
- [ ] Compile after every 2 maps

### pelagios-script-writer
- [ ] Arrival script + immediate silence atmosphere
- [ ] Orin inn meeting + lantern handoff
- [ ] Phantom Lantern field effect (AshFields1)
- [ ] AshFields environmental examine scripts (4)
- [ ] Dorne footprints examine
- [ ] Official report (Ruins1)
- [ ] Real documents + FLAG set (Ruins2)
- [ ] DORNE SCENE — full implementation
      lockall / extended dialogue / player choice /
      VAR_DORNE_RELATIONSHIP update / final line /
      quiet exit / releaseall
- [ ] Memorial grave examine scripts
- [ ] Sea chart discovery script
- [ ] Morthas encounter (5-turn endurance)
- [ ] Final vision (STOP MUSIC / text / RESUME)
- [ ] ITEM_SEAL_SHARD_ASHENVEIL award
- [ ] Cipher 9 — full journal entry text
- [ ] FLAG_TRUE_ENDING_UNLOCKED after cipher 9
- [ ] Aetheron activation via ITEM_SEA_CHART
- [ ] FLAG_ASHENVEIL_VISITED set
- [ ] Orin full dialogue
- [ ] Add Dorne and Orin to speaker names
      Morthas has no setspeaker (no dialogue)

CRITICAL: The Warden's final journal entry ends
with "I love you. I should have said that more.
I love you." Do not change these lines.
Do not add anything after them.
The script ends there.

### pelagios-build-debugger
- [ ] Full compile clean
- [ ] Verify Phantom Lantern field effect
- [ ] Verify all three Dorne choice flags
- [ ] Verify VAR_DORNE_RELATIONSHIP updates
- [ ] Verify sea chart activates Aetheron
- [ ] Verify Morthas encounter resolves
- [ ] Verify cipher 9 + FLAG_TRUE_ENDING_UNLOCKED
- [ ] Verify FLAG_ASHENVEIL_VISITED
- [ ] Update CLAUDE.md

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
ASHENVEIL_BRIEF.md. Primalis is complete and
compiling cleanly. Ashenveil has NO gym leaders
and NO trainers — do not add any. Implement:
flags from STORY BLOCK 4 (check FLAG_DORNE_CHOICE_*
already exist and reuse them), items including
ITEM_PHANTOM_LANTERN (replaces Defog/Flash),
ITEM_SEA_CHART (unlocks Aetheron — activates via
FLAG_SEA_CHART_FOUND not boat tier), and
ITEM_SEAL_SHARD_ASHENVEIL (the Decidueye Mega
Evolution trigger from the story bible). Update
boat menu to add Aetheron stub entry. Map group
stub. No var spares consumed. Compile and fix errors.
Do not build maps or scripts.
```

---

*After Ashenveil: Aetheron (Sky Island) — then
Convergence (Final Island). The end of the game.*
