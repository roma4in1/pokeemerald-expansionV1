Sirocco_MiradenOasis_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 1 - Silt (Ground). Local guide turned outlaw, at his converted
@ water warehouse. No gate - he is the island's first test.
@ Victory: Dust Badge (FLAG_BADGE06_GET), FLAG_SIROCCO_GYM1_CLEAR,
@ VAR_SIROCCO_PROGRESS = 2, then TM Earthquake (as briefed).
@ -------------------------------------------------------------------
SiroccoMiradenOasis_EventScript_Silt::
	goto_if_defeated TRAINER_LEADER_SIROCCO_SILT, SiroccoMiradenOasis_EventScript_SiltDefeated
	setspeaker Pelagios_Speaker_Silt
	trainerbattle_single TRAINER_LEADER_SIROCCO_SILT, SiroccoMiradenOasis_Text_SiltIntro, SiroccoMiradenOasis_Text_SiltDefeat, SiroccoMiradenOasis_EventScript_SiltVictory
	end

SiroccoMiradenOasis_EventScript_SiltVictory::
	msgbox SiroccoMiradenOasis_Text_SiltPostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message SiroccoMiradenOasis_Text_ReceivedDustBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE06_GET
	setflag FLAG_SIROCCO_GYM1_CLEAR
	setvar VAR_SIROCCO_PROGRESS, 2
	setspeaker Pelagios_Speaker_Silt
	msgbox SiroccoMiradenOasis_Text_SiltGoEast, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_EARTHQUAKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Silt
	msgbox SiroccoMiradenOasis_Text_SiltExplainTM, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_SiltDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Silt
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_SiltResolved
	msgbox SiroccoMiradenOasis_Text_SiltPostBadge, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_SiltResolved::
	msgbox SiroccoMiradenOasis_Text_SiltResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Gym 2 - Crag (Rock). Former archaeologist who took Gilt Claw money.
