Convergence_AncientCapital_Interior1_MapScripts::
	.byte 0

@ ===========================================================================
@ The temple's outer chamber. Eight murals tell the full history of the
@ ancient civilization, readable only now that all ciphers are known. If
@ FLAG_TRUE_ENDING_UNLOCKED is set, each mural appends a hidden section.
@ Dorne stands at the inner door (10,7); the confrontation branches on the
@ Ashenveil choice FLAG_DORNE_CHOICE_*.
@ ===========================================================================

@ ---------------------------------------------------------------------------
@ DORNE CONFRONTATION
@ STOP  -> tense; he intends to throw the switch; player must pass him.
@ HELP  -> resolved; they go in together.
@ DEFER -> he hands the choice back to the player NOW, here, before the door.
@ All paths advance to the inner sanctum (the player warps up). The actual
@ ending is selected/confirmed in Interior2; this sets the tone.
@ ---------------------------------------------------------------------------
ConvergenceInterior1_EventScript_Dorne::
	lockall
	applymovement LOCALID_CONVERGENCE_I1_DORNE, Common_Movement_FaceDown
	waitmovement 0
	goto_if_set FLAG_DORNE_CHOICE_HELP, ConvergenceInterior1_EventScript_DorneHelp
	goto_if_set FLAG_DORNE_CHOICE_DEFER, ConvergenceInterior1_EventScript_DorneDefer
	goto ConvergenceInterior1_EventScript_DorneStop

ConvergenceInterior1_EventScript_DorneStop::
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DorneStop, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_DorneHelp::
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DorneHelp, MSGBOX_DEFAULT
	releaseall
	end

@ DEFER: Dorne returns the choice to the player here.
ConvergenceInterior1_EventScript_DorneDefer::
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DorneDefer, MSGBOX_DEFAULT
	setspeaker 0
	@ Re-present the choice. The TRUE option is offered ONLY if unlocked.
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_DeferChoiceTrue
	goto ConvergenceInterior1_EventScript_DeferChoiceTwo

@ Two-way choice (no true ending available).
ConvergenceInterior1_EventScript_DeferChoiceTwo::
	msgbox ConvergenceInterior1_Text_DeferStopPrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ConvergenceInterior1_EventScript_DeferPickStop
	goto ConvergenceInterior1_EventScript_DeferPickHelp

@ Three-way choice (true ending available). STOP? then HELP? then TRUE.
ConvergenceInterior1_EventScript_DeferChoiceTrue::
	msgbox ConvergenceInterior1_Text_DeferStopPrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ConvergenceInterior1_EventScript_DeferPickStop
	msgbox ConvergenceInterior1_Text_DeferHelpPrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ConvergenceInterior1_EventScript_DeferPickHelp
	msgbox ConvergenceInterior1_Text_DeferTruePrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ConvergenceInterior1_EventScript_DeferPickTrue
	@ Declined all -> default to STOP (the Covenant-safe answer; never strands).
	goto ConvergenceInterior1_EventScript_DeferPickStop

ConvergenceInterior1_EventScript_DeferPickStop::
	clearflag FLAG_DORNE_CHOICE_DEFER
	setflag FLAG_DORNE_CHOICE_STOP
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DeferResultStop, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_DeferPickHelp::
	clearflag FLAG_DORNE_CHOICE_DEFER
	setflag FLAG_DORNE_CHOICE_HELP
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DeferResultHelp, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_DeferPickTrue::
	clearflag FLAG_DORNE_CHOICE_DEFER
	@ The true path is selected by leaving STOP/HELP unset and relying on
	@ FLAG_TRUE_ENDING_UNLOCKED in Interior2 / the chamber. Mark HELP as the
	@ cooperative posture so Dorne walks in WITH the player.
	setflag FLAG_DORNE_CHOICE_HELP
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceInterior1_Text_DeferResultTrue, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ MURALS - each is narration (setspeaker 0). Order follows the chamber wall:
@ NW, N1, N2, NE across the north wall; W1/W2 down the west; E1/E2 down the
@ east. Together they tell the rise, the seal, and the warning of the ancient
@ civilization. With the true ending unlocked, each adds a hidden line.
@ ---------------------------------------------------------------------------
ConvergenceInterior1_EventScript_MuralNW::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralNW, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralNWHidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralNWHidden::
	msgbox ConvergenceInterior1_Text_MuralNWHidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralN1::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralN1, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralN1Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralN1Hidden::
	msgbox ConvergenceInterior1_Text_MuralN1Hidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralN2::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralN2, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralN2Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralN2Hidden::
	msgbox ConvergenceInterior1_Text_MuralN2Hidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralNE::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralNE, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralNEHidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralNEHidden::
	msgbox ConvergenceInterior1_Text_MuralNEHidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralW1::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralW1, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralW1Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralW1Hidden::
	msgbox ConvergenceInterior1_Text_MuralW1Hidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralW2::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralW2, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralW2Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralW2Hidden::
	msgbox ConvergenceInterior1_Text_MuralW2Hidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralE1::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralE1, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralE1Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralE1Hidden::
	msgbox ConvergenceInterior1_Text_MuralE1Hidden, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceInterior1_EventScript_MuralE2::
	lockall
	setspeaker 0
	msgbox ConvergenceInterior1_Text_MuralE2, MSGBOX_DEFAULT
	goto_if_set FLAG_TRUE_ENDING_UNLOCKED, ConvergenceInterior1_EventScript_MuralE2Hidden
	releaseall
	end
