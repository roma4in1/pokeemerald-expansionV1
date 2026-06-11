---
name: build-workflow
description: How Pelagios island maps are built - the tooling pattern, registration checklist, and constants that bite at link time
metadata:
  type: project
---

Island maps in Pelagios are built with per-island Python scripts under `tools/pelagios/`,
mirroring Haven Isle's `build_layouts.py` + `build_mapjson.py`. Ironhold added
`build_ironhold.py` (composes new outdoor layouts) and `build_ironhold_mapjson.py`
(emits all map.json). Reusing/extending these scripts is the expected workflow - do NOT
hand-edit generated map.bin/map.json; edit the script and re-run.

**Why:** The user/project treats these generator scripts as the source of truth (CLAUDE.md
says "regenerate via the script, don't hand-edit"). Keeps layouts reproducible.

**How to apply:** For a new island, write `tools/pelagios/build_<island>.py` in the same
style: sample metatile words verbatim from a vanilla layout with the SAME tileset pair
(collision bits 10-11 / elevation bits 12-15 come along for free), reuse vanilla LAYOUT_*
wholesale for interiors (houses/marts/PCs/gyms/caves), then mirror that layout's known
warp tile coords (read them from the vanilla map.json that owns the layout).

Registration checklist for each island (all required or the build breaks):
1. data/layouts/layouts.json - one entry per NEW layout (id/name/w/h/tilesets/filepaths).
2. data/maps/map_groups.json - append group to `group_order` AND add the map list.
   Empty groups can't exist in generated groups.inc, so register the group WITH its maps.
3. data/maps/<Map>/map.json + scripts.inc per map. header.inc/events.inc are GENERATED.
4. data/event_scripts.s - a `.include "data/maps/<Map>/scripts.inc"` line per map.
5. src/data/region_map/region_map_sections.json (name-only MAPSEC entries) AND a matching
   designated-initializer block in region_map_entries.h (Haven did both).
6. src/data/heal_locations.json for the island's heal point (respawn_npc = a local_id NPC).
7. src/data/wild_encounters.json - append to wild_encounter_groups[0].encounters.

LOCALID_* constants are auto-generated from `local_id` fields in map.json - no manual header.

Land encounter tables need exactly 12 mons; slot rates are fixed
20/20/10/10/10/10/5/5/4/4/1/1. Map a 40/30/20/10 brief split to consecutive slots that
sum to those percents (40=slots0-1, 30=2-4, 20=5-7, 10=8-11).
