Emberveil_CinderholdCity_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 1 - Cinder (Fire). Talk-initiated leader at the converted
@ gatehouse. The doubter - questioning within the cult, not leaving it.
@ Victory: Ember Badge (narrative badge - no vanilla FLAG_BADGEXX
@ slots remain; gym clears are tracked by FLAG_EMBERVEIL_GYM1_CLEAR),
@ VAR_EMBERVEIL_PROGRESS = 2, TM Flamethrower (as briefed).
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_Cinder::
	goto_if_defeated TRAINER_LEADER_EMBERVEIL_CINDER, EmberveilCinderholdCity_EventScript_CinderDefeated
	setspeaker Pelagios_Speaker_Cinder
	trainerbattle_single TRAINER_LEADER_EMBERVEIL_CINDER, EmberveilCinderholdCity_Text_CinderIntro, EmberveilCinderholdCity_Text_CinderDefeat, EmberveilCinderholdCity_EventScript_CinderVictory
	end

EmberveilCinderholdCity_EventScript_CinderVictory::
	setspeaker Pelagios_Speaker_Cinder
	msgbox EmberveilCinderholdCity_Text_CinderPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message EmberveilCinderholdCity_Text_ReceivedEmberBadge
	waitmessage
	waitfanfare
	setflag FLAG_EMBERVEIL_GYM1_CLEAR
	setvar VAR_EMBERVEIL_PROGRESS, 2
	giveitem ITEM_TM_FLAMETHROWER
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Cinder
	msgbox EmberveilCinderholdCity_Text_CinderExplainTM, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_CinderDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Cinder
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_CinderResolved
	msgbox EmberveilCinderholdCity_Text_CinderPostBadge, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_CinderResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilCinderholdCity_EventScript_CinderPathB
	msgbox EmberveilCinderholdCity_Text_CinderPathA, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_CinderPathB::
	msgbox EmberveilCinderholdCity_Text_CinderPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Gym 2 - Slag (Fire/Ground). Talk-initiated leader at the lava works.
@ Not cult, never was. Gated on FLAG_EMBERVEIL_GYM1_CLEAR.
@ Victory order (key item BEFORE the TM so a full bag can never skip
@ it): Forge Badge text, FLAG_EMBERVEIL_GYM2_CLEAR, ITEM_LAVA_BOOTS +
@ FLAG_LAVA_BOOTS_OBTAINED, VAR_EMBERVEIL_PROGRESS = 3 (disarms the
@ city's east gate and both lava-gate trigger rows), then TM
@ Earthquake (brief asks for Earth Power - no such TM in the vanilla
@ set; the substitution is lampshaded).
@ Clearing Gym 2 also hides Solace's TempleHall object (her hide flag
@ is FLAG_EMBERVEIL_GYM2_CLEAR) - she moves to the summit.
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_Slag::
	goto_if_defeated TRAINER_LEADER_EMBERVEIL_SLAG, EmberveilCinderholdCity_EventScript_SlagDefeated
	goto_if_unset FLAG_EMBERVEIL_GYM1_CLEAR, EmberveilCinderholdCity_EventScript_SlagNotYet
	setspeaker Pelagios_Speaker_Slag
	trainerbattle_single TRAINER_LEADER_EMBERVEIL_SLAG, EmberveilCinderholdCity_Text_SlagIntro, EmberveilCinderholdCity_Text_SlagDefeat, EmberveilCinderholdCity_EventScript_SlagVictory
	end