ConvergenceInterior1_EventScript_MuralE2Hidden::
	msgbox ConvergenceInterior1_Text_MuralE2Hidden, MSGBOX_DEFAULT
	releaseall
	end

@ ===========================================================================
@ TEXT - DORNE
@ ===========================================================================
ConvergenceInterior1_Text_DorneStop:
	.string "So you came to stop me. Good. I'd think\n"
	.string "less of you if you'd changed your mind.\p"
	.string "The switch is through that door. I'm\n"
	.string "going to reach it. You're going to try\l"
	.string "to be standing in the way when I do.\p"
	.string "Let's not pretend either of us is going\n"
	.string "to blink.$"

ConvergenceInterior1_Text_DorneHelp:
	.string "You're still with me. After everything\n"
	.string "you've seen.\p"
	.string "Then we finish it together. Through the\n"
	.string "door. No more arguing in front of the\l"
	.string "dead.$"

ConvergenceInterior1_Text_DorneDefer:
	.string "On Ashenveil you told me you needed\n"
	.string "time. You've had it.\p"
	.string "I'm not going to choose for you. Your\n"
	.string "parent never would have let me.\p"
	.string "So. Standing here, at the door, knowing\n"
	.string "everything you know now - what do we\l"
	.string "do?$"

ConvergenceInterior1_Text_DeferStopPrompt:
	.string "Stop Dorne, and keep the seals as they\n"
	.string "are?$"

ConvergenceInterior1_Text_DeferHelpPrompt:
	.string "Help Dorne destroy the seal network?$"

ConvergenceInterior1_Text_DeferTruePrompt:
	.string "Or try the third way - the one your\n"
	.string "parent found?$"

ConvergenceInterior1_Text_DeferResultStop:
	.string "Then I'll have to go through you.\p"
	.string "I expected nothing else from the\n"
	.string "Warden's child. Come on.$"

ConvergenceInterior1_Text_DeferResultHelp:
	.string "Then we end it. Together, through the\n"
	.string "door.\p"
	.string "I hope you can live with what comes\n"
	.string "next. I've made my peace with it.$"

ConvergenceInterior1_Text_DeferResultTrue:
	.string "The third way. Of course you'd reach\n"
	.string "for it.\p"
	.string "Show me, then. Show me what your parent\n"
	.string "found. Through the door.$"

@ ===========================================================================
@ TEXT - MURALS
@ ===========================================================================
ConvergenceInterior1_Text_MuralNW:
	.string "The first panel. A people rising from\n"
	.string "scattered islands into one nation.\p"
	.string "They learned to read the legendaries\n"
	.string "not as gods to be worshipped, but as\l"
	.string "forces woven through the living world.$"
ConvergenceInterior1_Text_MuralNWHidden:
	.string "Beneath the worn surface, a fainter\n"
	.string "line, only now legible:\p"
	.string "WE DID NOT FEAR THEM. THAT WAS OUR\n"
	.string "FIRST MISTAKE, AND OUR BEST ONE.$"

ConvergenceInterior1_Text_MuralN1:
	.string "The second panel. The great working.\p"
	.string "They drew the legendaries' power into a\n"
	.string "single network, anchored across every\l"
	.string "island - not to cage the legends, but\l"
	.string "to hold the world steady.$"
