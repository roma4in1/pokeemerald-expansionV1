Aetheron_CloudLanding_Inn_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Scholar - the Covenant came two years ago. Explains how Arc left the
@ village to watch from the inside.
@ ===================================================================
AetheronCloudLandingInnInterior_EventScript_Scholar::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Scholar
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronCloudLandingInnInterior_EventScript_ScholarResolved
	msgbox AetheronCloudLandingInnInterior_Text_Scholar, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCloudLandingInnInterior_EventScript_ScholarResolved::
	msgbox AetheronCloudLandingInnInterior_Text_ScholarResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronCloudLandingInnInterior_Text_Scholar:
	.string "The Covenant came two years ago. Said\n"
	.string "they needed access to the peak for\l"
	.string "research.\p"
	.string "We said no. They came anyway.\p"
	.string "That's when ARC left the village. He\n"
	.string "said he wanted to watch from the\l"
	.string "inside.\p"
	.string "He's still down there. Beating people\n"
	.string "in their own gym, of all things.$"

AetheronCloudLandingInnInterior_Text_ScholarResolved:
	.string "Arc came back up the route last night.\n"
	.string "First time in two years.\p"
	.string "He didn't say much. Just sat by the\n"
	.string "window a long while, watching the\l"
	.string "lightning ease.\p"
	.string "I think he's finally allowed to come\n"
	.string "home.$"
