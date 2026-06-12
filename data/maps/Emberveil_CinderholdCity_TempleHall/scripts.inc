Emberveil_CinderholdCity_TempleHall_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Temple attendant in the foyer
@ -------------------------------------------------------------------
EmberveilTempleHall_EventScript_Attendant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Attendant
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilTempleHall_EventScript_AttendantResolved
	goto_if_set FLAG_EMBERVEIL_GYM2_CLEAR, EmberveilTempleHall_EventScript_AttendantSolaceGone
	msgbox EmberveilTempleHall_Text_Attendant, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHall_EventScript_AttendantSolaceGone::
	msgbox EmberveilTempleHall_Text_AttendantSolaceGone, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHall_EventScript_AttendantResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilTempleHall_EventScript_AttendantPathB
	msgbox EmberveilTempleHall_Text_AttendantPathA, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHall_EventScript_AttendantPathB::
	msgbox EmberveilTempleHall_Text_AttendantPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilTempleHall_Text_Attendant:
	.string "Welcome to the TEMPLE HALL.\p"
	.string "The HIGH EMBER is within, by the\n"
	.string "flame. She receives everyone -\l"
	.string "islander, visitor, skeptic.\p"
	.string "Especially skeptic, she says.\n"
	.string "They ask the best questions.$"

EmberveilTempleHall_Text_AttendantSolaceGone:
	.string "The HIGH EMBER has gone up the\n"
	.string "mountain to begin the great\l"
	.string "ritual.\p"
	.string "The hall feels larger without\n"
	.string "her. It shouldn't - she's a\l"
	.string "small woman. But it does.$"

EmberveilTempleHall_Text_AttendantPathA:
	.string "We keep the hall open. Fewer\n"
	.string "come, but it stays open.\p"
	.string "A door is a kind of promise.\n"
	.string "It's the one promise we're\l"
	.string "still sure of.$"

EmberveilTempleHall_Text_AttendantPathB:
	.string "The HIGH EMBER held the evening\n"
	.string "sitting herself yesterday.\p"
	.string "She told us what the flame had\n"
	.string "been all along. A lamp in a\l"
	.string "window. A kept watch.\p"
	.string "Nobody wanted to leave when it\n"
	.string "ended.$"
