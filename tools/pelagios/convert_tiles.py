#!/usr/bin/env python3
"""
Solar Eclipse → Pokémon Pelagios Tile Converter
Converts high-color Essentials tileset PNGs to GBA-compatible
indexed PNGs (16 colors max per palette group).

Usage:
  python3 convert_tiles.py <input.png> <output_dir>
  python3 convert_tiles.py --batch <input_dir> <output_dir>

Output: indexed PNG ready for gbagfx conversion
"""

import os
import sys
import argparse
from pathlib import Path
from PIL import Image

# GBA tileset constraints
GBA_TILE_SIZE = 8          # 8x8 pixels per tile
GBA_COLORS_PER_PALETTE = 16  # including transparent
GBA_MAX_PALETTES = 16
GBA_TOTAL_COLORS = GBA_COLORS_PER_PALETTE * GBA_MAX_PALETTES  # 256

# Pelagios island tile groupings
# Tells the converter which tiles belong together and share palettes
ISLAND_GROUPS = {
    "haven_isle_coastal": [
        "HGSS_Beach.png",
        "HGSS_Beach_Corner.png",
        "HGSS_Waves.png",
        "HGSS_Sea.png",
        "Sand_shore.png",
        "Sea.png",
        "Sea_without_shore.png",
    ],
    "sirocco_desert": [
        "Sand.png",
        "Sandy_Dirt.png",
        "Dirt.png",
        "Gravel.png",
        "Brown_cave_sand.png",
        "Light_grass.png",
    ],
    "emberveil_volcanic": [
        "Magma_Lava.png",
        "Magma_Water.png",
        "Red_cave_floor.png",
        "Red_cave_highlight.png",
        "Dirt_cave_highlight.png",
    ],
    "schism_ice": [
        "Snow_cave_floor.png",
        "Snow_cave_highlight.png",
        "Snow_cave_ice_border.png",
        "White_cave_floor.png",
        "White_cave_highlight.png",
        "White_path.png",
    ],
    "schism_poison": [
        "Blue_cave_floor.png",
        "Blue_cave_mud.png",
        "Green_cave_floor.png",
        "Brown_cave_floor.png",
    ],
    "thalvern_underwater": [
        "Underwater_dark.png",
        "Seaweed_dark.png",
        "Seaweed_light.png",
        "Sea_deep.png",
        "Still_water.png",
    ],
    "general_caves": [
        "Brown_cave_floor.png",
        "Dirt_cave_highlight.png",
        "Dungeon_floor.png",
        "Dungeon_wall.png",
    ],
    "general_water": [
        "Calm_Water.png",
        "Calm_Water2.png",
        "Water_current_east.png",
        "Water_current_north.png",
        "Water_current_south.png",
        "Water_current_west.png",
        "Water_rock.png",
        "Waterfall.png",
        "Waterfall_bottom.png",
        "Waterfall_crest.png",
        "New_Waterfall.png",
    ],
    "general_grass": [
        "GrassSpring.png",
        "GrassSummer.png",
        "GrassAutumn.png",
        "GrassWinter.png",
        "GrassFairy.png",
        "Flowers1.png",
        "Flowers2.png",
    ],
}

def quantize_to_gba(img: Image.Image, n_colors: int = 16,
                    transparent_color=None) -> Image.Image:
    """
    Quantize an RGB image to n_colors indexed colors.
    Ensures transparent color is at palette index 0 (GBA requirement).
    """
    # Handle palette mode (P) — convert to RGB first
    if img.mode == "P":
        img = img.convert("RGB")

    if img.mode == "RGBA":
        # Extract transparency mask
        rgba = img
        img = rgba.convert("RGB")
        has_alpha = True
        alpha = rgba.split()[3]
    else:
        img = img.convert("RGB")
        has_alpha = False
        alpha = None

    # Quantize to target color count
    # Reserve index 0 for transparency
    quantized = img.quantize(colors=n_colors - 1 if has_alpha else n_colors,
                              method=Image.Quantize.MEDIANCUT,
                              dither=Image.Dither.FLOYDSTEINBERG)

    if has_alpha:
        # Build new palette with transparent color at index 0
        palette_data = quantized.getpalette()
        # Shift all palette entries up by one slot
        new_palette = [0, 0, 0] + palette_data[:((n_colors - 1) * 3)]
        quantized = quantized.point(lambda x: x + 1)
        quantized.putpalette(new_palette)

        # Set fully transparent pixels to index 0
        pixels = quantized.load()
        alpha_pixels = alpha.load()
        for y in range(quantized.height):
            for x in range(quantized.width):
                if alpha_pixels[x, y] < 128:
                    pixels[x, y] = 0

    return quantized

def validate_tile_dimensions(img: Image.Image, filename: str) -> bool:
    """Check that image dimensions are multiples of 8 (GBA tile size)."""
    w, h = img.size
    if w % GBA_TILE_SIZE != 0 or h % GBA_TILE_SIZE != 0:
        print(f"  WARNING: {filename} is {w}x{h}px — not a multiple of 8.")
        print(f"  Will crop to {(w // 8) * 8}x{(h // 8) * 8}px")
        return False
    return True


