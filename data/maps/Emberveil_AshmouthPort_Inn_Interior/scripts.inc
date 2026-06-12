Emberveil_AshmouthPort_Inn_Interior_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Traveler who came to see the volcano - awed
@ -------------------------------------------------------------------
EmberveilAshmouthPortInnInterior_EventScript_TravelerBeauty::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPortInnInterior_EventScript_TravelerBeautyResolved
	msgbox EmberveilAshmouthPortInnInterior_Text_TravelerBeauty, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPortInnInterior_EventScript_TravelerBeautyResolved::
	msgbox EmberveilAshmouthPortInnInterior_Text_TravelerBeautyResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Traveler who is leaving - the ash is getting to her
@ -------------------------------------------------------------------
EmberveilAshmouthPortInnInterior_EventScript_TravelerLeaving::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Traveler
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPortInnInterior_EventScript_TravelerLeavingResolved
	msgbox EmberveilAshmouthPortInnInterior_Text_TravelerLeaving, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPortInnInterior_EventScript_TravelerLeavingResolved::
	msgbox EmberveilAshmouthPortInnInterior_Text_TravelerLeavingResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilAshmouthPortInnInterior_Text_TravelerBeauty:
	.string "It's beautiful. Terrifying. Both\n"
	.string "at once.\p"
	.string "I understand why people worship\n"
	.string "it.\p"
	.string "I don't understand why they'd\n"
	.string "want to wake it up.$"

EmberveilAshmouthPortInnInterior_Text_TravelerBeautyResolved:
	.string "The smoke over the summit went\n"
	.string "thin yesterday. Thin and straight.\p"
	.string "It's still beautiful.\p"
	.string "It's just not holding its breath\n"
	.string "anymore. Neither am I.$"

EmberveilAshmouthPortInnInterior_Text_TravelerLeaving:
	.string "The ash is getting thicker. I'm\n"
	.string "leaving tomorrow.\p"
	.string "The cult says that's PYRATH's\n"
	.string "breath.\p"
	.string "I prefer my lungs less blessed.$"

EmberveilAshmouthPortInnInterior_Text_TravelerLeavingResolved:
	.string "I had my bag packed. Boat booked,\n"
	.string "the whole thing.\p"
	.string "Then the ash just… stopped.\p"
	.string "I unpacked. Don't tell anyone\n"
	.string "I said the mountain won.$"
