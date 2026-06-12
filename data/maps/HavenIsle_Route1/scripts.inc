HavenIsle_Route1_MapScripts::
	.byte 0

HavenIsleRoute1_EventScript_Jay::
	trainerbattle_single TRAINER_YOUNGSTER_HAVEN_1, HavenIsleRoute1_Text_JayIntro, HavenIsleRoute1_Text_JayDefeat
	setspeaker Pelagios_Speaker_Jay
	msgbox HavenIsleRoute1_Text_JayPostBattle, MSGBOX_AUTOCLOSE
	end

HavenIsleRoute1_EventScript_RouteSign::
	msgbox HavenIsleRoute1_Text_RouteSign, MSGBOX_SIGN
	end

HavenIsleRoute1_Text_JayIntro:
	.string "Hey! You're the WARDEN's kid, right?\p"
	.string "Let's see if you inherited the\n"
	.string "talent!$"

HavenIsleRoute1_Text_JayDefeat:
	.string "Whoa! Okay, okay.\n"
	.string "You're legit.$"

HavenIsleRoute1_Text_JayPostBattle:
	.string "My RATTATA's been jumpy all\n"
	.string "week. Won't go near the ruins.\p"
	.string "Animals know stuff. That's what\n"
	.string "my gran says, anyway.$"

HavenIsleRoute1_Text_RouteSign:
	.string "COASTAL ROUTE 1\p"
	.string "North: the old ruins.\n"
	.string "South: TIDEMARK VILLAGE.$"
