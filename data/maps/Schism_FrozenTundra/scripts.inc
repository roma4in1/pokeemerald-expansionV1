Schism_FrozenTundra_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Scar gate (NORTH). Bounce triggers on the two east-edge gap tiles
@ (18,15)/(18,16), armed while VAR_SCHISM_PROGRESS is 1 or 2 - i.e.
@ before EIRA is defeated and grants the ice Scar pass. At progress 3+
@ (FLAG_EIRA_SCAR_PASS granted) the triggers disarm and the road to
@ the Scar opens. Coord triggers compare VARS, so the gating var is
@ VAR_SCHISM_PROGRESS; FLAG_EIRA_SCAR_PASS keys the soldier's dialogue.
@ -------------------------------------------------------------------
SchismFrozenTundra_EventScript_ScarGate::
	lockall
	setspeaker Pelagios_Speaker_Soldier
	msgbox SchismFrozenTundra_Text_ScarGateBlocked, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	applymovement OBJ_EVENT_ID_PLAYER, SchismFrozenTundra_Movement_BounceWest
	waitmovement 0
	releaseall
	end

SchismFrozenTundra_Movement_BounceWest:
	walk_left
	step_end

@ -------------------------------------------------------------------
@ Scar guard - stationed at the Scar road. Flavor + the "steps aside"
@ beat once the pass is held. (The actual block is the coord triggers
@ above; this NPC explains it.)
@ -------------------------------------------------------------------
SchismFrozenTundra_EventScript_ScarGuard::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Soldier
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrozenTundra_EventScript_ScarGuardCeasefire
	goto_if_set FLAG_EIRA_SCAR_PASS, SchismFrozenTundra_EventScript_ScarGuardPass
	msgbox SchismFrozenTundra_Text_ScarGuard, MSGBOX_DEFAULT
	release
	end

SchismFrozenTundra_EventScript_ScarGuardPass::
	msgbox SchismFrozenTundra_Text_ScarGuardPass, MSGBOX_DEFAULT
	release
	end

SchismFrozenTundra_EventScript_ScarGuardCeasefire::
	msgbox SchismFrozenTundra_Text_ScarGuardCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Scout Heln - sight trainer. trainerbattle MUST be the first command;
@ setspeaker only before the post-battle msgbox.
@ -------------------------------------------------------------------
SchismFrozenTundra_EventScript_ScoutHeln::
	trainerbattle_single TRAINER_ICEFACTION_SCHISM_1, SchismFrozenTundra_Text_HelnIntro, SchismFrozenTundra_Text_HelnDefeat, SchismFrozenTundra_EventScript_HelnPost

SchismFrozenTundra_EventScript_HelnPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Heln
	msgbox SchismFrozenTundra_Text_HelnPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Scout Vera - sight trainer.
@ -------------------------------------------------------------------
SchismFrozenTundra_EventScript_ScoutVera::
	trainerbattle_single TRAINER_ICEFACTION_SCHISM_2, SchismFrozenTundra_Text_VeraIntro, SchismFrozenTundra_Text_VeraDefeat, SchismFrozenTundra_EventScript_VeraPost

SchismFrozenTundra_EventScript_VeraPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Vera
	msgbox SchismFrozenTundra_Text_VeraPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismFrozenTundra_Text_ScarGateBlocked:
	.string "Stop.\p"
	.string "The Scar is restricted.\n"
	.string "Commander's orders.\p"
	.string "Come back when you carry EIRA's\n"
	.string "mark, or don't come back at all.$"

SchismFrozenTundra_Text_ScarGuard:
	.string "The road east leads to the Scar.\p"
	.string "Nobody crosses without the\n"
	.string "Commander's mark. Not even us.\p"
	.string "Beat EIRA and earn it, if you're\n"
	.string "set on seeing the dead ground.$"

SchismFrozenTundra_Text_ScarGuardPass:
	.string "…You carry her mark.\p"
	.string "Then I've nothing to say to you.\n"
	.string "The road's yours.$"

SchismFrozenTundra_Text_ScarGuardCeasefire:
	.string "Orders changed. We don't hold\n"
	.string "this road anymore.\p"
	.string "Felt wrong stepping aside the\n"
	.string "first day. Feels less wrong now.$"

SchismFrozenTundra_Text_HelnIntro:
	.string "Ice faction territory.\n"
	.string "State your business.$"

SchismFrozenTundra_Text_HelnDefeat:
	.string "…$"

SchismFrozenTundra_Text_HelnPost:
	.string "…Pass through. Carefully.$"

SchismFrozenTundra_Text_VeraIntro:
	.string "The cold doesn't bother us.\n"
	.string "Does it bother you?$"

SchismFrozenTundra_Text_VeraDefeat:
	.string "Hm.$"

SchismFrozenTundra_Text_VeraPost:
	.string "Apparently not. Interesting.$"
