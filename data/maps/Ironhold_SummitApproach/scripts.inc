Ironhold_SummitApproach_MapScripts::
	.byte 0

@ Elite Guard Sorn - TRAINER_COVENANT_IRONHOLD_3 (sight-initiated).
@ Says nothing after losing; steps aside in silence.
IronholdSummitApproach_EventScript_EliteSorn::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_3, IronholdSummitApproach_Text_SornIntro, IronholdSummitApproach_Text_SornDefeat
	msgbox IronholdSummitApproach_Text_SornPost, MSGBOX_AUTOCLOSE
	end

@ Soldier Bryn - TRAINER_COVENANT_IRONHOLD_5 (sight-initiated)
IronholdSummitApproach_EventScript_Patrol2::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_5, IronholdSummitApproach_Text_BrynIntro, IronholdSummitApproach_Text_BrynDefeat
	msgbox IronholdSummitApproach_Text_BrynPost, MSGBOX_AUTOCLOSE
	end

@ Soldier Karst - TRAINER_COVENANT_IRONHOLD_6 (sight-initiated)
IronholdSummitApproach_EventScript_Patrol3::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_6, IronholdSummitApproach_Text_KarstIntro, IronholdSummitApproach_Text_KarstDefeat
	msgbox IronholdSummitApproach_Text_KarstPost, MSGBOX_AUTOCLOSE
	end

@ Vista over the whole island
IronholdSummitApproach_EventScript_Vista::
	msgbox IronholdSummitApproach_Text_Vista, MSGBOX_SIGN
	end

IronholdSummitApproach_Text_SornIntro:
	.string "Final checkpoint.\p"
	.string "Last chance to turn around.$"

IronholdSummitApproach_Text_SornDefeat:
	.string "…$"

IronholdSummitApproach_Text_SornPost:
	.string "…\p"
	.string "He steps aside in silence.$"

IronholdSummitApproach_Text_BrynIntro:
	.string "Nobody walks this ridge by\n"
	.string "accident.\p"
	.string "So let's not pretend this is an\n"
	.string "accident.$"

IronholdSummitApproach_Text_BrynDefeat:
	.string "Storm take it.$"

IronholdSummitApproach_Text_BrynPost:
	.string "You feel it too, don't you.\n"
	.string "That hum under your boots.\p"
	.string "They tell us it's the wind.\p"
	.string "Wind doesn't keep time.$"

IronholdSummitApproach_Text_KarstIntro:
	.string "Orders say: nobody reaches the\n"
	.string "fortress.\p"
	.string "I always follow orders. It's\n"
	.string "easier than the alternative.$"

IronholdSummitApproach_Text_KarstDefeat:
	.string "…Maybe easier isn't the word.$"

IronholdSummitApproach_Text_KarstPost:
	.string "Go on, then.\p"
	.string "Whatever's up there, it was never\n"
	.string "in my orders to understand it.$"

IronholdSummitApproach_Text_Vista:
	.string "The whole of IRONHOLD spreads out\n"
	.string "below the ridge.\p"
	.string "The grey grid of the city. The\n"
	.string "port, and the small bright speck\l"
	.string "of the TENNYSON at her berth.\p"
	.string "From up here you can see what the\n"
	.string "island used to be, underneath\l"
	.string "what it has been made into.$"
