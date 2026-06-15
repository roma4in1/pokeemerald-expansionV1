---
name: underwater-and-vast-rooms
description: Thalvern patterns - custom underwater platform-puzzle layout, the vast-room (drowned cathedral) design recipe, fog/weather constant, water_mons mapping
metadata:
  type: project
---

Built on Thalvern Isle (13 maps, 2026-06-13, `tools/pelagios/build_thalvern*.py`).
Non-obvious patterns worth reusing:

**Underwater PLATFORM PUZZLE from a custom flat-fill secondary (NOT a vanilla
reuse).** For a "walkable stone platforms over flooded floor" puzzle, compose a
custom layout: fill the WHOLE grid with deep-water words (U_WATER =
mt(1, collision=1, elevation=0)), then stamp walkable platform islands
(U_GROUND = mt(1, collision=0, elevation=3)) as stepping stones, joined by a
narrow central walkable spine so exactly ONE path connects entry -> dais. Side
spurs are dead-ends over the flood (future Dive / wild-water flavor). This gives a
cleaner purpose-shaped path than reusing AQUA_HIDEOUT_1F (which I considered, per
[[dual-port-and-special-interiors]], but its elevation split is shaped for a
two-level lab, not a stepping-stone puzzle). The periodic-submerge tile animation
and Dive/SONAR_LENS logic are engine + script-writer concerns - the map-builder
only provides the elevation/collision split geometry. Thalvern_SubmergedRuins_Interior1
(22x22) is the reference.

**VAST-ROOM recipe (drowned cathedral, the biggest Pelagios room so far at 28x40).**
To make a room read as monumental: (1) oversize it (28x40); (2) keep a 2-tile
central processional spine walkable the FULL length (re-assert it as U_GROUND
AFTER every other patch, since later patches can stomp it); (3) flank a wide stone
nave with IMPASSABLE flooded outer aisles (deep water) so only the raised centre is
walkable; (4) line the nave with twin colonnades of solid pillars (a wall word every
3 rows) for rhythm/scale; (5) put the significant object (throne + seal apparatus +
sealed legendary) on a raised dais at the far end, player facing the throne block
from the south; (6) a coord-trigger row well inside the entrance (not on the landing
tile) fires the set-piece. Thalvern_ThroneRoom is the reference. The legendary is an
SS_TIDAL decoration (script 0x0, elev 1) like every SealChamber.

**WEATHER_FOG_1 IS NOT A REAL SYMBOL** (the brief names it; it doesn't exist). The
vanilla fog used by Cave of Origin etc. is **WEATHER_FOG_HORIZONTAL** (=6). Other
fog symbols: WEATHER_FOG (=22, aggregate), WEATHER_DOWNPOUR (=13). WEATHER_RAIN (=3)
is fine. See [[link-time-gotchas]].

**water_mons tables for surf/flooded/deep-water encounters:** 5 slots, fixed engine
rates 60/30/5/4/1 (vs land's 12 slots 20/20/10/10/10/10/5/5/4/4/1/1). A 40/30/20/10
brief split maps imperfectly - approximate (e.g. double the 20% mon into slots 2-3).
NOTE the same caveat as composed land tables: custom flat-fill secondary metatiles
carry NORMAL behavior, so the tables only fire where the engine sees real surf/water
behavior - flooded sections need Dive/Surf-behavior metatiles (a Porymap follow-up).
Attach the tables anyway per the established Schism/Emberveil precedent.

**build_thalvern.py merges layout entries straight into layouts.json** (idempotent
by id), unlike the older island scripts that only wrote a standalone *_layout_entries.json
needing a manual merge. Prefer the self-contained merge for new islands. Also note:
the brief listed a PokeCenter "building" for the FloatingMarket but the map group is
EXACTLY 13 maps - I dropped a separate PC map to honor the count; the island's only
heal point is the TidespirePort Inn (nurse-gfx innkeeper, so NOT in
IsLastHealLocationPlayerHouse, per [[dual-port-and-special-interiors]]).
