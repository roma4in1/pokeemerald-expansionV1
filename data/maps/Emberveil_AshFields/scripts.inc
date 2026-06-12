Emberveil_AshFields_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Devotee Mira - TRAINER_CULTMEMBER_EMBERVEIL_3 (sight-initiated)
@ -------------------------------------------------------------------
EmberveilAshFields_EventScript_DevoteeMira::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_3, EmberveilAshFields_Text_MiraIntro, EmberveilAshFields_Text_MiraDefeat
	setspeaker Pelagios_Speaker_Mira
	msgbox EmberveilAshFields_Text_MiraPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Dead trees preserved in ash - this was forest once
@ -------------------------------------------------------------------
EmberveilAshFields_EventScript_DeadTree1::
	msgbox EmberveilAshFields_Text_DeadTree1, MSGBOX_SIGN
	end

EmberveilAshFields_EventScript_DeadTree2::
	msgbox EmberveilAshFields_Text_DeadTree2, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilAshFields_Text_MiraIntro:
	.string "You're heading toward the city.\p"
	.string "The HIGH EMBER will want to know\n"
	.string "about visitors.$"

EmberveilAshFields_Text_MiraDefeat:
	.string "Mm. She'll want to know about\n"
	.string "THAT, too.$"

EmberveilAshFields_Text_MiraPost:
	.string "She'll know about you anyway.\n"
	.string "She always does.$"

EmberveilAshFields_Text_DeadTree1:
	.string "A dead tree stands sealed in\n"
	.string "grey ash, every branch intact.\p"
	.string "Under the crust you can still\n"
	.string "make out bark. Leaves, even.\p"
	.string "This was a forest once - before\n"
	.string "some older eruption put it to\l"
	.string "sleep standing up.$"

EmberveilAshFields_Text_DeadTree2:
	.string "Someone has tied a faded red\n"
	.string "cord around this trunk.\p"
	.string "One cord for every tree the ash\n"
	.string "kept whole, the locals say.\p"
	.string "“The mountain takes. The mountain\n"
	.string "keeps. The mountain remembers.”$"
