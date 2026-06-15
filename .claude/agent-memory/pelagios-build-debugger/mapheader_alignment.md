---
name: mapheader-alignment
description: Map-header asm must be 4-byte aligned per label AND emit .2byte for region_map_section, or the map tilemap won't load (black-screen boot freeze)
metadata:
  type: project
---

The map-header ASM emitter is `tools/mapjson/mapjson.cpp` (the ONLY raw-asm MapHeader
emitter; the 9 Pelagios `build_*_mapjson.py` generators feed JSON into it).

**Two requirements that must BOTH hold (regression-prone):**
1. `region_map_section` emitted as `\t.2byte ` (u16 — `mapsec_u8_t` was widened to u16;
   MAPSEC ids can be >= 256 on Primalis/Ashenveil). Reverting to `.byte` truncates and
   misaligns the header tail. See [[charmap-no-emdash]] sibling rules in CLAUDE.md.
2. `\t.align 2\n` (= 4-byte align; GAS/ARM `.align` is 2^N) emitted IMMEDIATELY BEFORE
   each `<MapName>:` header label.

**Why #2 is mandatory:** headers.inc packs headers back-to-back with no padding. With
`.2byte` the emitted header body is **0x1D bytes (odd)**, so without `.align 2` every
header after the first drifts to a non-4-aligned address. `struct MapHeader` starts with
`mapLayout` (a pointer at 0x00) read by a 32-bit LDR; on ARM7TDMI an unaligned LDR
**rotates the word** (no fault) → garbage layout pointer → tileset/tilemap never loads →
**black screen, player sprite still visible** at boot. Pre-`.2byte` the body was 0x1C
(=28, multiple of 4) so alignment held BY LUCK without `.align`.

**Verify after any header-emitter change:** `arm-none-eabi-nm pokeemerald.elf | grep ' r <MapName>$'`
— every map header address must end in 0/4/8/c. struct/asm offsets match field-for-field
(offsetof confirms): mapLayout 0x00 … regionMapSectionId 0x14 (.2byte) … battleType 0x1C,
sizeof 0x20. `map_header_flags` macro = 1 byte at 0x1B.

Regen-from-clean recipe: `rm -f tools/mapjson/mapjson` + `find data/maps -name header.inc -delete`
+ `find build -name header.inc -delete` + `gmake`.

Fixed 2026-06-14 (boot black-screen freeze). The `.2byte` change alone was correct; the
freeze was the missing per-header `.align 2` it exposed.
