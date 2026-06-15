#!/usr/bin/env python3
"""Append Ashenveil's 3 Ghost-type wild encounter tables to wild_encounters.json.

Ashenveil has wild encounters in ONLY 3 maps (per ASHENVEIL_BRIEF.md):
  AshFields1        : Gastly 40 / Shuppet 30 / Duskull 20 / Honedge 10
  DeadCity_Exterior : Misdreavus 40 / Sableye 30 / Runerigus 20 / Cursola 10
  MorthasGrove      : Phantump 50 / Spiritomb 30 / Dragapult 20  (pre-encounter)

12-slot land tables (standard Hoenn land-slot weighting: slots 0-1 = 20% each,
2-3 = 10% each, 4-5 = 5%, 6-7 = 5%, 8-9 = 4%, 10 = 1%, 11 = 1%). We map the
brief's percentages onto the 12 slots by repeating species across slots so the
aggregate matches: 40% -> slots 0-3? No - the canonical Emerald slot weights are
[20,20,10,10,10,10,5,5,5,5,4,1]. To hit the brief's 40/30/20/10 we fill:
  40% species -> slots 0,1 (20+20)
  30% species -> slots 2,3,4 (10+10+10)
  20% species -> slots 5,6,7? (10+5+5)=20
  10% species -> slots 8,9,10,11 (5+4+1)=10  -> close enough (we use 8,9,10,11)
Levels per the dead-island late-game placement (post-Galleon, ~lv48-54).
No water/fishing tables (the brief defines none).

Idempotent: replaces any existing gAshenveil* encounter entries by map id.
"""
import json, os

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
WILD = os.path.join(ROOT, 'src/data/wild_encounters.json')

# canonical Emerald 12 land slots
SLOT_W = [20, 20, 10, 10, 10, 10, 5, 5, 5, 5, 4, 1]

def land(mons_pcts, lmin, lmax):
    """mons_pcts: list of (species, pct). Distribute across 12 slots by weight."""
    # greedily assign slots to species to approximate pct
    slots = []
    pcts = [[s, p] for s, p in mons_pcts]
    for w in SLOT_W:
        # pick the species with the largest remaining pct
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
        'map': 'MAP_ASHENVEIL_ASH_FIELDS1',
        'base_label': 'gAshenveilAshFields1',
        'land_mons': land([
            ('SPECIES_GASTLY', 40), ('SPECIES_SHUPPET', 30),
            ('SPECIES_DUSKULL', 20), ('SPECIES_HONEDGE', 10),
        ], 48, 51),
    },
    {
        'map': 'MAP_ASHENVEIL_DEAD_CITY_EXTERIOR',
        'base_label': 'gAshenveilDeadCityExterior',
        'land_mons': land([
            ('SPECIES_MISDREAVUS', 40), ('SPECIES_SABLEYE', 30),
            ('SPECIES_RUNERIGUS', 20), ('SPECIES_CURSOLA', 10),
        ], 50, 53),
    },
    {
        'map': 'MAP_ASHENVEIL_MORTHAS_GROVE',
        'base_label': 'gAshenveilMorthasGrove',
        'land_mons': land([
            ('SPECIES_PHANTUMP', 50), ('SPECIES_SPIRITOMB', 30),
            ('SPECIES_DRAGAPULT', 20),
        ], 52, 55),
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
print(f'merged {len(ENTRIES)} Ashenveil wild tables into wild_encounters.json')
for e in ENTRIES:
    print(' ', e['base_label'], [m['species'] for m in e['land_mons']['mons']])
