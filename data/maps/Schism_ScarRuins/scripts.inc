Schism_ScarRuins_MapScripts::
	.byte 0

@ ===================================================================
@ SCAR RUINS - the deeper, most intact ancient site so far. Murals of
@ Glacith and Toxara sealed together; the cipher-5 pedestal.
@ ===================================================================

@ -------------------------------------------------------------------
@ Glacith mural (7,7) - the ice counterpart. Lore; partial decode
@ improves once enough ciphers are collected.
@ -------------------------------------------------------------------
SchismScarRuins_EventScript_GlacithMural::
	lockall
	msgbox SchismScarRuins_Text_GlacithMural, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Toxara mural (12,7) - the poison counterpart.
@ -------------------------------------------------------------------
SchismScarRuins_EventScript_ToxaraMural::
	lockall
	msgbox SchismScarRuins_Text_ToxaraMural, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Cipher pedestal (9,11) - the central inscription. First examination
@ resolves cipher 5 in the Warden's Journal: sets FLAG_CIPHER_5_FOUND
@ + FLAG_SCHISM_CIPHER_FOUND. (Cipher order: Haven 1, Ironhold 2,
@ Sirocco 3, Emberveil 4, Schism 5.)
@ -------------------------------------------------------------------
SchismScarRuins_EventScript_CipherPedestal::
	lockall
	goto_if_set FLAG_SCHISM_CIPHER_FOUND, SchismScarRuins_EventScript_CipherAgain
	msgbox SchismScarRuins_Text_Inscription, MSGBOX_DEFAULT
	closemessage
	delay 20
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox SchismScarRuins_Text_JournalDecode, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_CIPHER_5_FOUND
	setflag FLAG_SCHISM_CIPHER_FOUND
	releaseall
	end

SchismScarRuins_EventScript_CipherAgain::
	msgbox SchismScarRuins_Text_InscriptionAgain, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismScarRuins_Text_GlacithMural:
	.string "A mural, cut deep into the stone\n"
	.string "and somehow untouched by the\l"
	.string "ages.\p"
	.string "A vast figure of ice and steel\n"
	.string "stands over a still, white\l"
	.string "landscape.\p"
	.string "Beneath it, a single line:\n"
	.string "“GLACITH preserves.”$"

SchismScarRuins_Text_ToxaraMural:
	.string "A second mural, the twin of the\n"
	.string "first.\p"
	.string "A sinuous, dragon-shaped figure\n"
	.string "coils over dark, churning water,\l"
	.string "and where it passes the water\l"
	.string "runs clear.\p"
	.string "Beneath it, a single line:\n"
	.string "“TOXARA cleanses.”$"

SchismScarRuins_Text_Inscription:
	.string "Between the two murals, a central\n"
	.string "inscription runs floor to\l"
	.string "ceiling.\p"
	.string "The same people who built both\n"
	.string "cities carved it. The two\l"
	.string "POKéMON are not enemies here.\l"
	.string "They are counterparts.\p"
	.string "With enough of the WARDEN's\n"
	.string "ciphers, the old script gives up\l"
	.string "its meaning:\p"
	.string "“…GLACITH preserves. TOXARA\n"
	.string "cleanses. Neither alone. Both\l"
	.string "together.\p"
	.string "The island breathes between\n"
	.string "them…”$"

SchismScarRuins_Text_JournalDecode:
	.string "In your pack, the WARDEN's\n"
	.string "JOURNAL grows cold, then warm.\p"
	.string "A sealed page resolves into your\n"
	.string "parent's hand:\p"
	.string "“The island had a name before\n"
	.string "the split. I found it in these\l"
	.string "ruins.\p"
	.string "I'm not writing it here. If\n"
	.string "someone reads this before SCHISM\l"
	.string "is healed, the name won't help\l"
	.string "them.\p"
	.string "If someone reads it after - ask\n"
	.string "EIRA. She knows. She just\l"
	.string "doesn't say.\p"
	.string "…DORNE was here six months ago.\n"
	.string "He spoke to both factions. I\l"
	.string "found his documents in DRENN's\l"
	.string "office - copies.\p"
	.string "He's been feeding both sides the\n"
	.string "same lie in different\l"
	.string "directions. Brilliant.\l"
	.string "Terrifying.”\p"
	.string "(The fifth cipher fragment has\n"
	.string "been recorded in the journal!)$"

SchismScarRuins_Text_InscriptionAgain:
	.string "“GLACITH preserves. TOXARA\n"
	.string "cleanses. Neither alone. Both\l"
	.string "together.\p"
	.string "The island breathes between\n"
	.string "them.”$"
