Thalvern_SubmergedRuins_Interior2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_SubmergedRuins_Interior2_OnTransition
	.byte 0

@ ===================================================================
@ The Lens's Interior2 object is hidden by FLAG_THALVERN_GYM3_CLEAR
@ (visible once Gym 3 is done - he has slipped inside to defect). Once
@ he has handed over the access card (FLAG_THALVERN_LENS_DEFECTED), he
@ has gone on to Dex's camp, so remove him every load after that.
@ ===================================================================
Thalvern_SubmergedRuins_Interior2_OnTransition:
	goto_if_unset FLAG_THALVERN_LENS_DEFECTED, Thalvern_SubmergedRuins_Interior2_OnTransition_Done
	removeobject LOCALID_THALVERN_RUINS2_LENS
Thalvern_SubmergedRuins_Interior2_OnTransition_Done:
	end

@ ===================================================================
@ The Lens, defecting quietly. Appears after Gym 3. Gives the Covenant
@ access card (bypasses the CovenantSite/DeepApproach blockade) and
@ sets FLAG_THALVERN_LENS_DEFECTED. After this he relocates to Dex's
@ camp (DexCamp Lens object is gated on the defection flag).
@ The access card is given BEFORE any TM/extra so a full bag can never
@ block it - it has no bag-full failure branch (it is a key item; key
@ items always fit).
@ ===================================================================
ThalvernSubmergedRuinsInterior2_EventScript_LensDefect::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_THALVERN_LENS_DEFECTED, ThalvernSubmergedRuinsInterior2_EventScript_LensDone
	msgbox ThalvernSubmergedRuinsInterior2_Text_LensDefect, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	giveitem ITEM_COVENANT_ACCESS_CARD
	setspeaker Pelagios_Speaker_Lens
	msgbox ThalvernSubmergedRuinsInterior2_Text_LensCard, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_THALVERN_LENS_DEFECTED
	setspeaker 0
	@ He leaves for Dex's camp now.
	applymovement LOCALID_THALVERN_RUINS2_LENS, ThalvernSubmergedRuinsInterior2_Movement_LensLeave
	waitmovement 0
	removeobject LOCALID_THALVERN_RUINS2_LENS
	release
	end

ThalvernSubmergedRuinsInterior2_EventScript_LensDone::
	release
	end

ThalvernSubmergedRuinsInterior2_Movement_LensLeave:
	walk_down
	walk_down
	step_end

@ ===================================================================
@ Central mural - the clearest vision yet: the civilization at its
@ height, Pelagios as its central figure, the final days, the sealing.
@ This is NARRATION (setspeaker 0 throughout). Extended, paced with
@ white-flashes. Cipher 6 is NOT here - it resolves in the Throne Room
@ on both paths (per the brief). Re-examine gives a quiet revisit.
@ ===================================================================
ThalvernSubmergedRuinsInterior2_EventScript_CentralMural::
	lockall
	setspeaker 0
	goto_if_set FLAG_TEMP_5, ThalvernSubmergedRuinsInterior2_EventScript_MuralRevisit
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralApproach, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 35
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralHeight, MSGBOX_DEFAULT
	closemessage
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 40
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralFinalDays, MSGBOX_DEFAULT
	closemessage
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 45
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralSealing, MSGBOX_DEFAULT
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralEnds, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_TEMP_5
	releaseall
	end

ThalvernSubmergedRuinsInterior2_EventScript_MuralRevisit::
	msgbox ThalvernSubmergedRuinsInterior2_Text_MuralRevisit, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Side murals - flavor lore.
@ ===================================================================
ThalvernSubmergedRuinsInterior2_EventScript_SideMuralWest::
	lockall
	msgbox ThalvernSubmergedRuinsInterior2_Text_SideMuralWest, MSGBOX_SIGN
	releaseall
	end

ThalvernSubmergedRuinsInterior2_EventScript_SideMuralEast::
	lockall
	msgbox ThalvernSubmergedRuinsInterior2_Text_SideMuralEast, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Covenant blocker - guards a side passage. Once the player holds the
@ access card he steps aside (flavor only; the real gate is narrative).
@ ===================================================================
ThalvernSubmergedRuinsInterior2_EventScript_Blocker::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	checkitem ITEM_COVENANT_ACCESS_CARD
	goto_if_eq VAR_RESULT, TRUE, ThalvernSubmergedRuinsInterior2_EventScript_BlockerCard
	msgbox ThalvernSubmergedRuinsInterior2_Text_Blocker, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernSubmergedRuinsInterior2_EventScript_BlockerCard::
	msgbox ThalvernSubmergedRuinsInterior2_Text_BlockerCard, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Covenant researcher taking notes - flavor.
