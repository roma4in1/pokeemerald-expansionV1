Ironhold_OuterDistrict_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Officer Mace - TRAINER_OFFICER_IRONHOLD_1 (sight-initiated)
@ -------------------------------------------------------------------
IronholdOuterDistrict_EventScript_OfficerMace::
	trainerbattle_single TRAINER_OFFICER_IRONHOLD_1, IronholdOuterDistrict_Text_MaceIntro, IronholdOuterDistrict_Text_MaceDefeat
	msgbox IronholdOuterDistrict_Text_MacePost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Worker Finn - TRAINER_WORKER_IRONHOLD_1 (sight-initiated)
@ -------------------------------------------------------------------
IronholdOuterDistrict_EventScript_WorkerFinn::
	trainerbattle_single TRAINER_WORKER_IRONHOLD_1, IronholdOuterDistrict_Text_FinnIntro, IronholdOuterDistrict_Text_FinnDefeat
	msgbox IronholdOuterDistrict_Text_FinnPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Grapple Hook rubble gate (west exit to MountainPass).
@ Coord trigger, armed while VAR_IRONHOLD_PROGRESS < 3 (Forge sets
@ progress 3 together with FLAG_GRAPPLE_HOOK_OBTAINED, after which the
@ trigger no longer fires and the pass is open).
@ The checkitem branch is defensive: if the player somehow holds the
@ hook while the trigger is still armed, it clears rather than blocks.
@ -------------------------------------------------------------------
IronholdOuterDistrict_EventScript_Rubble::
	lockall
	checkitem ITEM_GRAPPLE_HOOK
	goto_if_eq VAR_RESULT, TRUE, IronholdOuterDistrict_EventScript_RubbleCleared
	msgbox IronholdOuterDistrict_Text_RubbleBlocked, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, IronholdOuterDistrict_Movement_StepBackEast
	waitmovement 0
	releaseall
	end

IronholdOuterDistrict_EventScript_RubbleCleared::
	playse SE_ICE_BREAK
	waitse
	msgbox IronholdOuterDistrict_Text_RubbleCleared, MSGBOX_DEFAULT
	releaseall
	end

IronholdOuterDistrict_Movement_StepBackEast:
	walk_right
	step_end

@ -------------------------------------------------------------------
@ Resistance graffiti
@ -------------------------------------------------------------------
IronholdOuterDistrict_EventScript_Graffiti::
	msgbox IronholdOuterDistrict_Text_Graffiti, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdOuterDistrict_Text_MaceIntro:
	.string "Halt.\p"
	.string "Covenant regulation 7: all\n"
	.string "travelers must be assessed for\l"
	.string "hostile intent.\p"
	.string "Prepare to be assessed.$"

IronholdOuterDistrict_Text_MaceDefeat:
	.string "…Assessment complete.$"

IronholdOuterDistrict_Text_MacePost:
	.string "…Assessment complete. You may\n"
	.string "pass.\p"
	.string "Don't make me regret it.$"

IronholdOuterDistrict_Text_FinnIntro:
	.string "I just work the forges. I don't\n"
	.string "want trouble.\p"
	.string "But I can't just let strangers\n"
	.string "walk past either.$"

IronholdOuterDistrict_Text_FinnDefeat:
	.string "You're strong.$"

IronholdOuterDistrict_Text_FinnPost:
	.string "You're strong. The WARDEN's kid,\n"
	.string "right?\p"
	.string "Someone said you'd come\n"
	.string "eventually.$"

IronholdOuterDistrict_Text_RubbleBlocked:
	.string "The pass west is buried under\n"
	.string "collapsed rock.\p"
	.string "The slabs are wedged tight - far\n"
	.string "too heavy to move by hand.\p"
	.string "Climbing gear could anchor into\n"
	.string "the cracks and haul them loose,\l"
	.string "if you had any.$"

IronholdOuterDistrict_Text_RubbleCleared:
	.string "You set the GRAPPLE HOOK into the\n"
	.string "cracked slab and haul-\p"
	.string "The rockfall tears loose and\n"
	.string "scatters down the slope!\p"
	.string "The way to the MOUNTAIN PASS is\n"
	.string "open.$"

IronholdOuterDistrict_Text_Graffiti:
	.string "Someone has painted words on the\n"
	.string "wall. The Covenant has scratched\l"
	.string "most of them out.\p"
	.string "What survives is still readable:\p"
	.string "“THE SUMMIT FEEDS ON SOMETHING\n"
	.string "IT SHOULDN'T”$"