EmberveilCinderholdCity_EventScript_SlagNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Slag
	msgbox EmberveilCinderholdCity_Text_SlagNotYet, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_SlagVictory::
	@ Narration first - the battle's field reload cleared the speaker.
	msgbox EmberveilCinderholdCity_Text_SlagPostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message EmberveilCinderholdCity_Text_ReceivedForgeBadge
	waitmessage
	waitfanfare
	setflag FLAG_EMBERVEIL_GYM2_CLEAR
	setspeaker Pelagios_Speaker_Slag
	msgbox EmberveilCinderholdCity_Text_SlagBoots, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_LAVA_BOOTS
	setflag FLAG_LAVA_BOOTS_OBTAINED
	setvar VAR_EMBERVEIL_PROGRESS, 3
	setspeaker Pelagios_Speaker_Slag
	msgbox EmberveilCinderholdCity_Text_SlagGoodPeople, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_EARTHQUAKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Slag
	msgbox EmberveilCinderholdCity_Text_SlagExplainTM, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_SlagDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Slag
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_SlagResolved
	msgbox EmberveilCinderholdCity_Text_SlagPostBadge, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_SlagResolved::
	msgbox EmberveilCinderholdCity_Text_SlagResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Elder cult historian - the founding, 200 years ago
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_Historian::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Historian
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_HistorianResolved
	msgbox EmberveilCinderholdCity_Text_Historian, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_HistorianResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilCinderholdCity_EventScript_HistorianPathB
	msgbox EmberveilCinderholdCity_Text_HistorianPathA, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_HistorianPathB::
	msgbox EmberveilCinderholdCity_Text_HistorianPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Quietly doubting cult member
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_DoubtingNpc::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Devotee
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_DoubtingResolved
	msgbox EmberveilCinderholdCity_Text_Doubting, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_DoubtingResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilCinderholdCity_EventScript_DoubtingPathB
	msgbox EmberveilCinderholdCity_Text_DoubtingPathA, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_DoubtingPathB::
	msgbox EmberveilCinderholdCity_Text_DoubtingPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Child watching the lanterns
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_Child::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_ChildResolved
	msgbox EmberveilCinderholdCity_Text_Child, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_ChildResolved::
	msgbox EmberveilCinderholdCity_Text_ChildResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Devout cult member (wanderer)
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_CultMember::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Devotee
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilCinderholdCity_EventScript_CultMemberResolved
	msgbox EmberveilCinderholdCity_Text_CultMember, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_CultMemberResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilCinderholdCity_EventScript_CultMemberPathB
	msgbox EmberveilCinderholdCity_Text_CultMemberPathA, MSGBOX_DEFAULT
	release
	end

EmberveilCinderholdCity_EventScript_CultMemberPathB::
	msgbox EmberveilCinderholdCity_Text_CultMemberPathB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ East gate to the caldera road - barred until Gym 2 (progress 0-2).
@ Coord triggers bounce the player back west.
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_CalderaGate::
	lockall
	playse SE_WALL_HIT
	waitse
	msgbox EmberveilCinderholdCity_Text_CalderaGate, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilCinderholdCity_Movement_StepBackWest
	waitmovement 0
	releaseall
	end

EmberveilCinderholdCity_Movement_StepBackWest:
	walk_left
	step_end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
EmberveilCinderholdCity_EventScript_GymSign::
	msgbox EmberveilCinderholdCity_Text_GymSign, MSGBOX_SIGN
	end

EmberveilCinderholdCity_EventScript_TempleSign::
	msgbox EmberveilCinderholdCity_Text_TempleSign, MSGBOX_SIGN
	end

EmberveilCinderholdCity_EventScript_CenterSign::
	msgbox EmberveilCinderholdCity_Text_CenterSign, MSGBOX_SIGN
	end

EmberveilCinderholdCity_EventScript_LavaWorksSign::
	msgbox EmberveilCinderholdCity_Text_LavaWorksSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text - Cinder
@ -------------------------------------------------------------------
EmberveilCinderholdCity_Text_CinderIntro:
	.string "I became an initiate because the\n"
	.string "fire felt true.\p"
	.string "PYRATH's warmth, the HIGH EMBER's\n"
	.string "teachings - I believed.\p"
	.string "I still believe. I just - have\n"
	.string "questions lately.\p"
	.string "The volcano's been louder. The\n"
	.string "HIGH EMBER says that's a\l"
	.string "blessing.\p"
	.string "It doesn't feel like a blessing.\p"
	.string "Beat me. I need to think.$"

EmberveilCinderholdCity_Text_CinderDefeat:
	.string "…Good. Now I can think.$"

EmberveilCinderholdCity_Text_CinderPostBattle:
	.string "When you find the HIGH EMBER -\n"
	.string "and you will - ask her what\l"
	.string "happens to the island if PYRATH\l"
	.string "wakes.\p"
	.string "Actually ask her. See what she\n"
	.string "says.$"

EmberveilCinderholdCity_Text_ReceivedEmberBadge:
	.string "{PLAYER} received the EMBER BADGE\n"
	.string "from CINDER!$"

EmberveilCinderholdCity_Text_CinderExplainTM:
	.string "FLAMETHROWER. The first move the\n"
	.string "fire ever taught me.\p"
	.string "It still feels true. That part\n"
	.string "never stopped feeling true.$"

EmberveilCinderholdCity_Text_CinderPostBadge:
	.string "SLAG runs the lava works on the\n"
	.string "east side. He'll battle anyone\l"
	.string "who's been through me first.\p"
	.string "He's not one of us. Sometimes I\n"
	.string "think that's why he sees things\l"
	.string "we don't.$"

