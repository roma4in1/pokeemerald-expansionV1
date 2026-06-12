Sirocco_DaganPalace_Exterior_MapScripts::
	.byte 0

@ Palace Guard Thorn - TRAINER_GILTCLAW_SIROCCO_7 (sight-initiated)
SiroccoDaganPalaceExterior_EventScript_GuardThorn::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_7, SiroccoDaganPalaceExterior_Text_ThornIntro, SiroccoDaganPalaceExterior_Text_ThornDefeat
	setspeaker Pelagios_Speaker_Thorn
	msgbox SiroccoDaganPalaceExterior_Text_ThornPost, MSGBOX_AUTOCLOSE
	end

@ Palace Guard Fen - TRAINER_GILTCLAW_SIROCCO_8 (sight-initiated)
SiroccoDaganPalaceExterior_EventScript_GuardFen::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_8, SiroccoDaganPalaceExterior_Text_FenIntro, SiroccoDaganPalaceExterior_Text_FenDefeat
	setspeaker Pelagios_Speaker_Fen
	msgbox SiroccoDaganPalaceExterior_Text_FenPost, MSGBOX_AUTOCLOSE
	end

SiroccoDaganPalaceExterior_Text_ThornIntro:
	.string "The palace is private property.$"

SiroccoDaganPalaceExterior_Text_ThornDefeat:
	.string "…Hm.$"

SiroccoDaganPalaceExterior_Text_ThornPost:
	.string "…The door's open.$"

SiroccoDaganPalaceExterior_Text_FenIntro:
	.string "I've seen DAGAN handle situations\n"
	.string "like this before.\p"
	.string "He always comes out ahead.\n"
	.string "I believe in him.$"

SiroccoDaganPalaceExterior_Text_FenDefeat:
	.string "That changes nothing.$"

SiroccoDaganPalaceExterior_Text_FenPost:
	.string "He's going to be fine.\p"
	.string "He always is.$"
