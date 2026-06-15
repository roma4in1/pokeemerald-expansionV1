Convergence_Approach_MapScripts::
	.byte 0

@ ===========================================================================
@ ARRIVAL + THE GATHERING SCENE
@ Player lands at (11,11)/(12,11) by Storm Compass. Dorne waits at (11,9).
@ Cass at (13,10) if she defected. Sollis arrives by boat from the south.
@ Sets FLAG_CONVERGENCE_ARRIVED, FLAG_CONVERGENCE_GATHERING_SEEN,
@ FLAG_SOLLIS_CONFESSION_HEARD; gives ITEM_WARDENS_RESEARCH; progress 0 -> 1.
@ ===========================================================================
ConvergenceApproach_EventScript_Gathering::
	lockall
	setflag FLAG_CONVERGENCE_ARRIVED
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30

	@ Dorne is already here, waiting. He turns to the player.
	applymovement LOCALID_CONVERGENCE_DORNE, Common_Movement_FaceDown
	waitmovement 0
	delay 20
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneYouMadeIt, MSGBOX_DEFAULT
	closemessage

	@ Player nods - turn-in-place beat as a nod.
	applymovement LOCALID_PLAYER, ConvergenceApproach_Movement_Nod
	waitmovement 0
	delay 10
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneGood, MSGBOX_DEFAULT
	closemessage

	@ Cass, if she defected, steps in alongside.
	goto_if_set FLAG_CASS_DEFECTED, ConvergenceApproach_EventScript_GatheringCass
	goto ConvergenceApproach_EventScript_GatheringBoat

ConvergenceApproach_EventScript_GatheringCass::
	applymovement LOCALID_CONVERGENCE_CASS, ConvergenceApproach_Movement_CassStepIn
	waitmovement 0
	applymovement LOCALID_CONVERGENCE_CASS, Common_Movement_FaceDown
	waitmovement 0
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceApproach_Text_CassArrive, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceApproach_EventScript_GatheringBoat

ConvergenceApproach_EventScript_GatheringBoat::
	@ A boat arrives from the south. Sollis steps off.
	setspeaker 0
	msgbox ConvergenceApproach_Text_BoatArrives, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_CONVERGENCE_GATHERING_SEEN
	addobject LOCALID_CONVERGENCE_SOLLIS
	applymovement LOCALID_CONVERGENCE_SOLLIS, ConvergenceApproach_Movement_SollisStepOff
	waitmovement 0
	applymovement LOCALID_CONVERGENCE_SOLLIS, Common_Movement_FaceUp
	waitmovement 0

	@ MANDATORY long pause - Dorne and Sollis look at each other.
	delay 60

	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_SollisVael, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneMaren, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_SollisSooner, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneYes, MSGBOX_DEFAULT
	closemessage

	@ Sollis turns to the player. The confession.
	applymovement LOCALID_CONVERGENCE_SOLLIS, ConvergenceApproach_Movement_SollisFacePlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, ConvergenceApproach_Movement_PlayerFaceSollis
	waitmovement 0
	delay 20
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_SollisThingsToTell, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneAllOfIt, MSGBOX_DEFAULT
	closemessage

	@ Extended confession (verbatim spirit of the brief).
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_Confession1, MSGBOX_DEFAULT
	msgbox ConvergenceApproach_Text_Confession2, MSGBOX_DEFAULT
	msgbox ConvergenceApproach_Text_Confession3, MSGBOX_DEFAULT
	msgbox ConvergenceApproach_Text_Confession4, MSGBOX_DEFAULT
	closemessage

	@ Sollis hands over the Warden's complete research (full-bag-safe).
	setspeaker 0
	giveitem ITEM_WARDENS_RESEARCH
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_Confession5, MSGBOX_DEFAULT
	closemessage

	setspeaker 0
	msgbox ConvergenceApproach_Text_TowardTemple, MSGBOX_DEFAULT
	closemessage

	setflag FLAG_CONVERGENCE_GATHERING_SEEN
	setflag FLAG_SOLLIS_CONFESSION_HEARD
	setvar VAR_CONVERGENCE_PROGRESS, 1
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Talk scripts (after the gathering, these characters linger on the shore).
@ ---------------------------------------------------------------------------
ConvergenceApproach_EventScript_Dorne::
	lockall
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceApproach_Text_DorneLinger, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceApproach_EventScript_Sollis::
	lockall
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceApproach_Text_SollisLinger, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceApproach_EventScript_Cass::
	lockall
	goto_if_set FLAG_CASS_DEFECTED, ConvergenceApproach_EventScript_CassLinger
	releaseall
	end

