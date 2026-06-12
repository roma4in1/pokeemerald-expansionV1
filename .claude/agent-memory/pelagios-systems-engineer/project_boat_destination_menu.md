---
name: boat-destination-menu
description: How the Pelagios Tennyson tier-gated destination-select system is built and how to extend it when a new island is added
metadata:
  type: project
---

The Tennyson boat-travel system (built 2026-06-12) is the single way to sail
between islands. Replaced per-harbor direct-warp YESNO Tennyson scripts.

**Why:** Sirocco + Emberveil were fully built but unreachable — the old Tennyson
scripts only sailed Haven<->Ironhold. Needed a shared, tier-gated menu so islands
expand naturally as VAR_BOAT_TIER (0x4100) rises.

**How to apply / architecture:**
- Shared script: `data/scripts/pelagios_boat.inc` (included from
  data/event_scripts.s after pelagios_speaker_names.inc). Entry point
  `Pelagios_EventScript_BoardTennyson`.
- Multichoice strategy = **one fixed static list per boat tier** (NOT dynamic, NOT
  per-harbor). 3 lists in src/data/script_menu.h: `MultichoiceList_BoatSloop`
  (Haven, Ironhold), `_BoatBrigantine` (+Sirocco, Emberveil), `_BoatGalleon`
  (+Schism, Thalvern, Gildhaven). Island order identical across tiers so higher
  tiers just append; CANCEL is always last. Constants MULTI_BOAT_SLOOP/
  _BRIGANTINE/_GALLEON appended to the MULTI_ enum in
  include/constants/script_menu.h, registered in sMultichoiceLists[].
- Entry switches on VAR_BOAT_TIER to pick the list, then switches on VAR_RESULT
  (chosen index) to a per-island sail handler. B-press returns 127 -> falls
  through to CANCEL.
- **Same-island handling:** each harbor sets **VAR_TEMP_1** = its
  PELAGIOS_ISLAND_* id (defined via `.set` in pelagios_boat.inc) before
  `goto`-ing the entry. Sail handler compares and shows "You're already moored
  here." instead of warping. VAR_TEMP_1 is safe (set fresh at boarding, cleared
  on warp). NOTE: Emberveil CalderaRuins reuses VAR_TEMP_1 as a panel counter but
  that's a different map/session — no conflict.
- **Stub islands** (Schism/Thalvern/Gildhaven, Galleon tier) are selectable but
  route to `Pelagios_EventScript_SailNoChart` -> "charts don't show a path there
  yet" until built.
- **Arrival tiles** (verified walkable via layout collision bits): Haven (13,9);
  Ironhold/Sirocco/Emberveil all (10,14) — the 3 ports share identical pier
  geometry (map-builder mirrored them). Sirocco/Emberveil arrival cutscenes fire
  because (10,14) is south of the trigger row (Sirocco coord triggers at
  9-10,11) or because arrival is an ON_FRAME table (Emberveil).
- **bg_events were NOT touched** — only the script bodies behind the existing
  *_EventScript_Tennyson labels were rewritten, so the map.json generators stay
  in sync. Per harbor rules, boarding always hangs off sign bg_events, never the
  SS_TIDAL decoration object.

**To add a future island to the menu:** (1) give it a PELAGIOS_ISLAND_* id; (2)
replace its SailNoChart dispatch case with a real SailTo<Island> handler (warp +
same-island guard), keeping the index in sync with its position in the matching
MultichoiceList_Boat*; (3) write that harbor's *_EventScript_Tennyson (lockall /
setvar VAR_TEMP_1 / optional intro / goto entry) and point its boarding bg_events
at it. See [[constant-space-budget]] for where new island constants go.

Build cost: zero RAM (script + tables). ROM actually ticked DOWN slightly (removed
more dead per-harbor sail text than added).
