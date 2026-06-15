---
name: undressed-outdoor-layouts
description: Pelagios island OUTDOOR layouts are generated as bare ground/wall/water with NO building metatiles — "flat ground in mGBA" is the expected undressed state, region-wide, not a tileset/render bug
metadata:
  type: project
---

All Pelagios custom OUTDOOR layouts (built by tools/pelagios/build_<island>.py)
stamp ONLY ground / wall / water / sign / tall-grass metatiles. **No building
metatiles are ever placed** — "buildings" are just warp tiles sitting on plain
ground. So when a map "shows only flat ground, no buildings" in mGBA, that is the
expected-but-UNDRESSED state, NOT a wrong-tileset or render bug.

**Why:** the generators (build_ironhold.py etc.) were written to produce "blocky
stone rectangles, dress with building metatiles later" (CLAUDE.md Ironhold MAPS
deviations). Confirmed region-wide on 2026-06-14: Ironhold, Sirocco, Schism,
Emberveil outdoor layouts ALL use only 0-2 secondary-tileset metatile IDs; the
rest are primary-tileset ground/wall IDs (<512). NONE have out-of-range IDs.

**How to apply:**
- DIAGNOSE before fixing. Decode the compiled map.bin: each tile is a u16,
  metatile_id = word & 0x3FF, collision = (word>>10)&3, elevation = (word>>12)&0xF.
  Print the SET of distinct metatile_ids. IDs < 512 come from the PRIMARY tileset;
  IDs >= 512 come from the SECONDARY (valid range 512 .. 512 + secondary_count - 1,
  where secondary_count = size(metatiles.bin)/16). Out-of-range secondary IDs =
  genuine "renders blank" bug; only-low-ground-IDs = undressed (content gap).
- A tileset reassignment does NOT add buildings to an undressed layout. Do not
  "fix" empty ground by swapping the secondary tileset.
- Safe way to add buildings: copy building blocks VERBATIM as full metatile WORDS
  (id|collision|elevation) out of a vanilla layout that uses the SAME tileset pair
  (e.g. LAYOUT_SLATEPORT_CITY for General/Slateport). That guarantees valid +
  tile-consistent IDs and can't introduce the out-of-range bug. Align the building's
  DOOR tile onto an existing warp coord, with the building rising NORTH of the warp
  (door enterable from the south). After regen, VALIDATE: every warp door tile and
  the tile directly south of it must have collision==0.
- A full town-dressing pass (many buildings, paths, cliffs, door anims) for all
  islands is DEFERRED map-builder content work, not a debugger one-liner. See
  [[generators-are-source-of-truth]].
