Schism_IceCity_Barracks_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ ICE BARRACKS INTERIOR - Gym 1, Sleet (Ice). The map reuses the SHOAL
@ ICE ROOM layout (real sliding-ice puzzle floor). Sleet stands at the
@ far end (3,2). Talk-initiated leader (TRAINER_TYPE_NONE).
@ ===================================================================

@ -------------------------------------------------------------------
@ Sleet - youngest gym leader. Gym 1 battle.
@ -------------------------------------------------------------------
SchismIceCityBarracksInterior_EventScript_Sleet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sleet
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismIceCityBarracksInterior_EventScript_SleetCeasefire
	goto_if_set FLAG_SCHISM_GYM1_CLEAR, SchismIceCityBarracksInterior_EventScript_SleetBeaten
	msgbox SchismIceCityBarracksInterior_Text_SleetPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_SCHISM_SLEET, SchismIceCityBarracksInterior_Text_SleetDefeat
	goto SchismIceCityBarracksInterior_EventScript_SleetVictory

SchismIceCityBarracksInterior_EventScript_SleetVictory::
	setspeaker Pelagios_Speaker_Sleet
	setflag FLAG_SCHISM_GYM1_CLEAR
	msgbox SchismIceCityBarracksInterior_Text_SleetVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message SchismIceCityBarracksInterior_Text_ReceivedFrostBadge
	waitmessage
	waitfanfare
	closemessage
	giveitem ITEM_TM_BLIZZARD
	goto_if_eq VAR_RESULT, FALSE, SchismIceCityBarracksInterior_EventScript_SleetBagFull
	setspeaker Pelagios_Speaker_Sleet
	msgbox SchismIceCityBarracksInterior_Text_SleetTM, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ progress 1 -> 2 (Sleet beaten), only if behind.
	goto_if_ge VAR_SCHISM_PROGRESS, 2, SchismIceCityBarracksInterior_EventScript_SleetDone
	setvar VAR_SCHISM_PROGRESS, 2
SchismIceCityBarracksInterior_EventScript_SleetDone::
	release
	end

SchismIceCityBarracksInterior_EventScript_SleetBagFull::
	setspeaker Pelagios_Speaker_Sleet
	msgbox SchismIceCityBarracksInterior_Text_SleetBagFull, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	goto_if_ge VAR_SCHISM_PROGRESS, 2, SchismIceCityBarracksInterior_EventScript_SleetDone
	setvar VAR_SCHISM_PROGRESS, 2
	release
	end

SchismIceCityBarracksInterior_EventScript_SleetBeaten::
	msgbox SchismIceCityBarracksInterior_Text_SleetBeaten, MSGBOX_DEFAULT
	release
	end

SchismIceCityBarracksInterior_EventScript_SleetCeasefire::
	msgbox SchismIceCityBarracksInterior_Text_SleetCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Puzzle sign.
@ -------------------------------------------------------------------
SchismIceCityBarracksInterior_EventScript_PuzzleSign::
	msgbox SchismIceCityBarracksInterior_Text_PuzzleSign, MSGBOX_SIGN
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismIceCityBarracksInterior_Text_SleetPre:
	.string "Young. Traveling alone. From the\n"
	.string "outside.\p"
	.string "Either very brave or very stupid.\p"
	.string "We'll find out which.$"

SchismIceCityBarracksInterior_Text_SleetDefeat:
	.string "Oh. Brave, then.$"

SchismIceCityBarracksInterior_Text_SleetVictory:
	.string "Brave then. EIRA will see you.\p"
	.string "Don't waste her time - she has\n"
	.string "very little patience for people\l"
	.string "who don't get to the point.$"

SchismIceCityBarracksInterior_Text_ReceivedFrostBadge:
	.string "{PLAYER} received the FROST\n"
	.string "BADGE.$"

SchismIceCityBarracksInterior_Text_SleetTM:
	.string "Here. BLIZZARD.\p"
	.string "It misses sometimes. So do I.\n"
	.string "Hit hard enough when it lands and\l"
	.string "nobody remembers the misses.$"

SchismIceCityBarracksInterior_Text_SleetBagFull:
	.string "Your pack's full.\p"
	.string "Come back for the TM when you've\n"
	.string "made room. I'm not going\l"
	.string "anywhere.$"

SchismIceCityBarracksInterior_Text_SleetBeaten:
	.string "EIRA's in the command room.\p"
	.string "Get to the point with her.\n"
	.string "Trust me on this.$"

SchismIceCityBarracksInterior_Text_SleetCeasefire:
	.string "I crossed the Scar yesterday.\p"
	.string "First one from our side in twenty\n"
	.string "years.\p"
	.string "I was scared the whole way. And\n"
	.string "then I wasn't.\p"
	.string "There's a kid on the poison side\n"
	.string "who collects shells too. We\l"
	.string "traded. Don't tell anyone yet.$"

SchismIceCityBarracksInterior_Text_PuzzleSign:
	.string "TRAINING FLOOR\p"
	.string "Find your footing on the ice, or\n"
	.string "the ice finds it for you.$"
