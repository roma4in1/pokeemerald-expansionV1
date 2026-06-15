#!/usr/bin/env python3
"""
Pelagios custom secondary tileset assembler.

Takes the GBA-converted source PNGs in graphics/tilesets/pelagios_source/
(full Essentials autotile sheets) and assembles them into six compact,
GBA-valid pokeemerald-expansion SECONDARY tilesets:

  gTileset_PelagiosCoastal   (haven_isle_coastal)
  gTileset_PelagiosDesert    (sirocco_desert)
  gTileset_PelagiosVolcanic  (emberveil_volcanic)
  gTileset_PelagiosIce       (schism_ice)
  gTileset_PelagiosPoison    (schism_poison)
  gTileset_PelagiosUnderwater(thalvern_underwater)

For each tileset it writes, into data/tilesets/secondary/pelagios_<name>/:
  - tiles.png                 (4bpp indexed, 16 tiles wide, <=480 tiles)
  - palettes/00.pal .. 15.pal (JASC-PAL; real terrain palette in slot 6)
  - metatiles.bin             (8 u16 per metatile, flat 2x2 fills)
  - metatile_attributes.bin   (2 bytes per metatile: behavior + layer type)

GBA constraints respected:
  - Secondary tile budget: NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY = 1024-512 = 512.
    We cap each tileset to <= 480 tiles (leaves headroom; tile 0 stays blank).
  - 16 colors per palette (15 + transparent index 0).
  - Secondary palettes live in slots 6-12 (NUM_PALS_IN_PRIMARY=6). We use slot 6.

The source sheets are autotile blobs with huge redundancy. We extract a curated,
deduplicated, capped set of representative 8x8 tiles per source so the tileset is
compact yet covers each terrain type. Metatiles are simple flat 2x2 fills (one
per usable tile) so every tile is reachable/paintable; Porymap fine-tuning later.

Run: python3 tools/pelagios/build_tilesets.py
(Run from repo root; uses absolute paths derived from this file's location.)
"""

import os
import struct
from pathlib import Path
from PIL import Image

REPO = Path(__file__).resolve().parents[2]
SRC = REPO / "graphics" / "tilesets" / "pelagios_source"
OUT_ROOT = REPO / "data" / "tilesets" / "secondary"

# GBA / pokeemerald constants
NUM_TILES_IN_PRIMARY = 512
NUM_TILES_TOTAL = 1024
SECONDARY_TILE_BUDGET = NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY  # 512
NUM_PALS_IN_PRIMARY = 6
SECONDARY_PAL_SLOT = 6      # first secondary palette slot
TILE_PX = 8
SHEET_WIDTH_TILES = 16      # tiles.png is 16 tiles (128px) wide

# Metatile attribute packing
ATTR_BEHAVIOR_SHIFT = 0
ATTR_LAYER_SHIFT = 12
LAYER_NORMAL = 0  # METATILE_LAYER_TYPE_NORMAL

# Behavior constants (authoritative numeric values from metatile_behaviors.h)
MB_NORMAL = 0
MB_TALL_GRASS = 2
MB_DEEP_SAND = 6
MB_CAVE = 8
MB_DEEP_WATER = 18
MB_OCEAN_WATER = 21

