Thalvern_DeepApproach_MapScripts::
	.byte 0

@ ===================================================================
@ Researcher Sael - TRAINER_COVENANT_THALVERN_2 (sight-initiated).
@ ===================================================================
ThalvernDeepApproach_EventScript_ResearcherSael::
	trainerbattle_single TRAINER_COVENANT_THALVERN_2, ThalvernDeepApproach_Text_SaelIntro, ThalvernDeepApproach_Text_SaelDefeat
	setspeaker Pelagios_Speaker_Sael
	msgbox ThalvernDeepApproach_Text_SaelPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Senior Researcher Cael - TRAINER_COVENANT_THALVERN_3 (sight-initiated).
@ ===================================================================
ThalvernDeepApproach_EventScript_ResearcherCael::
	trainerbattle_single TRAINER_COVENANT_THALVERN_3, ThalvernDeepApproach_Text_CaelIntro, ThalvernDeepApproach_Text_CaelDefeat
	setspeaker Pelagios_Speaker_Cael
	msgbox ThalvernDeepApproach_Text_CaelPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Checkpoint sign.
@ ===================================================================
ThalvernDeepApproach_EventScript_CheckpointSign::
	lockall
	msgbox ThalvernDeepApproach_Text_CheckpointSign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernDeepApproach_Text_SaelIntro:
	.string "Director Vess has classified this\n"
	.string "approach.\p"
	.string "No unauthorized personnel.$"

ThalvernDeepApproach_Text_SaelDefeat:
	.string "...That was authorized, apparently.$"

ThalvernDeepApproach_Text_SaelPost:
	.string "I'll note your authorization in my\n"
	.string "report.$"

ThalvernDeepApproach_Text_CaelIntro:
	.string "Last checkpoint before the Throne\n"
	.string "Room.\p"
	.string "You'll need more than strength to\n"
	.string "handle what's inside.$"

ThalvernDeepApproach_Text_CaelDefeat:
	.string "...$"

ThalvernDeepApproach_Text_CaelPost:
	.string "Apparently strength is enough.\p"
	.string "Don't say I didn't warn you.$"

ThalvernDeepApproach_Text_CheckpointSign:
	.string "COVENANT CHECKPOINT\n"
	.string "Authorized access only.\l"
	.string "(The LENS's card overrides this.)$"
