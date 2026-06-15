Primalis_JungleInterior_MapScripts::
	.byte 0

@ ===================================================================
@ Zoan Guardian Vex (TRAINER_ZOAN_PRIMALIS_2) - sight trainer.
@ ===================================================================
PrimalisJungleInterior_EventScript_Zoan2::
	trainerbattle_single TRAINER_ZOAN_PRIMALIS_2, PrimalisJungleInterior_Text_VexIntro, PrimalisJungleInterior_Text_VexDefeat, PrimalisJungleInterior_EventScript_Zoan2Post
	setspeaker Pelagios_Speaker_Vex
	msgbox PrimalisJungleInterior_Text_VexPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleInterior_EventScript_Zoan2Post::
	setspeaker Pelagios_Speaker_Vex
	msgbox PrimalisJungleInterior_Text_VexPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ River sign - the river that runs through the deep jungle.
@ ===================================================================
PrimalisJungleInterior_EventScript_RiverSign::
	lockall
	msgbox PrimalisJungleInterior_Text_RiverSign, MSGBOX_DEFAULT
	releaseall
	end

PrimalisJungleInterior_Text_VexIntro:
	.string "The jungle tests everyone who enters.\n"
	.string "I am part of the test.$"

PrimalisJungleInterior_Text_VexDefeat:
	.string "Hah. So you are.$"

PrimalisJungleInterior_Text_VexPost:
	.string "You passed this part.$"

PrimalisJungleInterior_Text_RiverSign:
	.string "Clear water runs down out of the\n"
	.string "highlands, threading between roots\l"
	.string "older than the river itself.\p"
	.string "The forest grew around the water. The\n"
	.string "water grew around the Root.$"
