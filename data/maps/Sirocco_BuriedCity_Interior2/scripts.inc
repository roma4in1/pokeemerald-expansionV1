Sirocco_BuriedCity_Interior2_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Central mural - the vision of Xerath and the sealing ceremony,
@ and the Warden's Journal cipher 3 of 9.
@ Numbering follows the implemented sequence: Haven ruins = cipher 1,
@ Ironhold apparatus = cipher 2, so the Sirocco mural = cipher 3
@ (FLAG_CIPHER_3_FOUND + FLAG_SIROCCO_CIPHER_FOUND, set together).
@ The vision is mostly visual: white flash, the seal's voice, then
@ narration. Re-examining gives a quiet revisit text.
@ -------------------------------------------------------------------
SiroccoBuriedCityInterior2_EventScript_CentralMural::
	lockall
	goto_if_set FLAG_SIROCCO_CIPHER_FOUND, SiroccoBuriedCityInterior2_EventScript_MuralRevisit
	msgbox SiroccoBuriedCityInterior2_Text_MuralApproach, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 30
	playmoncry SPECIES_REGIROCK, CRY_MODE_ENCOUNTER  @ placeholder cry for Xerath
	waitmoncry
	delay 30
	fadescreen FADE_FROM_WHITE
	msgbox SiroccoBuriedCityInterior2_Text_VisionXerath, MSGBOX_DEFAULT
	msgbox SiroccoBuriedCityInterior2_Text_VisionWorship, MSGBOX_DEFAULT
	closemessage
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 40
	fadescreen FADE_FROM_WHITE
	msgbox SiroccoBuriedCityInterior2_Text_VisionSealing, MSGBOX_DEFAULT
	msgbox SiroccoBuriedCityInterior2_Text_VisionEnds, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_CIPHER_3_FOUND
	setflag FLAG_SIROCCO_CIPHER_FOUND
	delay 20
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox SiroccoBuriedCityInterior2_Text_JournalUnlocks, MSGBOX_DEFAULT
	releaseall
	end

SiroccoBuriedCityInterior2_EventScript_MuralRevisit::
	msgbox SiroccoBuriedCityInterior2_Text_MuralRevisit, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Side murals - lore. Left: the city alive. Right: the drought and
@ the choice to seal. Flavor only; the cipher is the central mural.
@ -------------------------------------------------------------------
SiroccoBuriedCityInterior2_EventScript_WallMuralLeft::
	lockall
	msgbox SiroccoBuriedCityInterior2_Text_MuralLeft, MSGBOX_DEFAULT
	releaseall
	end

SiroccoBuriedCityInterior2_EventScript_WallMuralRight::
	lockall
	msgbox SiroccoBuriedCityInterior2_Text_MuralRight, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoBuriedCityInterior2_Text_MuralApproach:
	.string "The far wall is one vast mural,\n"
	.string "intact from floor to vault.\p"
	.string "The air here is so dry it pulls\n"
	.string "at your eyes. Every pigment looks\l"
	.string "freshly ground.\p"
	.string "At the mural's center, a single\n"
	.string "carved spiral - and as you lean\l"
	.string "close, it begins to glow.$"

SiroccoBuriedCityInterior2_Text_VisionXerath:
	.string "-water.\p"
	.string "Water in impossible volume,\n"
	.string "climbing from the rock in slow\l"
	.string "silver columns.\p"
	.string "And beneath it all, vast and\n"
	.string "patient, something made of stone\l"
	.string "and riverbed:\p"
	.string "XERATH. Awake. At full power.\n"
	.string "The desert green to every\l"
	.string "horizon.$"

SiroccoBuriedCityInterior2_Text_VisionWorship:
	.string "People in pale robes line the\n"
	.string "avenues - thousands of them -\l"
	.string "carrying bowls of water raised\l"
	.string "to the light.\p"
	.string "Not taking. Offering.\p"
	.string "They are giving the water BACK.$"

SiroccoBuriedCityInterior2_Text_VisionSealing:
	.string "Then the image turns.\p"
	.string "The same robed figures stand in\n"
	.string "a ring, heads bowed, while the\l"
	.string "great form sinks willingly into\l"
	.string "the stone.\p"
	.string "Nine points of light close over\n"
	.string "it like a hand closing.\p"
	.string "A ceremony. A promise.\n"
	.string "A seal.$"

SiroccoBuriedCityInterior2_Text_VisionEnds:
	.string "The chamber returns around you,\n"
	.string "dry and still.\p"
	.string "Your heart is pounding, and your\n"
	.string "mouth tastes of rain.$"

SiroccoBuriedCityInterior2_Text_JournalUnlocks:
	.string "In your pack, the WARDEN's\n"
	.string "JOURNAL grows warm.\p"
	.string "A coded page drinks in the\n"
	.string "mural's afterglow, and lines of\l"
	.string "your parent's handwriting come\l"
	.string "loose from the cipher:\p"
	.string "“…the BURIED CITY was not built\n"
	.string "as a settlement. It was built as\l"
	.string "a vault.\p"
	.string "The seal beneath it predates the\n"
	.string "civilization that built the city\l"
	.string "above it. They built the city to\l"
	.string "protect the seal.\p"
	.string "…DEX found a panel I missed,\n"
	.string "years ago. He doesn't know what\l"
	.string "it means yet.\p"
	.string "I do.”\p"
	.string "(The third cipher fragment has\n"
	.string "been recorded in the journal's\l"
	.string "margin!)$"

SiroccoBuriedCityInterior2_Text_MuralRevisit:
	.string "The spiral at the mural's center\n"
	.string "is dark now, its light spent.\p"
	.string "The painted water still seems to\n"
	.string "move when you stop looking\l"
	.string "straight at it.$"

SiroccoBuriedCityInterior2_Text_MuralLeft:
	.string "A city in full flower: gardens on\n"
	.string "the rooftops, canals down every\l"
	.string "avenue, fountains crowded with\l"
	.string "painted birds.\p"
	.string "Beneath the streets, rendered\n"
	.string "lovingly in blue and gold, an\l"
	.string "enormous sleeping shape -\p"
	.string "-and every canal in the city\n"
	.string "begins at its mouth.$"

SiroccoBuriedCityInterior2_Text_MuralRight:
	.string "The same city, late and dry. The\n"
	.string "gardens are brown. The painted\l"
	.string "birds are gone.\p"
	.string "Robed figures argue across a\n"
	.string "broken canal - and one panel\l"
	.string "shows a man alone, drinking,\l"
	.string "while a child watches with an\l"
	.string "empty bowl.\p"
	.string "Under it, a line of ancient\n"
	.string "script has been cut deeper than\l"
	.string "all the rest - rewritten, you\l"
	.string "think. Insisted upon.$"
