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
- Speaker nameboxes (`setspeaker` + Pelagios_Speaker_* labels): full usage rules
  documented in CLAUDE.md > Dialogue Style Guidelines > Speaker nameboxes.
  Engine facts verified 2026-06-11: speaker persists across msgboxes while the
  box is open; cleared by closemessage/script end/battle/warp (ResetNameboxData
  in InitStandardTextBoxWindows); `setspeaker 0` clears mid-conversation;
  SP_NAME_* is a C enum - unusable from asm, use string labels or literal 0.

Added during the Sirocco script build (2026-06-12):

- Badge flags END at FLAG_BADGE08_GET (NUM_BADGES = 8, no FLAG_BADGE09 exists).
  Pelagios has 4+ gyms per island, so the 9th+ badges must be narrative-only:
  play the fanfare + "received the X BADGE" text but set only the island's
  GYM_CLEAR flag. First hit: Dagan's Mire Badge (badge 9 overall). Any later
  badge that must affect obedience/HM checks needs a systems-engineer refactor.
- Objects hidden by a FLAG_TEMP_x hide flag are VISIBLE by default on every map
  load (temp flags are cleared on warp). A mid-scene cameo object (Miria in
  DaganPalace_Interior2) needs MAP_SCRIPT_ON_TRANSITION `setflag FLAG_TEMP_x`,
  then the scene does clearflag + addobject. The cameo auto-vanishes next entry.
- Critical cutscenes that hand out a TM must NOT goto Common_EventScript_ShowBagIsFull
  (it ends the script). Use `checkitemspace ITEM_X, 1` + `call_if_eq VAR_RESULT,
  TRUE, GiveSub` so a full bag silently skips the TM and the scene continues
  (Sever and Dagan escape scenes use this).
- Nonlinear island progress vars: only advance the island's VAR_*_PROGRESS when it is
  EXACTLY the expected predecessor (`call_if_eq VAR, n, AdvanceSub`). Sirocco's
  Miria is reachable before Crag; an unconditional setvar 4 would have disarmed
  the Miraden north-gate triggers (armed at 1/2) without Gym 2, and Crag's
  unconditional setvar 3 would later REGRESS it. The choke-point scene (Dagan
  escape -> 5) may set unconditionally.
- Parallel agent sessions can leave the tree unlinkable (dangling .string label
  in an Emberveil script broke MY baseline). Always `gmake` BEFORE writing
  anything and fix/triage other-island breakage first - otherwise you can't
  attribute new errors to your own edits.
- Two-sided unlock checks (Galleon = Sirocco AND Emberveil resolved) must be
  implemented in BOTH islands' resolution scripts - whichever resolves second
  sets VAR_BOAT_TIER. Verify the sibling island's script actually has the
  mirror branch before assuming the unlock works.

Added while finishing Emberveil (2026-06-12):

- A clean `gmake` exit 0 does NOT mean an island's scripts are done: the map-builder's
  stub generator (build_*_scripts_stubs.py) emits compilable placeholder labels for
  every map.json-referenced script. When resuming a cut-off session, `grep -rn TODO`
  across the island's scripts.inc is the real completeness check, then verify trigger
  wiring by dumping every map.json's coord/object/bg events against the scripts.