@ ===================================================================
ThalvernSubmergedRuinsInterior2_EventScript_Researcher::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	msgbox ThalvernSubmergedRuinsInterior2_Text_Researcher, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernSubmergedRuinsInterior2_Text_LensDefect:
	.string "Quietly. The researchers report\n"
	.string "everything.\p"
	.string "I told you I'd give you something\n"
	.string "out of sight. Here it is.$"

ThalvernSubmergedRuinsInterior2_Text_LensCard:
	.string "My access card. It opens the\n"
	.string "Covenant blockade between here and\l"
	.string "the Throne Room.\p"
	.string "The Covenant wants PELAGIOS as a\n"
	.string "weapon. I've seen the containment\l"
	.string "specs.\p"
	.string "Get to the Throne Room. Reinforce\n"
	.string "the seal before NUMA VESS finds a\l"
	.string "way to stop you.\p"
	.string "I'll be at DEX's camp. I think I've\n"
	.string "picked a side.$"

ThalvernSubmergedRuinsInterior2_Text_LensDone:
	.string "He's gone - off to DEX's camp, the\n"
	.string "way he said.$"

ThalvernSubmergedRuinsInterior2_Text_MuralApproach:
	.string "The central mural fills the wall -\n"
	.string "PELAGIOS, vast and serene, the\l"
	.string "civilization arranged around it\l"
	.string "like petals around a heart.\p"
	.string "You raise a hand toward it.$"

ThalvernSubmergedRuinsInterior2_Text_MuralHeight:
	.string "...A city of pale towers, alive.\p"
	.string "Markets, schools, harbors. Children\n"
	.string "in the streets. Boats on water that\l"
	.string "would one day become their grave.\p"
	.string "And at the center of all of it,\n"
	.string "tended like a hearth - the seal.$"

ThalvernSubmergedRuinsInterior2_Text_MuralFinalDays:
	.string "...The water, rising. Year by year.\p"
	.string "The people knew. You can feel that\n"
	.string "they knew. They did not flee.\p"
	.string "They stayed to keep the lock turned\n"
	.string "as long as hands remained to turn\l"
	.string "it.$"

ThalvernSubmergedRuinsInterior2_Text_MuralSealing:
	.string "...The last day. A circle of them\n"
	.string "around the seal, the sea at their\l"
	.string "ankles.\p"
	.string "And PELAGIOS - awake, aware,\n"
	.string "grieving with them. Letting itself\l"
	.string "be bound, so the deeper thing\l"
	.string "beneath would stay bound too.$"

ThalvernSubmergedRuinsInterior2_Text_MuralEnds:
	.string "The white light releases you.\p"
	.string "Your hand is still raised. The\n"
	.string "mural is just stone again.\p"
	.string "Everyone who touches it sees this.$"

ThalvernSubmergedRuinsInterior2_Text_MuralRevisit:
	.string "The mural waits, patient as the\n"
	.string "deep. You have already seen what\l"
	.string "it holds.$"

ThalvernSubmergedRuinsInterior2_Text_SideMuralWest:
	.string "A side mural: the city in its\n"
	.string "morning. Trade, music, a festival\l"
	.string "of lanterns on the water.$"

ThalvernSubmergedRuinsInterior2_Text_SideMuralEast:
	.string "A side mural: a network of figures\n"
	.string "across many islands, each tending a\l"
	.string "small bright lock.\p"
	.string "The first WARDENS, perhaps.$"

ThalvernSubmergedRuinsInterior2_Text_Blocker:
	.string "Side passage's closed. Director's\n"
	.string "orders. No card, no entry.$"

ThalvernSubmergedRuinsInterior2_Text_BlockerCard:
	.string "...That's the LENS's card. He\n"
	.string "cleared you?\p"
	.string "...Fine. I didn't see you. Go.$"

ThalvernSubmergedRuinsInterior2_Text_Researcher:
	.string "I've copied this mural a hundred\n"
	.string "times. My drawings never look like\l"
	.string "what I remember seeing.\p"
	.string "Maybe that's the point. Maybe it's\n"
	.string "not meant to be filed.$"
