# CONVERGENCE — ISLAND BRIEF (FINAL ISLAND)
## Pokémon Pelagios — Agent Build Document

---

## Prerequisites
- All prior islands complete ✅
- FLAG_AETHERON_RESOLVED set ✅
- FLAG_CASS_DEFECTED set ✅
- VAR_AETHERON_PROGRESS = 6 ✅
- ITEM_STORM_COMPASS obtained ✅
- VAR_CONVERGENCE_PROGRESS exists ✅
- Convergence in boat menu ✅

---

## Island Overview

| Property | Value |
|---|---|
| Island name | Convergence |
| Theme | The ancient capital — where everything was built, where everything ends |
| Types | All types — the convergence of all legendary energies |
| Legendary encounters | All legendaries (story only, not catchable) |
| Access | After Aetheron resolved |
| No gym leaders | No badges. No trainers. Pure story. |
| Three endings | Stop Dorne / Help Dorne / True Ending (liberation) |

---

## Narrative Summary

Convergence is the ancient civilization's capital — the island at the center
of the seal network, partially submerged, partially floating, partially
impossible. The kill switch is here, built into the foundation of the
civilization's greatest temple.

Everyone arrives here at the same time. The player and Cass. Dorne.
Sollis (who has finally come out from Haven Isle, carrying everything
she never said). The Covenant's fleet, offshore.

**What Convergence is:**
Not a dungeon. Not a final gym. A place where the game's questions
get answered and the player's choices determine the outcome.

