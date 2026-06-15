Primalis_TheHeartwood_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Primalis_TheHeartwood_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Primalis_TheHeartwood_OnFrame
	.byte 0

@ ===================================================================
@ The Heartwood Mako + Lens objects carry map.json hide flag
@ FLAG_PRIMALIS_RUINS_FOUND, but they should APPEAR here once the
@ ruins altar has been read (engine hides while the flag is SET -
@ wrong direction). Drive both explicitly: show after RUINS_FOUND,
@ hide before. (Gildhaven hide-flag-direction pattern.)
@ ===================================================================
Primalis_TheHeartwood_OnTransition:
	goto_if_unset FLAG_PRIMALIS_RUINS_FOUND, Primalis_TheHeartwood_HideEscort
	addobject LOCALID_PRIMALIS_HEARTWOOD_MAKO
	addobject LOCALID_PRIMALIS_HEARTWOOD_LENS
	end

Primalis_TheHeartwood_HideEscort:
	removeobject LOCALID_PRIMALIS_HEARTWOOD_MAKO
	removeobject LOCALID_PRIMALIS_HEARTWOOD_LENS
	end

@ ===================================================================
@ Atmospheric first-entry reaction - the jungle responds to the
@ player's presence. Fires once per session (VAR_TEMP_0) once the
@ player has the run of the Heartwood (progress >= 5). Ambient sound,
@ the root-knot stirs. No permanent flag needed - it is pure flavor.
@ ===================================================================
Primalis_TheHeartwood_OnFrame:
	map_script_2 VAR_TEMP_0, 0, Primalis_TheHeartwood_EventScript_Ambient
	.2byte 0

Primalis_TheHeartwood_EventScript_Ambient::
	lockall
	setvar VAR_TEMP_0, 1
	goto_if_lt VAR_PRIMALIS_PROGRESS, 5, Primalis_TheHeartwood_EventScript_AmbientDone
	delay 20
	playse SE_M_GRASSWHISTLE
	delay 30
	msgbox Primalis_TheHeartwood_Text_Ambient1, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_SUDOWOODO_SHAKE
	delay 20
	msgbox Primalis_TheHeartwood_Text_Ambient2, MSGBOX_DEFAULT
	closemessage
Primalis_TheHeartwood_EventScript_AmbientDone::
	releaseall
	end

@ ===================================================================
@ Mako escort - he came with the player to the Heartwood, as Thorn
@ promised. He waits at the threshold of the inner sanctum.
@ ===================================================================
PrimalisTheHeartwood_EventScript_Mako::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mako
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisTheHeartwood_EventScript_MakoResolved
	goto_if_set FLAG_PRIMALIS_SEAL_FOUND, PrimalisTheHeartwood_EventScript_MakoSealFound
	msgbox PrimalisTheHeartwood_Text_Mako, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisTheHeartwood_EventScript_MakoSealFound::
	msgbox PrimalisTheHeartwood_Text_MakoSealFound, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisTheHeartwood_EventScript_MakoResolved::
	msgbox PrimalisTheHeartwood_Text_MakoResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ The Lens - she would not miss this for the world.
@ ===================================================================
PrimalisTheHeartwood_EventScript_Lens::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisTheHeartwood_EventScript_LensResolved
	msgbox PrimalisTheHeartwood_Text_Lens, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisTheHeartwood_EventScript_LensResolved::
	msgbox PrimalisTheHeartwood_Text_LensResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ The root-knot - the centerpiece of the nave. The visible heart of
@ the Root's reach into the world above the seal.
@ ===================================================================
PrimalisTheHeartwood_EventScript_RootKnot::
	lockall
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisTheHeartwood_EventScript_RootKnotResolved
	msgbox PrimalisTheHeartwood_Text_RootKnot, MSGBOX_DEFAULT
	releaseall
	end

PrimalisTheHeartwood_EventScript_RootKnotResolved::
	msgbox PrimalisTheHeartwood_Text_RootKnotResolved, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - ambient
@ ===================================================================
Primalis_TheHeartwood_Text_Ambient1:
	.string "The moment you cross the threshold, the\n"
	.string "Heartwood goes quiet.\p"
	.string "Not the quiet of emptiness. The quiet\n"
	.string "of being noticed.$"

Primalis_TheHeartwood_Text_Ambient2:
	.string "Far down the long green nave, a knot of\n"
	.string "ancient root the size of a house shifts\l"
	.string "- slowly, deliberately - and turns what\l"
	.string "might be a face toward you.\p"
	.string "The whole jungle is one creature. And\n"
	.string "it is looking at you.$"

@ ===================================================================
@ Text - Mako (escort)
@ ===================================================================
PrimalisTheHeartwood_Text_Mako:
	.string "I have stood in this nave perhaps ten\n"
	.string "times in my life. It never grows\l"
	.string "smaller.\p"
	.string "The seal is below, down the stair past\n"
	.string "the root-knot. Reinforce it, child.\p"
	.string "I will wait here. This is your\n"
	.string "inheritance, not mine. I only kept the\l"
	.string "way open until you came.$"

PrimalisTheHeartwood_Text_MakoSealFound:
	.string "You have seen it now. The Root, and the\n"
	.string "seal that holds it.\p"
	.string "Reinforce it. I have waited my whole\n"
	.string "life to watch someone do this and mean\l"
	.string "it.$"

PrimalisTheHeartwood_Text_MakoResolved:
	.string "The Root sleeps easy. I feel it in my\n"
	.string "own old bones - the part of me that is\l"
	.string "shark and the part that is still a man,\l"
	.string "both at rest.\p"
	.string "Go home now. You have a harder hall\n"
	.string "than mine waiting, and questions in it.$"

@ ===================================================================
@ Text - the Lens (Heartwood)
@ ===================================================================
PrimalisTheHeartwood_Text_Lens:
	.string "I am not allowed past the threshold -\n"
	.string "guardian rules - and honestly I am\l"
	.string "relieved.\p"
	.string "I have read every account of this place\n"
	.string "and not one of them prepared me for\l"
	.string "standing in it. Some things resist\l"
	.string "being written down.$"

PrimalisTheHeartwood_Text_LensResolved:
	.string "You did it. I felt the whole nave\n"
	.string "exhale.\p"
	.string "I am going to spend the rest of my life\n"
	.string "trying to describe this and failing.\l"
	.string "Worth it.$"

@ ===================================================================
@ Text - root-knot
@ ===================================================================
PrimalisTheHeartwood_Text_RootKnot:
	.string "The root-knot breathes. You are sure of\n"
	.string "it now - a slow swell and settle, like\l"
	.string "a sleeper.\p"
	.string "Where you stand near it, the air is\n"
	.string "warm and green and very old. Something\l"
	.string "vast is dreaming just under your feet.$"

PrimalisTheHeartwood_Text_RootKnotResolved:
	.string "The root-knot's breathing is even now.\n"
	.string "Deep. Untroubled.\p"
	.string "A sleeper who was having a nightmare,\n"
	.string "and is not anymore.$"
