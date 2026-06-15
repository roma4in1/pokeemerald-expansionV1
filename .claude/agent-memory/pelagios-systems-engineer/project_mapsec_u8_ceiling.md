---
name: mapsec-u8-ceiling
description: RESOLVED 2026-06-14 - the MAPSEC u8 ceiling was cleared by widening the metLocation handler type to u16 and repacking the stored field to a 9-bit bitfield (ceiling now 508). Kanto NOT removed.
metadata:
  type: project
---

**STATUS: RESOLVED 2026-06-14** (was: blocking Primalis+ MAPSECs).

The Pelagios MAPSEC (RegionMapSecId) enum hit its u8 ceiling at Gildhaven
(MAPSEC_NONE == 255). This blocked Primalis and all later islands from adding
region-map sections. FIXED this way (NOT the originally-planned "remove Kanto"):

**What was done (the chosen approach):**
- `mapsec_u8_t` / `metloc_u8_t` (include/gametypes.h) widened u8 -> **u16**. Every
  variable/parameter that *handles* a MAPSEC id or Pokemon Met Location is now
  16-bit, so it can hold ids past 255 and the relocated sentinels.
- The METLOC_* sentinels moved from 0xFD/0xFE/0xFF to **0x1FD/0x1FE/0x1FF**
  (top of the 9-bit range), in src/data/region_map/region_map_sections.constants.json.txt
  (the .h is generated from it). So a real MAPSEC id can never collide with them.
- The **stored** Pokemon metLocation could NOT become a full u16: BoxPokemon
  cannot grow (the PC box storage save region is at its flash budget; a wider
  BoxPokemon trips the `PokemonStorageFreeSpace` static-assert in src/save.c, and
  also `RecordedBattleSaveFreeSpace` in recorded_battle.c). So the stored field is
  a **9-bit bitfield** (`u32 metLocation:9`) repacked into the existing 12-byte
  `struct PokemonSubstruct3` (include/pokemon.h), reclaiming the substruct's former
  `unused_0B:1`. The substruct stays EXACTLY 12 bytes -> BoxPokemon unchanged ->
  no save-region overflow.

**The new ceiling: 508.** MAPSEC ids may be 0-508; sentinels are 509-511.
MAPSEC_NONE is currently 255 (Kanto kept), so there are **253 free MAPSEC slots** -
ample for Primalis (7) + every remaining island. If MAPSECs ever needed to exceed
508 the bitfield would have to widen (and then BoxPokemon flash budget becomes the
real wall) - that will never happen for this project.

**Why Kanto was NOT removed** (the brief's Option B): the merged FRLG region is
fully compiled and live - ~400 FRLG map.json files set Kanto MAPSECs as their
region_map_section, and src/region_map.c + src/regions.c have the Kanto/Sevii
region-map layouts hardcoded. Removing the 105 Kanto MAPSEC entries would break all
of that. The brief itself said "if a Kanto MAPSEC is referenced by retained FRLG
map data, leave it." Widening to u16 clears the ceiling without that risk, so Kanto
removal became unnecessary.

**Key code facts (verify before relying on):**
- metLocation accessor: src/pokemon.c SetBoxMonData MON_DATA_MET_LOCATION uses
  **SET16** now (was SET8) so the u16 round-trips. GetBoxMonData returns the 9-bit
  field into a u32 - fine.
- Every `SetMonData(MON_DATA_MET_LOCATION, &x)` caller must pass a >=2-byte `x`
  (SET16 reads 2 bytes). Fixed: scrcmd.c `location` -> metloc_u8_t; battle_controllers.c
  REQUEST_MET_LOCATION_BATTLE GET now emits 2 bytes (size=2). daycare/egg_hatch/trade
  already used metloc_u8_t; pokemon.c CreateBoxMon uses u32 `value`.
- Header/.c signature mismatches the widen surfaced (pre-existing): map_preview_screen.h
  (MapPreview_CreateMapNameWindow/_GetDuration) and pokenav.h (MatchCall_GetMapSec)
  declared `u8` while the .c used `mapsec_u8_t` - fixed to the typedef.

**MAP-HEADER ASM EMITTER FOLLOW-UP (2026-06-14) — DO NOT REGRESS.** Widening
`struct MapHeader.regionMapSectionId` (include/global.fieldmap.h) to u16 (via
mapsec_u8_t) means the ROM map-header asm MUST emit the region section as a
**16-bit halfword**. The emitter `tools/mapjson/mapjson.cpp` (the ONLY raw-asm
MapHeader emitter; ~line 167) was left writing `\t.byte ` for `region_map_section`,
which (1) truncated MAPSEC>=256 (Primalis 0x100-0x105, Ashenveil 0x106-0x10A all
stored low-byte-only) AND (2) silently misaligned EVERY map's header tail by 1 byte
(cave/weather/mapType/floorNumber/flags/battleType read off-by-one vs the u16 struct).
FIX: emitter now writes `\t.2byte ` for the region section — this both stops truncation
and realigns the tail to the struct (requires_flash->cave 0x16, weather 0x17, mapType
0x18, floorNumber 0x19, filler 0x1A, map_header_flags bitfield 0x1B, battle_scene 0x1C).
The `map_header_flags` macro (asm/macros/map.inc) is already a correct single `.byte`
at 0x1B — leave it. There is NO hand-written `map_header` macro; the 9 Pelagios
build_*_mapjson.py generators feed region_map_section as a string into mapjson.cpp, so
fixing the tool fixes all islands. To regenerate: `rm -f tools/mapjson/mapjson` +
delete all data/maps/*/header.inc (and build/**/header.inc) + gmake. ROM-only (headers
are not saveblock) — NO new save break. RULE: never emit a MapHeader region section as
`.byte`, and never revert mapsec_u8_t to u8.

SAVE-BREAK (the substruct widening above, NOT the header fix): the substruct bit
layout changed -> all old test saves invalid. Accepted (dev phase). See
[[constant-space-budget]] and CLAUDE.md SAVE-COMPATIBILITY lineage. After the header
fix: build clean (gmake exit 0) EWRAM 86.46% / IWRAM 86.63% / ROM 80.03%.
