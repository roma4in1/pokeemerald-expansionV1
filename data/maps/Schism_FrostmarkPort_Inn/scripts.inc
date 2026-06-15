Schism_FrostmarkPort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Schism_FrostmarkPort_Inn_OnTransition
	.byte 0

Schism_FrostmarkPort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_SCHISM_FROSTMARK_PORT
	end

@ -------------------------------------------------------------------
@ Innkeeper - ice faction member, pragmatic. North whiteout respawn.
@ YESNO bed heal (reuses the Common out-of-center heal).
@ -------------------------------------------------------------------
SchismFrostmarkPortInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrostmarkPortInn_EventScript_InnkeeperCeasefire
	msgbox SchismFrostmarkPortInn_Text_Greeting, MSGBOX_DEFAULT
	goto SchismFrostmarkPortInn_EventScript_RoomOffer

SchismFrostmarkPortInn_EventScript_InnkeeperCeasefire::
	msgbox SchismFrostmarkPortInn_Text_GreetingCeasefire, MSGBOX_DEFAULT
	goto SchismFrostmarkPortInn_EventScript_RoomOffer

SchismFrostmarkPortInn_EventScript_RoomOffer::
	msgbox SchismFrostmarkPortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismFrostmarkPortInn_EventScript_NoRest
	msgbox SchismFrostmarkPortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox SchismFrostmarkPortInn_Text_AfterRest, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPortInn_EventScript_NoRest::
	msgbox SchismFrostmarkPortInn_Text_NoRest, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismFrostmarkPortInn_Text_Greeting:
	.string "Out of the snow, then. Good.\p"
	.string "I keep the rooms warm and the\n"
	.string "questions to a minimum. Most\l"
	.string "guests prefer it that way.$"

SchismFrostmarkPortInn_Text_GreetingCeasefire:
	.string "Quieter in here lately. The\n"
	.string "soldiers used to drink and argue\l"
	.string "about the south.\p"
	.string "Now they just drink. It's an\n"
	.string "improvement, mostly.$"

SchismFrostmarkPortInn_Text_RoomOffer:
	.string "Room for the night? Your POKéMON\n"
	.string "rest too. Faction covers it.$"

SchismFrostmarkPortInn_Text_RoomsYours:
	.string "End of the hall. Extra blankets\n"
	.string "in the chest.\p"
	.string "Sleep well.$"

SchismFrostmarkPortInn_Text_AfterRest:
	.string "There. Warm through again.\p"
	.string "If you're headed north, dress\n"
	.string "heavier than you think you need\l"
	.string "to.$"

SchismFrostmarkPortInn_Text_NoRest:
	.string "Suit yourself. The fire's here\n"
	.string "if you change your mind.$"
