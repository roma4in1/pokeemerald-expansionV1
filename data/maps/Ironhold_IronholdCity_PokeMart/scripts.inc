Ironhold_IronholdCity_PokeMart_MapScripts::
	.byte 0

@ Covenant-branded mart. The overpricing is a flavor detail - mart
@ prices are fixed per item, so the clerk jokes about the tariff.
IronholdMart_EventScript_Clerk::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Clerk
	message IronholdMart_Text_Greeting
	waitmessage
	pokemart IronholdMart_Pokemart
	setspeaker Pelagios_Speaker_Clerk
	msgbox IronholdMart_Text_Farewell, MSGBOX_DEFAULT
	release
	end

	.align 2
IronholdMart_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
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

IronholdMart_Text_Greeting:
	.string "Welcome to the IRONHOLD SUPPLY\n"
	.string "DEPOT, a Covenant retail partner.\p"
	.string "Prices include the garrison\n"
	.string "tariff. Don't ask what the\l"
	.string "garrison tariff funds.$"

IronholdMart_Text_Farewell:
	.string "Receipt's in the bag.\p"
	.string "Keep it. They check, sometimes.$"
