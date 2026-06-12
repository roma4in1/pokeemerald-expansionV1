Emberveil_SealChamber_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Discovery cutscene - first crossing of the mid-chamber line at
@ VAR_EMBERVEIL_PROGRESS == 5 (Solace confronted, Path A or B; the
@ summit warps are only reachable after her scene). Camera pans up
@ across the ancient monitoring instruments to Pyrath's seal. What
@ the player sees differs by path: a dimmed, restless Pyrath (Path A -
@ the ritual was stopped by force, nothing has steadied it yet) or a
@ calm, aware Pyrath (Path B - the commune already stabilized the
@ seal from above). Sets FLAG_EMBERVEIL_SEAL_FOUND and
@ VAR_EMBERVEIL_PROGRESS = 6, which disarms these triggers.
@ -------------------------------------------------------------------
EmberveilSealChamber_EventScript_Discovery::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, EmberveilSealChamber_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox EmberveilSealChamber_Text_DiscoveryChamber, MSGBOX_DEFAULT
	closemessage
	playse SE_M_FLAME_WHEEL
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 6  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilSealChamber_EventScript_DiscoveryPyrathB
	msgbox EmberveilSealChamber_Text_DiscoveryPyrathA, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_DiscoveryClose

EmberveilSealChamber_EventScript_DiscoveryPyrathB::
	msgbox EmberveilSealChamber_Text_DiscoveryPyrathB, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_DiscoveryClose

EmberveilSealChamber_EventScript_DiscoveryClose::
	applymovement OBJ_EVENT_ID_CAMERA, EmberveilSealChamber_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	msgbox EmberveilSealChamber_Text_DiscoveryInstruments, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_EMBERVEIL_SEAL_FOUND
	setvar VAR_EMBERVEIL_PROGRESS, 6
	releaseall
	end

EmberveilSealChamber_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

EmberveilSealChamber_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ The ancient monitoring apparatus - the island resolution.
@ Unlike Ironhold's siphon or Sirocco's pump rig, nothing here is
@ stealing from the seal: the wound was the ritual above, and this is
@ the instrument the sealing civilization left to mend such wear.
@ After the discovery (progress 6), interacting offers reinforcement:
@   FLAG_EMBERVEIL_RESOLVED + FLAG_EMBERVEIL_CIPHER_FOUND +
@   FLAG_CIPHER_4_FOUND (cipher 4: Haven 1, Ironhold 2, Sirocco 3 -
@   numbering consistent, no brief discrepancy this island),
@   VAR_EMBERVEIL_PROGRESS = 7, journal entry, Solace + Sollis PokeNav
@   beats (both path-split), then the conditional Galleon check:
@   FLAG_SIROCCO_RESOLVED also set -> VAR_BOAT_TIER = 3 + refit text
@   otherwise -> Emberveil acknowledged, Sirocco's sands still restless.
@   VAR_BOAT_TIER is only ever SET to 3 here, never lowered; either
@   island may resolve second and the check mirrors Sirocco's.
@ ITEM_SEAL_SHARD_EMBERVEIL stays a stub per the brief: NOT given here.
@ -------------------------------------------------------------------
EmberveilSealChamber_EventScript_Apparatus::
	lock
	faceplayer
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilSealChamber_EventScript_ApparatusAfter
	goto_if_unset FLAG_EMBERVEIL_SEAL_FOUND, EmberveilSealChamber_EventScript_ApparatusLocked
	msgbox EmberveilSealChamber_Text_ApparatusOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EmberveilSealChamber_EventScript_ApparatusStepBack
	msgbox EmberveilSealChamber_Text_ReinforceStart, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	playse SE_M_FLAME_WHEEL
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
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER  @ placeholder cry for Pyrath
	waitmoncry
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilSealChamber_EventScript_ReinforceDoneB
	msgbox EmberveilSealChamber_Text_ReinforceDoneA, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_Resolution

EmberveilSealChamber_EventScript_ReinforceDoneB::
	msgbox EmberveilSealChamber_Text_ReinforceDoneB, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_Resolution

