HavenIsle_Harbor_MapScripts::
	.byte 0

HavenIsleHarbor_EventScript_Sailor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sailor
	goto_if_set FLAG_BOAT_SLOOP_UNLOCKED, HavenIsleHarbor_EventScript_SailorSloop
	msgbox HavenIsleHarbor_Text_SailorTips, MSGBOX_DEFAULT
	release
	end

HavenIsleHarbor_EventScript_SailorSloop::
	msgbox HavenIsleHarbor_Text_SailorSloop, MSGBOX_DEFAULT
	release
	end

HavenIsleHarbor_EventScript_Tennyson::
	lockall
	goto_if_set FLAG_BOAT_SLOOP_UNLOCKED, HavenIsleHarbor_EventScript_TennysonReady
	goto_if_set FLAG_RECEIVED_TENNYSON_KEY, HavenIsleHarbor_EventScript_TennysonKey
	msgbox HavenIsleHarbor_Text_TennysonLocked, MSGBOX_DEFAULT
	releaseall
	end

HavenIsleHarbor_EventScript_TennysonKey::
	msgbox HavenIsleHarbor_Text_TennysonWithKey, MSGBOX_DEFAULT
	releaseall
	end

HavenIsleHarbor_EventScript_TennysonReady::
	@ Safety net: never downgrade the tier, but guarantee at least Sloop here.
	goto_if_ne VAR_BOAT_TIER, 0, HavenIsleHarbor_EventScript_TennysonMenu
	setvar VAR_BOAT_TIER, 1
HavenIsleHarbor_EventScript_TennysonMenu::
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_HAVEN
	goto Pelagios_EventScript_BoardTennyson

HavenIsleHarbor_EventScript_VillageSign::
	msgbox HavenIsleHarbor_Text_VillageSign, MSGBOX_SIGN
	end

HavenIsleHarbor_EventScript_BerthSign::
	msgbox HavenIsleHarbor_Text_BerthSign, MSGBOX_SIGN
	end

HavenIsleHarbor_Text_SailorTips:
	.string "First rule of sailing: the sea\n"
	.string "doesn't care about your schedule.\p"
	.string "Second rule: check your rigging\n"
	.string "twice, then check it again.\p"
	.string "Third rule… hm.\p"
	.string "Third rule is don't ask a sailor\n"
	.string "how many rules there are. We make\l"
	.string "them up as the weather demands.$"

HavenIsleHarbor_Text_SailorSloop:
	.string "We rigged the TENNYSON as a sloop,\n"
	.string "like the professor asked.\p"
	.string "She's tide-worthy, kid. More than.\n"
	.string "That boat was kept with LOVE.\p"
	.string "…The WARDEN would be glad it's\n"
	.string "you taking her out.\p"
	.string "Mind the Covenant patrols near\n"
	.string "IRONHOLD. They board first and\l"
	.string "apologize never.$"

HavenIsleHarbor_Text_TennysonLocked:
	.string "It's the TENNYSON - the WARDEN's\n"
	.string "boat.\p"
	.string "Her brass is dull and her mooring\n"
	.string "lines are stiff with salt, but she\l"
	.string "sits the water like she's only\l"
	.string "waiting for a reason.\p"
	.string "The cabin is locked.$"

HavenIsleHarbor_Text_TennysonWithKey:
	.string "The TENNYSON KEY fits the cabin\n"
	.string "lock like it never left.\p"
	.string "Inside it smells of cedar and rope\n"
	.string "and twenty years of careful hands.\p"
	.string "She isn't rigged for open water.\n"
	.string "Not yet.$"

HavenIsleHarbor_Text_VillageSign:
	.string "TIDEMARK VILLAGE - 1 AHEAD\p"
	.string "“Fair winds begin at home.”$"

HavenIsleHarbor_Text_BerthSign:
	.string "PRIVATE BERTH - THE TENNYSON\p"
	.string "Registered keeper: (the name has\n"
	.string "been lovingly re-painted many times)$"
