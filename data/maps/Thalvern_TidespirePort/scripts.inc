Thalvern_TidespirePort_MapScripts::
	.byte 0

@ ===================================================================
@ Arrival cutscene. Fires from the pier coord triggers at (9/10,13)
@ when VAR_THALVERN_PROGRESS == 0. The player steps off the Tennyson
@ and crosses the trigger walking north. Psychic shimmer on the water,
@ a fisherman who has stopped fishing approaches with the first
@ vision hint.
@ ===================================================================
ThalvernTidespirePort_EventScript_Arrival::
	lockall
	delay 20
	@ Psychic shimmer on the water - rendered as a soft white pulse.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 25
	fadescreen FADE_FROM_WHITE
	delay 20
	msgbox ThalvernTidespirePort_Text_ArrivalShimmer, MSGBOX_DEFAULT
	closemessage
	delay 10
	@ The fisherman walks down to the player.
	applymovement LOCALID_THALVERN_TIDESPIRE_FISHERMAN, ThalvernTidespirePort_Movement_FishermanApproach
	waitmovement 0
	delay 10
	setspeaker Pelagios_Speaker_Fisherman
	msgbox ThalvernTidespirePort_Text_ArrivalFisherman, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	applymovement LOCALID_THALVERN_TIDESPIRE_FISHERMAN, ThalvernTidespirePort_Movement_FishermanReturn
	waitmovement 0
	setflag FLAG_THALVERN_ARRIVED
	setvar VAR_THALVERN_PROGRESS, 1
	releaseall
	end

ThalvernTidespirePort_Movement_FishermanApproach:
	walk_down
	walk_down
	walk_down
	walk_left
	face_down
	step_end

ThalvernTidespirePort_Movement_FishermanReturn:
	walk_right
	walk_up
	walk_up
	walk_up
	face_down
	step_end

@ ===================================================================
@ The fisherman who has stopped fishing. Recurring talk; a post-
@ resolution variant once the visions go quiet.
@ ===================================================================
ThalvernTidespirePort_EventScript_Fisherman::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Fisherman
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernTidespirePort_EventScript_FishermanResolved
	msgbox ThalvernTidespirePort_Text_Fisherman, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernTidespirePort_EventScript_FishermanResolved::
	msgbox ThalvernTidespirePort_Text_FishermanResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Porter - warns about the deep ruins.
@ ===================================================================
ThalvernTidespirePort_EventScript_Porter::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Porter
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernTidespirePort_EventScript_PorterResolved
	msgbox ThalvernTidespirePort_Text_Porter, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernTidespirePort_EventScript_PorterResolved::
	msgbox ThalvernTidespirePort_Text_PorterResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Merchant - sells psychic-resist berries and basics.
@ ===================================================================
ThalvernTidespirePort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	msgbox ThalvernTidespirePort_Text_Merchant, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	pokemart ThalvernTidespirePort_Pokemart
	msgbox ThalvernTidespirePort_Text_MerchantThanks, MSGBOX_DEFAULT
	release
	end

	.align 2
ThalvernTidespirePort_Pokemart:
	.2byte ITEM_PERSIM_BERRY
	.2byte ITEM_LUM_BERRY
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_AWAKENING
	pokemartlistend

@ ===================================================================
@ Signs
@ ===================================================================
ThalvernTidespirePort_EventScript_PortSign::
	lockall
	msgbox ThalvernTidespirePort_Text_PortSign, MSGBOX_SIGN
	releaseall
	end

ThalvernTidespirePort_EventScript_BerthSign::
	lockall
	msgbox ThalvernTidespirePort_Text_BerthSign, MSGBOX_SIGN
	releaseall
	end

@ Boat-boarding wiring (map-builder owns this; NOT dialogue).
ThalvernTidespirePort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_THALVERN
	goto Pelagios_EventScript_BoardTennyson
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernTidespirePort_Text_ArrivalShimmer:
	.string "The harbor is built on stilts above\n"
	.string "the water - the island's true ground\l"
	.string "lies fathoms below.\p"
	.string "A faint shimmer crawls across the\n"
	.string "surface of the sea. Not light.\l"
	.string "Something else.$"

ThalvernTidespirePort_Text_ArrivalFisherman:
	.string "The fish left three months ago.\p"
	.string "Same time the visions started.\p"
	.string "I see the old city sometimes, when\n"
	.string "I'm out on the water. Full of\l"
	.string "people. Going about their day.\p"
	.string "Then I blink and it's just ruins.$"

ThalvernTidespirePort_Text_Fisherman:
	.string "I stopped going out. No fish, and\n"
	.string "the images come faster on the\l"
	.string "water.\p"
	.string "Not sounds. Pictures. Quick ones.\n"
	.string "A street. A face. Gone.$"

ThalvernTidespirePort_Text_FishermanResolved:
	.string "The visions stopped this morning.\n"
	.string "Just - stopped.\p"
	.string "I almost miss them.\p"
	.string "Is that strange? Grieving for\n"
	.string "something that was making you mad?$"

ThalvernTidespirePort_Text_Porter:
	.string "Don't go into the deep ruins\n"
	.string "without a guide.\p"
	.string "People get lost in the visions.\n"
	.string "Not physically - they come back.\p"
	.string "But something in their eyes is\n"
	.string "different after.$"

ThalvernTidespirePort_Text_PorterResolved:
	.string "The deep ruins are quieter now.\n"
	.string "Folk come back the same as they\l"
	.string "left.\p"
	.string "Took someone going all the way\n"
	.string "down to do it. I heard it was you.$"

ThalvernTidespirePort_Text_Merchant:
	.string "Persim, Lum - berries that steady\n"
	.string "the mind. You'll want them in the\l"
	.string "ruins.\p"
	.string "Stock up before you go down deep.$"

ThalvernTidespirePort_Text_MerchantThanks:
	.string "Keep your head about you down\n"
	.string "there.$"

ThalvernTidespirePort_Text_PortSign:
	.string "TIDESPIRE PORT\n"
	.string "Built on stilts. Mind the deep.$"

ThalvernTidespirePort_Text_BerthSign:
	.string "The TENNYSON is moored here.\n"
	.string "Step to the hull to board.$"
