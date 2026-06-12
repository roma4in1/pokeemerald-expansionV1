Sirocco_DexCamp_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ First-entry trigger (VAR_SIROCCO_PROGRESS == 1, pre-Gym 1).
@ The camp is shut tight - Gilt Claw eyes are on it. VAR_TEMP_0
@ keeps it to once per map session; it stops firing for good once
@ Silt is beaten (progress 2 disarms the coord trigger).
@ -------------------------------------------------------------------
SiroccoDexCamp_EventScript_FirstEntry::
	goto_if_eq VAR_TEMP_0, 1, SiroccoDexCamp_EventScript_FirstEntryEnd
	setvar VAR_TEMP_0, 1
	lockall
	msgbox SiroccoDexCamp_Text_CampShut, MSGBOX_DEFAULT
	releaseall
SiroccoDexCamp_EventScript_FirstEntryEnd::
	end

@ -------------------------------------------------------------------
@ Dex - archaeologist, first major recurring NPC. Warm, excitable,
@ slightly chaotic. SURVIVES this island - keep him safe and alive.
@ Branches:
@   resolved          -> Crag joins the dig, upbeat
@   cipher found      -> mural excitement
@   met               -> new panel + Warden's Journal recognition
@   gym 1 clear       -> full introduction (sets FLAG_SIROCCO_DEX_MET)
@   else              -> nervous brush-off (Gilt Claw watching)
@ -------------------------------------------------------------------
SiroccoDexCamp_EventScript_Dex::
	lock
	faceplayer
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDexCamp_EventScript_DexResolved
	goto_if_set FLAG_SIROCCO_CIPHER_FOUND, SiroccoDexCamp_EventScript_DexCipher
	goto_if_set FLAG_SIROCCO_DEX_MET, SiroccoDexCamp_EventScript_DexAgain
	goto_if_set FLAG_SIROCCO_GYM1_CLEAR, SiroccoDexCamp_EventScript_DexIntro
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexNervous, MSGBOX_DEFAULT
	release
	end

SiroccoDexCamp_EventScript_DexIntro::
	playse SE_PIN
	applymovement LOCALID_SIROCCO_DEX, Common_Movement_ExclamationMark
	waitmovement 0
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexIntro1, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDexCamp_Text_DexIntroBeat, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexIntro2, MSGBOX_DEFAULT
	msgbox SiroccoDexCamp_Text_DexIntro3, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SIROCCO_DEX_MET
	release
	end

SiroccoDexCamp_EventScript_DexAgain::
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexNewPanel, MSGBOX_DEFAULT
	checkitem ITEM_WARDENS_JOURNAL
	goto_if_eq VAR_RESULT, FALSE, SiroccoDexCamp_EventScript_DexNoJournal
	setspeaker 0
	msgbox SiroccoDexCamp_Text_DexSpotsJournal, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexJournalReaction, MSGBOX_DEFAULT
	release
	end

SiroccoDexCamp_EventScript_DexNoJournal::
	release
	end

SiroccoDexCamp_EventScript_DexCipher::
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexCipher, MSGBOX_DEFAULT
	release
	end

SiroccoDexCamp_EventScript_DexResolved::
	setspeaker Pelagios_Speaker_Dex
	msgbox SiroccoDexCamp_Text_DexResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Research notes on the table
@ -------------------------------------------------------------------
SiroccoDexCamp_EventScript_ResearchNotes::
	msgbox SiroccoDexCamp_Text_ResearchNotes, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoDexCamp_Text_CampShut:
	.string "The camp is shut up tight. Tarps\n"
	.string "lashed over the equipment, crates\l"
	.string "roped closed.\p"
	.string "A hand-lettered sign hangs from\n"
	.string "the guide rope:\p"
	.string "“NO VISITORS - GILT CLAW\n"
	.string "WATCHING.”$"

SiroccoDexCamp_Text_DexNervous:
	.string "Not now, not now! They count\n"
	.string "everyone who stops here.\p"
	.string "Unless you can make their\n"
	.string "enforcers look the other way,\l"
	.string "we have NOT met.\p"
	.string "…I hear the one in MIRADEN\n"
	.string "respects a good battle.$"

