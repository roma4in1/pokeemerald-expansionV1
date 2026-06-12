Ironhold_SummitFortress_Interior2_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 4 - Commander Holt Sever. Gated on FLAG_IRONHOLD_GYM3_CLEAR.
@ Victory plays the key scene:
@  - no badge speech; the Covenant Badge (FLAG_BADGE05_GET) and TM are
@    handed over wordlessly,
@  - Sever crosses to the SealChamber door and unlocks it,
@  - his one line, then the choice:
@      YES ("I understand.")  -> VAR_SEVER_RELATIONSHIP = 1 (Respected)
@      NO  (say nothing)      -> stays 0 (Unacknowledged)
@  - FLAG_IRONHOLD_GYM4_CLEAR, VAR_IRONHOLD_PROGRESS = 5.
@ He does not speak again on this island, and his object is hidden
@ entirely after FLAG_IRONHOLD_RESOLVED (object hide flag in map.json).
@ (Brief asks for a TM of Meteor Mash - no such TM in this project's
@ vanilla TM set; Iron Tail substituted. The brief's third dialogue
@ option "That's not enough." folds into the say-nothing branch.)
@ -------------------------------------------------------------------
IronholdFortressInterior2_EventScript_Sever::
	goto_if_defeated TRAINER_LEADER_IRONHOLD_SEVER, IronholdFortressInterior2_EventScript_SeverDefeated
	goto_if_unset FLAG_IRONHOLD_GYM3_CLEAR, IronholdFortressInterior2_EventScript_SeverNotYet
	setspeaker Pelagios_Speaker_Sever
	trainerbattle_single TRAINER_LEADER_IRONHOLD_SEVER, IronholdFortressInterior2_Text_SeverIntro, IronholdFortressInterior2_Text_SeverDefeat, IronholdFortressInterior2_EventScript_SeverKeyScene
	end

IronholdFortressInterior2_EventScript_SeverNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sever
	msgbox IronholdFortressInterior2_Text_SeverNotYet, MSGBOX_DEFAULT
	release
	end

IronholdFortressInterior2_EventScript_SeverKeyScene::
	@ No badge speech. The handover is wordless (narration - no namebox).
	setspeaker 0
	msgbox IronholdFortressInterior2_Text_WordlessHandover, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message IronholdFortressInterior2_Text_ReceivedCovenantBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE05_GET
	giveitem ITEM_TM_IRON_TAIL
	@ (Deliberately no bag-full branch: the key scene must not abort.
	@ If the bag is full the TM is skipped.)
	closemessage
	delay 20
	@ Sever crosses to the SealChamber door. Repositioned under a fade
	@ so no walking path can be blocked by the player.
	fadescreen FADE_TO_BLACK
	setobjectxy LOCALID_IRONHOLD_SEVER, 9, 3
	turnobject LOCALID_IRONHOLD_SEVER, DIR_NORTH
	fadescreen FADE_FROM_BLACK
	delay 20
	playse SE_UNLOCK
	waitse
	msgbox IronholdFortressInterior2_Text_UnlocksDoor, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IRONHOLD_SEVER, IronholdFortressInterior2_Movement_SeverFaceSouth
	waitmovement 0
	copyobjectxytoperm LOCALID_IRONHOLD_SEVER
	setspeaker Pelagios_Speaker_Sever
	msgbox IronholdFortressInterior2_Text_SeverKeyLine, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox IronholdFortressInterior2_Text_ChoicePrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, IronholdFortressInterior2_EventScript_ChoiceUnderstand
	msgbox IronholdFortressInterior2_Text_ChoiceSilence, MSGBOX_DEFAULT
	goto IronholdFortressInterior2_EventScript_KeySceneEnd

IronholdFortressInterior2_EventScript_ChoiceUnderstand::
	msgbox IronholdFortressInterior2_Text_ChoiceUnderstood, MSGBOX_DEFAULT
	setvar VAR_SEVER_RELATIONSHIP, 1
	goto IronholdFortressInterior2_EventScript_KeySceneEnd

