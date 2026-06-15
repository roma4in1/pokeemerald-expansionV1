Thalvern_TidespirePort_Inn_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Innkeeper's husband upstairs - flavor.
@ ===================================================================
ThalvernTidespirePortInnInterior_EventScript_Innkeeper2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox ThalvernTidespirePortInnInterior_Text_Innkeeper2, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Scholar studying the phenomenon.
@ ===================================================================
ThalvernTidespirePortInnInterior_EventScript_Scholar::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Scholar
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernTidespirePortInnInterior_EventScript_ScholarResolved
	msgbox ThalvernTidespirePortInnInterior_Text_Scholar, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernTidespirePortInnInterior_EventScript_ScholarResolved::
	msgbox ThalvernTidespirePortInnInterior_Text_ScholarResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernTidespirePortInnInterior_Text_Innkeeper2:
	.string "My wife runs the desk. I just keep\n"
	.string "the lamps lit.\p"
	.string "She wakes up sad some mornings.\n"
	.string "Grief for people who died before\l"
	.string "her grandparents were born.\p"
	.string "Is that strange? I don't think it's\n"
	.string "strange.$"

ThalvernTidespirePortInnInterior_Text_Scholar:
	.string "The visions are fragments. They're\n"
	.string "not random - they're PELAGIOS\l"
	.string "broadcasting.\p"
	.string "It's trying to tell us something\n"
	.string "before the ruins sink completely.\p"
	.string "A message in a bottle. Except the\n"
	.string "bottle is a legendary Pokemon, and\l"
	.string "the ocean is time.$"

ThalvernTidespirePortInnInterior_Text_ScholarResolved:
	.string "The broadcast stopped. The seal's\n"
	.string "holding again.\p"
	.string "I should be relieved. Mostly I'm\n"
	.string "grieving that I'll never hear the\l"
	.string "rest of the message.$"
