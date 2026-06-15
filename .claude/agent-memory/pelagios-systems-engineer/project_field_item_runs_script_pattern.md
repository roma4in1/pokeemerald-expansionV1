---
name: field-item-runs-script-pattern
description: How to wire a key item's bag "Use" to run a fixed field event script (Storm Compass pattern), map-agnostic, no field-condition gate
metadata:
  type: project
---

To make a key item's bag "Use" run a fixed overworld event script from ANY map (no
map-type / surf / harbor gate), mirror **ItemUseOutOfBattle_PokemonBoxLink** in
`src/item_use.c`. This is the clean menu/script-opening model — do NOT copy Surf/Fly
funcs (they call CanUseSurf/etc. and impose field-condition checks).

**Why:** ITEM_STORM_COMPASS (880, replaces Fly) needed bag-use to open the boat
destination menu from anywhere (outdoor/cave/surf). The boat SCRIPT
(Pelagios_EventScript_StormCompass in data/scripts/pelagios_boat.inc) already existed.

**How to apply** — three edits, all C/data side:
1. `src/item_use.c`:
   - forward decl: `static void Task_UseScript(u8);`
   - extern the script symbol locally (it's in a .inc included via event_scripts.s, NOT
     in event_scripts.h): `extern const u8 Pelagios_EventScript_Foo[];`
   - the func + task:
     ```c
     void ItemUseOutOfBattle_Foo(u8 taskId) {
         sItemUseOnFieldCB = Task_UseFoo;
         SetUpItemUseOnFieldCallback(taskId);  // fades bag out, returns to field; NO gate
     }
     static void Task_UseFoo(u8 taskId) {
         ScriptContext_SetupScript(Pelagios_EventScript_Foo);
         DestroyTask(taskId);
     }
     ```
2. `include/item_use.h`: add `void ItemUseOutOfBattle_Foo(u8 taskId);`.
3. `src/data/items.h`: set the item's `.fieldUseFunc = ItemUseOutOfBattle_Foo`
   (was ItemUseOutOfBattle_CannotUse). Field key item needs importance 1,
   pocket POCKET_KEY_ITEMS, type ITEM_USE_BAG_MENU — don't change those if already set.

`SetUpItemUseOnFieldCallback` (item_use.c ~line 154) imposes no map-type restriction —
verified by reading it. That's what makes the item work from any tile. Possession is the
gate; don't add a flag check. Build links because the .inc script is in the always-included
event_scripts tree. See [[project-constant-space-budget]] for item ID space.
