---
name: convergence-gotchas
description: Final-island ending scripts — TRUE-first ending gate, GameClear credits mechanism, spawn-gated epilogue roster, exact-verbatim final line, legendary-cry roster
metadata:
  type: project
---

Convergence (FINAL ISLAND) ending-script patterns, verified building 2026-06-14.

**TRUE ending overrides — check order matters.** At the KillSwitchChamber Ending
trigger (VAR_CONVERGENCE_PROGRESS==3), `goto_if_set FLAG_TRUE_ENDING_UNLOCKED` MUST
come BEFORE the FLAG_DORNE_CHOICE_STOP/HELP branches. Interior1 sets _STOP or _HELP
(DEFER resolves into one; the "true way" posture deliberately sets _HELP so Dorne
walks in alongside the player) — so without the TRUE-first check a true-ending player
would wrongly fall into Help. Same order in the Epilogue (TRUE, then HELP, else STOP).

**Credits = `special GameClear`** after `fadescreenspeed FADE_TO_BLACK, 24` (the
EverGrandeCity_HallOfFame end path, src/post_battle_event_funcs.c). It heals party,
records Hall of Fame, rolls FRLG credits, sets FLAG_SYS_GAME_CLEAR, and returns the
player to their last heal location for post-game. Set FLAG_CONVERGENCE_COMPLETE
IMMEDIATELY BEFORE GameClear so it persists. Storm Compass (bag) is the post-game
travel mechanism — do NOT lock the player.
**Why:** vanilla-consistent, gives the Hall-of-Fame record for free, auto-returns to
field. **How to apply:** any future "roll credits from a script" wants this exact pair.

**Spawn-gated epilogue roster -> fully narrative cutscene.** The Epilogue's named NPC
objects are all spawn-gated behind FLAG_CONVERGENCE_COMPLETE (map-builder choice), so
they are HIDDEN during the ending cutscene (the flag isn't set until just before
GameClear) and APPEAR for post-game flavor talk afterward. Consequence: the ending
cutscene is told ENTIRELY via setspeaker + msgbox narration/speech — NO object
movement/choreography, because the actors aren't on-screen yet. This is clean and
avoids addobject staging. If richer blocking is ever wanted, force-spawn via addobject.

**Exact-verbatim lines are load-bearing — do not paraphrase.** The brief/prompt pin
several lines word-for-word: Dorne Ending-1 post-battle "You fight like your parent.\p
They would have done the same thing."; Dorne TRUE "Your parent was right.\pI was going
to burn everything down when I could have just opened the door."; Dagan ALWAYS ends on
"It's the most interesting I've felt in years." (3 VAR_DAGAN_RELATIONSHIP variants, all
land on that line); and THE FINAL LINE OF THE GAME — Cass "{PLAYER}. Come on.\pLet's go
home." with NOTHING after it, credits immediately. Use `{PLAYER}` buffer, not a literal.

**Legendary-cry roster (Ending 2)** — reuse each island seal chamber's EXACT placeholder
`playmoncry SPECIES_*` (grep prior SealChamber/Summit/ThroneRoom scripts):
Thalvern=KYOGRE, Emberveil=GROUDON, Primalis=RAYQUAZA, Schism-north=REGICE,
Schism-south=REGIDRAGO, Aetheron=ZAPDOS, Sirocco=REGIROCK, Ironhold=REGISTEEL,
Gildhaven=GARDEVOIR. Ashenveil's seal chamber is SILENT (no cry). The two final-island
legendaries with no prior cry — Pelagios + Morthas — used LUGIA / GIRATINA placeholders.
`delay 25` between cries.

**Conventions reconfirmed:** Eira's "nods once" beat is NARRATION (setspeaker 0, no
namebox) — a character who says nothing gets no namebox. Dex/Drenn epilogue lines are
flag-gated (FLAG_DEX_ALIVE / FLAG_DRENN_ALIVE) via goto_if_set/skip. No new flags, vars,
items, trainers, or speakers were needed — all existed from prior islands. See
[[scripting-gotchas]] for the engine constraints that still apply.
