Aetheron_KnockUpStream_MapScripts::
	.byte 0

@ ===================================================================
@ KNOCK UP STREAM ASCENT - the scripted arrival cutscene.
@ The boat menu warps the player here to (8,11); the coord triggers at
@ (8,11)/(8,10) fire at VAR_AETHERON_PROGRESS == 0. The TENNYSON is caught
@ in the massive upward current - spinning, rising. Cass's Covenant vessel
@ rises alongside, not acknowledging the player. Fades to white and one-way
@ warps forward to CloudLanding (CloudLanding arrival tile 8,13).
@ No NPCs, no warps back. Sets FLAG_AETHERON_ARRIVED + progress 1.
@ Narration only (setspeaker 0 throughout).
@ ===================================================================
AetheronKnockUpStream_EventScript_Ascent::
	lockall
	setspeaker 0
	delay 20
	msgbox AetheronKnockUpStream_Text_Caught, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ The current takes hold - rapid lurches of light.
	playse SE_M_WHIRLPOOL
	fadescreen FADE_TO_WHITE
	delay 16
	fadescreen FADE_FROM_WHITE
	delay 12
	fadescreen FADE_TO_WHITE
	delay 16
	fadescreen FADE_FROM_WHITE
	delay 12
	msgbox AetheronKnockUpStream_Text_Rising, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ Cass's vessel, alongside - no interaction.
	msgbox AetheronKnockUpStream_Text_CassVessel, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_M_WHIRLPOOL
	fadescreen FADE_TO_WHITE
	delay 16
	fadescreen FADE_FROM_WHITE
	delay 10
	fadescreen FADE_TO_WHITE
	delay 24
	msgbox AetheronKnockUpStream_Text_AboveClouds, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_AETHERON_ARRIVED
	setvar VAR_AETHERON_PROGRESS, 1
	delay 20
	warp MAP_AETHERON_CLOUD_LANDING, 8, 13
	waitstate
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronKnockUpStream_Text_Caught:
	.string "The sea ahead is not flat.\p"
	.string "It rises - a vast column of water and\n"
	.string "wind, climbing straight up into a sky\l"
	.string "that should not have a top.\p"
	.string "The TENNYSON is already inside it before\n"
	.string "you can think to turn away.$"

AetheronKnockUpStream_Text_Rising:
	.string "The deck pitches. The horizon spins\n"
	.string "and tilts and drops away below you.\p"
	.string "You are not sailing anymore.\n"
	.string "You are climbing.$"

AetheronKnockUpStream_Text_CassVessel:
	.string "Through the spray, another hull rises\n"
	.string "alongside yours - grey, sharp-edged,\l"
	.string "Covenant.\p"
	.string "A figure stands at its rail. You know\n"
	.string "the shape of her, even from here.\p"
	.string "She does not look over.$"

AetheronKnockUpStream_Text_AboveClouds:
	.string "The water falls away.\p"
	.string "The TENNYSON settles onto something\n"
	.string "pale and solid, and the world below\l"
	.string "vanishes under a floor of cloud.\p"
	.string "You have come out above the sky.$"
