Primalis_VerdantLanding_MapScripts::
	.byte 0

@ ===================================================================
@ Arrival. Coord triggers at (9,12)/(10,12) fire at
@ VAR_PRIMALIS_PROGRESS == 0. Jungle sound, a wary Zoan guardian
@ comes down to size up the outsider. Sets FLAG_PRIMALIS_ARRIVED +
@ progress 1.
@ ===================================================================
PrimalisVerdantLanding_EventScript_Arrival::
	lockall
	delay 20
	playse SE_M_GRASSWHISTLE
	delay 30
	msgbox PrimalisVerdantLanding_Text_Arrival1, MSGBOX_DEFAULT
	closemessage
	@ The greeter approaches from the treeline.
	applymovement LOCALID_PRIMALIS_LANDING_GREETER, PrimalisVerdantLanding_Movement_GreeterApproach
	waitmovement 0
	delay 10
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Guardian
	msgbox PrimalisVerdantLanding_Text_GreeterChallenge, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 10
	setflag FLAG_PRIMALIS_ARRIVED
	setvar VAR_PRIMALIS_PROGRESS, 1
	@ The greeter steps back to her post.
	applymovement LOCALID_PRIMALIS_LANDING_GREETER, PrimalisVerdantLanding_Movement_GreeterReturn
	waitmovement 0
	releaseall
	end

PrimalisVerdantLanding_Movement_GreeterApproach:
	walk_down
	walk_down
	face_left
	step_end

PrimalisVerdantLanding_Movement_GreeterReturn:
	walk_up
	walk_up
	face_down
	step_end

@ ===================================================================
@ Greeter - the wary Zoan guardian who meets every boat. Part-plant
@ transformation, vine-knotted hair, slow to trust.
@ ===================================================================
PrimalisVerdantLanding_EventScript_Greeter::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Guardian
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisVerdantLanding_EventScript_GreeterResolved
	goto_if_set FLAG_PRIMALIS_LENS_MET, PrimalisVerdantLanding_EventScript_GreeterPostLens
	msgbox PrimalisVerdantLanding_Text_GreeterDefault, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLanding_EventScript_GreeterPostLens::
	msgbox PrimalisVerdantLanding_Text_GreeterPostLens, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLanding_EventScript_GreeterResolved::
	msgbox PrimalisVerdantLanding_Text_GreeterResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ Boat-boarding wiring (map-builder owns this; NOT dialogue).
PrimalisVerdantLanding_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_PRIMALIS
	goto Pelagios_EventScript_BoardTennyson
	end

@ ===================================================================
@ Text
@ ===================================================================
PrimalisVerdantLanding_Text_Arrival1:
	.string "The boat scrapes against a landing of\n"
	.string "living root.\p"
	.string "The air is thick and green and loud -\n"
	.string "birdcall, insect drone, something\l"
	.string "huge breathing somewhere far inland.\p"
	.string "This jungle is older than anything you\n"
	.string "have ever stood inside.$"

PrimalisVerdantLanding_Text_GreeterChallenge:
	.string "Stop there.\p"
	.string "Outsiders do not come to Primalis. Not\n"
	.string "in my lifetime. Not in my mother's.\p"
	.string "And yet here you are, stepping off a\n"
	.string "Galleon like the sea owes you a road.\p"
	.string "The village is north. Elder MAKO will\n"
	.string "not see you - he sees no one from\l"
	.string "outside.\p"
	.string "But the path is not mine to close. The\n"
	.string "jungle decides who it lets pass. Go,\l"
	.string "then. Let it look at you.$"

PrimalisVerdantLanding_Text_GreeterDefault:
	.string "There is an inn behind me. A traveler\n"
	.string "has been there two weeks, asking the\l"
	.string "kind of questions that make people\l"
	.string "nervous.\p"
	.string "Outsiders, asking about the Root. It is\n"
	.string "a strange season.$"

PrimalisVerdantLanding_Text_GreeterPostLens:
	.string "You spoke to the traveler in the inn.\p"
	.string "Good. If anyone outside understands\n"
	.string "what we guard here, let it be someone\l"
	.string "who at least wants to.$"

PrimalisVerdantLanding_Text_GreeterResolved:
	.string "The forest is quieter today. The kind\n"
	.string "of quiet that means it is resting, not\l"
	.string "the kind that means it is afraid.\p"
	.string "You did that. I watched the boat come\n"
	.string "in and I did not trust it. I was wrong\l"
	.string "to doubt the jungle. It knew you.$"
