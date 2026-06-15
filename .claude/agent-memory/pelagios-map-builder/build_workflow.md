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

**Per-island exceptions exist — read the island brief before assuming the full checklist
applies.** Ashenveil (the Dead Island, built 2026-06-14) deliberately OMITS heal locations
(step 6 skipped entirely — no inn/nurse, and IsLastHealLocationPlayerHouse() must stay
untouched), has ZERO trainers/gyms, and wild tables in only 3 of 9 maps. Its only "NPCs" are
examine bg_event signs + two script-managed hide-flag objects (Dorne uses
FLAG_ASHENVEIL_DORNE_MET so he's present-until-scene; Morthas uses FLAG_MORTHAS_ENCOUNTERED so
the object vanishes once set — the map-builder just sets the object's `flag` field, the engine
hides it). Sea Chart is a SCRIPTED examine, not a bg_hidden_item (no flag slot consumed).

**MAPSEC >= 0x100 truncation (open systems item):** the map_section field in the map-header
struct is a single .byte, so islands whose MAPSEC enum values exceed 0xFF (Primalis, Ashenveil)
emit a harmless "value 0x10X truncated to 0xX" warning at header.inc line 12. Maps still build
and function; region-map naming for those late islands may be slightly wrong until a
systems-engineer widens the field. NOT a map-builder bug — don't chase it.

**Aetheron (the SKY ISLAND, built 2026-06-14)** added a NEW arrival pattern: a SCRIPTED
ASCENT map (KnockUpStream) with NO warps back — the boat (Pelagios_EventScript_SailToAetheron,
which is Sea-Chart-gated, NOT boat-tier-gated) warps to KnockUpStream's arrival tile, the
ascent coord-trigger cutscene fires, then a ONE-WAY forward warp lands the player on a
DEDICATED non-door arrival tile on CloudLanding (point the forward warp at a warp_event index
whose (x,y) is a plain off-path walkable tile, NOT the inn door — otherwise the player re-warps
into the inn on arrival). Boat-stub swap: replace only the `goto SailNoChart` fall-through, keep
the `goto_if_unset FLAG_SEA_CHART_FOUND` gate. Cloud aesthetic = light General ground (0x3001) +
deep-water VOID (0x1170) as the open-sky abyss framing; true cloud-white is a Porymap pass.
Installation exterior pairs gTileset_Mauville (angular = "feels wrong"). Multiple script-managed
Cass objects across maps all share ONE hide flag (FLAG_AETHERON_CASS_SEEN) as a placeholder —
the script-writer owns runtime "walks alongside" visibility; the defection object uses
FLAG_AETHERON_GYM3_CLEAR. See [[link-time-gotchas]] for WEATHER_RAIN_THUNDERSTORM / MUS gotchas.

**Dusk/dark-palette maps:** there is no map-builder lever for tinting a map dark. Approximate
with the greyest available metatiles (Slateport stone for ash/ruin) + fog weather, and document
that true darkening needs a Porymap per-map palette pass or a C tint. Done this way for Ashenveil.
