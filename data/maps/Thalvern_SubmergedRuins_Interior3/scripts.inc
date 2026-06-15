Thalvern_SubmergedRuins_Interior3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_SubmergedRuins_Interior3_OnTransition
	.byte 0

@ ===================================================================
@ Dex's cameo object here (LOCALID_THALVERN_RUINS3_DEX) uses FLAG_TEMP_1
@ as its hide flag. Temp flags clear on every load (so he'd be visible
@ by default) - re-hide him each load. The choice scene clears the flag
@ and addobjects him for the scene, then he's gone again next entry
@ (his real presence in the deep is in the ThroneRoom flow).
@ ===================================================================
Thalvern_SubmergedRuins_Interior3_OnTransition:
	setflag FLAG_TEMP_1
	end

@ ===================================================================
@ The Choice Scene. Coord trigger at (8/9,3), gated VAR_THALVERN_PROGRESS
@ == 4 (Gym 3 cleared). The Lens delivers the warning about the vision's
@ weight; Dex steps in behind. The player chooses who goes into the
@ Throne Room first.
@
@ PATH DETERMINANT: VAR_PELAGIOS_RESERVED_0x410B
@   0 = unset, 1 = PATH A (player first), 2 = PATH B (Dex first)
@ The ThroneRoom reads this one var to branch PATH A vs PATH B, so the
@ two paths are MUTUALLY EXCLUSIVE by construction. FLAG_DEX_ALIVE is
@ NOT set here - it is set ONLY in PATH A's recovery.
@
@ Either choice advances progress to 5 (Throne Room entered) and sets
@ FLAG_THALVERN_THRONE_CHOICE + FLAG_THALVERN_SEAL_FOUND, then the
@ player proceeds up to the ThroneRoom warp on their own.
@ ===================================================================
ThalvernSubmergedRuinsInterior3_EventScript_ChoiceScene::
	lockall
	goto_if_set FLAG_THALVERN_THRONE_CHOICE, ThalvernSubmergedRuinsInterior3_EventScript_ChoiceDone
	@ Bring Dex into view behind the player.
	clearflag FLAG_TEMP_1
	addobject LOCALID_THALVERN_RUINS3_DEX
	delay 20
	setspeaker Pelagios_Speaker_Lens
	msgbox ThalvernSubmergedRuinsInterior3_Text_LensWarning, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernSubmergedRuinsInterior3_Text_DexReady, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	msgbox ThalvernSubmergedRuinsInterior3_Text_ChoicePrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ThalvernSubmergedRuinsInterior3_EventScript_LetDexGo
	@ "I'll go in." - PATH A.
	setvar VAR_PELAGIOS_RESERVED_0x410B, 1
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernSubmergedRuinsInterior3_Text_DexAcceptsPlayer, MSGBOX_DEFAULT
	goto ThalvernSubmergedRuinsInterior3_EventScript_ChoiceFinish

ThalvernSubmergedRuinsInterior3_EventScript_LetDexGo::
	@ "Let Dex go ahead." - PATH B.
	setvar VAR_PELAGIOS_RESERVED_0x410B, 2
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernSubmergedRuinsInterior3_Text_DexGoesFirst, MSGBOX_DEFAULT
	goto ThalvernSubmergedRuinsInterior3_EventScript_ChoiceFinish

ThalvernSubmergedRuinsInterior3_EventScript_ChoiceFinish::
	closemessage
	setspeaker 0
	setflag FLAG_THALVERN_THRONE_CHOICE
	setflag FLAG_THALVERN_SEAL_FOUND
	setvar VAR_THALVERN_PROGRESS, 5
	@ Dex withdraws to wait; the ThroneRoom flow handles his role.
	applymovement LOCALID_THALVERN_RUINS3_DEX, ThalvernSubmergedRuinsInterior3_Movement_DexStepBack
	waitmovement 0
	removeobject LOCALID_THALVERN_RUINS3_DEX
	setflag FLAG_TEMP_1
	releaseall
	end

ThalvernSubmergedRuinsInterior3_EventScript_ChoiceDone::
	releaseall
	end

ThalvernSubmergedRuinsInterior3_Movement_DexStepBack:
	walk_down
	walk_down
	face_up
	step_end

@ ===================================================================
@ The Lens as a talk NPC (before the trigger, or if approached). Quiet
@ pre-choice flavor. After the choice is locked, a brief line.
@ ===================================================================
ThalvernSubmergedRuinsInterior3_EventScript_Lens::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_THALVERN_THRONE_CHOICE, ThalvernSubmergedRuinsInterior3_EventScript_LensAfter
	msgbox ThalvernSubmergedRuinsInterior3_Text_LensTalk, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior3_EventScript_LensAfter::
	msgbox ThalvernSubmergedRuinsInterior3_Text_LensAfter, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Dex as a talk NPC - only meaningful during the cleared-cameo window;
@ since he is FLAG_TEMP_1-hidden outside the choice scene, this script
@ is a safe fallback if he is ever talkable.
@ ===================================================================
ThalvernSubmergedRuinsInterior3_EventScript_Dex::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernSubmergedRuinsInterior3_Text_DexFallback, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernSubmergedRuinsInterior3_Text_LensWarning:
	.string "Stop. Before you go through.\p"
	.string "The Throne Room vision isn't a\n"
	.string "fragment like the others. It's the\l"
	.string "whole thing. Complete.\p"
	.string "Whoever goes in first takes the\n"
	.string "full weight of it.\p"
	.string "DEX is right behind you.\p"
	.string "One of you goes first. That\n"
	.string "person - I don't know what it does\l"
	.string "to them.$"

ThalvernSubmergedRuinsInterior3_Text_DexReady:
	.string "I've prepared for this my whole\n"
	.string "life. I should be the one.\p"
	.string "...But it's your call. It's your\n"
	.string "parent's work. Your seal to mend.\p"
	.string "Tell me. Do I go first, or do you?$"

ThalvernSubmergedRuinsInterior3_Text_ChoicePrompt:
	.string "Will you go into the Throne Room\n"
	.string "first?$"

ThalvernSubmergedRuinsInterior3_Text_DexAcceptsPlayer:
	.string "...All right. All right.\p"
	.string "I'll be right behind you. Whatever\n"
	.string "happens in there - I'm not leaving\l"
	.string "your side.\p"
	.string "Go. We'll learn it together.$"

ThalvernSubmergedRuinsInterior3_Text_DexGoesFirst:
	.string "...Thank you. Truly.\p"
	.string "I've waited so long for this. Let\n"
	.string "me be the one to see it.\p"
	.string "Wait here a moment. I'll call you\n"
	.string "through when it's safe.$"

ThalvernSubmergedRuinsInterior3_Text_LensTalk:
	.string "The chamber beyond is the last one.\p"
	.string "Whatever you decide up there -\n"
	.string "decide it knowing someone bears the\l"
	.string "weight of going first.$"

ThalvernSubmergedRuinsInterior3_Text_LensAfter:
	.string "It's done, then. Go on.\p"
	.string "I'll keep the Covenant off this\n"
	.string "passage as long as I can.$"

ThalvernSubmergedRuinsInterior3_Text_DexFallback:
	.string "Ready when you are.$"
