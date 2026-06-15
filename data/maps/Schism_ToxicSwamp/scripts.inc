Schism_ToxicSwamp_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Scar gate (SOUTH). Bounce triggers on the two west-edge gap tiles
@ (1,15)/(1,16), armed while VAR_SCHISM_PROGRESS is 1-4 - i.e. before
@ DRENN is defeated and grants the poison Scar pass (progress 5). At
@ progress 5+ (FLAG_DRENN_SCAR_PASS granted) the triggers disarm.
@ -------------------------------------------------------------------
SchismToxicSwamp_EventScript_ScarGate::
	lockall
	setspeaker Pelagios_Speaker_Guard
	msgbox SchismToxicSwamp_Text_ScarGateBlocked, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	applymovement OBJ_EVENT_ID_PLAYER, SchismToxicSwamp_Movement_BounceEast
	waitmovement 0
	releaseall
	end

SchismToxicSwamp_Movement_BounceEast:
	walk_right
	step_end

@ -------------------------------------------------------------------
@ Scar guard - poison faction. Flavor + "steps aside" once the pass
@ is held.
@ -------------------------------------------------------------------
SchismToxicSwamp_EventScript_ScarGuard::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Guard
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismToxicSwamp_EventScript_ScarGuardCeasefire
	goto_if_set FLAG_DRENN_SCAR_PASS, SchismToxicSwamp_EventScript_ScarGuardPass
	msgbox SchismToxicSwamp_Text_ScarGuard, MSGBOX_DEFAULT
	release
	end

SchismToxicSwamp_EventScript_ScarGuardPass::
	msgbox SchismToxicSwamp_Text_ScarGuardPass, MSGBOX_DEFAULT
	release
	end

SchismToxicSwamp_EventScript_ScarGuardCeasefire::
	msgbox SchismToxicSwamp_Text_ScarGuardCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Wader Osk - sight trainer.
@ -------------------------------------------------------------------
SchismToxicSwamp_EventScript_WaderOsk::
	trainerbattle_single TRAINER_POISONFACTION_SCHISM_1, SchismToxicSwamp_Text_OskIntro, SchismToxicSwamp_Text_OskDefeat, SchismToxicSwamp_EventScript_OskPost

SchismToxicSwamp_EventScript_OskPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Osk
	msgbox SchismToxicSwamp_Text_OskPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Wader Fen - sight trainer. (FEN speaker reused from Sirocco.)
@ -------------------------------------------------------------------
SchismToxicSwamp_EventScript_WaderFen::
	trainerbattle_single TRAINER_POISONFACTION_SCHISM_2, SchismToxicSwamp_Text_FenIntro, SchismToxicSwamp_Text_FenDefeat, SchismToxicSwamp_EventScript_FenPost

SchismToxicSwamp_EventScript_FenPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Fen
	msgbox SchismToxicSwamp_Text_FenPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismToxicSwamp_Text_ScarGateBlocked:
	.string "Far enough.\p"
	.string "The Scar's closed to outsiders.\n"
	.string "DRENN's orders.\p"
	.string "No mark, no crossing. Turn\n"
	.string "around before the ground turns\l"
	.string "you around.$"

SchismToxicSwamp_Text_ScarGuard:
	.string "West road runs into the Scar.\p"
	.string "You don't set foot there without\n"
	.string "DRENN's mark. Nobody does.\p"
	.string "Beat the VENOM LORD and earn it,\n"
	.string "if the dead ground's calling you.$"

SchismToxicSwamp_Text_ScarGuardPass:
	.string "…DRENN's mark. Genuine, too.\p"
	.string "Then I won't waste your time or\n"
	.string "mine. Go on.$"

SchismToxicSwamp_Text_ScarGuardCeasefire:
	.string "Order came to open the road.\p"
	.string "Twenty years guarding dead\n"
	.string "ground, and now they tell me to\l"
	.string "just… let people walk it.\p"
	.string "Strange relief, that.$"

SchismToxicSwamp_Text_OskIntro:
	.string "Don't drink the water. Don't\n"
	.string "touch the water.\p"
	.string "Actually, just battle me and\n"
	.string "then keep moving.$"

SchismToxicSwamp_Text_OskDefeat:
	.string "Ngh.$"

SchismToxicSwamp_Text_OskPost:
	.string "Smart. Keep moving.$"

SchismToxicSwamp_Text_FenIntro:
	.string "You smell like the ice side.\p"
	.string "Or maybe that's just outside.\n"
	.string "Hard to tell anymore.$"

SchismToxicSwamp_Text_FenDefeat:
	.string "Tch.$"

SchismToxicSwamp_Text_FenPost:
	.string "You're tougher than you smell.$"
