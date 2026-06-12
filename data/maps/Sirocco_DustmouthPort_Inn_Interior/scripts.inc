Sirocco_DustmouthPort_Inn_Interior_MapScripts::
	.byte 0

@ Traveler A - the oasis was twice this size, measured in generations
SiroccoDustmouthPortInnInterior_EventScript_TravelerA::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	msgbox SiroccoDustmouthPortInnInterior_Text_TravelerA, MSGBOX_DEFAULT
	release
	end

@ Traveler B - something under the sand near the old ruins
SiroccoDustmouthPortInnInterior_EventScript_TravelerB::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	msgbox SiroccoDustmouthPortInnInterior_Text_TravelerB, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPortInnInterior_Text_TravelerA:
	.string "My grandmother described the oasis\n"
	.string "as a lake.\p"
	.string "My mother called it a pond.\p"
	.string "I call it a puddle.\p"
	.string "My daughter - if I have one -\n"
	.string "what will she call it?$"

SiroccoDustmouthPortInnInterior_Text_TravelerB:
	.string "Whatever's under those ruins,\n"
	.string "DAGAN found it before the\l"
	.string "archaeologists did.\p"
	.string "That's the only explanation for\n"
	.string "why he's so confident the water\l"
	.string "won't matter soon.$"
