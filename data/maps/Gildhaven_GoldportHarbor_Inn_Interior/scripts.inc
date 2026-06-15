Gildhaven_GoldportHarbor_Inn_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Innkeeper's assistant - upstairs, professionally warm.
@ ===================================================================
GildhavenGoldportHarborInnInterior_EventScript_Innkeeper2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenGoldportHarborInnInterior_EventScript_Innkeeper2Resolved
	msgbox GildhavenGoldportHarborInnInterior_Text_Innkeeper2, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenGoldportHarborInnInterior_EventScript_Innkeeper2Resolved::
	msgbox GildhavenGoldportHarborInnInterior_Text_Innkeeper2Resolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Traveler who has lost track of time.
@ ===================================================================
GildhavenGoldportHarborInnInterior_EventScript_Traveler::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenGoldportHarborInnInterior_EventScript_TravelerResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenGoldportHarborInnInterior_EventScript_TravelerCorrupt
	msgbox GildhavenGoldportHarborInnInterior_Text_Traveler, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenGoldportHarborInnInterior_EventScript_TravelerCorrupt::
	msgbox GildhavenGoldportHarborInnInterior_Text_TravelerCorrupt, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenGoldportHarborInnInterior_EventScript_TravelerResolved::
	msgbox GildhavenGoldportHarborInnInterior_Text_TravelerResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenGoldportHarborInnInterior_Text_Innkeeper2:
	.string "The rooms upstairs are our finest.\p"
	.string "Most guests book a single night.\n"
	.string "Most guests are still here.\p"
	.string "We do not mind. The rooms pay for\n"
	.string "themselves, you understand.$"

GildhavenGoldportHarborInnInterior_Text_Innkeeper2Resolved:
	.string "Three of our long-stay guests checked\n"
	.string "out this morning. All at once.\p"
	.string "They looked... lighter. Like people\n"
	.string "who had remembered an appointment.\p"
	.string "I am happy for them. I think.$"

GildhavenGoldportHarborInnInterior_Text_Traveler:
	.string "I have been here... three months?\n"
	.string "Four?\p"
	.string "The city has a way of keeping you.\n"
	.string "I should leave.\p"
	.string "I will leave. Tomorrow.$"

GildhavenGoldportHarborInnInterior_Text_TravelerCorrupt:
	.string "Leave? No, no. There is a deal\n"
	.string "closing next week. A big one.\p"
	.string "And another after that. There is\n"
	.string "always another after that.\p"
	.string "That is the beautiful part. It never\n"
	.string "has to end.$"

GildhavenGoldportHarborInnInterior_Text_TravelerResolved:
	.string "I packed my bags this morning and I\n"
	.string "could not tell you why I unpacked\l"
	.string "them in the first place.\p"
	.string "Four months. I lost four months to\n"
	.string "a feeling I cannot even find anymore.\p"
	.string "I am going home.$"
