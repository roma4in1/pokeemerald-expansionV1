Aetheron_SkyRoute_MapScripts::
	.byte 0

@ ===================================================================
@ Guardian Sael (TRAINER_GUARDIAN_AETHERON_1) - sight trainer.
@ Sight trainers MUST keep trainerbattle as the first command, so the
@ intro gets no namebox; setspeaker only before the post-battle msgbox.
@ ===================================================================
AetheronSkyRoute_EventScript_Guardian1::
	trainerbattle_single TRAINER_GUARDIAN_AETHERON_1, AetheronSkyRoute_Text_SaelIntro, AetheronSkyRoute_Text_SaelDefeat, AetheronSkyRoute_EventScript_Guardian1Post
	setspeaker Pelagios_Speaker_Guardian
	msgbox AetheronSkyRoute_Text_SaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronSkyRoute_EventScript_Guardian1Post::
	setspeaker Pelagios_Speaker_Guardian
	msgbox AetheronSkyRoute_Text_SaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Guardian Renn (TRAINER_GUARDIAN_AETHERON_2) - sight trainer.
@ ===================================================================
AetheronSkyRoute_EventScript_Guardian2::
	trainerbattle_single TRAINER_GUARDIAN_AETHERON_2, AetheronSkyRoute_Text_RennIntro, AetheronSkyRoute_Text_RennDefeat, AetheronSkyRoute_EventScript_Guardian2Post
	setspeaker Pelagios_Speaker_Guardian
	msgbox AetheronSkyRoute_Text_RennPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronSkyRoute_EventScript_Guardian2Post::
	setspeaker Pelagios_Speaker_Guardian
	msgbox AetheronSkyRoute_Text_RennPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronSkyRoute_Text_SaelIntro:
	.string "The sky tests everyone who walks it.\n"
	.string "You're being tested now.$"

AetheronSkyRoute_Text_SaelDefeat:
	.string "The sky judges true.$"

AetheronSkyRoute_Text_SaelPost:
	.string "You pass.\p"
	.string "Continue.$"

AetheronSkyRoute_Text_RennIntro:
	.string "We've kept this island hidden for\n"
	.string "generations. The Covenant found it\l"
	.string "anyway.\p"
	.string "At least you came up the honest way.$"

AetheronSkyRoute_Text_RennDefeat:
	.string "Honest and strong, then.$"

AetheronSkyRoute_Text_RennPost:
	.string "The village is ahead.\p"
	.string "GALE is waiting.$"
