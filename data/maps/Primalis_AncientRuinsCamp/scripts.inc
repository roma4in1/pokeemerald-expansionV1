Primalis_AncientRuinsCamp_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Primalis_AncientRuinsCamp_OnTransition
	.byte 0

@ ===================================================================
@ The camp Lens object's map.json hide flag is FLAG_PRIMALIS_LENS_MET,
@ but she should APPEAR here AFTER she is met at the inn (the engine
@ hides an object while its hide flag is SET - wrong direction). Drive
@ visibility explicitly: show her once LENS_MET is set, hide her
@ before. (Gildhaven BlackMarket-Dagan pattern.)
@ ===================================================================
Primalis_AncientRuinsCamp_OnTransition:
	goto_if_unset FLAG_PRIMALIS_LENS_MET, Primalis_AncientRuinsCamp_HideLens
	addobject LOCALID_PRIMALIS_LENS_CAMP
	end

Primalis_AncientRuinsCamp_HideLens:
	removeobject LOCALID_PRIMALIS_LENS_CAMP
	end

@ ===================================================================
@ The Lens, at the dig camp by the ruins. She has followed the trail
@ up from the inn. Warm, sharp, ALIVE-Dex enthusiasm carried secondhand
@ (never foreshadows his death).
@ ===================================================================
PrimalisAncientRuinsCamp_EventScript_LensCamp::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisAncientRuinsCamp_EventScript_LensResolved
	goto_if_set FLAG_PRIMALIS_RUINS_FOUND, PrimalisAncientRuinsCamp_EventScript_LensPostAltar
	msgbox PrimalisAncientRuinsCamp_Text_Lens, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisAncientRuinsCamp_EventScript_LensPostAltar::
	msgbox PrimalisAncientRuinsCamp_Text_LensPostAltar, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisAncientRuinsCamp_EventScript_LensResolved::
	msgbox PrimalisAncientRuinsCamp_Text_LensResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ The altar - "We did not make this." The Zoan's own statement of
@ purpose. Sets FLAG_PRIMALIS_RUINS_FOUND (which gates the Heartwood
@ tableau objects). This is NOT the Warden's cipher (cipher 8 lives at
@ the Heartwood SealChamber inscription, per the island pattern).
@ ===================================================================
PrimalisAncientRuinsCamp_EventScript_Altar::
	lockall
	goto_if_set FLAG_PRIMALIS_RUINS_FOUND, PrimalisAncientRuinsCamp_EventScript_AltarAgain
	msgbox PrimalisAncientRuinsCamp_Text_Altar1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox PrimalisAncientRuinsCamp_Text_Altar2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PRIMALIS_RUINS_FOUND
	delay 20
	msgbox PrimalisAncientRuinsCamp_Text_AltarClose, MSGBOX_DEFAULT
	releaseall
	end

PrimalisAncientRuinsCamp_EventScript_AltarAgain::
	msgbox PrimalisAncientRuinsCamp_Text_AltarAgain, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - the Lens at the camp
@ ===================================================================
PrimalisAncientRuinsCamp_Text_Lens:
	.string "You came up the trail. Good - this is\n"
	.string "where it gets interesting.\p"
	.string "This camp sits on the oldest worked\n"
	.string "stone I have ever catalogued. And the\l"
	.string "Zoan say they did not carve it.\p"
	.string "They found it. They chose to stay and\n"
	.string "guard it. A thousand years of\l"
	.string "volunteers. Read the altar. It is the\l"
	.string "whole philosophy in four lines.$"

PrimalisAncientRuinsCamp_Text_LensPostAltar:
	.string "You read it. Good.\p"
	.string "We did not make this. We chose to guard\n"
	.string "it anyway. There is a whole moral\l"
	.string "architecture in that, and I have\l"
	.string "fourteen pages of notes proving it.\p"
	.string "The Heartwood is past me, north. That\n"
	.string "is where the Root sleeps. Mind your\l"
	.string "step in there. The jungle pays\l"
	.string "attention.$"

PrimalisAncientRuinsCamp_Text_LensResolved:
	.string "I felt the Root settle from right here.\n"
	.string "The whole camp went still and then the\l"
	.string "birds started up again.\p"
	.string "I have spent my career reading about\n"
	.string "people who did the right thing a\l"
	.string "thousand years ago. It is something\l"
	.string "else to watch it happen.$"

@ ===================================================================
@ Text - the altar
@ ===================================================================
PrimalisAncientRuinsCamp_Text_Altar1:
	.string "A low altar of black, unfamiliar stone.\n"
	.string "The Zoan have carved their answer into\l"
	.string "the base of someone else's work.$"

PrimalisAncientRuinsCamp_Text_Altar2:
	.string "WE DID NOT MAKE THIS. WE FOUND THIS.\p"
	.string "WE CHOSE TO STAY. WE CHOSE TO REMEMBER.\p"
	.string "WE CHOSE TO GUARD WHAT WE DID NOT MAKE\n"
	.string "BECAUSE IT WAS WORTH GUARDING.$"

PrimalisAncientRuinsCamp_Text_AltarClose:
	.string "It is not a boast. It is a promise,\n"
	.string "kept out loud for a thousand years.$"

PrimalisAncientRuinsCamp_Text_AltarAgain:
	.string "WE CHOSE TO GUARD WHAT WE DID NOT MAKE\n"
	.string "BECAUSE IT WAS WORTH GUARDING.$"
