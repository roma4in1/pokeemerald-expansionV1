Sirocco_BuriedCity_Interior1_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 3 - Miria (Ground/Rock). Dagan's lieutenant, cataloguing the
@ ruins for him at the far end of the first chamber. Talk-initiated
@ (TRAINER_TYPE_NONE - gym leaders are never sight trainers here).
@ Victory: Gilt Badge (FLAG_BADGE08_GET), FLAG_SIROCCO_GYM3_CLEAR,
@ then TM Dragon Claw (as briefed).
@ VAR_SIROCCO_PROGRESS advances to 4 ONLY from exactly 3 - the player
@ can reach her before Crag (the dig is open after Gym 1), and the
@ Miraden north-gate triggers are armed at progress 1/2; an early
@ jump to 4 would open Dagan's territory without Gym 2.
@ -------------------------------------------------------------------
SiroccoBuriedCityInterior1_EventScript_Miria::
	goto_if_defeated TRAINER_LEADER_SIROCCO_MIRIA, SiroccoBuriedCityInterior1_EventScript_MiriaDefeated
	setspeaker Pelagios_Speaker_Miria
	trainerbattle_single TRAINER_LEADER_SIROCCO_MIRIA, SiroccoBuriedCityInterior1_Text_MiriaIntro, SiroccoBuriedCityInterior1_Text_MiriaDefeat, SiroccoBuriedCityInterior1_EventScript_MiriaVictory
	end

SiroccoBuriedCityInterior1_EventScript_MiriaVictory::
	msgbox SiroccoBuriedCityInterior1_Text_MiriaPostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message SiroccoBuriedCityInterior1_Text_ReceivedGiltBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE08_GET
	setflag FLAG_SIROCCO_GYM3_CLEAR
	call_if_eq VAR_SIROCCO_PROGRESS, 3, SiroccoBuriedCityInterior1_EventScript_AdvanceProgress
	setspeaker Pelagios_Speaker_Miria
	msgbox SiroccoBuriedCityInterior1_Text_MiriaHellSeeYou, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_DRAGON_CLAW
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Miria
	msgbox SiroccoBuriedCityInterior1_Text_MiriaExplainTM, MSGBOX_DEFAULT
	release
	end

SiroccoBuriedCityInterior1_EventScript_AdvanceProgress::
	setvar VAR_SIROCCO_PROGRESS, 4
	return

SiroccoBuriedCityInterior1_EventScript_MiriaDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Miria
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoBuriedCityInterior1_EventScript_MiriaResolved
	goto_if_set FLAG_SIROCCO_DAGAN_ESCAPED, SiroccoBuriedCityInterior1_EventScript_MiriaAfterEscape
	msgbox SiroccoBuriedCityInterior1_Text_MiriaPostBadge, MSGBOX_DEFAULT
	release
	end

SiroccoBuriedCityInterior1_EventScript_MiriaAfterEscape::
	msgbox SiroccoBuriedCityInterior1_Text_MiriaAfterEscape, MSGBOX_DEFAULT
	release
	end

SiroccoBuriedCityInterior1_EventScript_MiriaResolved::
	msgbox SiroccoBuriedCityInterior1_Text_MiriaResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoBuriedCityInterior1_Text_MiriaIntro:
	.string "DAGAN said someone would get this\n"
	.string "far eventually.\p"
	.string "He said to evaluate whether you\n"
	.string "were worth his time.\p"
	.string "Defeat me, and he'll see you\n"
	.string "himself.$"

SiroccoBuriedCityInterior1_Text_MiriaDefeat:
	.string "Evaluation complete.$"

SiroccoBuriedCityInterior1_Text_MiriaPostBattle:
	.string "She recalls her POKéMON and makes\n"
	.string "a small, precise note in her\l"
	.string "ledger before she looks at you\l"
	.string "again.$"

SiroccoBuriedCityInterior1_Text_ReceivedGiltBadge:
	.string "{PLAYER} received the GILT BADGE\n"
	.string "from MIRIA!$"

SiroccoBuriedCityInterior1_Text_MiriaHellSeeYou:
	.string "He'll be impressed. He respects\n"
	.string "competence.\p"
	.string "Just - don't expect him to change\n"
	.string "his plans.\p"
	.string "He never does.$"

SiroccoBuriedCityInterior1_Text_MiriaExplainTM:
	.string "DRAGON CLAW. We found a cache of\n"
	.string "these in the lower galleries.\p"
	.string "DAGAN took everything else.\n"
	.string "I logged this one as “broken.”\p"
	.string "I am occasionally inaccurate.$"

SiroccoBuriedCityInterior1_Text_MiriaPostBadge:
	.string "The compound is north of MIRADEN,\n"
	.string "past the territory gate.\p"
	.string "He knows you're coming.\n"
	.string "He has known for days.\p"
	.string "It doesn't worry him. Make of\n"
	.string "that what you will.$"

SiroccoBuriedCityInterior1_Text_MiriaAfterEscape:
	.string "He's gone, then. He had a boat\n"
	.string "ready. He always has a boat\l"
	.string "ready.\p"
	.string "The deep chamber is past the\n"
	.string "mural hall. Whatever his machine\l"
	.string "is doing down there - go end it.\p"
	.string "I'll see that nobody follows\n"
	.string "you.$"

SiroccoBuriedCityInterior1_Text_MiriaResolved:
	.string "I'm still cataloguing the ruins.\p"
	.string "For myself, this time. The work\n"
	.string "was always good. The ledger it\l"
	.string "went into wasn't.\p"
	.string "DEX talks too fast and files\n"
	.string "nothing. We balance.$"