EmberveilCinderholdCity_Text_CinderPathA:
	.string "She's still here. She hasn't left\n"
	.string "the summit.\p"
	.string "I don't know what to say to her.\p"
	.string "I had questions, and now the\n"
	.string "questions don't matter, because\l"
	.string "the thing I was questioning is\l"
	.string "gone.$"

EmberveilCinderholdCity_Text_CinderPathB:
	.string "She told me this morning. About\n"
	.string "the seal. About what the ritual\l"
	.string "actually does.\p"
	.string "I cried. I'm not ashamed of that.\p"
	.string "We've been keeping something\n"
	.string "alive for two centuries without\l"
	.string "knowing it.\p"
	.string "That's not less sacred.\n"
	.string "That's more.$"

@ -------------------------------------------------------------------
@ Text - Slag
@ -------------------------------------------------------------------
EmberveilCinderholdCity_Text_SlagNotYet:
	.string "Gatehouse first. The kid with\n"
	.string "the questions.\p"
	.string "Not a cult thing - a courtesy\n"
	.string "thing. He needs the challengers\l"
	.string "more than I do right now.$"

EmberveilCinderholdCity_Text_SlagIntro:
	.string "I'm not in the cult. Never was.\p"
	.string "I just work the lava fields -\n"
	.string "best fuel source on any island\l"
	.string "I've been to.\p"
	.string "I'll battle you because you look\n"
	.string "like you need the practice for\l"
	.string "whatever's up that mountain.\p"
	.string "Don't take it personally.$"

EmberveilCinderholdCity_Text_SlagDefeat:
	.string "Good. You might survive the\n"
	.string "caldera after all.$"

EmberveilCinderholdCity_Text_SlagPostBattle:
	.string "He wipes the ash off his hands\n"
	.string "and looks you over like a load\l"
	.string "of ore that came in better than\l"
	.string "expected.$"

EmberveilCinderholdCity_Text_ReceivedForgeBadge:
	.string "{PLAYER} received the FORGE BADGE\n"
	.string "from SLAG!$"

EmberveilCinderholdCity_Text_SlagBoots:
	.string "Here. LAVA BOOTS. Military\n"
	.string "salvage, came through the port\l"
	.string "last month.\p"
	.string "You'll need them on the caldera.$"

EmberveilCinderholdCity_Text_SlagGoodPeople:
	.string "And - the HIGH EMBER is a good\n"
	.string "person.\p"
	.string "Good people can be wrong about\n"
	.string "things that matter.$"

EmberveilCinderholdCity_Text_SlagExplainTM:
	.string "Take EARTHQUAKE too. Wanted to\n"
	.string "give you EARTH POWER, but nobody\l"
	.string "presses that one as a TM.\p"
	.string "The ground doesn't care which\n"
	.string "name you shake it by.$"

EmberveilCinderholdCity_Text_SlagPostBadge:
	.string "East gate's open to you now.\p"
	.string "Keep the boots tight. The crust\n"
	.string "out there lies about how solid\l"
	.string "it is.$"

EmberveilCinderholdCity_Text_SlagResolved:
	.string "Mountain's quiet. Fuel yield's\n"
	.string "down nine percent.\p"
	.string "Worth it.\p"
	.string "…Told you good people can be\n"
	.string "wrong. Glad somebody finally\l"
	.string "said it to her face, one way or\l"
	.string "another.$"

@ -------------------------------------------------------------------
@ Text - city NPCs
@ -------------------------------------------------------------------
EmberveilCinderholdCity_Text_Historian:
	.string "The EMBER CIRCLE was founded two\n"
	.string "hundred years ago by the\l"
	.string "survivors of a smaller eruption.\p"
	.string "They believed PYRATH had spared\n"
	.string "them for a purpose - to prepare\l"
	.string "for PYRATH's full awakening.\p"
	.string "Every HIGH EMBER since has\n"
	.string "refined the ritual.\p"
	.string "SOLACE has brought it further\n"
	.string "than anyone.$"

EmberveilCinderholdCity_Text_HistorianPathA:
	.string "Two hundred years of records in\n"
	.string "my keeping.\p"
	.string "I keep looking for the page\n"
	.string "where we went wrong, and the\l"
	.string "terrible thing is - there isn't\l"
	.string "one. Every page made sense at\l"
	.string "the time.\p"
	.string "That's how it happens, I think.\n"
	.string "Page by sensible page.$"

