HavenIsle_TidemarkVillage_MapScripts::
	.byte 0

@ Pattern mirrors the vanilla rival intercepts (lockall; every applymovement
@ followed by waitmovement 0; no dialogue until all movement has finished).
TidemarkVillage_EventScript_CassIntercept::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, TidemarkVillage_Movement_PlayerStepOut
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_TIDEMARK_CASS, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_TIDEMARK_CASS, TidemarkVillage_Movement_CassApproach
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	setspeaker Pelagios_Speaker_Cass
	msgbox TidemarkVillage_Text_CassIntercept, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TIDEMARK_CASS, TidemarkVillage_Movement_CassRunToLab
	waitmovement 0
	removeobject LOCALID_TIDEMARK_CASS
	setflag FLAG_HIDE_TIDEMARK_CASS_INTRO
	setvar VAR_PELAGIOS_INTRO_STATE, 3
	releaseall
	end

TidemarkVillage_EventScript_Cass::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Cass
	msgbox TidemarkVillage_Text_CassIntercept, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TIDEMARK_CASS, TidemarkVillage_Movement_CassRunToLab
	waitmovement 0
	removeobject LOCALID_TIDEMARK_CASS
	setflag FLAG_HIDE_TIDEMARK_CASS_INTRO
	setvar VAR_PELAGIOS_INTRO_STATE, 3
	release
	end

TidemarkVillage_Movement_PlayerStepOut:
	walk_down
	walk_down
	step_end

@ Cass starts at (8,10); two steps west puts her at (6,10), facing the
@ player at (5,10).
TidemarkVillage_Movement_CassApproach:
	walk_left
	walk_left
	face_left
	step_end

@ From (6,10): east along the open row to x19, then up to the lab doormat.
TidemarkVillage_Movement_CassRunToLab:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	step_end

TidemarkVillage_EventScript_BlockNorth::
	lockall
	msgbox TidemarkVillage_Text_NoPokemonYet, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

TidemarkVillage_EventScript_BlockEast::
	lockall
	msgbox TidemarkVillage_Text_NoPokemonYet, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkLeft
	waitmovement 0
	releaseall
	end

TidemarkVillage_EventScript_OldWoman::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_OldWoman
	goto_if_set FLAG_STARTER_CHOSEN, TidemarkVillage_EventScript_OldWomanAfter
	msgbox TidemarkVillage_Text_OldWomanBefore, MSGBOX_DEFAULT
	release
	end

TidemarkVillage_EventScript_OldWomanAfter::
	msgbox TidemarkVillage_Text_OldWomanAfter, MSGBOX_DEFAULT
	release
	end

TidemarkVillage_EventScript_Child::
	setspeaker Pelagios_Speaker_Child
	msgbox TidemarkVillage_Text_Child, MSGBOX_NPC
	end

TidemarkVillage_EventScript_Sailor::
	setspeaker Pelagios_Speaker_Sailor
	msgbox TidemarkVillage_Text_Sailor, MSGBOX_NPC
	end

TidemarkVillage_EventScript_VillageSign::
	msgbox TidemarkVillage_Text_VillageSign, MSGBOX_SIGN
	end

TidemarkVillage_EventScript_LabSign::
	msgbox TidemarkVillage_Text_LabSign, MSGBOX_SIGN
	end

TidemarkVillage_Text_CassIntercept:
	.string "{PLAYER}! There you are!\p"
	.string "SOLLIS sent for you too, right?\n"
	.string "She sent for ME at sunrise. SUNRISE!\p"
	.string "You know what that means? It means\n"
	.string "it's important. Professor-important!\p"
	.string "…Why are you still standing there?\n"
	.string "Race you to the lab!$"

TidemarkVillage_Text_NoPokemonYet:
	.string "The grass past the village whispers\n"
	.string "and shifts. Something is moving in it.\p"
	.string "Better not wander out there without\n"
	.string "a POKéMON of your own.$"

TidemarkVillage_Text_OldWomanBefore:
	.string "Mind the sprouts, dear, they're\n"
	.string "shy this spring.\p"
	.string "…You're the WARDEN's child, aren't\n"
	.string "you. You have the same way of\l"
	.string "standing. Like the wind can't\l"
	.string "decide which way to push you.\p"
	.string "Your parent fixed my fence every\n"
	.string "autumn for twenty years.\p"
	.string "Never asked for a thing. Just\n"
	.string "watched the sea while working,\l"
	.string "like it owed an explanation.$"

TidemarkVillage_Text_OldWomanAfter:
	.string "A partner of your own now, is it?\p"
	.string "Good. The sea is kinder to those\n"
	.string "who don't face it alone.\p"
	.string "The WARDEN taught me that.$"

TidemarkVillage_Text_Child:
	.string "I ran around the whole village\n"
	.string "THREE times before breakfast!\p"
	.string "Tomorrow I'm doing FOUR, and then\n"
	.string "NOBODY can stop me!\p"
	.string "…Stop me from what? I dunno!\n"
	.string "Something!$"

TidemarkVillage_Text_Sailor:
	.string "See those sails, way out past the\n"
	.string "point? Covenant ships.\p"
	.string "Used to be you'd spot one a season.\n"
	.string "Lately it's one a week, sometimes\l"
	.string "two, always heading for Ironhold.\p"
	.string "Patrol routes, cargo manifests…\n"
	.string "none of my business, sure.\p"
	.string "But a sailor learns to read weather.\n"
	.string "And something's blowing in.$"

TidemarkVillage_Text_VillageSign:
	.string "TIDEMARK VILLAGE\p"
	.string "“Where the tide goes out,\n"
	.string "and always comes home.”$"

TidemarkVillage_Text_LabSign:
	.string "POKéMON RESEARCH LAB\n"
	.string "PROF. SOLLIS, RESIDENT NATURALIST\p"
	.string "“Knock if the door's shut.\n"
	.string "Mind the TOTODILE if it isn't.”$"
