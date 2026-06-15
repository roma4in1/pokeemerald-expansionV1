Schism_TheScar_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Schism_TheScar_OnTransition
	.byte 0

@ The two standing leaders use FLAG_TEMP_2 as their hide flag (temp
@ flags clear on warp = object visible by default). This ON_TRANSITION
@ HIDES them (sets FLAG_TEMP_2) every load UNTIL the ceasefire is
@ achieved, after which it leaves them visible - so they appear only
@ as the post-ceasefire standing NPCs, never during normal play.
Schism_TheScar_OnTransition:
	goto_if_set FLAG_SCHISM_CEASEFIRE, Schism_TheScar_OnTransition_ShowLeaders
	setflag FLAG_TEMP_2
	end

Schism_TheScar_OnTransition_ShowLeaders:
	clearflag FLAG_TEMP_2
	end

@ ===================================================================
@ THE SCAR - the no-man's-land between the two factions.
@ Holds: the central ruins (sets FLAG_SCAR_RUINS_FOUND), Dorne's
@ footprints, the ceasefire-meeting cutscene, and the two post-
@ ceasefire standing leaders (Eira + Drenn, both hidden behind
@ FLAG_SCHISM_CEASEFIRE - they appear only AFTER the ceasefire).
@ ===================================================================

@ -------------------------------------------------------------------
@ Central ruins (sign at 13,9). Examining them once triggers the
@ unified-island vision and sets FLAG_SCAR_RUINS_FOUND - the key that
@ unlocks the ceasefire dialogue with both leaders.
@ -------------------------------------------------------------------
SchismTheScar_EventScript_CentralRuins::
	lockall
	goto_if_set FLAG_SCAR_RUINS_FOUND, SchismTheScar_EventScript_CentralRuinsAgain
	delay 20
	msgbox SchismTheScar_Text_RuinsApproach, MSGBOX_DEFAULT
	closemessage
	delay 20
	fadescreen FADE_TO_WHITE
	delay 30
	msgbox SchismTheScar_Text_RuinsVision, MSGBOX_DEFAULT
	closemessage
	delay 20
	fadescreen FADE_FROM_WHITE
	delay 20
	msgbox SchismTheScar_Text_RuinsRealize, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SCAR_RUINS_FOUND
	releaseall
	end

SchismTheScar_EventScript_CentralRuinsAgain::
	msgbox SchismTheScar_Text_RuinsAgain, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Dorne's footprints (sign at 20,12). Lore - first direct evidence of
@ Dorne's active presence this far into the game.
@ -------------------------------------------------------------------
SchismTheScar_EventScript_Footprints::
	lockall
	msgbox SchismTheScar_Text_Footprints, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ THE CEASEFIRE MEETING - the most important scene on the island.
