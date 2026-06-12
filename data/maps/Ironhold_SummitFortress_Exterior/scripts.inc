Ironhold_SummitFortress_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Ironhold_SummitFortress_Exterior_OnTransition
	.byte 0

@ -------------------------------------------------------------------
@ Sever gate cameo visibility (FLAG_HIDE_IRONHOLD_SEVER_GATE):
@ - hidden until BOTH elite guards are defeated (the post-battle
@   reveal below clears the flag and adds the object),
@ - hidden again permanently once the cameo plays (his own script
@   re-sets the flag) or once Gym 4 is cleared.
@ Flags default to 0 (visible), so ON_TRANSITION must re-assert the
@ hidden state on every map load while the guards still stand.
@ -------------------------------------------------------------------
Ironhold_SummitFortress_Exterior_OnTransition:
	goto_if_set FLAG_IRONHOLD_GYM4_CLEAR, Ironhold_SummitFortress_Exterior_OnTransition_Hide
	goto_if_defeated TRAINER_COVENANT_IRONHOLD_4, Ironhold_SummitFortress_Exterior_OnTransition_CheckSecond
	goto Ironhold_SummitFortress_Exterior_OnTransition_Hide

Ironhold_SummitFortress_Exterior_OnTransition_CheckSecond:
	goto_if_defeated TRAINER_COVENANT_IRONHOLD_9, Ironhold_SummitFortress_Exterior_OnTransition_End
	goto Ironhold_SummitFortress_Exterior_OnTransition_Hide

Ironhold_SummitFortress_Exterior_OnTransition_Hide:
	setflag FLAG_HIDE_IRONHOLD_SEVER_GATE

Ironhold_SummitFortress_Exterior_OnTransition_End:
	end

@ -------------------------------------------------------------------
@ Elite Guard Vael - TRAINER_COVENANT_IRONHOLD_4 (sight-initiated).
@ NOT the villain - a different character. Common Covenant name;
@ deliberate, subtle detail.
@ -------------------------------------------------------------------
IronholdSummitFortressExterior_EventScript_EliteVael::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_4, IronholdSummitFortressExterior_Text_VaelIntro, IronholdSummitFortressExterior_Text_VaelDefeat, IronholdSummitFortressExterior_EventScript_VaelVictory
	setspeaker Pelagios_Speaker_Vael
	msgbox IronholdSummitFortressExterior_Text_VaelPost, MSGBOX_AUTOCLOSE
	end

IronholdSummitFortressExterior_EventScript_VaelVictory::
	setspeaker Pelagios_Speaker_Vael
	msgbox IronholdSummitFortressExterior_Text_VaelPost, MSGBOX_DEFAULT
	closemessage
	goto_if_defeated TRAINER_COVENANT_IRONHOLD_9, IronholdSummitFortressExterior_EventScript_SeverReveal
	release
	end

@ -------------------------------------------------------------------
@ Elite Guard Senna - TRAINER_COVENANT_IRONHOLD_9 (sight-initiated)
@ -------------------------------------------------------------------
IronholdSummitFortressExterior_EventScript_EliteGuard2::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_9, IronholdSummitFortressExterior_Text_SennaIntro, IronholdSummitFortressExterior_Text_SennaDefeat, IronholdSummitFortressExterior_EventScript_SennaVictory
	setspeaker Pelagios_Speaker_Senna
	msgbox IronholdSummitFortressExterior_Text_SennaPost, MSGBOX_AUTOCLOSE
	end

IronholdSummitFortressExterior_EventScript_SennaVictory::
	setspeaker Pelagios_Speaker_Senna
	msgbox IronholdSummitFortressExterior_Text_SennaPost, MSGBOX_DEFAULT
	closemessage
	goto_if_defeated TRAINER_COVENANT_IRONHOLD_4, IronholdSummitFortressExterior_EventScript_SeverReveal
	release
	end

@ Both guards down: Sever steps out of the gate.
IronholdSummitFortressExterior_EventScript_SeverReveal::
	goto_if_set FLAG_IRONHOLD_GYM4_CLEAR, IronholdSummitFortressExterior_EventScript_SeverRevealSkip
	delay 30
	playse SE_DOOR
	clearflag FLAG_HIDE_IRONHOLD_SEVER_GATE
	addobject LOCALID_IRONHOLD_SEVER_GATE
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox IronholdSummitFortressExterior_Text_SeverAppears, MSGBOX_DEFAULT
	closemessage
	release
	end

IronholdSummitFortressExterior_EventScript_SeverRevealSkip::
	release
	end

@ -------------------------------------------------------------------
@ Commander Sever, at the gate. Brief dialogue, steps aside (walks
@ into the fortress ahead of you), says nothing more.
@ -------------------------------------------------------------------
IronholdSummitFortressExterior_EventScript_Sever::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sever
	msgbox IronholdSummitFortressExterior_Text_SeverGate, MSGBOX_DEFAULT
	closemessage
	delay 20
	applymovement LOCALID_IRONHOLD_SEVER_GATE, IronholdSummitFortressExterior_Movement_SeverIntoFortress
	waitmovement 0
	playse SE_DOOR
	removeobject LOCALID_IRONHOLD_SEVER_GATE
	setflag FLAG_HIDE_IRONHOLD_SEVER_GATE
	release
	end

IronholdSummitFortressExterior_Movement_SeverIntoFortress:
	face_up
	delay_16
	walk_up
	walk_up
	walk_up
	step_end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdSummitFortressExterior_Text_VaelIntro:
	.string "The Commander doesn't want\n"
	.string "visitors.$"

IronholdSummitFortressExterior_Text_VaelDefeat:
	.string "…$"

IronholdSummitFortressExterior_Text_VaelPost:
	.string "…Go ahead.\p"
	.string "He's expecting you.$"

IronholdSummitFortressExterior_Text_SennaIntro:
	.string "Two guards. One gate. The math\n"
	.string "has never failed before.$"

IronholdSummitFortressExterior_Text_SennaDefeat:
	.string "…The math failed.$"

IronholdSummitFortressExterior_Text_SennaPost:
	.string "Nobody has ever beaten both of\n"
	.string "us.\p"
	.string "…The Commander will want to see\n"
	.string "the one who did.$"

IronholdSummitFortressExterior_Text_SeverAppears:
	.string "The fortress gate grinds open.\p"
	.string "A man in a Commander's coat\n"
	.string "stands in the gap, watching you\l"
	.string "with the stillness of someone\l"
	.string "who has been waiting a long\l"
	.string "time.$"

IronholdSummitFortressExterior_Text_SeverGate:
	.string "Commander Holt SEVER.\p"
	.string "You already know that, and I\n"
	.string "already know you.\p"
	.string "…You have your parent's way of\n"
	.string "standing. Did anyone ever tell\l"
	.string "you that?\p"
	.string "The gate is open. My lieutenant\n"
	.string "is on the first floor. I am at\l"
	.string "the top.\p"
	.string "Come up when you're ready.$"
