---
name: link-time-gotchas
description: Constant names and validations that cause assembly/link failures in Pelagios map data - verify before building
metadata:
  type: feedback
---

map.json fields and event scripts reference C constants by name; a wrong name passes
assembly but fails at LINK with "undefined reference". Verify these against the headers
BEFORE building (grep the include/ tree):

- battle_scene: only MAP_BATTLE_SCENE_NORMAL / _GYM / _MAGMA / _AQUA / elite-four exist.
  There is NO `MAP_BATTLE_SCENE_BUILDING` or `_CAVE` (the IRONHOLD_BRIEF named these but
  they don't exist). Use _GYM for gym battle maps, _NORMAL otherwise.
- weather: WEATHER_SHADE is the overcast value (NOT `WEATHER_CLOUDS`). WEATHER_SUNNY_CLOUDS=1.
- music: many routes share MUS_ROUTE110; MUS_ROUTE111 does NOT exist as a symbol.
- regional-form species use the suffixed form: SPECIES_GEODUDE_ALOLA (not _ALOLAN),
  SPECIES_FARFETCHD_GALAR (not _GALARIAN).
- object gfx: OBJ_EVENT_GFX_OLD_MAN_1 exists; OBJ_EVENT_GFX_MAUVILLE_OLD_MAN_1 does not.

bg_hidden_item_event REQUIRES a flag >= FLAG_HIDDEN_ITEMS_START (asm/macros/map.inc has a
`.if \flag < FLAG_HIDDEN_ITEMS_START / .error` guard). A custom FLAG_ITEM_* in the wrong
range fails assembly with "non-constant expression in .if" / "*UND* section". If the
island's hidden-item flags don't exist yet, DEFER the hidden item (leave a TODO) rather
than inventing a constant - constants are the systems-engineer's lane.

**Why:** Writing constants (flags/items/gfx) is explicitly out of the map-builder's lane;
the brief's named constants are sometimes aspirational, not real symbols.

**How to apply:** When a link error names an undefined symbol, grep the header for the
real name and patch the generator; if it's a flag/item/gfx that genuinely doesn't exist,
substitute a valid placeholder (e.g. FLAG_UNUSED_0xNNN for a hide flag, flag 0 for
always-visible) and record a TODO in CLAUDE.md for systems-engineer + script-writer.

VALIDATE before building: (1) every object/warp/trigger tile is walkable (collision bits
== 0) - a trigger on a wall never fires, an NPC on a sign tile overlaps; (2) connection
offsets align openings: for vertical, offset = currentMap_open_minX - connectedMap_open_minX
(verified against Haven: Village.up->Route1 offset 2, Village top-open x=10, Route1
bottom-open x=8). Reverse connection uses the negated offset. A map can have only ONE
connection per direction.
