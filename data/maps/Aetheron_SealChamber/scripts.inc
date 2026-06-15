Aetheron_SealChamber_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Aetheron_SealChamber_OnTransition
	.byte 0

@ ON_TRANSITION: Cass is at the player's side here (FLAG_CASS_DEFECTED).
@ Her chamber object's hide flag is FLAG_AETHERON_CASS_SEEN (set), so
@ force-spawn her with addobject every load. Does not touch CASS_SEEN.
Aetheron_SealChamber_OnTransition:
	goto_if_set FLAG_CASS_DEFECTED, Aetheron_SealChamber_EventScript_SpawnCass
	end

Aetheron_SealChamber_EventScript_SpawnCass::
	addobject LOCALID_AETHERON_CASS_CHAMBER
	return

@ ===================================================================
@ Discovery cutscene. Coord triggers (10,14)/(9,14) at
@ VAR_AETHERON_PROGRESS == 5 (set on StormPeak arrival). The eye of the
@ storm: here the thunder is completely silent. Stormveil is vast,
@ present, electric, behind the apparatus. Sets FLAG_AETHERON_SEAL_FOUND
@ + progress 6 (arms the apparatus). Stormveil cry placeholder =
@ SPECIES_ZAPDOS (an Electric/Flying legendary - swap when a custom
@ Stormveil species exists).
@ ===================================================================
AetheronSealChamber_EventScript_Discovery::
	lockall
	setspeaker 0
	delay 20
	msgbox AetheronSealChamber_Text_Discovery1, MSGBOX_DEFAULT
	closemessage
	delay 20
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	msgbox AetheronSealChamber_Text_Discovery2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_AETHERON_SEAL_FOUND
	setvar VAR_AETHERON_PROGRESS, 6
	delay 20
	msgbox AetheronSealChamber_Text_DiscoveryClose, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Apparatus - the seal reinforcement. Player and Cass reinforce the
@ seal together. Stormveil answers; the storm above shifts character.
@ The extraction apparatus tears itself apart, and in the wreckage the
@ STORM COMPASS is laid bare (it was always part of the seal mechanism).
@ Routes into the Storm Compass giver, then the post-resolution Cass
@ beat and the Sollis call. Sets FLAG_AETHERON_RESOLVED.
@ ===================================================================
AetheronSealChamber_EventScript_Apparatus::
	lockall
	setspeaker 0
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronSealChamber_EventScript_ApparatusResolved
	goto_if_lt VAR_AETHERON_PROGRESS, 6, AetheronSealChamber_EventScript_ApparatusNotReady
	msgbox AetheronSealChamber_Text_ApparatusApproach, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, AetheronSealChamber_EventScript_ApparatusDecline
	closemessage
	delay 20
	@ Two-character reinforcement - Cass takes the other side.
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronSealChamber_Text_CassTogether, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 20
	playse SE_M_CHARGE
	fadescreen FADE_TO_WHITE
	delay 45
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	fadescreen FADE_FROM_WHITE
	msgbox AetheronSealChamber_Text_Reinforce1, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ The apparatus destroys itself.
	playse SE_M_EXPLOSION
	delay 30
	msgbox AetheronSealChamber_Text_Reinforce2, MSGBOX_DEFAULT
	closemessage
	delay 20
	goto AetheronSealChamber_EventScript_StormCompass
	end

AetheronSealChamber_EventScript_ApparatusNotReady::
	msgbox AetheronSealChamber_Text_ApparatusNotReady, MSGBOX_DEFAULT
	releaseall
	end

AetheronSealChamber_EventScript_ApparatusDecline::
	msgbox AetheronSealChamber_Text_ApparatusDecline, MSGBOX_DEFAULT
	releaseall
	end

AetheronSealChamber_EventScript_ApparatusResolved::
	msgbox AetheronSealChamber_Text_ApparatusAfter, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Storm Compass - freed from the wreckage. Gives ITEM_STORM_COMPASS +
