Schism_IceCave_MapScripts::
	.byte 0

@ ===================================================================
@ ICE CAVE - links IceCity to SealChamber_North. Guard Rael (sight
@ trainer) + cross-island ancient inscription.
@ ===================================================================

@ -------------------------------------------------------------------
@ Guard Rael - sight trainer. trainerbattle MUST be the first command.
@ -------------------------------------------------------------------
SchismIceCave_EventScript_GuardRael::
	trainerbattle_single TRAINER_ICEFACTION_SCHISM_3, SchismIceCave_Text_RaelIntro, SchismIceCave_Text_RaelDefeat, SchismIceCave_EventScript_RaelPost

SchismIceCave_EventScript_RaelPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Rael
	goto_if_set FLAG_EIRA_SCAR_PASS, SchismIceCave_EventScript_RaelPass
	msgbox SchismIceCave_Text_RaelPostNoPass, MSGBOX_DEFAULT
	release
	end

SchismIceCave_EventScript_RaelPass::
	msgbox SchismIceCave_Text_RaelPost, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Ancient inscription - cross-island lore (no cipher flag; flavor that
@ echoes the other islands' "one lock" refrain).
@ -------------------------------------------------------------------
SchismIceCave_EventScript_Inscription::
	lockall
	msgbox SchismIceCave_Text_Inscription, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
SchismIceCave_Text_RaelIntro:
	.string "Cave's restricted. Commander's\n"
	.string "orders.$"

SchismIceCave_Text_RaelDefeat:
	.string "…All right. All right.$"

SchismIceCave_Text_RaelPost:
	.string "You have the pass. Go ahead.\p"
	.string "It's cold down there. Colder than\n"
	.string "up here, and that's saying\l"
	.string "something.\p"
	.string "Whatever's at the bottom - I\n"
	.string "don't look at it too long.$"

SchismIceCave_Text_RaelPostNoPass:
	.string "You beat me, but the deep cave\n"
	.string "still isn't yours to enter.\p"
	.string "Earn EIRA's mark first. Then\n"
	.string "nobody here will stop you.$"

SchismIceCave_Text_Inscription:
	.string "Old letters, frost-rimed, cut\n"
	.string "into the cave wall.\p"
	.string "The same hand that marked the\n"
	.string "ruins on HAVEN ISLE. On IRONHOLD.\p"
	.string "“…while one lock holds, the\n"
	.string "other must not fail. The two are\l"
	.string "one chain…”\p"
	.string "The rest is lost to the ice.$"
