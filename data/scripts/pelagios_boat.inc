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
.set PELAGIOS_ISLAND_SCHISM,    4
.set PELAGIOS_ISLAND_THALVERN,  5
.set PELAGIOS_ISLAND_GILDHAVEN, 6

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
@ Tier 3 (Galleon): + Schism Isle, Thalvern, Gildhaven (stubs)
@ -------------------------------------------------------------------
Pelagios_EventScript_BoatMenuGalleon::
	msgbox Pelagios_Text_BoatPrompt, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_BOAT_GALLEON, FALSE
	switch VAR_RESULT
	case 0, Pelagios_EventScript_SailToHaven
	case 1, Pelagios_EventScript_SailToIronhold
	case 2, Pelagios_EventScript_SailToSirocco
	case 3, Pelagios_EventScript_SailToEmberveil
	case 4, Pelagios_EventScript_SailNoChart
	case 5, Pelagios_EventScript_SailNoChart
	case 6, Pelagios_EventScript_SailNoChart
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

Pelagios_Text_AlreadyHere:
	.string "You're already moored here.$"

Pelagios_Text_BoatStay:
	.string "You rest a hand on the gunwale.\p"
	.string "No rush. She'll keep.$"
