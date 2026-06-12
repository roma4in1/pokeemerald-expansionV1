Emberveil_LavaRoute1_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Initiate Sera - TRAINER_CULTMEMBER_EMBERVEIL_1 (sight-initiated;
@ no namebox on the intro - engine constraint)
@ -------------------------------------------------------------------
EmberveilLavaRoute1_EventScript_InitiateSera::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_1, EmberveilLavaRoute1_Text_SeraIntro, EmberveilLavaRoute1_Text_SeraDefeat
	setspeaker Pelagios_Speaker_Sera
	msgbox EmberveilLavaRoute1_Text_SeraPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Initiate Bren - TRAINER_CULTMEMBER_EMBERVEIL_2 (sight-initiated)
@ -------------------------------------------------------------------
EmberveilLavaRoute1_EventScript_InitiateBren::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_2, EmberveilLavaRoute1_Text_BrenIntro, EmberveilLavaRoute1_Text_BrenDefeat
	setspeaker Pelagios_Speaker_Bren
	msgbox EmberveilLavaRoute1_Text_BrenPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilLavaRoute1_Text_SeraIntro:
	.string "PYRATH's warmth protects the\n"
	.string "faithful!\p"
	.string "Let me show you what devotion\n"
	.string "looks like.$"

EmberveilLavaRoute1_Text_SeraDefeat:
	.string "Oh. That's what it looks like.$"

EmberveilLavaRoute1_Text_SeraPost:
	.string "…Devotion also means accepting\n"
	.string "defeat with grace.\p"
	.string "PYRATH be with you.$"

EmberveilLavaRoute1_Text_BrenIntro:
	.string "The HIGH EMBER says the awakening\n"
	.string "is near.\p"
	.string "I want to be ready when it comes.$"

EmberveilLavaRoute1_Text_BrenDefeat:
	.string "Not ready. Noted.$"

EmberveilLavaRoute1_Text_BrenPost:
	.string "I'll be readier next time.$"
