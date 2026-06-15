Convergence_KillSwitchChamber_MapScripts::
	.byte 0

@ ===========================================================================
@ THE KILL SWITCH CHAMBER - where all three endings resolve.
@
@ The player warps in from Interior2 at the south (12,37) and walks north.
@ Crossing row 34 (VAR_CONVERGENCE_PROGRESS == 3) fires the Ending trigger,
@ which resolves the chosen ending and warps to the Epilogue.
@
@ ROUTING (must match Interior1/Interior2):
@   - Interior1 sets FLAG_DORNE_CHOICE_STOP or _HELP (DEFER resolves into one
@     of those; the true path sets _HELP as the cooperative posture).
@   - FLAG_TRUE_ENDING_UNLOCKED OVERRIDES both - it is checked FIRST here.
@
@ Each ending sets its FLAG_ENDING_*_PLAYED marker, advances
@ VAR_CONVERGENCE_PROGRESS to 5 (the value the Epilogue's credits trigger
@ waits on), and warps to MAP_CONVERGENCE_EPILOGUE.
@
@ The kill switch object (12,4), Dorne (14,7), and Cass (11,9) are present
@ for flavor talk; the real ending logic is the trigger below.
@ ===========================================================================

@ ---------------------------------------------------------------------------
@ ENDING ENTRY - the trigger the player crosses at (12,34)/(13,34).
@ TRUE first, then STOP/HELP.
@ ---------------------------------------------------------------------------
ConvergenceKillSwitch_EventScript_Ending::
	lockall
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceKillSwitch_EventScript_EndingTrue
	goto_if_set FLAG_DORNE_CHOICE_STOP, ConvergenceKillSwitch_EventScript_EndingStop
	goto ConvergenceKillSwitch_EventScript_EndingHelp

@ ===========================================================================
@ ENDING 1 - STOP DORNE (true ending NOT unlocked, FLAG_DORNE_CHOICE_STOP).
@ ===========================================================================
ConvergenceKillSwitch_EventScript_EndingStop::
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_StopDorneActivates, MSGBOX_DEFAULT
	closemessage
	@ Dorne sets his hand to the mechanism. The player must reach him first.
	applymovement LOCALID_CONVERGENCE_KS_DORNE, ConvergenceKillSwitch_Movement_DorneToSwitch
	waitmovement 0
	delay 20

	@ THE FINAL BATTLE.
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_StopBattleIntro, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_1, 0
	trainerbattle_no_intro TRAINER_LEADER_DORNE, ConvergenceKillSwitch_Text_StopBattleDefeat

	@ Post-battle. He steps back from the switch.
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_StopPostBattle, MSGBOX_DEFAULT
	closemessage
	delay 40

	@ He leaves - quietly. No music sting, no camera. Just done.
	applymovement LOCALID_CONVERGENCE_KS_DORNE, ConvergenceKillSwitch_Movement_DorneLeave
	waitmovement 0
	removeobject LOCALID_CONVERGENCE_KS_DORNE
	delay 20

	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_StopFleetWithdraws, MSGBOX_DEFAULT
	closemessage

	setflag FLAG_DORNE_FINAL_BATTLE_DONE
	setflag FLAG_ENDING_STOP_PLAYED
	goto ConvergenceKillSwitch_EventScript_GoToEpilogue

@ ===========================================================================
@ ENDING 2 - HELP DORNE (true ending NOT unlocked, FLAG_DORNE_CHOICE_HELP).
@ ===========================================================================
ConvergenceKillSwitch_EventScript_EndingHelp::
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_HelpApproach, MSGBOX_DEFAULT
	closemessage
	@ Joint activation - Dorne and the player both reach for the mechanism.
	applymovement LOCALID_CONVERGENCE_KS_DORNE, ConvergenceKillSwitch_Movement_DorneToSwitch
	waitmovement 0
	delay 20
	msgbox ConvergenceKillSwitch_Text_HelpActivate, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ The cries of every island's legendary, simultaneous and overwhelming.
	@ Placeholder cries reuse each island seal chamber's SPECIES_* exactly.
	playmoncry SPECIES_KYOGRE, CRY_MODE_ENCOUNTER     @ Thalvern - Feraligatr line
	delay 25
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER    @ Emberveil - Infernape line
	delay 25
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER   @ Primalis - Decidueye/Verdath
	delay 25
	playmoncry SPECIES_REGICE, CRY_MODE_ENCOUNTER     @ Schism north - Glacith
	delay 25
	playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER  @ Schism south - Toxara
	delay 25
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER     @ Aetheron - Stormveil
	delay 25
	playmoncry SPECIES_LUGIA, CRY_MODE_ENCOUNTER      @ Pelagios (no prior cry - placeholder)
	delay 25
	playmoncry SPECIES_GIRATINA, CRY_MODE_ENCOUNTER   @ Morthas (no prior cry - placeholder)
	delay 30

	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_HelpNetworkCollapses, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ Dorne and the player speak over the collapse.
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_HelpDorneDone, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceKillSwitch_Text_HelpCassFleet, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_HelpDorneLetThem, MSGBOX_DEFAULT
	closemessage

	@ The fleet arrives, finds nothing to exploit, and anchors.
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_HelpFleetArrives, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ The quiet exchange that closes the scene.
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_HelpDorneDifferent, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceKillSwitch_Text_HelpCassWrong, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_HelpDorneQuieter, MSGBOX_DEFAULT
	closemessage

	setflag FLAG_ENDING_HELP_PLAYED
	goto ConvergenceKillSwitch_EventScript_GoToEpilogue

