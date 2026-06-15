Gildhaven_GoldportHarbor_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Gildhaven_GoldportHarbor_Inn_OnTransition
	.byte 0

Gildhaven_GoldportHarbor_Inn_OnTransition:
	setrespawn HEAL_LOCATION_GILDHAVEN_GOLDPORT_HARBOR
	end

@ ===================================================================
@ Innkeeper - professionally warm, slightly hollow. Island heal NPC.
@ ===================================================================
GildhavenGoldportHarborInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenGoldportHarborInn_EventScript_Resolved
	msgbox GildhavenGoldportHarborInn_Text_Greeting, MSGBOX_DEFAULT
	goto GildhavenGoldportHarborInn_EventScript_RoomOffer

GildhavenGoldportHarborInn_EventScript_Resolved::
	msgbox GildhavenGoldportHarborInn_Text_GreetingResolved, MSGBOX_DEFAULT
	goto GildhavenGoldportHarborInn_EventScript_RoomOffer

GildhavenGoldportHarborInn_EventScript_RoomOffer::
	msgbox GildhavenGoldportHarborInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, GildhavenGoldportHarborInn_EventScript_NoRest
	msgbox GildhavenGoldportHarborInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox GildhavenGoldportHarborInn_Text_AfterRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenGoldportHarborInn_EventScript_NoRest::
	msgbox GildhavenGoldportHarborInn_Text_NoRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenGoldportHarborInn_Text_Greeting:
	.string "Welcome to the GILT REST. The finest\n"
	.string "accommodations in Goldport.\p"
	.string "We pride ourselves on making guests\n"
	.string "feel they never need to leave.$"

GildhavenGoldportHarborInn_Text_GreetingResolved:
	.string "Welcome back.\p"
	.string "Funny - we used to say guests never\n"
	.string "needed to leave. This morning that\l"
	.string "line stuck in my throat.\p"
	.string "Rest as long as you like. Or go\n"
	.string "whenever you please. Truly.$"

GildhavenGoldportHarborInn_Text_RoomOffer:
	.string "Would you care to rest? On the house\n"
	.string "for a guest of your standing.$"

GildhavenGoldportHarborInn_Text_RoomsYours:
	.string "Of course. Right this way.$"

GildhavenGoldportHarborInn_Text_AfterRest:
	.string "Your party looks wonderful. Refreshed.\p"
	.string "Do stay. Everyone stays.$"

GildhavenGoldportHarborInn_Text_NoRest:
	.string "As you wish. We will be here.\p"
	.string "We are always here.$"
