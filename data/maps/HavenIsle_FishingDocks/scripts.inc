HavenIsle_FishingDocks_MapScripts::
	.byte 0

FishingDocks_EventScript_OldFisherman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_OLD_ROD_HAVEN, FishingDocks_EventScript_AlreadyGaveRod
	msgbox FishingDocks_Text_TakeOldRod, MSGBOX_DEFAULT
	giveitem ITEM_OLD_ROD
	setflag FLAG_RECEIVED_OLD_ROD_HAVEN
	msgbox FishingDocks_Text_RodAdvice, MSGBOX_DEFAULT
	release
	end

FishingDocks_EventScript_AlreadyGaveRod::
	msgbox FishingDocks_Text_AlreadyGaveRod, MSGBOX_DEFAULT
	release
	end

FishingDocks_EventScript_TipsSailor::
	msgbox FishingDocks_Text_FishingTips, MSGBOX_NPC
	end

FishingDocks_EventScript_Sign::
	msgbox FishingDocks_Text_Sign, MSGBOX_SIGN
	end

FishingDocks_Text_TakeOldRod:
	.string "Sit down, sit down. The fish don't\n"
	.string "bite for people who hover.\p"
	.string "…The WARDEN's child, out at my\n"
	.string "docks. Ha. Time is a tide, no\l"
	.string "mistake.\p"
	.string "Your parent fished this exact spot,\n"
	.string "you know. Said it was the only place\l"
	.string "on the island where nobody asked\l"
	.string "questions.\p"
	.string "Here. I keep spares for promising\n"
	.string "company.$"

FishingDocks_Text_RodAdvice:
	.string "Cast it anywhere the water's calm.\n"
	.string "Feel the tug, then press your luck.\p"
	.string "And if you hook something you don't\n"
	.string "recognize…\p"
	.string "…throw it back, and tell me what\n"
	.string "it looked like.$"

FishingDocks_Text_AlreadyGaveRod:
	.string "Any bites yet?\p"
	.string "Patience. The sea gives you what\n"
	.string "you need, rarely what you ask for.\p"
	.string "Your parent heard that from me\n"
	.string "first, whatever anyone else claims.$"

FishingDocks_Text_FishingTips:
	.string "Press the button when you feel a\n"
	.string "tug — not before, not after. THE\l"
	.string "MOMENT.\p"
	.string "I've seen grown sailors weep over\n"
	.string "a slow thumb.$"

FishingDocks_Text_Sign:
	.string "TIDEMARK FISHING DOCKS\p"
	.string "“Quiet, please.\n"
	.string "The fish are listening.”$"
