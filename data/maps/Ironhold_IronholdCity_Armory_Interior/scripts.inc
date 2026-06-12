Ironhold_IronholdCity_Armory_Interior_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 2 - Forge (Steel), at his anvil in the workshop.
@ Gated on FLAG_IRONHOLD_GYM1_CLEAR (Petra screens challengers).
@ Victory: Steel Badge (FLAG_BADGE03_GET), ITEM_GRAPPLE_HOOK +
@ FLAG_GRAPPLE_HOOK_OBTAINED, VAR_IRONHOLD_PROGRESS = 3 (disarms the
@ OuterDistrict rubble triggers), then TM Steel Wing.
@ (Brief asks for a TM of Iron Defense, which has no TM in this
@ project's vanilla TM set - Steel Wing substituted.)
@ -------------------------------------------------------------------
IronholdArmoryInterior_EventScript_Forge::
	goto_if_defeated TRAINER_LEADER_IRONHOLD_FORGE, IronholdArmoryInterior_EventScript_ForgeDefeated
	goto_if_unset FLAG_IRONHOLD_GYM1_CLEAR, IronholdArmoryInterior_EventScript_ForgeNotYet
	trainerbattle_single TRAINER_LEADER_IRONHOLD_FORGE, IronholdArmoryInterior_Text_ForgeIntro, IronholdArmoryInterior_Text_ForgeDefeat, IronholdArmoryInterior_EventScript_ForgeVictory
	end

IronholdArmoryInterior_EventScript_ForgeNotYet::
	lock
	faceplayer
	msgbox IronholdArmoryInterior_Text_ForgeNotYet, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgeNotYetSpeech, MSGBOX_DEFAULT
	release
	end

IronholdArmoryInterior_EventScript_ForgeVictory::
	msgbox IronholdArmoryInterior_Text_ForgePostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message IronholdArmoryInterior_Text_ReceivedSteelBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE03_GET
	setflag FLAG_IRONHOLD_GYM2_CLEAR
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgeSalvage, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_GRAPPLE_HOOK
	setflag FLAG_GRAPPLE_HOOK_OBTAINED
	setvar VAR_IRONHOLD_PROGRESS, 3
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgeDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_STEEL_WING
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgeExplainTM, MSGBOX_DEFAULT
	release
	end

IronholdArmoryInterior_EventScript_ForgeDefeated::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdArmoryInterior_EventScript_ForgeResolved
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgePostBadge, MSGBOX_DEFAULT
	release
	end

IronholdArmoryInterior_EventScript_ForgeResolved::
	msgbox IronholdArmoryInterior_Text_ForgeResolved, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Forge
	msgbox IronholdArmoryInterior_Text_ForgeResolvedSpeech, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdArmoryInterior_Text_ForgeNotYet:
	.string "He doesn't look up from the\n"
	.string "anvil. The hammer keeps its\l"
	.string "rhythm.$"

IronholdArmoryInterior_Text_ForgeNotYetSpeech:
	.string "PETRA screens my challengers.\n"
	.string "Town hall. North end.\p"
	.string "Come back with her badge or\n"
	.string "don't come back.$"

IronholdArmoryInterior_Text_ForgeIntro:
	.string "The hammer stops.\p"
	.string "I make weapons for the Covenant.\n"
	.string "I don't ask what they do with\l"
	.string "them.\p"
	.string "That used to feel like enough.\p"
	.string "Beat me and maybe I'll think\n"
	.string "harder.$"

IronholdArmoryInterior_Text_ForgeDefeat:
	.string "…Then I'll think harder.$"

IronholdArmoryInterior_Text_ForgePostBattle:
	.string "He sets the hammer down. It's\n"
	.string "the first time his hands have\l"
	.string "been still since you walked in.$"

IronholdArmoryInterior_Text_ReceivedSteelBadge:
	.string "{PLAYER} received the STEEL BADGE\n"
	.string "from FORGE!$"

IronholdArmoryInterior_Text_ForgeSalvage:
	.string "Take this. Military salvage -\n"
	.string "they'll never miss one.$"

IronholdArmoryInterior_Text_ForgeDirections:
	.string "The mountain pass west of the\n"
	.string "outer district. That hook will\l"
	.string "tear the rockfall right out.\p"
	.string "There's more up there than the\n"
	.string "Covenant wants people to see.$"

IronholdArmoryInterior_Text_ForgeExplainTM:
	.string "And STEEL WING. I roll the metal\n"
	.string "for those myself.\p"
	.string "Wanted a TM of IRON DEFENSE for\n"
	.string "you, but the Covenant requisitions\l"
	.string "every press run.\p"
	.string "Story of this whole island,\n"
	.string "really.$"

IronholdArmoryInterior_Text_ForgePostBadge:
	.string "The pass is west of the outer\n"
	.string "district. The hook does the rest.\p"
	.string "…I've started asking questions.\n"
	.string "Quietly. You should know the\l"
	.string "answers aren't good.$"

IronholdArmoryInterior_Text_ForgeResolved:
	.string "He is staring at his hands.$"

IronholdArmoryInterior_Text_ForgeResolvedSpeech:
	.string "I made tools for that machine.\n"
	.string "Didn't ask what it was for.\p"
	.string "I should have asked.$"
