Ironhold_GatemarkPort_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Arrival cutscene - fires once, when VAR_IRONHOLD_PROGRESS == 0.
@ The dock guard marches down from the checkpoint, demands the
@ Navigator's Log, and (if the player carries it) waves them through
@ with visible discomfort. Camera tilts up toward the summit fortress.
@ Sets FLAG_IRONHOLD_ARRIVED and VAR_IRONHOLD_PROGRESS = 1.
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_Arrival::
	lockall
	delay 16
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_IRONHOLD_DOCK_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_IRONHOLD_DOCK_GUARD, IronholdGatemarkPort_Movement_GuardApproach
	waitmovement 0
	setspeaker Pelagios_Speaker_Guard
	msgbox IronholdGatemarkPort_Text_ArrivalHalt, MSGBOX_DEFAULT
	checkitem ITEM_NAVIGATORS_LOG
	goto_if_eq VAR_RESULT, FALSE, IronholdGatemarkPort_EventScript_ArrivalNoLog
	setspeaker 0
	msgbox IronholdGatemarkPort_Text_ArrivalLogCheck, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Guard
	msgbox IronholdGatemarkPort_Text_ArrivalLogPass, MSGBOX_DEFAULT
	setspeaker 0
	msgbox IronholdGatemarkPort_Text_ArrivalLogReturned, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IRONHOLD_DOCK_GUARD, IronholdGatemarkPort_Movement_GuardReturn
	waitmovement 0
	setflag FLAG_IRONHOLD_ARRIVED
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, IronholdGatemarkPort_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox IronholdGatemarkPort_Text_ArrivalSummitView, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, IronholdGatemarkPort_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setvar VAR_IRONHOLD_PROGRESS, 1
	releaseall
	end

IronholdGatemarkPort_EventScript_ArrivalNoLog::
	@ Speaker is still the guard (set before the checkitem branch).
	msgbox IronholdGatemarkPort_Text_ArrivalNoLog, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, IronholdGatemarkPort_Movement_PlayerTurnedBack
	waitmovement 0
	applymovement LOCALID_IRONHOLD_DOCK_GUARD, IronholdGatemarkPort_Movement_GuardReturn
	waitmovement 0
	releaseall
	end

IronholdGatemarkPort_Movement_GuardApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

IronholdGatemarkPort_Movement_GuardReturn:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

IronholdGatemarkPort_Movement_PlayerTurnedBack:
	walk_down
	step_end

IronholdGatemarkPort_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

IronholdGatemarkPort_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Dock guard (checkpoint NPC, post-arrival flavor)
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_DockGuard::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Guard
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdGatemarkPort_EventScript_DockGuardResolved
	msgbox IronholdGatemarkPort_Text_DockGuard, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPort_EventScript_DockGuardResolved::
	msgbox IronholdGatemarkPort_Text_DockGuardResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Sailor who recognizes the Tennyson - knew the Warden
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_Sailor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sailor
	msgbox IronholdGatemarkPort_Text_Sailor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, IronholdGatemarkPort_EventScript_SailorNotAlone
	msgbox IronholdGatemarkPort_Text_SailorAlone, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPort_EventScript_SailorNotAlone::
	msgbox IronholdGatemarkPort_Text_SailorNotAlone, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Dockside merchant - basic supplies
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message IronholdGatemarkPort_Text_MerchantGreeting
	waitmessage
	pokemart IronholdGatemarkPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox IronholdGatemarkPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
IronholdGatemarkPort_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

@ -------------------------------------------------------------------
@ Resistance contact disguised as a fisherman.
@ Bland small talk until Gym 1 is cleared, then points the player at
@ the watched house in IronholdCity. Brief's "speak to the fisherman"
@ step is advisory: the hideout itself opens on FLAG_IRONHOLD_GYM1_CLEAR.
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_DisguisedFisherman::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Fisherman
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdGatemarkPort_EventScript_FishermanResolved
	goto_if_set FLAG_IRONHOLD_RESISTANCE_MET, IronholdGatemarkPort_EventScript_FishermanMet
	goto_if_set FLAG_IRONHOLD_GYM1_CLEAR, IronholdGatemarkPort_EventScript_FishermanHint
	msgbox IronholdGatemarkPort_Text_FishermanBland, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPort_EventScript_FishermanHint::
	msgbox IronholdGatemarkPort_Text_FishermanHint, MSGBOX_DEFAULT
	setspeaker 0
	msgbox IronholdGatemarkPort_Text_FishermanHintAside, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Fisherman
	msgbox IronholdGatemarkPort_Text_FishermanHintHouse, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPort_EventScript_FishermanMet::
	msgbox IronholdGatemarkPort_Text_FishermanMet, MSGBOX_DEFAULT
	release
	end

IronholdGatemarkPort_EventScript_FishermanResolved::
	msgbox IronholdGatemarkPort_Text_FishermanResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ The Tennyson, moored at the visitor berth.
@ Sailing back to Haven Isle is available once docked here. After
@ FLAG_IRONHOLD_RESOLVED the brigantine rigging text plays (next
@ islands gated until their chapters exist).
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_IRONHOLD
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdGatemarkPort_EventScript_TennysonBrigantine
	goto Pelagios_EventScript_BoardTennyson

IronholdGatemarkPort_EventScript_TennysonBrigantine::
	msgbox IronholdGatemarkPort_Text_TennysonBrigantine, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
IronholdGatemarkPort_EventScript_GarrisonSign::
	msgbox IronholdGatemarkPort_Text_GarrisonSign, MSGBOX_SIGN
	end

