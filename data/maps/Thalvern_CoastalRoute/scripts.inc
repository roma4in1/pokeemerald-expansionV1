Thalvern_CoastalRoute_MapScripts::
	.byte 0

@ ===================================================================
@ Scholar Wren - TRAINER_SCHOLAR_THALVERN_1 (sight-initiated).
@ Sight trainers keep trainerbattle as the first command; setspeaker
@ only before the post-battle line.
@ ===================================================================
ThalvernCoastalRoute_EventScript_ScholarWren::
	trainerbattle_single TRAINER_SCHOLAR_THALVERN_1, ThalvernCoastalRoute_Text_WrenIntro, ThalvernCoastalRoute_Text_WrenDefeat
	setspeaker Pelagios_Speaker_Wren
	msgbox ThalvernCoastalRoute_Text_WrenPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Researcher Holt - TRAINER_COVENANT_THALVERN_1 (sight-initiated).
@ ===================================================================
ThalvernCoastalRoute_EventScript_ResearcherHolt::
	trainerbattle_single TRAINER_COVENANT_THALVERN_1, ThalvernCoastalRoute_Text_HoltIntro, ThalvernCoastalRoute_Text_HoltDefeat
	setspeaker Pelagios_Speaker_Holt
	msgbox ThalvernCoastalRoute_Text_HoltPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Vision patches - purely atmospheric. The coord triggers all gate on
@ VAR_TEMP_2 == 0 (never set on this map, so always armed). Each patch
@ guards itself with its own FLAG_TEMP bit so it plays at most once per
@ map visit (temp flags clear on re-entry, by design - the brief calls
@ for "local flag tracking"; this is session-local once-per-tile).
@ A brief white flash + a single line of ancient text, then the player
@ keeps walking. setspeaker is cleared throughout - this is narration.
@ ===================================================================
ThalvernCoastalRoute_EventScript_VisionPatch1::
	lockall
	goto_if_set FLAG_TEMP_2, ThalvernCoastalRoute_EventScript_VisionPatchDone
	setflag FLAG_TEMP_2
	setspeaker 0
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 18
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernCoastalRoute_Text_Vision1, MSGBOX_DEFAULT
	releaseall
	end

ThalvernCoastalRoute_EventScript_VisionPatch2::
	lockall
	goto_if_set FLAG_TEMP_3, ThalvernCoastalRoute_EventScript_VisionPatchDone
	setflag FLAG_TEMP_3
	setspeaker 0
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 18
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernCoastalRoute_Text_Vision2, MSGBOX_DEFAULT
	releaseall
	end

ThalvernCoastalRoute_EventScript_VisionPatch3::
	lockall
	goto_if_set FLAG_TEMP_4, ThalvernCoastalRoute_EventScript_VisionPatchDone
	setflag FLAG_TEMP_4
	setspeaker 0
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 18
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernCoastalRoute_Text_Vision3, MSGBOX_DEFAULT
	releaseall
	end

ThalvernCoastalRoute_EventScript_VisionPatchDone::
	releaseall
	end

@ ===================================================================
@ Exposed inscription - lore sign.
@ ===================================================================
ThalvernCoastalRoute_EventScript_Inscription::
	lockall
	msgbox ThalvernCoastalRoute_Text_Inscription, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernCoastalRoute_Text_WrenIntro:
	.string "I came here to study the visions.\n"
	.string "Now I'm having them.\p"
	.string "Science requires participation, I\n"
	.string "suppose.$"

ThalvernCoastalRoute_Text_WrenDefeat:
	.string "Observed and recorded.$"

ThalvernCoastalRoute_Text_WrenPost:
	.string "The deeper you go, the stronger\n"
	.string "they get.\p"
	.string "Take something grounding with you.$"

ThalvernCoastalRoute_Text_HoltIntro:
	.string "Covenant Archaeological Division.\p"
	.string "This site is under research\n"
	.string "jurisdiction.$"

ThalvernCoastalRoute_Text_HoltDefeat:
	.string "...$"

ThalvernCoastalRoute_Text_HoltPost:
	.string "...Jurisdiction apparently\n"
	.string "disputed.$"

ThalvernCoastalRoute_Text_Vision1:
	.string "The fog whitens for an instant -\p"
	.string "and you see a market street,\n"
	.string "crowded, awnings bright, a child\l"
	.string "running. Then it is gone.$"

ThalvernCoastalRoute_Text_Vision2:
	.string "A flash -\p"
	.string "towers of pale stone rising where\n"
	.string "the water now stands. Whole.\l"
	.string "Lived in. Loved.$"

ThalvernCoastalRoute_Text_Vision3:
	.string "White light, and a voice without\n"
	.string "sound:\p"
	.string "Remember us. We were here. We\n"
	.string "were real.$"

ThalvernCoastalRoute_Text_Inscription:
	.string "Worn glyphs on a fallen slab.\p"
	.string "...the deep is patient. We are\n"
	.string "not. So we built the locks, and\l"
	.string "we kept them...$"
