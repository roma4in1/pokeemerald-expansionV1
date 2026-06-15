Schism_PoisonCity_Laboratory_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ POISON LAB INTERIOR - Gym 3 (Murk, lower, 10,21) and Gym 4 (Drenn,
@ upper office, 12,3). Researcher Siv (sight trainer, 13,17). Both
@ leaders talk-initiated (TRAINER_TYPE_NONE).
@ ===================================================================

@ -------------------------------------------------------------------
@ Murk - Gym 3 (Poison/Ghost). The one who found Dorne's documents
@ suspicious. Sends the player upstairs to Drenn.
@ -------------------------------------------------------------------
SchismPoisonCityLaboratoryInterior_EventScript_Murk::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Murk
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismPoisonCityLaboratoryInterior_EventScript_MurkCeasefire
	goto_if_set FLAG_SCHISM_GYM3_CLEAR, SchismPoisonCityLaboratoryInterior_EventScript_MurkBeaten
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_SCHISM_MURK, SchismPoisonCityLaboratoryInterior_Text_MurkDefeat
	goto SchismPoisonCityLaboratoryInterior_EventScript_MurkVictory

SchismPoisonCityLaboratoryInterior_EventScript_MurkVictory::
	setspeaker Pelagios_Speaker_Murk
	setflag FLAG_SCHISM_GYM3_CLEAR
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message SchismPoisonCityLaboratoryInterior_Text_ReceivedVenomBadge
	waitmessage
	waitfanfare
	closemessage
	giveitem ITEM_TM_SLUDGE_BOMB
	goto_if_eq VAR_RESULT, FALSE, SchismPoisonCityLaboratoryInterior_EventScript_MurkBagFull
	setspeaker Pelagios_Speaker_Murk
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkTM, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	goto SchismPoisonCityLaboratoryInterior_EventScript_MurkAdvance

SchismPoisonCityLaboratoryInterior_EventScript_MurkBagFull::
	setspeaker Pelagios_Speaker_Murk
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkBagFull, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	goto SchismPoisonCityLaboratoryInterior_EventScript_MurkAdvance

@ progress -> 4 (Murk beaten), only if behind (south side may be ahead
@ of the north side; never regress).
SchismPoisonCityLaboratoryInterior_EventScript_MurkAdvance::
	goto_if_ge VAR_SCHISM_PROGRESS, 4, SchismPoisonCityLaboratoryInterior_EventScript_MurkAdvanceDone
	setvar VAR_SCHISM_PROGRESS, 4
SchismPoisonCityLaboratoryInterior_EventScript_MurkAdvanceDone::
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_MurkBeaten::
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkBeaten, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_MurkCeasefire::
	msgbox SchismPoisonCityLaboratoryInterior_Text_MurkCeasefire, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Researcher Siv - sight trainer.
@ -------------------------------------------------------------------
SchismPoisonCityLaboratoryInterior_EventScript_ResearcherSiv::
	trainerbattle_single TRAINER_POISONFACTION_SCHISM_3, SchismPoisonCityLaboratoryInterior_Text_SivIntro, SchismPoisonCityLaboratoryInterior_Text_SivDefeat, SchismPoisonCityLaboratoryInterior_EventScript_SivPost

SchismPoisonCityLaboratoryInterior_EventScript_SivPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Siv
	msgbox SchismPoisonCityLaboratoryInterior_Text_SivPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Venom Lord Cassius Drenn - Gym 4 (Poison/Dragon). States:
