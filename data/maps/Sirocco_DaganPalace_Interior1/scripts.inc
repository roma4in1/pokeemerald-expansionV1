Sirocco_DaganPalace_Interior1_MapScripts::
	.byte 0

@ Inner Guard Rusk - TRAINER_GILTCLAW_SIROCCO_9 (sight-initiated)
SiroccoDaganPalaceInterior1_EventScript_GuardRusk::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_9, SiroccoDaganPalaceInterior1_Text_RuskIntro, SiroccoDaganPalaceInterior1_Text_RuskDefeat
	setspeaker Pelagios_Speaker_Rusk
	msgbox SiroccoDaganPalaceInterior1_Text_RuskPost, MSGBOX_AUTOCLOSE
	end

@ Inner Guard Lace - TRAINER_GILTCLAW_SIROCCO_10 (sight-initiated)
SiroccoDaganPalaceInterior1_EventScript_GuardLace::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_10, SiroccoDaganPalaceInterior1_Text_LaceIntro, SiroccoDaganPalaceInterior1_Text_LaceDefeat
	setspeaker Pelagios_Speaker_Lace
	msgbox SiroccoDaganPalaceInterior1_Text_LacePost, MSGBOX_AUTOCLOSE
	end

SiroccoDaganPalaceInterior1_Text_RuskIntro:
	.string "You really walked in here.\n"
	.string "Incredible.$"

SiroccoDaganPalaceInterior1_Text_RuskDefeat:
	.string "Incredible.$"

SiroccoDaganPalaceInterior1_Text_RuskPost:
	.string "And now you're walking further\n"
	.string "in. More incredible.$"

SiroccoDaganPalaceInterior1_Text_LaceIntro:
	.string "DAGAN says you're interesting.\p"
	.string "I'll see about that.$"

SiroccoDaganPalaceInterior1_Text_LaceDefeat:
	.string "…Saw it.$"

SiroccoDaganPalaceInterior1_Text_LacePost:
	.string "…Okay. You're interesting.\p"
	.string "Stairs are at the back. He'll\n"
	.string "have heard the whole thing.$"
