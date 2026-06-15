Gildhaven_TheExchange_Interior1_MapScripts::
	.byte 0

@ ===================================================================
@ Elite Guard Morn - TRAINER_GUARD_GILDHAVEN_3 (sight-initiated).
@ ===================================================================
GildhavenTheExchangeInterior1_EventScript_GuardMorn::
	trainerbattle_single TRAINER_GUARD_GILDHAVEN_3, GildhavenTheExchangeInterior1_Text_MornIntro, GildhavenTheExchangeInterior1_Text_MornDefeat
	setspeaker Pelagios_Speaker_Guard
	msgbox GildhavenTheExchangeInterior1_Text_MornPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Elite Guard Daven - TRAINER_GUARD_GILDHAVEN_4 (sight-initiated).
@ ===================================================================
GildhavenTheExchangeInterior1_EventScript_GuardDaven::
	trainerbattle_single TRAINER_GUARD_GILDHAVEN_4, GildhavenTheExchangeInterior1_Text_DavenIntro, GildhavenTheExchangeInterior1_Text_DavenDefeat
	setspeaker Pelagios_Speaker_Guard
	msgbox GildhavenTheExchangeInterior1_Text_DavenPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Serel Vane on the upper level - watching, untouchable here. Flavor
@ only; the gym battle is in Interior2.
@ ===================================================================
GildhavenTheExchangeInterior1_EventScript_SerelWatching::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Serel
	goto_if_set FLAG_GILDHAVEN_GYM4_CLEAR, GildhavenTheExchangeInterior1_EventScript_SerelWatchingDone
	msgbox GildhavenTheExchangeInterior1_Text_SerelWatching, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenTheExchangeInterior1_EventScript_SerelWatchingDone::
	msgbox GildhavenTheExchangeInterior1_Text_SerelWatchingDone, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Lobby plaque.
@ ===================================================================
GildhavenTheExchangeInterior1_EventScript_Plaque::
	lockall
	msgbox GildhavenTheExchangeInterior1_Text_Plaque, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
GildhavenTheExchangeInterior1_Text_MornIntro:
	.string "You have gotten further than anyone\n"
	.string "expected.\p"
	.string "Mr. VANE is impressed. He asked me to\n"
	.string "make it hurt.$"

GildhavenTheExchangeInterior1_Text_MornDefeat:
	.string "...That hurt. The wrong one of us.$"

GildhavenTheExchangeInterior1_Text_MornPost:
	.string "He will be less impressed when I\n"
	.string "report this.$"

GildhavenTheExchangeInterior1_Text_DavenIntro:
	.string "Final checkpoint. Mr. VANE is\n"
	.string "upstairs.$"

GildhavenTheExchangeInterior1_Text_DavenDefeat:
	.string "...Go up, then. He has been waiting\n"
	.string "for you longer than you know.$"

GildhavenTheExchangeInterior1_Text_DavenPost:
	.string "He is expecting you now.$"

GildhavenTheExchangeInterior1_Text_SerelWatching:
	.string "(Far above, on a glass balcony, a man\n"
	.string "in a fine coat watches you climb. He\l"
	.string "does not move.)\p"
	.string "He lifts one hand - not a wave. An\n"
	.string "acknowledgement. The gesture of a man\l"
	.string "ticking a box.\p"
	.string "Then he turns and walks deeper into\n"
	.string "the building.$"

GildhavenTheExchangeInterior1_Text_SerelWatchingDone:
	.string "(The glass balcony is empty now.\n"
	.string "Nobody watches the hall anymore.)$"

GildhavenTheExchangeInterior1_Text_Plaque:
	.string "A bronze plaque, polished to a glare.\p"
	.string "FOUNDED BY THE VANE FAMILY. Built on\n"
	.string "vision, it reads.\p"
	.string "Built on something, anyway.$"
