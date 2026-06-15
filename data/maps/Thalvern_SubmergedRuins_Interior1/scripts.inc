Thalvern_SubmergedRuins_Interior1_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 2 - Psalm (Psychic). Scholar who stayed to study the visions.
@ Talk-initiated. Gated on Gym 1 (Tide). Badge NARRATIVE-ONLY. Gives
@ the Vision Badge then TM Psychic (exact, as briefed).
@ ===================================================================
ThalvernSubmergedRuinsInterior1_EventScript_Psalm::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Psalm
	goto_if_set FLAG_THALVERN_GYM2_CLEAR, ThalvernSubmergedRuinsInterior1_EventScript_PsalmPostBattle
	goto_if_unset FLAG_THALVERN_GYM1_CLEAR, ThalvernSubmergedRuinsInterior1_EventScript_PsalmLocked
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_THALVERN_PSALM, ThalvernSubmergedRuinsInterior1_Text_PsalmDefeat
	goto ThalvernSubmergedRuinsInterior1_EventScript_PsalmVictory

ThalvernSubmergedRuinsInterior1_EventScript_PsalmLocked::
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmLocked, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior1_EventScript_PsalmVictory::
	setspeaker Pelagios_Speaker_Psalm
	setflag FLAG_THALVERN_GYM2_CLEAR
	setvar VAR_THALVERN_PROGRESS, 3
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message ThalvernSubmergedRuinsInterior1_Text_ReceivedVisionBadge
	waitmessage
	waitfanfare
	closemessage
	giveitem ITEM_TM_PSYCHIC
	goto_if_eq VAR_RESULT, FALSE, ThalvernSubmergedRuinsInterior1_EventScript_PsalmBagFull
	setspeaker Pelagios_Speaker_Psalm
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior1_EventScript_PsalmBagFull::
	setspeaker Pelagios_Speaker_Psalm
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmBagFull, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior1_EventScript_PsalmPostBattle::
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernSubmergedRuinsInterior1_EventScript_PsalmResolved
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior1_EventScript_PsalmResolved::
	msgbox ThalvernSubmergedRuinsInterior1_Text_PsalmResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Puzzle sign - the platforms.
@ ===================================================================
ThalvernSubmergedRuinsInterior1_EventScript_PuzzleSign::
	lockall
	msgbox ThalvernSubmergedRuinsInterior1_Text_PuzzleSign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernSubmergedRuinsInterior1_Text_PsalmLocked:
	.string "You shouldn't be down here yet.\p"
	.string "TIDE guards the market for a\n"
	.string "reason. Earn his badge first.\p"
	.string "The deep doesn't reward the\n"
	.string "unready.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmPre:
	.string "I came to study the visions\n"
	.string "academically.\p"
	.string "Then I started having them myself.\p"
	.string "Now I'm not sure where the study\n"
	.string "ends and the experience begins.\p"
	.string "Battle me. It helps me stay\n"
	.string "present.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmDefeat:
	.string "There. I'm here. I'm now. Thank\n"
	.string "you.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmVictory:
	.string "Present and accounted for.\p"
	.string "Take the Vision Badge. You held my\n"
	.string "attention, and that's harder than\l"
	.string "you know lately.$"

ThalvernSubmergedRuinsInterior1_Text_ReceivedVisionBadge:
	.string "{PLAYER} received the Vision Badge.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmTM:
	.string "And TM Psychic. It's only fitting,\n"
	.string "down here.\p"
	.string "The Throne Room. That's what you\n"
	.string "want.\p"
	.string "The visions there are complete -\n"
	.string "not fragments. I went once. Came\l"
	.string "back changed.\p"
	.string "Take someone you trust. Don't go\n"
	.string "alone.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmBagFull:
	.string "Your bag's full. The TM will keep.\p"
	.string "The Throne Room is what you want.\n"
	.string "Don't go alone. Promise me that\l"
	.string "much.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmPost:
	.string "I keep reading the same inscription.\p"
	.string "It says the same thing every time,\n"
	.string "and every time I understand it\l"
	.string "less. Or more. I can't tell.$"

ThalvernSubmergedRuinsInterior1_Text_PsalmResolved:
	.string "The inscription is just stone now.\p"
	.string "No voice behind it. No pull.\p"
	.string "I think I can finally leave this\n"
	.string "chamber.\p"
	.string "I've been trying to for a year.$"

ThalvernSubmergedRuinsInterior1_Text_PuzzleSign:
	.string "The floor between the platforms\n"
	.string "floods and drains in slow turns.\p"
	.string "Cross when the stone is dry. Wait\n"
	.string "when it isn't.$"
