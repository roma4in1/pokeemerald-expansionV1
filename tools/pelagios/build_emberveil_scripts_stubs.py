#!/usr/bin/env python3
"""Generate stub scripts.inc for every Emberveil map so map.json script references
resolve at link time. Each stub defines:
  <MapName>_MapScripts:: .byte 0      (empty map-script table)
  one minimal placeholder per unique EventScript label referenced by the map.json
    (objects, warps would not, signs, coord triggers, bg signs).

Placeholders are intentionally minimal (lock/release/end or a TODO msgbox). The
pelagios-script-writer replaces these with real content. Shared vanilla labels
(BerryTreeScript) are NOT redefined.

Re-runnable: overwrites the stub files. Will NOT clobber a scripts.inc that already
contains real content (heuristic: presence of a '.string' line) unless --force.
"""
import json, os, sys, re

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
FORCE = '--force' in sys.argv

SHARED = {'BerryTreeScript', '0x0', ''}

def collect_labels(m):
    # returns list of (label, kind) where kind in {'talk','trigger'}
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
        add(b.get('script'), 'talk')   # signs use msgbox; lock/release stub is safe
    return labels

def is_real(path):
    if not os.path.exists(path):
        return False
    txt = open(path).read()
    return '.string' in txt

written = []
for d in sorted(os.listdir(os.path.join(ROOT, 'data/maps'))):
    if not d.startswith('Emberveil_'):
        continue
    mp = os.path.join(ROOT, 'data/maps', d, 'map.json')
    if not os.path.exists(mp):
        continue
    m = json.load(open(mp))
    sp = os.path.join(ROOT, 'data/maps', d, 'scripts.inc')
    if is_real(sp) and not FORCE:
        continue  # already has real content
    labels = collect_labels(m)
    lines = []
    lines.append(f'{d}_MapScripts::')
    lines.append('\t.byte 0')
    lines.append('')
    for lab, kind in labels:
        lines.append(f'@ TODO (script-writer): implement {lab}')
        lines.append(f'{lab}::')
        if kind == 'trigger':
            # engine-invoked coord triggers: bare end (no lock context to release)
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
