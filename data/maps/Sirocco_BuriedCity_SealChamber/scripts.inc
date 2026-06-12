Sirocco_BuriedCity_SealChamber_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Discovery cutscene - first crossing of the mid-chamber line at
@ VAR_SIROCCO_PROGRESS == 5 (Dagan beaten and escaped). The camera
@ pans up across the Gilt Claw pump rig to Xerath's seal, the chamber
@ groans, and the drought finally has a face.
@ Sets FLAG_SIROCCO_SEAL_FOUND and VAR_SIROCCO_PROGRESS = 6.
@ (At progress < 5 the triggers are unarmed; the apparatus script
@ politely stonewalls anyone who wanders in early.)
@ -------------------------------------------------------------------
SiroccoSealChamber_EventScript_Discovery::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoSealChamber_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SiroccoSealChamber_Text_DiscoveryMachinery, MSGBOX_DEFAULT
	closemessage
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 6  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox SiroccoSealChamber_Text_DiscoveryXerath, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoSealChamber_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	msgbox SiroccoSealChamber_Text_DiscoveryDeliberate, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SIROCCO_SEAL_FOUND
	setvar VAR_SIROCCO_PROGRESS, 6
	releaseall
	end

SiroccoSealChamber_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SiroccoSealChamber_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ The Gilt Claw pump apparatus - the island resolution.
@ After the discovery (progress 6): disable the rig, then reinforce
@ the seal through the Warden's spiral panel. Sets
@ FLAG_SIROCCO_RESOLVED, VAR_SIROCCO_PROGRESS = 7, then the Sollis
@ PokeNav call and the conditional Galleon check:
@   FLAG_EMBERVEIL_RESOLVED also set -> VAR_BOAT_TIER = 3 + refit text
@   otherwise -> Sirocco acknowledged, Emberveil still burning.
@ ITEM_SEAL_SHARD_SIROCCO stays a stub per the brief: NOT given here.
@ The cipher was the Interior2 mural, so no journal beat here.
@ -------------------------------------------------------------------
SiroccoSealChamber_EventScript_Apparatus::
	lock
	faceplayer
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoSealChamber_EventScript_ApparatusAfter
	goto_if_unset FLAG_SIROCCO_SEAL_FOUND, SiroccoSealChamber_EventScript_ApparatusLocked
	msgbox SiroccoSealChamber_Text_ApparatusOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SiroccoSealChamber_EventScript_ApparatusStepBack
	msgbox SiroccoSealChamber_Text_ShutdownStart, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	playse SE_TRUCK_DOOR
	waitse
	playse SE_UNLOCK
	waitse
	fadescreen FADE_FROM_WHITE
	msgbox SiroccoSealChamber_Text_PumpDies, MSGBOX_DEFAULT
	closemessage
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 4  @ shake delay
	special ShakeCamera
	waitstate
	playmoncry SPECIES_REGIROCK, CRY_MODE_ENCOUNTER  @ placeholder cry for Xerath
	waitmoncry
	msgbox SiroccoSealChamber_Text_SealReinforced, MSGBOX_DEFAULT
	msgbox SiroccoSealChamber_Text_WaterReturns, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SIROCCO_RESOLVED
	setvar VAR_SIROCCO_PROGRESS, 7
	delay 30
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox SiroccoSealChamber_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox SiroccoSealChamber_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoSealChamber_Text_SollisCallReply, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox SiroccoSealChamber_Text_SollisCallSeed, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_EMBERVEIL_RESOLVED, SiroccoSealChamber_EventScript_GalleonUnlock
	msgbox SiroccoSealChamber_Text_EmberveilRemains, MSGBOX_DEFAULT
	release
	end

SiroccoSealChamber_EventScript_GalleonUnlock::
	setvar VAR_BOAT_TIER, 3
	msgbox SiroccoSealChamber_Text_GalleonUnlocked, MSGBOX_DEFAULT
	release
	end

SiroccoSealChamber_EventScript_ApparatusStepBack::
	msgbox SiroccoSealChamber_Text_ApparatusStepBack, MSGBOX_DEFAULT
	release
	end

SiroccoSealChamber_EventScript_ApparatusLocked::
	msgbox SiroccoSealChamber_Text_ApparatusLocked, MSGBOX_DEFAULT
	release
	end

SiroccoSealChamber_EventScript_ApparatusAfter::
	msgbox SiroccoSealChamber_Text_ApparatusAfter, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoSealChamber_Text_DiscoveryMachinery:
	.string "The deepest chamber is full of\n"
	.string "pipework.\p"
	.string "Not the Covenant's clean, humming\n"
	.string "lattices - this is welded scrap\l"
	.string "and salvaged pumps, bolted\l"
	.string "straight into thousand-year-old\l"
	.string "stone.\p"
	.string "Crude. Hungry. And it is drinking\n"
	.string "in long, slow pulls, like a man\l"
	.string "at a stolen well.$"

