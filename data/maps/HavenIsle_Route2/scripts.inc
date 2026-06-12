HavenIsle_Route2_MapScripts::
	.byte 0

HavenIsleRoute2_EventScript_OldBren::
	trainerbattle_single TRAINER_FISHERMAN_HAVEN_1, HavenIsleRoute2_Text_BrenIntro, HavenIsleRoute2_Text_BrenDefeat
	setspeaker Pelagios_Speaker_OldBren
	msgbox HavenIsleRoute2_Text_BrenPostBattle, MSGBOX_AUTOCLOSE
	end

HavenIsleRoute2_EventScript_RouteSign::
	msgbox HavenIsleRoute2_Text_RouteSign, MSGBOX_SIGN
	end

HavenIsleRoute2_Text_BrenIntro:
	.string "These waters've been strange lately.\n"
	.string "Fish acting weird.\p"
	.string "Battle me while I think.$"

HavenIsleRoute2_Text_BrenDefeat:
	.string "Strange times.\p"
	.string "Your parent used to say the same\n"
	.string "thing, years back.$"

HavenIsleRoute2_Text_BrenPostBattle:
	.string "The MAGIKARP school past the\n"
	.string "point every dawn, regular as\l"
	.string "bells. Not this week.\p"
	.string "This week they circle. Like they\n"
	.string "don't want to pass over something.$"

HavenIsleRoute2_Text_RouteSign:
	.string "COASTAL ROUTE 2\p"
	.string "West: TIDEMARK VILLAGE.\n"
	.string "East: FISHING DOCKS.$"
