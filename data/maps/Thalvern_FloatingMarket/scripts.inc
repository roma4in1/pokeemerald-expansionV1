Thalvern_FloatingMarket_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 1 - Tide (Water). Converted warehouse on the market.
@ Talk-initiated (TRAINER_TYPE_NONE) so it can branch. Badge is
@ NARRATIVE-ONLY: fanfare + "received" text, FLAG_THALVERN_GYM1_CLEAR
@ only, no FLAG_BADGE*_GET (engine badge slots are exhausted). Gives
@ the Tide Badge then TM Surf (as briefed).
@ ===================================================================
ThalvernFloatingMarket_EventScript_Tide::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Tide
	goto_if_set FLAG_THALVERN_GYM1_CLEAR, ThalvernFloatingMarket_EventScript_TidePostBattle
	msgbox ThalvernFloatingMarket_Text_TidePre, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEADER_THALVERN_TIDE, ThalvernFloatingMarket_Text_TideDefeat
	goto ThalvernFloatingMarket_EventScript_TideVictory

ThalvernFloatingMarket_EventScript_TideVictory::
	setspeaker Pelagios_Speaker_Tide
	setflag FLAG_THALVERN_GYM1_CLEAR
	@ Advance progress to 2 (linear: Tide is the first gym).
	setvar VAR_THALVERN_PROGRESS, 2
	msgbox ThalvernFloatingMarket_Text_TideVictory, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message ThalvernFloatingMarket_Text_ReceivedTideBadge
	waitmessage
	waitfanfare
	closemessage
	@ Brief asks for "TM for Surf equivalent". No Surf TM exists (Surf is
	@ an HM in the vanilla set), so substitute TM Water Pulse - the only
	@ offensive Water TM - and lampshade it in dialogue.
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, ThalvernFloatingMarket_EventScript_TideBagFull
	setspeaker Pelagios_Speaker_Tide
	msgbox ThalvernFloatingMarket_Text_TideTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernFloatingMarket_EventScript_TideBagFull::
	setspeaker Pelagios_Speaker_Tide
	msgbox ThalvernFloatingMarket_Text_TideBagFull, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernFloatingMarket_EventScript_TidePostBattle::
	msgbox ThalvernFloatingMarket_Text_TidePost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Numa Vess on the balcony - distant, watching, not interactable in a
@ meaningful sense. A single observation line.
@ ===================================================================
ThalvernFloatingMarket_EventScript_NumaVessBalcony::
	lockall
	msgbox ThalvernFloatingMarket_Text_NumaBalcony, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Child drawing the visions - innocent, disturbing. Post-resolution
@ variant: the drawing is finished and accurate.
@ ===================================================================
ThalvernFloatingMarket_EventScript_DrawingChild::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernFloatingMarket_EventScript_ChildResolved
	msgbox ThalvernFloatingMarket_Text_Child, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernFloatingMarket_EventScript_ChildResolved::
	msgbox ThalvernFloatingMarket_Text_ChildResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Elder who has seen visions for years.
@ ===================================================================
ThalvernFloatingMarket_EventScript_Elder::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Elder
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernFloatingMarket_EventScript_ElderResolved
	msgbox ThalvernFloatingMarket_Text_Elder, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernFloatingMarket_EventScript_ElderResolved::
	msgbox ThalvernFloatingMarket_Text_ElderResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Market vendor - flavor.
@ ===================================================================
ThalvernFloatingMarket_EventScript_Vendor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Vendor
	msgbox ThalvernFloatingMarket_Text_Vendor, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Signs
@ ===================================================================
ThalvernFloatingMarket_EventScript_WarehouseSign::
	lockall
	msgbox ThalvernFloatingMarket_Text_WarehouseSign, MSGBOX_SIGN
	releaseall
	end