IronholdGatemarkPort_EventScript_BerthSign::
	msgbox IronholdGatemarkPort_Text_BerthSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdGatemarkPort_Text_ArrivalHalt:
	.string "Halt.\p"
	.string "This is a Covenant checkpoint.\n"
	.string "All arriving vessels are subject\l"
	.string "to documentation review.\p"
	.string "Sailing papers. Now.$"

IronholdGatemarkPort_Text_ArrivalNoLog:
	.string "No sailing papers, no entry.\n"
	.string "Covenant law.\p"
	.string "Back to your boat.$"

IronholdGatemarkPort_Text_ArrivalLogCheck:
	.string "You hand over the NAVIGATOR'S LOG.\p"
	.string "The guard flips it open with\n"
	.string "practiced boredom-\p"
	.string "-and stops.\p"
	.string "He reads the name on the inside\n"
	.string "cover twice. His thumb hovers\l"
	.string "over the registry stamp without\l"
	.string "landing.$"

IronholdGatemarkPort_Text_ArrivalLogPass:
	.string "…Warden's log. You may pass.$"

IronholdGatemarkPort_Text_ArrivalLogReturned:
	.string "He hands it back without meeting\n"
	.string "your eyes.$"

IronholdGatemarkPort_Text_ArrivalSummitView:
	.string "Above the grey roofs of GATEMARK\n"
	.string "PORT, the mountain climbs into\l"
	.string "low cloud.\p"
	.string "At its peak: a fortress, black\n"
	.string "against the sky, Covenant banners\l"
	.string "snapping from every tower.\p"
	.string "And behind the stonework - faint,\n"
	.string "rhythmic - something glows.$"

IronholdGatemarkPort_Text_DockGuard:
	.string "Move along. Loitering on the dock\n"
	.string "is a category two infraction.\p"
	.string "…Everything is a category two\n"
	.string "infraction.$"

IronholdGatemarkPort_Text_DockGuardResolved:
	.string "The summit's gone dark.\p"
	.string "Command says it's scheduled\n"
	.string "maintenance.\p"
	.string "Command says a lot of things.$"

IronholdGatemarkPort_Text_Sailor:
	.string "That boat.\p"
	.string "I haven't seen her in port since…\n"
	.string "well.\p"
	.string "Your parent kept her in good\n"
	.string "shape. You sailing her alone?$"

IronholdGatemarkPort_Text_SailorAlone:
	.string "Good. She's a strong vessel.\p"
	.string "IRONHOLD's a harder harbor than\n"
	.string "HAVEN ISLE. Keep your eyes open.$"

IronholdGatemarkPort_Text_SailorNotAlone:
	.string "Hah. Your POKéMON count as crew.\n"
	.string "Better crew than most, honestly.\p"
	.string "Still. Watch yourself here.\n"
	.string "This isn't HAVEN ISLE.$"

IronholdGatemarkPort_Text_MerchantGreeting:
	.string "Supplies? I've got what the\n"
	.string "Covenant lets me stock.$"

IronholdGatemarkPort_Text_MerchantFarewell:
	.string "Mind the checkpoint fees.\n"
	.string "They went up again last month.$"

IronholdGatemarkPort_Text_FishermanBland:
	.string "Nothing's biting.\p"
	.string "Nothing ever bites this close to\n"
	.string "the patrol lanes. Too many hulls.\p"
	.string "…You get used to being watched.\n"
	.string "That's the worst part.$"

IronholdGatemarkPort_Text_FishermanHint:
	.string "…You're the one who took the\n"
	.string "town hall match off PETRA.$"

IronholdGatemarkPort_Text_FishermanHintAside:
	.string "He keeps his voice low, eyes on\n"
	.string "his line.$"

IronholdGatemarkPort_Text_FishermanHintHouse:
	.string "There's a house in the city.\n"
	.string "South side, under the HQ placard.\l"
	.string "The soldier they kept posted\l"
	.string "outside just got rotated north.\p"
	.string "Some friends of mine would like\n"
	.string "a word. Downstairs.$"

IronholdGatemarkPort_Text_FishermanMet:
	.string "You found them, then.\p"
	.string "Whatever they showed you - don't\n"
	.string "carry it like it's heavy.\l"
	.string "People notice heavy.$"

IronholdGatemarkPort_Text_FishermanResolved:
	.string "The glow's gone from the summit.\p"
	.string "First time in months I've watched\n"
	.string "the mountain and felt nothing\l"
	.string "looking back.\p"
	.string "…The WARDEN would've liked you,\n"
	.string "I think. I mean that.$"

IronholdGatemarkPort_Text_TennysonBrigantine:
	.string "The TENNYSON rides proud under\n"
	.string "her new brigantine rigging - the\l"
	.string "dockworkers refit her overnight\l"
	.string "and wouldn't take payment.\p"
	.string "The open straits beyond IRONHOLD\n"
	.string "are yours to cross now.$"

IronholdGatemarkPort_Text_GarrisonSign:
	.string "NORTH GATE\p"
	.string "IRONHOLD CITY - COVENANT\n"
	.string "GARRISON HQ\p"
	.string "All travelers subject to\n"
	.string "inspection. No exceptions.$"

IronholdGatemarkPort_Text_BerthSign:
	.string "VISITOR BERTH 3\p"
	.string "Mooring by Covenant permit only.\p"
	.string "(Someone has scratched a tiny\n"
	.string "sailboat into the corner of the\l"
	.string "sign. It looks old.)$"
