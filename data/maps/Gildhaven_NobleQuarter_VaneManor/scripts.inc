Gildhaven_NobleQuarter_VaneManor_MapScripts::
	.byte 0

@ ===================================================================
@ Manor steward - manages the household; obsessed with the estate's
@ standing. Corruption-flavored throughout (this is the seal's epicentre).
@ ===================================================================
GildhavenVaneManor_EventScript_Steward::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Butler
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenVaneManor_EventScript_StewardResolved
	msgbox GildhavenVaneManor_Text_Steward, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenVaneManor_EventScript_StewardResolved::
	msgbox GildhavenVaneManor_Text_StewardResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Manor maid - quieter, sees more than she says.
@ ===================================================================
GildhavenVaneManor_EventScript_Maid::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Woman
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenVaneManor_EventScript_MaidResolved
	msgbox GildhavenVaneManor_Text_Maid, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenVaneManor_EventScript_MaidResolved::
	msgbox GildhavenVaneManor_Text_MaidResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Display case - wealth on display. Lore / atmosphere.
@ ===================================================================
GildhavenVaneManor_EventScript_DisplayCase::
	lockall
	msgbox GildhavenVaneManor_Text_DisplayCase, MSGBOX_SIGN
	releaseall
	end

GildhavenVaneManor_Text_Steward:
	.string "You are a guest of the young mistress,\n"
	.string "so I will permit it.\p"
	.string "Mr. VANE values his privacy. And his\n"
	.string "acquisitions. And the precise\l"
	.string "arrangement of both.\p"
	.string "Do not touch anything. Everything\n"
	.string "here has been valued. Twice.$"

GildhavenVaneManor_Text_StewardResolved:
	.string "The master has not come home.\p"
	.string "The valuations are all wrong now.\n"
	.string "Everything is worth less than it was\l"
	.string "yesterday, and I find I do not\l"
	.string "mind.\p"
	.string "I have worked here thirty years. I\n"
	.string "think I will go see my sister.$"

GildhavenVaneManor_Text_Maid:
	.string "The young mistress used to laugh in\n"
	.string "these halls. When she was small.\p"
	.string "Then the master changed, and she\n"
	.string "stopped, and the house got very\l"
	.string "quiet and very full at the same\l"
	.string "time.\p"
	.string "Full of things. Quiet of people.$"

GildhavenVaneManor_Text_MaidResolved:
	.string "Miss LACE smiled at me this morning.\n"
	.string "A real one.\p"
	.string "I had forgotten what this house\n"
	.string "sounded like with someone happy in\l"
	.string "it.$"

GildhavenVaneManor_Text_DisplayCase:
	.string "A display case of impossible objects -\n"
	.string "relics, gems, a sealed jar of nothing\l"
	.string "labeled at a staggering price.\p"
	.string "Each card lists a value. None list\n"
	.string "what the thing actually is.$"
