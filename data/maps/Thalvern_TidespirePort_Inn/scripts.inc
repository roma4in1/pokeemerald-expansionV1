Thalvern_TidespirePort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_TidespirePort_Inn_OnTransition
	.byte 0

Thalvern_TidespirePort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_THALVERN_TIDESPIRE_PORT
	end

@ ===================================================================
@ Innkeeper - the island's heal NPC (whiteout respawn target). She
@ sees visions occasionally. YESNO bed heal.
@ ===================================================================
ThalvernTidespirePortInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernTidespirePortInn_EventScript_InnkeeperResolved
	msgbox ThalvernTidespirePortInn_Text_Greeting, MSGBOX_DEFAULT
	goto ThalvernTidespirePortInn_EventScript_RoomOffer

ThalvernTidespirePortInn_EventScript_InnkeeperResolved::
	msgbox ThalvernTidespirePortInn_Text_GreetingResolved, MSGBOX_DEFAULT
	goto ThalvernTidespirePortInn_EventScript_RoomOffer

ThalvernTidespirePortInn_EventScript_RoomOffer::
	msgbox ThalvernTidespirePortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ThalvernTidespirePortInn_EventScript_NoRest
	msgbox ThalvernTidespirePortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox ThalvernTidespirePortInn_Text_AfterRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernTidespirePortInn_EventScript_NoRest::
	msgbox ThalvernTidespirePortInn_Text_NoRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernTidespirePortInn_Text_Greeting:
	.string "Welcome to the Tidespire Inn.\p"
	.string "I saw a city last night. Not this\n"
	.string "one. Bigger. Whole.\p"
	.string "Then the water came.\p"
	.string "...Sorry. Can I get you a room?$"

ThalvernTidespirePortInn_Text_GreetingResolved:
	.string "I slept the whole night through.\n"
	.string "No city. No water.\p"
	.string "First time in months. A room?$"

ThalvernTidespirePortInn_Text_RoomOffer:
	.string "Rest here and recover your team?$"

ThalvernTidespirePortInn_Text_RoomsYours:
	.string "Take the room at the end. The bed\n"
	.string "by the window is the dry one.$"

ThalvernTidespirePortInn_Text_AfterRest:
	.string "Your team looks rested.\p"
	.string "Whatever you're going down there to\n"
	.string "do - do it carefully.$"

ThalvernTidespirePortInn_Text_NoRest:
	.string "The kettle's on if you change your\n"
	.string "mind.$"