SiroccoSealChamber_Text_DiscoveryXerath:
	.string "Past the rig, under a vault of\n"
	.string "carved spirals, the floor itself\l"
	.string "bulges-\p"
	.string "-and you realize the stone there\n"
	.string "is not stone.\p"
	.string "A flank. A shoulder. Something\n"
	.string "vast, asleep beneath the city\l"
	.string "that was built to keep it.\p"
	.string "XERATH. The desert's water,\n"
	.string "wearing a body.$"

SiroccoSealChamber_Text_DiscoveryDeliberate:
	.string "This is what's been draining the\n"
	.string "oasis.\p"
	.string "Not a drought. Not climate.\p"
	.string "Someone built a machine to do\n"
	.string "this deliberately.$"

SiroccoSealChamber_Text_ApparatusOffer:
	.string "The pump rig shudders under your\n"
	.string "hands, sucking at the seal below.\p"
	.string "Behind a tangle of scavenged\n"
	.string "valves, a panel of ancient stone\l"
	.string "is set flush into the rock - the\l"
	.string "same spiral the mural showed you,\l"
	.string "the same carving from the HAVEN\l"
	.string "ISLE ruins.\p"
	.string "The WARDEN's work. A way in.\p"
	.string "Shut down the rig and reinforce\n"
	.string "the seal?$"

SiroccoSealChamber_Text_ApparatusStepBack:
	.string "You step back.\p"
	.string "The pumps keep drinking, patient\n"
	.string "as the man who built them.$"

SiroccoSealChamber_Text_ApparatusLocked:
	.string "A machine of welded scrap and\n"
	.string "pipework, sucking at the floor of\l"
	.string "the chamber.\p"
	.string "The control housing is sealed\n"
	.string "with a heavy Gilt Claw lock.\p"
	.string "Whoever holds the key holds this\n"
	.string "island - and he isn't here.$"

SiroccoSealChamber_Text_ShutdownStart:
	.string "You wrench the valves shut one by\n"
	.string "one, then press your palm to the\l"
	.string "spiral panel.\p"
	.string "It is cool - river-cool, in all\n"
	.string "this heat - and it turns as if it\l"
	.string "remembers being turned.$"

SiroccoSealChamber_Text_PumpDies:
	.string "The rig coughs, knocks twice\n"
	.string "along its whole length-\p"
	.string "-and dies.\p"
	.string "For the first time in years, the\n"
	.string "chamber is silent.$"

SiroccoSealChamber_Text_SealReinforced:
	.string "Then, under the silence, a sound\n"
	.string "you feel in your teeth: water,\l"
	.string "moving in deep rock.\p"
	.string "The carved spirals across the\n"
	.string "vault light one by one, and the\l"
	.string "great sleeping form beneath the\l"
	.string "floor settles - dim, then even,\l"
	.string "then strong.\p"
	.string "XERATH sleeps. Properly, this\n"
	.string "time. And the desert drinks.$"

SiroccoSealChamber_Text_WaterReturns:
	.string "Somewhere far above, an old\n"
	.string "riverbed is remembering its\l"
	.string "name.$"

SiroccoSealChamber_Text_PokenavBuzz:
	.string "…Your POKéNAV buzzes.$"

SiroccoSealChamber_Text_SollisCall:
	.string "{PLAYER}! The SIROCCO readings\n"
	.string "just stabilized. What happened?$"

SiroccoSealChamber_Text_SollisCallReply:
	.string "You tell her. A crime lord was\n"
	.string "draining the legendary's seal for\l"
	.string "water control.\p"
	.string "He got away.\p"
	.string "The line is quiet for a moment.$"

SiroccoSealChamber_Text_SollisCallSeed:
	.string "…The Covenant's work is usually\n"
	.string "more subtle than that.\p"
	.string "DAGAN was operating\n"
	.string "independently?\p"
	.string "…Never mind. Patch your POKéMON\n"
	.string "up and drink some water. You're\l"
	.string "in a desert, you know.$"

SiroccoSealChamber_Text_EmberveilRemains:
	.string "Two of the nine seals now stand\n"
	.string "reinforced.\p"
	.string "But across the strait, EMBERVEIL\n"
	.string "still burns - and the TENNYSON\l"
	.string "stays a BRIGANTINE until both\l"
	.string "anchors of this crossing are\l"
	.string "quiet.$"

SiroccoSealChamber_Text_GalleonUnlocked:
	.string "SIROCCO's sands and EMBERVEIL's\n"
	.string "fires - both seals now stand\l"
	.string "quiet.\p"
	.string "Word will reach the shipwrights\n"
	.string "before you reach the dock.\p"
	.string "The TENNYSON is to be refit as a\n"
	.string "GALLEON - fit for the deep\l"
	.string "southern ocean, and whatever\l"
	.string "waits across it.$"

SiroccoSealChamber_Text_ApparatusAfter:
	.string "The pump rig is cold scrap now.\p"
	.string "Beneath the floor, XERATH's seal\n"
	.string "pulses slow and even, and the\l"
	.string "sound of deep water runs through\l"
	.string "the stone like a kept promise.$"
