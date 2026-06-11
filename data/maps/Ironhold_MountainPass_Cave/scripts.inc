Ironhold_MountainPass_Cave_MapScripts::
	.byte 0

@ Scout Hale - TRAINER_COVENANT_IRONHOLD_2 (sight-initiated)
IronholdMountainPassCave_EventScript_ScoutHale::
	trainerbattle_single TRAINER_COVENANT_IRONHOLD_2, IronholdMountainPassCave_Text_HaleIntro, IronholdMountainPassCave_Text_HaleDefeat
	msgbox IronholdMountainPassCave_Text_HalePost, MSGBOX_AUTOCLOSE
	end

@ Ancient inscription - the same carved script as the Haven Isle
@ ruins. First cross-island lore connection. (Haven's inscription is
@ where cipher 1 was recorded; this wall is flavor only - Ironhold's
@ cipher unlocks in the SealChamber at resolution.)
IronholdMountainPassCave_EventScript_Inscription::
	msgbox IronholdMountainPassCave_Text_Inscription, MSGBOX_SIGN
	end

IronholdMountainPassCave_Text_HaleIntro:
	.string "The cave's restricted.\p"
	.string "I don't make the rules.$"

IronholdMountainPassCave_Text_HaleDefeat:
	.string "Fine.$"

IronholdMountainPassCave_Text_HalePost:
	.string "Fine. Go.\p"
	.string "I never saw you.$"

IronholdMountainPassCave_Text_Inscription:
	.string "Carved script covers the cave\n"
	.string "wall - line after line of it.\p"
	.string "You have seen this alphabet\n"
	.string "before. In the ruins beneath\l"
	.string "HAVEN ISLE.\p"
	.string "The same spiral. The same\n"
	.string "patient, careful hand - carved an\l"
	.string "ocean apart.\p"
	.string "One sequence repeats over and\n"
	.string "over, like a warning, or a\l"
	.string "lullaby:\p"
	.string "“WHILE ONE LOCK HOLDS.”$"
