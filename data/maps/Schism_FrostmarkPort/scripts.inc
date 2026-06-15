Schism_FrostmarkPort_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Schism_FrostmarkPort_OnFrame
	.byte 0

@ Arrival fires once, on the first frame the player stands on this map with
@ VAR_SCHISM_PROGRESS == 0 (the moment they dock). ON_FRAME instead of coord
@ triggers: the boat warps the player onto the pier and the dock is fully
@ walkable, so a fixed dock-head trigger would be bypassable.
Schism_FrostmarkPort_OnFrame:
	map_script_2 VAR_SCHISM_PROGRESS, 0, SchismFrostmarkPort_EventScript_Arrival
	.2byte 0

@ -------------------------------------------------------------------
@ Dual arrival - NORTH port. An ice-faction soldier marks the new
@ arrival but does not block. Camera pans up the frozen hills.
@ Sets FLAG_SCHISM_ARRIVED, VAR_SCHISM_PROGRESS = 1. The SOUTH port
@ (Venomquay) runs an identical-purpose arrival; either sets the same
@ flag/var, so the island opens from whichever side the player picks.
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_Arrival::
	lockall
	delay 20
	msgbox SchismFrostmarkPort_Text_ArrivalSnow, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_SCHISM_FROSTMARK_SOLDIER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Soldier
	msgbox SchismFrostmarkPort_Text_ArrivalSoldier, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SchismFrostmarkPort_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SchismFrostmarkPort_Text_ArrivalView, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SchismFrostmarkPort_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setflag FLAG_SCHISM_ARRIVED
	setvar VAR_SCHISM_PROGRESS, 1
	releaseall
	end

SchismFrostmarkPort_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SchismFrostmarkPort_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Ice faction soldier - watchful, not hostile.
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_IceSoldier::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Soldier
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrostmarkPort_EventScript_IceSoldierCeasefire
	msgbox SchismFrostmarkPort_Text_IceSoldier, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPort_EventScript_IceSoldierCeasefire::
	msgbox SchismFrostmarkPort_Text_IceSoldierCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Sailor - explains the split history briefly.
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_Sailor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sailor
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrostmarkPort_EventScript_SailorCeasefire
	msgbox SchismFrostmarkPort_Text_Sailor, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPort_EventScript_SailorCeasefire::
	msgbox SchismFrostmarkPort_Text_SailorCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Merchant - cold-resistance gear. (Same person runs the Venomquay
@ shop in a different coat - paid off post-ceasefire.)
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismFrostmarkPort_EventScript_MerchantCeasefire
	message SchismFrostmarkPort_Text_Merchant
	waitmessage
	pokemart SchismFrostmarkPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox SchismFrostmarkPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

SchismFrostmarkPort_EventScript_MerchantCeasefire::
	message SchismFrostmarkPort_Text_MerchantCeasefire
	waitmessage
	pokemart SchismFrostmarkPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox SchismFrostmarkPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SchismFrostmarkPort_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_ASPEAR_BERRY
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_TerritorySign::
	msgbox SchismFrostmarkPort_Text_TerritorySign, MSGBOX_SIGN
	end

SchismFrostmarkPort_EventScript_BerthSign::
	msgbox SchismFrostmarkPort_Text_BerthSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ The Tennyson, moored at the ice-rimed berth. NORTH island id.
@ -------------------------------------------------------------------
SchismFrostmarkPort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM
	msgbox SchismFrostmarkPort_Text_TennysonSail, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismFrostmarkPort_Text_ArrivalSnow:
	.string "Snow falls steadily over the\n"
	.string "docks, muffling everything.\p"
	.string "The water along the pilings has\n"
	.string "frozen into grey, ridged ice.\p"
	.string "Soldiers in white coats watch\n"
	.string "the TENNYSON come in, and do not\l"
	.string "wave.$"

SchismFrostmarkPort_Text_ArrivalSoldier:
	.string "Outsider. Galleon flag.\p"
	.string "You're a long way from anywhere\n"
	.string "that flies one of those.\p"
	.string "This is FROSTMARK. Ice territory.\n"
	.string "Keep to the north and we'll have\l"
	.string "no trouble.$"

SchismFrostmarkPort_Text_ArrivalView:
	.string "Past the soldiers, the land\n"
	.string "climbs into white hills under a\l"
	.string "low, iron sky.\p"
	.string "Somewhere beyond them - the old\n"
	.string "charts say - the island simply\l"
	.string "stops being winter.\p"
	.string "Nobody here will tell you what's\n"
	.string "on the other side.$"

SchismFrostmarkPort_Text_IceSoldier:
	.string "We don't get visitors. We get\n"
	.string "spies.\p"
	.string "I'll assume you're a visitor\n"
	.string "until you give me a reason not\l"
	.string "to.$"

SchismFrostmarkPort_Text_IceSoldierCeasefire:
	.string "The order came down to stop\n"
	.string "patrolling the Scar road.\p"
	.string "Twenty years we held that line.\n"
	.string "Now we just… don't.\p"
	.string "Feels like missing a tooth with\n"
	.string "my tongue. Can't stop checking.$"

SchismFrostmarkPort_Text_Sailor:
	.string "Split island. Has been since\n"
	.string "before I was born.\p"
	.string "Used to be one port - or so the\n"
	.string "old charts say.\p"
	.string "Now there's two, and neither\n"
	.string "talks to the other.\p"
	.string "I do deliveries to both. Don't\n"
	.string "tell either.$"

SchismFrostmarkPort_Text_SailorCeasefire:
	.string "Word is the two commanders stood\n"
	.string "in the Scar and looked at the\l"
	.string "same pile of old stones.\p"
	.string "Didn't shake hands. Didn't need\n"
	.string "to, I suppose.\p"
	.string "My deliveries just got a lot\n"
	.string "less complicated.$"

SchismFrostmarkPort_Text_Merchant:
	.string "Cold-resistance gear. Best\n"
	.string "investment you'll make here.\p"
	.string "The south side has the same shop\n"
	.string "selling antidotes.\p"
	.string "I know because I'm the same\n"
	.string "person. Different coat.\p"
	.string "Don't tell EIRA.$"

SchismFrostmarkPort_Text_MerchantCeasefire:
	.string "I can use one coat now.\p"
	.string "Do you have any idea how much\n"
	.string "that saves me? Coats are\l"
	.string "expensive. Two coats, two of\l"
	.string "everything.\p"
	.string "Whatever you did up at the Scar,\n"
	.string "you did it for my wardrobe.$"

SchismFrostmarkPort_Text_MerchantFarewell:
	.string "Stay warm. Or don't. I sell\n"
	.string "remedies for both.$"

SchismFrostmarkPort_Text_TerritorySign:
	.string "FROSTMARK - ICE TERRITORY.\p"
	.string "VENOMQUAY access prohibited.\n"
	.string "By order of FROST COMMANDER\l"
	.string "EIRA.$"

SchismFrostmarkPort_Text_BerthSign:
	.string "VISITOR BERTH\p"
	.string "Keep your lines clear of the\n"
	.string "ice. The harbor does not thaw\l"
	.string "your mistakes.$"

SchismFrostmarkPort_Text_TennysonSail:
	.string "The TENNYSON rides at the\n"
	.string "ice-rimed berth, her decks\l"
	.string "dusted with snow.$"
