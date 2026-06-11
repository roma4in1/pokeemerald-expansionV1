HavenIsle_PlayerHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HavenIsle_PlayerHouse_2F_OnFrame
	.byte 0

HavenIsle_PlayerHouse_2F_OnFrame:
	map_script_2 VAR_PELAGIOS_INTRO_STATE, 0, HavenIsle_PlayerHouse_2F_EventScript_MomCalls
	.2byte 0

HavenIsle_PlayerHouse_2F_EventScript_MomCalls::
	lockall
	delay 30
	msgbox HavenIsle_PlayerHouse_2F_Text_MomCalls, MSGBOX_DEFAULT
	closemessage
	setvar VAR_PELAGIOS_INTRO_STATE, 1
	releaseall
	end

HavenIslePlayerHouse_2F_EventScript_PC::
	lockall
	goto_if_set FLAG_RECEIVED_TENNYSON_KEY, HavenIslePlayerHouse_2F_EventScript_UsePC
	goto_if_set FLAG_STARTER_CHOSEN, HavenIslePlayerHouse_2F_EventScript_FindKey
	msgbox HavenIslePlayerHouse_2F_Text_PCLocked, MSGBOX_DEFAULT
	releaseall
	end

HavenIslePlayerHouse_2F_EventScript_FindKey::
	msgbox HavenIslePlayerHouse_2F_Text_ParcelOnPC, MSGBOX_DEFAULT
	giveitem ITEM_TENNYSON_KEY
	setflag FLAG_RECEIVED_TENNYSON_KEY
	msgbox HavenIslePlayerHouse_2F_Text_KeyNote, MSGBOX_DEFAULT
	releaseall
	end

HavenIslePlayerHouse_2F_EventScript_UsePC::
	releaseall
	goto EventScript_PC
	end

HavenIslePlayerHouse_2F_EventScript_Notebook::
	msgbox HavenIslePlayerHouse_2F_Text_Notebook, MSGBOX_SIGN
	end

HavenIsle_PlayerHouse_2F_Text_MomCalls:
	.string "MOM: {PLAYER}! Are you up yet?\p"
	.string "Come downstairs, sweetheart.\n"
	.string "There's something I need to tell you!$"

HavenIslePlayerHouse_2F_Text_PCLocked:
	.string "It's {PLAYER}'s PC.\p"
	.string "The screen is dark. Nothing on it\n"
	.string "needs your attention right now.$"

HavenIslePlayerHouse_2F_Text_ParcelOnPC:
	.string "Something has been left on top\n"
	.string "of the PC.\p"
	.string "A small parcel, wrapped in old\n"
	.string "sailcloth. There's a tag.\p"
	.string "“For {PLAYER}. When you're ready.\n"
	.string "It was always going to be yours.”$"

HavenIslePlayerHouse_2F_Text_KeyNote:
	.string "It's the key to the TENNYSON…\p"
	.string "Your parent's boat, moored down\n"
	.string "at the harbor.\p"
	.string "Nobody has touched it since the\n"
	.string "WARDEN died.$"

HavenIslePlayerHouse_2F_Text_Notebook:
	.string "It's an old logbook, kept since you\n"
	.string "were small.\p"
	.string "The last entry is written in a hand\n"
	.string "that isn't yours:\p"
	.string "“The sea remembers what we forget.”$"
