Schism_IceCity_Barracks_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Quartermaster - runs the barracks foyer. Points to Sleet, foreshadows
@ Eira's impatience.
@ -------------------------------------------------------------------
SchismIceCityBarracks_EventScript_Quartermaster::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Soldier
	goto_if_set FLAG_SCHISM_GYM1_CLEAR, SchismIceCityBarracks_EventScript_QuartermasterBeaten
	msgbox SchismIceCityBarracks_Text_Quartermaster, MSGBOX_DEFAULT
	release
	end

SchismIceCityBarracks_EventScript_QuartermasterBeaten::
	msgbox SchismIceCityBarracks_Text_QuartermasterBeaten, MSGBOX_DEFAULT
	release
	end

SchismIceCityBarracks_Text_Quartermaster:
	.string "Challenger? SLEET's on the\n"
	.string "training floor, in the back.\p"
	.string "He's young. Don't let that fool\n"
	.string "you. He grew up on that ice.$"

SchismIceCityBarracks_Text_QuartermasterBeaten:
	.string "Beat SLEET, did you. He'll be\n"
	.string "insufferable about it for weeks.\p"
	.string "“They were worthy,” he'll say.\n"
	.string "Over and over.$"
