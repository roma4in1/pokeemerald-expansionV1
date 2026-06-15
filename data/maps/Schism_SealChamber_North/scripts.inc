Schism_SealChamber_North_MapScripts::
	.byte 0

@ ===================================================================
@ SEAL CHAMBER NORTH - Glacith's seal. Discovery cutscene + the
@ reinforcement apparatus. Reinforcing here sets
@ FLAG_SCHISM_SEAL_NORTH_DONE, then runs the shared dual-seal check:
@ if the SOUTH seal is also done, the island resolves (whichever seal
@ is reinforced SECOND triggers it - order-independent, mirrored in
@ SealChamber_South, modeled on the Sirocco/Emberveil Galleon check).
@ ===================================================================

@ -------------------------------------------------------------------
@ Discovery cutscene. Coord triggers (row 14) armed at
@ VAR_SCHISM_PROGRESS == 3 (Eira beaten + ice Scar pass = the ice side
@ is open). Camera pans up to Glacith and the crude faction machinery.
@ Sets FLAG_SCHISM_SEAL_NORTH_FOUND. (Does NOT bump VAR_SCHISM_PROGRESS:
@ the progress var stays a city-side counter; the seal state lives on
@ its own FOUND/DONE flags so north and south are independent.)
@ -------------------------------------------------------------------
SchismSealChamberNorth_EventScript_Discovery::
	lockall
	@ Fire-once guard: armed across several progress values (nonlinear
	@ island), so skip if already discovered.
	goto_if_set FLAG_SCHISM_SEAL_NORTH_FOUND, SchismSealChamberNorth_EventScript_DiscoverySkip
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SchismSealChamberNorth_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SchismSealChamberNorth_Text_Discovery, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 6
	setvar VAR_0x8007, 5
	special ShakeCamera
	waitstate
	msgbox SchismSealChamberNorth_Text_DiscoveryGlacith, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SchismSealChamberNorth_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	msgbox SchismSealChamberNorth_Text_DiscoveryMachinery, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_SEAL_NORTH_FOUND
	releaseall
	end

SchismSealChamberNorth_EventScript_DiscoverySkip::
	releaseall
	end

SchismSealChamberNorth_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SchismSealChamberNorth_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ The apparatus (CABLE_CAR at 10,8). First, the player tears out the
@ crude faction siphon; then reinforces the seal.
@ -------------------------------------------------------------------
SchismSealChamberNorth_EventScript_Apparatus::
	lock
	faceplayer
	goto_if_set FLAG_SCHISM_SEAL_NORTH_DONE, SchismSealChamberNorth_EventScript_ApparatusAfter
	goto_if_unset FLAG_SCHISM_SEAL_NORTH_FOUND, SchismSealChamberNorth_EventScript_ApparatusLocked
	msgbox SchismSealChamberNorth_Text_ApparatusOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismSealChamberNorth_EventScript_ApparatusStepBack
	msgbox SchismSealChamberNorth_Text_ReinforceStart, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	playse SE_M_HAIL
	waitse
	playse SE_UNLOCK
	waitse
	fadescreen FADE_FROM_WHITE
	playmoncry SPECIES_REGICE, CRY_MODE_ENCOUNTER  @ placeholder cry for Glacith
	waitmoncry
	msgbox SchismSealChamberNorth_Text_ReinforceDone, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_SEAL_NORTH_DONE
	@ Mark which chamber we resolved in (north = 0) for the failure flavor.
	setvar VAR_TEMP_2, 0
	goto Schism_EventScript_CheckDualSeals

SchismSealChamberNorth_EventScript_ApparatusStepBack::
	msgbox SchismSealChamberNorth_Text_ApparatusStepBack, MSGBOX_DEFAULT
	release
	end

SchismSealChamberNorth_EventScript_ApparatusLocked::
	msgbox SchismSealChamberNorth_Text_ApparatusLocked, MSGBOX_DEFAULT
	release
	end

SchismSealChamberNorth_EventScript_ApparatusAfter::
	msgbox SchismSealChamberNorth_Text_ApparatusAfter, MSGBOX_DEFAULT
	release
	end

