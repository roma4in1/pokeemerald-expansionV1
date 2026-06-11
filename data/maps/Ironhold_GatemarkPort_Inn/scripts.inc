Ironhold_GatemarkPort_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Ironhold_GatemarkPort_Inn_OnTransition
	.byte 0

Ironhold_GatemarkPort_Inn_OnTransition:
	setrespawn HEAL_LOCATION_IRONHOLD_GATEMARK_PORT
	end

@ Innkeeper - nervous, clearly watched. Offers a bed (full heal).
IronholdGatemarkPortInn_EventScript_Innkeeper::
	lock
	faceplayer
	msgbox IronholdGatemarkPortInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, IronholdGatemarkPortInn_EventScript_NoRest
	msgbox IronholdGatemarkPortInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox IronholdGatemarkPortInn_Text_AfterRest, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPortInn_EventScript_NoRest::
	msgbox IronholdGatemarkPortInn_Text_NoRest, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPortInn_Text_RoomOffer:
	.string "Evening. Or morning. The fog makes\n"
	.string "it hard to tell, here.\p"
	.string "She glances at the window before\n"
	.string "she finishes the thought.\p"
	.string "Need a room? Your POKéMON can\n"
	.string "rest too.$"

IronholdGatemarkPortInn_Text_RoomsYours:
	.string "Room's yours.\p"
	.string "I'd say enjoy your stay but -\n"
	.string "well. It's IRONHOLD.\p"
	.string "Get some rest.$"

IronholdGatemarkPortInn_Text_AfterRest:
	.string "Your POKéMON look better for it.\p"
	.string "…If anyone official asks, you\n"
	.string "were never here. Habit. Sorry.$"

IronholdGatemarkPortInn_Text_NoRest:
	.string "Suit yourself.\p"
	.string "The lamp in the hall stays lit\n"
	.string "all night. House rule.$"
