Convergence_AncientCapital_Interior2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ConvergenceInterior2_OnTransition
	.byte 0

@ ===========================================================================
@ The inner sanctum. The kill switch is visible ahead through the north door.
@ Dorne stands at (10,8). Entering the room (from Interior1) advances progress
@ 1 -> 2, which arms the FinalChoice trigger at the north door (10,6)/(11,6).
@ Crossing it plays Dorne's clear explanation of the switch and the final
@ confirmation, then advances 2 -> 3 and lets the player into the chamber.
@ ===========================================================================
ConvergenceInterior2_OnTransition::
	call_if_eq VAR_CONVERGENCE_PROGRESS, 1, ConvergenceInterior2_EventScript_ArmRoom
	end

ConvergenceInterior2_EventScript_ArmRoom::
	setvar VAR_CONVERGENCE_PROGRESS, 2
	return

@ ---------------------------------------------------------------------------
@ Dorne talk (before crossing to the switch).
@ ---------------------------------------------------------------------------
ConvergenceInterior2_EventScript_Dorne::
	lockall
	applymovement LOCALID_CONVERGENCE_I2_DORNE, Common_Movement_FaceDown
	waitmovement 0
	setspeaker Pelagios_Speaker_Dorne
	goto_if_set FLAG_DORNE_CHOICE_STOP, ConvergenceInterior2_EventScript_DorneTalkStop
	msgbox ConvergenceInterior2_Text_DorneTalkWith, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior2_EventScript_DorneTalkStop::
	msgbox ConvergenceInterior2_Text_DorneTalkStop, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Sign: the kill switch ahead. Also the place the player can examine the
@ mechanism to discover the third function (true ending only).
@ ---------------------------------------------------------------------------
ConvergenceInterior2_EventScript_KillSwitchAhead::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior2_Text_KillSwitchAhead, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior2_EventScript_ThirdFunction
	releaseall
	end

@ TRUE ENDING discovery - revealed only with all ciphers known.
ConvergenceInterior2_EventScript_ThirdFunction::
	msgbox ConvergenceInterior2_Text_ThirdFunction, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ FINAL CHOICE trigger (progress 2, at the north door). Dorne explains the
@ switch clearly one last time, the player confirms, progress -> 3, and the
@ player steps through into the chamber where the chosen ending plays.
@ ---------------------------------------------------------------------------
ConvergenceInterior2_EventScript_FinalChoice::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_CONVERGENCE_I2_DORNE, ConvergenceInterior2_Movement_DorneToDoor
	waitmovement 0
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior2_Text_ExplainSwitch, MSGBOX_DEFAULT
	closemessage

	@ Branch the confirmation prompt on the standing choice.
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior2_EventScript_ConfirmTrueAware
	goto ConvergenceInterior2_EventScript_ConfirmStandard

@ When the third option is known, name it so the player chooses with open eyes.
ConvergenceInterior2_EventScript_ConfirmTrueAware::
	setspeaker 0
	msgbox ConvergenceInterior2_Text_ConfirmTrue, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ConvergenceInterior2_EventScript_HoldBack
	goto ConvergenceInterior2_EventScript_Proceed

ConvergenceInterior2_EventScript_ConfirmStandard::
	setspeaker 0
	msgbox ConvergenceInterior2_Text_ConfirmStandard, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ConvergenceInterior2_EventScript_HoldBack
	goto ConvergenceInterior2_EventScript_Proceed

@ Not yet - let the player step back and gather themselves (trigger re-arms).
ConvergenceInterior2_EventScript_HoldBack::
	setspeaker 0
	msgbox ConvergenceInterior2_Text_HoldBack, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, ConvergenceInterior2_Movement_StepBack
	waitmovement 0
	applymovement LOCALID_CONVERGENCE_I2_DORNE, ConvergenceInterior2_Movement_DorneBack
	waitmovement 0
	releaseall
	end

ConvergenceInterior2_EventScript_Proceed::
	setspeaker 0
	setvar VAR_CONVERGENCE_PROGRESS, 3
	@ Move Dorne aside so he does not block the door, then let the player pass.
	applymovement LOCALID_CONVERGENCE_I2_DORNE, ConvergenceInterior2_Movement_DorneAside
	waitmovement 0
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Movements
@ ---------------------------------------------------------------------------
ConvergenceInterior2_Movement_DorneToDoor:
	walk_up
	face_down
	step_end

ConvergenceInterior2_Movement_DorneBack:
	walk_down
	face_down
	step_end

ConvergenceInterior2_Movement_DorneAside:
	walk_left
	face_right
	step_end

ConvergenceInterior2_Movement_StepBack:
	walk_down
	face_up
	step_end

@ ===========================================================================
@ TEXT
@ ===========================================================================
ConvergenceInterior2_Text_DorneTalkWith:
	.string "The mechanism is ancient and it still\n"
	.string "works. That tells you everything about\l"
	.string "the people who built it.\p"
	.string "When you're ready, we go through.$"

ConvergenceInterior2_Text_DorneTalkStop:
	.string "Don't look for a way to talk me out of\n"
	.string "this. There isn't one.\p"
	.string "If you want to stop me, you'll have to\n"
	.string "stop me. Through that door.$"

ConvergenceInterior2_Text_KillSwitchAhead:
	.string "Through the doorway, the kill switch:\n"
	.string "an enormous mechanism grown into the\l"
	.string "foundation of the temple itself.\p"
	.string "Vast. Patient. Built to last as long as\n"
	.string "the islands do.$"

ConvergenceInterior2_Text_ThirdFunction:
	.string "With every cipher known, the player's\n"
	.string "eye catches what no one has read in two\l"
	.string "thousand years.\p"
	.string "A third setting, dormant beneath the\n"
	.string "other two - built by the same hands,\l"
	.string "hidden on purpose.\p"
	.string "Not END. Not FREE. A third word, worn\n"
	.string "but unmistakable now: OPEN.$"

ConvergenceInterior2_Text_ExplainSwitch:
	.string "Last time, plainly, so no one can say\n"
	.string "they didn't understand.\p"
	.string "This switch ends the seal network.\n"
	.string "Every legendary, unbound at once. The\l"
	.string "Covenant loses what it's been bleeding\l"
	.string "the islands to steal.\p"
	.string "And the world that has leaned on those\n"
	.string "seals for two thousand years finds out\l"
	.string "what it's like to stand without them.$"

ConvergenceInterior2_Text_ConfirmStandard:
	.string "Step through to the switch?$"

ConvergenceInterior2_Text_ConfirmTrue:
	.string "You've seen the third setting now. What\n"
	.string "happens in that chamber is up to the\l"
	.string "hand on the switch.\p"
	.string "Step through?$"

ConvergenceInterior2_Text_HoldBack:
	.string "Take the moment. It's the last one you\n"
	.string "get before the world changes.$"
