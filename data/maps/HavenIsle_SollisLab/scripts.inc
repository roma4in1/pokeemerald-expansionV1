HavenIsle_SollisLab_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HavenIsle_SollisLab_OnFrame
	.byte 0

HavenIsle_SollisLab_OnFrame:
	map_script_2 VAR_PELAGIOS_INTRO_STATE, 3, SollisLab_EventScript_FirstEntry
	.2byte 0

SollisLab_EventScript_FirstEntry::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkUp
	waitmovement 0
	msgbox SollisLab_Text_CassBeatYou, MSGBOX_DEFAULT
	applymovement LOCALID_LAB_SOLLIS, Common_Movement_FaceDown
	waitmovement 0
	msgbox SollisLab_Text_SollisIntro, MSGBOX_DEFAULT
	closemessage
	setvar VAR_PELAGIOS_INTRO_STATE, 4
	releaseall
	end

SollisLab_EventScript_Sollis::
	lock
	faceplayer
	goto_if_set FLAG_HAVEN_ISLE_COMPLETE, SollisLab_EventScript_SollisIronholdReminder
	goto_if_eq VAR_HAVEN_RUINS_STATE, 3, SollisLab_EventScript_SollisWardenTalk
	goto_if_eq VAR_HAVEN_RUINS_STATE, 2, SollisLab_EventScript_SollisInvestigate
	goto_if_set FLAG_STARTER_CHOSEN, SollisLab_EventScript_SollisExplore
	msgbox SollisLab_Text_SollisChooseHint, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_SollisExplore::
	msgbox SollisLab_Text_SollisExplore, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_SollisInvestigate::
	msgbox SollisLab_Text_SollisInvestigate, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_SollisWardenTalk::
	msgbox SollisLab_Text_SollisWardenTalk, MSGBOX_DEFAULT
	giveitem ITEM_NAVIGATORS_LOG
	msgbox SollisLab_Text_SollisSloop, MSGBOX_DEFAULT
	setflag FLAG_HAVEN_ISLE_COMPLETE
	setflag FLAG_BOAT_SLOOP_UNLOCKED
	release
	end

SollisLab_EventScript_SollisIronholdReminder::
	msgbox SollisLab_Text_SollisIronholdReminder, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_Cass::
	lock
	faceplayer
	msgbox SollisLab_Text_CassWaiting, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Starter selection
@ ---------------------------------------------------------------------------

SollisLab_EventScript_Totodile::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_TOTODILE, CRY_MODE_NORMAL
	goto_if_unset FLAG_STARTER_CHOSEN, SollisLab_EventScript_TotodileOffer
	msgbox SollisLab_Text_TotodileLeftover, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_TotodileOffer::
	msgbox SollisLab_Text_TotodileFlavor, MSGBOX_DEFAULT
	msgbox SollisLab_Text_TotodileOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_DeclineStarter
	givemon SPECIES_TOTODILE, 5
	playfanfare MUS_OBTAIN_ITEM
	message SollisLab_Text_ReceivedTotodile
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_STARTER_CHOSEN
	setflag FLAG_ADVENTURE_STARTED
	setvar VAR_PELAGIOS_STARTER, 1
	setflag FLAG_HIDE_SOLLIS_LAB_TOTODILE
	removeobject LOCALID_LAB_TOTODILE
	msgbox SollisLab_Text_GiveNickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_TotodileChosen
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
SollisLab_EventScript_TotodileChosen::
	msgbox SollisLab_Text_CassTakesChimchar, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_SOLLIS_LAB_CHIMCHAR
	removeobject LOCALID_LAB_CHIMCHAR
	goto SollisLab_EventScript_AfterChoice
	end

SollisLab_EventScript_Chimchar::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CHIMCHAR, CRY_MODE_NORMAL
	goto_if_unset FLAG_STARTER_CHOSEN, SollisLab_EventScript_ChimcharOffer
	msgbox SollisLab_Text_ChimcharLeftover, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_ChimcharOffer::
	msgbox SollisLab_Text_ChimcharFlavor, MSGBOX_DEFAULT
	msgbox SollisLab_Text_ChimcharOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_DeclineStarter
	givemon SPECIES_CHIMCHAR, 5
	playfanfare MUS_OBTAIN_ITEM
	message SollisLab_Text_ReceivedChimchar
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_STARTER_CHOSEN
	setflag FLAG_ADVENTURE_STARTED
	setvar VAR_PELAGIOS_STARTER, 2
	setflag FLAG_HIDE_SOLLIS_LAB_CHIMCHAR
	removeobject LOCALID_LAB_CHIMCHAR
	msgbox SollisLab_Text_GiveNickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_ChimcharChosen
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
SollisLab_EventScript_ChimcharChosen::
	msgbox SollisLab_Text_CassTakesRowlet, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_SOLLIS_LAB_ROWLET
	removeobject LOCALID_LAB_ROWLET
	goto SollisLab_EventScript_AfterChoice
	end

