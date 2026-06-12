Ironhold_IronholdCity_Armory_MapScripts::
	.byte 0

@ Forge's apprentice, minding the foyer.
IronholdArmory_EventScript_Apprentice::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Apprentice
	goto_if_set FLAG_IRONHOLD_GYM2_CLEAR, IronholdArmory_EventScript_ApprenticeAfter
	goto_if_set FLAG_IRONHOLD_GYM1_CLEAR, IronholdArmory_EventScript_ApprenticeReady
	msgbox IronholdArmory_Text_ApprenticeNotYet, MSGBOX_DEFAULT
	release
	end

IronholdArmory_EventScript_ApprenticeReady::
	msgbox IronholdArmory_Text_ApprenticeReady, MSGBOX_DEFAULT
	release
	end

IronholdArmory_EventScript_ApprenticeAfter::
	msgbox IronholdArmory_Text_ApprenticeAfter, MSGBOX_DEFAULT
	release
	end

IronholdArmory_Text_ApprenticeNotYet:
	.string "The boss is at the anvil through\n"
	.string "the back. He's always at the\l"
	.string "anvil.\p"
	.string "Fair warning: he doesn't look up\n"
	.string "for anyone who hasn't gotten past\l"
	.string "PETRA at the town hall.\p"
	.string "House rule. Petra screens, FORGE\n"
	.string "forges.$"

IronholdArmory_Text_ApprenticeReady:
	.string "Word came down from the town\n"
	.string "hall already. You beat PETRA.\p"
	.string "Go on through. He'll look up\n"
	.string "this time.$"

IronholdArmory_Text_ApprenticeAfter:
	.string "He gave you the hook, didn't he.\p"
	.string "He's been setting “military\n"
	.string "salvage” aside for months.\p"
	.string "I think he was waiting to find\n"
	.string "out who it was for.$"
