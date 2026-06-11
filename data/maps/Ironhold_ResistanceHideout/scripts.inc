Ironhold_ResistanceHideout_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Ironhold_ResistanceHideout_OnFrame
	.byte 0

Ironhold_ResistanceHideout_OnFrame:
	map_script_2 VAR_TEMP_0, 0, IronholdResistanceHideout_EventScript_FirstEntry
	.2byte 0

@ -------------------------------------------------------------------
@ First-entry scene. VAR_TEMP_0 disarms the ON_FRAME for the session;
@ FLAG_IRONHOLD_RESISTANCE_MET makes it permanent. The leader comes
@ down the room, sizes the player up, and hands over the document
@ fragment - the first hard evidence of Covenant seal exploitation.
@ Sets FLAG_IRONHOLD_RESISTANCE_MET + FLAG_DOCUMENT_FRAGMENT_OBTAINED.
@ If the player somehow slips in before Gym 1, the cell stonewalls.
@ -------------------------------------------------------------------
IronholdResistanceHideout_EventScript_FirstEntry::
	setvar VAR_TEMP_0, 1
	goto_if_set FLAG_IRONHOLD_RESISTANCE_MET, IronholdResistanceHideout_EventScript_FirstEntryDone
	lockall
	goto_if_unset FLAG_IRONHOLD_GYM1_CLEAR, IronholdResistanceHideout_EventScript_TooEarly
	delay 20
	playse SE_PIN
	applymovement LOCALID_IRONHOLD_RESISTANCE_LEADER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_IRONHOLD_RESISTANCE_LEADER, IronholdResistanceHideout_Movement_LeaderApproach
	waitmovement 0
	msgbox IronholdResistanceHideout_Text_EntryChallenge, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox IronholdResistanceHideout_Text_EntryRecognized, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_DOCUMENT_FRAGMENT
	setflag FLAG_DOCUMENT_FRAGMENT_OBTAINED
	setflag FLAG_IRONHOLD_RESISTANCE_MET
	msgbox IronholdResistanceHideout_Text_EntryWarden, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IRONHOLD_RESISTANCE_LEADER, IronholdResistanceHideout_Movement_LeaderReturn
	waitmovement 0
	releaseall
	end

IronholdResistanceHideout_EventScript_TooEarly::
	msgbox IronholdResistanceHideout_Text_TooEarly, MSGBOX_DEFAULT
	releaseall
	end

IronholdResistanceHideout_EventScript_FirstEntryDone::
	end

IronholdResistanceHideout_Movement_LeaderApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

IronholdResistanceHideout_Movement_LeaderReturn:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

@ -------------------------------------------------------------------
@ The contact (resistance leader)
@ -------------------------------------------------------------------
IronholdResistanceHideout_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdResistanceHideout_EventScript_LeaderResolved
	goto_if_set FLAG_IRONHOLD_RESISTANCE_MET, IronholdResistanceHideout_EventScript_LeaderMet
	msgbox IronholdResistanceHideout_Text_LeaderWary, MSGBOX_DEFAULT
	release
	end

IronholdResistanceHideout_EventScript_LeaderMet::
	msgbox IronholdResistanceHideout_Text_LeaderMet, MSGBOX_DEFAULT
	release
	end

IronholdResistanceHideout_EventScript_LeaderResolved::
	msgbox IronholdResistanceHideout_Text_LeaderResolved, MSGBOX_DEFAULT
	release
	end

IronholdResistanceHideout_EventScript_MemberA::
	lock
	faceplayer
	msgbox IronholdResistanceHideout_Text_MemberA, MSGBOX_DEFAULT
	release
	end

IronholdResistanceHideout_EventScript_MemberB::
	lock
	faceplayer
	msgbox IronholdResistanceHideout_Text_MemberB, MSGBOX_DEFAULT
	release
	end

IronholdResistanceHideout_EventScript_MapWall::
	msgbox IronholdResistanceHideout_Text_MapWall, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdResistanceHideout_Text_TooEarly:
	.string "Three people turn at once. Nobody\n"
	.string "speaks.\p"
	.string "A young man steps in front of the\n"
	.string "papers on the table, casual as a\l"
	.string "shrug.\p"
	.string "“Wrong basement, friend.”\p"
	.string "They watch you, silent, until you\n"
	.string "go.$"

IronholdResistanceHideout_Text_EntryChallenge:
	.string "“Easy. EASY. How did you-”\p"
	.string "He stops an arm's length away.\n"
	.string "Whatever he was reaching for,\l"
	.string "he lets it go.\p"
	.string "“…Wait. I know your face.”$"

IronholdResistanceHideout_Text_EntryRecognized:
	.string "“The WARDEN's kid. Took PETRA's\n"
	.string "badge. The whole city's whispering\l"
	.string "about you - quietly, the way this\l"
	.string "city does everything.”\p"
	.string "“Then you should have this.\n"
	.string "We've been sitting on it too\l"
	.string "long, waiting for someone the\l"
	.string "Covenant couldn't just disappear.”$"

IronholdResistanceHideout_Text_EntryWarden:
	.string "“It's a page of a Covenant energy\n"
	.string "requisition. Half of it's redacted.\p"
	.string "The half that isn't says the\n"
	.string "summit installation draws its\l"
	.string "power from a “contained asset.””\p"
	.string "His voice drops.\p"
	.string "“Your parent was here. Months\n"
	.string "ago. They took a copy of this\l"
	.string "document.\p"
	.string "We never heard from them again.”$"

IronholdResistanceHideout_Text_LeaderWary:
	.string "“Wrong basement, friend.”$"

IronholdResistanceHideout_Text_LeaderMet:
	.string "“Read it again when you doubt\n"
	.string "yourself. “Contained asset.”\l"
	.string "That's how they write about a\l"
	.string "living thing.”\p"
	.string "“The summit fortress is where the\n"
	.string "cables run. Commander SEVER signs\l"
	.string "every requisition.\p"
	.string "Make of that what you will.”$"

IronholdResistanceHideout_Text_LeaderResolved:
	.string "“The siphon's dead. We heard it\n"
	.string "from three sources before noon.”\p"
	.string "He almost smiles. It looks out of\n"
	.string "practice.\p"
	.string "“Your parent started this file.\n"
	.string "Feels right that their kid is the\l"
	.string "one closing it.\p"
	.string "It's not the only file, {PLAYER}.\n"
	.string "Remember that.”$"

IronholdResistanceHideout_Text_MemberA:
	.string "“I had a forge job. Good one.\n"
	.string "Asked one question about where\l"
	.string "the shipments went.\p"
	.string "One question.”\p"
	.string "He turns a radio dial that isn't\n"
	.string "on.\p"
	.string "“Now I live in a basement.”$"

IronholdResistanceHideout_Text_MemberB:
	.string "“Scared? Of course I'm scared.\n"
	.string "Everyone on this island is\l"
	.string "scared.\p"
	.string "The trick is being scared and\n"
	.string "useful at the same time.”$"

IronholdResistanceHideout_Text_MapWall:
	.string "A hand-drawn map of IRONHOLD\n"
	.string "covers the wall.\p"
	.string "Red thread runs from the armory,\n"
	.string "the port, and the garrison HQ -\l"
	.string "all of it converging on the\l"
	.string "summit fortress.\p"
	.string "Someone has circled the summit\n"
	.string "and written: “WHAT DOES IT EAT?”$"
