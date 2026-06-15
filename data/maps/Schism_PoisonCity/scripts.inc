Schism_PoisonCity_MapScripts::
	.byte 0

@ ===================================================================
@ POISON CITY - the poison faction's grimy industrial settlement.
@ ===================================================================

@ -------------------------------------------------------------------
@ Elderly painter - paints the island as it was, whole.
@ -------------------------------------------------------------------
SchismPoisonCity_EventScript_Painter::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Painter
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismPoisonCity_EventScript_PainterCeasefire
	msgbox SchismPoisonCity_Text_Painter, MSGBOX_DEFAULT
	release
	end

SchismPoisonCity_EventScript_PainterCeasefire::
	msgbox SchismPoisonCity_Text_PainterCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Faction members.
@ -------------------------------------------------------------------
SchismPoisonCity_EventScript_FactionMember::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Citizen
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismPoisonCity_EventScript_FactionMemberCeasefire
	msgbox SchismPoisonCity_Text_FactionMember, MSGBOX_DEFAULT
	release
	end

SchismPoisonCity_EventScript_FactionMemberCeasefire::
	msgbox SchismPoisonCity_Text_FactionMemberCeasefire, MSGBOX_DEFAULT
	release
	end

SchismPoisonCity_EventScript_FactionMember2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Citizen
	goto_if_set FLAG_SCHISM_RESOLVED, SchismPoisonCity_EventScript_FactionMember2Resolved
	msgbox SchismPoisonCity_Text_FactionMember2, MSGBOX_DEFAULT
	release
	end

SchismPoisonCity_EventScript_FactionMember2Resolved::
	msgbox SchismPoisonCity_Text_FactionMember2Resolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SchismPoisonCity_EventScript_LabSign::
	msgbox SchismPoisonCity_Text_LabSign, MSGBOX_SIGN
	end

SchismPoisonCity_EventScript_CenterSign::
	msgbox SchismPoisonCity_Text_CenterSign, MSGBOX_SIGN
	end

SchismPoisonCity_EventScript_CaveSign::
	msgbox SchismPoisonCity_Text_CaveSign, MSGBOX_SIGN
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismPoisonCity_Text_Painter:
	.string "I paint it the way it looked\n"
	.string "before. Green and white together.\p"
	.string "Nobody buys these paintings.\p"
	.string "This one's the harbor. One\n"
	.string "harbor. One name.\p"
	.string "I'll paint until I can't anymore.$"

SchismPoisonCity_Text_PainterCeasefire:
	.string "Someone bought a painting today.\p"
	.string "Ice faction uniform. Young.\p"
	.string "He didn't say anything. Just paid\n"
	.string "and left.\p"
	.string "I think I'll paint faster now.$"

SchismPoisonCity_Text_FactionMember:
	.string "DRENN says the ice side poisoned\n"
	.string "our water on purpose.\p"
	.string "I believe him. He's never been\n"
	.string "wrong about anything that\l"
	.string "mattered.\p"
	.string "…He's been wrong about small\n"
	.string "things, though. Lately. More\l"
	.string "than he used to be.$"

SchismPoisonCity_Text_FactionMemberCeasefire:
	.string "DRENN stood in the Scar with the\n"
	.string "ice commander. Didn't kill her.\n"
	.string "Didn't get killed.\p"
	.string "Half of us think he's gone soft.\p"
	.string "The other half are quietly\n"
	.string "relieved we've stopped digging\l"
	.string "graves.$"

SchismPoisonCity_Text_FactionMember2:
	.string "Whole city smells of rust and\n"
	.string "rot. You get used to it.\p"
	.string "That's the trouble. You get used\n"
	.string "to anything if it lasts long\l"
	.string "enough.$"

SchismPoisonCity_Text_FactionMember2Resolved:
	.string "The water's clearing. Slowly.\p"
	.string "I caught myself breathing deep\n"
	.string "this morning, out of habit, and\l"
	.string "it didn't burn.\p"
	.string "First time in years I noticed\n"
	.string "the not-burning.$"

SchismPoisonCity_Text_LabSign:
	.string "DRENN LABORATORIES\p"
	.string "Research floor below. Office\n"
	.string "above. The VENOM LORD does not\l"
	.string "take appointments.$"

SchismPoisonCity_Text_CenterSign:
	.string "POISON CITY POKéMON CENTER\p"
	.string "Healing and antitoxins. Wash\n"
	.string "your hands on the way in.$"

SchismPoisonCity_Text_CaveSign:
	.string "RESTRICTED - LOWER WORKS\p"
	.string "The cave west leads down to the\n"
	.string "deep water. Do not go alone.$"
