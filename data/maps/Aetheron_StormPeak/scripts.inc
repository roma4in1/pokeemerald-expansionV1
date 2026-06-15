Aetheron_StormPeak_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Aetheron_StormPeak_OnTransition
	.byte 0

@ ON_TRANSITION: after the defection (FLAG_CASS_DEFECTED), Cass walks
@ alongside the player here. Her object's hide flag is FLAG_AETHERON_CASS_SEEN
@ (set permanently after the village sighting), so the engine won't spawn
@ her on its own - we force-spawn her with addobject every load once she
@ has defected. This does NOT touch CASS_SEEN, so the village/other Cass
@ objects are unaffected.
Aetheron_StormPeak_OnTransition:
	goto_if_set FLAG_CASS_DEFECTED, Aetheron_StormPeak_EventScript_SpawnCass
	end

Aetheron_StormPeak_EventScript_SpawnCass::
	addobject LOCALID_AETHERON_CASS_PEAK
	return

@ ===================================================================
@ Cass walking alongside - no dialogue until after the seal is
@ reinforced. If talked to here she gives one line: "Let's just get it
@ done." (post-resolution she will not be on this map; she rejoins in
@ the SealChamber.)
@ ===================================================================
AetheronStormPeak_EventScript_CassAlongside::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronStormPeak_Text_CassAlongside, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Arrival trigger (8,17)/(9,17) at VAR_AETHERON_PROGRESS == 4. The
@ player crests the peak with Cass at their side. Brief atmosphere beat,
@ then advances progress to 5 so the SealChamber discovery trigger
@ (progress == 5) arms. Fires once: setvar to 5 disarms this progress==4
@ trigger.
@ ===================================================================
AetheronStormPeak_EventScript_Arrival::
	lockall
	setspeaker 0
	delay 16
	msgbox AetheronStormPeak_Text_Arrival, MSGBOX_DEFAULT
	closemessage
	setvar VAR_AETHERON_PROGRESS, 5
	releaseall
	end

@ ===================================================================
@ Apparatus rig (sign bg_event) - the largest extraction array yet,
@ feeding down into the seal beneath the peak. Flavor examine; the
@ actual override/reinforcement is in the SealChamber below.
@ ===================================================================
AetheronStormPeak_EventScript_Apparatus::
	lockall
	setspeaker 0
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronStormPeak_EventScript_ApparatusResolved
	msgbox AetheronStormPeak_Text_Apparatus, MSGBOX_DEFAULT
	releaseall
	end

AetheronStormPeak_EventScript_ApparatusResolved::
	msgbox AetheronStormPeak_Text_ApparatusResolved, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronStormPeak_Text_CassAlongside:
	.string "Let's just get it done.$"

AetheronStormPeak_Text_Arrival:
	.string "The peak is bare rock and screaming\n"
	.string "wind. Lightning walks the ground in\l"
	.string "white forks, close enough to taste.\p"
	.string "Cass keeps pace at your shoulder. She\n"
	.string "doesn't flinch at the strikes. Neither\l"
	.string "of you says anything.\p"
	.string "The way down to the seal is ahead.$"

AetheronStormPeak_Text_Apparatus:
	.string "The extraction array straddles the peak\n"
	.string "like a fist closed around the storm -\l"
	.string "the biggest the Covenant has built.\p"
	.string "Every cable runs down into the stone,\n"
	.string "toward the seal beneath your feet.$"

AetheronStormPeak_Text_ApparatusResolved:
	.string "The array stands dead and cold. The\n"
	.string "cables hang slack, drawing nothing.\p"
	.string "The storm belongs to the sky again.$"