@ ===================================================================
@ SHARED dual-seal check + resolution (called from BOTH chambers).
@ ORDER-INDEPENDENT: only fires the resolution when BOTH
@ FLAG_SCHISM_SEAL_NORTH_DONE and FLAG_SCHISM_SEAL_SOUTH_DONE are set,
@ so whichever seal is reinforced second resolves the island.
@   - ceasefire held  -> peaceful resolution, FLAG_DRENN_ALIVE set
@   - no ceasefire     -> failure resolution, Drenn dies
@ The failure FLAVOR branches on which chamber the player is standing
@ in (the SECOND seal): north -> the ice faction's assault on the
@ south; south -> the poison faction's retaliation. In BOTH cases
@ Drenn dies and FLAG_DRENN_ALIVE is left unset; EIRA always survives.
@ This script must be reached with the player still locked.
@ ===================================================================
Schism_EventScript_CheckDualSeals::
	goto_if_unset FLAG_SCHISM_SEAL_NORTH_DONE, Schism_EventScript_OneSealOnly
	goto_if_unset FLAG_SCHISM_SEAL_SOUTH_DONE, Schism_EventScript_OneSealOnly
	@ Both seals are reinforced - the island resolves.
	goto_if_set FLAG_SCHISM_CEASEFIRE, Schism_EventScript_ResolutionPeaceful
	goto Schism_EventScript_ResolutionFailure

@ Only one seal so far - reassure and exit.
Schism_EventScript_OneSealOnly::
	msgbox Schism_Text_OneSealStable, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Peaceful resolution (ceasefire held).
@ -------------------------------------------------------------------
Schism_EventScript_ResolutionPeaceful::
	msgbox Schism_Text_BothSealsStable, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_RESOLVED
	setflag FLAG_DRENN_ALIVE
	setvar VAR_SCHISM_PROGRESS, 7
	delay 30
	msgbox Schism_Text_ResolutionScene, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox Schism_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox Schism_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	msgbox Schism_Text_SollisPeacefulReply, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox Schism_Text_SollisPeacefulClose, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	goto Schism_EventScript_AshenveilCheck

@ -------------------------------------------------------------------
@ Failure resolution (no ceasefire). Drenn dies. The flavor differs by
@ which chamber (the second seal) the player completed.
@ -------------------------------------------------------------------
Schism_EventScript_ResolutionFailure::
	msgbox Schism_Text_BothSealsStable, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCHISM_RESOLVED
	setvar VAR_SCHISM_PROGRESS, 7
	delay 30
	@ This shared script lives in the NORTH chamber file but is reached
	@ from BOTH chambers. Branch the assault flavor on which seal the
	@ player did NOT do first - i.e. were they standing in north or
	@ south? The chamber-specific entry sets VAR_TEMP_2 before calling:
	@   VAR_TEMP_2 == 0 -> resolved in NORTH (ice assault on south)
	@   VAR_TEMP_2 == 1 -> resolved in SOUTH (poison retaliation)
	goto_if_eq VAR_TEMP_2, 1, Schism_EventScript_FailureSouth
	msgbox Schism_Text_FailureNorth, MSGBOX_DEFAULT
	closemessage
	goto Schism_EventScript_DrennDeath

Schism_EventScript_FailureSouth::
	msgbox Schism_Text_FailureSouth, MSGBOX_DEFAULT
	closemessage
	goto Schism_EventScript_DrennDeath

@ -------------------------------------------------------------------
@ Drenn's death scene (ceasefire-failure path). FLAG_DRENN_ALIVE stays
@ UNSET. Rendered as the player arriving in the aftermath.
@ -------------------------------------------------------------------
Schism_EventScript_DrennDeath::
	delay 30
	msgbox Schism_Text_DrennDeathScene, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Drenn
	msgbox Schism_Text_DrennLastWords, MSGBOX_DEFAULT
	setspeaker 0
	msgbox Schism_Text_DrennDies, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox Schism_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox Schism_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	msgbox Schism_Text_SollisFailureReply, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox Schism_Text_SollisFailureClose, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	goto Schism_EventScript_AshenveilCheck

@ -------------------------------------------------------------------
@ Ashenveil unlock check. Ashenveil opens only when Schism, Thalvern,
@ and Gildhaven are ALL resolved. Thalvern/Gildhaven are not built yet,
@ so this is a STUB: if both their RESOLVED flags are somehow set,
@ acknowledge the way opening; otherwise the standard "not yet" text.
@ No boat-tier change here - Schism is already Galleon-tier; the
@ Ashenveil sea-chart is future work. Order-independent like the
@ Galleon check (any of the three may resolve last).
@ -------------------------------------------------------------------
Schism_EventScript_AshenveilCheck::
	goto_if_unset FLAG_THALVERN_RESOLVED, Schism_EventScript_AshenveilNotYet
	goto_if_unset FLAG_GILDHAVEN_RESOLVED, Schism_EventScript_AshenveilNotYet
	msgbox Schism_Text_AshenveilOpens, MSGBOX_DEFAULT
	release
	end