SollisLab_EventScript_Rowlet::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ROWLET, CRY_MODE_NORMAL
	goto_if_unset FLAG_STARTER_CHOSEN, SollisLab_EventScript_RowletOffer
	msgbox SollisLab_Text_RowletLeftover, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_RowletOffer::
	msgbox SollisLab_Text_RowletFlavor, MSGBOX_DEFAULT
	msgbox SollisLab_Text_RowletOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_DeclineStarter
	givemon SPECIES_ROWLET, 5
	playfanfare MUS_OBTAIN_ITEM
	message SollisLab_Text_ReceivedRowlet
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_STARTER_CHOSEN
	setflag FLAG_ADVENTURE_STARTED
	setvar VAR_PELAGIOS_STARTER, 3
	setflag FLAG_HIDE_SOLLIS_LAB_ROWLET
	removeobject LOCALID_LAB_ROWLET
	msgbox SollisLab_Text_GiveNickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SollisLab_EventScript_RowletChosen
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
SollisLab_EventScript_RowletChosen::
	msgbox SollisLab_Text_CassTakesTotodile, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_SOLLIS_LAB_TOTODILE
	removeobject LOCALID_LAB_TOTODILE
	goto SollisLab_EventScript_AfterChoice
	end

SollisLab_EventScript_DeclineStarter::
	msgbox SollisLab_Text_DeclineStarter, MSGBOX_DEFAULT
	release
	end

SollisLab_EventScript_AfterChoice::
	applymovement LOCALID_LAB_CASS, SollisLab_Movement_CassLeave
	waitmovement 0
	removeobject LOCALID_LAB_CASS
	setflag FLAG_HIDE_SOLLIS_LAB_CASS
	applymovement LOCALID_LAB_SOLLIS, Common_Movement_FaceDown
	waitmovement 0
	msgbox SollisLab_Text_SollisJournal, MSGBOX_DEFAULT
	giveitem ITEM_WARDENS_JOURNAL
	setflag FLAG_JOURNAL_OBTAINED
	msgbox SollisLab_Text_SollisAfterJournal, MSGBOX_DEFAULT
	setvar VAR_PELAGIOS_INTRO_STATE, 5
	setvar VAR_HAVEN_RUINS_STATE, 1
	release
	end

SollisLab_Movement_CassLeave:
	face_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_down
	step_end

SollisLab_EventScript_Journal::
	lockall
	goto_if_set FLAG_JOURNAL_OBTAINED, SollisLab_EventScript_DeskAfter
	msgbox SollisLab_Text_DeskBefore, MSGBOX_DEFAULT
	releaseall
	end

SollisLab_EventScript_DeskAfter::
	msgbox SollisLab_Text_DeskAfter, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

SollisLab_Text_CassBeatYou:
	.string "CASS: Ha! Slowpoke!\p"
	.string "I've been here for AGES. Years,\n"
	.string "possibly. I have a beard now.$"

SollisLab_Text_SollisIntro:
	.string "SOLLIS: And there's the other one.\n"
	.string "Good. Close the door, the TOTODILE\l"
	.string "bolts for the beach if you don't.\p"
	.string "{PLAYER}. Cass. You both know why\n"
	.string "I asked you here, more or less.\p"
	.string "You're old enough, and this island\n"
	.string "needs more trainers than it has.\p"
	.string "Three POKéMON have been living in\n"
	.string "this lab with me.\p"
	.string "I've spent six months trying to\n"
	.string "figure out which one to give you.\p"
	.string "Apparently they've already decided.\p"
	.string "The TOTODILE patrols the door.\n"
	.string "The CHIMCHAR claims the top shelf.\l"
	.string "And the ROWLET will not get off\l"
	.string "that journal, no matter what I do.\p"
	.string "Go on. Introduce yourself.\n"
	.string "Whichever one answers… is yours.$"

SollisLab_Text_SollisChooseHint:
	.string "SOLLIS: Don't overthink it.\p"
	.string "Walk up to one of them and see\n"
	.string "what happens. They've had six\l"
	.string "months to make up their minds.$"

SollisLab_Text_SollisExplore:
	.string "SOLLIS: Take today. Walk the island,\n"
	.string "say your goodbyes, let your partner\l"
	.string "stretch its legs.\p"
	.string "The coastal routes are good training.\n"
	.string "Gentle, mostly.\p"
	.string "…Mostly.$"

