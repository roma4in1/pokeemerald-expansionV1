HavenIsle_PlayerHouse_1F_MapScripts::
	.byte 0

HavenIslePlayerHouse_1F_EventScript_Mom::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mom
	goto_if_set FLAG_HAVEN_ISLE_COMPLETE, HavenIslePlayerHouse_1F_EventScript_MomFarewell
	goto_if_set FLAG_STARTER_CHOSEN, HavenIslePlayerHouse_1F_EventScript_MomAfterStarter
	goto_if_eq VAR_PELAGIOS_INTRO_STATE, 2, HavenIslePlayerHouse_1F_EventScript_MomReminder
	msgbox HavenIslePlayerHouse_1F_Text_MomIntro, MSGBOX_DEFAULT
	setvar VAR_PELAGIOS_INTRO_STATE, 2
	release
	end

HavenIslePlayerHouse_1F_EventScript_MomReminder::
	msgbox HavenIslePlayerHouse_1F_Text_MomReminder, MSGBOX_DEFAULT
	release
	end

HavenIslePlayerHouse_1F_EventScript_MomAfterStarter::
	msgbox HavenIslePlayerHouse_1F_Text_MomAfterStarter, MSGBOX_DEFAULT
	release
	end

HavenIslePlayerHouse_1F_EventScript_MomFarewell::
	msgbox HavenIslePlayerHouse_1F_Text_MomFarewell, MSGBOX_DEFAULT
	release
	end

HavenIslePlayerHouse_1F_Text_MomIntro:
	.string "There you are.\p"
	.string "PROFESSOR SOLLIS came by at dawn,\n"
	.string "asking for you. She wouldn't say why,\l"
	.string "but she had that look about her.\p"
	.string "Her lab is the big building on the\n"
	.string "east side of the village.\p"
	.string "…{PLAYER}.\p"
	.string "I walked past the harbor yesterday.\n"
	.string "The TENNYSON is still at her berth.\p"
	.string "Nobody's touched her since your\n"
	.string "parent… since the WARDEN left us.\p"
	.string "Maybe it's time somebody did.$"

HavenIslePlayerHouse_1F_Text_MomReminder:
	.string "Go on, now. PROFESSOR SOLLIS\n"
	.string "is waiting at the lab.\p"
	.string "East side of the village. You can't\n"
	.string "miss it.$"

HavenIslePlayerHouse_1F_Text_MomAfterStarter:
	.string "Oh! Look at the little thing.\p"
	.string "It has your eyes. Don't ask me how,\n"
	.string "but it does.\p"
	.string "Your parent always said a WARDEN's\n"
	.string "first partner chooses them.\p"
	.string "I suppose some things are still true.$"

HavenIslePlayerHouse_1F_Text_MomFarewell:
	.string "So. Ironhold.\p"
	.string "…I'm not going to pretend I'm not\n"
	.string "scared. But I won't stop you.\p"
	.string "The sea is in this family, {PLAYER}.\n"
	.string "It always finds its way back.\p"
	.string "Eat properly. Sleep when you can.\n"
	.string "And come home.$"
