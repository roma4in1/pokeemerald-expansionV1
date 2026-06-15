Gildhaven_BlackMarket_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Gildhaven_BlackMarket_OnTransition
	.byte 0

@ Dagan's object in map.json carries hide flag FLAG_GILDHAVEN_GYM1_CLEAR.
@ The brief wants Dagan to APPEAR after Gym 1, but the engine HIDES an
@ object while its hide flag is SET. So we drive visibility explicitly in
@ ON_TRANSITION instead of relying on the spawn flag:
@   Gym 1 NOT clear -> remove Dagan (he is not here yet).
@   Gym 1 clear      -> force-add Dagan (override the hide flag).
Gildhaven_BlackMarket_OnTransition::
	goto_if_unset FLAG_GILDHAVEN_GYM1_CLEAR, Gildhaven_BlackMarket_HideDagan
	addobject LOCALID_GILDHAVEN_DAGAN
	end

Gildhaven_BlackMarket_HideDagan::
	removeobject LOCALID_GILDHAVEN_DAGAN
	end

@ ===================================================================
@ Dagan Mire - returning from Sirocco. Gated by Gym 1 (object hidden
@ until then). First meeting: extended monologue, two question beats
@ that bump VAR_DAGAN_RELATIONSHIP (continuity with Sirocco's 0-2;
@ var is a 0-3 comedy scale, so we cap at 3). Sets FLAG_GILDHAVEN_DAGAN_MET.
@ ===================================================================
GildhavenBlackMarket_EventScript_Dagan::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dagan
	goto_if_set FLAG_GILDHAVEN_DAGAN_MET, GildhavenBlackMarket_EventScript_DaganSubsequent
	goto GildhavenBlackMarket_EventScript_DaganFirstMeeting

@ ----- First meeting -----
GildhavenBlackMarket_EventScript_DaganFirstMeeting::
	msgbox GildhavenBlackMarket_Text_DaganMeet1, MSGBOX_DEFAULT
	msgbox GildhavenBlackMarket_Text_DaganMeet2, MSGBOX_DEFAULT
	@ Question 1: did the player engage with his Sirocco callback?
	msgbox GildhavenBlackMarket_Text_DaganQuestion1, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, GildhavenBlackMarket_EventScript_DaganBumpRel
	setspeaker Pelagios_Speaker_Dagan
	goto_if_eq VAR_RESULT, YES, GildhavenBlackMarket_EventScript_DaganQ1Yes
	msgbox GildhavenBlackMarket_Text_DaganQ1No, MSGBOX_DEFAULT
	goto GildhavenBlackMarket_EventScript_DaganMeetIntel

GildhavenBlackMarket_EventScript_DaganQ1Yes::
	msgbox GildhavenBlackMarket_Text_DaganQ1Yes, MSGBOX_DEFAULT
	goto GildhavenBlackMarket_EventScript_DaganMeetIntel

GildhavenBlackMarket_EventScript_DaganMeetIntel::
	setspeaker Pelagios_Speaker_Dagan
	msgbox GildhavenBlackMarket_Text_DaganIntel, MSGBOX_DEFAULT
	@ Question 2: the player presses Dagan on what he actually wants.
	msgbox GildhavenBlackMarket_Text_DaganQuestion2, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, GildhavenBlackMarket_EventScript_DaganBumpRel
	setspeaker Pelagios_Speaker_Dagan
	goto_if_eq VAR_RESULT, YES, GildhavenBlackMarket_EventScript_DaganQ2Yes
	msgbox GildhavenBlackMarket_Text_DaganQ2No, MSGBOX_DEFAULT
	goto GildhavenBlackMarket_EventScript_DaganMeetClose

GildhavenBlackMarket_EventScript_DaganQ2Yes::
	msgbox GildhavenBlackMarket_Text_DaganQ2Yes, MSGBOX_DEFAULT
	goto GildhavenBlackMarket_EventScript_DaganMeetClose

GildhavenBlackMarket_EventScript_DaganMeetClose::
	setspeaker Pelagios_Speaker_Dagan
	msgbox GildhavenBlackMarket_Text_DaganMeetClose, MSGBOX_DEFAULT
	setspeaker 0
	setflag FLAG_GILDHAVEN_DAGAN_MET
	release
	end

