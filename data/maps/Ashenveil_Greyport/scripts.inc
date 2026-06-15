Ashenveil_Greyport_MapScripts::
	.byte 0

@ Arrival. The boat lands on a dead port at permanent dusk. Silence first -
@ no music sting, no fanfare. Then Orin walks down from the outpost.
AshenveilGreyport_EventScript_Arrival::
	lockall
	delay 30
	msgbox AshenveilGreyport_Text_ArrivalSilence, MSGBOX_DEFAULT
	closemessage
	delay 20
	applymovement LOCALID_ASHENVEIL_ORIN, AshenveilGreyport_Movement_OrinApproach
	waitmovement 0
	delay 10
	setspeaker Pelagios_Speaker_Orin
	msgbox AshenveilGreyport_Text_OrinGreeting, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	setflag FLAG_ASHENVEIL_ARRIVED
	setvar VAR_ASHENVEIL_PROGRESS, 1
	releaseall
	end

@ Orin, talked to in the port. Variants gated on island progress.
AshenveilGreyport_EventScript_Orin::
	lockall
	faceplayer
	setspeaker Pelagios_Speaker_Orin
	goto_if_set FLAG_SEA_CHART_FOUND, AshenveilGreyport_EventScript_OrinAfterChart
	goto_if_set FLAG_ASHENVEIL_DORNE_MET, AshenveilGreyport_EventScript_OrinAfterDorne
	goto_if_set FLAG_PHANTOM_LANTERN_OBTAINED, AshenveilGreyport_EventScript_OrinAfterLantern
	msgbox AshenveilGreyport_Text_OrinBeforeLantern, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

AshenveilGreyport_EventScript_OrinAfterLantern::
	msgbox AshenveilGreyport_Text_OrinWhyStay, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

AshenveilGreyport_EventScript_OrinAfterDorne::
	msgbox AshenveilGreyport_Text_OrinAfterDorne, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

AshenveilGreyport_EventScript_OrinAfterChart::
	msgbox AshenveilGreyport_Text_OrinAfterChart, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

@ Boat-boarding wiring (map-builder owns this; NOT dialogue).
AshenveilGreyport_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_ASHENVEIL
	goto Pelagios_EventScript_BoardTennyson
	end

AshenveilGreyport_Movement_OrinApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	face_down
	step_end

AshenveilGreyport_Text_ArrivalSilence::
	.string "The boat touches the dock and the\n"
	.string "engine goes quiet.\p"
	.string "There is no sound here. No gulls.\n"
	.string "No wind in the rigging. Nothing.\p"
	.string "Ash falls like grey snow, slow and\n"
	.string "even, over a port no one has used in\l"
	.string "twenty years.$"

AshenveilGreyport_Text_OrinGreeting::
	.string "Someone is coming down from the only\n"
	.string "building still standing.$"

AshenveilGreyport_Text_OrinBeforeLantern::
	.string "You shouldn't be here. Nobody comes\n"
	.string "here.\p"
	.string "But you're here. And you have that\n"
	.string "look. Like you already know why they\l"
	.string "said it was uninhabitable.\p"
	.string "Come inside. I'll give you the\n"
	.string "lantern.$"

AshenveilGreyport_Text_OrinWhyStay::
	.string "Why do I stay? Because someone\n"
	.string "should.\p"
	.string "Because 847 people lived here and the\n"
	.string "official record says they all\l"
	.string "evacuated safely and they didn't and\l"
	.string "someone should stay and know that.\p"
	.string "Even if knowing doesn't change\n"
	.string "anything.$"

AshenveilGreyport_Text_OrinAfterDorne::
	.string "You went to the memorial. I can see\n"
	.string "it on you.\p"
	.string "He comes every few months. Tends the\n"
	.string "garden. Doesn't say much.\p"
	.string "Whatever he told you up there - I\n"
	.string "expect it was the truth. He's past\l"
	.string "the point of lying to anyone.$"

AshenveilGreyport_Text_OrinAfterChart::
	.string "You found what the young one left in\n"
	.string "the grove. Good.\p"
	.string "I don't want to know where it leads.\n"
	.string "I just wanted someone to carry it off\l"
	.string "this island.\p"
	.string "Go on, then. Whatever's out there is\n"
	.string "more for the living than this place\l"
	.string "is.$"
