Emberveil_CalderaApproach_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 3 - Vex (Fire/Flying). Talk-initiated leader, stationed to bar
@ caldera entry. Victory: Caldera Badge (narrative badge - no vanilla
@ FLAG_BADGEXX slots remain), FLAG_EMBERVEIL_GYM3_CLEAR,
@ VAR_EMBERVEIL_PROGRESS = 4 (disarms the VexIntercept triggers),
@ TM Aerial Ace (brief asks for Brave Bird - no such TM in the
@ vanilla set; the substitution is lampshaded).
@ -------------------------------------------------------------------
EmberveilCalderaApproach_EventScript_Vex::
	goto_if_defeated TRAINER_LEADER_EMBERVEIL_VEX, EmberveilCalderaApproach_EventScript_VexDefeated
	setspeaker Pelagios_Speaker_Vex
	trainerbattle_single TRAINER_LEADER_EMBERVEIL_VEX, EmberveilCalderaApproach_Text_VexIntro, EmberveilCalderaApproach_Text_VexDefeat, EmberveilCalderaApproach_EventScript_VexVictory
	end

EmberveilCalderaApproach_EventScript_VexVictory::
	setspeaker Pelagios_Speaker_Vex
	msgbox EmberveilCalderaApproach_Text_VexPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message EmberveilCalderaApproach_Text_ReceivedCalderaBadge
	waitmessage
	waitfanfare
	setflag FLAG_EMBERVEIL_GYM3_CLEAR
	setvar VAR_EMBERVEIL_PROGRESS, 4
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Vex
	msgbox EmberveilCalderaApproach_Text_VexExplainTM, MSGBOX_DEFAULT
	release
	end

EmberveilCalderaApproach_EventScript_VexDefeated::
	lock
	faceplayer
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCalderaApproach_EventScript_VexResolved
	setspeaker Pelagios_Speaker_Vex
	msgbox EmberveilCalderaApproach_Text_VexPostBadge, MSGBOX_DEFAULT
	release
	end

EmberveilCalderaApproach_EventScript_VexResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilCalderaApproach_EventScript_VexPathB
	@ Path A: he says nothing - just watches the community. Narration only.
	msgbox EmberveilCalderaApproach_Text_VexPathA, MSGBOX_DEFAULT
	release
	end

EmberveilCalderaApproach_EventScript_VexPathB::
	setspeaker Pelagios_Speaker_Vex
	msgbox EmberveilCalderaApproach_Text_VexPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Vex intercept - coord triggers at the corridor mouth (row 8),
@ armed at progress 3 (boots obtained, Vex undefeated). The corridor
@ at x=10-11 is the only way north, so he can't be slipped past.
@ -------------------------------------------------------------------
EmberveilCalderaApproach_EventScript_VexIntercept::
	lockall
	playse SE_PIN
	applymovement LOCALID_EMBERVEIL_VEX, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_EMBERVEIL_VEX, Common_Movement_FaceDown
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Vex
	msgbox EmberveilCalderaApproach_Text_VexInterceptLine, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilCalderaApproach_Movement_StepBackSouth
	waitmovement 0
	releaseall
	end

@ -------------------------------------------------------------------
@ Lava-Boots heat gate (coord triggers, armed at progress 0-2).
@ -------------------------------------------------------------------
EmberveilCalderaApproach_EventScript_LavaGate::
	lockall
	checkitem ITEM_LAVA_BOOTS
	goto_if_eq VAR_RESULT, TRUE, EmberveilCalderaApproach_EventScript_LavaGateBoots
	playse SE_M_FLAME_WHEEL
	waitse
	msgbox EmberveilCalderaApproach_Text_LavaGate, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilCalderaApproach_Movement_StepBackSouth
	waitmovement 0
	releaseall
	end

EmberveilCalderaApproach_EventScript_LavaGateBoots::
	msgbox EmberveilCalderaApproach_Text_LavaGateBoots, MSGBOX_DEFAULT
	releaseall
	end

EmberveilCalderaApproach_Movement_StepBackSouth:
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Heat sign
@ -------------------------------------------------------------------
EmberveilCalderaApproach_EventScript_HeatSign::
	msgbox EmberveilCalderaApproach_Text_HeatSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilCalderaApproach_Text_VexIntro:
	.string "The caldera is sacred. The HIGH\n"
	.string "EMBER's instructions are clear:\l"
	.string "no outsiders past this point.\p"
	.string "I follow the HIGH EMBER.$"

EmberveilCalderaApproach_Text_VexDefeat:
	.string "…$"

EmberveilCalderaApproach_Text_VexPostBattle:
	.string "…Go. I won't stop you.\p"
	.string "But if you hurt her - if you\n"
	.string "hurt this community -\p"
	.string "I will find you.$"

EmberveilCalderaApproach_Text_ReceivedCalderaBadge:
	.string "{PLAYER} received the CALDERA\n"
	.string "BADGE from VEX!$"

EmberveilCalderaApproach_Text_VexExplainTM:
	.string "AERIAL ACE. I wanted BRAVE BIRD\n"
	.string "pressed for you - no one makes\l"
	.string "that TM.\p"
	.string "Fitting. The brave way isn't\n"
	.string "something you can hand to\l"
	.string "somebody.$"

EmberveilCalderaApproach_Text_VexPostBadge:
	.string "She's at the summit. The ritual\n"
	.string "is nearly complete.\p"
	.string "Remember what I said.$"

EmberveilCalderaApproach_Text_VexPathA:
	.string "VEX stands at his post, watching\n"
	.string "the thin lines of smoke rise\l"
	.string "from CINDERHOLD's chimneys.\p"
	.string "He sees you. He says nothing.\p"
	.string "There is nothing left for him\n"
	.string "to guard, and he guards it\l"
	.string "anyway.$"

EmberveilCalderaApproach_Text_VexPathB:
	.string "She came down from the summit\n"
	.string "herself. To ME. First.\p"
	.string "Explained the seal. The commune.\n"
	.string "Your parent's notes.\p"
	.string "I was ready to fight you to the\n"
	.string "end, you know.\p"
	.string "I have never been so glad to\n"
	.string "lose.$"

EmberveilCalderaApproach_Text_VexInterceptLine:
	.string "The caldera is sacred.\p"
	.string "You want the road north, you\n"
	.string "come through me. Properly.\l"
	.string "Face to face.$"

EmberveilCalderaApproach_Text_LavaGate:
	.string "A band of cooling lava seals the\n"
	.string "way north, its crust crazed\l"
	.string "with glowing seams.\p"
	.string "The heat is a wall. Without\n"
	.string "fireproof boots, this is as far\l"
	.string "as anyone goes.$"

EmberveilCalderaApproach_Text_LavaGateBoots:
	.string "The LAVA BOOTS grip the crazed\n"
	.string "crust, and the glowing seams\l"
	.string "hold your weight.\p"
	.string "You can cross.$"

EmberveilCalderaApproach_Text_HeatSign:
	.string "The lettering on this sign has\n"
	.string "half-melted down its post.\p"
	.string "“CALDERA APPROACH - THE MOUNTAIN\n"
	.string "IS AWAKE HERE. WALK GENTLY.”$"
