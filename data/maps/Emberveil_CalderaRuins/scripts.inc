Emberveil_CalderaRuins_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Path B unlock - the Warden's field notes.
@ The three panels must be examined IN ORDER (1 -> 2 -> 3) within one
@ visit: VAR_TEMP_1 is the brief's "local step counter" (VAR_TEMP_*
@ resets on map load, so leaving the ruins restarts the sequence).
@   Panel 1: VAR_TEMP_1 0 -> 1
@   Panel 2: VAR_TEMP_1 1 -> 2
@   Panel 3: at VAR_TEMP_1 == 2, opens the alcove - ITEM_WARDEN_NOTES
@            + FLAG_WARDEN_NOTES_EMBERVEIL (permanent; Path B armed)
@ Partial translations show once the player carries two decoded
@ ciphers (FLAG_CIPHER_2_FOUND - always true by Emberveil, which
@ requires the Brigantine, i.e. Ironhold resolved).
@ -------------------------------------------------------------------
EmberveilCalderaRuins_EventScript_Panel1::
	lockall
	msgbox EmberveilCalderaRuins_Text_Panel1Script, MSGBOX_DEFAULT
	goto_if_unset FLAG_CIPHER_2_FOUND, EmberveilCalderaRuins_EventScript_Panel1Advance
	msgbox EmberveilCalderaRuins_Text_Panel1Translation, MSGBOX_DEFAULT
	goto EmberveilCalderaRuins_EventScript_Panel1Advance

EmberveilCalderaRuins_EventScript_Panel1Advance::
	goto_if_eq VAR_TEMP_1, 0, EmberveilCalderaRuins_EventScript_Panel1Set
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel1Set::
	setvar VAR_TEMP_1, 1
	msgbox EmberveilCalderaRuins_Text_Panel1Cue, MSGBOX_DEFAULT
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel2::
	lockall
	msgbox EmberveilCalderaRuins_Text_Panel2Script, MSGBOX_DEFAULT
	goto_if_unset FLAG_CIPHER_2_FOUND, EmberveilCalderaRuins_EventScript_Panel2Advance
	msgbox EmberveilCalderaRuins_Text_Panel2Translation, MSGBOX_DEFAULT
	goto EmberveilCalderaRuins_EventScript_Panel2Advance

EmberveilCalderaRuins_EventScript_Panel2Advance::
	goto_if_eq VAR_TEMP_1, 1, EmberveilCalderaRuins_EventScript_Panel2Set
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel2Set::
	setvar VAR_TEMP_1, 2
	msgbox EmberveilCalderaRuins_Text_Panel2Cue, MSGBOX_DEFAULT
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel3::
	lockall
	goto_if_set FLAG_WARDEN_NOTES_EMBERVEIL, EmberveilCalderaRuins_EventScript_Panel3Empty
	goto_if_eq VAR_TEMP_1, 2, EmberveilCalderaRuins_EventScript_Panel3Discovery
	msgbox EmberveilCalderaRuins_Text_Panel3Sealed, MSGBOX_DEFAULT
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel3Discovery::
	msgbox EmberveilCalderaRuins_Text_Panel3Opens, MSGBOX_DEFAULT
	closemessage
	playse SE_CLICK
	fadescreen FADE_TO_BLACK
	delay 30
	fadescreen FADE_FROM_BLACK
	msgbox EmberveilCalderaRuins_Text_Panel3Notes, MSGBOX_DEFAULT
	msgbox EmberveilCalderaRuins_Text_Panel3Margin, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_WARDEN_NOTES_EMBERVEIL
	giveitem ITEM_WARDEN_NOTES
	msgbox EmberveilCalderaRuins_Text_Panel3AfterTake, MSGBOX_DEFAULT
	releaseall
	end

EmberveilCalderaRuins_EventScript_Panel3Empty::
	msgbox EmberveilCalderaRuins_Text_Panel3Empty, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilCalderaRuins_Text_Panel1Script:
	.string "A stone panel, older than the\n"
	.string "cult, older than CINDERHOLD.\p"
	.string "Lines of ancient script spiral\n"
	.string "inward - the same carving you\l"
	.string "saw in the HAVEN ISLE ruins and\l"
	.string "under IRONHOLD's fortress.$"

EmberveilCalderaRuins_Text_Panel1Translation:
	.string "With two cipher fragments in the\n"
	.string "journal, fragments of meaning\l"
	.string "surface:\p"
	.string "“…the seal requires maintenance\n"
	.string "through resonance…\p"
	.string "…the ritual of communion\n"
	.string "sustains without breaking…”$"

EmberveilCalderaRuins_Text_Panel1Cue:
	.string "The spiral's last line runs off\n"
	.string "the panel's edge - eastward,\l"
	.string "toward its twin.$"

EmberveilCalderaRuins_Text_Panel2Script:
	.string "A second panel, twin to the\n"
	.string "first. The spiral here turns\l"
	.string "outward, like an answer.$"

EmberveilCalderaRuins_Text_Panel2Translation:
	.string "The journal's fragments tease\n"
	.string "out a little more:\p"
	.string "“…those who tend the flame tend\n"
	.string "the seal…\p"
	.string "…the WARDEN need not be present\n"
	.string "if the community…”\p"
	.string "The sentence breaks off,\n"
	.string "unfinished for a thousand years.$"

EmberveilCalderaRuins_Text_Panel2Cue:
	.string "Both spirals' final strokes\n"
	.string "angle up the wall - toward a\l"
	.string "small third panel set higher in\l"
	.string "the stone.$"

EmberveilCalderaRuins_Text_Panel3Sealed:
	.string "A small panel set into the\n"
	.string "caldera wall, higher than the\l"
	.string "others.\p"
	.string "A hairline seam runs around its\n"
	.string "border - it could open, but no\l"
	.string "amount of prying budges it.\p"
	.string "Perhaps the larger panels\n"
	.string "explain the mechanism.$"

EmberveilCalderaRuins_Text_Panel3Opens:
	.string "You trace the two spirals'\n"
	.string "paths to the small panel and\l"
	.string "press, the way the strokes\l"
	.string "seem to ask.\p"
	.string "The seam exhales a breath of\n"
	.string "ancient air, and the panel\l"
	.string "swings open on a hidden alcove.$"

EmberveilCalderaRuins_Text_Panel3Notes:
	.string "Inside: a weatherproof folder,\n"
	.string "tucked deep where the heat\l"
	.string "can't reach.\p"
	.string "Field notes. Dozens of pages.\p"
	.string "These are in your parent's\n"
	.string "handwriting. Recent - within\l"
	.string "the last few years.$"

EmberveilCalderaRuins_Text_Panel3Margin:
	.string "A diagram, carefully drawn.\n"
	.string "A ritual.\p"
	.string "Notes in the margin:\p"
	.string "“Communion without breaking.\n"
	.string "The cult is doing it wrong, but\l"
	.string "they're doing SOMETHING.\p"
	.string "This is the correction.”$"

EmberveilCalderaRuins_Text_Panel3AfterTake:
	.string "Your parent stood here. Read\n"
	.string "these same spirals. Left this\l"
	.string "for whoever came next.\p"
	.string "The HIGH EMBER is at the summit,\n"
	.string "and now you have something to\l"
	.string "show her.$"

EmberveilCalderaRuins_Text_Panel3Empty:
	.string "The alcove stands open and\n"
	.string "empty.\p"
	.string "Your parent's notes are in your\n"
	.string "pack, where they belong.$"