@ FLAG_STORM_COMPASS_OBTAINED, then ITEM_SEAL_SHARD_AETHERON, sets
@ FLAG_AETHERON_RESOLVED, and flows into the post-resolution Cass beat
@ and the Sollis call. Reached as the tail of the reinforcement (above);
@ also a standalone examine on the wreckage sign (11,8) - if examined
@ before the reinforcement it just describes the running apparatus.
@ ===================================================================
AetheronSealChamber_EventScript_StormCompass::
	@ Standalone examine path: not yet reinforced -> flavor only.
	goto_if_lt VAR_AETHERON_PROGRESS, 6, AetheronSealChamber_EventScript_CompassNotReady
	goto_if_set FLAG_STORM_COMPASS_OBTAINED, AetheronSealChamber_EventScript_CompassAlready
	lockall
	setspeaker 0
	msgbox AetheronSealChamber_Text_CompassReveal, MSGBOX_DEFAULT
	closemessage
	@ Story flags + key items. RESOLVED set before the gives so a full
	@ key-item pocket can never strand the resolution.
	setflag FLAG_AETHERON_RESOLVED
	setvar VAR_AETHERON_PROGRESS, 6
	giveitem ITEM_STORM_COMPASS
	setflag FLAG_STORM_COMPASS_OBTAINED
	msgbox AetheronSealChamber_Text_CompassGet, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_SEAL_SHARD_AETHERON
	msgbox AetheronSealChamber_Text_SealShardGet, MSGBOX_DEFAULT
	closemessage
	delay 30
	goto AetheronSealChamber_EventScript_PostResolutionCass
	end

AetheronSealChamber_EventScript_CompassNotReady::
	lockall
	setspeaker 0
	msgbox AetheronSealChamber_Text_CompassNotReady, MSGBOX_DEFAULT
	releaseall
	end

AetheronSealChamber_EventScript_CompassAlready::
	lockall
	setspeaker 0
	msgbox AetheronSealChamber_Text_CompassAlready, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Post-resolution Cass dialogue - verbatim. The sudden stillness.
@ Player answers "Convergence." Cass: "Right. (beat) Together then."
@ ===================================================================
AetheronSealChamber_EventScript_PostResolutionCass::
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronSealChamber_Text_CassPost1, MSGBOX_DEFAULT
	setspeaker 0
	msgbox AetheronSealChamber_Text_PlayerNods, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronSealChamber_Text_CassPost2, MSGBOX_DEFAULT
	setspeaker 0
	msgbox AetheronSealChamber_Text_PlayerConvergence, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronSealChamber_Text_CassRight, MSGBOX_DEFAULT
	closemessage
	delay 20
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronSealChamber_Text_CassTogether2, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 30
	goto AetheronSealChamber_EventScript_SollisCall
	end

@ ===================================================================
@ Island resolution - the Sollis PokeNav call. Verbatim. She clocks
@ Cass at the player's side. The call is the in-world signal that
@ Convergence is now reachable (the boat menu's SailToConvergence is
@ already gated on FLAG_AETHERON_RESOLVED, set above). No boat-tier
@ change - the Storm Compass and the open way to Convergence are the
@ rewards.
@ ===================================================================
AetheronSealChamber_EventScript_SollisCall::
	lockall
	setspeaker 0
	delay 20
	msgbox AetheronSealChamber_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox AetheronSealChamber_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 20
	msgbox AetheronSealChamber_Text_ResolutionClose, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Cass in the chamber - talk-initiated. Before the reinforcement she
@ holds her one line; the post-resolution exchange is delivered by the
@ reinforcement chain above, so talking afterward gives a quiet closer.
@ ===================================================================
AetheronSealChamber_EventScript_CassChamber::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Cass
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronSealChamber_EventScript_CassChamberResolved
	msgbox AetheronSealChamber_Text_CassChamberBefore, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronSealChamber_EventScript_CassChamberResolved::
	msgbox AetheronSealChamber_Text_CassChamberAfter, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text - discovery
@ ===================================================================
AetheronSealChamber_Text_Discovery1:
	.string "The stair opens into stillness.\p"
	.string "Above the peak the storm rages on - but\n"
	.string "here, in the eye of it, there is no\l"
	.string "sound at all.\p"
	.string "Light hangs in the air like held\n"
	.string "breath.$"

AetheronSealChamber_Text_Discovery2:
	.string "Something vast turns its attention\n"
	.string "toward you.\p"
	.string "STORMVEIL. Coiled in the silence,\n"
	.string "electric and immense, watching through\l"
	.string "the seal that holds it.$"

AetheronSealChamber_Text_DiscoveryClose:
	.string "Cass stops beside you and looks up at\n"
	.string "it for a long moment.\p"
	.string "“…All right,” she says quietly.\n"
	.string "“Let's mend it.”$"

@ ===================================================================
@ Text - reinforcement
@ ===================================================================
AetheronSealChamber_Text_ApparatusApproach:
	.string "The extraction core hums where the\n"
	.string "cables converge - the override ARC\l"
	.string "described.\p"
	.string "Reinforce the seal and tear the core\n"
	.string "down with it?$"

