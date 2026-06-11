HavenIsle_AncientRuinsInterior2_MapScripts::
	.byte 0

AncientRuinsInterior2_EventScript_Inscription::
	lockall
	msgbox AncientRuinsInterior2_Text_Inscription, MSGBOX_DEFAULT
	goto_if_set FLAG_CIPHER_1_FOUND, AncientRuinsInterior2_EventScript_Done
	setflag FLAG_CIPHER_1_FOUND
	msgbox AncientRuinsInterior2_Text_CipherNoted, MSGBOX_DEFAULT
AncientRuinsInterior2_EventScript_Done::
	releaseall
	end

AncientRuinsInterior2_Text_Inscription:
	.string "The far wall is covered in carved\n"
	.string "script, line after line, older than\l"
	.string "any alphabet you know.\p"
	.string "But someone has translated one\n"
	.string "passage, scratched beneath it in\l"
	.string "a newer hand:\p"
	.string "“NINE LOCKS FOR ONE DOOR.\n"
	.string "NINE ISLANDS FOR ONE SLEEPER.\p"
	.string "WHILE ONE LOCK HOLDS, IT DREAMS.\n"
	.string "WARDENS, COUNT YOUR KEYS.”$"

AncientRuinsInterior2_Text_CipherNoted:
	.string "…The newer handwriting is one you\n"
	.string "have seen before.\p"
	.string "It's the same hand that wrote the\n"
	.string "coded pages of the WARDEN's journal.\p"
	.string "You copy the symbols down carefully.\p"
	.string "(The first cipher fragment has been\n"
	.string "recorded in the journal's margin!)$"
