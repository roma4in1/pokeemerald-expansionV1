Gildhaven_GoldportHarbor_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GildhavenGoldportHarbor_OnTransition
	.byte 0

@ Once the arrival cutscene has played (progress >= 1), the Cass glimpse
@ object is permanently gone from the harbor. FLAG_CASS_GILDHAVEN_SEEN is
@ reserved for the NobleQuarter scene, so the harbor object is removed by
@ a removeobject keyed on the progress var (its map.json hide flag is
@ FLAG_CASS_GILDHAVEN_SEEN, which the harbor never sets - this transition
@ removes it before the map draws).
GildhavenGoldportHarbor_OnTransition::
	compare VAR_GILDHAVEN_PROGRESS, 0
	call_if_ne GildhavenGoldportHarbor_RemoveCass
	end

GildhavenGoldportHarbor_RemoveCass::
	removeobject LOCALID_GILDHAVEN_CASS_HARBOR
	return

@ ===================================================================
@ Arrival cutscene. Fires once at progress 0 (the boat warp lands the
@ player at (10,14); walking north onto row 13 crosses the trigger).
@ Cass is glimpsed across the harbor, turns, and walks off before the
@ player can reach her. Sets FLAG_GILDHAVEN_ARRIVED + progress 1.
@ ===================================================================
GildhavenGoldportHarbor_EventScript_Arrival::
	lockall
	setvar VAR_GILDHAVEN_PROGRESS, 1
	setflag FLAG_GILDHAVEN_ARRIVED
	delay 20
	msgbox GildhavenGoldportHarbor_Text_ArrivalNarration, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ Cass notices the player from across the docks.
	applymovement LOCALID_GILDHAVEN_CASS_HARBOR, GildhavenGoldportHarbor_Movement_CassNotice
	waitmovement 0
	delay 30
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 20
	msgbox GildhavenGoldportHarbor_Text_CassGlimpse, MSGBOX_DEFAULT
	closemessage
	@ Cass turns and walks off the docks before the player can reach her.
	applymovement LOCALID_GILDHAVEN_CASS_HARBOR, GildhavenGoldportHarbor_Movement_CassLeave
	waitmovement 0
	removeobject LOCALID_GILDHAVEN_CASS_HARBOR
	delay 20
	msgbox GildhavenGoldportHarbor_Text_CassGone, MSGBOX_DEFAULT
	releaseall
	end

GildhavenGoldportHarbor_Movement_CassNotice:
	walk_down
	walk_down
	face_down
	delay_16
	step_end

GildhavenGoldportHarbor_Movement_CassLeave:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

@ The Cass object is removed during the arrival scene; this talk script
@ is a defensive fallback only (it should never be reachable).
GildhavenGoldportHarbor_EventScript_CassGlimpse::
	lockall
	releaseall
	end

@ ===================================================================
@ Harbor official - efficient, slightly cold.
@ ===================================================================
GildhavenGoldportHarbor_EventScript_HarborOfficial::
	lockall
	setspeaker Pelagios_Speaker_Official
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenGoldportHarbor_EventScript_HarborOfficialResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenGoldportHarbor_EventScript_HarborOfficialCorrupt
	msgbox GildhavenGoldportHarbor_Text_Official, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

GildhavenGoldportHarbor_EventScript_HarborOfficialCorrupt::
	msgbox GildhavenGoldportHarbor_Text_OfficialCorrupt, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

GildhavenGoldportHarbor_EventScript_HarborOfficialResolved::
	msgbox GildhavenGoldportHarbor_Text_OfficialResolved, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

@ ===================================================================
@ Sailor who has been here too long.
@ ===================================================================
GildhavenGoldportHarbor_EventScript_Sailor::
	lockall
	setspeaker Pelagios_Speaker_Sailor
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenGoldportHarbor_EventScript_SailorResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenGoldportHarbor_EventScript_SailorCorrupt
	msgbox GildhavenGoldportHarbor_Text_Sailor, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

