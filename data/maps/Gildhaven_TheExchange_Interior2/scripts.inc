Gildhaven_TheExchange_Interior2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Gildhaven_TheExchange_Interior2_OnTransition
	.byte 0

@ Lace's post-battle cameo object (LOCALID_GILDHAVEN_LACE_EXCHANGE)
@ uses FLAG_TEMP_1 as its hide flag. Temp flags clear on every map
@ load, which would make her visible on entry - so the transition
@ re-hides her. She is revealed only during the Serel post-battle
@ scene (clearflag + addobject), then this transition hides her again
@ on any later entry.
Gildhaven_TheExchange_Interior2_OnTransition:
	setflag FLAG_TEMP_1
	end

@ ===================================================================
@ Gym 4 - Merchant Lord Serel Vane (Fairy/Dark). Talk-initiated. He
@ is not the villain of his own story and knows exactly what he is.
@ Victory: post-battle speech, then the Serel/Lace choreography (Lace
@ enters from the side door, a silent look, Serel leaves ruined, Lace
@ speaks to the player). Sets FLAG_GILDHAVEN_GYM4_CLEAR + progress 5.
@ Badge narrative-only (engine badge flags exhausted). TM: no Moonblast
@ TM exists -> TM Shadow Ball (powerful special) lampshaded.
@ ===================================================================
GildhavenTheExchangeInterior2_EventScript_Serel::
	goto_if_defeated TRAINER_LEADER_GILDHAVEN_SEREL, GildhavenTheExchangeInterior2_EventScript_SerelDefeated
	setspeaker Pelagios_Speaker_Serel
	trainerbattle_single TRAINER_LEADER_GILDHAVEN_SEREL, GildhavenTheExchangeInterior2_Text_SerelIntro, GildhavenTheExchangeInterior2_Text_SerelDefeat, GildhavenTheExchangeInterior2_EventScript_SerelVictory
	end

GildhavenTheExchangeInterior2_EventScript_SerelVictory::
	setspeaker Pelagios_Speaker_Serel
	msgbox GildhavenTheExchangeInterior2_Text_SerelPost1, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	@ Serel walks to the window and looks out at the city.
	applymovement LOCALID_GILDHAVEN_SEREL, GildhavenTheExchangeInterior2_Movement_SerelToWindow
	waitmovement 0
	delay 30
	setspeaker Pelagios_Speaker_Serel
	msgbox GildhavenTheExchangeInterior2_Text_SerelPost2, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	@ Lace enters from the side door.
	playse SE_DOOR
	clearflag FLAG_TEMP_1
	addobject LOCALID_GILDHAVEN_LACE_EXCHANGE
	applymovement LOCALID_GILDHAVEN_LACE_EXCHANGE, GildhavenTheExchangeInterior2_Movement_LaceEnter
	waitmovement 0
	delay 20
	@ A silent look between father and daughter - no dialogue.
	applymovement LOCALID_GILDHAVEN_SEREL, GildhavenTheExchangeInterior2_Movement_SerelFaceLace
	waitmovement 0
	delay 40
	applymovement LOCALID_GILDHAVEN_LACE_EXCHANGE, GildhavenTheExchangeInterior2_Movement_LaceFaceSerel
	waitmovement 0
	delay 50
	@ Serel leaves - not arrested, just ruined.
	applymovement LOCALID_GILDHAVEN_SEREL, GildhavenTheExchangeInterior2_Movement_SerelLeave
	waitmovement 0
	removeobject LOCALID_GILDHAVEN_SEREL
	delay 30
	@ Lace turns to the player.
	applymovement LOCALID_GILDHAVEN_LACE_EXCHANGE, GildhavenTheExchangeInterior2_Movement_LaceFacePlayer
	waitmovement 0
	setspeaker Pelagios_Speaker_Lace
	msgbox GildhavenTheExchangeInterior2_Text_LaceThanks, MSGBOX_DEFAULT
	setspeaker 0
	@ Badge + flags. Badge is narrative-only.
	playfanfare MUS_OBTAIN_BADGE
	message GildhavenTheExchangeInterior2_Text_ReceivedExchangeBadge
	waitmessage
	waitfanfare
	setflag FLAG_GILDHAVEN_GYM4_CLEAR
	setvar VAR_GILDHAVEN_PROGRESS, 5
	giveitem ITEM_TM_SHADOW_BALL
	goto_if_eq VAR_RESULT, FALSE, GildhavenTheExchangeInterior2_EventScript_SerelFinish
	msgbox GildhavenTheExchangeInterior2_Text_BadgeNote, MSGBOX_DEFAULT
