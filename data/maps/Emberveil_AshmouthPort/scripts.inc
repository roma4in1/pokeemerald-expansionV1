Emberveil_AshmouthPort_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Emberveil_AshmouthPort_OnFrame
	.byte 0

@ Arrival fires exactly once, on the first frame the player stands on this
@ map with VAR_EMBERVEIL_PROGRESS == 0 (i.e. the moment they dock). ON_FRAME
@ instead of coord triggers: the west half of the port is fully walkable, so
@ dock-head triggers were bypassable, and the future boat-transition warp
@ position is not yet fixed.
Emberveil_AshmouthPort_OnFrame:
	map_script_2 VAR_EMBERVEIL_PROGRESS, 0, EmberveilAshmouthPort_EventScript_Arrival
	.2byte 0

@ -------------------------------------------------------------------
@ Arrival cutscene. The cult greeter notices the new boat and comes
@ down to the dock - friendly, not blocking. Camera pans up toward
@ the volcano. Sets FLAG_EMBERVEIL_ARRIVED, VAR_EMBERVEIL_PROGRESS = 1.
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_Arrival::
	lockall
	delay 20
	msgbox EmberveilAshmouthPort_Text_ArrivalAsh, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_EMBERVEIL_CULT_GREETER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_EMBERVEIL_CULT_GREETER, EmberveilAshmouthPort_Movement_GreeterApproach
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Devotee
	msgbox EmberveilAshmouthPort_Text_ArrivalGreeting, MSGBOX_DEFAULT
	closemessage
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, EmberveilAshmouthPort_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox EmberveilAshmouthPort_Text_ArrivalVolcanoView, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, EmberveilAshmouthPort_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setspeaker Pelagios_Speaker_Devotee
	msgbox EmberveilAshmouthPort_Text_ArrivalGreeterCloses, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_EMBERVEIL_CULT_GREETER, EmberveilAshmouthPort_Movement_GreeterReturn
	waitmovement 0
	setflag FLAG_EMBERVEIL_ARRIVED
	setvar VAR_EMBERVEIL_PROGRESS, 1
	releaseall
	end

EmberveilAshmouthPort_Movement_GreeterApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	face_down
	step_end

EmberveilAshmouthPort_Movement_GreeterReturn:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

EmberveilAshmouthPort_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

EmberveilAshmouthPort_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Cult greeter - earnest, welcoming. Post-resolution variants differ
@ sharply between Path A (community fracturing) and Path B (intact).
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_CultGreeter::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Devotee
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPort_EventScript_GreeterResolved
	msgbox EmberveilAshmouthPort_Text_Greeter, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPort_EventScript_GreeterResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilAshmouthPort_EventScript_GreeterPathB
	msgbox EmberveilAshmouthPort_Text_GreeterPathA, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPort_EventScript_GreeterPathB::
	msgbox EmberveilAshmouthPort_Text_GreeterPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Sailor - three weeks in port, watching the ash thicken
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_Sailor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sailor
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPort_EventScript_SailorResolved
	msgbox EmberveilAshmouthPort_Text_Sailor, MSGBOX_DEFAULT
	release
	end

EmberveilAshmouthPort_EventScript_SailorResolved::
	msgbox EmberveilAshmouthPort_Text_SailorResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Merchant - fire-resistance supplies, local charcoal
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message EmberveilAshmouthPort_Text_MerchantGreeting
	waitmessage
	pokemart EmberveilAshmouthPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox EmberveilAshmouthPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
EmberveilAshmouthPort_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_RAWST_BERRY
	.2byte ITEM_LUM_BERRY
	.2byte ITEM_CHARCOAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

@ -------------------------------------------------------------------
@ The Tennyson, moored at the singed pier. Sail back to Haven Isle.
@ Post-resolution: galleon refit text only when BOTH Sirocco and
@ Emberveil are resolved (VAR_BOAT_TIER 3 is set in the SealChamber).
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_EMBERVEIL
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilAshmouthPort_EventScript_TennysonResolved
	msgbox EmberveilAshmouthPort_Text_TennysonSail, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

EmberveilAshmouthPort_EventScript_TennysonResolved::
	goto_if_set FLAG_SIROCCO_RESOLVED, EmberveilAshmouthPort_EventScript_TennysonGalleon
	msgbox EmberveilAshmouthPort_Text_TennysonSirocco, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

EmberveilAshmouthPort_EventScript_TennysonGalleon::
	msgbox EmberveilAshmouthPort_Text_TennysonGalleon, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
EmberveilAshmouthPort_EventScript_PortSign::
	msgbox EmberveilAshmouthPort_Text_PortSign, MSGBOX_SIGN
	end

EmberveilAshmouthPort_EventScript_BerthSign::
	msgbox EmberveilAshmouthPort_Text_BerthSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilAshmouthPort_Text_ArrivalAsh:
	.string "Ash drifts down over the harbor\n"
	.string "like grey snow.\p"
	.string "Everything here is lit from\n"
	.string "behind - a warm orange glow that\l"
	.string "doesn't come from the sun.$"

