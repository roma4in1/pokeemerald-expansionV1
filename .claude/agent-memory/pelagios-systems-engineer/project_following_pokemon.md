---
name: following-pokemon
description: Following Pokémon (overworld follower) enabled globally via OW_FOLLOWERS_ENABLED; gated by config not map data; zero RAM cost
metadata:
  type: project
---

Following Pokémon enabled 2026-06-14 by flipping ONE define in include/config/overworld.h:
`OW_FOLLOWERS_ENABLED` FALSE -> TRUE. Prereq `OW_POKEMON_OBJECT_EVENTS` was already TRUE.
There is NO `OW_FOLLOWERS_POKEMON` define in this expansion — `OW_FOLLOWERS_ENABLED` is the
single documented enable.

**Why:** turn on the HGSS-style overworld follower for all Pelagios maps.

**How to apply:** follower gating is GLOBAL, not per-map. struct MapHeader has no follower
bit; map.json has no allow_followers field; tools/mapjson/mapjson.cpp has none. Spawning is
decided at runtime in UpdateFollowingPokemon() (src/event_object_movement.c ~2354) via the
config + B_FLAG_FOLLOWERS_DISABLED / FLAG_TEMP_HIDE_FOLLOWER flags. So NO per-map json edits
and NO tools/pelagios/build_*_mapjson.py generator edits are ever needed for followers.
Indoors, a follower whose gfx > 32x32 is auto-removed (no assert). RAM cost is ZERO (the
follower object-event slot is statically allocated regardless of the toggle); enabling cost
only +544 B ROM. In-emulator behavior on water/warps/indoor was NOT tested. Note the build
was blocked at verification time by an UNRELATED script bug — see [[primalis-script-build-blocker]].
