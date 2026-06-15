Schism_SealChamber_South_MapScripts::
	.byte 0

@ ===================================================================
@ SEAL CHAMBER SOUTH - Toxara's seal. Mirror of the north chamber.
@ Reinforcing here sets FLAG_SCHISM_SEAL_SOUTH_DONE, then runs the
@ SHARED dual-seal check (Schism_EventScript_CheckDualSeals, defined in
@ Schism_SealChamber_North/scripts.inc): if the NORTH seal is also
@ done, the island resolves. Order-independent - whichever seal is
@ reinforced second triggers resolution.
@ ===================================================================

@ -------------------------------------------------------------------
@ Discovery cutscene. Coord triggers (row 14) armed at
@ VAR_SCHISM_PROGRESS == 5 (Drenn beaten + poison Scar pass = south
@ side open). Sets FLAG_SCHISM_SEAL_SOUTH_FOUND.
@ -------------------------------------------------------------------
SchismSealChamberSouth_EventScript_Discovery::
	lockall
	@ Fire-once guard: armed across several progress values (nonlinear
	@ island), so skip if already discovered.
	goto_if_set FLAG_SCHISM_SEAL_SOUTH_FOUND, SchismSealChamberSouth_EventScript_DiscoverySkip
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SchismSealChamberSouth_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SchismSealChamberSouth_Text_Discovery, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 6
	setvar VAR_0x8007, 5
	special ShakeCamera
	waitstate
	msgbox SchismSealChamberSouth_Text_DiscoveryToxara, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SchismSealChamberSouth_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	msgbox SchismSealChamberSouth_Text_DiscoveryMachinery, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_SEAL_SOUTH_FOUND
	releaseall
	end

SchismSealChamberSouth_EventScript_DiscoverySkip::
	releaseall
	end

SchismSealChamberSouth_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SchismSealChamberSouth_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ The apparatus (CABLE_CAR at 10,8). Tear out the faction siphon and
@ reinforce Toxara's seal.
@ -------------------------------------------------------------------
SchismSealChamberSouth_EventScript_Apparatus::
	lock
	faceplayer
	goto_if_set FLAG_SCHISM_SEAL_SOUTH_DONE, SchismSealChamberSouth_EventScript_ApparatusAfter
	goto_if_unset FLAG_SCHISM_SEAL_SOUTH_FOUND, SchismSealChamberSouth_EventScript_ApparatusLocked
	msgbox SchismSealChamberSouth_Text_ApparatusOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismSealChamberSouth_EventScript_ApparatusStepBack
	msgbox SchismSealChamberSouth_Text_ReinforceStart, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	playse SE_M_TOXIC
	waitse
	playse SE_UNLOCK
	waitse
	fadescreen FADE_FROM_WHITE
	playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER  @ placeholder cry for Toxara
	waitmoncry
	msgbox SchismSealChamberSouth_Text_ReinforceDone, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_SEAL_SOUTH_DONE
	@ Mark which chamber we resolved in (south = 1) for the failure flavor.
	setvar VAR_TEMP_2, 1
	goto Schism_EventScript_CheckDualSeals

SchismSealChamberSouth_EventScript_ApparatusStepBack::
	msgbox SchismSealChamberSouth_Text_ApparatusStepBack, MSGBOX_DEFAULT
	release
	end

SchismSealChamberSouth_EventScript_ApparatusLocked::
	msgbox SchismSealChamberSouth_Text_ApparatusLocked, MSGBOX_DEFAULT
	release
	end

SchismSealChamberSouth_EventScript_ApparatusAfter::
	msgbox SchismSealChamberSouth_Text_ApparatusAfter, MSGBOX_DEFAULT
	release
	end

@ ===================================================================
@ Text - discovery / apparatus (south)
@ ===================================================================
SchismSealChamberSouth_Text_Discovery:
	.string "The cave bottoms out into a vault\n"
	.string "of dark, dripping stone.\p"
	.string "Instruments of carved rock ring\n"
	.string "the chamber, slick with\l"
	.string "condensation - the same\l"
	.string "monitoring station as the north,\l"
	.string "built by the same hands.\p"
	.string "And bolted crudely over them:\n"
	.string "faction machinery. Stealing, not\l"
	.string "watching.$"

SchismSealChamberSouth_Text_DiscoveryToxara:
	.string "Filling the far wall, half-sunk in\n"
	.string "black water, something vast\l"
	.string "uncoils.\p"
	.string "TOXARA. Poison and dragon, wearing\n"
	.string "a shape.\p"
	.string "The crude machinery has been\n"
	.string "bleeding its strength for years -\l"
	.string "and far to the north, the ice\l"
	.string "faction does the same to its\l"
	.string "twin.\p"
	.string "Weaken one, weaken both. The\n"
	.string "WARDEN's notes were exact.$"

SchismSealChamberSouth_Text_DiscoveryMachinery:
	.string "At the center, one ancient\n"
	.string "apparatus still stands clear of\l"
	.string "the faction rig - and set in its\l"
	.string "housing, the spiral panel you\l"
	.string "know by heart.\p"
	.string "Tear out the theft. Mend the\n"
	.string "wear.$"

SchismSealChamberSouth_Text_ApparatusOffer:
	.string "The faction siphon clamps over\n"
	.string "the ancient instruments, dripping\l"
	.string "with corrosion.\p"
	.string "Below it, the WARDEN's spiral\n"
	.string "panel waits.\p"
	.string "Tear out the siphon and reinforce\n"
	.string "TOXARA's seal?$"

SchismSealChamberSouth_Text_ApparatusStepBack:
	.string "You step back.\p"
	.string "The siphon hums on, bleeding the\n"
	.string "seal a little thinner with every\l"
	.string "breath.$"

SchismSealChamberSouth_Text_ApparatusLocked:
	.string "Ancient instruments ring the\n"
	.string "chamber, slick and dark.\p"
	.string "Their purpose stays closed to\n"
	.string "you. Perhaps the seal itself must\l"
	.string "be understood first.$"

SchismSealChamberSouth_Text_ReinforceStart:
	.string "You wrench the faction siphon\n"
	.string "loose. It dies in a gout of\l"
	.string "stinking steam.\p"
	.string "Your hands find the spiral panel.\n"
	.string "It turns under your palm as if it\l"
	.string "remembers being turned.$"

SchismSealChamberSouth_Text_ReinforceDone:
	.string "Light races the channels in the\n"
	.string "floor and pours into the black\l"
	.string "water.\p"
	.string "The worn places fill. The vast\n"
	.string "coiled shape settles deeper, and\l"
	.string "the water goes clear and still.\p"
	.string "TOXARA's seal stands reinforced.$"

SchismSealChamberSouth_Text_ApparatusAfter:
	.string "The apparatus is still now, its\n"
	.string "work done.\p"
	.string "Beyond it, TOXARA sleeps deep in\n"
	.string "the clearing water - whole, and\l"
	.string "let be.$"