EmberveilSealChamber_EventScript_Resolution::
	msgbox EmberveilSealChamber_Text_MountainQuiets, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_EMBERVEIL_RESOLVED
	setflag FLAG_EMBERVEIL_CIPHER_FOUND
	setflag FLAG_CIPHER_4_FOUND
	setvar VAR_EMBERVEIL_PROGRESS, 7
	delay 30
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox EmberveilSealChamber_Text_JournalUnlocks, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox EmberveilSealChamber_Text_PokenavBuzz, MSGBOX_DEFAULT
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilSealChamber_EventScript_SolaceCallB
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilSealChamber_Text_SolaceCallA, MSGBOX_DEFAULT
	setspeaker 0
	goto EmberveilSealChamber_EventScript_SollisCall

EmberveilSealChamber_EventScript_SolaceCallB::
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilSealChamber_Text_SolaceCallB, MSGBOX_DEFAULT
	setspeaker 0
	goto EmberveilSealChamber_EventScript_SollisCall

EmberveilSealChamber_EventScript_SollisCall::
	msgbox EmberveilSealChamber_Text_SecondCall, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox EmberveilSealChamber_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilSealChamber_EventScript_SollisReplyB
	msgbox EmberveilSealChamber_Text_SollisReplyA, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox EmberveilSealChamber_Text_SollisCloseA, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_GalleonCheck

EmberveilSealChamber_EventScript_SollisReplyB::
	msgbox EmberveilSealChamber_Text_SollisReplyB, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox EmberveilSealChamber_Text_SollisCloseB, MSGBOX_DEFAULT
	closemessage
	goto EmberveilSealChamber_EventScript_GalleonCheck

EmberveilSealChamber_EventScript_GalleonCheck::
	goto_if_set FLAG_SIROCCO_RESOLVED, EmberveilSealChamber_EventScript_GalleonUnlock
	msgbox EmberveilSealChamber_Text_SiroccoRemains, MSGBOX_DEFAULT
	release
	end

EmberveilSealChamber_EventScript_GalleonUnlock::
	setvar VAR_BOAT_TIER, 3
	msgbox EmberveilSealChamber_Text_GalleonUnlocked, MSGBOX_DEFAULT
	release
	end

EmberveilSealChamber_EventScript_ApparatusStepBack::
	msgbox EmberveilSealChamber_Text_ApparatusStepBack, MSGBOX_DEFAULT
	release
	end

EmberveilSealChamber_EventScript_ApparatusLocked::
	msgbox EmberveilSealChamber_Text_ApparatusLocked, MSGBOX_DEFAULT
	release
	end

EmberveilSealChamber_EventScript_ApparatusAfter::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilSealChamber_EventScript_ApparatusAfterB
	msgbox EmberveilSealChamber_Text_ApparatusAfterA, MSGBOX_DEFAULT
	release
	end

EmberveilSealChamber_EventScript_ApparatusAfterB::
	msgbox EmberveilSealChamber_Text_ApparatusAfterB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text - discovery
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_DiscoveryChamber:
	.string "The chamber beneath the summit is\n"
	.string "older than anything above it.\p"
	.string "Ringed around the walls stand\n"
	.string "instruments of carved stone -\l"
	.string "dials, channels, basins of\l"
	.string "still fire - the monitoring\l"
	.string "station of the civilization that\l"
	.string "made the seal.\p"
	.string "And filling the far wall, floor\n"
	.string "to ceiling, the seal itself\l"
	.string "burns like a sunrise caught in\l"
	.string "amber.$"

EmberveilSealChamber_Text_DiscoveryPyrathA:
	.string "Through the glowing stone,\n"
	.string "something enormous shifts.\p"
	.string "PYRATH. The fire under the\n"
	.string "island, wearing a shape.\p"
	.string "It turns, restless, the way a\n"
	.string "sleeper turns when the blanket\l"
	.string "has worn thin.\p"
	.string "Thirty years of ritual wore it\n"
	.string "thin. Stopping the ritual\l"
	.string "stopped the wear -\p"
	.string "-but nothing has mended it yet.$"

EmberveilSealChamber_Text_DiscoveryPyrathB:
	.string "Through the glowing stone,\n"
	.string "something enormous rests.\p"
	.string "PYRATH. The fire under the\n"
	.string "island, wearing a shape.\p"
	.string "It is not asleep. It watches you\n"
	.string "cross the chamber the way a\l"
	.string "hearth watches a familiar room -\l"
	.string "warm, unhurried, awake.\p"
	.string "The commune steadied the seal.\n"
	.string "It has not yet been made to\l"
	.string "last.$"

