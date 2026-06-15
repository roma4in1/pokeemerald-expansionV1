Primalis_VerdantLanding_Inn_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Lodger - a guardian off-duty, resting in the inn.
@ ===================================================================
PrimalisVerdantLandingInnInterior_EventScript_Lodger::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Villager
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisVerdantLandingInnInterior_EventScript_LodgerResolved
	msgbox PrimalisVerdantLandingInnInterior_Text_Lodger, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLandingInnInterior_EventScript_LodgerResolved::
	msgbox PrimalisVerdantLandingInnInterior_Text_LodgerResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLandingInnInterior_Text_Lodger:
	.string "I came off the route rotation a week\n"
	.string "ago. Could not sleep out there anymore.\p"
	.string "The forest used to feel like a friend\n"
	.string "at my back. Now it feels like something\l"
	.string "with a fever. Restless. Hot.\p"
	.string "We are guardians. We are supposed to\n"
	.string "know what to do. None of us do.$"

PrimalisVerdantLandingInnInterior_Text_LodgerResolved:
	.string "I slept through the night and woke up\n"
	.string "crying and could not tell you why.\p"
	.string "The forest is at my back again. A\n"
	.string "friend again. I had stopped believing\l"
	.string "it would be.$"
