---
name: scripting-gotchas
description: Hard-won pokeemerald-expansion event-scripting constraints discovered while building Ironhold (trigger vars, trainer-see, TM set, common script labels)
metadata:
  type: project
---

Technical constraints verified during the Ironhold script build (2026-06-11). The
island-specific outcomes live in CLAUDE.md; these are the reusable engine facts.

**Why:** each of these cost a search/debug cycle to establish; they apply to every
future island's scripts.

**How to apply:** check this list before writing any new island's scripts.inc or
map-generator triggers.

- Coord-event triggers compare a **VAR** against var_value. Putting a FLAG_ constant
  in the "var" field never fires (VarGet of an id < VARS_START returns the id itself).
  Use the island's VAR_*_PROGRESS, with one trigger entry per matching value.
- Trainer-see (TRAINER_TYPE_NORMAL + sight) reads the trainer flag from the FIRST
  script command, which must be `trainerbattle`. Any gated trainer (gym leaders with
  goto_if_unset checks) must be TRAINER_TYPE_NONE / talk-initiated in map.json.
- TM set is vanilla-only: include/constants/tms_hms.h lists the 50 TM moves. Use
  `ITEM_TM_<MOVE>` aliases (e.g. ITEM_TM_BULK_UP); no Iron Defense / Close Combat /
  Meteor Mash TMs exist. Numeric ITEM_TM51+ constants exist but have no move mapping.
- `OBJ_EVENT_ID_NPC_TALKING_TO` does not exist - use `VAR_LAST_TALKED` in
  applymovement for the NPC being spoken to.
- giveitem with a full bag: VAR_RESULT = FALSE, item silently lost.
  `Common_EventScript_ShowBagIsFull` ends the script (release/end inside) - never
  branch to it before critical setflag/setvar/key-item lines; give key items and set
  flags FIRST, TMs last.
- Useful verified labels/macros: Common_EventScript_OutOfCenterPartyHeal,
  Common_EventScript_PkmnCenterNurse (needs VAR_0x800B = nurse LOCALID),
  Common_Movement_FaceUp/FaceDown/ExclamationMark, goto_if_defeated,
  special SpawnCameraObject / RemoveCameraObject (+ OBJ_EVENT_ID_CAMERA movements),
  setobjectxy + turnobject + copyobjectxytoperm (fade-to-black reposition pattern
  avoids NPC walk paths the player can block), ShakeCamera (VAR_0x8004..7 + waitstate),
  warp MAP_X, x, y + waitstate.
- ON_FRAME first-entry scenes: map_script_2 VAR_TEMP_0, 0, Scene; Scene sets
  VAR_TEMP_0=1 immediately (session disarm) and a permanent story flag for re-entry.
- Object hide flags can REUSE story flags (e.g. an NPC hidden once
  FLAG_<ISLAND>_GYM1_CLEAR or FLAG_<ISLAND>_RESOLVED is set) - no new flag needed.
  Flags default to 0 = visible, so "hidden until X happens" objects need an
  ON_TRANSITION script re-asserting setflag on every map load.
- msgbox MSGBOX_YESNO -> goto_if_eq VAR_RESULT, YES/NO. Custom multichoice menus
  live in C (forbidden) - phrase binary story choices as yes/no prompts.
- charmap: ASCII `-` only (no em/en dashes); `é`, `…`, and curly quotes are fine.
