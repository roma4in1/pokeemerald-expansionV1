Sirocco_DustmouthPort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Sirocco_DustmouthPort_Inn_OnTransition
	.byte 0

Sirocco_DustmouthPort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_SIROCCO_DUSTMOUTH_PORT
	end

@ -------------------------------------------------------------------
@ Innkeeper - exhausted, clearly paying Gilt Claw protection.
@ Offers a bed (full heal). Doubles as the respawn healer NPC.
@ -------------------------------------------------------------------
SiroccoDustmouthPortInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPortInn_EventScript_InnkeeperResolved
	msgbox SiroccoDustmouthPortInn_Text_Greeting, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDustmouthPortInn_Text_CountsCoins, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Innkeeper
	goto SiroccoDustmouthPortInn_EventScript_RoomOffer

SiroccoDustmouthPortInn_EventScript_InnkeeperResolved::
	msgbox SiroccoDustmouthPortInn_Text_GreetingResolved, MSGBOX_DEFAULT
SiroccoDustmouthPortInn_EventScript_RoomOffer::
	msgbox SiroccoDustmouthPortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SiroccoDustmouthPortInn_EventScript_NoRest
	msgbox SiroccoDustmouthPortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox SiroccoDustmouthPortInn_Text_AfterRest, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPortInn_EventScript_NoRest::
	msgbox SiroccoDustmouthPortInn_Text_NoRest, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Regular by the wall - knows about the basement, says too much,
@ then pretends he didn't. (The actual stairs are past the rooms.)
@ -------------------------------------------------------------------
SiroccoDustmouthPortInn_EventScript_BlackMarketDoor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Citizen
	msgbox SiroccoDustmouthPortInn_Text_BasementHint, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDustmouthPortInn_Text_BasementHintBeat, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Citizen
	msgbox SiroccoDustmouthPortInn_Text_BasementHintCover, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoDustmouthPortInn_Text_Greeting:
	.string "Welcome to the DRY ANCHOR.\n"
	.string "Rooms upstairs. Water's extra.\p"
	.string "Everything's extra.$"

SiroccoDustmouthPortInn_Text_CountsCoins:
	.string "She counts a little stack of coins\n"
	.string "without looking at it, the way\l"
	.string "people count things they're about\l"
	.string "to lose.$"

SiroccoDustmouthPortInn_Text_GreetingResolved:
	.string "Welcome to the DRY ANCHOR.\p"
	.string "Nobody came for the protection\n"
	.string "money this week.\p"
	.string "I bought real coffee with it.\n"
	.string "Don't tell anyone.$"

SiroccoDustmouthPortInn_Text_RoomOffer:
	.string "Need a room? Your POKéMON can\n"
	.string "rest too.$"

SiroccoDustmouthPortInn_Text_RoomsYours:
	.string "Second door on the left.\p"
	.string "If anyone knocks after dark,\n"
	.string "don't answer. That's not a\l"
	.string "DUSTMOUTH superstition.\l"
	.string "That's just good advice.$"

SiroccoDustmouthPortInn_Text_AfterRest:
	.string "Your POKéMON look better.\n"
	.string "You look better.\p"
	.string "Nobody leaves this island looking\n"
	.string "better. Enjoy it.$"

SiroccoDustmouthPortInn_Text_NoRest:
	.string "Suit yourself.\p"
	.string "The heat does strange things to\n"
	.string "people who don't sleep.$"

SiroccoDustmouthPortInn_Text_BasementHint:
	.string "You didn't hear this from me, but\n"
	.string "if the prices upstairs sting,\l"
	.string "there's a cellar under this inn\l"
	.string "where the GILT CLAW tariff has\l"
	.string "never once been paid.\p"
	.string "Past the rooms. Down the far\n"
	.string "stairs.$"

SiroccoDustmouthPortInn_Text_BasementHintBeat:
	.string "He freezes, as if hearing his own\n"
	.string "voice a moment too late.$"

SiroccoDustmouthPortInn_Text_BasementHintCover:
	.string "…I said NOTHING. I'm a man who\n"
	.string "drinks quietly and minds his\l"
	.string "own business.$"
