Ironhold_SummitFortress_Interior1_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 3 - Rook (Fighting/Steel), Sever's lieutenant, in the central
@ training hall. Victory: Garrison Badge (FLAG_BADGE04_GET),
@ FLAG_IRONHOLD_GYM3_CLEAR, VAR_IRONHOLD_PROGRESS = 4, TM Brick Break.
@ (Brief asks for a TM of Close Combat, which has no TM in this
@ project's vanilla TM set - Brick Break substituted.)
@ The brief's locked-door soldier puzzle is simplified to the two
@ stationed trainers below - the Victory Road layout has no doors.
@ -------------------------------------------------------------------
IronholdFortressInterior1_EventScript_Rook::
	goto_if_defeated TRAINER_LEADER_IRONHOLD_ROOK, IronholdFortressInterior1_EventScript_RookDefeated
	setspeaker Pelagios_Speaker_Rook
	trainerbattle_single TRAINER_LEADER_IRONHOLD_ROOK, IronholdFortressInterior1_Text_RookIntro, IronholdFortressInterior1_Text_RookDefeat, IronholdFortressInterior1_EventScript_RookVictory
	end

IronholdFortressInterior1_EventScript_RookVictory::
	setspeaker Pelagios_Speaker_Rook
	msgbox IronholdFortressInterior1_Text_RookPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message IronholdFortressInterior1_Text_ReceivedGarrisonBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE04_GET
	setflag FLAG_IRONHOLD_GYM3_CLEAR
	setvar VAR_IRONHOLD_PROGRESS, 4
	giveitem ITEM_TM_BRICK_BREAK
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Rook
	msgbox IronholdFortressInterior1_Text_RookExplainTM, MSGBOX_DEFAULT
	release
	end

IronholdFortressInterior1_EventScript_RookDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Rook
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdFortressInterior1_EventScript_RookResolved
	msgbox IronholdFortressInterior1_Text_RookPostBadge, MSGBOX_DEFAULT
	release
	end

IronholdFortressInterior1_EventScript_RookResolved::
	msgbox IronholdFortressInterior1_Text_RookResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Stationed soldiers (sight-initiated)
@ -------------------------------------------------------------------
IronholdFortressInterior1_EventScript_StationedA::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_7, IronholdFortressInterior1_Text_OstinIntro, IronholdFortressInterior1_Text_OstinDefeat
	setspeaker Pelagios_Speaker_Ostin
	msgbox IronholdFortressInterior1_Text_OstinPost, MSGBOX_AUTOCLOSE
	end

IronholdFortressInterior1_EventScript_StationedB::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_8, IronholdFortressInterior1_Text_WrennaIntro, IronholdFortressInterior1_Text_WrennaDefeat
	setspeaker Pelagios_Speaker_Wrenna
	msgbox IronholdFortressInterior1_Text_WrennaPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdFortressInterior1_Text_RookIntro:
	.string "Commander SEVER said to let you\n"
	.string "through if you beat me.\p"
	.string "I don't understand why.\p"
	.string "I'm going to follow orders\n"
	.string "anyway.$"

IronholdFortressInterior1_Text_RookDefeat:
	.string "…Orders are orders.$"

IronholdFortressInterior1_Text_RookPostBattle:
	.string "…He said you'd get this far.\p"
	.string "He said to tell you:\p"
	.string "“The report exists. It always\n"
	.string "has.”$"

IronholdFortressInterior1_Text_ReceivedGarrisonBadge:
	.string "{PLAYER} received the GARRISON\n"
	.string "BADGE from ROOK!$"

IronholdFortressInterior1_Text_RookExplainTM:
	.string "BRICK BREAK. Standard issue.\p"
	.string "I requisitioned CLOSE COMBAT for\n"
	.string "you. Denied. Everything is\l"
	.string "denied lately.\p"
	.string "The stairs to the command level\n"
	.string "are open.$"

IronholdFortressInterior1_Text_RookPostBadge:
	.string "The Commander is upstairs.\p"
	.string "Whatever he says to you up\n"
	.string "there… listen to all of it.\l"
	.string "Including the parts he doesn't\l"
	.string "say.$"

IronholdFortressInterior1_Text_RookResolved:
	.string "The Commander filed no report.\p"
	.string "Neither did I.\p"
	.string "…That's the first order he never\n"
	.string "gave that I followed anyway.$"

IronholdFortressInterior1_Text_OstinIntro:
	.string "Barracks floor. Civilians need a\n"
	.string "movement chit, a stamp, and a\l"
	.string "reason.\p"
	.string "You have none of the three.$"

IronholdFortressInterior1_Text_OstinDefeat:
	.string "…You had a reason.$"

IronholdFortressInterior1_Text_OstinPost:
	.string "The lieutenant drills in the\n"
	.string "central hall.\p"
	.string "He's not like the rest of us.\n"
	.string "He still believes the words on\l"
	.string "the recruitment posters.$"

IronholdFortressInterior1_Text_WrennaIntro:
	.string "Stop. This corridor is a\n"
	.string "restricted-\p"
	.string "…You're going to make me do this\n"
	.string "the loud way, aren't you.$"

IronholdFortressInterior1_Text_WrennaDefeat:
	.string "The loud way. Every time.$"

IronholdFortressInterior1_Text_WrennaPost:
	.string "You hear it down here too,\n"
	.string "right? Through the floor.\p"
	.string "That hum.\p"
	.string "They pay us not to hear it.$"
