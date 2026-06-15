Thalvern_SubmergedRuins_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_SubmergedRuins_Exterior_OnTransition
	.byte 0

@ ===================================================================
@ Once Gym 3 is cleared, the Lens has left his post at the entrance
@ (he slips inside to Interior2 to defect quietly, then on to Dex's
@ camp). His Exterior object has no hide flag in map.json, so remove
@ it every load after FLAG_THALVERN_GYM3_CLEAR.
@ ===================================================================
Thalvern_SubmergedRuins_Exterior_OnTransition:
	goto_if_unset FLAG_THALVERN_GYM3_CLEAR, Thalvern_SubmergedRuins_Exterior_OnTransition_Done
	removeobject LOCALID_THALVERN_LENS
Thalvern_SubmergedRuins_Exterior_OnTransition_Done:
	end

@ ===================================================================
@ Gym 3 - The Lens (Water/Psychic). Stationed outside to "control
@ access". Talk-initiated. Gated on Gym 2 (Psalm) being cleared.
@ Badge NARRATIVE-ONLY. Gives the Depth Badge then a TM. He does NOT
@ hand over the access card here - he tells the player to meet him
@ inside (Interior2), where the quiet defection scene gives the card
@ and sets FLAG_THALVERN_LENS_DEFECTED.
@ ===================================================================
ThalvernSubmergedRuinsExterior_EventScript_Lens::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_unset FLAG_THALVERN_GYM2_CLEAR, ThalvernSubmergedRuinsExterior_EventScript_LensLocked
	msgbox ThalvernSubmergedRuinsExterior_Text_LensPre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_THALVERN_LENS, ThalvernSubmergedRuinsExterior_Text_LensDefeat
	goto ThalvernSubmergedRuinsExterior_EventScript_LensVictory

ThalvernSubmergedRuinsExterior_EventScript_LensLocked::
	msgbox ThalvernSubmergedRuinsExterior_Text_LensLocked, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsExterior_EventScript_LensVictory::
	setspeaker Pelagios_Speaker_Lens
	setflag FLAG_THALVERN_GYM3_CLEAR
	setvar VAR_THALVERN_PROGRESS, 4
	msgbox ThalvernSubmergedRuinsExterior_Text_LensVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message ThalvernSubmergedRuinsExterior_Text_ReceivedDepthBadge
	waitmessage
	waitfanfare
	closemessage
	@ TM. Brief asks for Scald; no Scald TM exists, so give TM Rain
	@ Dance (Water-typed, suits the deep) and lampshade it.
	giveitem ITEM_TM_RAIN_DANCE
	goto_if_eq VAR_RESULT, FALSE, ThalvernSubmergedRuinsExterior_EventScript_LensBagFull
	setspeaker Pelagios_Speaker_Lens
	msgbox ThalvernSubmergedRuinsExterior_Text_LensTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsExterior_EventScript_LensBagFull::
	setspeaker Pelagios_Speaker_Lens
	msgbox ThalvernSubmergedRuinsExterior_Text_LensBagFull, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Covenant researchers in the background - flavor.
@ ===================================================================
ThalvernSubmergedRuinsExterior_EventScript_Researcher1::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	msgbox ThalvernSubmergedRuinsExterior_Text_Researcher1, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsExterior_EventScript_Researcher2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	msgbox ThalvernSubmergedRuinsExterior_Text_Researcher2, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Inscription - lore.
@ ===================================================================
ThalvernSubmergedRuinsExterior_EventScript_Inscription::
	lockall
	msgbox ThalvernSubmergedRuinsExterior_Text_Inscription, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernSubmergedRuinsExterior_Text_LensLocked:
	.string "Not yet. PSALM hasn't cleared you,\n"
	.string "and I answer to procedure - in\l"
	.string "public, anyway.\p"
	.string "Earn the Vision Badge inside. Then\n"
	.string "come back and make me look like I\l"
	.string "tried to stop you.$"

ThalvernSubmergedRuinsExterior_Text_LensPre:
	.string "I'm stationed here to control\n"
	.string "access. Covenant orders.\p"
	.string "I've been controlling access for\n"
	.string "three weeks. In that time I've read\l"
	.string "every inscription DEX decoded.\p"
	.string "I want you to reach the Throne Room.\p"
	.string "Beat me so I can say I tried to\n"
	.string "stop you.$"

ThalvernSubmergedRuinsExterior_Text_LensDefeat:
	.string "Good. On the record: I resisted.$"

ThalvernSubmergedRuinsExterior_Text_LensVictory:
	.string "There. That's my report written for\n"
	.string "me.\p"
	.string "The Depth Badge. You've gone deeper\n"
	.string "than most who hold it.$"

ThalvernSubmergedRuinsExterior_Text_ReceivedDepthBadge:
	.string "{PLAYER} received the Depth Badge.$"

ThalvernSubmergedRuinsExterior_Text_LensTM:
	.string "And a TM. Rain Dance.\p"
	.string "I know - you wanted something that\n"
	.string "scalds. We work with the sea we're\l"
	.string "given. The rain suits this place.\p"
	.string "Meet me inside. Past the second\n"
	.string "chamber. There's something I can\l"
	.string "only give you where my superiors\l"
	.string "won't see.$"

ThalvernSubmergedRuinsExterior_Text_LensBagFull:
	.string "Your bag's full - the TM can wait.\p"
	.string "Meet me inside, past the second\n"
	.string "chamber. There's something I can\l"
	.string "only give you out of sight.$"

ThalvernSubmergedRuinsExterior_Text_Researcher1:
	.string "We're cataloguing the structures\n"
	.string "before they sink. Director's\l"
	.string "orders.\p"
	.string "Funny. We catalogue them, but we\n"
	.string "never seem to save them.$"

ThalvernSubmergedRuinsExterior_Text_Researcher2:
	.string "The water's risen a hand's width\n"
	.string "since I got here. You can watch it\l"
	.string "swallow a step a day.\p"
	.string "Whatever the seal is, it's losing.$"

ThalvernSubmergedRuinsExterior_Text_Inscription:
	.string "Carved above the entrance, half-\n"
	.string "drowned:\p"
	.string "...enter with care. The deep\n"
	.string "remembers, and it will show you\l"
	.string "what it remembers...$"