def crop_to_tile_boundary(img: Image.Image) -> Image.Image:
    """Crop image to nearest tile boundary."""
    w, h = img.size
    new_w = (w // GBA_TILE_SIZE) * GBA_TILE_SIZE
    new_h = (h // GBA_TILE_SIZE) * GBA_TILE_SIZE
    return img.crop((0, 0, new_w, new_h))


def convert_single(input_path: Path, output_path: Path,
                   n_colors: int = 16, verbose: bool = True):
    """Convert a single tile PNG to GBA-compatible indexed PNG."""
    if verbose:
        print(f"Converting: {input_path.name}")

    img = Image.open(input_path)
    original_size = img.size
    original_mode = img.mode

    # Crop to tile boundary if needed
    if not validate_tile_dimensions(img, input_path.name):
        img = crop_to_tile_boundary(img)

    # Count original colors
    rgb = img.convert("RGB")
    colors = rgb.getcolors(maxcolors=1000000)
    n_original = len(colors) if colors else 999999

    if verbose:
        print(f"  Original: {original_size[0]}x{original_size[1]}px, "
              f"{original_mode}, ~{n_original} colors")

    # Quantize
    quantized = quantize_to_gba(img, n_colors=n_colors)

    # Verify output
    final_colors = len(set(quantized.getdata()))
    if verbose:
        print(f"  Output:   {quantized.size[0]}x{quantized.size[1]}px, "
              f"indexed, {final_colors} colors (max {n_colors})")

    # Save
    output_path.parent.mkdir(parents=True, exist_ok=True)
    # Strip tuple transparency metadata that breaks PNG save on some palette images
    if "transparency" in quantized.info and isinstance(quantized.info["transparency"], tuple):
        del quantized.info["transparency"]
    quantized.save(output_path, format="PNG")
    if verbose:
        print(f"  Saved:    {output_path}")

    return quantized


def convert_batch(input_dir: Path, output_dir: Path,
                  n_colors: int = 16):
    """Convert all PNG files in a directory."""
    pngs = list(input_dir.glob("*.png"))
    if not pngs:
        print(f"No PNG files found in {input_dir}")
        return

    print(f"Converting {len(pngs)} files from {input_dir}")
    print(f"Output directory: {output_dir}")
    print(f"Target colors per tile: {n_colors}")
    print()

    success = 0
    failed = []

    for png in sorted(pngs):
        output_path = output_dir / png.name
        try:
            convert_single(png, output_path, n_colors=n_colors)
            success += 1
            print()
        except Exception as e:
            print(f"  ERROR: {e}")
            failed.append(png.name)
            print()

    print(f"Done: {success}/{len(pngs)} converted successfully")
    if failed:
        print(f"Failed: {', '.join(failed)}")


def convert_group(input_dir: Path, output_dir: Path,
                  group_name: str):
    """Convert a named island group of tiles."""
    if group_name not in ISLAND_GROUPS:
        print(f"Unknown group: {group_name}")
        print(f"Available groups: {', '.join(ISLAND_GROUPS.keys())}")
        return

    tiles = ISLAND_GROUPS[group_name]
    group_output = output_dir / group_name
    group_output.mkdir(parents=True, exist_ok=True)

    print(f"Converting group: {group_name}")
    print(f"Tiles: {len(tiles)}")
    print()

    for tile_name in tiles:
        input_path = input_dir / tile_name
        if not input_path.exists():
            print(f"  MISSING: {tile_name}")
            continue
        output_path = group_output / tile_name
        try:
            convert_single(input_path, output_path)
            print()
        except Exception as e:
            print(f"  ERROR converting {tile_name}: {e}")
            print()


def list_groups():
    """Print available island groups."""
    print("Available island tile groups:")
    print()
    for group, tiles in ISLAND_GROUPS.items():
        print(f"  {group}:")
        for tile in tiles:
            print(f"    - {tile}")
        print()


def main():
    parser = argparse.ArgumentParser(
        description="Convert Solar Eclipse tiles to GBA-compatible format"
    )
    parser.add_argument("--list-groups", action="store_true",
                        help="List available island tile groups")
    parser.add_argument("--batch", action="store_true",
                        help="Convert all PNGs in input directory")
    parser.add_argument("--group", type=str,
                        help="Convert a named island group")
    parser.add_argument("--colors", type=int, default=16,
                        help="Colors per palette (default: 16, GBA max: 16)")
    parser.add_argument("input", nargs="?",
                        help="Input file or directory")
    parser.add_argument("output", nargs="?",
                        help="Output file or directory")

    args = parser.parse_args()

    if args.list_groups:
        list_groups()
        return

    if args.colors > GBA_COLORS_PER_PALETTE:
        print(f"WARNING: GBA supports max {GBA_COLORS_PER_PALETTE} colors "
              f"per palette. Clamping to {GBA_COLORS_PER_PALETTE}.")
        args.colors = GBA_COLORS_PER_PALETTE

    if args.group:
        input_dir = Path(args.input) if args.input else Path(".")
        output_dir = Path(args.output) if args.output else Path("./converted")
        convert_group(input_dir, output_dir, args.group)
        return

    if not args.input:
        parser.print_help()
        return

    input_path = Path(args.input)
    output_path = Path(args.output) if args.output else Path("./converted")

    if args.batch or input_path.is_dir():
        convert_batch(input_path, output_path, n_colors=args.colors)
    else:
        if output_path.is_dir():
            output_path = output_path / input_path.name
        convert_single(input_path, output_path, n_colors=args.colors)


if __name__ == "__main__":
    main()
