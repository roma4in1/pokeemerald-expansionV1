Schism_VenomquayPort_Inn_Interior_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Traveler - tried to cross the Scar once and was turned back.
@ -------------------------------------------------------------------
SchismVenomquayPortInnInterior_EventScript_Traveler::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	goto_if_set FLAG_SCAR_RUINS_FOUND, SchismVenomquayPortInnInterior_EventScript_TravelerRuins
	msgbox SchismVenomquayPortInnInterior_Text_Traveler, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPortInnInterior_EventScript_TravelerRuins::
	msgbox SchismVenomquayPortInnInterior_Text_TravelerRuins, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPortInnInterior_Text_Traveler:
	.string "I tried to cross to the ice side\n"
	.string "once. The Scar stopped me.\p"
	.string "Not the guards. The place itself.\p"
	.string "Something in the middle doesn't\n"
	.string "want you there. You feel it in\l"
	.string "your teeth.$"

SchismVenomquayPortInnInterior_Text_TravelerRuins:
	.string "You went into the Scar? And out\n"
	.string "again?\p"
	.string "Then you've seen the old stones.\n"
	.string "The ones that look like both\l"
	.string "cities at once.\p"
	.string "I couldn't get near them. You\n"
	.string "must be made of sterner stuff\l"
	.string "than me.$"
