HavenIsle_CassHouse_MapScripts::
	.byte 0

HavenIsleCassHouse_EventScript_CassMother::
	lock
	faceplayer
	goto_if_set FLAG_STARTER_CHOSEN, HavenIsleCassHouse_EventScript_MotherAfter
	msgbox HavenIsleCassHouse_Text_MotherBefore, MSGBOX_DEFAULT
	release
	end

HavenIsleCassHouse_EventScript_MotherAfter::
	msgbox HavenIsleCassHouse_Text_MotherAfter, MSGBOX_DEFAULT
	release
	end

HavenIsleCassHouse_Text_MotherBefore:
	.string "Oh, {PLAYER}, hello!\p"
	.string "Cass shot out of here at dawn like\n"
	.string "a TAILLOW out of a chimney.\p"
	.string "Something about the professor and\n"
	.string "“destiny” and not finishing\l"
	.string "breakfast.\p"
	.string "If you find my child, make sure\n"
	.string "something gets eaten. Anything.$"

HavenIsleCassHouse_Text_MotherAfter:
	.string "A POKéMON of Cass's own… and one\n"
	.string "for you too, I hear.\p"
	.string "You two used to chart imaginary\n"
	.string "islands on this very floor.\p"
	.string "Paper boats, the whole fleet.\n"
	.string "I kept some of them, you know.\p"
	.string "…Look after each other out there.\n"
	.string "Paper boats sink.$"