SollisLab_Text_SollisInvestigate:
	.string "SOLLIS: You felt it too, didn't you.\n"
	.string "The whole island shivered.\p"
	.string "My instruments all point the same\n"
	.string "way — the old ruins, north of\l"
	.string "COASTAL ROUTE 1.\p"
	.string "Please. Go and look. I'd go myself,\n"
	.string "but somebody has to watch the\l"
	.string "readings.\p"
	.string "Be careful, {PLAYER}.$"

SollisLab_Text_SollisWardenTalk:
	.string "SOLLIS: A wild RALTS, terrified.\n"
	.string "A flash of light. And the floor\l"
	.string "tile still warm.\p"
	.string "…Sit down, {PLAYER}. There's a\n"
	.string "word you need to learn.\p"
	.string "WARDEN.\p"
	.string "It's what your parent was. Not a\n"
	.string "sailor, not a fisher — a WARDEN.\p"
	.string "This archipelago is held together\n"
	.string "by something older than any of us.\l"
	.string "A network of seals, scattered\l"
	.string "across the islands.\p"
	.string "The Wardens kept them. Quietly.\n"
	.string "For generations.\p"
	.string "And three weeks after we bury the\n"
	.string "last one, the ruins on this island\l"
	.string "wake up screaming.\p"
	.string "That is not a coincidence.\p"
	.string "Word came this morning from\n"
	.string "IRONHOLD — the same disturbance,\l"
	.string "ten times the size. And the\l"
	.string "Covenant has locked the island down.\p"
	.string "Someone has to go. Someone the\n"
	.string "seals will answer to.\p"
	.string "I think you know who that is.$"

SollisLab_Text_SollisSloop:
	.string "SOLLIS: That chart was the WARDEN's.\n"
	.string "With it, the TENNYSON will hold a\l"
	.string "course through open water.\p"
	.string "I've asked the harbor hands to rig\n"
	.string "her as a sloop. She's small, but\l"
	.string "she'll make IRONHOLD.\p"
	.string "Your parent's boat, your parent's\n"
	.string "sea chart, your parent's work.\p"
	.string "…Forgive me. It should never have\n"
	.string "landed on you all at once like this.$"

SollisLab_Text_SollisIronholdReminder:
	.string "SOLLIS: The TENNYSON is rigged and\n"
	.string "waiting at the harbor.\p"
	.string "IRONHOLD is occupied ground, so\n"
	.string "keep your head down and your\l"
	.string "partner close.\p"
	.string "And {PLAYER} — read the journal.\n"
	.string "Even the parts that look like\l"
	.string "nonsense.\p"
	.string "ESPECIALLY those.$"

SollisLab_Text_CassWaiting:
	.string "CASS: I already know which one\n"
	.string "I want.\p"
	.string "…Okay, no, I don't. They're all\n"
	.string "perfect. This is the hardest thing\l"
	.string "I've ever done.\p"
	.string "You pick first. I want to see what\n"
	.string "fate leaves me.$"

SollisLab_Text_TotodileFlavor:
	.string "The TOTODILE charges over before\n"
	.string "you take two steps!\p"
	.string "It skids to a stop at your feet,\n"
	.string "jaws wide in something that is\l"
	.string "either a grin or a threat.\p"
	.string "Probably a grin.$"

SollisLab_Text_TotodileOffer:
	.string "It's not leaving.\p"
	.string "Take TOTODILE as your partner?$"

SollisLab_Text_ReceivedTotodile:
	.string "{PLAYER} received TOTODILE!$"

SollisLab_Text_TotodileLeftover:
	.string "TOTODILE patrols the doorway with\n"
	.string "tremendous self-importance.\p"
	.string "SOLLIS: It's decided it guards the\n"
	.string "lab now. I've stopped arguing.$"

SollisLab_Text_ChimcharFlavor:
	.string "The CHIMCHAR peers down at you\n"
	.string "from the highest bookshelf.\p"
	.string "It drops a single sunflower seed\n"
	.string "on your head. A test, clearly.\p"
	.string "It seems to approve of how you\n"
	.string "took it.$"

SollisLab_Text_ChimcharOffer:
	.string "It's leaning so far off the shelf\n"
	.string "it's nearly falling.\p"
	.string "Take CHIMCHAR as your partner?$"

SollisLab_Text_ReceivedChimchar:
	.string "{PLAYER} received CHIMCHAR!$"

SollisLab_Text_ChimcharLeftover:
	.string "CHIMCHAR has reorganized the top\n"
	.string "shelf into a nest of field notes.\p"
	.string "SOLLIS: Those were alphabetized.\n"
	.string "…Were.$"

