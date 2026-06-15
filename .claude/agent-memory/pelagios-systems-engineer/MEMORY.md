# Pelagios Systems Engineer — Memory Index

- [Trainer flag space EXPANDED](project_trainer_flag_space_full.md) — was full at 864, raised to 1024 (156 free); how the SYSTEM_FLAGS-floats expansion works
- [Constant space budget](project_constant_space_budget.md) — free flag/var/item/trainer space + ceilings (post-refactor: vars to 0x410F, trainers to 1024, SAVE-BREAKING)
- [Boat destination menu](project_boat_destination_menu.md) — tier-gated Tennyson multichoice system; how it's built and how to add a new island to it
- [MAPSEC u8 ceiling — RESOLVED](project_mapsec_u8_ceiling.md) — FIXED 2026-06-14: metLocation handler widened to u16, stored field repacked to 9-bit (ceiling 508); Kanto kept. ALSO: mapjson.cpp MUST emit map-header region section as `.2byte` not `.byte` — DO NOT REGRESS.
- [Primalis script build-blocker](project_primalis_script_build_blocker.md) — untracked Primalis_VerdantLanding/scripts.inc has invalid SE_M_GROWL; breaks whole-tree link on event_scripts rebuild (script-writer fix)
- [Following Pokémon enabled](project_following_pokemon.md) — OW_FOLLOWERS_ENABLED TRUE; global config, no per-map field; +0 RAM
- [Field item runs a script](project_field_item_runs_script_pattern.md) — key item bag "Use" -> fixed event script, map-agnostic; mirror ItemUseOutOfBattle_PokemonBoxLink (Storm Compass)