GildhavenTheExchangeInterior2_EventScript_SerelFinish::
	closemessage
	@ Lace withdraws toward the side door, leaving the way down open.
	applymovement LOCALID_GILDHAVEN_LACE_EXCHANGE, GildhavenTheExchangeInterior2_Movement_LaceWithdraw
	waitmovement 0
	removeobject LOCALID_GILDHAVEN_LACE_EXCHANGE
	releaseall
	end

GildhavenTheExchangeInterior2_EventScript_SerelDefeated::
	lockall
	msgbox GildhavenTheExchangeInterior2_Text_SerelGone, MSGBOX_DEFAULT
	releaseall
	end

@ ----- Movements -----
GildhavenTheExchangeInterior2_Movement_SerelToWindow:
	walk_left
	walk_left
	walk_up
	face_up
	step_end

GildhavenTheExchangeInterior2_Movement_LaceEnter:
	walk_down
	walk_down
	walk_left
	face_left
	step_end

GildhavenTheExchangeInterior2_Movement_SerelFaceLace:
	face_right
	step_end

GildhavenTheExchangeInterior2_Movement_LaceFaceSerel:
	face_up
	step_end

GildhavenTheExchangeInterior2_Movement_SerelLeave:
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	step_end

GildhavenTheExchangeInterior2_Movement_LaceFacePlayer:
	face_down
	step_end

GildhavenTheExchangeInterior2_Movement_LaceWithdraw:
	walk_right
	walk_right
	walk_up
	step_end

@ ===================================================================
@ Covenant yield map - the exploitation plan, written out. Significant
@ lore beat. Sets FLAG_GILDHAVEN_COVENANT_MAP_SEEN.
@ ===================================================================
GildhavenTheExchangeInterior2_EventScript_CovenantMap::
	lockall
	goto_if_set FLAG_GILDHAVEN_COVENANT_MAP_SEEN, GildhavenTheExchangeInterior2_EventScript_CovenantMapAgain
	msgbox GildhavenTheExchangeInterior2_Text_CovenantMap1, MSGBOX_DEFAULT
	msgbox GildhavenTheExchangeInterior2_Text_CovenantMap2, MSGBOX_DEFAULT
	msgbox GildhavenTheExchangeInterior2_Text_CovenantMap3, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_GILDHAVEN_COVENANT_MAP_SEEN
	releaseall
	end

GildhavenTheExchangeInterior2_EventScript_CovenantMapAgain::
	msgbox GildhavenTheExchangeInterior2_Text_CovenantMapAgain, MSGBOX_DEFAULT
	releaseall
	end

@ Defensive fallback (the Lace cameo object is removed at scene end).
GildhavenTheExchangeInterior2_EventScript_LacePostBattle::
	lockall
	releaseall
	end

@ ===================================================================
@ Text - Serel
@ ===================================================================
GildhavenTheExchangeInterior2_Text_SerelIntro:
	.string "I have been expecting someone like\n"
	.string "you for years.\p"
	.string "The WARDEN's child, following the\n"
	.string "seals from island to island.\p"
	.string "I want you to understand something\n"
	.string "before we battle.\p"
	.string "I am not the villain of this story.\p"
	.string "I am a man who found a resource and\n"
	.string "used it. The same as every city ever\l"
	.string "built. The same as every civilization\l"
	.string "that came before.\p"
	.string "They sealed MIRATH to keep it from\n"
	.string "people like me.\p"
	.string "I simply found it anyway.$"

