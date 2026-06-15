@ ===================================================================
@ Pelagios — shared Tennyson destination-select system
@ ===================================================================
@ One menu, wired into every harbor's boarding bg_events. The menu is
@ gated by VAR_BOAT_TIER (0x4100): 0=Dinghy, 1=Sloop, 2=Brigantine,
@ 3=Galleon. Higher tiers reveal more islands.
@
@ HOW A HARBOR CALLS THIS:
@   Each harbor's *_EventScript_Tennyson does:
@       lockall
@       setvar VAR_TEMP_1, PELAGIOS_ISLAND_<thisharbor>
@       (optional intro/flavor msgbox here)
@       goto Pelagios_EventScript_BoardTennyson
@   VAR_TEMP_1 tells the menu which island we are currently moored at, so
@   it can answer "You're already moored here." instead of warping to the
@   island the player is already standing on. VAR_TEMP_* clears on warp, so
@   it never persists.
@
@ TIER GATING: tier 0 (Dinghy) never reaches a harbor with this menu
@   (Haven's pre-Sloop boarding handles that itself), but it is handled
@   defensively below by routing to the Sloop list with a safety bump.
@
@ ADDING A NEW ISLAND (e.g. Schism once it is built):
@   1. Give it a PELAGIOS_ISLAND_* id below.
@   2. Replace its stub dispatch case with a real
@      Pelagios_EventScript_SailTo<Island> (warp to that port's arrival
@      tile). Keep the menu-index order in sync with the matching list in
@      src/data/script_menu.h (MultichoiceList_Boat*).
@   3. In that island's harbor scripts.inc, add a *_EventScript_Tennyson
@      that sets VAR_TEMP_1 + goto Pelagios_EventScript_BoardTennyson, and
@      point its boarding bg_events at it.
@ ===================================================================

@ Island ids (stored in VAR_TEMP_1 by the calling harbor).
.set PELAGIOS_ISLAND_HAVEN,     0
.set PELAGIOS_ISLAND_IRONHOLD,  1
.set PELAGIOS_ISLAND_SIROCCO,   2
.set PELAGIOS_ISLAND_EMBERVEIL, 3
@ Schism Isle has TWO ports (the Split island): FrostmarkPort (ice/north) and
@ VenomquayPort (poison/south). They get distinct island ids so the same-island
@ "already moored here" guard works correctly from either port. The boat menu
@ lists both as separate destinations ("SCHISM (NORTH)" / "SCHISM (SOUTH)").
.set PELAGIOS_ISLAND_SCHISM,        4   @ Frostmark / ice / north
.set PELAGIOS_ISLAND_SCHISM_SOUTH,  7   @ Venomquay / poison / south
.set PELAGIOS_ISLAND_THALVERN,      5
.set PELAGIOS_ISLAND_GILDHAVEN,     6
.set PELAGIOS_ISLAND_PRIMALIS,      8   @ Beast Island (Galleon)
.set PELAGIOS_ISLAND_ASHENVEIL,     9   @ Dead Island (Galleon)
.set PELAGIOS_ISLAND_AETHERON,     10   @ Sky Island — unlocked by ITEM_SEA_CHART / FLAG_SEA_CHART_FOUND, not boat tier
.set PELAGIOS_ISLAND_CONVERGENCE,  11   @ Final Island — unlocked by FLAG_AETHERON_RESOLVED, not boat tier

@ -------------------------------------------------------------------
@ Entry point. Caller has already lockall'd and set VAR_TEMP_1.
@ Branches to the multichoice list for the current boat tier.
@ -------------------------------------------------------------------
Pelagios_EventScript_BoardTennyson::
	switch VAR_BOAT_TIER
	case 3, Pelagios_EventScript_BoatMenuGalleon
	case 2, Pelagios_EventScript_BoatMenuBrigantine
	@ tiers 0 and 1 both show the Sloop menu (tier 0 should never reach a
	@ shared harbor menu, but never strand the player if it does).
	goto Pelagios_EventScript_BoatMenuSloop

@ -------------------------------------------------------------------
@ Tier 1 (Sloop): Haven Isle, Ironhold
@ -------------------------------------------------------------------
Pelagios_EventScript_BoatMenuSloop::
	msgbox Pelagios_Text_BoatPrompt, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BOAT_SLOOP, FALSE
	switch VAR_RESULT
	case 0, Pelagios_EventScript_SailToHaven
	case 1, Pelagios_EventScript_SailToIronhold
	goto Pelagios_EventScript_BoatCancel

@ -------------------------------------------------------------------
@ Tier 2 (Brigantine): + Sirocco Isle, Emberveil
@ -------------------------------------------------------------------
Pelagios_EventScript_BoatMenuBrigantine::
	msgbox Pelagios_Text_BoatPrompt, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BOAT_BRIGANTINE, FALSE
	switch VAR_RESULT
	case 0, Pelagios_EventScript_SailToHaven
	case 1, Pelagios_EventScript_SailToIronhold
	case 2, Pelagios_EventScript_SailToSirocco
	case 3, Pelagios_EventScript_SailToEmberveil
	goto Pelagios_EventScript_BoatCancel

@ -------------------------------------------------------------------
@ Tier 3 (Galleon): + Schism (N/S), Thalvern, Gildhaven, Primalis, Ashenveil,
@ Aetheron, Convergence. Aetheron is ALWAYS listed but its handler refuses to
@ sail until FLAG_SEA_CHART_FOUND is set (Sea Chart gates it, not boat tier).
@ Convergence is ALWAYS listed but its handler refuses until FLAG_AETHERON_RESOLVED
@ is set (the final island opens only after Aetheron resolves).
@ Schism appears as TWO entries (its two ports). Both currently route to the
@ no-chart stub because the Schism maps don't exist yet — the map-builder must
@ swap cases 4/5 to Pelagios_EventScript_SailToSchismNorth/South once
@ MAP_SCHISM_FROSTMARK_PORT / MAP_SCHISM_VENOMQUAY_PORT exist (see the stub
@ handlers below).
@ -------------------------------------------------------------------
Pelagios_EventScript_BoatMenuGalleon::
	msgbox Pelagios_Text_BoatPrompt, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BOAT_GALLEON, FALSE
	switch VAR_RESULT
	case 0, Pelagios_EventScript_SailToHaven
	case 1, Pelagios_EventScript_SailToIronhold
	case 2, Pelagios_EventScript_SailToSirocco
	case 3, Pelagios_EventScript_SailToEmberveil
	case 4, Pelagios_EventScript_SailToSchismNorth
	case 5, Pelagios_EventScript_SailToSchismSouth
	case 6, Pelagios_EventScript_SailToThalvern
	case 7, Pelagios_EventScript_SailToGildhaven
	case 8, Pelagios_EventScript_SailToPrimalis
	case 9, Pelagios_EventScript_SailToAshenveil
	case 10, Pelagios_EventScript_SailToAetheron
	case 11, Pelagios_EventScript_SailToConvergence
	goto Pelagios_EventScript_BoatCancel

@ -------------------------------------------------------------------
@ Destination handlers. Each refuses to sail to the island the player
@ is already moored at (VAR_TEMP_1), otherwise plays the cast-off line
@ and warps to that port's arrival tile.
@ -------------------------------------------------------------------
Pelagios_EventScript_SailToHaven::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_HAVEN, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_HAVEN_ISLE_HARBOR, 13, 9
	waitstate
	releaseall
	end

Pelagios_EventScript_SailToIronhold::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_IRONHOLD, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_IRONHOLD_GATEMARK_PORT, 10, 14
	waitstate
	releaseall
	end

Pelagios_EventScript_SailToSirocco::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_SIROCCO, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_SIROCCO_DUSTMOUTH_PORT, 10, 14
	waitstate
	releaseall
	end

Pelagios_EventScript_SailToEmberveil::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_EMBERVEIL, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_EMBERVEIL_ASHMOUTH_PORT, 10, 14
	waitstate
	releaseall
	end

@ Schism Isle — DUAL PORTS. Stubbed to SailNoChart until the Schism maps exist.
@ MAP-BUILDER SWAP-IN: once MAP_SCHISM_FROSTMARK_PORT / MAP_SCHISM_VENOMQUAY_PORT
@ exist with verified-walkable arrival tiles, replace each "goto SailNoChart" with
@ the same-island guard + warp pattern used by the other handlers, e.g.:
@   Pelagios_EventScript_SailToSchismNorth::
@       goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM, Pelagios_EventScript_AlreadyHere
@       msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
@       closemessage
@       warp MAP_SCHISM_FROSTMARK_PORT, <x>, <y>
@       waitstate
@       releaseall
@       end
@ (and the SOUTH equivalent guarding PELAGIOS_ISLAND_SCHISM_SOUTH ->
@  MAP_SCHISM_VENOMQUAY_PORT). Also wire each port's *_EventScript_Tennyson to set
@ VAR_TEMP_1 to PELAGIOS_ISLAND_SCHISM (Frostmark) or PELAGIOS_ISLAND_SCHISM_SOUTH
@ (Venomquay) before goto Pelagios_EventScript_BoardTennyson.
Pelagios_EventScript_SailToSchismNorth::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_SCHISM_FROSTMARK_PORT, 10, 14
	waitstate
	releaseall
	end

Pelagios_EventScript_SailToSchismSouth::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM_SOUTH, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_SCHISM_VENOMQUAY_PORT, 10, 14
	waitstate
	releaseall
	end

@ Thalvern Isle (the Sunken Kingdom) — single Galleon-tier port. Arrival tile
@ (10,14) is the central stone dock, verified walkable against the compiled
@ layout collision bits.
Pelagios_EventScript_SailToThalvern::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_THALVERN, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_THALVERN_TIDESPIRE_PORT, 10, 14
	waitstate
	releaseall
	end

@ Gildhaven Isle (the Merchant Island) — single Galleon-tier port. Arrival tile
@ (10,14) is the central stone dock, verified walkable against the compiled
@ layout collision bits (same dock geometry as the other ports).
Pelagios_EventScript_SailToGildhaven::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_GILDHAVEN, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_GILDHAVEN_GOLDPORT_HARBOR, 10, 14
	waitstate
	releaseall
	end

@ Primalis Isle (the Beast Island) - single Galleon-tier port. Arrival tile
@ (10,14) is the central dock, verified walkable against the compiled layout.
Pelagios_EventScript_SailToPrimalis::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_PRIMALIS, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_PRIMALIS_VERDANT_LANDING, 10, 14
	waitstate
	releaseall
	end

@ Ashenveil Isle (the Dead Island) — Galleon-tier destination. Arrival tile
@ (10,14) is the central dock, verified walkable against the compiled layout.
Pelagios_EventScript_SailToAshenveil::
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_ASHENVEIL, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_ASHENVEIL_GREYPORT, 10, 14
	waitstate
	releaseall
	end

@ Aetheron Isle (the Sky Island) — NEW ACTIVATION PATH. Unlike every other
@ island, Aetheron is NOT gated by boat tier. It always APPEARS in the Galleon
@ menu, but this handler refuses to sail until FLAG_SEA_CHART_FOUND is set
@ (the player must find ITEM_SEA_CHART in Ashenveil's MorthasGrove first).
@ Until then it shows the "no chart for that heading" message. Once the chart is
@ found, the Sea Chart routes the Tennyson into the KNOCK UP STREAM: the boat warps
@ to MAP_AETHERON_KNOCK_UP_STREAM at arrival tile (8,11), where the scripted ascent
@ cutscene plays and then one-way-warps the player forward to CloudLanding. The
@ same-island guard prevents re-sailing while already on Aetheron's CloudLanding.
Pelagios_EventScript_SailToAetheron::
	goto_if_unset FLAG_SEA_CHART_FOUND, Pelagios_EventScript_NoSeaChart
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_AETHERON, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_AETHERON_KNOCK_UP_STREAM, 8, 11
	waitstate
	releaseall
	end

@ Convergence Isle (the Final Island) — NOT gated by boat tier. It always
@ APPEARS in the Galleon menu, but this handler refuses to sail until
@ FLAG_AETHERON_RESOLVED is set (Cass defects and the way opens only after
@ Aetheron's seal is reinforced). After that it currently falls through to
@ SailNoChart because Convergence's maps don't exist yet — MAP-BUILDER SWAP-IN:
@ replace the post-resolve branch with the same-island guard + warp to
@ MAP_CONVERGENCE_<port> once that map exists.
Pelagios_EventScript_SailToConvergence::
	goto_if_unset FLAG_AETHERON_RESOLVED, Pelagios_EventScript_ConvergenceClosed
	goto_if_eq VAR_TEMP_1, PELAGIOS_ISLAND_CONVERGENCE, Pelagios_EventScript_AlreadyHere
	msgbox Pelagios_Text_CastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_CONVERGENCE_APPROACH, 11, 13
	waitstate
	releaseall
	end

Pelagios_EventScript_ConvergenceClosed::
	msgbox Pelagios_Text_ConvergenceClosed, MSGBOX_DEFAULT
	releaseall
	end

Pelagios_EventScript_NoSeaChart::
	msgbox Pelagios_Text_NoSeaChart, MSGBOX_DEFAULT
	releaseall
	end

@ Galleon-tier stub islands — selectable so the menu grows naturally, but
@ no port exists yet.
Pelagios_EventScript_SailNoChart::
	msgbox Pelagios_Text_NoChart, MSGBOX_DEFAULT
	releaseall
	end

Pelagios_EventScript_AlreadyHere::
	msgbox Pelagios_Text_AlreadyHere, MSGBOX_DEFAULT
	releaseall
	end

Pelagios_EventScript_BoatCancel::
	msgbox Pelagios_Text_BoatStay, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ STORM COMPASS field effect (Aetheron key item, replaces Fly).
@ Used from the BAG on ANY map - opens an island-selection menu and
@ warps to the chosen harbor's arrival tile, just like the Tennyson
@ menu but usable anywhere (not only at a harbor). It REUSES the
@ Galleon multichoice + every Pelagios_EventScript_SailTo* handler, so
@ the destination list and warp tiles stay in one place.
@
@ WORKS-ANYWHERE NOTE: the sail handlers' same-island guard compares
@ VAR_TEMP_1 to each island id. Used from the field there is no "current
@ harbor", so VAR_TEMP_1 is set to 0xFF (a value no PELAGIOS_ISLAND_*
@ uses) - the guard never matches and every island is selectable.
@ The handlers' msgbox + warp + waitstate + releaseall need a prior
@ lockall, which this entry provides.
@
@ C HANDOFF (systems-engineer): "Use" from the bag must call this
@ script. ITEM_STORM_COMPASS (880) currently has
@ .fieldUseFunc = ItemUseOutOfBattle_CannotUse in src/data/items.h.
@ Mirror the ItemUseOutOfBattle_PokemonBoxLink pattern in src/item_use.c:
@   void ItemUseOutOfBattle_StormCompass(u8 taskId) {
@       sItemUseOnFieldCB = Task_UseStormCompass;
@       SetUpItemUseOnFieldCallback(taskId);
@   }
@   static void Task_UseStormCompass(u8 taskId) {
@       ScriptContext_SetupScript(Pelagios_EventScript_StormCompass);
@       DestroyTask(taskId);
@   }
@ then set ITEM_STORM_COMPASS's .fieldUseFunc to ItemUseOutOfBattle_StormCompass.
@ (Declare Pelagios_EventScript_StormCompass extern in the .c, as other
@ script labels are.) The script below already compiles and is correct;
@ only that field-use registration remains, and it is C-side.
@ ===================================================================
Pelagios_EventScript_StormCompass::
	lockall
	setvar VAR_TEMP_1, 255
	msgbox Pelagios_Text_StormCompassPrompt, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BOAT_GALLEON, FALSE
	switch VAR_RESULT
	case 0, Pelagios_EventScript_SailToHaven
	case 1, Pelagios_EventScript_SailToIronhold
	case 2, Pelagios_EventScript_SailToSirocco
	case 3, Pelagios_EventScript_SailToEmberveil
	case 4, Pelagios_EventScript_SailToSchismNorth
	case 5, Pelagios_EventScript_SailToSchismSouth
	case 6, Pelagios_EventScript_SailToThalvern
	case 7, Pelagios_EventScript_SailToGildhaven
	case 8, Pelagios_EventScript_SailToPrimalis
	case 9, Pelagios_EventScript_SailToAshenveil
	case 10, Pelagios_EventScript_SailToAetheron
	case 11, Pelagios_EventScript_SailToConvergence
	goto Pelagios_EventScript_StormCompassCancel

Pelagios_EventScript_StormCompassCancel::
	msgbox Pelagios_Text_StormCompassStay, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Shared text. Narration style (no namebox) — the boat is described,
@ no NPC speaks. setspeaker is left at 0 by the callers.
@ -------------------------------------------------------------------
Pelagios_Text_BoatPrompt:
	.string "The TENNYSON rides at her mooring,\n"
	.string "rigging taut and ready.\p"
	.string "Where to?$"

Pelagios_Text_CastOff:
	.string "You cast off the mooring lines.\p"
	.string "The TENNYSON leans into the wind,\n"
	.string "and the harbor falls away behind\l"
	.string "you.$"

Pelagios_Text_NoChart:
	.string "You trace the route on the\n"
	.string "WARDEN's charts…\p"
	.string "The charts don't show a path\n"
	.string "there yet.$"

Pelagios_Text_NoSeaChart:
	.string "AETHERON lies somewhere out past\n"
	.string "the open sea…\p"
	.string "But you have no chart for that\n"
	.string "heading. You'd never find it.$"

Pelagios_Text_ConvergenceClosed:
	.string "The WARDEN's charts mark a place\n"
	.string "at the center of it all…\p"
	.string "But the way there is not yet open.\n"
	.string "Something still holds it shut.$"

Pelagios_Text_AlreadyHere:
	.string "You're already moored here.$"

Pelagios_Text_BoatStay:
	.string "You rest a hand on the gunwale.\p"
	.string "No rush. She'll keep.$"

Pelagios_Text_StormCompassPrompt:
	.string "The STORM COMPASS hums in your hand,\n"
	.string "its needle turning toward every harbor\l"
	.string "you have ever made.\p"
	.string "Where to?$"

Pelagios_Text_StormCompassStay:
	.string "The needle settles. You stay where you\n"
	.string "are.$"