@ Coord trigger at (13,11)/(14,11), armed when
@ VAR_SCHISM_CEASEFIRE_PROGRESS == 2 (both leaders willing). The
@ silence is the point: Eira and Drenn walk in from OPPOSITE sides,
@ a long pause, EIRA speaks first, DRENN responds, another silence,
@ neither moves toward the other, both leave SEPARATELY.
@ The scene SETS FLAG_SCHISM_CEASEFIRE up front so addobject reveals
@ the two leaders (their object hide flag IS FLAG_SCHISM_CEASEFIRE),
@ and after they walk back to their spawns they remain as the post-
@ ceasefire standing NPCs the brief describes.
@ -------------------------------------------------------------------
SchismTheScar_EventScript_CeasefireScene::
	lockall
	delay 30
	@ Player is at (13,11) or (14,11). Face up toward the ruins.
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	delay 30
	@ Reveal both leaders at their spawns (5,9 Eira west / 22,9 Drenn east).
	@ FLAG_TEMP_2 is set (this load's ON_TRANSITION hid them); clear it and
	@ addobject so they appear. FLAG_SCHISM_CEASEFIRE is set so future loads'
	@ ON_TRANSITION keeps them shown as the standing post-ceasefire NPCs.
	setflag FLAG_SCHISM_CEASEFIRE
	clearflag FLAG_TEMP_2
	addobject LOCALID_SCHISM_SCAR_EIRA
	addobject LOCALID_SCHISM_SCAR_DRENN
	delay 20
	@ They walk in from opposite sides toward the ruins, stopping well
	@ apart with the player and the ruins between them.
	applymovement LOCALID_SCHISM_SCAR_EIRA, SchismTheScar_Movement_EiraIn
	applymovement LOCALID_SCHISM_SCAR_DRENN, SchismTheScar_Movement_DrennIn
	waitmovement 0
	@ A long, deliberate silence. Neither speaks. Neither approaches.
	delay 80
	applymovement LOCALID_SCHISM_SCAR_EIRA, Common_Movement_FaceUp
	applymovement LOCALID_SCHISM_SCAR_DRENN, Common_Movement_FaceUp
	waitmovement 0
	delay 80
	@ Eira speaks first.
	setspeaker Pelagios_Speaker_Eira
	msgbox SchismTheScar_Text_CeasefireEira, MSGBOX_DEFAULT
	closemessage
	delay 50
	@ Drenn responds. One word.
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismTheScar_Text_CeasefireDrenn, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ Another silence. Neither shakes hands.
	delay 80
	msgbox SchismTheScar_Text_CeasefireSilence, MSGBOX_DEFAULT
	closemessage
	delay 30
	@ Both turn and walk back to their own sides - separately.
	applymovement LOCALID_SCHISM_SCAR_EIRA, SchismTheScar_Movement_EiraOut
	applymovement LOCALID_SCHISM_SCAR_DRENN, SchismTheScar_Movement_DrennOut
	waitmovement 0
	delay 30
	setvar VAR_SCHISM_PROGRESS, 6
	setvar VAR_SCHISM_CEASEFIRE_PROGRESS, 3
	releaseall
	end

@ Eira: from (5,9) east toward (10,9), then down to (10,10) facing up.
SchismTheScar_Movement_EiraIn:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	face_up
	step_end

@ Drenn: from (22,9) west toward (17,9), then down to (17,10) facing up.
SchismTheScar_Movement_DrennIn:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	face_up
	step_end

@ Eira returns west to her spawn (5,9).
SchismTheScar_Movement_EiraOut:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_down
	step_end

@ Drenn returns east to his spawn (22,9).
SchismTheScar_Movement_DrennOut:
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_down
	step_end

@ -------------------------------------------------------------------
@ Shared willing-check. Called by the Eira (IceCity) and Drenn (Lab)
@ ceasefire-willing scripts. When BOTH leaders have agreed to meet,
@ arm the ceasefire meeting by setting VAR_SCHISM_CEASEFIRE_PROGRESS
@ to 2 (the value TheScar's coord trigger fires on). Until then it
@ leaves the var alone (it may sit at 1 = Drenn engaged/skeptical).
@ -------------------------------------------------------------------
SchismCommon_EventScript_CheckBothWilling::
	goto_if_unset FLAG_EIRA_CEASEFIRE_WILLING, SchismCommon_EventScript_CheckBothWilling_Done
	goto_if_unset FLAG_DRENN_CEASEFIRE_WILLING, SchismCommon_EventScript_CheckBothWilling_Done
	setvar VAR_SCHISM_CEASEFIRE_PROGRESS, 2
SchismCommon_EventScript_CheckBothWilling_Done::
	return

@ -------------------------------------------------------------------
@ Post-ceasefire standing leaders. They appear at their spawns (hide
@ flag FLAG_TEMP_2, managed by ON_TRANSITION) and watch the border.
@ -------------------------------------------------------------------
SchismTheScar_EventScript_EiraCeasefire::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Eira
	msgbox SchismTheScar_Text_EiraStanding, MSGBOX_DEFAULT
	release
	end

SchismTheScar_EventScript_DrennCeasefire::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismTheScar_Text_DrennStanding, MSGBOX_DEFAULT
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismTheScar_Text_RuinsApproach:
	.string "Grey ash lies over everything\n"
	.string "here, undisturbed.\p"
	.string "At the center of the dead ground\n"
	.string "stands a ruin - half-buried\l"
	.string "stone, older than either city.\p"
	.string "You rest a hand against it.$"

SchismTheScar_Text_RuinsVision:
	.string "The ash is gone.\p"
	.string "Where the dead ground was, a\n"
	.string "single settlement stands whole -\l"
	.string "green and white woven together,\l"
	.string "one harbor, one square, one\l"
	.string "people.\p"
	.string "Far off, two great shapes move in\n"
	.string "the distance. One of ice. One of\l"
	.string "poison.\p"
	.string "They do not fight. They keep the\n"
	.string "balance between them, and the\l"
	.string "island breathes.$"

SchismTheScar_Text_RuinsRealize:
	.string "The vision fades. The ash\n"
	.string "returns.\p"
	.string "The ruins show a single\n"
	.string "settlement - the same\l"
	.string "architecture as both cities,\l"
	.string "because they were built by the\l"
	.string "same people.\p"
	.string "This was one place.\p"
	.string "EIRA should see this. So should\n"
	.string "DRENN.$"

SchismTheScar_Text_RuinsAgain:
	.string "The half-buried stone hums faintly\n"
	.string "under your hand.\p"
	.string "One settlement. One people.\n"
	.string "Before the split.$"

SchismTheScar_Text_Footprints:
	.string "Fresh tracks in the ash. Recently\n"
	.string "made.\p"
	.string "Someone stood here, looking at\n"
	.string "the ruins.\p"
	.string "Then they walked toward the\n"
	.string "poison side.\p"
	.string "The tracks are precise.\n"
	.string "Purposeful. Whoever this was,\l"
	.string "they weren't lost.$"

SchismTheScar_Text_CeasefireEira:
	.string "This was one place.$"

SchismTheScar_Text_CeasefireDrenn:
	.string "Yes.$"

SchismTheScar_Text_CeasefireSilence:
	.string "Neither of them moves toward the\n"
	.string "other.\p"
	.string "There is no handshake. No treaty.\n"
	.string "No forgiveness offered or asked.\p"
	.string "Only two people standing in the\n"
	.string "ruins of something they both\l"
	.string "lost, agreeing - without saying\l"
	.string "it - to stop making it worse.$"

SchismTheScar_Text_EiraStanding:
	.string "I come here sometimes now. To the\n"
	.string "border.\p"
	.string "I don't cross it. He doesn't\n"
	.string "cross it.\p"
	.string "But we both stand where it used\n"
	.string "to not exist. That's further than\l"
	.string "I thought we'd get.$"

SchismTheScar_Text_DrennStanding:
	.string "I still don't trust her.\p"
	.string "She still doesn't trust me.\p"
	.string "But we're both done weakening the\n"
	.string "seals. Both done burying our\l"
	.string "people for a lie DORNE handed us.\p"
	.string "That will have to be enough.$"
