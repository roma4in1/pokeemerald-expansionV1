Ashenveil_Greyport_Outpost_MapScripts::
	.byte 0

@ Orin's research notes - the arithmetic that the official record refuses to do.
AshenveilGreyportOutpost_EventScript_ResearchNotes::
	lockall
	msgbox AshenveilGreyportOutpost_Text_ResearchNotes, MSGBOX_DEFAULT
	releaseall
	end

@ The lantern handoff. Orin's voice. Includes the Cass detail ("someone else
@ was here two days ago"). Full-bag-safe: key item + flags first, never abort.
AshenveilGreyportOutpost_EventScript_LanternShelf::
	lockall
	goto_if_set FLAG_PHANTOM_LANTERN_OBTAINED, AshenveilGreyportOutpost_EventScript_LanternTaken
	setspeaker Pelagios_Speaker_Orin
	msgbox AshenveilGreyportOutpost_Text_LanternGive, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	giveitem ITEM_PHANTOM_LANTERN
	setflag FLAG_PHANTOM_LANTERN_OBTAINED
	setflag FLAG_ASHENVEIL_OUTPOST_MET
	setvar VAR_ASHENVEIL_PROGRESS, 2
	setspeaker Pelagios_Speaker_Orin
	msgbox AshenveilGreyportOutpost_Text_LanternCassDetail, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

AshenveilGreyportOutpost_EventScript_LanternTaken::
	setspeaker Pelagios_Speaker_Orin
	msgbox AshenveilGreyportOutpost_Text_LanternTaken, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

AshenveilGreyportOutpost_Text_ResearchNotes::
	.string "Orin's notes are pinned in neat rows,\n"
	.string "the same lines worked over and over.\p"
	.string "847 residents. Evacuation capacity:\n"
	.string "400. Recorded survivors: 401.\p"
	.string "Unaccounted for: 446.\p"
	.string "Below that, underlined twice: NO\n"
	.string "FOLLOW-UP REPORT WAS EVER FILED.$"

AshenveilGreyportOutpost_Text_LanternGive::
	.string "Take it. Phantom Lantern.\p"
	.string "The ash out there isn't just ash.\n"
	.string "It hangs in the air thick enough to\l"
	.string "hide a whole street from you.\p"
	.string "The lantern shows the path through.\n"
	.string "It's the only way across the fields.$"

AshenveilGreyportOutpost_Text_LanternCassDetail::
	.string "One more thing, since you're going\n"
	.string "out there anyway.\p"
	.string "Someone else was here two days ago.\n"
	.string "Young. Covenant uniform.\p"
	.string "Took something from the memorial\n"
	.string "area. Left something in the grove.\p"
	.string "Didn't speak to me. Just worked and\n"
	.string "left.\p"
	.string "They looked troubled.$"

AshenveilGreyportOutpost_Text_LanternTaken::
	.string "You've got the lantern. The fields\n"
	.string "are north. Mind the path.$"
