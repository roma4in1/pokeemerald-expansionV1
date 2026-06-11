HavenIsle_Harbor_MapScripts::
	.byte 0

HavenIsleHarbor_EventScript_Sailor::
	lock
	faceplayer
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
	msgbox HavenIsleHarbor_Text_TennysonReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HavenIsleHarbor_EventScript_TennysonStay
	@ Safety net: never downgrade the tier, but guarantee at least Sloop here.
	goto_if_ne VAR_BOAT_TIER, 0, HavenIsleHarbor_EventScript_SetSail
	setvar VAR_BOAT_TIER, 1
HavenIsleHarbor_EventScript_SetSail::
	msgbox HavenIsleHarbor_Text_TennysonCastOff, MSGBOX_DEFAULT
	closemessage
	warp MAP_IRONHOLD_GATEMARK_PORT, 10, 14
	waitstate
	releaseall
	end

HavenIsleHarbor_EventScript_TennysonStay::
	msgbox HavenIsleHarbor_Text_TennysonStay, MSGBOX_DEFAULT
	releaseall
	end

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

HavenIsleHarbor_Text_TennysonReady:
	.string "The TENNYSON rides high under her\n"
	.string "new sloop rigging, lines creaking\l"
	.string "softly in the wind.\p"
	.string "Set sail for IRONHOLD?$"

HavenIsleHarbor_Text_TennysonCastOff:
	.string "You cast off the mooring lines.\p"
	.string "The TENNYSON leans into the wind\n"
	.string "like she's been waiting years for\l"
	.string "permission.\p"
	.string "Haven Isle shrinks behind you…\n"
	.string "and the iron coast rises ahead.$"

HavenIsleHarbor_Text_TennysonStay:
	.string "You rest a hand on the gunwale.\p"
	.string "No rush. She's waited this long.$"

HavenIsleHarbor_Text_VillageSign:
	.string "TIDEMARK VILLAGE - 1 AHEAD\p"
	.string "“Fair winds begin at home.”$"

HavenIsleHarbor_Text_BerthSign:
	.string "PRIVATE BERTH - THE TENNYSON\p"
	.string "Registered keeper: (the name has\n"
	.string "been lovingly re-painted many times)$"
