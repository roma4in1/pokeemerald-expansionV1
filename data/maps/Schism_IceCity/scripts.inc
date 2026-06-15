Schism_IceCity_MapScripts::
	.byte 0

@ ===================================================================
@ ICE CITY - the ice faction's fortress settlement. Eira (Gym 2) is an
@ in-overworld NPC at the command post (14,6). Talk-initiated leader
@ (TRAINER_TYPE_NONE) so her battle/dialogue can be flag-gated.
@ ===================================================================

@ -------------------------------------------------------------------
@ Frost Commander Eira. States:
@   - pre-Gym1: turns the player away (command room "locked")
@   - post-Gym1, pre-defeat: Gym 2 battle
@   - post-defeat: gives the ice Scar pass, advances progress
@   - after ScarRuins found: agrees to meet at the Scar (willing)
@ -------------------------------------------------------------------
SchismIceCity_EventScript_Eira::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Eira
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismIceCity_EventScript_EiraCeasefire
	goto_if_set FLAG_EIRA_CEASEFIRE_WILLING, SchismIceCity_EventScript_EiraWaiting
	goto_if_set FLAG_SCHISM_GYM2_CLEAR, SchismIceCity_EventScript_EiraPostBattle
	goto_if_unset FLAG_SCHISM_GYM1_CLEAR, SchismIceCity_EventScript_EiraLocked
	goto SchismIceCity_EventScript_EiraBattle

@ Pre-Gym1: command room effectively closed.
SchismIceCity_EventScript_EiraLocked::
	msgbox SchismIceCity_Text_EiraLocked, MSGBOX_DEFAULT
	release
	end

@ Gym 2 battle (Ice).
SchismIceCity_EventScript_EiraBattle::
	msgbox SchismIceCity_Text_EiraPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_SCHISM_EIRA, SchismIceCity_Text_EiraDefeat
	goto SchismIceCity_EventScript_EiraVictory

@ First victory: badge fanfare (narrative-only), Scar pass, TM, progress.
@ Give the Scar pass + flag BEFORE the TM so a full bag can't skip story.
SchismIceCity_EventScript_EiraVictory::
	setspeaker Pelagios_Speaker_Eira
	setflag FLAG_SCHISM_GYM2_CLEAR
	msgbox SchismIceCity_Text_EiraVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message SchismIceCity_Text_ReceivedCommanderBadge
	waitmessage
	waitfanfare
	closemessage
	@ Scar pass (item + flag) before the TM.
	giveitem ITEM_SCAR_PASS_ICE
	setflag FLAG_EIRA_SCAR_PASS
	setspeaker Pelagios_Speaker_Eira
	msgbox SchismIceCity_Text_EiraScarPass, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ TM Ice Beam (as briefed).
	giveitem ITEM_TM_ICE_BEAM
	goto_if_eq VAR_RESULT, FALSE, SchismIceCity_EventScript_EiraBagFull
	setspeaker Pelagios_Speaker_Eira
	msgbox SchismIceCity_Text_EiraTM, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	call SchismIceCity_EventScript_AdvanceToThree
	release
	end

SchismIceCity_EventScript_EiraBagFull::
	setspeaker 0
	msgbox SchismIceCity_Text_BagFull, MSGBOX_DEFAULT
	closemessage
	call SchismIceCity_EventScript_AdvanceToThree
	release
	end

@ Advance VAR_SCHISM_PROGRESS to 3 only if currently below 3 (nonlinear-
@ island safety: never regress if the player did the south side first).
SchismIceCity_EventScript_AdvanceToThree::
	goto_if_ge VAR_SCHISM_PROGRESS, 3, SchismIceCity_EventScript_AdvanceToThree_Done
	setvar VAR_SCHISM_PROGRESS, 3
SchismIceCity_EventScript_AdvanceToThree_Done::
	return

@ Post-battle, pre-ruins: she sent you to the Scar.
SchismIceCity_EventScript_EiraPostBattle::
	goto_if_set FLAG_SCAR_RUINS_FOUND, SchismIceCity_EventScript_EiraRuinsKnown
	msgbox SchismIceCity_Text_EiraPostBattle, MSGBOX_DEFAULT
	release
	end