Schism_EventScript_AshenveilNotYet::
	msgbox Schism_Text_AshenveilNotYet, MSGBOX_DEFAULT
	release
	end

@ ===================================================================
@ Text - discovery / apparatus (north)
@ ===================================================================
SchismSealChamberNorth_Text_Discovery:
	.string "The cave bottoms out into a vault\n"
	.string "of blue-white ice.\p"
	.string "Frost-rimed instruments of carved\n"
	.string "stone ring the chamber - the\l"
	.string "monitoring station of the people\l"
	.string "who made the seal.\p"
	.string "And bolted crudely over them:\n"
	.string "newer machinery. Faction-made.\l"
	.string "Stealing, not watching.$"

SchismSealChamberNorth_Text_DiscoveryGlacith:
	.string "Filling the far wall, frozen into\n"
	.string "the glacier itself, something\l"
	.string "vast shifts.\p"
	.string "GLACITH. Ice and steel, wearing a\n"
	.string "shape.\p"
	.string "The crude machinery has been\n"
	.string "drawing its strength out for\l"
	.string "years - and somewhere to the\l"
	.string "south, the poison faction is\l"
	.string "doing the same to its twin.\p"
	.string "Weaken one, the WARDEN's notes\n"
	.string "warned, and you weaken both.$"

SchismSealChamberNorth_Text_DiscoveryMachinery:
	.string "At the center, one ancient\n"
	.string "apparatus still stands clear of\l"
	.string "the faction rig - and set in its\l"
	.string "housing, the spiral panel you\l"
	.string "know by heart.\p"
	.string "Tear out the theft. Mend the\n"
	.string "wear. The WARDEN's work waits.$"

SchismSealChamberNorth_Text_ApparatusOffer:
	.string "The faction siphon clamps over\n"
	.string "the ancient instruments like a\l"
	.string "tick.\p"
	.string "Below it, the WARDEN's spiral\n"
	.string "panel waits, cold and patient.\p"
	.string "Tear out the siphon and reinforce\n"
	.string "GLACITH's seal?$"

SchismSealChamberNorth_Text_ApparatusStepBack:
	.string "You step back.\p"
	.string "The siphon hums on, bleeding the\n"
	.string "seal a little thinner with every\l"
	.string "breath.$"

SchismSealChamberNorth_Text_ApparatusLocked:
	.string "Ancient instruments ring the\n"
	.string "chamber under a crust of frost.\p"
	.string "Their purpose stays closed to\n"
	.string "you. Perhaps the seal itself must\l"
	.string "be understood first.$"

SchismSealChamberNorth_Text_ReinforceStart:
	.string "You wrench the faction siphon\n"
	.string "loose. It dies with a falling\l"
	.string "whine.\p"
	.string "Your hands find the spiral panel.\n"
	.string "It turns under your palm as if it\l"
	.string "remembers being turned.$"

SchismSealChamberNorth_Text_ReinforceDone:
	.string "Light races the frost-channels in\n"
	.string "the floor and pours into the\l"
	.string "glacier.\p"
	.string "The worn places fill. The vast\n"
	.string "frozen shape settles deeper, and\l"
	.string "the air stops feeling like a held\l"
	.string "breath.\p"
	.string "GLACITH's seal stands\n"
	.string "reinforced.$"

SchismSealChamberNorth_Text_ApparatusAfter:
	.string "The apparatus is still now, its\n"
	.string "work done.\p"
	.string "Beyond it, GLACITH sleeps deep in\n"
	.string "the ice - whole, and let be.$"

@ ===================================================================
@ Text - SHARED resolution / failure / Ashenveil (used by both chambers)
@ ===================================================================
Schism_Text_OneSealStable:
	.string "One seal stands stable.\p"
	.string "The other still weakens, far\n"
	.string "across the island.\p"
	.string "Both must be reinforced, or\n"
	.string "neither holds for long.$"

