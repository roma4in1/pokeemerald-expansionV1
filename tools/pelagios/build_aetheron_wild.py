#!/usr/bin/env python3
"""Append Aetheron's 2 Electric/Flying wild encounter tables to wild_encounters.json.

Aetheron has wild encounters in ONLY 2 maps (per AETHERON_BRIEF.md):
  SkyRoute  : Emolga 40 / Swablu 30 / Togetic 20 / Jolteon 10
  StormPeak : Electabuzz 40 / Manectric 30 / Vikavolt 20 / Xurkitree 10

12-slot land tables (canonical Emerald slot weights). The 40/30/20/10 brief split
is distributed greedily across the 12 slots so the aggregate matches. Levels per
the sky-island late-game placement (post-Ashenveil, ~lv53-59). No water/fishing.

Idempotent: replaces any existing gAetheron* encounter entries by map id.
"""
import json, os

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
WILD = os.path.join(ROOT, 'src/data/wild_encounters.json')

# canonical Emerald 12 land slots
SLOT_W = [20, 20, 10, 10, 10, 10, 5, 5, 5, 5, 4, 1]

def land(mons_pcts, lmin, lmax):
    slots = []
    pcts = [[s, p] for s, p in mons_pcts]
    for w in SLOT_W:
        pcts.sort(key=lambda sp: -sp[1])
        sp = pcts[0]
        sp[1] -= w
        slots.append(sp[0])
    return {
        'encounter_rate': 20,
        'mons': [{'min_level': lmin, 'max_level': lmax, 'species': s} for s in slots],
    }

ENTRIES = [
    {
        'map': 'MAP_AETHERON_SKY_ROUTE',
        'base_label': 'gAetheronSkyRoute',
        'land_mons': land([
            ('SPECIES_EMOLGA', 40), ('SPECIES_SWABLU', 30),
            ('SPECIES_TOGETIC', 20), ('SPECIES_JOLTEON', 10),
        ], 53, 56),
    },
    {
        'map': 'MAP_AETHERON_STORM_PEAK',
        'base_label': 'gAetheronStormPeak',
        'land_mons': land([
            ('SPECIES_ELECTABUZZ', 40), ('SPECIES_MANECTRIC', 30),
            ('SPECIES_VIKAVOLT', 20), ('SPECIES_XURKITREE', 10),
        ], 56, 59),
    },
]

with open(WILD) as f:
    doc = json.load(f)
grp = [g for g in doc['wild_encounter_groups'] if g['label'] == 'gWildMonHeaders'][0]
enc = grp['encounters']
by_map = {e['map']: i for i, e in enumerate(enc)}
for e in ENTRIES:
    if e['map'] in by_map:
        enc[by_map[e['map']]] = e
    else:
        enc.append(e)
with open(WILD, 'w') as f:
    json.dump(doc, f, indent=2)
    f.write('\n')
print(f'merged {len(ENTRIES)} Aetheron wild tables into wild_encounters.json')
for e in ENTRIES:
    print(' ', e['base_label'], [m['species'] for m in e['land_mons']['mons']])