@   - pre-Gym3: locked (points to Murk)
@   - post-Gym3, pre-defeat: Gym 4 battle
@   - post-defeat (first time): the A/B/C ceasefire choice
@   - after ruins found + engaged: agrees to meet (willing)
@ -------------------------------------------------------------------
SchismPoisonCityLaboratoryInterior_EventScript_Drenn::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Drenn
	goto_if_set FLAG_SCHISM_CEASEFIRE, SchismPoisonCityLaboratoryInterior_EventScript_DrennCeasefire
	goto_if_set FLAG_DRENN_CEASEFIRE_WILLING, SchismPoisonCityLaboratoryInterior_EventScript_DrennWaiting
	goto_if_set FLAG_SCHISM_GYM4_CLEAR, SchismPoisonCityLaboratoryInterior_EventScript_DrennPostBattle
	goto_if_unset FLAG_SCHISM_GYM3_CLEAR, SchismPoisonCityLaboratoryInterior_EventScript_DrennLocked
	goto SchismPoisonCityLaboratoryInterior_EventScript_DrennBattle

SchismPoisonCityLaboratoryInterior_EventScript_DrennLocked::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennLocked, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennBattle::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_SCHISM_DRENN, SchismPoisonCityLaboratoryInterior_Text_DrennDefeat
	goto SchismPoisonCityLaboratoryInterior_EventScript_DrennVictory

@ First victory: badge, Scar pass, TM, progress, THEN the A/B/C choice.
SchismPoisonCityLaboratoryInterior_EventScript_DrennVictory::
	setspeaker Pelagios_Speaker_Drenn
	setflag FLAG_SCHISM_GYM4_CLEAR
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message SchismPoisonCityLaboratoryInterior_Text_ReceivedDrennBadge
	waitmessage
	waitfanfare
	closemessage
	@ South Scar pass (item + flag) before the TM.
	giveitem ITEM_SCAR_PASS_POISON
	setflag FLAG_DRENN_SCAR_PASS
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennScarPass, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ TM Dragon Claw (brief asks Dragon Pulse; no Dragon Pulse TM exists -
	@ Dragon Claw is the real Dragon-type TM; lampshaded in dialogue).
	giveitem ITEM_TM_DRAGON_CLAW
	goto_if_eq VAR_RESULT, FALSE, SchismPoisonCityLaboratoryInterior_EventScript_DrennTMSkip
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennTM, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
SchismPoisonCityLaboratoryInterior_EventScript_DrennTMSkip::
	@ progress -> 5 (Drenn beaten + south Scar pass), only if behind.
	goto_if_ge VAR_SCHISM_PROGRESS, 5, SchismPoisonCityLaboratoryInterior_EventScript_DrennChoice
	setvar VAR_SCHISM_PROGRESS, 5
	goto SchismPoisonCityLaboratoryInterior_EventScript_DrennChoice

@ The player choice. Drenn: "Say what you came to say." Presented as
@ two sequential YESNO prompts (no custom multichoice list - that is
@ systems-engineer domain): offer line A, then line B; declining both
@ is option C (say nothing / leave).
SchismPoisonCityLaboratoryInterior_EventScript_DrennChoice::
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennSpeak, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SchismPoisonCityLaboratoryInterior_Text_ChoicePromptA, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceA
	msgbox SchismPoisonCityLaboratoryInterior_Text_ChoicePromptB, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceB
	goto SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceC

@ A: "Dorne manipulated both sides." -> skeptical but listening (>=1).
SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceA::
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennChoiceA, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	call SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMin
	release
	end

@ B: "The Scar has proof. Come see it." Only meaningful if the ruins
@ are already found - then it counts as agreeing to meet (willing).
SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceB::
	setspeaker Pelagios_Speaker_Drenn
	goto_if_unset FLAG_SCAR_RUINS_FOUND, SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceBNoRuins
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennChoiceBRuins, MSGBOX_DEFAULT
	setflag FLAG_DRENN_CEASEFIRE_WILLING
	closemessage
	setspeaker 0
	@ Engaged at least; CheckBothWilling arms the meeting (var 2) only
	@ once EIRA is also willing - do not set var 2 directly here.
	call SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMin
	call SchismCommon_EventScript_CheckBothWilling
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceBNoRuins::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennChoiceBNoRuins, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	call SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMin
	release
	end

