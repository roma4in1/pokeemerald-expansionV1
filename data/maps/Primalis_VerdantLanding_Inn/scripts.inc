Primalis_VerdantLanding_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Primalis_VerdantLanding_Inn_OnTransition
	.byte 0

Primalis_VerdantLanding_Inn_OnTransition:
	setrespawn HEAL_LOCATION_PRIMALIS_VERDANT_LANDING
	end

@ ===================================================================
@ Innkeeper - island heal NPC (nurse-gfx, normal nurse path).
@ ===================================================================
PrimalisVerdantLandingInn_EventScript_Innkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Innkeeper
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisVerdantLandingInn_EventScript_InnkeeperResolved
	msgbox PrimalisVerdantLandingInn_Text_InnkeeperGreeting, MSGBOX_DEFAULT
	goto PrimalisVerdantLandingInn_EventScript_RoomOffer

PrimalisVerdantLandingInn_EventScript_InnkeeperResolved::
	msgbox PrimalisVerdantLandingInn_Text_InnkeeperResolved, MSGBOX_DEFAULT
	goto PrimalisVerdantLandingInn_EventScript_RoomOffer

PrimalisVerdantLandingInn_EventScript_RoomOffer::
	msgbox PrimalisVerdantLandingInn_Text_RoomOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PrimalisVerdantLandingInn_EventScript_NoRest
	msgbox PrimalisVerdantLandingInn_Text_RoomsYours, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Pelagios_Speaker_Innkeeper
	msgbox PrimalisVerdantLandingInn_Text_AfterRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLandingInn_EventScript_NoRest::
	msgbox PrimalisVerdantLandingInn_Text_NoRest, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ The Lens (first meeting, with DEX_ALIVE variant). Warm, sharp,
@ following Dex's research leads. Sets FLAG_PRIMALIS_LENS_MET.
@ The Lens NEVER foreshadows Dex's death - if Dex is alive she is
@ warm and forward; if gone, one quiet beat, then she moves on.
@ ===================================================================
PrimalisVerdantLandingInn_EventScript_Lens::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_PRIMALIS_LENS_MET, PrimalisVerdantLandingInn_EventScript_LensAgain
	msgbox PrimalisVerdantLandingInn_Text_LensIntro, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_DEX_ALIVE, PrimalisVerdantLandingInn_EventScript_LensDexAlive
	goto PrimalisVerdantLandingInn_EventScript_LensDexGone

PrimalisVerdantLandingInn_EventScript_LensDexAlive::
	setspeaker Pelagios_Speaker_Lens
	msgbox PrimalisVerdantLandingInn_Text_LensDexAlive, MSGBOX_DEFAULT
	goto PrimalisVerdantLandingInn_EventScript_LensClose

PrimalisVerdantLandingInn_EventScript_LensDexGone::
	setspeaker Pelagios_Speaker_Lens
	msgbox PrimalisVerdantLandingInn_Text_LensDexGone, MSGBOX_DEFAULT
	goto PrimalisVerdantLandingInn_EventScript_LensClose

PrimalisVerdantLandingInn_EventScript_LensClose::
	setspeaker Pelagios_Speaker_Lens
	msgbox PrimalisVerdantLandingInn_Text_LensClose, MSGBOX_DEFAULT
	setspeaker 0
	setflag FLAG_PRIMALIS_LENS_MET
	release
	end

PrimalisVerdantLandingInn_EventScript_LensAgain::
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisVerdantLandingInn_EventScript_LensResolved
	msgbox PrimalisVerdantLandingInn_Text_LensAgain, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisVerdantLandingInn_EventScript_LensResolved::
	msgbox PrimalisVerdantLandingInn_Text_LensResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text - innkeeper
@ ===================================================================
PrimalisVerdantLandingInn_Text_InnkeeperGreeting:
	.string "Rest your team if you need it. We do not\n"
	.string "see many travelers, but a guest is a\l"
	.string "guest.\p"
	.string "Mind the guardians on the routes. They\n"
	.string "are not cruel. They are only careful.$"

PrimalisVerdantLandingInn_Text_InnkeeperResolved:
	.string "The whole village slept through the\n"
	.string "night. First time in three months.\p"
	.string "Some of us had forgotten what the\n"
	.string "forest sounds like when it is calm.\p"
	.string "Rest. You have earned it more than any\n"
	.string "of us.$"

PrimalisVerdantLandingInn_Text_RoomOffer:
	.string "Would you like to rest?$"

PrimalisVerdantLandingInn_Text_RoomsYours:
	.string "Take the back room. It is yours.$"

PrimalisVerdantLandingInn_Text_AfterRest:
	.string "Your team looks ready. The jungle waits.$"

PrimalisVerdantLandingInn_Text_NoRest:
	.string "When you are ready, then.$"

@ ===================================================================
@ Text - the Lens
@ ===================================================================
PrimalisVerdantLandingInn_Text_LensIntro:
	.string "You made it. I wondered if you would.\p"
	.string "I have been here two weeks. The\n"
	.string "community calls the legendary the Root.\p"
	.string "Their oral history is the clearest\n"
	.string "account of the ancient civilization I\l"
	.string "have ever found - and they did not even\l"
	.string "write it down. They remembered it.\l"
	.string "Out loud. For a thousand years.$"

PrimalisVerdantLandingInn_Text_LensDexAlive:
	.string "I sent DEX every page of my notes last\n"
	.string "night. He wrote back within the hour -\l"
	.string "three exclamation points and a list of\l"
	.string "follow-up questions a mile long.\p"
	.string "He would book passage tomorrow if I let\n"
	.string "him. This place is everything he has\l"
	.string "spent his life chasing.$"

PrimalisVerdantLandingInn_Text_LensDexGone:
	.string "DEX would have loved this.\p"
	.string "...\p"
	.string "He chased the ancient civilization his\n"
	.string "whole life. He would have walked into\l"
	.string "that jungle and never looked back.$"

PrimalisVerdantLandingInn_Text_LensClose:
	.string "Elder MAKO holds the full history, and\n"
	.string "he tells it to no outsider. You will\l"
	.string "have to earn that.\p"
	.string "The guardians here measure people by\n"
	.string "how they fight - not whether they win,\l"
	.string "but what they are willing to protect.\p"
	.string "So. Go be measured.$"

PrimalisVerdantLandingInn_Text_LensAgain:
	.string "Every guardian I have spoken to says\n"
	.string "the same thing - it started three\l"
	.string "months ago. The seal slipping, the\l"
	.string "transformations going wrong.\p"
	.string "Three months. That is not ancient\n"
	.string "history. That is now.$"

PrimalisVerdantLandingInn_Text_LensResolved:
	.string "You reinforced it. I felt the change\n"
	.string "even from down here.\p"
	.string "I have a lifetime of notes and you have\n"
	.string "a lifetime of consequences. Funny how\l"
	.string "that works out.\p"
	.string "Go home. There are people who need to\n"
	.string "hear what you learned in that hall.$"