@ Ruins found - she agrees to meet at the Scar (sets her willing flag).
SchismIceCity_EventScript_EiraRuinsKnown::
	msgbox SchismIceCity_Text_EiraRuins, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismIceCity_EventScript_EiraRuinsDeclined
	msgbox SchismIceCity_Text_EiraAgrees, MSGBOX_DEFAULT
	setflag FLAG_EIRA_CEASEFIRE_WILLING
	call SchismCommon_EventScript_CheckBothWilling
	release
	end

SchismIceCity_EventScript_EiraRuinsDeclined::
	msgbox SchismIceCity_Text_EiraRuinsDeclined, MSGBOX_DEFAULT
	release
	end

@ Willing, ceasefire not yet held (waiting for Drenn / the meeting).
SchismIceCity_EventScript_EiraWaiting::
	msgbox SchismIceCity_Text_EiraWaiting, MSGBOX_DEFAULT
	release
	end

@ Post-ceasefire.
SchismIceCity_EventScript_EiraCeasefire::
	msgbox SchismIceCity_Text_EiraResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Soldier who remembers the unified island.
@ -------------------------------------------------------------------
SchismIceCity_EventScript_SoldierRemembers::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Soldier
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismIceCity_EventScript_SoldierRemembersCeasefire
	msgbox SchismIceCity_Text_SoldierRemembers, MSGBOX_DEFAULT
	release
	end

SchismIceCity_EventScript_SoldierRemembersCeasefire::
	msgbox SchismIceCity_Text_SoldierRemembersCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Child uneasy about saluting the Covenant.
@ -------------------------------------------------------------------
SchismIceCity_EventScript_SalutingChild::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	msgbox SchismIceCity_Text_SalutingChild, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Generic ice faction soldier.
@ -------------------------------------------------------------------
SchismIceCity_EventScript_Soldier2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Soldier
	goto_if_set FLAG_SCHISM_GYM2_CLEAR, SchismIceCity_EventScript_Soldier2Beaten
	msgbox SchismIceCity_Text_Soldier2, MSGBOX_DEFAULT
	release
	end

SchismIceCity_EventScript_Soldier2Beaten::
	msgbox SchismIceCity_Text_Soldier2Beaten, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SchismIceCity_EventScript_BarracksSign::
	msgbox SchismIceCity_Text_BarracksSign, MSGBOX_SIGN
	end

SchismIceCity_EventScript_CenterSign::
	msgbox SchismIceCity_Text_CenterSign, MSGBOX_SIGN
	end

SchismIceCity_EventScript_CommandSign::
	msgbox SchismIceCity_Text_CommandSign, MSGBOX_SIGN
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismIceCity_Text_EiraLocked:
	.string "I am FROST COMMANDER EIRA, and I\n"
	.string "do not see outsiders on a whim.\p"
	.string "Earn an audience. SLEET runs the\n"
	.string "barracks. Beat him and I'll know\l"
	.string "you're worth two minutes of my\l"
	.string "time.$"

SchismIceCity_Text_EiraPre:
	.string "The WARDEN's child.\p"
	.string "I've read the intelligence\n"
	.string "reports. Your parent visited\l"
	.string "this island seven years ago.\p"
	.string "They sat with me for two hours\n"
	.string "and said almost nothing. Then\l"
	.string "they left.\p"
	.string "I've been trying to understand\n"
	.string "that visit ever since.\p"
	.string "Defeat me and perhaps you'll\n"
	.string "explain it.$"

SchismIceCity_Text_EiraDefeat:
	.string "…Cold and precise. Good.$"

SchismIceCity_Text_EiraVictory:
	.string "…You fight like someone who knows\n"
	.string "what they're protecting.\p"
	.string "That's rarer than you'd think.$"

SchismIceCity_Text_ReceivedCommanderBadge:
	.string "{PLAYER} received the COMMANDER\n"
	.string "BADGE.$"

SchismIceCity_Text_EiraScarPass:
	.string "The Scar pass. Take it.\p"
	.string "Find out what's in the middle.\p"
	.string "Then come back and tell me what\n"
	.string "my parent never did.$"

