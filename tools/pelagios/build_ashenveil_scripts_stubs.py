#!/usr/bin/env python3
"""Generate stub scripts.inc for every Ashenveil map so map.json script references
resolve at link time. Mirrors build_primalis_scripts_stubs.py.

Each stub defines:
  <MapName>_MapScripts:: .byte 0      (empty map-script table)
  one minimal placeholder per unique EventScript label referenced by the map.json.

Placeholders are intentionally minimal (lockall/releaseall/end for talk/sign,
bare end for coord triggers). The pelagios-script-writer replaces these.

EXCEPTION: Greyport's *_EventScript_Tennyson is written as REAL boat-boarding
wiring (not a stub) - it sets VAR_TEMP_1 = PELAGIOS_ISLAND_ASHENVEIL and gotos the
shared boat menu. This is boat-system wiring (map-builder scope), not dialogue.

Re-runnable: WILL NOT clobber a scripts.inc that already contains real content
(heuristic: a '.string' line) unless --force.
"""
import json, os, sys

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
FORCE = '--force' in sys.argv

SHARED = {'BerryTreeScript', '0x0', ''}

TENNYSON_LABEL = 'AshenveilGreyport_EventScript_Tennyson'
TENNYSON_BODY = (
    f'@ Boat-boarding wiring (map-builder owns this; NOT dialogue).\n'
    f'{TENNYSON_LABEL}::\n'
    '\tlockall\n'
    '\tsetvar VAR_TEMP_1, PELAGIOS_ISLAND_ASHENVEIL\n'
    '\tgoto Pelagios_EventScript_BoardTennyson\n'
    '\tend\n'
)

def collect_labels(m):
    labels = []
    seen = set()
    def add(s, kind):
        if not s or s in SHARED or s.startswith('0x'):
            return
        if s not in seen:
            seen.add(s); labels.append((s, kind))
    for o in m.get('object_events', []):
        add(o.get('script'), 'talk')
    for c in m.get('coord_events', []):
        add(c.get('script'), 'trigger')
    for b in m.get('bg_events', []):
        add(b.get('script'), 'talk')
    return labels

def is_real(path):
    if not os.path.exists(path):
        return False
    return '.string' in open(path).read()

written = []
for d in sorted(os.listdir(os.path.join(ROOT, 'data/maps'))):
    if not d.startswith('Ashenveil_'):
        continue
    mp = os.path.join(ROOT, 'data/maps', d, 'map.json')
    if not os.path.exists(mp):
        continue
    m = json.load(open(mp))
    sp = os.path.join(ROOT, 'data/maps', d, 'scripts.inc')
    if is_real(sp) and not FORCE:
        continue
    labels = collect_labels(m)
    lines = [f'{d}_MapScripts::', '\t.byte 0', '']
    for lab, kind in labels:
        if lab == TENNYSON_LABEL:
            lines.append(TENNYSON_BODY)
            continue
        lines.append(f'@ TODO (script-writer): implement {lab}')
        lines.append(f'{lab}::')
        if kind == 'trigger':
            lines.append('\tend')
        else:
            lines.append('\tlockall')
            lines.append('\treleaseall')
            lines.append('\tend')
        lines.append('')
    open(sp, 'w').write('\n'.join(lines) + '\n')
    written.append((d, len(labels)))

for d, n in written:
    print(f'{d}: {n} stub labels')
print(f'{len(written)} scripts.inc stubs written')