ConvergenceApproach_EventScript_CassLinger::
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceApproach_Text_CassLinger, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Movements
@ ---------------------------------------------------------------------------
ConvergenceApproach_Movement_Nod:
	face_down
	delay_8
	face_up
	step_end

ConvergenceApproach_Movement_CassStepIn:
	walk_down
	walk_left
	step_end

ConvergenceApproach_Movement_SollisStepOff:
	walk_up
	walk_up
	step_end

ConvergenceApproach_Movement_SollisFacePlayer:
	face_up
	step_end

ConvergenceApproach_Movement_PlayerFaceSollis:
	face_down
	step_end

@ ===========================================================================
@ TEXT
@ ===========================================================================
ConvergenceApproach_Text_DorneYouMadeIt:
	.string "You made it.\p"
	.string "All of them?$"

ConvergenceApproach_Text_DorneGood:
	.string "Good.$"

ConvergenceApproach_Text_CassArrive:
	.string "I told you I'd be here for the end.\p"
	.string "Whatever the end turns out to be.$"

ConvergenceApproach_Text_BoatArrives:
	.string "A boat cut a slow line in from the\n"
	.string "south, riding low with the tide.\p"
	.string "It came to rest against the old stone,\n"
	.string "and a figure stepped down onto the\l"
	.string "shore of the place where it all began.\p"
	.string "Maren Sollis. Out from Haven Isle at\n"
	.string "last, carrying everything she never said.$"

ConvergenceApproach_Text_SollisVael:
	.string "Vael.$"

ConvergenceApproach_Text_DorneMaren:
	.string "Maren.$"

ConvergenceApproach_Text_SollisSooner:
	.string "I should have come sooner.$"

ConvergenceApproach_Text_DorneYes:
	.string "Yes.$"

ConvergenceApproach_Text_SollisThingsToTell:
	.string "There are things I should have told you\n"
	.string "from the beginning.\p"
	.string "I'm going to tell you now.\p"
	.string "All of it.$"

ConvergenceApproach_Text_DorneAllOfIt:
	.string "All of it.$"

ConvergenceApproach_Text_Confession1:
	.string "I knew about the kill switch from the\n"
	.string "start. Before you were born.\p"
	.string "Your parent and I argued about it for\n"
	.string "years. Whether it should exist. Whether\l"
	.string "anyone should ever be told it did.$"

ConvergenceApproach_Text_Confession2:
	.string "I agreed to keep it secret. Because\n"
	.string "your parent believed there was a third\l"
	.string "option buried in the old work - a way\l"
	.string "through that didn't burn anything down.$"

ConvergenceApproach_Text_Confession3:
	.string "And I didn't tell you. After they were\n"
	.string "gone, I told myself you weren't ready.\p"
	.string "The truth is I wasn't ready. To hand a\n"
	.string "child the thing that took their parent.$"

ConvergenceApproach_Text_Confession4:
	.string "I'm still not sure I was right to wait.\p"
	.string "But you're standing here, having done\n"
	.string "what no one thought could be done.\p"
	.string "So I'll stop deciding for you.$"

ConvergenceApproach_Text_Confession5:
	.string "This is everything. Your parent's\n"
	.string "complete research. The third option,\l"
	.string "fully documented.\p"
	.string "Whatever you choose in there - choose\n"
	.string "it knowing all of it.$"

ConvergenceApproach_Text_TowardTemple:
	.string "Ahead, past the broken colonnade, the\n"
	.string "temple waited. The others were already\l"
	.string "moving toward it.$"

ConvergenceApproach_Text_DorneLinger:
	.string "It's strange, standing where they\n"
	.string "stood. The people who built all this.\p"
	.string "They're a long time gone. The choice\n"
	.string "they left is still here.$"

ConvergenceApproach_Text_SollisLinger:
	.string "Go on when you're ready. I'll be right\n"
	.string "behind you.\p"
	.string "I've spent long enough behind you.$"

ConvergenceApproach_Text_CassLinger:
	.string "I keep waiting to feel scared.\p"
	.string "Mostly I just don't want to be anywhere\n"
	.string "else. Funny, that.$"