EmberveilSealChamber_Text_DiscoveryInstruments:
	.string "At the center of the ring of\n"
	.string "instruments, one apparatus\l"
	.string "stands apart - and set into its\l"
	.string "housing, a spiral panel you know\l"
	.string "by heart now.\p"
	.string "HAVEN ISLE. IRONHOLD's fortress.\n"
	.string "The caldera wall.\p"
	.string "The WARDEN's work waits for\n"
	.string "hands that know what it is.$"

@ -------------------------------------------------------------------
@ Text - apparatus and reinforcement
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_ApparatusOffer:
	.string "The ancient apparatus radiates a\n"
	.string "slow, patient heat.\p"
	.string "Nothing here is broken - no\n"
	.string "siphon, no pump, no theft. Only\l"
	.string "two centuries of honest wear,\l"
	.string "waiting for the mending the\l"
	.string "builders planned for.\p"
	.string "The spiral panel sits warm in\n"
	.string "the housing, worn smooth by\l"
	.string "hands long gone.\p"
	.string "Reinforce PYRATH's seal?$"

EmberveilSealChamber_Text_ApparatusStepBack:
	.string "You step back.\p"
	.string "The seal's light washes the\n"
	.string "chamber, patient as the mountain\l"
	.string "around it.$"

EmberveilSealChamber_Text_ApparatusLocked:
	.string "Instruments of carved stone ring\n"
	.string "the chamber, warm to the touch.\p"
	.string "Their purpose folds away from\n"
	.string "you like a closed book.\p"
	.string "Perhaps the seal itself must be\n"
	.string "understood first.$"

EmberveilSealChamber_Text_ReinforceStart:
	.string "Your hands find the spiral panel.\p"
	.string "It is warm - hearth-warm, not\n"
	.string "furnace-hot - and it turns under\l"
	.string "your palm as if it remembers\l"
	.string "being turned.$"

EmberveilSealChamber_Text_ReinforceDoneA:
	.string "Light floods the channels in the\n"
	.string "floor, races the ring of\l"
	.string "instruments, and pours into the\l"
	.string "seal.\p"
	.string "The worn places fill. The\n"
	.string "restless shape behind the stone\l"
	.string "slows, settles-\p"
	.string "-and sleeps. Truly sleeps, for\n"
	.string "the first time in thirty years.\p"
	.string "Whatever was taken from PYRATH\n"
	.string "by mistake, in faith, has been\l"
	.string "given back.$"

EmberveilSealChamber_Text_ReinforceDoneB:
	.string "Light floods the channels in the\n"
	.string "floor, races the ring of\l"
	.string "instruments, and pours into the\l"
	.string "seal.\p"
	.string "The worn places fill - and the\n"
	.string "watching presence behind the\l"
	.string "stone leans into the mending the\l"
	.string "way a cat leans into a hand.\p"
	.string "Acknowledged, the commune said.\n"
	.string "Now it is kept, too.\p"
	.string "Far above, a community's two\n"
	.string "centuries of devotion finally\l"
	.string "mean exactly what they hoped.$"

EmberveilSealChamber_Text_MountainQuiets:
	.string "Far overhead, through a mile of\n"
	.string "rock, the mountain growls once-\p"
	.string "-and goes quiet.\p"
	.string "Not the held-breath quiet of the\n"
	.string "past weeks. The quiet of a thing\l"
	.string "at rest.$"

@ -------------------------------------------------------------------
@ Text - journal cipher 4
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_JournalUnlocks:
	.string "In your pack, the WARDEN's\n"
	.string "JOURNAL grows warm.\p"
	.string "An encoded page drinks in the\n"
	.string "seal's light, and part of it\l"
	.string "resolves into your parent's\l"
	.string "handwriting:\p"
	.string "“The commune ritual works. I\n"
	.string "tested it here - PYRATH\l"
	.string "acknowledged me.\p"
	.string "Not freedom. Not imprisonment.\n"
	.string "Something in between that I\l"
	.string "don't have a word for yet.\p"
	.string "The cult has been sustaining the\n"
	.string "seal for two centuries without\l"
	.string "knowing it.\p"
	.string "If someone finds my notes in\n"
	.string "time, that community survives.\p"
	.string "If not - I hope whoever comes\n"
	.string "after me is kind about how they\l"
	.string "do what has to be done.\p"
	.string "…DORNE called again. He found\n"
	.string "EMBERVEIL. He's moving faster\l"
	.string "than I thought.”\p"
	.string "(The fourth cipher fragment has\n"
	.string "been recorded in the journal's\l"
	.string "margin!)$"