@ C: say nothing / leave -> remains convinced by Dorne. (Harder, not
@ impossible - the recurring talk can still re-engage him.)
SchismPoisonCityLaboratoryInterior_EventScript_DrennChoiceC::
	setspeaker Pelagios_Speaker_Drenn
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennChoiceC, MSGBOX_DEFAULT
	release
	end

@ Set VAR_SCHISM_CEASEFIRE_PROGRESS to at least 1 (skeptical/engaged),
@ never lowering it (it may already be 2 if both leaders are willing).
SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMin::
	goto_if_ge VAR_SCHISM_CEASEFIRE_PROGRESS, 1, SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMinDone
	setvar VAR_SCHISM_CEASEFIRE_PROGRESS, 1
SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMinDone::
	return

@ Post-battle recurring. If the ruins are found, offer to meet (this is
@ Drenn's ceasefire-willing trigger). Re-engages a C-path player.
SchismPoisonCityLaboratoryInterior_EventScript_DrennPostBattle::
	goto_if_set FLAG_SCAR_RUINS_FOUND, SchismPoisonCityLaboratoryInterior_EventScript_DrennRuinsKnown
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennPostBattle, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennRuinsKnown::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennRuins, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SchismPoisonCityLaboratoryInterior_EventScript_DrennRuinsDeclined
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennAgrees, MSGBOX_DEFAULT
	setflag FLAG_DRENN_CEASEFIRE_WILLING
	call SchismPoisonCityLaboratoryInterior_EventScript_SetEngagedMin
	call SchismCommon_EventScript_CheckBothWilling
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennRuinsDeclined::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennRuinsDeclined, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennWaiting::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennWaiting, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratoryInterior_EventScript_DrennCeasefire::
	msgbox SchismPoisonCityLaboratoryInterior_Text_DrennResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Vat sign.
@ -------------------------------------------------------------------
SchismPoisonCityLaboratoryInterior_EventScript_VatSign::
	msgbox SchismPoisonCityLaboratoryInterior_Text_VatSign, MSGBOX_SIGN
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismPoisonCityLaboratoryInterior_Text_MurkPre:
	.string "Outsider. Interesting.\p"
	.string "The last outsider who came\n"
	.string "through here worked for someone\l"
	.string "called DORNE. Left some\l"
	.string "documents.\p"
	.string "DRENN found them very convincing.\n"
	.string "I found them suspicious.\p"
	.string "Beat me and you can tell DRENN\n"
	.string "why.$"

SchismPoisonCityLaboratoryInterior_Text_MurkDefeat:
	.string "Hah. Capable, at least.$"

SchismPoisonCityLaboratoryInterior_Text_MurkVictory:
	.string "DRENN is upstairs. He's been\n"
	.string "studying those documents for\l"
	.string "three weeks.\p"
	.string "Whatever DORNE told him - be\n"
	.string "ready to argue against it.\p"
	.string "DRENN is brilliant and wrong in\n"
	.string "equal measure.$"

SchismPoisonCityLaboratoryInterior_Text_ReceivedVenomBadge:
	.string "{PLAYER} received the VENOM\n"
	.string "BADGE.$"

SchismPoisonCityLaboratoryInterior_Text_MurkTM:
	.string "SLUDGE BOMB. Take it.\p"
	.string "Reliable. Unsubtle. Occasionally\n"
	.string "leaves the other fellow too sick\l"
	.string "to argue.\p"
	.string "I find that restful.$"

SchismPoisonCityLaboratoryInterior_Text_MurkBagFull:
	.string "Your pack is full.\p"
	.string "The TM will keep. Come back for\n"
	.string "it. I'm always here, regrettably.$"

SchismPoisonCityLaboratoryInterior_Text_MurkBeaten:
	.string "Go on up. Argue well.\p"
	.string "I've spent three weeks unable to\n"
	.string "make him listen. Maybe an\l"
	.string "outsider will land where I\l"
	.string "didn't.$"

SchismPoisonCityLaboratoryInterior_Text_MurkCeasefire:
	.string "So the man was right to be\n"
	.string "suspicious. Me, I mean. I was\l"
	.string "right.\p"
	.string "I don't gloat. Much.\p"
	.string "EIRA's people sent word they want\n"
	.string "a contact on our side. They\l"
	.string "asked for me.\p"
	.string "First ice-side letter I've ever\n"
	.string "wanted to answer.$"

SchismPoisonCityLaboratoryInterior_Text_SivIntro:
	.string "I'm running experiments. You're\n"
	.string "interrupting.\p"
	.string "Defeat me and I'll at least be\n"
	.string "interrupted by someone capable.$"

SchismPoisonCityLaboratoryInterior_Text_SivDefeat:
	.string "Noted.$"

SchismPoisonCityLaboratoryInterior_Text_SivPost:
	.string "Noted. DRENN's upstairs. Good\n"
	.string "luck.\p"
	.string "You'll need it. He doesn't lose\n"
	.string "arguments. Ask anyone he's\l"
	.string "buried.$"

SchismPoisonCityLaboratoryInterior_Text_DrennLocked:
	.string "Up here. Yes, you can see me.\n"
	.string "No, you can't reach me. Not yet.\p"
	.string "MURK guards the floor below.\n"
	.string "Get past him and we'll have our\l"
	.string "conversation.$"

SchismPoisonCityLaboratoryInterior_Text_DrennPre:
	.string "DORNE was here. Did you know\n"
	.string "that?\p"
	.string "Left me evidence that the ice\n"
	.string "faction has been deliberately\l"
	.string "destabilizing our seal for\l"
	.string "years. Proof, he called it.\p"
	.string "I believe him. I've seen what\n"
	.string "they've done to our waters. I've\l"
	.string "watched our people sicken.\p"
	.string "This DRAGALGE has been at my side\n"
	.string "since I was a boy. It is sick\l"
	.string "now too.\p"
	.string "You're going to tell me DORNE was\n"
	.string "lying. I'd like to see you try.$"

SchismPoisonCityLaboratoryInterior_Text_DrennDefeat:
	.string "…You fight like you mean it.$"

SchismPoisonCityLaboratoryInterior_Text_DrennVictory:
	.string "…You're strong. Fine.$"

SchismPoisonCityLaboratoryInterior_Text_ReceivedDrennBadge:
	.string "{PLAYER} received the DRENN\n"
	.string "BADGE.$"

SchismPoisonCityLaboratoryInterior_Text_DrennScarPass:
	.string "And the Scar pass, from our side.\p"
	.string "If you're going to keep telling\n"
	.string "me I'm wrong, you'll want to walk\l"
	.string "the dead ground yourself.$"

SchismPoisonCityLaboratoryInterior_Text_DrennTM:
	.string "Take this as well. DRAGON CLAW.\p"
	.string "I'd have given you DRAGON PULSE\n"
	.string "if I had it spare. I don't. The\l"
	.string "claw bites just as deep.$"

SchismPoisonCityLaboratoryInterior_Text_DrennSpeak:
	.string "Say what you came to say.$"

SchismPoisonCityLaboratoryInterior_Text_ChoicePromptA:
	.string "Tell him DORNE manipulated both\n"
	.string "sides?$"

SchismPoisonCityLaboratoryInterior_Text_ChoicePromptB:
	.string "Tell him the Scar holds the\n"
	.string "proof - and he should come see\l"
	.string "it?$"

SchismPoisonCityLaboratoryInterior_Text_DrennChoiceA:
	.string "“DORNE manipulated both sides.”\p"
	.string "…You sound like MURK.\p"
	.string "It's a convenient story. It would\n"
	.string "mean I buried my people for\l"
	.string "nothing.\p"
	.string "I don't want it to be true.\n"
	.string "That's exactly why I don't trust\l"
	.string "how badly I want it to be false.\p"
	.string "…Go on. Keep talking. I'm\n"
	.string "listening. Barely.$"

SchismPoisonCityLaboratoryInterior_Text_DrennChoiceBRuins:
	.string "“The Scar has proof. Come see\n"
	.string "it.”\p"
	.string "…You've been to the ruins.\p"
	.string "The single settlement. The\n"
	.string "architecture that's ours and\l"
	.string "hers at once.\p"
	.string "I've seen the sketches. I told\n"
	.string "myself they were forgeries.\p"
	.string "…Fine. The Scar. If she's there,\n"
	.string "I'll stand in it. Once.$"

SchismPoisonCityLaboratoryInterior_Text_DrennChoiceBNoRuins:
	.string "“The Scar has proof. Come see\n"
	.string "it.”\p"
	.string "What proof? You haven't been to\n"
	.string "the dead ground yourself.\p"
	.string "Don't sell me a thing you haven't\n"
	.string "seen. Go look first. Then come\l"
	.string "back and try that line again.$"

SchismPoisonCityLaboratoryInterior_Text_DrennChoiceC:
	.string "…Nothing to say.\p"
	.string "Then we're done. DORNE's evidence\n"
	.string "stands until something better\l"
	.string "knocks it down.\p"
	.string "Come back if you find that\n"
	.string "something. I'll be here, being\l"
	.string "wrong, as MURK keeps insisting.$"

SchismPoisonCityLaboratoryInterior_Text_DrennPostBattle:
	.string "Still no argument worth the\n"
	.string "name?\p"
	.string "Go to the Scar. Walk it. Then\n"
	.string "bring me something I can't\l"
	.string "dismiss.$"

SchismPoisonCityLaboratoryInterior_Text_DrennRuins:
	.string "…You've stood in the Scar now.\n"
	.string "I can see it on you.\p"
	.string "You're going to ask me to meet\n"
	.string "her there. EIRA. In the open.\p"
	.string "Do you have any idea what she's\n"
	.string "taken from us? What I'd be\l"
	.string "standing next to?\p"
	.string "…Ask me. Properly. Will I come?$"

SchismPoisonCityLaboratoryInterior_Text_DrennAgrees:
	.string "…\p"
	.string "Once. I'll stand in the Scar\n"
	.string "once.\p"
	.string "Not to forgive her. I won't.\p"
	.string "But I've read DORNE's documents\n"
	.string "forty times, and the only thing\l"
	.string "they prove is that he wanted\l"
	.string "exactly this war.\p"
	.string "I'm done giving him what he\n"
	.string "wanted. Bring her.$"

SchismPoisonCityLaboratoryInterior_Text_DrennRuinsDeclined:
	.string "Then I stay here, with my sick\n"
	.string "DRAGALGE and my certainties.\p"
	.string "Ask again when you mean it.$"

SchismPoisonCityLaboratoryInterior_Text_DrennWaiting:
	.string "I said I'd stand in the Scar\n"
	.string "once. I don't repeat myself, and\l"
	.string "I don't go back on my word.\p"
	.string "Get her there. I'll come.$"

SchismPoisonCityLaboratoryInterior_Text_DrennResolved:
	.string "I still don't trust her.\p"
	.string "But I've stopped poisoning the\n"
	.string "thing under the ice, and she's\l"
	.string "stopped freezing the thing under\l"
	.string "the swamp.\p"
	.string "My DRAGALGE is getting better.\n"
	.string "The water's clearing.\p"
	.string "DORNE did this to us. When you\n"
	.string "find him, remember whose hand\l"
	.string "really held the knife.$"

SchismPoisonCityLaboratoryInterior_Text_VatSign:
	.string "CAUTION: ACTIVE VATS\p"
	.string "Stay on the raised platforms.\n"
	.string "The floor is not your friend.$"