# Per-tileset config: list of (source_png, behavior, max_tiles_to_sample).
# behavior is the MB_* value applied to metatiles built from that source's tiles.
# max_tiles_to_sample caps how many UNIQUE tiles we pull from that source so the
# whole tileset stays well under the 480-tile budget.
TILESETS = {
    "PelagiosCoastal": {
        "dir": "pelagios_coastal",
        "sources": [
            ("Sand shore.png",        MB_NORMAL,      80),
            ("Sea.png",               MB_OCEAN_WATER, 60),
            ("Sea without shore.png", MB_OCEAN_WATER, 60),
            ("HGSS Beach.png",        MB_NORMAL,      90),
            ("HGSS Beach Corner.png", MB_NORMAL,      40),
            ("HGSS Waves.png",        MB_OCEAN_WATER, 60),
            ("HGSS Sea.png",          MB_DEEP_WATER,  60),
        ],
    },
    "PelagiosDesert": {
        "dir": "pelagios_desert",
        "sources": [
            ("Sand.png",            MB_DEEP_SAND, 54),
            ("Sandy Dirt.png",      MB_NORMAL,    90),
            ("Dirt.png",            MB_NORMAL,    54),
            ("Gravel.png",          MB_NORMAL,    54),
            ("Brown cave sand.png", MB_NORMAL,    54),
            ("Light grass.png",     MB_TALL_GRASS,40),
        ],
    },
    "PelagiosVolcanic": {
        "dir": "pelagios_volcanic",
        "sources": [
            ("Magma Lava.png",          MB_DEEP_WATER, 90),   # lava: impassable-ish deep
            ("Magma Water.png",         MB_DEEP_WATER, 70),
            ("Red cave floor.png",      MB_CAVE,       54),
            ("Red cave highlight.png",  MB_CAVE,       54),
            ("Dirt cave highlight.png", MB_CAVE,       54),
        ],
    },
    "PelagiosIce": {
        "dir": "pelagios_ice",
        "sources": [
            ("Snow cave floor.png",      MB_CAVE,   54),
            ("Snow cave highlight.png",  MB_CAVE,   54),
            ("Snow cave ice border.png", MB_CAVE,   54),
            ("White cave floor.png",     MB_CAVE,   54),
            ("White cave highlight.png", MB_CAVE,   54),
            ("White path.png",           MB_NORMAL, 54),
        ],
    },
    "PelagiosPoison": {
        "dir": "pelagios_poison",
        "sources": [
            ("Blue cave floor.png",  MB_CAVE,       54),
            ("Blue cave mud.png",    MB_DEEP_WATER, 54),
            ("Green cave floor.png", MB_CAVE,       54),
            ("Brown cave floor.png", MB_CAVE,       54),
        ],
    },
    "PelagiosUnderwater": {
        "dir": "pelagios_underwater",
        "sources": [
            ("Underwater dark.png", MB_DEEP_WATER, 54),
            ("Seaweed dark.png",    MB_DEEP_WATER, 32),
            ("Seaweed light.png",   MB_DEEP_WATER, 32),
            ("Sea deep.png",        MB_DEEP_WATER, 90),
            ("Still water.png",     MB_DEEP_WATER, 54),
        ],
    },
}

MAX_TILES = 480  # hard cap per tileset (budget is 512; keep headroom)