SollisLab_Text_RowletFlavor:
	.string "The ROWLET is sitting on an old\n"
	.string "leather journal, perfectly round,\l"
	.string "perfectly still.\p"
	.string "It swivels its head all the way\n"
	.string "around to study you.\p"
	.string "Then it bows. Slowly. Like a tiny\n"
	.string "feathered butler.$"

SollisLab_Text_RowletOffer:
	.string "It has already closed its eyes,\n"
	.string "as if the matter is settled.\p"
	.string "Take ROWLET as your partner?$"

SollisLab_Text_ReceivedRowlet:
	.string "{PLAYER} received ROWLET!$"

SollisLab_Text_RowletLeftover:
	.string "ROWLET has relocated to the desk\n"
	.string "lamp and become spherical.\p"
	.string "SOLLIS: It does that when it's\n"
	.string "content. Or judging you. The two\l"
	.string "are indistinguishable.$"

SollisLab_Text_GiveNickname:
	.string "SOLLIS: Going to give it a nickname?$"

SollisLab_Text_DeclineStarter:
	.string "It tilts its head, unbothered.\p"
	.string "SOLLIS: No rush. They've waited\n"
	.string "six months. They can wait six\l"
	.string "more minutes.$"

SollisLab_Text_CassTakesChimchar:
	.string "CASS: TOTODILE?! That's the one\n"
	.string "I— okay. Okay! Fine!\p"
	.string "The CHIMCHAR's been eyeing me\n"
	.string "anyway. We have an understanding.\l"
	.string "A fiery, chaotic understanding.\p"
	.string "SOLLIS: I'll bring it down to your\n"
	.string "house tonight, Cass. With gloves.\p"
	.string "CASS: {PLAYER} — first battle, the\n"
	.string "moment we're both ready. Deal?\l"
	.string "Deal. You can't undeal it now!$"

SollisLab_Text_CassTakesRowlet:
	.string "CASS: The CHIMCHAR? Bold choice.\n"
	.string "Spicy choice.\p"
	.string "Then I call the ROWLET! Look at it.\n"
	.string "It's a tiny round gentleman and\l"
	.string "we are going to be unstoppable.\p"
	.string "SOLLIS: I'll bring it to your house\n"
	.string "tonight, Cass. If it agrees to wake up.\p"
	.string "CASS: {PLAYER} — first battle, the\n"
	.string "moment we're both ready. Deal?\l"
	.string "Deal. You can't undeal it now!$"

SollisLab_Text_CassTakesTotodile:
	.string "CASS: The ROWLET, huh… yeah.\n"
	.string "Yeah, it picked you ages ago,\l"
	.string "honestly.\p"
	.string "Which means the TOTODILE is MINE!\n"
	.string "The chompy one! YES!\p"
	.string "SOLLIS: I'll bring it to your house\n"
	.string "tonight, Cass. Count your fingers\l"
	.string "after every meal.\p"
	.string "CASS: {PLAYER} — first battle, the\n"
	.string "moment we're both ready. Deal?\l"
	.string "Deal. You can't undeal it now!$"

SollisLab_Text_SollisJournal:
	.string "SOLLIS: Before you go.\p"
	.string "There's something here that\n"
	.string "belongs to you.\p"
	.string "The WARDEN left it in my keeping.\n"
	.string "“For when the kid has a partner,”\l"
	.string "were the exact words.\p"
	.string "I don't think your parent meant a\n"
	.string "boat.$"

SollisLab_Text_SollisAfterJournal:
	.string "SOLLIS: Most of it reads like an\n"
	.string "ordinary logbook. Weather, tides,\l"
	.string "repairs.\p"
	.string "But some pages are written in\n"
	.string "cipher. I never found the key,\l"
	.string "and I did look. Forgive me.\p"
	.string "Whatever the WARDEN was protecting,\n"
	.string "it's in those pages.\p"
	.string "…Go home before you set out.\n"
	.string "Your mother left something for you\l"
	.string "on the PC, I'm told.\p"
	.string "And take the long way. The island\n"
	.string "is beautiful this time of year.$"

SollisLab_Text_DeskBefore:
	.string "The desk is buried in tide charts\n"
	.string "and pinned specimens.\p"
	.string "An old leather journal lies in the\n"
	.string "middle, worn smooth at the corners.$"

SollisLab_Text_DeskAfter:
	.string "Tide charts, pinned specimens, and\n"
	.string "a clean rectangle in the dust where\l"
	.string "the WARDEN's journal used to sit.$"