@ Gated on FLAG_SIROCCO_GYM1_CLEAR. Victory: Stone Badge
@ (FLAG_BADGE07_GET), FLAG_SIROCCO_GYM2_CLEAR,
@ FLAG_SIROCCO_NORTH_TERRITORY_OPEN (hides the gate enforcer),
@ VAR_SIROCCO_PROGRESS = 3 (disarms the north-gate triggers), then
@ TM Rock Tomb (brief asks for Stone Edge - no such TM in this
@ project's vanilla set; the dialogue lampshades the substitution).
@ -------------------------------------------------------------------
SiroccoMiradenOasis_EventScript_Crag::
	goto_if_defeated TRAINER_LEADER_SIROCCO_CRAG, SiroccoMiradenOasis_EventScript_CragDefeated
	goto_if_unset FLAG_SIROCCO_GYM1_CLEAR, SiroccoMiradenOasis_EventScript_CragNotYet
	setspeaker Pelagios_Speaker_Crag
	trainerbattle_single TRAINER_LEADER_SIROCCO_CRAG, SiroccoMiradenOasis_Text_CragIntro, SiroccoMiradenOasis_Text_CragDefeat, SiroccoMiradenOasis_EventScript_CragVictory
	end

SiroccoMiradenOasis_EventScript_CragNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Crag
	msgbox SiroccoMiradenOasis_Text_CragNotYet, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_CragVictory::
	msgbox SiroccoMiradenOasis_Text_CragPostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message SiroccoMiradenOasis_Text_ReceivedStoneBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE07_GET
	setflag FLAG_SIROCCO_GYM2_CLEAR
	setflag FLAG_SIROCCO_NORTH_TERRITORY_OPEN
	setvar VAR_SIROCCO_PROGRESS, 3
	setspeaker Pelagios_Speaker_Crag
	msgbox SiroccoMiradenOasis_Text_CragNorthTerritory, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Crag
	msgbox SiroccoMiradenOasis_Text_CragExplainTM, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_CragDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Crag
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_CragResolved
	msgbox SiroccoMiradenOasis_Text_CragPostBadge, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_CragResolved::
	msgbox SiroccoMiradenOasis_Text_CragResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Townsfolk
@ -------------------------------------------------------------------
SiroccoMiradenOasis_EventScript_Child::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_ChildResolved
	msgbox SiroccoMiradenOasis_Text_Child, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_ChildResolved::
	msgbox SiroccoMiradenOasis_Text_ChildResolved, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_OldWoman::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_OldWoman
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_OldWomanResolved
	msgbox SiroccoMiradenOasis_Text_OldWoman, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_OldWomanResolved::
	msgbox SiroccoMiradenOasis_Text_OldWomanResolved, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_WaterDistributor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_GiltClaw
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_DistributorResolved
	msgbox SiroccoMiradenOasis_Text_WaterDistributor, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_DistributorResolved::
	msgbox SiroccoMiradenOasis_Text_DistributorResolved, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_Scholar::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Scholar
	goto_if_set FLAG_SIROCCO_BURIED_CITY_FOUND, SiroccoMiradenOasis_EventScript_ScholarLate
	msgbox SiroccoMiradenOasis_Text_Scholar, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_ScholarLate::
	msgbox SiroccoMiradenOasis_Text_ScholarLate, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_Elder::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Elder
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoMiradenOasis_EventScript_ElderResolved
	msgbox SiroccoMiradenOasis_Text_Elder, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_ElderResolved::
	msgbox SiroccoMiradenOasis_Text_ElderResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ North gate - Gilt Claw territory, blocked until Gym 2.
@ The enforcer stands ON the west gate tile (13,1); the coord
@ triggers (armed at progress 1 and 2) bounce the player back from
@ (14,1). Both disappear once Crag falls.
@ -------------------------------------------------------------------
SiroccoMiradenOasis_EventScript_NorthBlocker::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Enforcer
	msgbox SiroccoMiradenOasis_Text_NorthBlocker, MSGBOX_DEFAULT
	release
	end

SiroccoMiradenOasis_EventScript_NorthGate::
	lockall
	playse SE_PIN
	applymovement LOCALID_SIROCCO_NORTH_BLOCKER, Common_Movement_ExclamationMark
	waitmovement 0
	setspeaker Pelagios_Speaker_Enforcer
	msgbox SiroccoMiradenOasis_Text_NorthGateHalt, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, SiroccoMiradenOasis_Movement_PlayerStepBack
	waitmovement 0
	releaseall
	end

SiroccoMiradenOasis_Movement_PlayerStepBack:
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SiroccoMiradenOasis_EventScript_Gym1Sign::
	msgbox SiroccoMiradenOasis_Text_Gym1Sign, MSGBOX_SIGN
	end

SiroccoMiradenOasis_EventScript_Gym2Sign::
	msgbox SiroccoMiradenOasis_Text_Gym2Sign, MSGBOX_SIGN
	end

SiroccoMiradenOasis_EventScript_CenterSign::
	msgbox SiroccoMiradenOasis_Text_CenterSign, MSGBOX_SIGN
	end

SiroccoMiradenOasis_EventScript_ShopSign::
	msgbox SiroccoMiradenOasis_Text_ShopSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text - Silt
@ -------------------------------------------------------------------
SiroccoMiradenOasis_Text_SiltIntro:
	.string "I used to guide people across\n"
	.string "this desert for a living.\p"
	.string "Knew every dune. Every safe path.\p"
	.string "Then DAGAN bought the water\n"
	.string "rights, and the paths stopped\l"
	.string "mattering. Now I work for him.\p"
	.string "Beat me, and maybe I'll remember\n"
	.string "what I used to be.$"

SiroccoMiradenOasis_Text_SiltDefeat:
	.string "…There it is.\n"
	.string "There's the old feeling.$"

SiroccoMiradenOasis_Text_SiltPostBattle:
	.string "For a long moment he looks past\n"
	.string "you, out at the dunes, the way a\l"
	.string "man looks at an old friend across\l"
	.string "a crowded room.$"

SiroccoMiradenOasis_Text_ReceivedDustBadge:
	.string "{PLAYER} received the DUST BADGE\n"
	.string "from SILT!$"

SiroccoMiradenOasis_Text_SiltGoEast:
	.string "…Go east. Find the\n"
	.string "archaeologist's camp.\p"
	.string "Don't let DAGAN's people see you\n"
	.string "heading there.$"

SiroccoMiradenOasis_Text_SiltExplainTM:
	.string "EARTHQUAKE. The desert's own\n"
	.string "argument.\p"
	.string "A guide's last lesson: when the\n"
	.string "ground speaks, everything else\l"
	.string "goes quiet.$"

SiroccoMiradenOasis_Text_SiltPostBadge:
	.string "The camp's east of the main\n"
	.string "route. Stay off the ridgeline -\l"
	.string "the watchers are lazy, but\l"
	.string "they're not blind.$"

SiroccoMiradenOasis_Text_SiltResolved:
	.string "I remembered what the old paths\n"
	.string "feel like.\p"
	.string "Turns out they're still there.$"

@ -------------------------------------------------------------------
@ Text - Crag
@ -------------------------------------------------------------------
SiroccoMiradenOasis_Text_CragNotYet:
	.string "You want the excavation gym?\n"
	.string "Earn the desert's badge first.\p"
	.string "SILT's warehouse, west side.\n"
	.string "He checks people. I dig.$"

SiroccoMiradenOasis_Text_CragIntro:
	.string "I needed funding for the dig.\n"
	.string "DAGAN provided it.\p"
	.string "I told myself I wasn't\n"
	.string "responsible for what he did\l"
	.string "with the information.\p"
	.string "Funny how that works,\n"
	.string "isn't it.$"

SiroccoMiradenOasis_Text_CragDefeat:
	.string "…Not funny at all,\n"
	.string "as it turns out.$"

SiroccoMiradenOasis_Text_CragPostBattle:
	.string "He brushes rock dust from his\n"
	.string "hands, slowly, like he's deciding\l"
	.string "something much heavier than a\l"
	.string "battle.$"

SiroccoMiradenOasis_Text_ReceivedStoneBadge:
	.string "{PLAYER} received the STONE BADGE\n"
	.string "from CRAG!$"

SiroccoMiradenOasis_Text_CragNorthTerritory:
	.string "The north territory. DAGAN's\n"
	.string "compound.\p"
	.string "I've given him everything he\n"
	.string "asked for.\p"
	.string "I don't know what's under that\n"
	.string "seal. But I know it's not his\l"
	.string "to take.\p"
	.string "I'll tell the gate crew you're\n"
	.string "cleared. It's the least of what\l"
	.string "I owe.$"

SiroccoMiradenOasis_Text_CragExplainTM:
	.string "And this. ROCK TOMB - quarried\n"
	.string "the disc myself.\p"
	.string "I wanted to give you STONE EDGE,\n"
	.string "but the Gilt Claw buys every\l"
	.string "shipment before it lands.\p"
	.string "He even owns the rocks.\n"
	.string "Think about that.$"

SiroccoMiradenOasis_Text_CragPostBadge:
	.string "The north gate will let you\n"
	.string "through now.\p"
	.string "Whatever you find up there -\n"
	.string "I helped him find it first.\l"
	.string "Remember that when you're\l"
	.string "deciding what I am.$"

SiroccoMiradenOasis_Text_CragResolved:
	.string "I'm going back to archaeology.\n"
	.string "Properly, this time.\p"
	.string "DEX said I could work his camp.\p"
	.string "I said yes before he finished\n"
	.string "the sentence.$"

@ -------------------------------------------------------------------
@ Text - townsfolk
@ -------------------------------------------------------------------
SiroccoMiradenOasis_Text_Child:
	.string "Mum says don't go past the wet\n"
	.string "part.\p"
	.string "The wet part keeps getting\n"
	.string "smaller.\p"
	.string "I measure it every morning with\n"
	.string "a stick.\p"
	.string "Today it was smaller again.$"

SiroccoMiradenOasis_Text_ChildResolved:
	.string "The wet part got BIGGER!\p"
	.string "I measured it this morning.\n"
	.string "It got bigger by half a stick!$"

SiroccoMiradenOasis_Text_OldWoman:
	.string "I've lived here seventy years.\p"
	.string "The oasis used to take ten\n"
	.string "minutes to walk around.\l"
	.string "Now it takes two.\p"
	.string "Something is drinking it,\n"
	.string "and it isn't us.$"

SiroccoMiradenOasis_Text_OldWomanResolved:
	.string "The oasis is breathing again.\p"
	.string "I didn't think I'd live to\n"
	.string "see it.$"

SiroccoMiradenOasis_Text_WaterDistributor:
	.string "Daily ration. Don't argue about\n"
	.string "the amount.\p"
	.string "Don't ask what happens when it\n"
	.string "runs out.\p"
	.string "Those aren't questions I'm\n"
	.string "allowed to answer.$"

SiroccoMiradenOasis_Text_DistributorResolved:
	.string "The ration's suspended. The\n"
	.string "oasis is up past the old\l"
	.string "waterline.\p"
	.string "First time in years I've handed\n"
	.string "someone water and watched them\l"
	.string "drink it slowly.$"

SiroccoMiradenOasis_Text_Scholar:
	.string "The BURIED CITY is thousands of\n"
	.string "years old.\p"
	.string "Whatever civilization built it\n"
	.string "had water in abundance.\p"
	.string "Makes you wonder what they were\n"
	.string "protecting it from.$"

SiroccoMiradenOasis_Text_ScholarLate:
	.string "You've been down there, haven't\n"
	.string "you? Under the sand.\p"
	.string "I won't ask what you saw.\n"
	.string "The old texts say some knowledge\l"
	.string "drinks the knower.\p"
	.string "…But if you ever want to talk,\n"
	.string "I'm always here.$"

SiroccoMiradenOasis_Text_Elder:
	.string "Before the Gilt Claw, before\n"
	.string "DAGAN, before any of this -\p"
	.string "there was a story the old people\n"
	.string "told about the desert.\p"
	.string "That the sand remembers what\n"
	.string "grew here once.\p"
	.string "That something in the deep rock\n"
	.string "is keeping it alive.\p"
	.string "DAGAN found the something.$"

SiroccoMiradenOasis_Text_ElderResolved:
	.string "The story changed, child.\p"
	.string "Now the old people will tell of\n"
	.string "the year the sand remembered,\l"
	.string "and the water came home.\p"
	.string "You're in the story. I hope you\n"
	.string "don't mind.$"

@ -------------------------------------------------------------------
@ Text - north gate and signs
@ -------------------------------------------------------------------
SiroccoMiradenOasis_Text_NorthBlocker:
	.string "North territory is Gilt Claw\n"
	.string "ground. Members and invitations\l"
	.string "only.\p"
	.string "You want an invitation? Impress\n"
	.string "somebody. The boss only hears\l"
	.string "about people worth hearing\l"
	.string "about.$"

SiroccoMiradenOasis_Text_NorthGateHalt:
	.string "Hey! Wrong side of the gate.\p"
	.string "Nobody walks the water route\n"
	.string "without DAGAN's say-so.$"

SiroccoMiradenOasis_Text_Gym1Sign:
	.string "MIRADEN GYM 1 - “THE WAREHOUSE”\p"
	.string "LEADER: SILT\n"
	.string "“The desert keeps its own maps.”$"

SiroccoMiradenOasis_Text_Gym2Sign:
	.string "MIRADEN GYM 2 - “THE DIG”\p"
	.string "LEADER: CRAG\n"
	.string "“What's buried isn't gone.”$"

SiroccoMiradenOasis_Text_CenterSign:
	.string "POKéMON CENTER\p"
	.string "Free water for POKéMON.\n"
	.string "(For POKéMON. We check.)$"

SiroccoMiradenOasis_Text_ShopSign:
	.string "MIRADEN GENERAL SUPPLY\p"
	.string "A GILT CLAW RETAIL PARTNER\n"
	.string "“Fair prices. Smile.”$"