SchismIceCity_Text_EiraTM:
	.string "And this. ICE BEAM.\p"
	.string "Precision over force. It will\n"
	.string "serve you better than BLIZZARD\l"
	.string "ever did me.$"

SchismIceCity_Text_BagFull:
	.string "Your pack is full.\p"
	.string "Make room and ask SLEET to pass\n"
	.string "it along. He won't mind.$"

SchismIceCity_Text_EiraPostBattle:
	.string "Still here? The Scar is east of\n"
	.string "the FROZEN TUNDRA.\p"
	.string "Go. Look at what twenty years of\n"
	.string "this did to the middle of our\l"
	.string "island.\p"
	.string "Then we'll talk.$"

SchismIceCity_Text_EiraRuins:
	.string "…You've been to the ruins.\p"
	.string "I can see it on your face.\p"
	.string "You're going to tell me the\n"
	.string "island was one place once.\p"
	.string "I already know. I've always\n"
	.string "known.\p"
	.string "Are you asking me to stand in\n"
	.string "the Scar and admit it where\l"
	.string "DRENN can hear?$"

SchismIceCity_Text_EiraAgrees:
	.string "…\p"
	.string "Bring him to the Scar. If he\n"
	.string "comes, I'll come.\p"
	.string "I am not doing this for him. Or\n"
	.string "for you.\p"
	.string "I'm doing it because I'm tired of\n"
	.string "burying my people for a war I\l"
	.string "no longer believe in.$"

SchismIceCity_Text_EiraRuinsDeclined:
	.string "Then we have nothing further to\n"
	.string "discuss today.\p"
	.string "The offer stands when you're\n"
	.string "ready to make it.$"

SchismIceCity_Text_EiraWaiting:
	.string "I said I would come to the Scar.\n"
	.string "I keep my word.\p"
	.string "Get DRENN to the ruins. The rest\n"
	.string "is up to two stubborn people and\l"
	.string "a pile of old stones.$"

SchismIceCity_Text_EiraResolved:
	.string "Strange, isn't it.\p"
	.string "Twenty years certain the poison\n"
	.string "side was the enemy.\p"
	.string "Turns out the enemy was a man\n"
	.string "named DORNE, handing both of us\l"
	.string "the same knife and pointing.\p"
	.string "When you find him - and you will\n"
	.string "- the ice faction stands with\l"
	.string "you.$"

SchismIceCity_Text_SoldierRemembers:
	.string "My grandmother told me about the\n"
	.string "old name.\p"
	.string "I don't repeat it. It makes the\n"
	.string "commanders uncomfortable.\p"
	.string "She said the island used to smell\n"
	.string "like pine and salt at the same\l"
	.string "time.\p"
	.string "I've never smelled that.$"

SchismIceCity_Text_SoldierRemembersCeasefire:
	.string "SLEET crossed the Scar this\n"
	.string "morning.\p"
	.string "Nobody stopped him.\p"
	.string "I don't know what that means\n"
	.string "yet. I think it means something.$"

SchismIceCity_Text_SalutingChild:
	.string "My dad says don't salute the\n"
	.string "Covenant ships when they pass.\p"
	.string "EIRA says we don't answer to the\n"
	.string "Covenant.\p"
	.string "I salute anyway. It seems safer.$"

SchismIceCity_Text_Soldier2:
	.string "Everything here is built to\n"
	.string "survive permanent winter.\p"
	.string "Thick walls. Banked fires.\n"
	.string "Patience.\p"
	.string "We've had a lot of practice at\n"
	.string "patience.$"

SchismIceCity_Text_Soldier2Beaten:
	.string "You beat the Commander.\p"
	.string "Half of us are relieved and\n"
	.string "won't admit it.$"

SchismIceCity_Text_BarracksSign:
	.string "ICE FACTION BARRACKS\p"
	.string "Recruits and challengers report\n"
	.string "to SLEET within.$"

SchismIceCity_Text_CenterSign:
	.string "ICE CITY POKéMON CENTER\p"
	.string "Warmth and healing. Faction does\n"
	.string "not charge the wounded.$"

SchismIceCity_Text_CommandSign:
	.string "FROST COMMAND\p"
	.string "FROST COMMANDER EIRA presiding.\n"
	.string "Audiences by merit only.$"
