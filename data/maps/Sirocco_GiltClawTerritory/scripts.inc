Sirocco_GiltClawTerritory_MapScripts::
	.byte 0

@ Elite Enforcer Voss - TRAINER_GILTCLAW_SIROCCO_5 (sight-initiated;
@ trainerbattle must stay the first command, so the intro gets no
@ namebox - the post-battle line carries the speaker instead).
SiroccoGiltClawTerritory_EventScript_EliteVoss::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_5, SiroccoGiltClawTerritory_Text_VossIntro, SiroccoGiltClawTerritory_Text_VossDefeat
	setspeaker Pelagios_Speaker_Voss
	msgbox SiroccoGiltClawTerritory_Text_VossPost, MSGBOX_AUTOCLOSE
	end

@ Elite Enforcer Mave - TRAINER_GILTCLAW_SIROCCO_6 (sight-initiated)
SiroccoGiltClawTerritory_EventScript_EliteMave::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_6, SiroccoGiltClawTerritory_Text_MaveIntro, SiroccoGiltClawTerritory_Text_MaveDefeat
	setspeaker Pelagios_Speaker_Mave
	msgbox SiroccoGiltClawTerritory_Text_MavePost, MSGBOX_AUTOCLOSE
	end

SiroccoGiltClawTerritory_Text_VossIntro:
	.string "You've gotten further than anyone\n"
	.string "expected.\p"
	.string "DAGAN said that would happen.\p"
	.string "He said to make it hurt anyway.$"

SiroccoGiltClawTerritory_Text_VossDefeat:
	.string "…Didn't hurt you much,\n"
	.string "did it.$"

SiroccoGiltClawTerritory_Text_VossPost:
	.string "He also said to let you through\n"
	.string "when you did.\p"
	.string "He wants to meet you.$"

SiroccoGiltClawTerritory_Text_MaveIntro:
	.string "Last line before the palace.\n"
	.string "Make it count.$"

SiroccoGiltClawTerritory_Text_MaveDefeat:
	.string "It counted.\n"
	.string "Just not for me.$"

SiroccoGiltClawTerritory_Text_MavePost:
	.string "Go ahead. He's expecting you.\p"
	.string "He's always expecting people.$"