IronholdFortressInterior2_EventScript_KeySceneEnd::
	closemessage
	setflag FLAG_IRONHOLD_GYM4_CLEAR
	setvar VAR_IRONHOLD_PROGRESS, 5
	msgbox IronholdFortressInterior2_Text_StepsAside, MSGBOX_DEFAULT
	release
	end

IronholdFortressInterior2_Movement_SeverFaceSouth:
	face_down
	step_end

@ He does not speak again on this island.
IronholdFortressInterior2_EventScript_SeverDefeated::
	lock
	faceplayer
	msgbox IronholdFortressInterior2_Text_SeverSilent, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Region map on the wall (worldbuilding)
@ -------------------------------------------------------------------
IronholdFortressInterior2_EventScript_RegionMap::
	msgbox IronholdFortressInterior2_Text_RegionMap, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdFortressInterior2_Text_SeverNotYet:
	.string "My lieutenant holds the floor\n"
	.string "below. ROOK.\p"
	.string "If you cannot answer him, you\n"
	.string "are not ready to ask me\l"
	.string "anything.$"

IronholdFortressInterior2_Text_SeverIntro:
	.string "I know why you're here.\p"
	.string "I know what you'll find in that\n"
	.string "chamber.\p"
	.string "I've known for three years.\p"
	.string "Defeat me, and I won't stop you.$"

IronholdFortressInterior2_Text_SeverDefeat:
	.string "…So be it.$"

IronholdFortressInterior2_Text_WordlessHandover:
	.string "SEVER says nothing.\p"
	.string "No speech. No ceremony.\p"
	.string "He takes the COVENANT BADGE from\n"
	.string "his coat and sets it in your\l"
	.string "hand, along with a worn TM case.$"

IronholdFortressInterior2_Text_ReceivedCovenantBadge:
	.string "{PLAYER} received the COVENANT\n"
	.string "BADGE from SEVER!$"

IronholdFortressInterior2_Text_UnlocksDoor:
	.string "SEVER crosses the room and turns\n"
	.string "a key that has clearly never\l"
	.string "left his person.\p"
	.string "The seal chamber door unlocks.$"

IronholdFortressInterior2_Text_SeverKeyLine:
	.string "Your parent came here six months\n"
	.string "ago.\p"
	.string "I told them the same thing I'm\n"
	.string "telling you now:\p"
	.string "I know. I won't act.\p"
	.string "I'm sorry.$"

IronholdFortressInterior2_Text_ChoicePrompt:
	.string "He doesn't wait for an answer.\n"
	.string "He doesn't seem to expect one.\p"
	.string "Tell him you understand?$"

IronholdFortressInterior2_Text_ChoiceUnderstood:
	.string "“…I understand.”\p"
	.string "SEVER goes very still.\p"
	.string "Then he closes his eyes, once,\n"
	.string "slowly - the way a man does when\l"
	.string "he sets something heavy down.$"

IronholdFortressInterior2_Text_ChoiceSilence:
	.string "You say nothing.\p"
	.string "Some things don't have answers.\n"
	.string "He knows that better than\l"
	.string "anyone.$"

IronholdFortressInterior2_Text_StepsAside:
	.string "SEVER steps aside from the door.\p"
	.string "He does not speak again.$"

IronholdFortressInterior2_Text_SeverSilent:
	.string "SEVER watches the dark window.\p"
	.string "He does not speak.$"

IronholdFortressInterior2_Text_RegionMap:
	.string "A chart of all PELAGIOS covers\n"
	.string "the wall - finer than any map\l"
	.string "you've sailed by.\p"
	.string "HAVEN ISLE, small and green at\n"
	.string "the edge. IRONHOLD. The sand\l"
	.string "shoals of SIROCCO, the fire\l"
	.string "glow of EMBERVEIL.\p"
	.string "Beyond them: a split island, a\n"
	.string "sunken crown, a golden harbor…\l"
	.string "and far north, an island inked\l"
	.string "entirely in black.\p"
	.string "Nine islands are marked with the\n"
	.string "same small spiral.\p"
	.string "Someone has counted them in\n"
	.string "pencil. 1 through 9.$"
