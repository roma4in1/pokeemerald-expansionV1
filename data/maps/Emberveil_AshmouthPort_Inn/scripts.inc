Emberveil_AshmouthPort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Emberveil_AshmouthPort_Inn_OnTransition
	.byte 0

Emberveil_AshmouthPort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_EMBERVEIL_ASHMOUTH_PORT
	end

@ -------------------------------------------------------------------
@ Innkeeper - cult member, genuinely hospitable. The island's heal
@ NPC (whiteout respawn target). YESNO bed heal.
@ -------------------------------------------------------------------
EmberveilAshmouthPortInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPortInn_EventScript_InnkeeperResolved
	msgbox EmberveilAshmouthPortInn_Text_Greeting, MSGBOX_DEFAULT
	goto EmberveilAshmouthPortInn_EventScript_RoomOffer

EmberveilAshmouthPortInn_EventScript_InnkeeperResolved::
	msgbox EmberveilAshmouthPortInn_Text_GreetingResolved, MSGBOX_DEFAULT
	goto EmberveilAshmouthPortInn_EventScript_RoomOffer

EmberveilAshmouthPortInn_EventScript_RoomOffer::
	msgbox EmberveilAshmouthPortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EmberveilAshmouthPortInn_EventScript_NoRest
	msgbox EmberveilAshmouthPortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox EmberveilAshmouthPortInn_Text_AfterRest, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPortInn_EventScript_NoRest::
	msgbox EmberveilAshmouthPortInn_Text_NoRest, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilAshmouthPortInn_Text_Greeting:
	.string "Welcome in. Mind the mat - the\n"
	.string "ash follows everyone inside.\p"
	.string "PYRATH's warmth keeps the rooms\n"
	.string "cozy. Free of charge. The\l"
	.string "mountain provides.$"

EmberveilAshmouthPortInn_Text_GreetingResolved:
	.string "Welcome back in.\p"
	.string "The rooms are a touch cooler\n"
	.string "lately. First time in years.\p"
	.string "I find I don't mind. A blanket's\n"
	.string "a fair trade for sleeping easy.$"

EmberveilAshmouthPortInn_Text_RoomOffer:
	.string "Need a room? Your POKéMON are\n"
	.string "welcome to rest too.$"

EmberveilAshmouthPortInn_Text_RoomsYours:
	.string "The corner room's yours. Warmest\n"
	.string "bed this side of the caldera.\p"
	.string "Sleep well.$"

EmberveilAshmouthPortInn_Text_AfterRest:
	.string "There. You look less grey\n"
	.string "already.\p"
	.string "If you climb the mountain road,\n"
	.string "take water. Everyone forgets\l"
	.string "water.$"

EmberveilAshmouthPortInn_Text_NoRest:
	.string "Suit yourself.\p"
	.string "The kettle's always on if you\n"
	.string "change your mind.$"