AetheronSealChamber_Text_ApparatusNotReady:
	.string "The core hums, drawing the storm down\n"
	.string "into itself. You should look at the\l"
	.string "seal first.$"

AetheronSealChamber_Text_ApparatusDecline:
	.string "Not yet. Cass waits, one hand resting\n"
	.string "on the housing.$"

AetheronSealChamber_Text_ApparatusAfter:
	.string "The core is a husk of cooling metal.\n"
	.string "It will never draw breath from this\l"
	.string "place again.$"

AetheronSealChamber_Text_CassTogether:
	.string "Two sets of hands. I've got this side.$"

AetheronSealChamber_Text_Reinforce1:
	.string "You and Cass reach into the seal\n"
	.string "together, and STORMVEIL answers.\p"
	.string "Far overhead, the endless thunder\n"
	.string "changes its voice - lower, slower, like\l"
	.string "a held breath finally let go.$"

AetheronSealChamber_Text_Reinforce2:
	.string "Starved of the seal it was bleeding,\n"
	.string "the extraction core convulses and\l"
	.string "tears itself apart.\p"
	.string "When the sparks settle, the silence is\n"
	.string "complete.$"

@ ===================================================================
@ Text - Storm Compass + Seal Shard
@ ===================================================================
AetheronSealChamber_Text_CompassReveal:
	.string "In the wreckage, something gleams that\n"
	.string "is not Covenant make - older, finer, a\l"
	.string "disc of pale metal etched with the\l"
	.string "marks of the seal-builders.\p"
	.string "It was never the Covenant's. It was\n"
	.string "part of the seal all along.$"

AetheronSealChamber_Text_CompassGet:
	.string "{PLAYER} received the STORM COMPASS.$"

AetheronSealChamber_Text_SealShardGet:
	.string "{PLAYER} received the AETHERON SEAL\n"
	.string "SHARD.\p"
	.string "It crackles faintly, storm-charged, in\n"
	.string "your palm.$"

AetheronSealChamber_Text_CompassNotReady:
	.string "The pale disc is locked deep in the\n"
	.string "running apparatus. You can't reach it\l"
	.string "yet.$"

AetheronSealChamber_Text_CompassAlready:
	.string "Bare wreckage. The STORM COMPASS is\n"
	.string "yours now, riding safe in your pack.$"

@ ===================================================================
@ Text - post-resolution Cass (verbatim)
@ ===================================================================
AetheronSealChamber_Text_CassPost1:
	.string "Is this what it felt like? All the\n"
	.string "others?$"

AetheronSealChamber_Text_PlayerNods:
	.string "You nod.$"

AetheronSealChamber_Text_CassPost2:
	.string "I've been on the other side of this for\n"
	.string "two years. Watching the yield numbers\l"
	.string "go up.\p"
	.string "Telling myself it was stable\n"
	.string "management.\p"
	.string "What do we do now?$"

AetheronSealChamber_Text_PlayerConvergence:
	.string "“Convergence,” you say.$"

AetheronSealChamber_Text_CassRight:
	.string "Right.$"

AetheronSealChamber_Text_CassTogether2:
	.string "Together then.$"

@ ===================================================================
@ Text - Sollis call (verbatim) + close
@ ===================================================================
AetheronSealChamber_Text_PokenavBuzz:
	.string "…Your POKeNAV buzzes.$"

AetheronSealChamber_Text_SollisCall:
	.string "Aetheron readings stabilized.\n"
	.string "STORMVEIL's seal is holding. And -- is\l"
	.string "that Cass?\p"
	.string "Good. I'm glad she's with you.\p"
	.string "Come home soon. Both of you.$"

AetheronSealChamber_Text_ResolutionClose:
	.string "The way to Convergence lies open now.\p"
	.string "Whatever waits at the center of it all,\n"
	.string "you won't face it alone.$"

@ ===================================================================
@ Text - Cass in the chamber (talk-initiated)
@ ===================================================================
AetheronSealChamber_Text_CassChamberBefore:
	.string "The seal's just ahead.\p"
	.string "Let's just get it done.$"

AetheronSealChamber_Text_CassChamberAfter:
	.string "Quiet, isn't it. After all that.\p"
	.string "I spent two years afraid of what I'd\n"
	.string "find if I looked too closely. Turns out\l"
	.string "the looking was the easy part.\p"
	.string "Come on. Convergence won't wait.$"