def iter_tiles(img):
    """Yield (tx, ty, RGB bytes) for each 8x8 tile in an RGB image."""
    w, h = img.size
    for ty in range(h // TILE_PX):
        for tx in range(w // TILE_PX):
            box = (tx * TILE_PX, ty * TILE_PX, tx * TILE_PX + TILE_PX, ty * TILE_PX + TILE_PX)
            yield tx, ty, img.crop(box)


def collect_unique_tiles(png_path, cap):
    """Return up to `cap` unique RGB 8x8 tile images from a source PNG, skipping
    fully-uniform 'empty' tiles only if they are pure transparent/black duplicates
    beyond the first one. Preserves source order for visual coherence."""
    img = Image.open(png_path).convert("RGB")
    seen = set()
    out = []
    for _, _, tile in iter_tiles(img):
        key = tile.tobytes()
        if key in seen:
            continue
        seen.add(key)
        out.append(tile)
        if len(out) >= cap:
            break
    return out


def build_tileset(name, cfg):
    out_dir = OUT_ROOT / cfg["dir"]
    (out_dir / "palettes").mkdir(parents=True, exist_ok=True)

    # 1. Collect curated tiles per source, recording the behavior for each tile.
    #    Tile index 0 is reserved as a blank/transparent tile (engine convention).
    tile_images = []      # list of RGB 8x8 Image
    tile_behaviors = []   # parallel list of MB_* for the metatile built from it
    # Reserve tile 0 as blank
    blank = Image.new("RGB", (TILE_PX, TILE_PX), (0, 0, 0))
    tile_images.append(blank)
    tile_behaviors.append(MB_NORMAL)

    global_seen = {blank.tobytes()}
    for src_name, behavior, cap in cfg["sources"]:
        src_path = SRC / src_name
        if not src_path.exists():
            print(f"  WARNING: missing source {src_name}, skipping")
            continue
        for tile in collect_unique_tiles(src_path, cap):
            if len(tile_images) >= MAX_TILES:
                break
            key = tile.tobytes()
            if key in global_seen:
                continue
            global_seen.add(key)
            tile_images.append(tile)
            tile_behaviors.append(behavior)
        if len(tile_images) >= MAX_TILES:
            print(f"  NOTE: hit {MAX_TILES}-tile cap at source {src_name}")
            break

    num_tiles = len(tile_images)

    # 2. Compose the full RGB sheet (16 tiles wide), then quantize to one 15-color
    #    palette (index 0 reserved transparent). Single shared palette keeps it
    #    simple and within the secondary-slot budget.
    rows = (num_tiles + SHEET_WIDTH_TILES - 1) // SHEET_WIDTH_TILES
    sheet_w = SHEET_WIDTH_TILES * TILE_PX
    sheet_h = rows * TILE_PX
    sheet = Image.new("RGB", (sheet_w, sheet_h), (0, 0, 0))
    for i, tile in enumerate(tile_images):
        tx = (i % SHEET_WIDTH_TILES) * TILE_PX
        ty = (i // SHEET_WIDTH_TILES) * TILE_PX
        sheet.paste(tile, (tx, ty))

    # Quantize whole sheet to 15 colors, reserve index 0 for transparent/black.
    quant = sheet.quantize(colors=15, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = quant.getpalette()[: 15 * 3]
    # Shift indices up by one so index 0 is free for transparency (black).
    new_pal = [0, 0, 0] + pal
    quant = quant.point(lambda x: x + 1)
    quant.putpalette(new_pal + [0, 0, 0] * (256 - 16))

    # Force the blank tile (slot 0) AND every unused padding tile in the final row
    # to palette index 0 (transparent). gbagfx's -num_tiles cap errors out if any
    # tile beyond the count holds non-transparent pixels, so the trailing padding
    # must be index 0. We declare num_tiles = full padded sheet, so only true
    # padding (beyond num_tiles) needs to be blank, but zeroing tile 0 + padding
    # keeps the sheet clean either way.
    px = quant.load()
    total_slots = rows * SHEET_WIDTH_TILES
    for slot in [0] + list(range(num_tiles, total_slots)):
        bx = (slot % SHEET_WIDTH_TILES) * TILE_PX
        by = (slot // SHEET_WIDTH_TILES) * TILE_PX
        for y in range(by, by + TILE_PX):
            for x in range(bx, bx + TILE_PX):
                px[x, y] = 0

    quant.save(out_dir / "tiles.png", format="PNG")

    # 3. Write 16 palette files (JASC-PAL). Slot 6 holds the real terrain palette;
    #    all others are zero-filled (primary tileset overwrites 0-5; 7-12 unused).
    def write_pal(idx, rgb_triples):
        lines = ["JASC-PAL", "0100", "16"]
        for r, g, b in rgb_triples:
            lines.append(f"{r} {g} {b}")
        # pad to 16 colors
        while len(lines) < 3 + 16:
            lines.append("0 0 0")
        (out_dir / "palettes" / f"{idx:02d}.pal").write_text("\n".join(lines) + "\n")

    # Build the 16-color palette list from new_pal (already has transparent at 0).
    pal16 = [(new_pal[i * 3], new_pal[i * 3 + 1], new_pal[i * 3 + 2]) for i in range(16)]
    for i in range(16):
        if i == SECONDARY_PAL_SLOT:
            write_pal(i, pal16)
        else:
            write_pal(i, [(0, 0, 0)] * 16)

    # 4. metatiles.bin: one flat metatile per usable tile (skip blank tile 0).
    #    Each metatile = 8 u16: bottom layer = 4 copies of the tile, top layer
    #    = blank tile 0 (transparent), all using palette slot 6.
    #    u16 = tileid(0-9) | hflip(10) | vflip(11) | palnum(12-15)
    metatiles = bytearray()
    attributes = bytearray()
    pal_bits = SECONDARY_PAL_SLOT << 12

    def tile_word(tile_id):
        return (tile_id & 0x3FF) | pal_bits

    blank_word = (0 & 0x3FF) | pal_bits

    # Metatile 0: fully blank (engine convention - keep index 0 empty/walkable).
    for _ in range(8):
        metatiles += struct.pack("<H", 0)
    attributes += struct.pack("<H", (MB_NORMAL << ATTR_BEHAVIOR_SHIFT) | (LAYER_NORMAL << ATTR_LAYER_SHIFT))

    for tile_id in range(1, num_tiles):
        # bottom layer: 4 copies of this tile
        for _ in range(4):
            metatiles += struct.pack("<H", tile_word(tile_id))
        # top layer: blank/transparent
        for _ in range(4):
            metatiles += struct.pack("<H", blank_word)
        behavior = tile_behaviors[tile_id]
        attr = (behavior << ATTR_BEHAVIOR_SHIFT) | (LAYER_NORMAL << ATTR_LAYER_SHIFT)
        attributes += struct.pack("<H", attr)

    (out_dir / "metatiles.bin").write_bytes(metatiles)
    (out_dir / "metatile_attributes.bin").write_bytes(attributes)

    num_metatiles = len(metatiles) // 16
    return {
        "name": name,
        "dir": cfg["dir"],
        "num_tiles": num_tiles,
        "num_metatiles": num_metatiles,
        "sheet": f"{sheet_w}x{sheet_h}",
    }


def main():
    results = []
    for name, cfg in TILESETS.items():
        print(f"Building gTileset_{name} ...")
        r = build_tileset(name, cfg)
        results.append(r)
        print(f"  -> {r['num_tiles']} tiles ({r['sheet']}), {r['num_metatiles']} metatiles")
    print("\nSummary:")
    for r in results:
        print(f"  gTileset_{r['name']:20s} dir={r['dir']:22s} "
              f"tiles={r['num_tiles']:4d} metatiles={r['num_metatiles']:4d} sheet={r['sheet']}")


if __name__ == "__main__":
    main()