ThalvernFloatingMarket_EventScript_BalconySign::
	lockall
	msgbox ThalvernFloatingMarket_Text_BalconySign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernFloatingMarket_Text_TidePre:
	.string "I was a fisherman for thirty years.\p"
	.string "Then the fish left and the visions\n"
	.string "started.\p"
	.string "Now I guard this market, because\n"
	.string "someone has to.\p"
	.string "You want to go into the ruins.\n"
	.string "Everyone wants to go into the ruins.\p"
	.string "Show me you can handle what's in\n"
	.string "there.$"

ThalvernFloatingMarket_Text_TideDefeat:
	.string "Strong. And steady with it.$"

ThalvernFloatingMarket_Text_TideVictory:
	.string "All right. You'll do.\p"
	.string "Take the Tide Badge. You earned the\n"
	.string "right to drown in those visions\l"
	.string "like the rest of us.$"

ThalvernFloatingMarket_Text_ReceivedTideBadge:
	.string "{PLAYER} received the Tide Badge.$"

ThalvernFloatingMarket_Text_TideTM:
	.string "And this. Water Pulse. Not the open-\n"
	.string "sea move you were maybe hoping for,\l"
	.string "but it carries in the flooded\l"
	.string "chambers. Trust me.\p"
	.string "The visions get louder the deeper\n"
	.string "you go.\p"
	.string "Find the scholar - PSALM. She's been\n"
	.string "down there longer than anyone and\l"
	.string "she's still sane. Mostly.$"

ThalvernFloatingMarket_Text_TideBagFull:
	.string "Your bag's full. Come back for the\n"
	.string "TM when you've made room.\p"
	.string "Find PSALM in the ruins. She'll\n"
	.string "point you deeper.$"

ThalvernFloatingMarket_Text_TidePost:
	.string "The visions get louder the deeper\n"
	.string "you go.\p"
	.string "Find PSALM. She's still sane.\n"
	.string "Mostly.$"

ThalvernFloatingMarket_Text_NumaBalcony:
	.string "A woman watches the market from a\n"
	.string "high balcony. Covenant grey.\p"
	.string "She does not move. She is simply\n"
	.string "noting things down.$"

ThalvernFloatingMarket_Text_Child:
	.string "I keep dreaming the same city.\p"
	.string "Can you draw? I can't get the\n"
	.string "towers right.\p"
	.string "(The child is sketching buildings\n"
	.string "with impossible detail - buildings\l"
	.string "no one alive has seen.)$"

ThalvernFloatingMarket_Text_ChildResolved:
	.string "I finished the drawing.\p"
	.string "(A complete, accurate sketch of an\n"
	.string "ancient capital. Perfect. Drawn by\l"
	.string "a child who has never seen it.)\p"
	.string "I don't know why I drew this.\p"
	.string "I don't think I'll draw it again.$"

ThalvernFloatingMarket_Text_Elder:
	.string "It gets easier. You stop being\n"
	.string "frightened of the dead city. You\l"
	.string "start trying to understand it.\p"
	.string "I've been seeing it for eleven\n"
	.string "years. I know their faces now.\l"
	.string "Some of them.\p"
	.string "I don't know their names.$"

ThalvernFloatingMarket_Text_ElderResolved:
	.string "The faces are gone. Eleven years\n"
	.string "of faces - gone this morning.\p"
	.string "I hope they know it's peaceful\n"
	.string "now.$"

ThalvernFloatingMarket_Text_Vendor:
	.string "Buy something, will you? Half my\n"
	.string "regulars stopped coming when the\l"
	.string "visions started.\p"
	.string "Hard to haggle when you keep seeing\n"
	.string "a city that drowned.$"

ThalvernFloatingMarket_Text_WarehouseSign:
	.string "TIDE'S WAREHOUSE\n"
	.string "Gym Leader: Tide.\l"
	.string "Mind your footing - the floor\l"
	.string "floods.$"

ThalvernFloatingMarket_Text_BalconySign:
	.string "Covenant Archaeological Division.\n"
	.string "Field Office. No admittance.$"
