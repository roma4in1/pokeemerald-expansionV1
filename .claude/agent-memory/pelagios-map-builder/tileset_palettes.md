---
name: tileset-palettes
description: Verified vanilla metatile words per tileset for composing Pelagios outdoor layouts (desert/stone/cave) - copy verbatim so collision+elevation come free
metadata:
  type: project
---

Composed outdoor layouts sample metatile WORDS verbatim from a vanilla layout that
uses the SAME tileset pair, so collision (bits 10-11) and elevation (bits 12-15) are
inherited correctly. Known-good palettes used so far (see the per-island build_*.py):

**Desert (General/Mauville), sampled from LAYOUT_ROUTE111** — used for all Sirocco
outdoor maps (`tools/pelagios/build_sirocco_layouts.py`):
- 0x3121 plain desert sand, walkable, elev 3
- 0x3251 DeepSand_Center, walkable — the wild-encounter sand tile (use for grass-patch
  equivalents; encounters only spawn where a wild table exists for the map)
- 0x0471 / 0x0473 desert rock wall blocks (collision) — walls, riverbed scar, sign markers
- 0x1170 General CalmWater (deep water, collision); 0x1179 water-with-land-to-north shore edge
- 0x3001 plain green ground (used for the oasis grass ring), walkable elev 3

**Stone/port (General/Slateport), sampled from LAYOUT_SLATEPORT_CITY** — Ironhold outdoors
(`build_ironhold.py`): 0x3001 ground, 0x04C6/0x04C7 walls, 0x0403 sign, 0x1170 water,
0x1179/0x1189/0x1190/0x1192 shore edges (N/S/W/E), 0x300D TallGrass (wild encounters).

**Cave (General/Cave), sampled from LAYOUT_VICTORY_ROAD_1F** — floor/wall picked
programmatically as the most-common collision-0 / collision-1 words.

**Volcanic (General/Lavaridge), sampled from LAYOUT_MT_CHIMNEY** — all Emberveil outdoor
maps (`build_emberveil.py`):
- 0x3271 volcanic ash ground, WALKABLE floor (col0 elev3) — base floor (dark volcanic rock)
- 0x3671 lava field, IMPASSABLE (col1 elev3) — lava walls/channels + Lava-Boots-gated
  corridors (leave a 2-tile walkable gap a coord trigger guards)
- 0x0674 rock wall (col1 elev0) — border ring + pillars
- 0x300D General TallGrass (wild encounters) — prefer over Lavaridge AshGrass 0x207 (0x207
  isn't placed in any vanilla layout so its full word can't be sampled)
- 0x0403 sign, 0x1170 deep water, 0x1189 shore-edge land-to-north
- SUBSTITUTIONS: no OBJ_EVENT_GFX_LADY (use OBJ_EVENT_GFX_WOMAN_5); no MUS_ROUTE111 /
  MUS_RG_DEEP_DEEP_WATER (use MUS_ROUTE110 / MUS_MT_CHIMNEY). See [[link-time-gotchas]].

Interiors reuse vanilla LAYOUT_* wholesale (zero new binary). Useful reuse picks:
POKEMON_CENTER_1F/2F (inn lobby/rooms), MART (shop), SEALED_CHAMBER_OUTER_ROOM &
SEALED_CHAMBER_INNER_ROOM (ruin chambers/black-market basement), VICTORY_ROAD_1F (gym
dungeon), MOSSDEEP_CITY_SPACE_CENTER_1F/2F (multi-room palace interiors).

GOTCHA: LAYOUT_SEALED_CHAMBER_OUTER_ROOM (21x23) has its CENTER columns 8-12 walled off
on the bottom rows (17-19) - the walkable bottom pockets are cols 3-7 and 13-17. A warp
landing at (10,17/18/19) lands ON A WALL. Land returns in the SE/SW pockets instead
(e.g. (16,18)). Always run the walkability validator (link-time-gotchas) after generating.