GildhavenTheExchangeInterior2_Text_SerelDefeat:
	.string "...Ah.\p"
	.string "Yes. I see.$"

GildhavenTheExchangeInterior2_Text_SerelPost1:
	.string "You have won.\p"
	.string "...\p"
	.string "The seal will hold now. The city will\n"
	.string "change.$"

GildhavenTheExchangeInterior2_Text_SerelPost2:
	.string "You are going to watch forty years of\n"
	.string "accumulated prosperity dissolve in a\l"
	.string "season.\p"
	.string "I hope the people who live here\n"
	.string "appreciate what you have done for\l"
	.string "them.\p"
	.string "...\p"
	.string "I do not think they will.$"

GildhavenTheExchangeInterior2_Text_LaceThanks:
	.string "Thank you.\p"
	.string "...\p"
	.string "I do not know if he ever actually\n"
	.string "loved this place. Or just what it\l"
	.string "gave him.\p"
	.string "I do not know if that matters.\p"
	.string "...\p"
	.string "It matters. I have decided it\n"
	.string "matters.$"

GildhavenTheExchangeInterior2_Text_ReceivedExchangeBadge:
	.string "{PLAYER} received the EXCHANGE BADGE.$"

GildhavenTheExchangeInterior2_Text_BadgeNote:
	.string "LACE presses something into your hand -\n"
	.string "her father's last gym token, and a TM.\p"
	.string "TM - SHADOW BALL.\p"
	.string "He fought with MOONBLAST, she says.\n"
	.string "There is no press for that one. SHADOW\l"
	.string "BALL is the closest the old machines\l"
	.string "can manage. He would have hated the\l"
	.string "substitution. Take it anyway.$"

GildhavenTheExchangeInterior2_Text_SerelGone:
	.string "The upper office is empty.\p"
	.string "A fine coat hangs on a hook by the\n"
	.string "window. Its owner did not come back\l"
	.string "for it.\p"
	.string "The hidden stair stands open in the\n"
	.string "far wall.$"

@ ===================================================================
@ Text - Covenant map
@ ===================================================================
GildhavenTheExchangeInterior2_Text_CovenantMap1:
	.string "A map of the entire PELAGIOS\n"
	.string "archipelago covers the wall.\p"
	.string "Each island carries a notation in a\n"
	.string "neat, clerical hand.$"

GildhavenTheExchangeInterior2_Text_CovenantMap2:
	.string "HAVEN ISLE: legacy node, low yield.\p"
	.string "IRONHOLD: military site, contested\n"
	.string "access.\p"
	.string "SIROCCO: depleted, write-off.\p"
	.string "EMBERVEIL: volatile, monitoring\n"
	.string "required.\p"
	.string "SCHISM: unstable twin system, high\n"
	.string "risk.\p"
	.string "THALVERN: primary target, extraction\n"
	.string "pending.\p"
	.string "GILDHAVEN: current site, active\n"
	.string "management.$"

GildhavenTheExchangeInterior2_Text_CovenantMap3:
	.string "This is the COVENANT's plan. Written\n"
	.string "out. Filed. Approved.\p"
	.string "Every legendary. Every island.\n"
	.string "Every yield estimate.\p"
	.string "They do not believe the legendaries\n"
	.string "are myths at all. They never did.\p"
	.string "They are an inventory.$"

GildhavenTheExchangeInterior2_Text_CovenantMapAgain:
	.string "The yield map. Every seal in the\n"
	.string "archipelago, priced like grain.\p"
	.string "You will not forget it. That is\n"
	.string "rather the point of having seen it.$"
