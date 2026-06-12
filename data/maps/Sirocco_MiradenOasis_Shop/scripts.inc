Sirocco_MiradenOasis_Shop_MapScripts::
	.byte 0

@ Gilt Claw branded shop - normal prices here. Dagan is smart enough
@ to keep the main town functional.
SiroccoMiradenOasisShop_EventScript_Clerk::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Clerk
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasisShop_EventScript_ClerkResolved
	message SiroccoMiradenOasisShop_Text_Greeting
	waitmessage
	goto SiroccoMiradenOasisShop_EventScript_Mart

SiroccoMiradenOasisShop_EventScript_ClerkResolved::
	message SiroccoMiradenOasisShop_Text_GreetingResolved
	waitmessage
SiroccoMiradenOasisShop_EventScript_Mart::
	pokemart SiroccoMiradenOasisShop_Pokemart
	setspeaker Pelagios_Speaker_Clerk
	msgbox SiroccoMiradenOasisShop_Text_Farewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SiroccoMiradenOasisShop_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_NONE
	release
	end

SiroccoMiradenOasisShop_Text_Greeting:
	.string "Welcome to MIRADEN GENERAL\n"
	.string "SUPPLY, a Gilt Claw retail\l"
	.string "partner.\p"
	.string "Standard prices in MIRADEN.\n"
	.string "The boss likes this town\l"
	.string "comfortable.\p"
	.string "Comfortable towns don't ask\n"
	.string "questions.$"

SiroccoMiradenOasisShop_Text_GreetingResolved:
	.string "Welcome to MIRADEN GENERAL\n"
	.string "SUPPLY!\p"
	.string "We pried the claw sign off the\n"
	.string "roof yesterday. Took four of us.\p"
	.string "Heaviest thing I've ever been\n"
	.string "happy to carry.$"

SiroccoMiradenOasisShop_Text_Farewell:
	.string "Need anything else, you know\n"
	.string "where I am.$"
