Emberveil_LavaRoute2_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Acolyte Rem - TRAINER_CULTMEMBER_EMBERVEIL_4 (sight-initiated)
@ -------------------------------------------------------------------
EmberveilLavaRoute2_EventScript_AcolyteRem::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_4, EmberveilLavaRoute2_Text_RemIntro, EmberveilLavaRoute2_Text_RemDefeat
	setspeaker Pelagios_Speaker_Rem
	msgbox EmberveilLavaRoute2_Text_RemPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Acolyte Voss - TRAINER_CULTMEMBER_EMBERVEIL_5 (sight-initiated)
@ -------------------------------------------------------------------
EmberveilLavaRoute2_EventScript_AcolyteVoss::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_5, EmberveilLavaRoute2_Text_VossIntro, EmberveilLavaRoute2_Text_VossDefeat
	setspeaker Pelagios_Speaker_Voss
	msgbox EmberveilLavaRoute2_Text_VossPost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Acolyte Tane - TRAINER_CULTMEMBER_EMBERVEIL_6 (sight-initiated)
@ -------------------------------------------------------------------
EmberveilLavaRoute2_EventScript_AcolyteTane::
	trainerbattle_single TRAINER_CULTMEMBER_EMBERVEIL_6, EmberveilLavaRoute2_Text_TaneIntro, EmberveilLavaRoute2_Text_TaneDefeat
	setspeaker Pelagios_Speaker_Tane
	msgbox EmberveilLavaRoute2_Text_TanePost, MSGBOX_AUTOCLOSE
	end

@ -------------------------------------------------------------------
@ Lava-Boots heat gate (coord triggers, armed at progress 0-2).
@ Normally unreachable before progress 3 (the city's east gate blocks
@ first) - defensive depth, mirroring Ironhold's rubble pattern.
@ The checkitem branch covers the impossible-but-cheap case of
@ holding the boots while the trigger is still armed.
@ -------------------------------------------------------------------
EmberveilLavaRoute2_EventScript_LavaGate::
	lockall
	checkitem ITEM_LAVA_BOOTS
	goto_if_eq VAR_RESULT, TRUE, EmberveilLavaRoute2_EventScript_LavaGateBoots
	playse SE_M_FLAME_WHEEL
	waitse
	msgbox EmberveilLavaRoute2_Text_LavaGate, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilLavaRoute2_Movement_StepBackWest
	waitmovement 0
	releaseall
	end

EmberveilLavaRoute2_EventScript_LavaGateBoots::
	msgbox EmberveilLavaRoute2_Text_LavaGateBoots, MSGBOX_DEFAULT
	releaseall
	end

EmberveilLavaRoute2_Movement_StepBackWest:
	walk_left
	step_end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilLavaRoute2_Text_RemIntro:
	.string "The caldera is the HIGH EMBER's\n"
	.string "domain.\p"
	.string "Outsiders don't belong there.$"

EmberveilLavaRoute2_Text_RemDefeat:
	.string "…Hm.$"

EmberveilLavaRoute2_Text_RemPost:
	.string "You belong there about as much\n"
	.string "as anyone.$"

EmberveilLavaRoute2_Text_VossIntro:
	.string "I've trained here my whole life.\p"
	.string "The heat doesn't touch me\n"
	.string "anymore.$"

EmberveilLavaRoute2_Text_VossDefeat:
	.string "Hah! Scorched!$"

EmberveilLavaRoute2_Text_VossPost:
	.string "You run hotter than I expected.$"

EmberveilLavaRoute2_Text_TaneIntro:
	.string "Three of us guard this route.\p"
	.string "You won't get through all three.$"

EmberveilLavaRoute2_Text_TaneDefeat:
	.string "…Three for three.$"

EmberveilLavaRoute2_Text_TanePost:
	.string "Apparently you will.$"

EmberveilLavaRoute2_Text_LavaGate:
	.string "The crust ahead glows dull red\n"
	.string "through a web of cracks.\p"
	.string "The heat shoves you back like a\n"
	.string "hand. No crossing this without\l"
	.string "proper boots.$"

EmberveilLavaRoute2_Text_LavaGateBoots:
	.string "The LAVA BOOTS bite into the\n"
	.string "cooling crust.\p"
	.string "The heat is just heat now. You\n"
	.string "can cross.$"