@ ===========================================================================
@ ENDING 3 - TRUE (FLAG_TRUE_ENDING_UNLOCKED, overrides STOP/HELP).
@ ===========================================================================
ConvergenceKillSwitch_EventScript_EndingTrue::
	@ The player examines the switch - the third function surfaces.
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_TrueThirdFunction, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ The player explains it to Dorne; he reads, and pauses.
	msgbox ConvergenceKillSwitch_Text_TruePlayerExplains, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CONVERGENCE_KS_DORNE, ConvergenceKillSwitch_Movement_DorneToSwitch
	waitmovement 0
	delay 60

	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_TrueDorneReads, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ Joint activation requiring the player's own touch.
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_TrueWardensTouch, MSGBOX_DEFAULT
	closemessage
	delay 20

	msgbox ConvergenceKillSwitch_Text_TrueNetworkDissolves, MSGBOX_DEFAULT
	closemessage
	delay 20

	msgbox ConvergenceKillSwitch_Text_TrueFleetLeaves, MSGBOX_DEFAULT
	closemessage

	setflag FLAG_ENDING_TRUE_PLAYED
	goto ConvergenceKillSwitch_EventScript_GoToEpilogue

@ ---------------------------------------------------------------------------
@ Shared tail: advance to the Epilogue. Set progress to 5 so the Epilogue's
@ credits trigger (which waits on VAR_CONVERGENCE_PROGRESS == 5) fires on
@ arrival, then warp.
@ ---------------------------------------------------------------------------
ConvergenceKillSwitch_EventScript_GoToEpilogue::
	setvar VAR_CONVERGENCE_PROGRESS, 5
	fadescreen FADE_TO_BLACK
	warp MAP_CONVERGENCE_EPILOGUE, 11, 16
	waitstate
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Flavor talk (only reachable if the player turns back before the trigger;
@ the mechanism, Dorne, and Cass each have a line).
@ ---------------------------------------------------------------------------
ConvergenceKillSwitch_EventScript_Mechanism::
	lockall
	setspeaker 0
	msgbox ConvergenceKillSwitch_Text_MechanismExamine, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceKillSwitch_EventScript_Dorne::
	lockall
	applymovement LOCALID_CONVERGENCE_KS_DORNE, Common_Movement_FaceDown
	waitmovement 0
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceKillSwitch_Text_DorneWaiting, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceKillSwitch_EventScript_Cass::
	lockall
	applymovement LOCALID_CONVERGENCE_KS_CASS, Common_Movement_FaceDown
	waitmovement 0
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceKillSwitch_Text_CassWaiting, MSGBOX_DEFAULT
	releaseall
	end

@ ===========================================================================
@ MOVEMENTS
@ ===========================================================================
ConvergenceKillSwitch_Movement_DorneToSwitch:
	walk_up
	face_up
	step_end

ConvergenceKillSwitch_Movement_DorneLeave:
	walk_down
	walk_down
	walk_down
	step_end

@ ===========================================================================
@ TEXT - ENDING 1 (STOP)
@ ===========================================================================
ConvergenceKillSwitch_Text_StopDorneActivates:
	.string "Dorne is already at the mechanism, one\n"
	.string "hand resting on the ancient switch.\p"
	.string "He doesn't look back. He just waits for\n"
	.string "the player to close the distance.$"

ConvergenceKillSwitch_Text_StopBattleIntro:
	.string "I'm going to throw this switch.\p"
	.string "If you mean to stop me, there's only\n"
	.string "one way left to do it.\p"
	.string "Come on, then. Like your parent would\n"
	.string "have.$"

ConvergenceKillSwitch_Text_StopBattleDefeat:
	.string "Enough.\p"
	.string "Enough.$"

ConvergenceKillSwitch_Text_StopPostBattle:
	.string "You fight like your parent.\p"
	.string "They would have done the same thing.$"

