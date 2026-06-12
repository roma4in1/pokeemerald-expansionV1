Emberveil_VolcanoAscent_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Elder Disciple Horne - TRAINER_CULTMEMBER_EMBERVEIL_7 (sight)
@ -------------------------------------------------------------------
EmberveilVolcanoAscent_EventScript_DiscipleHorne::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_7, EmberveilVolcanoAscent_Text_HorneIntro, EmberveilVolcanoAscent_Text_HorneDefeat
	setspeaker Pelagios_Speaker_Horne
	msgbox EmberveilVolcanoAscent_Text_HornePost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Elder Disciple Cael - TRAINER_CULTMEMBER_EMBERVEIL_8 (sight)
@ -------------------------------------------------------------------
EmberveilVolcanoAscent_EventScript_DiscipleCael::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_8, EmberveilVolcanoAscent_Text_CaelIntro, EmberveilVolcanoAscent_Text_CaelDefeat
	setspeaker Pelagios_Speaker_Cael
	msgbox EmberveilVolcanoAscent_Text_CaelPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilVolcanoAscent_Text_HorneIntro:
	.string "I've served the HIGH EMBER for\n"
	.string "forty years.\p"
	.string "I won't let you interrupt the\n"
	.string "culmination of her work.$"

EmberveilVolcanoAscent_Text_HorneDefeat:
	.string "Forty years…$"

EmberveilVolcanoAscent_Text_HornePost:
	.string "…Go. If you can beat me, perhaps\n"
	.string "PYRATH has a reason for your\l"
	.string "being here.$"

EmberveilVolcanoAscent_Text_CaelIntro:
	.string "The summit is sacred.\n"
	.string "Turn back.$"

EmberveilVolcanoAscent_Text_CaelDefeat:
	.string "…I see.$"

EmberveilVolcanoAscent_Text_CaelPost:
	.string "I misjudged you. Go.$"