@ Increment the relationship var, capped at 3.
GildhavenBlackMarket_EventScript_DaganBumpRel::
	compare VAR_DAGAN_RELATIONSHIP, 3
	goto_if_ge GildhavenBlackMarket_EventScript_DaganBumpRelDone
	addvar VAR_DAGAN_RELATIONSHIP, 1
GildhavenBlackMarket_EventScript_DaganBumpRelDone::
	return

@ ----- Subsequent visits -----
GildhavenBlackMarket_EventScript_DaganSubsequent::
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenBlackMarket_EventScript_DaganResolved
	compare VAR_DAGAN_RELATIONSHIP, 2
	goto_if_ge GildhavenBlackMarket_EventScript_DaganSubsequentWarm
	msgbox GildhavenBlackMarket_Text_DaganSubsequent, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenBlackMarket_EventScript_DaganSubsequentWarm::
	msgbox GildhavenBlackMarket_Text_DaganSubsequentWarm, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenBlackMarket_EventScript_DaganResolved::
	msgbox GildhavenBlackMarket_Text_DaganResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Black-market vendors - fair prices (the honest counterpoint to the
@ overcharging stalls above).
@ ===================================================================
GildhavenBlackMarket_EventScript_Vendor1::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message GildhavenBlackMarket_Text_Vendor1Greeting
	waitmessage
	pokemart GildhavenBlackMarket_Pokemart1
	setspeaker Pelagios_Speaker_Merchant
	msgbox GildhavenBlackMarket_Text_Vendor1Farewell, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

	.align 2
GildhavenBlackMarket_Pokemart1:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REVIVE
	.2byte ITEM_PP_UP
	.2byte ITEM_NONE
	release
	end

GildhavenBlackMarket_EventScript_Vendor2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message GildhavenBlackMarket_Text_Vendor2Greeting
	waitmessage
	pokemart GildhavenBlackMarket_Pokemart2
	setspeaker Pelagios_Speaker_Merchant
	msgbox GildhavenBlackMarket_Text_Vendor2Farewell, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

	.align 2
GildhavenBlackMarket_Pokemart2:
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CALCIUM
	.2byte ITEM_CARBOS
	.2byte ITEM_ZINC
	.2byte ITEM_HP_UP
	.2byte ITEM_RARE_CANDY
	.2byte ITEM_NONE
	release
	end

@ ===================================================================
@ Information broker - knows about the seal beneath the Exchange.
@ ===================================================================
GildhavenBlackMarket_EventScript_InfoBroker::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Broker
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenBlackMarket_EventScript_InfoBrokerResolved
	goto_if_set FLAG_GILDHAVEN_COVENANT_MAP_SEEN, GildhavenBlackMarket_EventScript_InfoBrokerMapSeen
	msgbox GildhavenBlackMarket_Text_InfoBroker, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenBlackMarket_EventScript_InfoBrokerMapSeen::
	msgbox GildhavenBlackMarket_Text_InfoBrokerMapSeen, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenBlackMarket_EventScript_InfoBrokerResolved::
	msgbox GildhavenBlackMarket_Text_InfoBrokerResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Text - Dagan
@ ===================================================================
GildhavenBlackMarket_Text_DaganMeet1:
	.string "Ah. The WARDEN's child. You found me.\p"
	.string "I am impressed. It took the GILT CLAW\n"
	.string "three weeks to find this place. You\l"
	.string "managed it in an afternoon.\p"
	.string "Sit. I have information, and you have\n"
	.string "the only interesting situation on\l"
	.string "this entire glittering island.$"

GildhavenBlackMarket_Text_DaganMeet2:
	.string "I have been here three months.\p"
	.string "Do you know what three months in\n"
	.string "GILDHAVEN does to a man who is paying\l"
	.string "attention?\p"
	.string "It makes him very, very bored.$"

GildhavenBlackMarket_Text_DaganQuestion1:
	.string "He waits, watching you. Ask him how\n"
	.string "SIROCCO turned out?$"

GildhavenBlackMarket_Text_DaganQ1Yes:
	.string "Heh. You actually want to know.\p"
	.string "The oasis came back. I heard the\n"
	.string "water rose a full foot. Good for\l"
	.string "MIRADEN. Good for the people I left\l"
	.string "holding the bill.\p"
	.string "I do not regret much. I am almost\n"
	.string "glad about that one.$"

GildhavenBlackMarket_Text_DaganQ1No:
	.string "No? Straight to business. Fine. I can\n"
	.string "respect a closed door.\p"
	.string "Most people at least pretend to care\n"
	.string "how the last job ended.$"

