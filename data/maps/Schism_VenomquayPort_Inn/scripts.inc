Schism_VenomquayPort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Schism_VenomquayPort_Inn_OnTransition
	.byte 0

Schism_VenomquayPort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_SCHISM_VENOMQUAY_PORT
	end

@ -------------------------------------------------------------------
@ Innkeeper - doubles as an information broker. South whiteout
@ respawn. YESNO bed heal.
@ -------------------------------------------------------------------
SchismVenomquayPortInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismVenomquayPortInn_EventScript_InnkeeperCeasefire
	msgbox SchismVenomquayPortInn_Text_Greeting, MSGBOX_DEFAULT
	goto SchismVenomquayPortInn_EventScript_RoomOffer

SchismVenomquayPortInn_EventScript_InnkeeperCeasefire::
	msgbox SchismVenomquayPortInn_Text_GreetingCeasefire, MSGBOX_DEFAULT
	goto SchismVenomquayPortInn_EventScript_RoomOffer

SchismVenomquayPortInn_EventScript_RoomOffer::
	msgbox SchismVenomquayPortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismVenomquayPortInn_EventScript_NoRest
	msgbox SchismVenomquayPortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox SchismVenomquayPortInn_Text_AfterRest, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPortInn_EventScript_NoRest::
	msgbox SchismVenomquayPortInn_Text_NoRest, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismVenomquayPortInn_Text_Greeting:
	.string "Beds upstairs, rumors down here.\n"
	.string "Both for a price.\p"
	.string "What you hear in this room you\n"
	.string "keep in this room. That's the\l"
	.string "only rule that matters.$"

SchismVenomquayPortInn_Text_GreetingCeasefire:
	.string "Strange days. My best gossip just\n"
	.string "evaporated overnight.\p"
	.string "“The commanders met at the Scar.”\n"
	.string "That's the whole story. No blood,\l"
	.string "no betrayal. Terrible for\l"
	.string "business.$"

SchismVenomquayPortInn_Text_RoomOffer:
	.string "Room for the night? Your POKéMON\n"
	.string "rest too. We don't charge\l"
	.string "outsiders extra. Much.$"

SchismVenomquayPortInn_Text_RoomsYours:
	.string "Top of the stairs. Mind the leak\n"
	.string "in the hall - everything leaks\l"
	.string "here.\p"
	.string "Sleep well.$"

SchismVenomquayPortInn_Text_AfterRest:
	.string "There. Good as the south side\n"
	.string "gets.\p"
	.string "Heading up to the swamp? Don't\n"
	.string "drink anything you didn't bring\l"
	.string "yourself.$"

SchismVenomquayPortInn_Text_NoRest:
	.string "Suit yourself. Rumors are still\n"
	.string "for sale if you get bored.$"