**Three story paths converge here based on:**
- FLAG_DORNE_CHOICE_STOP / HELP / DEFER (set on Ashenveil)
- FLAG_TRUE_ENDING_UNLOCKED (all 9 ciphers decoded)
- VAR_DORNE_RELATIONSHIP (affects Dorne's dialogue)
- VAR_CASS_RELATIONSHIP (affects Cass's final role)
- FLAG_DRENN_ALIVE (affects who arrives)
- FLAG_DEX_ALIVE (affects who arrives)

**Who arrives at Convergence:**
- Player + Cass (always)
- Dorne (always — either as ally, adversary, or uncertain)
- Sollis (always — she has things to say)
- Eira (if FLAG_SCHISM_CEASEFIRE — with ice fighters)
- Lace Vane (always — running honest supplies)
- The Lens (always — continuing Dex's work)
- Dex (if FLAG_DEX_ALIVE — weakened but present)
- Drenn (if FLAG_DRENN_ALIVE — with poison fighters)
- Elder Mako (always — with Zoan fighters)
- Dagan (always — because of course he's here)

**The Covenant fleet is offshore.** They know what's here.
They're waiting to see who wins so they can deal with the winner.

---

## Map Group

```
MAP_GROUP_CONVERGENCE:
  - Convergence_Approach
  - Convergence_AncientCapital_Exterior
  - Convergence_AncientCapital_Interior1
  - Convergence_AncientCapital_Interior2
  - Convergence_KillSwitchChamber
  - Convergence_Epilogue
```

---

## Map Descriptions

### Convergence_Approach
- The sea approach — Covenant fleet visible offshore
- The island rises from the water impossibly —
  architecture from multiple eras, partly submerged,
  partly floating
- Player arrives by Storm Compass (air) or Tennyson
- Cass arrives alongside
- Dorne is already here — waiting at the shore
- Brief scene: Dorne, Cass, and player at the shore
  Before anything else is said, Sollis arrives
  (by boat — she came from Haven Isle)
- This is the gathering point before the final push
- No wild Pokémon
- No inn — no turning back from here

### Convergence_AncientCapital_Exterior
- The ruins of the greatest city ever built
- Scale is overwhelming — everything else was
  a fragment of this
- All the inscription styles the player has seen
  across every island converge here
- NPCs from every island are positioned here:
  Eira at the north edge, Mako at the east,
  Drenn (if alive) at the south, Lace at the west
  The Lens cataloguing everything
  Dagan leaning against a wall, eating something
- Path to the temple visible ahead
- The kill switch is inside the temple

### Convergence_AncientCapital_Interior1
- The temple's outer chamber
- The full story of the ancient civilization
  told in murals across every wall
- Examine any mural for extended lore text
  (the full history, readable only now with
  all ciphers — if FLAG_TRUE_ENDING_UNLOCKED,
  the hidden sections are also readable)
- Dorne is here — the confrontation begins
- The player's choice from Ashenveil determines
  what this confrontation looks like

### Convergence_AncientCapital_Interior2
- The inner sanctum
- The kill switch visible ahead — an enormous
  ancient mechanism, still functional
- Dorne explains what it does (again, clearly)
- The player makes the final choice here
  (or confirms the choice from Ashenveil)

### Convergence_KillSwitchChamber
- The kill switch itself
- Ancient, vast, clearly built to last forever
- Each of the three endings plays out here
- The legendaries respond from their islands —
  their cries audible even here
- This is where the game ends

### Convergence_Epilogue
- Post-ending epilogue map
- Varies by ending (see Endings section)
- Fade in on a specific location per ending
- Brief final scenes with key characters
- The last thing the player sees before credits

---

## The Gathering Scene (Approach)

After the player arrives, this scene plays:

Dorne is at the shore when the player lands.
He doesn't seem surprised.
Dorne: "You made it. All of them?"
Player: [nods / yes]
Dorne: "Good."

A boat arrives from the south.
Sollis steps off.

Long pause. Dorne and Sollis look at each other.
They clearly know each other well.

Sollis: "Vael."
Dorne: "Maren."
Sollis: "I should have come sooner."
Dorne: "Yes."

Sollis turns to the player.
Sollis: "There are things I should have told you
 from the beginning. I'm going to tell you now.
 [to Dorne]
 All of it."
Dorne: "All of it."

Sollis explains (extended dialogue):
- She knew about the kill switch from the start
- She and the Warden argued about it for years
- She agreed to keep it secret because the Warden
  believed in the third option
- She didn't tell the player because she was afraid
  they weren't ready
- She's not sure she was right to wait
- She hands the player the Warden's original
  research notes (lore item — completes the cipher)

After this scene: everyone moves toward the temple.

---

## The Three Endings

### ENDING 1: Stop Dorne
**Trigger:** FLAG_DORNE_CHOICE_STOP set
**How it plays:**

Interior2: Dorne activates the kill switch mechanism.
Player battles Dorne (the game's final battle).
Dorne: "You fight like your parent.
 They would have done the same thing."

**Dorne Battle:**
- Level 65-70 team, full ace trainer difficulty
- His final Pokémon: a level 70 legendary
  (use Zekrom or Kyurem as placeholder)
- After defeat: Dorne steps back from the switch

Dorne: "So. You stop me.
 The Covenant wins.
 The seals hold — for now.
 Until they find another way."
Player: [says nothing]
Dorne: "...Your parent would be proud of you.
 I'm not sure if that helps."

Dorne leaves — not arrested, not dead. Just done.

The Covenant fleet begins to withdraw —
they got what they wanted (status quo preserved).

The legendaries remain sealed.
The Covenant remains in power.
The seals are stable but the exploitation continues.

**Epilogue:** Haven Isle. The player returns home.
Sollis is there. Brief conversation.
Sollis: "You stopped him. The seals are stable.
 Is that enough?"
Player: [no answer given]
Credits roll on the player walking to the harbor,
looking out at the sea.

---

### ENDING 2: Help Dorne
**Trigger:** FLAG_DORNE_CHOICE_HELP set
**How it plays:**

Interior2: Dorne and player approach the kill switch
together.

Dorne activates it. The legendaries' cries from
every island — simultaneous, overwhelming.

The seal network collapses.
Every legendary is freed — but without the seals,
their power is uncontrolled. Islands destabilize.

Dorne: "It's done. The Covenant can't use them now."
Player: "What happens to them?"
Dorne: "They're free. What they do with that —"
[a massive tremor]
"— is their choice."

Cass: "The Covenant fleet is moving. They're coming in."
Dorne: "Let them. There's nothing to exploit now."

The Covenant fleet arrives — but without the seal
network to exploit, they have no purpose here.
They anchor offshore. Do nothing.

The legendaries emerge — briefly visible from the
island, shapes in the sky and sea, massive and free.
Then gone. Into the world.

**Epilogue:** The world is changed. The legendaries
are free but unpredictable. The Covenant's power
is broken but the region is destabilized.
Dorne looks at the empty seal chamber.
Dorne: "I thought this would feel different."
Cass: "Does it feel wrong?"
Dorne: "No. Just... quieter than I expected."
Credits roll on the legendaries visible in the
distance — free, vast, unreachable.

---

### ENDING 3: True Ending (Liberation)
**Trigger:** FLAG_TRUE_ENDING_UNLOCKED set
(All 9 ciphers decoded — Warden's third option)
**How it plays:**

This ending is only available if the player decoded
all 9 ciphers. The option appears in Interior2.

Player examines the kill switch mechanism.
A third function is revealed — hidden, dormant,
built by the same people who built the switch.

Player: [describes the third function to Dorne]
Dorne: "[reads the inscription]
 ...Liberation. Not destruction.
 The seals dissolve — but the legendaries aren't
 freed into chaos. They're... integrated.
 Part of the world, but stable. Protected.
 [long pause]
 Your parent found this."
Player: "Yes."
Dorne: "And didn't tell me."
Player: "They didn't have time."

Dorne looks at the mechanism for a long moment.
Dorne: "If the seals are all fully reinforced —
 which they are, because of you —
 this function works differently.
 The network doesn't collapse.
 It dissolves cleanly.
 The legendaries remain in the world,
 stable, present, and beyond exploitation.
 [pause]
 Your parent was right.
 I was going to burn everything down
 when I could have just... opened the door."

Player and Dorne activate the third function
together. The mechanism requires a Warden's touch.

The seal network dissolves — not violently,
but like morning fog clearing.
Every legendary becomes part of the world —
present but uncapturable, known but ungovernable.
The Covenant's exploitation becomes impossible.
Not because the seals are broken, but because
there's nothing to exploit anymore.

The Covenant fleet offshore. They watch.
One by one their ships turn and leave.
There's nothing here for them now.

**Epilogue:** All the characters on the island.
Sollis, Cass, Dorne, Eira, Mako, The Lens,
Dex (if alive), Drenn (if alive), Lace, Dagan.
Brief final lines from each:

Sollis: "Your parent would have loved this ending."
Cass: "I almost didn't make it in time."
Dorne: "Twenty years. A wrong answer and a right one.
 I'm glad I didn't get to the wrong one first."
Eira: [says nothing. Nods once. Enough.]
Mako: "The Root will remember this too."
The Lens: "I have so much to write down."
Dex (if alive): "I need to see all of it.
 Every island. Start over with what we know now."
Drenn (if alive): "Don't tell Eira I'm crying."
Lace: "What happens now?"
Dagan: "I have absolutely no idea.
 [beat]
 It's the most interesting I've felt in years."
Cass (final line): "[Player name].
 Come on. Let's go home."

Credits roll on the legendaries visible everywhere —
in the sky, in the sea, on the horizon.
Present. Free. Part of the world.

---

## Key NPCs at Convergence

Each NPC has dialogue reflecting their island arc
and the relationship the player built with them.

**Sollis:**
Multiple dialogue variants based on VAR_MAREN_RELATIONSHIP.
She apologizes. She explains. She stays.

**Eira:**
Present if FLAG_SCHISM_CEASEFIRE.
One or two lines maximum. She's not a talker.
"I brought fighters. In case."

**Mako:**
"The Root sent us here. I didn't know until I
arrived and understood."

**The Lens:**
"This is everything Dex was looking for.
[if DEX_ALIVE: Dex is going to lose his mind.]
[if DEX dead: I'll find a way to tell him.
Even if I have to write it on the wall.]"

**Dex (if alive):**
Visibly weakened from Thalvern but present.
Carried his notebooks. Taking notes right now.
"I'm fine. I'll sleep when this is over."

**Drenn (if alive):**
"Eira and I haven't spoken since the Scar.
[pause]
We're not going to speak here either.
But we're both here.
That seems like something."

**Lace:**
Running supplies to whoever needs them.
"Someone has to make sure everyone has water.
It might as well be me."

**Dagan:**
Somewhere he shouldn't be, eating something.
"What? I was invited. Mostly."
VAR_DAGAN_RELATIONSHIP affects his specific lines.

---

## Dorne's Final Battle (Ending 1 Only)

If FLAG_DORNE_CHOICE_STOP:

```
TRAINER_LEADER_DORNE:
  Name: "Dorne"
  Party:
    Bisharp Lv.65
    Hydreigon Lv.66
    Weavile Lv.67
    Aegislash Lv.68
    Garchomp Lv.69
    [Legendary placeholder] Lv.70
Pre-battle: "You fight like your parent.
 They would have done the same thing."
Post-battle: "So. You stop me.
 The Covenant wins.
 The seals hold — for now."
```

---

## New Flags Required (STORY BLOCK 4)

```c
FLAG_CONVERGENCE_ARRIVED
FLAG_CONVERGENCE_GATHERING_SEEN  // Gathering scene played
FLAG_SOLLIS_CONFESSION_HEARD     // Sollis told everything
FLAG_DORNE_FINAL_BATTLE_DONE     // Ending 1 battle complete
FLAG_ENDING_STOP_PLAYED          // Ending 1 played
FLAG_ENDING_HELP_PLAYED          // Ending 2 played
FLAG_ENDING_TRUE_PLAYED          // Ending 3 played
FLAG_CONVERGENCE_COMPLETE        // Any ending played
```

---

## New Variables Required

Confirm VAR_CONVERGENCE_PROGRESS exists (0x4109).
No new vars needed. 3 spares preserved.

VAR_CONVERGENCE_PROGRESS:
0=not arrived, 1=arrived+gathering, 2=temple entered,
3=inner sanctum, 4=kill switch chamber, 5=ending played

---

## New Items Required

```c
ITEM_WARDENS_RESEARCH    // Sollis gives this at gathering
                         // "Your parent's complete research.
                         //  The third option, fully documented."
                         // Examine for extended lore text
```

---

## Wild Pokémon

No wild Pokémon on Convergence.
This is a story-only island.

---

## Task Checklist

### pelagios-systems-engineer
- [x] Convergence flags in flags.h (BLOCK 4, 0x29F-0x2A6, 8 flags)
- [x] ITEM_WARDENS_RESEARCH (903)
- [x] TRAINER_LEADER_DORNE (940, Ending 1 only; party verbatim from brief)
- [x] Map group stub (6 maps) — DEFERRED to map-builder (empty groups break groups.inc; documented handoff)
- [x] Compile clean (gmake exit 0, 2026-06-14)

### pelagios-map-builder
- [ ] All 6 Convergence maps
- [ ] Approach — shore gathering space, Covenant
      fleet implied (bg detail)
- [ ] AncientCapital_Exterior — large open space,
      NPC positions for all returning characters
- [ ] Interior1 — mural walls (bg_events)
- [ ] Interior2 — kill switch visible ahead
- [ ] KillSwitchChamber — the mechanism itself,
      space for three ending sequences
- [ ] Epilogue — varies by ending (use single map
      with flag-gated NPC positions)
- [ ] No heal location — no inn on Convergence
- [ ] No wild encounters
- [ ] No boat registration needed — Storm Compass
      handles return travel
- [ ] Compile after every 2 maps

### pelagios-script-writer
THIS IS THE MOST IMPORTANT SCRIPT PASS.
Take more time than any prior island.

- [ ] Gathering scene (Approach) — full
      choreography, Dorne + Sollis reunion,
      Sollis confession, all returning NPCs
      arrive and take positions
- [ ] All returning NPC dialogue — every
      character from every island, relationship-
      variable-aware variants
- [ ] Interior1 mural examinations (full lore)
- [ ] Interior2 Dorne confrontation — branches
      based on FLAG_DORNE_CHOICE_*
- [ ] ENDING 1: Stop Dorne
      Dorne battle, post-battle dialogue,
      Dorne leaving, Covenant withdrawal,
      Epilogue scene on Haven Isle
- [ ] ENDING 2: Help Dorne
      Kill switch activation, legendary cries,
      Covenant fleet arrival and withdrawal,
      Dorne's quiet final line, Epilogue scene
- [ ] ENDING 3: True Ending (Liberation)
      Third function discovery, player+Dorne
      activation, network dissolution,
      Covenant fleet leaving, full epilogue
      with ALL returning characters' final lines
- [ ] Credits trigger after each ending epilogue
- [ ] Post-credits: player can return to all
      islands via Storm Compass (post-game)

CRITICAL WRITING NOTES:
- The gathering scene is the emotional payoff
  of the entire game. Every character the player
  met is here. Write each appearance as a reward
  for the player having met them.
- Dorne's post-battle line in Ending 1:
  "Your parent would be proud of you.
   I'm not sure if that helps."
  Do not change this line.
- Ending 3's final epilogue line is Cass:
  "[Player name]. Come on. Let's go home."
  This is the last line of dialogue in the game.
  Do not change it. Do not add anything after it.
- Dagan must be funny in all three endings.
  He has survived the entire game by being
  charming and slightly somewhere he shouldn't be.
  Honor that.
- Credits roll on:
  Ending 1: player at the harbor, looking out
  Ending 2: legendaries visible in the distance
  Ending 3: legendaries everywhere, Cass beside
             the player, walking toward the harbor

### pelagios-build-debugger
- [ ] Full compile clean
- [ ] Verify all three ending flags set correctly
- [ ] Verify Ending 3 only available if
      FLAG_TRUE_ENDING_UNLOCKED is set
- [ ] Verify all returning NPC appearances
      gated correctly by their flags
- [ ] Verify Dorne battle only in Ending 1
- [ ] Verify credits trigger after each epilogue
- [ ] Verify Storm Compass works post-game
      for island exploration
- [ ] Full ROM size check — confirm under 32MB
- [ ] Update CLAUDE.md — mark game COMPLETE

---

## Prompt to Start

```
use pelagios-systems-engineer: Read CLAUDE.md and
CONVERGENCE_BRIEF.md. Aetheron is complete and
compiling cleanly. FLAG_CASS_DEFECTED is set.
Implement all Convergence constants — flags from
STORY BLOCK 4, ITEM_WARDENS_RESEARCH (lore item
given by Sollis at gathering), TRAINER_LEADER_DORNE
(Ending 1 only — level 65-70 team), and map group
stub (6 maps). Confirm VAR_CONVERGENCE_PROGRESS
exists (0x4109) and reuse it. No new vars needed.
No inn, no wild encounters, no boat registration
needed on this island. Compile and fix errors.
Do not build maps or scripts.
```

---

## After Convergence

The game is complete. Post-launch tasks:
- mGBA playtest of all islands (substantial backlog)
- Following Pokémon cutscene hide flags audit
- Key item field effects (Navigator's Log etc.)
- Mega Evolution system wiring (Seal Shards)
- Custom title screen
- Island Journal UI (replaces Trainer Card)
- Visual polish pass (Porymap tile painting)
- Difficulty modes
- Consider Essentials port for enhanced visuals
  (CrossOver on Mac — all briefs transfer directly)
