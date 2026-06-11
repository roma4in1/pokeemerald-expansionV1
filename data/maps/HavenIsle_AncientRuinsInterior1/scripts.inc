HavenIsle_AncientRuinsInterior1_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HavenIsle_AncientRuinsInterior1_OnFrame
	.byte 0

HavenIsle_AncientRuinsInterior1_OnFrame:
	map_script_2 VAR_HAVEN_RUINS_STATE, 2, AncientRuinsInterior1_EventScript_RaltsScene
	.2byte 0

AncientRuinsInterior1_EventScript_RaltsScene::
	lockall
	delay 30
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
	setflag FLAG_HAVEN_RUINS_VISITED
	setvar VAR_HAVEN_RUINS_STATE, 3
	releaseall
	end

AncientRuinsInterior1_Movement_RaltsFlee:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_fast_down
	walk_fast_down
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

AncientRuinsInterior1_Text_RaltsAppears:
	.string "Light floods the chamber!\p"
	.string "When your eyes adjust, something\n"
	.string "small is standing on the centre\l"
	.string "tile, trembling.\p"
	.string "A RALTS…?\p"
	.string "It stares at you — through you —\n"
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

AncientRuinsInterior1_Text_GlowDormant:
	.string "One floor tile is worn smoother\n"
	.string "than the rest, carved with a spiral\l"
	.string "of fine lines.\p"
	.string "It is faintly, impossibly warm.$"

AncientRuinsInterior1_Text_GlowWarm:
	.string "The spiral tile pulses with a slow,\n"
	.string "fading glow — like an ember, or a\l"
	.string "heartbeat winding down.\p"
	.string "Touching it fills your head with\n"
	.string "the sound of deep water.$"