@ -------------------------------------------------------------------
@ Text - PokeNav calls
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_PokenavBuzz:
	.string "…Your POKéNAV buzzes.$"

EmberveilSealChamber_Text_SolaceCallA:
	.string "…It's done, then.\p"
	.string "I felt it from the summit. The\n"
	.string "whole mountain sighed.\p"
	.string "Go safely, child of the WARDEN.\n"
	.string "I have a great deal of thinking\l"
	.string "to do.$"

EmberveilSealChamber_Text_SolaceCallB:
	.string "The island is breathing normally\n"
	.string "again. I can feel it.\p"
	.string "PYRATH is… calm.\p"
	.string "Come up and stand in the warmth\n"
	.string "before you sail, child. It's\l"
	.string "yours too, now.$"

EmberveilSealChamber_Text_SecondCall:
	.string "Before the line clears, a second\n"
	.string "call cuts in.$"

EmberveilSealChamber_Text_SollisCall:
	.string "{PLAYER}! The EMBERVEIL readings\n"
	.string "just stabilized. The seal is\l"
	.string "holding.\p"
	.string "What happened up there?$"

EmberveilSealChamber_Text_SollisReplyA:
	.string "You tell her. The cult's ritual\n"
	.string "was wearing the seal thin. You\l"
	.string "defeated the HIGH EMBER and\l"
	.string "mended it by hand.\p"
	.string "The line is quiet for a long\n"
	.string "moment.$"

EmberveilSealChamber_Text_SollisCloseA:
	.string "…Then it holds by strength\n"
	.string "alone. That works. It worked on\l"
	.string "IRONHOLD.\p"
	.string "Your parent always hoped\n"
	.string "EMBERVEIL wouldn't come to\l"
	.string "that. They'd spent years on\l"
	.string "some other approach there-\p"
	.string "…Never mind. It's holding.\n"
	.string "That's what matters. Come home\l"
	.string "when you can.$"

EmberveilSealChamber_Text_SollisReplyB:
	.string "You tell her. The notes hidden\n"
	.string "in the caldera wall. The commune\l"
	.string "ritual. SOLACE's choice.\p"
	.string "The line is quiet for a long\n"
	.string "moment.$"

EmberveilSealChamber_Text_SollisCloseB:
	.string "The commune ritual.\p"
	.string "Your parent found that notation\n"
	.string "years ago. I didn't know if it\l"
	.string "would work.\p"
	.string "…I didn't know they'd hidden it\n"
	.string "on EMBERVEIL, either.\p"
	.string "That's two things your parent\n"
	.string "didn't tell me. Come home when\l"
	.string "you can, {PLAYER}. Safely.$"

@ -------------------------------------------------------------------
@ Text - Galleon check
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_SiroccoRemains:
	.string "Another seal stands reinforced.\p"
	.string "But across the strait, SIROCCO's\n"
	.string "sands are still restless - and\l"
	.string "the TENNYSON stays a BRIGANTINE\l"
	.string "until both anchors of this\l"
	.string "crossing are quiet.$"

EmberveilSealChamber_Text_GalleonUnlocked:
	.string "EMBERVEIL's fires and SIROCCO's\n"
	.string "sands - both seals now stand\l"
	.string "quiet.\p"
	.string "Word will reach the shipwrights\n"
	.string "before you reach the dock.\p"
	.string "The TENNYSON is to be refit as a\n"
	.string "GALLEON - fit for the deep\l"
	.string "southern ocean, and whatever\l"
	.string "waits across it.$"

@ -------------------------------------------------------------------
@ Text - post-resolution
@ -------------------------------------------------------------------
EmberveilSealChamber_Text_ApparatusAfterA:
	.string "The apparatus is cool now, its\n"
	.string "work done.\p"
	.string "Beyond the ring of instruments,\n"
	.string "PYRATH's seal pulses slow and\l"
	.string "even.\p"
	.string "Asleep. Properly asleep, for the\n"
	.string "first time in thirty years.$"

EmberveilSealChamber_Text_ApparatusAfterB:
	.string "The apparatus is cool now, its\n"
	.string "work done.\p"
	.string "Beyond the ring of instruments,\n"
	.string "PYRATH's seal pulses slow and\l"
	.string "warm.\p"
	.string "Standing in its light feels like\n"
	.string "being known.\p"
	.string "Somewhere far above, a chant\n"
	.string "drifts down the mountain -\l"
	.string "slower now, lower. A kept watch,\l"
	.string "not a countdown.$"
