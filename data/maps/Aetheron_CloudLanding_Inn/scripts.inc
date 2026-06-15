Aetheron_CloudLanding_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Aetheron_CloudLanding_Inn_OnTransition
	.byte 0

Aetheron_CloudLanding_Inn_OnTransition:
	setrespawn HEAL_LOCATION_AETHERON_CLOUD_LANDING
	end

@ ===================================================================
@ Innkeeper - island heal NPC (nurse-gfx, normal nurse path). The storm
@ never stops; you stop noticing.
@ ===================================================================
AetheronCloudLandingInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronCloudLandingInn_EventScript_InnkeeperResolved
	msgbox AetheronCloudLandingInn_Text_Greeting, MSGBOX_DEFAULT
	goto AetheronCloudLandingInn_EventScript_RoomOffer

AetheronCloudLandingInn_EventScript_InnkeeperResolved::
	msgbox AetheronCloudLandingInn_Text_Resolved, MSGBOX_DEFAULT
	goto AetheronCloudLandingInn_EventScript_RoomOffer

AetheronCloudLandingInn_EventScript_RoomOffer::
	msgbox AetheronCloudLandingInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, AetheronCloudLandingInn_EventScript_NoRest
	msgbox AetheronCloudLandingInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox AetheronCloudLandingInn_Text_AfterRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCloudLandingInn_EventScript_NoRest::
	msgbox AetheronCloudLandingInn_Text_NoRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronCloudLandingInn_Text_Greeting:
	.string "The storm never stops. You stop\n"
	.string "noticing after a while.\p"
	.string "It becomes like breathing. The silence\n"
	.string "would be worse now, I think.$"

AetheronCloudLandingInn_Text_Resolved:
	.string "The thunder eased off this morning.\n"
	.string "Just a touch.\p"
	.string "After forty years up here, you learn to\n"
	.string "feel a change that small. Whatever you\l"
	.string "did - thank you.$"

AetheronCloudLandingInn_Text_RoomOffer:
	.string "Would you like to rest?$"

AetheronCloudLandingInn_Text_RoomsYours:
	.string "The far room. The wind's quietest\n"
	.string "there.$"

AetheronCloudLandingInn_Text_AfterRest:
	.string "Your team looks rested. The village is\n"
	.string "up the route, past the guardians.$"

AetheronCloudLandingInn_Text_NoRest:
	.string "Whenever you need it, then.$"