EmberveilAshmouthPort_Text_ArrivalGreeting:
	.string "Welcome to EMBERVEIL. PYRATH's\n"
	.string "warmth be with you.\p"
	.string "The HIGH EMBER's teachings have\n"
	.string "sustained this island for two\l"
	.string "centuries. We hope you'll feel\l"
	.string "that warmth during your visit.$"

EmberveilAshmouthPort_Text_ArrivalVolcanoView:
	.string "Above the port, the volcano fills\n"
	.string "half the sky.\p"
	.string "A thin line of smoke climbs from\n"
	.string "the summit, steady as breathing.\p"
	.string "Somewhere under all that stone,\n"
	.string "something is keeping the light on.$"

EmberveilAshmouthPort_Text_ArrivalGreeterCloses:
	.string "The inn is just there if the\n"
	.string "crossing tired you.\p"
	.string "The mountain road starts at the\n"
	.string "north end of the port.\p"
	.string "Walk gently. The island is\n"
	.string "louder than usual these days.$"

EmberveilAshmouthPort_Text_Greeter:
	.string "PYRATH's warmth be with you.\p"
	.string "Visitors think we say that the\n"
	.string "way other islands say hello.\p"
	.string "We don't. We mean it. The warmth\n"
	.string "is the whole reason anyone can\l"
	.string "live here at all.$"

EmberveilAshmouthPort_Text_GreeterPathA:
	.string "PYRATH's warmth be…\p"
	.string "…I'm sorry. The words come out\n"
	.string "by habit.\p"
	.string "Half my friends won't say them\n"
	.string "anymore. The other half won't\l"
	.string "say anything else.\p"
	.string "I don't know which half I'm in\n"
	.string "yet.$"

EmberveilAshmouthPort_Text_GreeterPathB:
	.string "PYRATH's warmth be with you!\p"
	.string "It means something different now.\n"
	.string "Better, I think.\p"
	.string "We weren't calling out to PYRATH\n"
	.string "all those years. We were keeping\l"
	.string "a promise we didn't know we'd\l"
	.string "made.\p"
	.string "The HIGH EMBER says that's what\n"
	.string "faith was the whole time.$"

EmberveilAshmouthPort_Text_Sailor:
	.string "She's been rumbling more than\n"
	.string "usual. The cult says it's PYRATH\l"
	.string "stirring. I say it's a problem.\p"
	.string "I've been docked here three weeks\n"
	.string "and the ash is twice as thick as\l"
	.string "when I arrived.\p"
	.string "Nobody else seems to be doing\n"
	.string "the math.$"

EmberveilAshmouthPort_Text_SailorResolved:
	.string "Rumbling's stopped.\p"
	.string "Three weeks of grinding under the\n"
	.string "waterline, and then one morning -\l"
	.string "quiet.\p"
	.string "I don't know what you did up\n"
	.string "there, and I've been at sea long\l"
	.string "enough not to ask.$"

EmberveilAshmouthPort_Text_MerchantGreeting:
	.string "Fire-resistance supplies. Best\n"
	.string "stock on any island - we need\l"
	.string "them.\p"
	.string "RAWST BERRIES grow wild here.\n"
	.string "CHARCOAL's a local product.\p"
	.string "We do good business as long as\n"
	.string "the mountain stays where it is.$"

EmberveilAshmouthPort_Text_MerchantFarewell:
	.string "Mind the ash on the road north.\n"
	.string "It gets in everything.$"

EmberveilAshmouthPort_Text_TennysonSail:
	.string "The TENNYSON rides at the singed\n"
	.string "pier, her rigging dusted grey\l"
	.string "with ash.$"

EmberveilAshmouthPort_Text_TennysonSirocco:
	.string "The TENNYSON waits, patient under\n"
	.string "the falling ash.\p"
	.string "EMBERVEIL's seal is steady now -\n"
	.string "but somewhere across the strait,\l"
	.string "SIROCCO's sands are still\l"
	.string "restless.$"

EmberveilAshmouthPort_Text_TennysonGalleon:
	.string "The TENNYSON sits tall at the\n"
	.string "pier under full GALLEON rigging -\l"
	.string "the shipwrights' thanks from two\l"
	.string "quieted islands.\p"
	.string "The deep southern waters are open\n"
	.string "to you now.$"

EmberveilAshmouthPort_Text_PortSign:
	.string "ASHMOUTH PORT\p"
	.string "“Gateway to EMBERVEIL -\n"
	.string "PYRATH's warmth be with all\l"
	.string "who land here.”$"

EmberveilAshmouthPort_Text_BerthSign:
	.string "VISITOR BERTH\p"
	.string "Sweep ash from your deck daily.\n"
	.string "The harbormaster is not\l"
	.string "responsible for buried cargo.$"
