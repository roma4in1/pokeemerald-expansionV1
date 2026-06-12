HavenIsle_Mart_MapScripts::
	.byte 0

HavenIsleMart_EventScript_Clerk::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Clerk
	message gText_HowMayIServeYou
	waitmessage
	pokemart HavenIsleMart_Pokemart
	setspeaker Pelagios_Speaker_Clerk
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
HavenIsleMart_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

HavenIsleMart_EventScript_Woman::
	setspeaker Pelagios_Speaker_Woman
	msgbox HavenIsleMart_Text_Woman, MSGBOX_NPC
	end

HavenIsleMart_Text_Woman:
	.string "Half this stock comes in by boat\n"
	.string "from SLATEPORT, weeks at sea.\p"
	.string "The other half is dried fish.\n"
	.string "Guess which half sells out first.$"
