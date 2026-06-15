Primalis_JungleRoute2_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 3 - Thorn (Grass/Dragon). Met on the route. Talk-initiated,
@ gated on FLAG_PRIMALIS_GYM2_CLEAR (and Mako's first meeting having
@ sent the player here). Victory sets FLAG_PRIMALIS_GYM3_CLEAR +
@ progress 4. Badge NARRATIVE-ONLY. TM: no Power Whip TM exists; Thorn
@ gives TM Bullet Seed (a thrashing Grass move) and lampshades it.
@ ===================================================================
PrimalisJungleRoute2_EventScript_Thorn::
	goto_if_defeated TRAINER_LEADER_PRIMALIS_THORN, PrimalisJungleRoute2_EventScript_ThornDefeated
	goto_if_unset FLAG_PRIMALIS_GYM2_CLEAR, PrimalisJungleRoute2_EventScript_ThornLocked
	setspeaker Pelagios_Speaker_Thorn
	trainerbattle_single TRAINER_LEADER_PRIMALIS_THORN, PrimalisJungleRoute2_Text_ThornIntro, PrimalisJungleRoute2_Text_ThornDefeat, PrimalisJungleRoute2_EventScript_ThornVictory
	end

PrimalisJungleRoute2_EventScript_ThornLocked::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Thorn
	msgbox PrimalisJungleRoute2_Text_ThornLocked, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleRoute2_EventScript_ThornVictory::
	setspeaker Pelagios_Speaker_Thorn
	msgbox PrimalisJungleRoute2_Text_ThornPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message PrimalisJungleRoute2_Text_ReceivedThornBadge
	waitmessage
	waitfanfare
	setflag FLAG_PRIMALIS_GYM3_CLEAR
	setvar VAR_PRIMALIS_PROGRESS, 4
	setspeaker Pelagios_Speaker_Thorn
	msgbox PrimalisJungleRoute2_Text_ThornDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_BULLET_SEED
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Thorn
	msgbox PrimalisJungleRoute2_Text_ThornTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleRoute2_EventScript_ThornDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Thorn
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisJungleRoute2_EventScript_ThornResolved
	msgbox PrimalisJungleRoute2_Text_ThornPostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleRoute2_EventScript_ThornResolved::
	msgbox PrimalisJungleRoute2_Text_ThornResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Zoan villager (former 5th sight trainer, converted to a talk NPC -
@ only four TRAINER_ZOAN_PRIMALIS ids exist). A guardian who has
@ stepped back from the rotation; battle-flavor + lore.
@ ===================================================================
PrimalisJungleRoute2_EventScript_Zoan5::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Guardian
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisJungleRoute2_EventScript_Zoan5Resolved
	msgbox PrimalisJungleRoute2_Text_Zoan5, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleRoute2_EventScript_Zoan5Resolved::
	msgbox PrimalisJungleRoute2_Text_Zoan5Resolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Beast-Whistle gate - the NORTH EXIT to the Heartwood approach
@ (AncientRuinsCamp). Reached only after Gym4 + the oral history,
@ when the Whistle is held. Same deferred-field-effect approach as
@ Route1: bounce south until the Whistle is held, then let through.
@ Tile-clearing field effect is DEFERRED to systems (known limit).
@ ===================================================================
PrimalisJungleRoute2_EventScript_BeastGate::
	lockall
	checkitem ITEM_BEAST_WHISTLE
	goto_if_eq VAR_RESULT, TRUE, PrimalisJungleRoute2_EventScript_BeastGateOpen
	msgbox PrimalisJungleRoute2_Text_BeastGateBlocked, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PrimalisJungleRoute2_Movement_BounceSouth
	waitmovement 0
	releaseall
	end

PrimalisJungleRoute2_EventScript_BeastGateOpen::
	playse SE_M_GRASSWHISTLE
	msgbox PrimalisJungleRoute2_Text_BeastGateOpen, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

PrimalisJungleRoute2_Movement_BounceSouth:
	walk_down
	step_end

@ ===================================================================
@ Text - Thorn (Gym 3)
@ ===================================================================
PrimalisJungleRoute2_Text_ThornLocked:
	.string "You have not beaten SCALE. I can smell\n"
	.string "it on you - the village has not let you\l"
	.string "through.\p"
	.string "Come back when it has.$"

PrimalisJungleRoute2_Text_ThornIntro:
	.string "Elder MAKO told me about you.\p"
	.string "He said you might be the one the Root\n"
	.string "has been waiting for.\p"
	.string "He also said to make sure. I am making\n"
	.string "sure.$"

PrimalisJungleRoute2_Text_ThornDefeat:
	.string "...Sure.$"

PrimalisJungleRoute2_Text_ThornPostBattle:
	.string "You are sure.\p"
	.string "The Heartwood is past the ruins camp.\n"
	.string "Elder MAKO will go with you.$"

PrimalisJungleRoute2_Text_ReceivedThornBadge:
	.string "{PLAYER} received the THORN BADGE.$"

PrimalisJungleRoute2_Text_ThornDirections:
	.string "Go back to MAKO. He will want to speak\n"
	.string "to you himself now. Properly.\p"
	.string "The thing he tells almost no one - he\n"
	.string "will tell you.$"

PrimalisJungleRoute2_Text_ThornTM:
	.string "Take this. We keep no POWER WHIP - but\n"
	.string "BULLET SEED is the jungle's own answer.\p"
	.string "A hundred small strikes. The forest\n"
	.string "does not win with one blow. It wins by\l"
	.string "never stopping.$"

PrimalisJungleRoute2_Text_ThornPostBadge:
	.string "MAKO is in his hall. Go. The Heartwood\n"
	.string "is past the ruins, north - but you will\l"
	.string "need the WHISTLE to reach it.$"

PrimalisJungleRoute2_Text_ThornResolved:
	.string "I made sure of you. I am glad I was\n"
	.string "wrong to doubt.\p"
	.string "The Root is quiet now. I can feel it\n"
	.string "the way you feel a held breath finally\l"
	.string "let go.$"

@ ===================================================================
@ Text - Zoan villager (former trainer 5)
@ ===================================================================
PrimalisJungleRoute2_Text_Zoan5:
	.string "I used to stand a post on this route.\n"
	.string "Tested every soul who came through.\p"
	.string "Then the changing started going wrong,\n"
	.string "and I stopped trusting my own claws in\l"
	.string "a fight. Stepped back.\p"
	.string "The four who still guard the approaches\n"
	.string "are better than me now. Earn past them\l"
	.string "and you have earned past me.$"

PrimalisJungleRoute2_Text_Zoan5Resolved:
	.string "I flexed my hand this morning and it\n"
	.string "was just a hand. Mine. Five fingers.\p"
	.string "I think I might take a post again.\n"
	.string "There is no shame in it anymore.$"

@ ===================================================================
@ Text - Beast gate (north exit)
@ ===================================================================
PrimalisJungleRoute2_Text_BeastGateBlocked:
	.string "The way north is sealed by a wall of\n"
	.string "living growth - older and denser than\l"
	.string "anything on the lower routes.\p"
	.string "This is the Heartwood's threshold. It\n"
	.string "does not open for force.$"

PrimalisJungleRoute2_Text_BeastGateOpen:
	.string "You raise the BEAST WHISTLE. The note\n"
	.string "rolls out low and the wall of growth\l"
	.string "breathes inward, parting.\p"
	.string "The Heartwood is letting you in.$"