ConvergenceKillSwitch_Text_StopFleetWithdraws:
	.string "Dorne takes his hand off the switch and\n"
	.string "walks out without another word.\p"
	.string "Far offshore, the Covenant fleet turns\n"
	.string "and begins to withdraw. They got what\l"
	.string "they came for. The seals will hold.\p"
	.string "For now.$"

@ ===========================================================================
@ TEXT - ENDING 2 (HELP)
@ ===========================================================================
ConvergenceKillSwitch_Text_HelpApproach:
	.string "Dorne moves to the mechanism. He doesn't\n"
	.string "reach for it alone.\p"
	.string "He waits for the player to stand beside\n"
	.string "him.$"

ConvergenceKillSwitch_Text_HelpActivate:
	.string "Two hands on the switch. Together, they\n"
	.string "throw it.$"

ConvergenceKillSwitch_Text_HelpNetworkCollapses:
	.string "Across every island, the seals let go\n"
	.string "at once.\p"
	.string "The network that held the world steady\n"
	.string "for two thousand years comes apart in a\l"
	.string "single breath.$"

ConvergenceKillSwitch_Text_HelpDorneDone:
	.string "It's done.\p"
	.string "The Covenant can't use them now.$"

ConvergenceKillSwitch_Text_HelpCassFleet:
	.string "The Covenant fleet is moving. They're\n"
	.string "coming in.$"

ConvergenceKillSwitch_Text_HelpDorneLetThem:
	.string "Let them.\p"
	.string "There's nothing left to exploit.$"

ConvergenceKillSwitch_Text_HelpFleetArrives:
	.string "The fleet arrives - and finds nothing\n"
	.string "to take. No network to bleed. No power\l"
	.string "to siphon.\p"
	.string "One by one the ships drop anchor offshore\n"
	.string "and simply stop, purposeless in calm\l"
	.string "water.$"

ConvergenceKillSwitch_Text_HelpDorneDifferent:
	.string "I thought this would feel different.$"

ConvergenceKillSwitch_Text_HelpCassWrong:
	.string "Does it feel wrong?$"

ConvergenceKillSwitch_Text_HelpDorneQuieter:
	.string "No.\p"
	.string "Just quieter than I expected.$"

@ ===========================================================================
@ TEXT - ENDING 3 (TRUE)
@ ===========================================================================
ConvergenceKillSwitch_Text_TrueThirdFunction:
	.string "Up close, with every cipher known, the\n"
	.string "mechanism shows its secret.\p"
	.string "Beneath END and FREE, a third setting\n"
	.string "wakes under the player's hand: OPEN.\p"
	.string "Not a way to break the seals. A way to\n"
	.string "let them go.$"

ConvergenceKillSwitch_Text_TruePlayerExplains:
	.string "The player traces the third setting and\n"
	.string "shows Dorne what the Warden's child can\l"
	.string "read and he cannot.$"

ConvergenceKillSwitch_Text_TrueDorneReads:
	.string "Your parent was right.\p"
	.string "I was going to burn everything down when\n"
	.string "I could have just opened the door.$"

ConvergenceKillSwitch_Text_TrueWardensTouch:
	.string "The third setting answers to one thing\n"
	.string "only: a Warden's touch. Dorne cannot do\l"
	.string "this alone.\p"
	.string "The player lays a hand on the mechanism.\n"
	.string "It recognizes the keeping passed down,\l"
	.string "parent to child, and turns.$"

ConvergenceKillSwitch_Text_TrueNetworkDissolves:
	.string "The seal network doesn't collapse. It\n"
	.string "dissolves - quietly, evenly, like\l"
	.string "morning fog clearing off the water.\p"
	.string "Every legendary remains in the world.\n"
	.string "Present. Known. Beyond any hand that\l"
	.string "would own them.$"

ConvergenceKillSwitch_Text_TrueFleetLeaves:
	.string "Far offshore, the Covenant fleet watches.\p"
	.string "There is nothing here for them now - not\n"
	.string "broken, not stolen, simply gone beyond\l"
	.string "reach.\p"
	.string "One by one, the ships turn and leave.$"

@ ===========================================================================
@ TEXT - FLAVOR TALK
@ ===========================================================================
ConvergenceKillSwitch_Text_MechanismExamine:
	.string "The kill switch. Two thousand years old\n"
	.string "and still warm with waiting.\p"
	.string "Whatever happens next happens here.$"

ConvergenceKillSwitch_Text_DorneWaiting:
	.string "No more words between us. You know what\n"
	.string "I'm here to do.\p"
	.string "When you're ready, so am I.$"

ConvergenceKillSwitch_Text_CassWaiting:
	.string "I'm right here. Whatever you decide.\p"
	.string "I didn't come all this way to talk you\n"
	.string "out of it. I came so you wouldn't be\l"
	.string "alone at the end.$"
