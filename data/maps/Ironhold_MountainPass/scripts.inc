Ironhold_MountainPass_MapScripts::
	.byte 0

@ Hiker Crag - TRAINER_HIKER_IRONHOLD_1 (sight-initiated)
IronholdMountainPass_EventScript_HikerCrag::
	trainerbattle_single TRAINER_HIKER_IRONHOLD_1, IronholdMountainPass_Text_CragIntro, IronholdMountainPass_Text_CragDefeat
	setspeaker Pelagios_Speaker_Crag
	msgbox IronholdMountainPass_Text_CragPost, MSGBOX_AUTOCLOSE
	end

@ Soldier Venn - TRAINER_COVENANT_IRONHOLD_1 (sight-initiated)
IronholdMountainPass_EventScript_SoldierVenn::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_1, IronholdMountainPass_Text_VennIntro, IronholdMountainPass_Text_VennDefeat
	setspeaker Pelagios_Speaker_Venn
	msgbox IronholdMountainPass_Text_VennPost, MSGBOX_AUTOCLOSE
	end

@ Rock pile (the hidden Iron sits one tile east)
IronholdMountainPass_EventScript_RockPile::
	msgbox IronholdMountainPass_Text_RockPile, MSGBOX_SIGN
	end

IronholdMountainPass_Text_CragIntro:
	.string "The mountain doesn't care who\n"
	.string "you are.\p"
	.string "Neither do I. Let's go.$"

IronholdMountainPass_Text_CragDefeat:
	.string "…The mountain noticed that one.$"

IronholdMountainPass_Text_CragPost:
	.string "The summit's changed. Something\n"
	.string "up there feels wrong.\p"
	.string "Has for months.$"

IronholdMountainPass_Text_VennIntro:
	.string "Unauthorized personnel beyond\n"
	.string "this point.\p"
	.string "Covenant standing order - turn\n"
	.string "back or be escorted.$"

IronholdMountainPass_Text_VennDefeat:
	.string "How…$"

IronholdMountainPass_Text_VennPost:
	.string "How… Commander SEVER said no one\n"
	.string "could get past here.\p"
	.string "I need to file a report.$"

IronholdMountainPass_Text_RockPile:
	.string "A spill of ore-bearing rock,\n"
	.string "picked over by miners.\p"
	.string "Most of the good seams are gone.\n"
	.string "Most.$"