EmberveilCinderholdCity_Text_HistorianPathB:
	.string "Two hundred years of records,\n"
	.string "and every page just changed its\l"
	.string "meaning overnight.\p"
	.string "Not one word of them wasted.\n"
	.string "We were keepers all along.\p"
	.string "The boy from the gatehouse came\n"
	.string "asking for the archives this\l"
	.string "morning. The one with the\l"
	.string "questions.\p"
	.string "Good. The story needs a keeper\n"
	.string "who asked.$"

EmberveilCinderholdCity_Text_Doubting:
	.string "I believe in PYRATH. I'm just\n"
	.string "not sure about the timing.\p"
	.string "The HIGH EMBER says the awakening\n"
	.string "is imminent.\p"
	.string "Imminent has meant different\n"
	.string "things over the years.\p"
	.string "This time it feels - different.$"

EmberveilCinderholdCity_Text_DoubtingPathA:
	.string "I'm leaving.\p"
	.string "Not because I stopped believing.\p"
	.string "Because I don't know what I\n"
	.string "believe anymore, and I need\l"
	.string "somewhere quiet to find out.$"

EmberveilCinderholdCity_Text_DoubtingPathB:
	.string "I was right to wonder about the\n"
	.string "timing. Wrong about everything\l"
	.string "else.\p"
	.string "We weren't waiting for PYRATH\n"
	.string "to wake. PYRATH was waiting for\l"
	.string "us to understand.\p"
	.string "I can live inside that kind of\n"
	.string "wrong.$"

EmberveilCinderholdCity_Text_Child:
	.string "The lanterns never go out! Not\n"
	.string "even in the rain!\p"
	.string "Papa says the mountain lights\n"
	.string "them. I've never seen it do it.\l"
	.string "I've been watching FOREVER.$"

EmberveilCinderholdCity_Text_ChildResolved:
	.string "The mountain got quieter. Did\n"
	.string "you notice?\p"
	.string "It used to grumble at bedtime.\n"
	.string "Now it just breathes.\p"
	.string "I sleep better. Don't tell\n"
	.string "anyone - I'm supposed to be\l"
	.string "brave.$"

EmberveilCinderholdCity_Text_CultMember:
	.string "Visitors always ask if we're\n"
	.string "afraid, living on a volcano.\p"
	.string "Afraid! It's the warmest place\n"
	.string "in PELAGIOS. The fire lights our\l"
	.string "streets and cooks our food.\p"
	.string "PYRATH provides. It always has.$"

EmberveilCinderholdCity_Text_CultMemberPathA:
	.string "Half the temple benches were\n"
	.string "empty this morning.\p"
	.string "I still went. I'll keep going.\p"
	.string "I just don't know what I'm\n"
	.string "sitting there waiting for\l"
	.string "anymore.$"

EmberveilCinderholdCity_Text_CultMemberPathB:
	.string "The temple was FULL this morning.\n"
	.string "Fuller than festival days.\p"
	.string "Turns out nobody leaves a faith\n"
	.string "that turns out to be true.\p"
	.string "Just - true differently.$"

EmberveilCinderholdCity_Text_CalderaGate:
	.string "The iron gate to the caldera\n"
	.string "road is barred.\p"
	.string "A devotee shakes her head from\n"
	.string "the gatepost:\p"
	.string "“The east road needs SLAG's\n"
	.string "badge and SLAG's boots. The\l"
	.string "crust out there eats sandals.”$"

EmberveilCinderholdCity_Text_GymSign:
	.string "EMBERVEIL TRIALS\p"
	.string "1: CINDER - the gatehouse\n"
	.string "2: SLAG - the lava works\l"
	.string "3: VEX - the caldera road\l"
	.string "4: the HIGH EMBER - the summit\p"
	.string "“Climb with respect.”$"

EmberveilCinderholdCity_Text_TempleSign:
	.string "TEMPLE HALL of the EMBER CIRCLE\p"
	.string "“The flame has burned for two\n"
	.string "centuries. All are welcome to\l"
	.string "sit with it.”$"

EmberveilCinderholdCity_Text_CenterSign:
	.string "CINDERHOLD CITY POKéMON CENTER\p"
	.string "Heat-stressed POKéMON treated\n"
	.string "free of charge.$"

EmberveilCinderholdCity_Text_LavaWorksSign:
	.string "CINDERHOLD LAVA WORKS\n"
	.string "SLAG, foreman.\p"
	.string "“Fuel for five islands. Mind\n"
	.string "the channels. No, really.”$"
