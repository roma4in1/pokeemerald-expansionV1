---
name: dual-port-and-special-interiors
description: Schism Isle patterns - dual-port boat handling, ice-puzzle + two-level-lab interior reuse, and the heal-location whiteout nuance that contradicts the brief
metadata:
  type: project
---

Built on Schism Isle (21 maps, 2026-06-13). Non-obvious patterns worth reusing:

**Dual-port island (two arrival ports in the boat menu):**
- Each port is a SEPARATE `PELAGIOS_ISLAND_*` id in data/scripts/pelagios_boat.inc
  (Schism = PELAGIOS_ISLAND_SCHISM north / PELAGIOS_ISLAND_SCHISM_SOUTH south).
  The systems engineer pre-stubs both as separate Galleon-tier multichoice entries
  ("SCHISM (NORTH)" / "SCHISM (SOUTH)") whose dispatch cases goto SailNoChart.
- Map-builder swap-in: replace each stub's `goto SailNoChart` with the same-island
  guard + warp (`goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_*, Pelagios_EventScript_AlreadyHere`
  then `warp MAP_*_PORT, 10, 14`). The shared "already moored" label is
  `Pelagios_EventScript_AlreadyHere` (NOT AlreadyMoored). Cast-off text reuse =
  `Pelagios_Text_CastOff`. Arrival tile 10,14 (the dock) is the project convention
  for every port (Ironhold/Sirocco/Emberveil/Schism), verified walkable.
- Each port's `*_EventScript_Tennyson` must `setvar VAR_TEMP_1, PELAGIOS_ISLAND_*`
  before `goto Pelagios_EventScript_BoardTennyson`. Map-builder writes this minimal
  boarding wiring into the (otherwise stubbed) port scripts.inc - it is boat-system
  wiring, not dialogue, so it is map-builder scope (mirrors the hand-written Emberveil
  Tennyson script). Boarding hangs off pier sign bg_events, never the SS_TIDAL object.

**Ice sliding-puzzle interior:** the custom gTileset_PelagiosIce is flat-fill (no
MB_ICE behavior metatiles), so an ice-slide gym CANNOT be built from it. Reuse the
vanilla `LAYOUT_SHOAL_CAVE_LOW_TIDE_ICE_ROOM` (General/Cave) - it has real MB_ICE /
cracked-ice tiles and a built-in slide puzzle. Entrance is the bottom-right open area
(warp ~12-13,28); the gym leader goes top-left past the ice (~3,2). Sliding is pure
engine behavior - no extra triggers, leave a TODO for any reset/hint sign only.

**Two-level lab interior (lower floor + stairs to upper office) in ONE map:** reuse
`LAYOUT_AQUA_HIDEOUT_1F` (General/Facility). Its elev-1 platform bridges over water
read as toxic-vat catwalks (lower gym leader, place objects with elevation=1); its
elev-3 north area reads as the upper office (second leader at elev 3). Entrance is the
bottom corridor (warp ~12-13,27). One map conveys both levels via the elevation split -
no second map needed.

**Heal-location whiteout nuance (CONTRADICTS the Schism brief's instruction):** the
brief said add both port-Inn heal locations to `IsLastHealLocationPlayerHouse()` in
src/heal_location.c. DO NOT. That function returning TRUE routes whiteout to the
mom/prof heal path (EventScript_AfterWhiteOutMomHeal); FALSE routes to the nurse path
(EventScript_AfterWhiteOutHeal, which does the Poke-Center-nurse bow animation). Pelagios
Inns reuse LAYOUT_POKEMON_CENTER_1F with a NURSE-gfx innkeeper at the nurse's standard
spot (7,2), so the nurse path works correctly - exactly why Ironhold/Sirocco/Emberveil
Inns are NOT in that function. The Haven case (HAVEN_ISLE_PLAYER_HOUSE_2F) needs it only
because it respawns to a HOUSE with no nurse. Follow the working Ironhold/Emberveil Inn
pattern, not the brief. See [[build_workflow]].