Schism_Text_BothSealsStable:
	.string "Both seals - GLACITH's and\n"
	.string "TOXARA's - now hold.\p"
	.string "The twin chains that kept the\n"
	.string "island in balance are whole\l"
	.string "again.$"

Schism_Text_ResolutionScene:
	.string "Far above, the island steadies.\p"
	.string "The north stays cold. The south\n"
	.string "stays damp. But out in the Scar,\l"
	.string "where nothing has grown in twenty\l"
	.string "years -\p"
	.string "-the faintest green.$"

Schism_Text_FailureNorth:
	.string "Both seals hold. But you reached\n"
	.string "them through war, not peace.\p"
	.string "Word comes up through the ice:\n"
	.string "with GLACITH secured, EIRA's\l"
	.string "people have moved on the south.\p"
	.string "They say the poison faction never\n"
	.string "stood a chance.$"

Schism_Text_FailureSouth:
	.string "Both seals hold. But you reached\n"
	.string "them through war, not peace.\p"
	.string "Word comes up through the cave:\n"
	.string "with TOXARA secured, DRENN's\l"
	.string "people struck north in fury -\p"
	.string "-and EIRA's cautious lines held,\n"
	.string "and broke them.$"

Schism_Text_DrennDeathScene:
	.string "By the time you climb back to\n"
	.string "POISON CITY, it is over.\p"
	.string "DRENN is propped against the lab\n"
	.string "wall, his sick DRAGALGE coiled\l"
	.string "close, both of them failing.\p"
	.string "He fought to the last. Of course\n"
	.string "he did.$"

Schism_Text_DrennLastWords:
	.string "…You.\p"
	.string "The seals hold. I can feel it.\n"
	.string "The water's already clearing.\p"
	.string "You were right. DORNE played us.\n"
	.string "Both of us.\p"
	.string "You should have talked faster.$"

Schism_Text_DrennDies:
	.string "His eyes close.\p"
	.string "The DRAGALGE that was with him\n"
	.string "since boyhood goes still beside\l"
	.string "him.\p"
	.string "EIRA's people hold the city now.\n"
	.string "There was no one left to fight\l"
	.string "them.$"

Schism_Text_PokenavBuzz:
	.string "…Your POKéNAV buzzes.$"

Schism_Text_SollisCall:
	.string "{PLAYER}! SCHISM's readings just\n"
	.string "stabilized - both seals.\l"
	.string "Simultaneously.\p"
	.string "How did you manage that?$"

Schism_Text_SollisPeacefulReply:
	.string "You tell her. The ruins. The\n"
	.string "meeting in the Scar. Two\l"
	.string "commanders standing in the dead\l"
	.string "ground, agreeing to stop.\p"
	.string "The line is quiet for a moment.$"

Schism_Text_SollisPeacefulClose:
	.string "…Your parent tried that once.\p"
	.string "Sat with both factions. Couldn't\n"
	.string "get them to the same table.\p"
	.string "You did.\p"
	.string "Come home when you can. I think\n"
	.string "they'd have wanted to hear how.$"

Schism_Text_SollisFailureReply:
	.string "You tell her. The seals are\n"
	.string "secure. DRENN is dead. The ice\l"
	.string "faction holds both halves now.\p"
	.string "The line is quiet for a long\n"
	.string "moment.$"

Schism_Text_SollisFailureClose:
	.string "…The seals are stable. That's\n"
	.string "what matters.\p"
	.string "I hope the cost was worth it.\p"
	.string "Come home, {PLAYER}. We'll talk\n"
	.string "about the rest when you're\l"
	.string "here.$"

Schism_Text_AshenveilNotYet:
	.string "On the WARDEN's charts, three\n"
	.string "islands ring a fourth, darker\l"
	.string "mark: ASHENVEIL.\p"
	.string "SCHISM is quiet now. But THALVERN\n"
	.string "and GILDHAVEN still stir.\p"
	.string "The way to the dead island stays\n"
	.string "closed until all three are\l"
	.string "stilled.$"

Schism_Text_AshenveilOpens:
	.string "On the WARDEN's charts, the last\n"
	.string "of three rings around ASHENVEIL\l"
	.string "goes still.\p"
	.string "SCHISM. THALVERN. GILDHAVEN. All\n"
	.string "quiet.\p"
	.string "The way to the dead island is\n"
	.string "open at last.$"
