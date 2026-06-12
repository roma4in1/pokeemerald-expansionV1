Sirocco_BuriedCity_Exterior_MapScripts::
	.byte 0

@ Grunt Pell - TRAINER_GILTCLAW_SIROCCO_3 (sight-initiated)
SiroccoBuriedCityExterior_EventScript_GruntPell::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_3, SiroccoBuriedCityExterior_Text_PellIntro, SiroccoBuriedCityExterior_Text_PellDefeat
	setspeaker Pelagios_Speaker_Pell
	msgbox SiroccoBuriedCityExterior_Text_PellPost, MSGBOX_AUTOCLOSE
	end

@ Grunt Sera - TRAINER_GILTCLAW_SIROCCO_4 (sight-initiated)
SiroccoBuriedCityExterior_EventScript_GruntSera::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_4, SiroccoBuriedCityExterior_Text_SeraIntro, SiroccoBuriedCityExterior_Text_SeraDefeat
	setspeaker Pelagios_Speaker_Sera
	msgbox SiroccoBuriedCityExterior_Text_SeraPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Discovery cutscene - first entry (coord triggers, progress 1 or 2;
@ the flag guard makes it strictly once). The player takes in the
@ scale of the excavation; if Dex has been met, his dig markers and
@ a pinned note carry his voice into the scene.
@ Sets FLAG_SIROCCO_BURIED_CITY_FOUND.
@ -------------------------------------------------------------------
SiroccoBuriedCityExterior_EventScript_Discovery::
	goto_if_set FLAG_SIROCCO_BURIED_CITY_FOUND, SiroccoBuriedCityExterior_EventScript_DiscoveryEnd
	lockall
	delay 16
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	delay 20
	msgbox SiroccoBuriedCityExterior_Text_DiscoveryScale, MSGBOX_DEFAULT
	closemessage
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoBuriedCityExterior_Movement_CameraPanRight
	waitmovement 0
	delay 40
	msgbox SiroccoBuriedCityExterior_Text_DiscoveryCity, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoBuriedCityExterior_Movement_CameraPanBack
	waitmovement 0
	special RemoveCameraObject
	goto_if_set FLAG_SIROCCO_DEX_MET, SiroccoBuriedCityExterior_EventScript_DiscoveryDexNote
	msgbox SiroccoBuriedCityExterior_Text_DiscoveryNoDex, MSGBOX_DEFAULT
	goto SiroccoBuriedCityExterior_EventScript_DiscoveryDone

SiroccoBuriedCityExterior_EventScript_DiscoveryDexNote::
	msgbox SiroccoBuriedCityExterior_Text_DiscoveryDexNote, MSGBOX_DEFAULT
SiroccoBuriedCityExterior_EventScript_DiscoveryDone::
	closemessage
	setflag FLAG_SIROCCO_BURIED_CITY_FOUND
	releaseall
SiroccoBuriedCityExterior_EventScript_DiscoveryEnd::
	end

SiroccoBuriedCityExterior_Movement_CameraPanRight:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SiroccoBuriedCityExterior_Movement_CameraPanBack:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ -------------------------------------------------------------------
@ Exposed wall - the ancient script, third cross-island sighting.
@ Flavor lore only; the cipher itself is the Interior2 mural.
@ -------------------------------------------------------------------
SiroccoBuriedCityExterior_EventScript_AncientScript::
	lockall
	msgbox SiroccoBuriedCityExterior_Text_AncientScript, MSGBOX_DEFAULT
	goto_if_unset FLAG_CIPHER_1_FOUND, SiroccoBuriedCityExterior_EventScript_AncientScriptDone
	msgbox SiroccoBuriedCityExterior_Text_AncientScriptFamiliar, MSGBOX_DEFAULT
SiroccoBuriedCityExterior_EventScript_AncientScriptDone::
	releaseall
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoBuriedCityExterior_Text_PellIntro:
	.string "Nobody's supposed to be down\n"
	.string "here.\p"
	.string "DAGAN's not going to like this.$"

SiroccoBuriedCityExterior_Text_PellDefeat:
	.string "…Oh no.$"

SiroccoBuriedCityExterior_Text_PellPost:
	.string "Going to like it even less now.$"

SiroccoBuriedCityExterior_Text_SeraIntro:
	.string "I don't get paid enough for\n"
	.string "this.$"

SiroccoBuriedCityExterior_Text_SeraDefeat:
	.string "Called it.$"

SiroccoBuriedCityExterior_Text_SeraPost:
	.string "I DEFINITELY don't get paid\n"
	.string "enough for this.$"

SiroccoBuriedCityExterior_Text_DiscoveryScale:
	.string "The dunes fall away ahead of you-\p"
	.string "-and the desert floor opens like\n"
	.string "a wound.$"

SiroccoBuriedCityExterior_Text_DiscoveryCity:
	.string "Stone avenues. Archways. The\n"
	.string "shoulders of buildings, shrugging\l"
	.string "free of a thousand years of sand.\p"
	.string "This wasn't a settlement.\n"
	.string "This was a CITY - and the\l"
	.string "excavation has barely touched\l"
	.string "its rooftops.$"

SiroccoBuriedCityExterior_Text_DiscoveryNoDex:
	.string "Wooden stakes with weathered\n"
	.string "flags mark out a careful grid\l"
	.string "across the site.\p"
	.string "Someone has been digging here\n"
	.string "with great patience - and, by\l"
	.string "the look of the snapped stakes\l"
	.string "near the road, against great\l"
	.string "resistance.$"

SiroccoBuriedCityExterior_Text_DiscoveryDexNote:
	.string "DEX's dig markers grid the whole\n"
	.string "site, each one labeled in his\l"
	.string "eager, sprawling hand.\p"
	.string "A note is pinned under a stone\n"
	.string "on the nearest crate:\p"
	.string "“If you're reading this, you made\n"
	.string "it! Main stairwell is at the old\l"
	.string "doorway, northeast. Touch NOTHING\l"
	.string "the Gilt Claw has touched.\l"
	.string "Touch everything else. - D.”$"

SiroccoBuriedCityExterior_Text_AncientScript:
	.string "Lines of ancient script run along\n"
	.string "the exposed wall, every character\l"
	.string "cut deep and sure.\p"
	.string "Sand pours from the grooves as\n"
	.string "the wind works them, like the\l"
	.string "wall is still being written.$"

SiroccoBuriedCityExterior_Text_AncientScriptFamiliar:
	.string "You've seen this script before.\p"
	.string "Carved in the ruins above\n"
	.string "TIDEMARK VILLAGE. Scratched into\l"
	.string "the cave under IRONHOLD's pass.\p"
	.string "Three islands. One hand.\p"
	.string "Whoever they were, they were\n"
	.string "EVERYWHERE.$"
