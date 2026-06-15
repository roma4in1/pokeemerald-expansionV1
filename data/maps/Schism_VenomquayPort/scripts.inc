Schism_VenomquayPort_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Schism_VenomquayPort_OnFrame
	.byte 0

@ Dual arrival - SOUTH port. Mirror of Frostmark: ON_FRAME at
@ VAR_SCHISM_PROGRESS == 0, sets the SAME FLAG_SCHISM_ARRIVED and
@ VAR_SCHISM_PROGRESS = 1. Whichever port the player picks from the
@ boat menu opens the island.
Schism_VenomquayPort_OnFrame:
	map_script_2 VAR_SCHISM_PROGRESS, 0, SchismVenomquayPort_EventScript_Arrival
	.2byte 0

SchismVenomquayPort_EventScript_Arrival::
	lockall
	delay 20
	msgbox SchismVenomquayPort_Text_ArrivalRain, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_SCHISM_VENOMQUAY_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Guard
	msgbox SchismVenomquayPort_Text_ArrivalGuard, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SchismVenomquayPort_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SchismVenomquayPort_Text_ArrivalView, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SchismVenomquayPort_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setflag FLAG_SCHISM_ARRIVED
	setvar VAR_SCHISM_PROGRESS, 1
	releaseall
	end

SchismVenomquayPort_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SchismVenomquayPort_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Poison faction guard - suspicious of outsiders.
@ -------------------------------------------------------------------
SchismVenomquayPort_EventScript_PoisonGuard::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Guard
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismVenomquayPort_EventScript_PoisonGuardCeasefire
	msgbox SchismVenomquayPort_Text_PoisonGuard, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPort_EventScript_PoisonGuardCeasefire::
	msgbox SchismVenomquayPort_Text_PoisonGuardCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Fisherman - notices the fish are dying.
@ -------------------------------------------------------------------
SchismVenomquayPort_EventScript_Fisherman::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Fisherman
	goto_if_set FLAG_SCHISM_RESOLVED, SchismVenomquayPort_EventScript_FishermanResolved
	msgbox SchismVenomquayPort_Text_Fisherman, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPort_EventScript_FishermanResolved::
	msgbox SchismVenomquayPort_Text_FishermanResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Merchant - antidotes and remedies. (The Frostmark merchant's other
@ coat - the joke pays off post-ceasefire on both sides.)
@ -------------------------------------------------------------------
SchismVenomquayPort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismVenomquayPort_EventScript_MerchantCeasefire
	message SchismVenomquayPort_Text_Merchant
	waitmessage
	pokemart SchismVenomquayPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox SchismVenomquayPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

SchismVenomquayPort_EventScript_MerchantCeasefire::
	message SchismVenomquayPort_Text_MerchantCeasefire
	waitmessage
	pokemart SchismVenomquayPort_Pokemart
	setspeaker Pelagios_Speaker_Merchant
	msgbox SchismVenomquayPort_Text_MerchantFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SchismVenomquayPort_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PECHA_BERRY
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SchismVenomquayPort_EventScript_TerritorySign::
	msgbox SchismVenomquayPort_Text_TerritorySign, MSGBOX_SIGN
	end

SchismVenomquayPort_EventScript_BerthSign::
	msgbox SchismVenomquayPort_Text_BerthSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ The Tennyson, moored at the corroded berth. SOUTH island id.
@ -------------------------------------------------------------------
SchismVenomquayPort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_SCHISM_SOUTH
	msgbox SchismVenomquayPort_Text_TennysonSail, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SchismVenomquayPort_Text_ArrivalRain:
	.string "A thin, stinging rain falls over\n"
	.string "the harbor.\p"
	.string "The water here is green-black and\n"
	.string "still, and the docks are eaten\l"
	.string "soft at the edges with\l"
	.string "corrosion.\p"
	.string "Guards in dark oilskins watch the\n"
	.string "TENNYSON tie up, and do not\l"
	.string "wave.$"

SchismVenomquayPort_Text_ArrivalGuard:
	.string "Outsider. From the deep water.\p"
	.string "You picked a poor island to be\n"
	.string "curious about.\p"
	.string "This is VENOMQUAY. Poison\n"
	.string "territory. Stay south of the\l"
	.string "Scar and breathe shallow.$"

SchismVenomquayPort_Text_ArrivalView:
	.string "Past the guards, the land falls\n"
	.string "away into swamp - black water,\l"
	.string "dead trees, walkways slick with\l"
	.string "rain.\p"
	.string "Somewhere beyond it - the old\n"
	.string "charts say - the island simply\l"
	.string "stops being poisoned.\p"
	.string "Nobody here will tell you what's\n"
	.string "on the other side.$"

SchismVenomquayPort_Text_PoisonGuard:
	.string "We watch the water. We watch the\n"
	.string "road. We watch you.\p"
	.string "Nothing personal. Watching is\n"
	.string "all anyone does here anymore.$"

SchismVenomquayPort_Text_PoisonGuardCeasefire:
	.string "Stand-down order came through.\n"
	.string "Hard to believe.\p"
	.string "I've been pointing a spear at the\n"
	.string "ice side since I was sixteen.\p"
	.string "Don't know what I point it at\n"
	.string "now. Maybe nothing. Maybe that's\l"
	.string "the idea.$"

SchismVenomquayPort_Text_Fisherman:
	.string "The fish are dying. Not just\n"
	.string "fewer - wrong.\p"
	.string "Colors are off. Behavior's off.\p"
	.string "Something in the deep water\n"
	.string "changed months ago.\p"
	.string "The faction says it's the ice\n"
	.string "side's fault. The fish don't\l"
	.string "care whose fault it is.$"

SchismVenomquayPort_Text_FishermanResolved:
	.string "Pulled up a netful this morning.\n"
	.string "Right colors. Right behavior.\p"
	.string "First clean catch in half a year.\p"
	.string "I don't understand what fixed\n"
	.string "it. I'm too busy fishing to ask.$"

SchismVenomquayPort_Text_Merchant:
	.string "Antidotes. Remedies. Everything\n"
	.string "you need to survive the south\l"
	.string "side.\p"
	.string "The ice side has the same shop\n"
	.string "selling cold gear.\p"
	.string "Funny thing - the fellow who\n"
	.string "runs it looks just like me.\p"
	.string "Don't tell DRENN.$"

SchismVenomquayPort_Text_MerchantCeasefire:
	.string "One coat. One shop. One me.\p"
	.string "You wouldn't believe the strain\n"
	.string "of being two merchants on two\l"
	.string "sides of a war.\p"
	.string "I owe you a discount. I won't\n"
	.string "give you one, but I owe you it.$"

SchismVenomquayPort_Text_MerchantFarewell:
	.string "Breathe shallow out there. Or\n"
	.string "buy a remedy. Your choice.$"

SchismVenomquayPort_Text_TerritorySign:
	.string "VENOMQUAY - POISON TERRITORY.\p"
	.string "FROSTMARK access prohibited.\n"
	.string "By order of VENOM LORD DRENN.$"

SchismVenomquayPort_Text_BerthSign:
	.string "VISITOR BERTH\p"
	.string "Do not let your hull sit long in\n"
	.string "this water. It does not\l"
	.string "negotiate.$"

SchismVenomquayPort_Text_TennysonSail:
	.string "The TENNYSON rides at the\n"
	.string "corroded berth, rain hissing\l"
	.string "faintly off her deck.$"