ConvergenceInterior1_Text_MuralN1Hidden:
	.string "Hidden in the border script:\p"
	.string "A SEAL IS NOT A PRISON. A SEAL IS A\n"
	.string "PROMISE THAT THE DOOR CAN BE OPENED\l"
	.string "AGAIN BY THE RIGHT HAND.$"

ConvergenceInterior1_Text_MuralN2:
	.string "The third panel. The golden age.\p"
	.string "Cities like this one, on islands that\n"
	.string "no longer exist. A civilization that\l"
	.string "believed it had solved the oldest\l"
	.string "problem: how to live beside power\l"
	.string "without being ruined by it.$"
ConvergenceInterior1_Text_MuralN2Hidden:
	.string "A scholar's marginal note, recovered:\p"
	.string "WE BUILT THE DOOR AND THEN ARGUED FOR A\n"
	.string "HUNDRED YEARS ABOUT WHETHER ANYONE\l"
	.string "SHOULD BE ALLOWED TO USE IT.$"

ConvergenceInterior1_Text_MuralNE:
	.string "The fourth panel. The first abuse.\p"
	.string "Someone tapped the network for power\n"
	.string "directly - bled the seals to light\l"
	.string "their lamps. The islands shuddered.\p"
	.string "An island died. The first dead place.\n"
	.string "The first Ashenveil.$"
ConvergenceInterior1_Text_MuralNEHidden:
	.string "The hidden section reads:\p"
	.string "THE NETWORK DID NOT BREAK. IT WAS\n"
	.string "MISUSED. THERE IS A DIFFERENCE, AND THE\l"
	.string "DIFFERENCE IS A CHOICE.$"

ConvergenceInterior1_Text_MuralW1:
	.string "The west wall. The reckoning.\p"
	.string "The builders saw what their own gift\n"
	.string "could become in the wrong hands. So\l"
	.string "they built a final safeguard into the\l"
	.string "foundation of this temple.$"
ConvergenceInterior1_Text_MuralW1Hidden:
	.string "Below it, a second mechanism is drawn -\n"
	.string "faint, deliberately half-buried.\p"
	.string "TWO HANDS ON THE SWITCH. ONE TO END.\n"
	.string "ONE TO OPEN. THEY ARE NOT THE SAME\l"
	.string "HAND.$"

ConvergenceInterior1_Text_MuralW2:
	.string "Lower still. The safeguard's purpose,\n"
	.string "stated plainly:\p"
	.string "IF THE POWER IS EVER TURNED TO RULE\n"
	.string "INSTEAD OF KEEP, LET THE NETWORK BE\l"
	.string "UNMADE, THAT NO ONE INHERIT WHAT WE\l"
	.string "COULD NOT BE TRUSTED WITH.$"
ConvergenceInterior1_Text_MuralW2Hidden:
	.string "The hidden line completes the thought:\p"
	.string "BUT UNMAKING IS THE COWARD'S MERCY.\n"
	.string "THE BRAVE ANSWER IS HARDER, AND IS\l"
	.string "WRITTEN WHERE ONLY THE PATIENT WILL\l"
	.string "LOOK.$"

ConvergenceInterior1_Text_MuralE1:
	.string "The east wall. The fall.\p"
	.string "The ancient nation did not destroy the\n"
	.string "network. They could not agree to. They\l"
	.string "fell to the same hunger they'd warned\l"
	.string "against, island by island, until only\l"
	.string "the seals remained.$"
ConvergenceInterior1_Text_MuralE1Hidden:
	.string "A single recovered name, set apart:\p"
	.string "THE LAST WARDENS KEPT THE SEALS WHEN\n"
	.string "THE BUILDERS WERE GONE. THEY PASSED THE\l"
	.string "KEEPING DOWN. HAND TO HAND. PARENT TO\l"
	.string "CHILD.$"

ConvergenceInterior1_Text_MuralE2:
	.string "The final panel is unfinished - or\n"
	.string "rather, left open.\p"
	.string "It shows the temple, the switch, and a\n"
	.string "figure at the mechanism whose face was\l"
	.string "never carved. Waiting for whoever would\l"
	.string "stand here at the end.$"
ConvergenceInterior1_Text_MuralE2Hidden:
	.string "With every cipher known, the open panel\n"
	.string "resolves. The faceless figure holds\l"
	.string "neither a torch nor a weapon.\p"
	.string "It holds a key. And the inscription\n"
	.string "beneath it, hidden until now, says only:\p"
	.string "LIBERATION. NOT DESTRUCTION. OPEN THE\n"
	.string "DOOR.$"
