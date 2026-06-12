Sirocco_DustmouthPort_BlackMarket_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Hidden basement market. Three vendors at NORMAL prices - the
@ player's reward for exploring (contrast with the taxed overworld).
@ -------------------------------------------------------------------
SiroccoBlackMarket_EventScript_VendorA::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message SiroccoBlackMarket_Text_VendorAGreeting
	waitmessage
	pokemart SiroccoBlackMarket_PokemartGeneral
	setspeaker Pelagios_Speaker_Merchant
	msgbox SiroccoBlackMarket_Text_VendorAFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SiroccoBlackMarket_PokemartGeneral:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_NONE
	release
	end

SiroccoBlackMarket_EventScript_VendorB::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message SiroccoBlackMarket_Text_VendorBGreeting
	waitmessage
	pokemart SiroccoBlackMarket_PokemartMedicine
	setspeaker Pelagios_Speaker_Merchant
	msgbox SiroccoBlackMarket_Text_VendorBFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SiroccoBlackMarket_PokemartMedicine:
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

SiroccoBlackMarket_EventScript_VendorC::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	message SiroccoBlackMarket_Text_VendorCGreeting
	waitmessage
	pokemart SiroccoBlackMarket_PokemartBattle
	setspeaker Pelagios_Speaker_Merchant
	msgbox SiroccoBlackMarket_Text_VendorCFarewell, MSGBOX_DEFAULT
	release
	end

	.align 2
SiroccoBlackMarket_PokemartBattle:
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_NONE
	release
	end

@ -------------------------------------------------------------------
@ Information broker - hints at Dagan's real plan
@ -------------------------------------------------------------------
SiroccoBlackMarket_EventScript_InfoBroker::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Broker
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoBlackMarket_EventScript_InfoBrokerResolved
	goto_if_set FLAG_SIROCCO_SEAL_FOUND, SiroccoBlackMarket_EventScript_InfoBrokerLate
	msgbox SiroccoBlackMarket_Text_InfoBroker, MSGBOX_DEFAULT
	release
	end

SiroccoBlackMarket_EventScript_InfoBrokerLate::
	msgbox SiroccoBlackMarket_Text_InfoBrokerLate, MSGBOX_DEFAULT
	release
	end

SiroccoBlackMarket_EventScript_InfoBrokerResolved::
	msgbox SiroccoBlackMarket_Text_InfoBrokerResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoBlackMarket_Text_VendorAGreeting:
	.string "No tariff down here. No fee.\n"
	.string "No claw stamped on anything.\p"
	.string "Just honest goods at honest\n"
	.string "prices, sold dishonestly.$"

SiroccoBlackMarket_Text_VendorAFarewell:
	.string "You were never here. I was never\n"
	.string "here. Standard arrangement.$"

SiroccoBlackMarket_Text_VendorBGreeting:
	.string "Medicine. The real kind, not the\n"
	.string "watered-down bottles upstairs.\p"
	.string "Heatstroke season never ends on\n"
	.string "this island. Stock up.$"

SiroccoBlackMarket_Text_VendorBFarewell:
	.string "Drink before you're thirsty.\n"
	.string "That's free advice. The only\l"
	.string "free thing I carry.$"

SiroccoBlackMarket_Text_VendorCGreeting:
	.string "Battle goods. The Gilt Claw\n"
	.string "doesn't like islanders owning\l"
	.string "these.\p"
	.string "Which is exactly why I sell\n"
	.string "them.$"

SiroccoBlackMarket_Text_VendorCFarewell:
	.string "Aim well.$"

SiroccoBlackMarket_Text_InfoBroker:
	.string "Information's my trade, and here's\n"
	.string "a free sample.\p"
	.string "DAGAN isn't just selling water.\n"
	.string "He's waiting for something.\p"
	.string "Like he knows something will run\n"
	.string "out.\p"
	.string "Nobody hoards a thing unless they\n"
	.string "know exactly when it ends.$"

SiroccoBlackMarket_Text_InfoBrokerLate:
	.string "So. You've seen what's under the\n"
	.string "BURIED CITY.\p"
	.string "I'd pay well for the details.\p"
	.string "…No? Can't blame me for asking.\n"
	.string "Some things shouldn't be sold,\l"
	.string "I suppose. Not MANY things.\l"
	.string "But some.$"

SiroccoBlackMarket_Text_InfoBrokerResolved:
	.string "The water's back, the boss is\n"
	.string "gone, and information about\l"
	.string "where he went is worth more\l"
	.string "than water ever was.\p"
	.string "When I know, the right people\n"
	.string "will know. For the right price.$"
