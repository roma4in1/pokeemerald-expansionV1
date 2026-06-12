Emberveil_CinderholdCity_TempleHall_Interior_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ High Ember Solace - here until FLAG_EMBERVEIL_GYM2_CLEAR (her object
@ hide flag) moves her to the summit. Serene, genuinely kind,
@ absolutely convinced. She already knows who the player is.
@ -------------------------------------------------------------------
EmberveilTempleHallInterior_EventScript_Solace::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilTempleHallInterior_Text_SolaceGreeting, MSGBOX_DEFAULT
	setspeaker 0
	msgbox EmberveilTempleHallInterior_Text_SolaceStudiesYou, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilTempleHallInterior_Text_SolaceKnowsYou, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Worshippers at the flame
@ -------------------------------------------------------------------
EmberveilTempleHallInterior_EventScript_Worshipper1::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Worshipper
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilTempleHallInterior_EventScript_Worshipper1Resolved
	msgbox EmberveilTempleHallInterior_Text_Worshipper1, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHallInterior_EventScript_Worshipper1Resolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilTempleHallInterior_EventScript_Worshipper1PathB
	msgbox EmberveilTempleHallInterior_Text_Worshipper1PathA, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHallInterior_EventScript_Worshipper1PathB::
	msgbox EmberveilTempleHallInterior_Text_Worshipper1PathB, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHallInterior_EventScript_Worshipper2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Worshipper
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilTempleHallInterior_EventScript_Worshipper2Resolved
	msgbox EmberveilTempleHallInterior_Text_Worshipper2, MSGBOX_DEFAULT
	release
	end

EmberveilTempleHallInterior_EventScript_Worshipper2Resolved::
	msgbox EmberveilTempleHallInterior_Text_Worshipper2Resolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ The central flame - vision flash. The cult's founders witnessed
@ Pyrath's sealing and began the rituals believing they were
@ communion. Repeatable (the vision is the room's memory, not a
@ one-time event); no flags involved.
@ -------------------------------------------------------------------
EmberveilTempleHallInterior_EventScript_CentralFlame::
	lockall
	msgbox EmberveilTempleHallInterior_Text_FlameApproach, MSGBOX_DEFAULT
	closemessage
	playse SE_M_FLAME_WHEEL
	fadescreen FADE_TO_WHITE
	delay 30
	fadescreen FADE_FROM_WHITE
	msgbox EmberveilTempleHallInterior_Text_FlameVision1, MSGBOX_DEFAULT
	msgbox EmberveilTempleHallInterior_Text_FlameVision2, MSGBOX_DEFAULT
	msgbox EmberveilTempleHallInterior_Text_FlameVision3, MSGBOX_DEFAULT
	closemessage
	playse SE_M_FLAME_WHEEL
	fadescreen FADE_TO_WHITE
	delay 20
	fadescreen FADE_FROM_WHITE
	msgbox EmberveilTempleHallInterior_Text_FlameAfter, MSGBOX_DEFAULT
	releaseall
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
EmberveilTempleHallInterior_Text_SolaceGreeting:
	.string "Welcome, traveler. Sit, if you\n"
	.string "like. The flame doesn't mind\l"
	.string "being watched.\p"
	.string "Most visitors expect me to\n"
	.string "preach at them. I won't. The\l"
	.string "mountain speaks better than I\l"
	.string "do.$"

EmberveilTempleHallInterior_Text_SolaceStudiesYou:
	.string "She studies your face for a\n"
	.string "long, unhurried moment.\p"
	.string "Whatever she finds there seems\n"
	.string "to settle something.$"

EmberveilTempleHallInterior_Text_SolaceKnowsYou:
	.string "You have a keeper's eyes.\p"
	.string "I knew someone with eyes like\n"
	.string "that, once. They sat with me an\l"
	.string "hour and asked one question the\l"
	.string "whole time.\p"
	.string "Climb when you're ready, child.\n"
	.string "The mountain and I will both\l"
	.string "still be here.$"

EmberveilTempleHallInterior_Text_Worshipper1:
	.string "I sit here an hour every\n"
	.string "evening. So did my father.\l"
	.string "So did his mother.\p"
	.string "Some families pass down land.\n"
	.string "Ours passes down warmth.$"

EmberveilTempleHallInterior_Text_Worshipper1PathA:
	.string "My father sat here. His mother\n"
	.string "before him.\p"
	.string "I keep sitting. The bench is\n"
	.string "warm whether or not I know what\l"
	.string "the warmth is for.\p"
	.string "Maybe that's all faith ever was.\n"
	.string "I'm still deciding.$"

EmberveilTempleHallInterior_Text_Worshipper1PathB:
	.string "Three generations of my family\n"
	.string "sat on this bench calling out\l"
	.string "to PYRATH.\p"
	.string "And PYRATH heard every one of\n"
	.string "us. That's what the HIGH EMBER\l"
	.string "says. Heard. All along.\p"
	.string "I'd sit here another hundred\n"
	.string "years for that.$"

EmberveilTempleHallInterior_Text_Worshipper2:
	.string "The flame leans toward the\n"
	.string "mountain. Have you noticed?\p"
	.string "No draft in here. It just\n"
	.string "leans. Like it's listening.$"

EmberveilTempleHallInterior_Text_Worshipper2Resolved:
	.string "The flame stands straight now.\n"
	.string "First time I've ever seen it.\p"
	.string "Whatever it was listening for,\n"
	.string "I think it finally heard.$"

EmberveilTempleHallInterior_Text_FlameApproach:
	.string "The central flame fills the\n"
	.string "brazier without a sound - no\l"
	.string "crackle, no smoke.\p"
	.string "The heat of it reaches you like\n"
	.string "a hand on the shoulder.\p"
	.string "You lean closer-$"

EmberveilTempleHallInterior_Text_FlameVision1:
	.string "-and the hall is gone.\p"
	.string "Figures in ancient robes stand\n"
	.string "on bare volcanic rock, arms\l"
	.string "raised against a sky the color\l"
	.string "of a wound.\p"
	.string "Below them, something vast and\n"
	.string "burning sinks into the mountain,\l"
	.string "and the rock closes over it\l"
	.string "like water.$"

EmberveilTempleHallInterior_Text_FlameVision2:
	.string "The figures kneel. One of them\n"
	.string "kindles a small fire and begins\l"
	.string "to chant - grief, gratitude,\l"
	.string "something between.\p"
	.string "The seal beneath them pulses\n"
	.string "once, softly, in answer.$"

EmberveilTempleHallInterior_Text_FlameVision3:
	.string "Generations blur past. The fire\n"
	.string "is passed hand to hand. The\l"
	.string "chant changes words but never\l"
	.string "rhythm.\p"
	.string "Each generation believes it is\n"
	.string "calling to its god.\p"
	.string "Each generation is keeping a\n"
	.string "lock turned.$"

EmberveilTempleHallInterior_Text_FlameAfter:
	.string "The hall returns around you.\p"
	.string "The flame has burned here for\n"
	.string "two centuries.\p"
	.string "It has never gone out.$"
