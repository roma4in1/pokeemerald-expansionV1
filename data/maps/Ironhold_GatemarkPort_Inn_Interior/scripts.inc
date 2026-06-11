Ironhold_GatemarkPort_Inn_Interior_MapScripts::
	.byte 0

@ Traveling trader - mentions the strange readings near the summit.
IronholdGatemarkPortInnInterior_EventScript_Traveler::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdGatemarkPortInnInterior_EventScript_TravelerResolved
	msgbox IronholdGatemarkPortInnInterior_Text_Traveler, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPortInnInterior_EventScript_TravelerResolved::
	msgbox IronholdGatemarkPortInnInterior_Text_TravelerResolved, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPortInnInterior_Text_Traveler:
	.string "I came here for trade. The\n"
	.string "Covenant's doubled the checkpoint\l"
	.string "fees since last year.\p"
	.string "And the summit - something's\n"
	.string "making the STEEL-types aggressive\l"
	.string "up there.\p"
	.string "Nobody official will say why.$"

IronholdGatemarkPortInnInterior_Text_TravelerResolved:
	.string "Funny thing. The STEEL-types on\n"
	.string "the mountain settled down this\l"
	.string "week. Just like that.\p"
	.string "The Covenant put out a notice\n"
	.string "taking credit for it.\p"
	.string "…They spelled “equipment” wrong.$"
