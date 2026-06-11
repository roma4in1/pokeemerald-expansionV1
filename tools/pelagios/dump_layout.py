#!/usr/bin/env python3
"""Dump a pokeemerald layout's map.bin as a grid of metatile words for inspection."""
import json, struct, sys

def load_layouts(root='.'):
    with open(f'{root}/data/layouts/layouts.json') as f:
        return {l['id']: l for l in json.load(f)['layouts'] if 'id' in l}

def load_blocks(layout):
    with open(layout['blockdata_filepath'], 'rb') as f:
        data = f.read()
    w, h = layout['width'], layout['height']
    words = struct.unpack(f'<{w*h}H', data[:w*h*2])
    return [list(words[y*w:(y+1)*w]) for y in range(h)]

if __name__ == '__main__':
    layouts = load_layouts()
    lay = layouts[sys.argv[1]]
    grid = load_blocks(lay)
    w, h = lay['width'], lay['height']
    print(f"{sys.argv[1]} {w}x{h} {lay['primary_tileset']}/{lay['secondary_tileset']}")
    print('     ' + ''.join(f'{x:5}' for x in range(w)))
    for y, row in enumerate(grid):
        print(f'{y:4} ' + ''.join(f' {v:04X}' for v in row))
