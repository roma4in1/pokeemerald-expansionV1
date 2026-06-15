Convergence_AncientCapital_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ConvergenceExterior_OnTransition
	.byte 0

@ ===========================================================================
@ The ruins of the greatest city ever built. Every character the player met
@ across the archipelago is gathered here, positioned per the map.
@ Eira (north) and Drenn (south) carry hide-flag fields in map.json that are
@ the WRONG polarity (an object hides while its hide flag is SET). We drive
@ their visibility explicitly: show Eira only if FLAG_SCHISM_CEASEFIRE, show
@ Drenn only if FLAG_DRENN_ALIVE (Gildhaven-Dagan pattern).
@ ===========================================================================
ConvergenceExterior_OnTransition::
	call_if_unset FLAG_SCHISM_CEASEFIRE, ConvergenceExterior_HideEira
	call_if_set FLAG_SCHISM_CEASEFIRE, ConvergenceExterior_ShowEira
	call_if_unset FLAG_DRENN_ALIVE, ConvergenceExterior_HideDrenn
	call_if_set FLAG_DRENN_ALIVE, ConvergenceExterior_ShowDrenn
	end

ConvergenceExterior_ShowEira::
	addobject LOCALID_CONVERGENCE_EIRA
	return

ConvergenceExterior_HideEira::
	removeobject LOCALID_CONVERGENCE_EIRA
	return

ConvergenceExterior_ShowDrenn::
	addobject LOCALID_CONVERGENCE_DRENN
	return

ConvergenceExterior_HideDrenn::
	removeobject LOCALID_CONVERGENCE_DRENN
	return

@ ---------------------------------------------------------------------------
@ Eira - only present if FLAG_SCHISM_CEASEFIRE. Not a talker.
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Eira::
	lockall
	setspeaker Pelagios_Speaker_Eira
	msgbox ConvergenceExterior_Text_Eira, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Mako - always present.
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Mako::
	lockall
	setspeaker Pelagios_Speaker_Mako
	msgbox ConvergenceExterior_Text_Mako, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Drenn - only present if FLAG_DRENN_ALIVE.
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Drenn::
	lockall
	setspeaker Pelagios_Speaker_Drenn
	msgbox ConvergenceExterior_Text_Drenn, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Lace - always present, running supplies.
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Lace::
	lockall
	setspeaker Pelagios_Speaker_Lace
	msgbox ConvergenceExterior_Text_Lace, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ The Lens - always present; varies on FLAG_DEX_ALIVE.
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Lens::
	lockall
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_DEX_ALIVE, ConvergenceExterior_EventScript_LensDexAlive
	msgbox ConvergenceExterior_Text_LensDexGone, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceExterior_EventScript_LensDexAlive::
	msgbox ConvergenceExterior_Text_LensDexAlive, MSGBOX_DEFAULT
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Dagan - always present. Always funny. VAR_DAGAN_RELATIONSHIP flavors his
@ line. His map object is RICH_BOY at (24,20), hide flag 0 (always shown).
@ Dex has no separate object on this map, so when alive Dagan hands the player
@ off to him (he is leaning where he shouldn't be, as ever).
@ ---------------------------------------------------------------------------
ConvergenceExterior_EventScript_Dagan::
	lockall
	setspeaker Pelagios_Speaker_Dagan
	goto_if_eq VAR_DAGAN_RELATIONSHIP, 0, ConvergenceExterior_EventScript_DaganLow
	goto_if_eq VAR_DAGAN_RELATIONSHIP, 1, ConvergenceExterior_EventScript_DaganMid
	goto ConvergenceExterior_EventScript_DaganHigh

ConvergenceExterior_EventScript_DaganLow::
	msgbox ConvergenceExterior_Text_DaganLow, MSGBOX_DEFAULT
	goto ConvergenceExterior_EventScript_DaganTail

ConvergenceExterior_EventScript_DaganMid::
	msgbox ConvergenceExterior_Text_DaganMid, MSGBOX_DEFAULT
	goto ConvergenceExterior_EventScript_DaganTail

ConvergenceExterior_EventScript_DaganHigh::
	msgbox ConvergenceExterior_Text_DaganHigh, MSGBOX_DEFAULT
	goto ConvergenceExterior_EventScript_DaganTail

ConvergenceExterior_EventScript_DaganTail::
	goto_if_set FLAG_DEX_ALIVE, ConvergenceExterior_EventScript_DaganDex
	releaseall
	end

ConvergenceExterior_EventScript_DaganDex::
	setspeaker Pelagios_Speaker_Dagan
	msgbox ConvergenceExterior_Text_DaganDexHandoff, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ConvergenceExterior_Text_Dex, MSGBOX_DEFAULT
	releaseall
	end

@ ===========================================================================
@ TEXT
@ ===========================================================================
ConvergenceExterior_Text_Eira:
	.string "I brought fighters. In case.$"

ConvergenceExterior_Text_Mako:
	.string "The Root sent us here.\p"
	.string "I didn't know why until I arrived and\n"
	.string "stood under all this. Then I understood.$"

ConvergenceExterior_Text_Drenn:
	.string "Eira and I haven't spoken since the\n"
	.string "Scar.\p"
	.string "We're not going to speak here either.\p"
	.string "But we're both here. That seems like\n"
	.string "something.$"

ConvergenceExterior_Text_Lace:
	.string "Someone has to make sure everyone has\n"
	.string "water.\p"
	.string "It might as well be me. It usually is.$"

ConvergenceExterior_Text_LensDexAlive:
	.string "This is everything Dex was looking for.\p"
	.string "Every style of inscription, every era,\n"
	.string "all in one place.\p"
	.string "He is going to lose his mind. In the\n"
	.string "good way, for once.$"

ConvergenceExterior_Text_LensDexGone:
	.string "This is everything Dex was looking for.\p"
	.string "He never got to see it.\p"
	.string "I'll find a way to tell him. Even if I\n"
	.string "have to write it on the wall.$"

ConvergenceExterior_Text_DaganLow:
	.string "Oh, you're surprised I'm here.\p"
	.string "Everyone always is. What? I was\n"
	.string "invited. Mostly.$"

ConvergenceExterior_Text_DaganMid:
	.string "There you are. I saved you the good\n"
	.string "spot, with the view of the temple.\p"
	.string "Then I ate the snacks that came with\n"
	.string "the good spot. But the view's still\l"
	.string "yours.$"

ConvergenceExterior_Text_DaganHigh:
	.string "I'll be honest. Of all the people I\n"
	.string "expected to genuinely like by the end\l"
	.string "of this, you were not on the list.\p"
	.string "And yet. Go fix the world. I'll keep an\n"
	.string "eye on things. And on the buffet.$"

ConvergenceExterior_Text_DaganDexHandoff:
	.string "Anyway, your historian friend is over\n"
	.string "there refusing to sit down.\p"
	.string "Go tell him to sit down. He won't\n"
	.string "listen to me. Nobody does. It's a gift.$"

ConvergenceExterior_Text_Dex:
	.string "Don't. Don't say it. I'm fine.\p"
	.string "I'll sleep when this is over.\p"
	.string "There is two thousand years of history\n"
	.string "on these walls and I have one pencil.$"
