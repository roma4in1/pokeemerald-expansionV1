---
name: tileset-creation
description: How to assemble custom Pelagios secondary tilesets in this repo (C-based registration, GBA budgets, generator, format gotchas)
metadata:
  type: project
---

First custom tileset work in the project (2026-06-12): six Pelagios secondary
tilesets built from `graphics/tilesets/pelagios_source/` converted Essentials art.

**Why:** the project needs island-themed terrain art for future Pelagios maps
(coastal/desert/volcanic/ice/poison/underwater) instead of reusing vanilla tilesets.

**How to apply:** when building or extending Pelagios tilesets, reuse this exact
pipeline and constraints. See the "Custom Pelagios Tilesets" section of CLAUDE.md
for the authoritative reference; key load-bearing facts below.

### This repo uses C-based tilesets (pokeemerald-expansion), NOT asm
Tileset GRAPHICS live in `data/tilesets/secondary/<dir>/` as
`tiles.png` + `palettes/00.pal..15.pal` (JASC-PAL) + `metatiles.bin` +
`metatile_attributes.bin`. They are wired up in C, in FOUR files, all inside the
Emerald `#if !IS_FRLG` section (NEVER inside the `#if IS_FRLG` / `#else` FRLG tail
at the end of each file — that is the trap; the files' tails are FRLG-only):
1. `src/data/tilesets/graphics.h` — `gTilesetTiles_*` INCGFX_U32 ".4bpp.fastSmol"
   with `-num_tiles N -Wnum_tiles`, + `gTilesetPalettes_*[][16]` (16 INCGFX_U16 .gbapal).
2. `src/data/tilesets/metatiles.h` — `gMetatiles_*` + `gMetatileAttributes_*` INCBIN_U16.
3. `src/data/tilesets/headers.h` — `const struct Tileset gTileset_*` (isCompressed=TRUE,
   isSecondary=TRUE, callback=NULL).
4. `include/tilesets.h` — `extern const struct Tileset gTileset_*;`
No `.mk` edit needed (INCGFX preproc auto-builds PNG->4bpp). Porymap auto-discovers
from these C files — no porymap.project.cfg / json edit.

### GBA / engine constants that bound the work
- Secondary tile budget = NUM_TILES_TOTAL(1024) − NUM_TILES_IN_PRIMARY(512) = **512 tiles**.
- 16 colors/palette; transparent at index 0. Secondary palettes use slots 6-12
  (NUM_PALS_IN_PRIMARY=6). Put the real palette in **slot 6**; slots 0-5 get
  overwritten by the paired PRIMARY tileset at runtime.
- metatiles.bin = 8 u16/metatile (4 bottom + 4 top layer). u16 =
  tileid(0-9) | hflip(10) | vflip(11) | palnum(12-15). Metatile 0 = fully blank.
- metatile_attributes.bin = 2 bytes/metatile (porymap metatile_attributes_size=2):
  behavior bits0-7, layer type bits12-15. Authoritative MB values: NORMAL=0,
  TALL_GRASS=2, DEEP_SAND=6, CAVE=8, POND_WATER=16, DEEP_WATER=18, OCEAN_WATER=21.
- tiles.png is 16 tiles (128px) wide, 4bpp indexed.

### Generator: tools/pelagios/build_tilesets.py
- Source PNGs are full HGSS/Essentials AUTOTILE BLOB SHEETS with massive redundancy
  (coastal sources ~29k tiles total) — you CANNOT load them whole within 512 tiles.
  The generator extracts a curated, DEDUPED, capped representative subset per source.
- `TILESETS` dict drives it: per source PNG -> (behavior, max-tiles-to-sample). Caps
  keep each tileset <=480 tiles. To add tiles: raise a source cap or add a PNG, rerun,
  and update `-num_tiles N` in graphics.h if the count changed.
- GOTCHA fixed: the sheet pads its last row to 16 tiles; quantize maps that black
  padding to a near-black non-zero index, which makes gbagfx IGNORE `-num_tiles N`
  ("tile X contains non-transparent pixels"). FIX: after quantize, force palette
  index 0 on tile 0 AND every padding slot beyond num_tiles. Then num_tiles matches.

### Cost / result
Six tilesets (413/336/294/255/197/229 tiles) added **~53 KB ROM (+0.16 pp)**, EWRAM
and IWRAM UNCHANGED (tilesets are pure ROM data). gmake exit 0. Metatiles are flat
2x2 fills (one per usable tile) — usable/paintable but plain; richer metatiles
(edges/cliffs/overlays) are Porymap hand-tuning later. Re-running the generator
CLOBBERS Porymap edits, so stop regenerating a tileset once it's hand-tuned.