GildhavenGoldportHarbor_EventScript_SailorCorrupt::
	msgbox GildhavenGoldportHarbor_Text_SailorCorrupt, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

GildhavenGoldportHarbor_EventScript_SailorResolved::
	msgbox GildhavenGoldportHarbor_Text_SailorResolved, MSGBOX_NPC
	setspeaker 0
	releaseall
	end

@ ===================================================================
@ Port sign - narration.
@ ===================================================================
GildhavenGoldportHarbor_EventScript_PortSign::
	lockall
	msgbox GildhavenGoldportHarbor_Text_PortSign, MSGBOX_SIGN
	releaseall
	end

@ Boat-boarding wiring (map-builder owns this; NOT dialogue).
GildhavenGoldportHarbor_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_GILDHAVEN
	goto Pelagios_EventScript_BoardTennyson
	end

@ ===================================================================
@ Text
@ ===================================================================
GildhavenGoldportHarbor_Text_ArrivalNarration:
	.string "The TENNYSON slides into a berth of\n"
	.string "polished stone.\p"
	.string "Gold trim. Spotless docks. Flags in\n"
	.string "a dozen colors snapping in the wind.\p"
	.string "Covenant ships ride at anchor\n"
	.string "further out - sleek, grey, patient.\p"
	.string "Everything gleams. It is the\n"
	.string "wealthiest place you have ever seen.\p"
	.string "And something about it sits wrong,\n"
	.string "the way a smile can be too wide.$"

GildhavenGoldportHarbor_Text_CassGlimpse:
	.string "Across the docks, a figure in a grey\n"
	.string "coat turns at the sound of the ship.\p"
	.string "For half a second she looks straight\n"
	.string "at you.\p"
	.string "CASS.$"

GildhavenGoldportHarbor_Text_CassGone:
	.string "She is already walking away, fast,\n"
	.string "into the crowd.\p"
	.string "By the time you reach the rail she\n"
	.string "is gone.$"

GildhavenGoldportHarbor_Text_Official:
	.string "Welcome to GILDHAVEN. The finest\n"
	.string "trading port in the archipelago.\p"
	.string "Dock fees are five hundred credits.\n"
	.string "We accept all currencies.\p"
	.string "There is no enforcement mechanism.\n"
	.string "People simply pay. They want to.$"

GildhavenGoldportHarbor_Text_OfficialCorrupt:
	.string "Welcome to GILDHAVEN. Dock fees\n"
	.string "apply. They have gone up since you\l"
	.string "arrived, actually. They go up daily.\p"
	.string "That is correct. That is good.\n"
	.string "Up is the direction things go here.$"

GildhavenGoldportHarbor_Text_OfficialResolved:
	.string "Dock fees are... I was about to\n"
	.string "quote you a number.\p"
	.string "I have quoted that number ten\n"
	.string "thousand times. Just now I could not\l"
	.string "remember why it mattered.\p"
	.string "Strange morning.$"

GildhavenGoldportHarbor_Text_Sailor:
	.string "I came to deliver cargo six months\n"
	.string "ago.\p"
	.string "I keep finding reasons to stay. Good\n"
	.string "opportunities. Always good ones.\p"
	.string "I am not sure the reasons are real.\p"
	.string "I should leave. I will leave.\n"
	.string "Tomorrow.$"

GildhavenGoldportHarbor_Text_SailorCorrupt:
	.string "Leave? Why would I leave? The\n"
	.string "numbers here are the best I have\l"
	.string "ever seen.\p"
	.string "Every day a little better. I just\n"
	.string "need one more good week. One more.\p"
	.string "Then I will go. Obviously.$"

GildhavenGoldportHarbor_Text_SailorResolved:
	.string "I am leaving today. Actually today.\p"
	.string "I have been saying tomorrow for\n"
	.string "months. Something changed this\l"
	.string "morning.\p"
	.string "I remembered where I was going.$"

GildhavenGoldportHarbor_Text_PortSign:
	.string "GOLDPORT HARBOR\n"
	.string "Gateway to Gildhaven.\l"
	.string "Where every welcome has a price.$"
