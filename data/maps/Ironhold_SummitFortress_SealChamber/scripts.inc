Ironhold_SummitFortress_SealChamber_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Covenant security barrier - coord triggers at the mid-chamber line,
@ armed while VAR_IRONHOLD_PROGRESS is 3 or 4 (Sever not yet defeated).
@ Turns the player back south.
@ -------------------------------------------------------------------
IronholdSealChamber_EventScript_DoorSealed::
	lockall
	playse SE_BANG
	waitse
	msgbox IronholdSealChamber_Text_DoorSealed, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, IronholdSealChamber_Movement_StepBackSouth
	waitmovement 0
	releaseall
	end

IronholdSealChamber_Movement_StepBackSouth:
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Discovery cutscene - first crossing at VAR_IRONHOLD_PROGRESS == 5.
@ The camera pans across the siphon machinery to Ferrath's seal, the
@ chamber rumbles, and the player understands what the Covenant built.
@ Sets FLAG_IRONHOLD_SEAL_FOUND and VAR_IRONHOLD_PROGRESS = 6.
@ -------------------------------------------------------------------
IronholdSealChamber_EventScript_Discovery::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, IronholdSealChamber_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox IronholdSealChamber_Text_DiscoveryMachinery, MSGBOX_DEFAULT
	closemessage
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 6  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox IronholdSealChamber_Text_DiscoveryFerrath, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, IronholdSealChamber_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	msgbox IronholdSealChamber_Text_DiscoveryDraining, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_IRONHOLD_SEAL_FOUND
	setvar VAR_IRONHOLD_PROGRESS, 6
	releaseall
	end

IronholdSealChamber_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

IronholdSealChamber_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ The siphon apparatus. Interacting after the discovery offers the
@ reinforcement sequence - the island resolution:
@   FLAG_IRONHOLD_RESOLVED + FLAG_IRONHOLD_CIPHER_FOUND +
@   FLAG_CIPHER_2_FOUND, VAR_IRONHOLD_PROGRESS = 7, VAR_BOAT_TIER = 2,
@   journal entry unlock, Sollis PokeNav call, Brigantine unlock.
@ NOTE: the brief calls this "cipher 1 of 9", but Haven Isle's ruins
@ inscription already set FLAG_CIPHER_1_FOUND - Ironhold's entry is
@ therefore cipher 2 (see report). ITEM_SEAL_SHARD_IRONHOLD stays a
@ stub per the brief: NOT given here.
@ -------------------------------------------------------------------
IronholdSealChamber_EventScript_Apparatus::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdSealChamber_EventScript_ApparatusAfter
	msgbox IronholdSealChamber_Text_ApparatusOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, IronholdSealChamber_EventScript_ApparatusStepBack
	msgbox IronholdSealChamber_Text_ReinforceStart, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	playse SE_TRUCK_DOOR
	waitse
	playse SE_UNLOCK
	waitse
	fadescreen FADE_FROM_WHITE
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 4  @ shake delay
	special ShakeCamera
	waitstate
	playmoncry SPECIES_REGISTEEL, CRY_MODE_ENCOUNTER  @ placeholder cry for Ferrath
	waitmoncry
	msgbox IronholdSealChamber_Text_ReinforceDone, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_IRONHOLD_SEAL_FOUND
	setflag FLAG_IRONHOLD_RESOLVED
	setflag FLAG_IRONHOLD_CIPHER_FOUND
	setflag FLAG_CIPHER_2_FOUND
	setvar VAR_IRONHOLD_PROGRESS, 7
	setvar VAR_BOAT_TIER, 2
	delay 30
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox IronholdSealChamber_Text_JournalUnlocks, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox IronholdSealChamber_Text_SollisCall, MSGBOX_DEFAULT
	closemessage
	msgbox IronholdSealChamber_Text_BrigantineUnlocked, MSGBOX_DEFAULT
	release
	end

IronholdSealChamber_EventScript_ApparatusStepBack::
	msgbox IronholdSealChamber_Text_ApparatusStepBack, MSGBOX_DEFAULT
	release
	end

IronholdSealChamber_EventScript_ApparatusAfter::
	msgbox IronholdSealChamber_Text_ApparatusAfter, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdSealChamber_Text_DoorSealed:
	.string "A lattice of humming Covenant\n"
	.string "machinery bars the walkway.\p"
	.string "A speaker crackles overhead:\p"
	.string "“RESTRICTED AREA. COMMAND-LEVEL\n"
	.string "AUTHORIZATION REQUIRED.”$"

