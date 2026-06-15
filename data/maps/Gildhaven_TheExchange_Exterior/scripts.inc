Gildhaven_TheExchange_Exterior_MapScripts::
	.byte 0

@ ===================================================================
@ Exchange Guard Rael - TRAINER_GUARD_GILDHAVEN_1 (sight-initiated).
@ Sight trainers keep trainerbattle as the FIRST command (no namebox
@ on the intro); the post-battle line gets the GUARD namebox.
@ ===================================================================
GildhavenTheExchangeExterior_EventScript_GuardRael::
	trainerbattle_single TRAINER_GUARD_GILDHAVEN_1, GildhavenTheExchangeExterior_Text_RaelIntro, GildhavenTheExchangeExterior_Text_RaelDefeat
	setspeaker Pelagios_Speaker_Guard
	msgbox GildhavenTheExchangeExterior_Text_RaelPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Exchange Guard Sera - TRAINER_GUARD_GILDHAVEN_2 (sight-initiated).
@ ===================================================================
GildhavenTheExchangeExterior_EventScript_GuardSera::
	trainerbattle_single TRAINER_GUARD_GILDHAVEN_2, GildhavenTheExchangeExterior_Text_SeraIntro, GildhavenTheExchangeExterior_Text_SeraDefeat
	setspeaker Pelagios_Speaker_Guard
	msgbox GildhavenTheExchangeExterior_Text_SeraPost, MSGBOX_AUTOCLOSE
	end

@ ===================================================================
@ Exchange sign.
@ ===================================================================
GildhavenTheExchangeExterior_EventScript_ExchangeSign::
	lockall
	msgbox GildhavenTheExchangeExterior_Text_ExchangeSign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
GildhavenTheExchangeExterior_Text_RaelIntro:
	.string "The EXCHANGE is private property.\n"
	.string "Appointment required.$"

GildhavenTheExchangeExterior_Text_RaelDefeat:
	.string "Hgh - that is not in the procedure\n"
	.string "manual.$"

GildhavenTheExchangeExterior_Text_RaelPost:
	.string "...Your appointment has been approved.$"

GildhavenTheExchangeExterior_Text_SeraIntro:
	.string "Mr. VANE does not receive unannounced\n"
	.string "visitors.$"

GildhavenTheExchangeExterior_Text_SeraDefeat:
	.string "...Reconsidering the unannounced part.$"

GildhavenTheExchangeExterior_Text_SeraPost:
	.string "He will receive you.$"

GildhavenTheExchangeExterior_Text_ExchangeSign:
	.string "THE EXCHANGE\n"
	.string "Financial Heart of Gildhaven.\p"
	.string "The building is taller than it has\n"
	.string "any reason to be. It feels like it is\l"
	.string "looking down at you.$"
