HavenIsle_AncientRuinsInterior1_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HavenIsle_AncientRuinsInterior1_OnFrame
	.byte 0

@ VAR_HAVEN_RUINS_STATE: 1 = armed (starter chosen) -> disturbance plays on
@ entry; it sets 2, so the Ralts scene chains on the very next frame.
@ The two-entry table also recovers cleanly if a save sits at state 2.
HavenIsle_AncientRuinsInterior1_OnFrame:
	map_script_2 VAR_HAVEN_RUINS_STATE, 1, AncientRuinsInterior1_EventScript_Disturbance
	map_script_2 VAR_HAVEN_RUINS_STATE, 2, AncientRuinsInterior1_EventScript_RaltsScene
	.2byte 0

AncientRuinsInterior1_EventScript_Disturbance::
	lockall
	delay 30
	playse SE_THUNDER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox AncientRuinsInterior1_Text_ChamberShudders, MSGBOX_DEFAULT
	closemessage
	setvar VAR_HAVEN_RUINS_STATE, 2
	releaseall
	end

AncientRuinsInterior1_EventScript_RaltsScene::
	lockall
	delay 10
	fadescreen FADE_TO_WHITE
	clearflag FLAG_HIDE_RUINS_RALTS
	addobject LOCALID_RUINS_RALTS
	fadescreen FADE_FROM_WHITE
	playse SE_PIN
	applymovement LOCALID_RUINS_RALTS, Common_Movement_ExclamationMark
	waitmovement 0
	waitse
	playmoncry SPECIES_RALTS, CRY_MODE_ENCOUNTER
	msgbox AncientRuinsInterior1_Text_RaltsAppears, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RUINS_RALTS, AncientRuinsInterior1_Movement_RaltsFlee
	waitmovement 0
	removeobject LOCALID_RUINS_RALTS
	setflag FLAG_HIDE_RUINS_RALTS
	msgbox AncientRuinsInterior1_Text_RaltsGone, MSGBOX_DEFAULT
	closemessage
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox AncientRuinsInterior1_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox AncientRuinsInterior1_Text_SollisCall, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HAVEN_RUINS_VISITED
	setvar VAR_HAVEN_RUINS_STATE, 3
	releaseall
	end

@ From (16,19): two west, two north - all verified floor tiles - then despawn
@ into the cave depths.
AncientRuinsInterior1_Movement_RaltsFlee:
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	step_end

AncientRuinsInterior1_EventScript_GlowingTile::
	lockall
	goto_if_eq VAR_HAVEN_RUINS_STATE, 3, AncientRuinsInterior1_EventScript_GlowWarm
	msgbox AncientRuinsInterior1_Text_GlowDormant, MSGBOX_DEFAULT
	releaseall
	end

AncientRuinsInterior1_EventScript_GlowWarm::
	msgbox AncientRuinsInterior1_Text_GlowWarm, MSGBOX_DEFAULT
	releaseall
	end

AncientRuinsInterior1_Text_ChamberShudders:
	.string "The chamber shudders!\p"
	.string "Dust sifts down from the ceiling,\n"
	.string "and for half a heartbeat every\l"
	.string "carving on the walls glows from\l"
	.string "within -\p"
	.string "- and then everything is still.\n"
	.string "Too still.$"

AncientRuinsInterior1_Text_RaltsAppears:
	.string "Light floods the chamber!\p"
	.string "When your eyes adjust, something\n"
	.string "small is standing in the middle\l"
	.string "of the chamber, trembling.\p"
	.string "A RALTS…?\p"
	.string "It stares at you - through you -\n"
	.string "eyes wide with a fear that doesn't\l"
	.string "feel like its own.$"

AncientRuinsInterior1_Text_RaltsGone:
	.string "It's gone.\p"
	.string "The chamber is silent again, but\n"
	.string "the air still prickles, the way it\l"
	.string "does before a storm makes landfall.\p"
	.string "Whatever frightened that RALTS\n"
	.string "didn't come from inside these walls.\p"
	.string "It came from underneath them.$"

AncientRuinsInterior1_Text_PokenavBuzz:
	.string "…Your POKéNAV buzzes.$"

AncientRuinsInterior1_Text_SollisCall:
	.string "{PLAYER}! My instruments\n"
	.string "just went off the chart - that\l"
	.string "came from the ruins!\p"
	.string "Are you alright? What did you see?\p"
	.string "…No. Not over the POKéNAV.\p"
	.string "Come back to the lab and tell me\n"
	.string "everything.$"

AncientRuinsInterior1_Text_GlowDormant:
	.string "One floor tile is worn smoother\n"
	.string "than the rest, carved with a spiral\l"
	.string "of fine lines.\p"
	.string "It is faintly, impossibly warm.$"

AncientRuinsInterior1_Text_GlowWarm:
	.string "The spiral tile pulses with a slow,\n"
	.string "fading glow - like an ember, or a\l"
	.string "heartbeat winding down.\p"
	.string "Touching it fills your head with\n"
	.string "the sound of deep water.$"
