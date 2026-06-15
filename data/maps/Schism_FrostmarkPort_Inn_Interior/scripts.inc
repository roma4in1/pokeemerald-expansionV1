Schism_FrostmarkPort_Inn_Interior_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Traveler - came to study the island, found a hidden history.
@ -------------------------------------------------------------------
SchismFrostmarkPortInnInterior_EventScript_Traveler::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrostmarkPortInnInterior_EventScript_TravelerCeasefire
	msgbox SchismFrostmarkPortInnInterior_Text_Traveler, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPortInnInterior_EventScript_TravelerCeasefire::
	msgbox SchismFrostmarkPortInnInterior_Text_TravelerCeasefire, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPortInnInterior_Text_Traveler:
	.string "I found documents suggesting this\n"
	.string "island had one name once.\p"
	.string "Nobody here will tell me what it\n"
	.string "was.\p"
	.string "I think they know. I think\n"
	.string "remembering hurts.$"

SchismFrostmarkPortInnInterior_Text_TravelerCeasefire:
	.string "I asked EIRA's people about the\n"
	.string "old name again. Different answer\l"
	.string "this time.\p"
	.string "One of them said: “Ask us in a\n"
	.string "year. Maybe we'll be able to say\l"
	.string "it by then.”\p"
	.string "That's not a yes. But it isn't a\n"
	.string "door slammed, either.$"
