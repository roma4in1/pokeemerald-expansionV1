HavenIsle_AncientRuinsExterior_MapScripts::
	.byte 0

AncientRuinsExterior_EventScript_Disturbance::
	lockall
	delay 20
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox AncientRuinsExterior_Text_GroundShudders, MSGBOX_DEFAULT
	closemessage
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox AncientRuinsExterior_Text_SollisCall, MSGBOX_DEFAULT
	closemessage
	setvar VAR_HAVEN_RUINS_STATE, 2
	releaseall
	end

AncientRuinsExterior_EventScript_Sign::
	msgbox AncientRuinsExterior_Text_Sign, MSGBOX_SIGN
	end

AncientRuinsExterior_Text_GroundShudders:
	.string "The ground shudders!\p"
	.string "A flock of TAILLOW erupts from the\n"
	.string "treeline, shrieking.\p"
	.string "For half a heartbeat, the mouth of\n"
	.string "the ruins glows from within —\p"
	.string "— and then everything is still.\n"
	.string "Too still.$"

AncientRuinsExterior_Text_SollisCall:
	.string "…Your POKéNAV buzzes.\p"
	.string "SOLLIS: {PLAYER}! My instruments\n"
	.string "just went off the chart — it's\l"
	.string "coming from the ruins, right where\l"
	.string "you're standing!\p"
	.string "Can you take a look inside?\n"
	.string "Carefully. CAREFULLY, {PLAYER}.$"

AncientRuinsExterior_Text_Sign:
	.string "The letters carved here are older\n"
	.string "than the village.\p"
	.string "Most have worn away. What remains\n"
	.string "reads:\p"
	.string "“…SLEEPS BENEATH. LET IT.”$"