GildhavenBlackMarket_Text_DaganIntel:
	.string "Here is what I know.\p"
	.string "VANE's grandfather built the EXCHANGE\n"
	.string "directly above something old.\l"
	.string "Something old that makes people want\l"
	.string "things.\p"
	.string "I think you already know what that\n"
	.string "something is.\p"
	.string "The passage down is through the manor\n"
	.string "study. His daughter knows about it.\l"
	.string "She has never used it.$"

GildhavenBlackMarket_Text_DaganQuestion2:
	.string "And his price for all this? Press him\n"
	.string "on what he actually wants?$"

GildhavenBlackMarket_Text_DaganQ2Yes:
	.string "Honestly?\p"
	.string "To watch you pull this off.\p"
	.string "I designed half the security in that\n"
	.string "building. I know exactly how hard\l"
	.string "what you are about to do is.\p"
	.string "I want a front-row seat. That is my\n"
	.string "fee. Consider it paid the moment you\l"
	.string "walk back out of there.$"

GildhavenBlackMarket_Text_DaganQ2No:
	.string "You are not going to ask. Smart.\p"
	.string "People who ask my price usually do\n"
	.string "not like the answer.\p"
	.string "This time they would have. Pity.$"

GildhavenBlackMarket_Text_DaganMeetClose:
	.string "Go on. Beat the daughter, get the key,\n"
	.string "and go ruin the richest man in the\l"
	.string "archipelago.\p"
	.string "I will be right here, having the time\n"
	.string "of my life.$"

GildhavenBlackMarket_Text_DaganSubsequent:
	.string "Still here. Still bored. How is\n"
	.string "progress?\p"
	.string "Do not actually tell me the details.\n"
	.string "The suspense is the only thing\l"
	.string "keeping me entertained.$"

GildhavenBlackMarket_Text_DaganSubsequentWarm:
	.string "There you are. This is the most fun I\n"
	.string "have had since SIROCCO, and I mean\l"
	.string "that sincerely, which alarms me.\p"
	.string "Go. Finish it. I have a small fortune\n"
	.string "riding on you with a bookmaker who\l"
	.string "does not know it yet.$"

GildhavenBlackMarket_Text_DaganResolved:
	.string "You did it. You actually did it.\p"
	.string "The whole island is deflating like a\n"
	.string "soufflé and it is the best theatre I\l"
	.string "have ever paid nothing to see.\p"
	.string "I am leaving before the prices crash\n"
	.string "and someone asks me to explain\l"
	.string "myself. I will send a postcard.\p"
	.string "I always send a postcard.$"

@ ===================================================================
@ Text - Vendors / broker
@ ===================================================================
GildhavenBlackMarket_Text_Vendor1Greeting:
	.string "Down here we do not lie about the\n"
	.string "price. Funny which market turned out\l"
	.string "honest, isn't it.$"

GildhavenBlackMarket_Text_Vendor1Farewell:
	.string "Tell nobody upstairs what you paid.\n"
	.string "They would not believe you anyway.$"

GildhavenBlackMarket_Text_Vendor2Greeting:
	.string "Vitamins, candies, the good stuff.\n"
	.string "No artifacts. No genuine anything.\l"
	.string "Just what it says on the tin.$"

GildhavenBlackMarket_Text_Vendor2Farewell:
	.string "Come back when your team needs an\n"
	.string "edge. We deal in real ones.$"

GildhavenBlackMarket_Text_InfoBroker:
	.string "The EXCHANGE basement has been\n"
	.string "off-limits for forty years.\p"
	.string "VANE's grandfather built it. Built it\n"
	.string "deep.\p"
	.string "Nobody asks why a financial building\n"
	.string "needs a basement that deep.\p"
	.string "I ask. It is my whole job. And the\n"
	.string "answer scares me.$"

GildhavenBlackMarket_Text_InfoBrokerMapSeen:
	.string "You saw the map upstairs, then. The\n"
	.string "one with the yields.\p"
	.string "Now you know what I know, and you\n"
	.string "know why I keep my voice down.$"

GildhavenBlackMarket_Text_InfoBrokerResolved:
	.string "Forty years of off-limits, and now\n"
	.string "the basement is just a basement.\p"
	.string "I am out of secrets to sell. Best\n"
	.string "thing that has happened to this city\l"
	.string "in my lifetime.$"