SiroccoDexCamp_Text_DexIntro1:
	.string "Oh! A visitor who isn't Gilt\n"
	.string "Claw. WONDERFUL.\p"
	.string "And the one who put SILT on his\n"
	.string "heels, no less! They're far too\l"
	.string "busy gossiping about you to\l"
	.string "watch my little camp.\p"
	.string "You see this sand? UNDERNEATH\n"
	.string "it? A whole city.\p"
	.string "They sealed something down there,\n"
	.string "and the Gilt Claw doesn't want\l"
	.string "anyone to find out what.$"

SiroccoDexCamp_Text_DexIntroBeat:
	.string "He spreads a rubbing of carved\n"
	.string "script across the table, smoothing\l"
	.string "it with both hands like a map of\l"
	.string "somewhere he'd rather be.$"

SiroccoDexCamp_Text_DexIntro2:
	.string "I've found three inscription\n"
	.string "panels in a WEEK.\p"
	.string "Same script as some ruins I\n"
	.string "documented years ago, on a place\l"
	.string "called HAVEN ISLE.\p"
	.string "Remarkable, isn't it? Same\n"
	.string "civilization, thousands of miles\l"
	.string "apart.\p"
	.string "What were they building? And why\n"
	.string "did they seal it?$"

SiroccoDexCamp_Text_DexIntro3:
	.string "The excavation's east of here,\n"
	.string "past MIRADEN. My dig markers\l"
	.string "will show you the way down.\p"
	.string "If anyone with a claw on their\n"
	.string "coat asks - you're a lost\l"
	.string "tourist. Works for me every\l"
	.string "time. Almost every time.$"

SiroccoDexCamp_Text_DexNewPanel:
	.string "Found another panel today!\p"
	.string "The script mentions something\n"
	.string "called a WARDEN. Someone who\l"
	.string "maintained a network of some\l"
	.string "kind.\p"
	.string "Does that mean anything to you?$"

SiroccoDexCamp_Text_DexSpotsJournal:
	.string "His eyes catch on the worn journal\n"
	.string "tucked in your bag - and go very,\l"
	.string "very wide.$"

SiroccoDexCamp_Text_DexJournalReaction:
	.string "Wait. That journal.\n"
	.string "Can I see it?\p"
	.string "I've been trying to decode this\n"
	.string "script for MONTHS, and your\l"
	.string "journal uses the same cipher\l"
	.string "structure. Extraordinary.\p"
	.string "Whoever kept that book knew this\n"
	.string "civilization better than anyone\l"
	.string "alive.\p"
	.string "…Keep it close, would you? Some\n"
	.string "things shouldn't pass through\l"
	.string "Gilt Claw hands.$"

SiroccoDexCamp_Text_DexCipher:
	.string "You found the central mural?\n"
	.string "THE central mural?\p"
	.string "I've only seen the edge of it\n"
	.string "past the rubble line!\p"
	.string "A sealing ceremony, rendered by\n"
	.string "people who WATCHED it happen…\p"
	.string "When this island calms down, I'm\n"
	.string "going to spend a decade in that\l"
	.string "room. A happy, happy decade.$"

SiroccoDexCamp_Text_DexResolved:
	.string "CRAG asked to join the dig!\n"
	.string "Asked twice, actually - I said\l"
	.string "yes faster than he could finish.\p"
	.string "A proper rock specialist, funding\n"
	.string "that isn't criminal, and a city\l"
	.string "nobody's allowed to bulldoze.\p"
	.string "Best season of my career, and it\n"
	.string "hasn't even started.$"

SiroccoDexCamp_Text_ResearchNotes:
	.string "Field notes, in handwriting that\n"
	.string "starts neat and loses the fight\l"
	.string "by mid-page:\p"
	.string "“Masonry matches the HAVEN ISLE\n"
	.string "ruins. Not similar - IDENTICAL.\l"
	.string "Same hands, or same teachers.”\p"
	.string "“They didn't live atop the deep\n"
	.string "chamber. They built AROUND it.\l"
	.string "Like a shrine. Or a cage.”\p"
	.string "“NB: stop writing locations down.\n"
	.string "The Gilt Claw reads faster than\l"
	.string "they dig.”$"
