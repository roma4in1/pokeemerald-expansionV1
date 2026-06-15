Gildhaven_NobleQuarter_VaneManor_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Butler - guards the study. Before manor access (FLAG_GILDHAVEN_MANOR_ACCESS,
@ set by Lace's Gym 3 victory) he turns the player away from the study;
@ after, he stands aside (Lace's authority overrides his).
@ ===================================================================
GildhavenVaneManorInterior_EventScript_Butler::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Butler
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenVaneManorInterior_EventScript_ButlerResolved
	goto_if_set FLAG_GILDHAVEN_MANOR_ACCESS, GildhavenVaneManorInterior_EventScript_ButlerAccess
	msgbox GildhavenVaneManorInterior_Text_ButlerNoAccess, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenVaneManorInterior_EventScript_ButlerAccess::
	msgbox GildhavenVaneManorInterior_Text_ButlerAccess, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenVaneManorInterior_EventScript_ButlerResolved::
	msgbox GildhavenVaneManorInterior_Text_ButlerResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Covenant documents in the study - Serel has been in contact with the
@ Covenant about Mirath for years. Lore reveal (no flag; the big reveal
@ is the yield map in the Exchange).
@ ===================================================================
GildhavenVaneManorInterior_EventScript_CovenantDocuments::
	lockall
	msgbox GildhavenVaneManorInterior_Text_CovenantDocuments, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Hidden passage - the study floor opens onto a stair leading down,
@ toward the Exchange. The warp tile itself sits beside this sign;
@ before access the passage reads as a sealed seam.
@ ===================================================================
GildhavenVaneManorInterior_EventScript_HiddenPassage::
	lockall
	goto_if_set FLAG_GILDHAVEN_MANOR_ACCESS, GildhavenVaneManorInterior_EventScript_HiddenPassageOpen
	msgbox GildhavenVaneManorInterior_Text_HiddenPassageSealed, MSGBOX_SIGN
	releaseall
	end

GildhavenVaneManorInterior_EventScript_HiddenPassageOpen::
	msgbox GildhavenVaneManorInterior_Text_HiddenPassageOpen, MSGBOX_SIGN
	releaseall
	end

GildhavenVaneManorInterior_Text_ButlerNoAccess:
	.string "The study is the master's private\n"
	.string "office. Strictly off-limits.\p"
	.string "I do not care whose guest you are.\n"
	.string "Some doors do not open for charm.$"

GildhavenVaneManorInterior_Text_ButlerAccess:
	.string "...Miss LACE gave you the key herself.\p"
	.string "I have served this house since before\n"
	.string "she was born. If she has decided, then\l"
	.string "it is decided.\p"
	.string "The passage is behind the desk. Go.\n"
	.string "I will not stop you. I am not sure I\l"
	.string "want to.$"

GildhavenVaneManorInterior_Text_ButlerResolved:
	.string "The master is not coming back, is he.\p"
	.string "I keep dusting the study anyway. Old\n"
	.string "habit. The passage stands open now.\p"
	.string "Let it. There is nothing down there\n"
	.string "worth locking anymore.$"

GildhavenVaneManorInterior_Text_CovenantDocuments:
	.string "Covenant letterhead. Years of it,\n"
	.string "filed neatly.\p"
	.string "Energy assessments. Extraction\n"
	.string "schedules. Polite language wrapped\l"
	.string "around something that is not polite\l"
	.string "at all.\p"
	.string "SEREL VANE has been talking to the\n"
	.string "COVENANT about MIRATH for a very long\l"
	.string "time.$"

GildhavenVaneManorInterior_Text_HiddenPassageSealed:
	.string "A seam in the study floor, almost\n"
	.string "invisible.\p"
	.string "It does not open. Not without knowing\n"
	.string "exactly where to press - and you do\l"
	.string "not.$"

GildhavenVaneManorInterior_Text_HiddenPassageOpen:
	.string "The seam in the floor has been\n"
	.string "opened. A narrow stair drops into\l"
	.string "cold air and older stone.\p"
	.string "It leads down, under the city, toward\n"
	.string "the EXCHANGE.$"