IronholdSealChamber_Text_DiscoveryMachinery:
	.string "The chamber opens around you.\p"
	.string "Ancient stone - older than the\n"
	.string "fortress, older than the city -\l"
	.string "and bolted into it, floor to\l"
	.string "ceiling, Covenant machinery.\p"
	.string "Cables thick as masts run from\n"
	.string "the walls to a humming apparatus,\l"
	.string "and every cable pulses with the\l"
	.string "same slow rhythm.\p"
	.string "The rhythm of the glow you saw\n"
	.string "from the harbor.$"

IronholdSealChamber_Text_DiscoveryFerrath:
	.string "Behind the machines, held in a\n"
	.string "cage of pale light-\p"
	.string "-something vast stirs.\p"
	.string "FERRATH. The sealed warden of\n"
	.string "IRONHOLD's summit.\p"
	.string "Its glow is thin. Faded. Like a\n"
	.string "lamp running out of oil.$"

IronholdSealChamber_Text_DiscoveryDraining:
	.string "The machinery is draining\n"
	.string "something.\p"
	.string "Something that shouldn't be\n"
	.string "drained.$"

IronholdSealChamber_Text_ApparatusOffer:
	.string "The siphon apparatus hums under\n"
	.string "your hands.\p"
	.string "Among the Covenant dials, a panel\n"
	.string "of ancient stone is set into the\l"
	.string "housing - the same spiral carving\l"
	.string "you saw in the HAVEN ISLE ruins.\p"
	.string "The WARDEN's work. A maintenance\n"
	.string "channel into the seal itself.\p"
	.string "Reroute the siphon and reinforce\n"
	.string "the seal?$"

IronholdSealChamber_Text_ApparatusStepBack:
	.string "You step back.\p"
	.string "The machinery keeps drinking,\n"
	.string "slow and patient.$"

IronholdSealChamber_Text_ReinforceStart:
	.string "Your hands find the spiral panel.\p"
	.string "It is warm - impossibly warm -\n"
	.string "and it turns under your palm as\l"
	.string "if it remembers being turned.$"

IronholdSealChamber_Text_ReinforceDone:
	.string "The siphon chokes, stutters-\n"
	.string "and dies.\p"
	.string "One by one the cables go dark.\p"
	.string "Behind the silent machines,\n"
	.string "FERRATH's glow steadies - dim,\l"
	.string "then even, then strong.\p"
	.string "Like breathing.$"

IronholdSealChamber_Text_JournalUnlocks:
	.string "In your pack, the WARDEN's\n"
	.string "JOURNAL grows warm.\p"
	.string "An encoded page drinks in the\n"
	.string "seal's light, and part of it\l"
	.string "resolves into your parent's\l"
	.string "handwriting:\p"
	.string "“SEVER knows. He won't act, but\n"
	.string "he knows. I don't blame him.\p"
	.string "What can one person do inside a\n"
	.string "system that large?\p"
	.string "I used to think I knew the answer\n"
	.string "to that question.”\p"
	.string "(The second cipher fragment has\n"
	.string "been recorded in the journal's\l"
	.string "margin!)$"

IronholdSealChamber_Text_SollisCall:
	.string "…Your POKéNAV buzzes.\p"
	.string "SOLLIS: {PLAYER}! The readings\n"
	.string "from IRONHOLD just changed.\l"
	.string "What did you find up there?\p"
	.string "You tell her. The Covenant was\n"
	.string "using the seal. Draining it.\p"
	.string "The line is quiet for a long\n"
	.string "moment.\p"
	.string "SOLLIS: …Come back to HAVEN ISLE\n"
	.string "when you can.\p"
	.string "SOLLIS: There are things I should\n"
	.string "have told you earlier.$"

IronholdSealChamber_Text_BrigantineUnlocked:
	.string "Word travels down the mountain\n"
	.string "faster than you do.\p"
	.string "By the time you reach the harbor,\n"
	.string "the TENNYSON will be rigged as a\l"
	.string "BRIGANTINE - fit for the open\l"
	.string "straits beyond IRONHOLD.$"

IronholdSealChamber_Text_ApparatusAfter:
	.string "The apparatus is cold and dark.\p"
	.string "Beyond it, FERRATH's seal pulses\n"
	.string "slow and even.\p"
	.string "Asleep. Properly asleep, for the\n"
	.string "first time in years.$"
