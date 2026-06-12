Sirocco_DesertRoute1_MapScripts::
	.byte 0

@ Enforcer Rael - TRAINER_GILTCLAW_SIROCCO_1 (sight-initiated)
SiroccoDesertRoute1_EventScript_EnforcerRael::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_1, SiroccoDesertRoute1_Text_RaelIntro, SiroccoDesertRoute1_Text_RaelDefeat
	setspeaker Pelagios_Speaker_Rael
	msgbox SiroccoDesertRoute1_Text_RaelPost, MSGBOX_AUTOCLOSE
	end

@ Traveler Yoss - TRAINER_TRAVELER_SIROCCO_1 (sight-initiated)
SiroccoDesertRoute1_EventScript_TravelerYoss::
	trainerbattle_single TRAINER_TRAVELER_SIROCCO_1, SiroccoDesertRoute1_Text_YossIntro, SiroccoDesertRoute1_Text_YossDefeat
	setspeaker Pelagios_Speaker_Yoss
	msgbox SiroccoDesertRoute1_Text_YossPost, MSGBOX_AUTOCLOSE
	end

@ Dried bush - the hidden Rawst Berry sits one tile north
SiroccoDesertRoute1_EventScript_DriedBush::
	msgbox SiroccoDesertRoute1_Text_DriedBush, MSGBOX_SIGN
	end

SiroccoDesertRoute1_Text_RaelIntro:
	.string "Gilt Claw business. Move along\n"
	.string "or move through me.$"

SiroccoDesertRoute1_Text_RaelDefeat:
	.string "…Fine.$"

SiroccoDesertRoute1_Text_RaelPost:
	.string "…Fine. The boss will hear about\n"
	.string "this.\p"
	.string "He hears about everything.$"

SiroccoDesertRoute1_Text_YossIntro:
	.string "I'm trying to get to MIRADEN\n"
	.string "before my water runs out.\p"
	.string "Battle me - it'll distract me\n"
	.string "from the heat.$"

SiroccoDesertRoute1_Text_YossDefeat:
	.string "Ha! Completely distracted!$"

SiroccoDesertRoute1_Text_YossPost:
	.string "Good fight. Want some water?\p"
	.string "I have exactly enough for both\n"
	.string "of us if we're careful.$"

SiroccoDesertRoute1_Text_DriedBush:
	.string "A bush, dead long enough that the\n"
	.string "wind has sanded it smooth.\p"
	.string "This was the old riverbank, once.\p"
	.string "Something small and red is caught\n"
	.string "in the roots on the far side.$"
