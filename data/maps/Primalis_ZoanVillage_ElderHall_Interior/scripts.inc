Primalis_ZoanVillage_ElderHall_Interior_MapScripts::
	.byte 0

@ ===================================================================
@ Elder Mako - the community leader. Part-shark transformation,
@ ancient, deeply suspicious of outsiders. Talk-initiated
@ (TRAINER_TYPE_NONE). His script is a small state machine keyed on
@ VAR_PRIMALIS_PROGRESS and FLAG_PRIMALIS_GYM3_CLEAR:
@   progress < 3        : not yet (shouldn't normally reach him)
@   progress == 3       : FIRST MEETING (suspicious; parent 12 yrs ago;
@                         sends player to Thorn). Sets TRUST_EARNED so
@                         the first-meeting beat plays once.
@   Gym3 clear, not Gym4: GYM 4 BATTLE -> oral-history victory scene.
@   Gym4 clear          : post-history dialogue.
@ ===================================================================
PrimalisElderHallInterior_EventScript_Mako::
	goto_if_set FLAG_PRIMALIS_GYM4_CLEAR, PrimalisElderHallInterior_EventScript_MakoAfter
	goto_if_set FLAG_PRIMALIS_GYM3_CLEAR, PrimalisElderHallInterior_EventScript_MakoGym4
	@ Pre-Gym3: either the first meeting (Gym2 done) or a brush-off.
	goto_if_set FLAG_PRIMALIS_TRUST_EARNED, PrimalisElderHallInterior_EventScript_MakoAwaitThorn
	goto_if_lt VAR_PRIMALIS_PROGRESS, 3, PrimalisElderHallInterior_EventScript_MakoNotYet
	goto PrimalisElderHallInterior_EventScript_MakoFirstMeeting

@ -------------------------------------------------------------------
@ Defensive: somehow reached before Gym2.
@ -------------------------------------------------------------------
PrimalisElderHallInterior_EventScript_MakoNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoNotYet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ -------------------------------------------------------------------
@ FIRST MEETING (post-Gym2, pre-Gym3). The trust mechanic's first
@ beat. He mentions the player's PARENT, who sat here twelve years
@ ago and wept. Sends the player to Thorn (Gym3). Sets
@ FLAG_PRIMALIS_TRUST_EARNED so this scene plays once. (TRUST_EARNED
@ here means "the conversation has begun" - the full trust resolves
@ in the oral history.)
@ -------------------------------------------------------------------
PrimalisElderHallInterior_EventScript_MakoFirstMeeting::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoFirst1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox PrimalisElderHallInterior_Text_MakoFirst2, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox PrimalisElderHallInterior_Text_MakoFirst3, MSGBOX_DEFAULT
	setspeaker 0
	setflag FLAG_PRIMALIS_TRUST_EARNED
	release
	end

PrimalisElderHallInterior_EventScript_MakoAwaitThorn::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoAwaitThorn, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ -------------------------------------------------------------------
@ GYM 4 (post-Gym3). Mako tests the player himself.
@ Dragon/Water shark transformation. Victory -> oral-history scene.
@ -------------------------------------------------------------------
PrimalisElderHallInterior_EventScript_MakoGym4::
	setspeaker Pelagios_Speaker_Mako
	trainerbattle_single TRAINER_LEADER_PRIMALIS_MAKO, PrimalisElderHallInterior_Text_MakoIntro, PrimalisElderHallInterior_Text_MakoDefeat, PrimalisElderHallInterior_EventScript_MakoVictory
	end

@ ===================================================================
@ ORAL HISTORY SCENE - the narrative pivot of the whole game. Mako
@ reveals the kill switch EXISTS and Haven Isle is the central node.
@ He gives ITEM_BEAST_WHISTLE and ITEM_PRIMALIS_TOKEN (both BEFORE
@ any TM, full-bag-safe via checkitemspace). The parent-weeping
@ detail lands here. Sets FLAG_PRIMALIS_GYM4_CLEAR,
@ FLAG_PRIMALIS_ORAL_HISTORY_HEARD, progress 5. (TRUST_EARNED was set
@ at the first meeting.) Paced with deliberate delays.
@ ===================================================================
PrimalisElderHallInterior_EventScript_MakoVictory::
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	@ Badge - NARRATIVE-ONLY (engine badge flags exhausted at 08).
	playfanfare MUS_OBTAIN_BADGE
	message PrimalisElderHallInterior_Text_ReceivedElderBadge
	waitmessage
	waitfanfare
	setflag FLAG_PRIMALIS_GYM4_CLEAR
	closemessage
	delay 30
	@ --- the history ---
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_History1, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox PrimalisElderHallInterior_Text_History2, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox PrimalisElderHallInterior_Text_History3, MSGBOX_DEFAULT
	closemessage
	delay 40
	@ --- the parent ---
	msgbox PrimalisElderHallInterior_Text_HistoryParent, MSGBOX_DEFAULT
	closemessage
	delay 40
	msgbox PrimalisElderHallInterior_Text_HistoryQuestion, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 30
	@ --- the gifts: key items FIRST, full-bag-safe ---
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoGiveWhistle, MSGBOX_DEFAULT
	setspeaker 0
	checkitemspace ITEM_BEAST_WHISTLE, 1
	call_if_eq VAR_RESULT, TRUE, PrimalisElderHallInterior_EventScript_GiveWhistle
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoGiveToken, MSGBOX_DEFAULT
	setspeaker 0
	checkitemspace ITEM_PRIMALIS_TOKEN, 1
	call_if_eq VAR_RESULT, TRUE, PrimalisElderHallInterior_EventScript_GiveToken
	@ Set the story flags AFTER the key items so a full bag can never
	@ skip them silently and still mark the history heard.
	setflag FLAG_PRIMALIS_ORAL_HISTORY_HEARD
	setvar VAR_PRIMALIS_PROGRESS, 5
	@ --- the parting charge ---
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoCharge, MSGBOX_DEFAULT
	setspeaker 0
	@ TM last (Outrage -> Dragon Claw; no Outrage TM exists).
	giveitem ITEM_TM_DRAGON_CLAW
	goto_if_eq VAR_RESULT, FALSE, PrimalisElderHallInterior_EventScript_MakoTMFull
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisElderHallInterior_Text_MakoTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHallInterior_EventScript_GiveWhistle::
	additem ITEM_BEAST_WHISTLE, 1
	setflag FLAG_BEAST_WHISTLE_OBTAINED
	playfanfare MUS_OBTAIN_ITEM
	message PrimalisElderHallInterior_Text_ReceivedWhistle
	waitmessage
	waitfanfare
	closemessage
	return

PrimalisElderHallInterior_EventScript_GiveToken::
	additem ITEM_PRIMALIS_TOKEN, 1
	setflag FLAG_PRIMALIS_TOKEN_GIVEN
	playfanfare MUS_OBTAIN_ITEM
	message PrimalisElderHallInterior_Text_ReceivedToken
	waitmessage
	waitfanfare
	closemessage
	return

PrimalisElderHallInterior_EventScript_MakoTMFull::
	@ TM forfeited if the bag is full - the scene must still complete.
	release
	end

@ -------------------------------------------------------------------
@ After Gym4 / oral history. Mako stays in his hall; he points the
@ player toward the Heartwood.
@ -------------------------------------------------------------------
PrimalisElderHallInterior_EventScript_MakoAfter::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Mako
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisElderHallInterior_EventScript_MakoResolved
	msgbox PrimalisElderHallInterior_Text_MakoAfter, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHallInterior_EventScript_MakoResolved::
	msgbox PrimalisElderHallInterior_Text_MakoResolvedHall, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Elder Two - the second elder in the tableau, facing Mako. Holds
@ the keeping with him; speaks rarely.
@ ===================================================================
PrimalisElderHallInterior_EventScript_Elder2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Elder
	goto_if_set FLAG_PRIMALIS_ORAL_HISTORY_HEARD, PrimalisElderHallInterior_EventScript_Elder2PostHistory
	msgbox PrimalisElderHallInterior_Text_Elder2, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHallInterior_EventScript_Elder2PostHistory::
	msgbox PrimalisElderHallInterior_Text_Elder2PostHistory, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Text - Mako pre-Gym4
@ ===================================================================
PrimalisElderHallInterior_Text_MakoNotYet:
	.string "You smell of saltwater and somewhere\n"
	.string "else.\p"
	.string "You have not earned this hall. Go.\n"
	.string "Prove yourself in the village first.$"

PrimalisElderHallInterior_Text_MakoFirst1:
	.string "So. The outside sends another.\p"
	.string "Sit. No - stand. You will not be here\n"
	.string "long enough to sit.$"

PrimalisElderHallInterior_Text_MakoFirst2:
	.string "Your parent sat in this hall twelve\n"
	.string "years ago.\p"
	.string "I told them what I know. They wept.\n"
	.string "Then left without explaining.\p"
	.string "I have wondered since what they did\n"
	.string "with it.$"

PrimalisElderHallInterior_Text_MakoFirst3:
	.string "You have their eyes. That earns you a\n"
	.string "second look. Nothing more.\p"
	.string "Prove yourself to THORN, on the route\n"
	.string "east. Then we talk.$"

PrimalisElderHallInterior_Text_MakoAwaitThorn:
	.string "THORN, child. The route east. I will\n"
	.string "not say it a third time.$"

@ ===================================================================
@ Text - Mako Gym4 battle
@ ===================================================================
PrimalisElderHallInterior_Text_MakoIntro:
	.string "Our oral history says a guardian will\n"
	.string "come from outside.\p"
	.string "They will not understand at first. But\n"
	.string "they will learn.\p"
	.string "I need to know if you are that\n"
	.string "guardian - or just someone who fights\l"
	.string "well.$"

PrimalisElderHallInterior_Text_MakoDefeat:
	.string "...\p"
	.string "Ah. There it is. The thing in you that\n"
	.string "does not quit when quitting would be\l"
	.string "wiser.$"

@ ===================================================================
@ Text - ORAL HISTORY (the pivot)
@ ===================================================================
PrimalisElderHallInterior_Text_MakoPostBattle:
	.string "You fight to protect, not to win. Few\n"
	.string "do. Fewer still know the difference.\p"
	.string "I will tell you what I told your parent.\n"
	.string "Then we will see if you weep too.$"

PrimalisElderHallInterior_Text_ReceivedElderBadge:
	.string "{PLAYER} received the ELDER BADGE.$"

PrimalisElderHallInterior_Text_History1:
	.string "The Root is older than the people who\n"
	.string "sealed it. Older than the jungle, which\l"
	.string "grew from it. Older than memory, which\l"
	.string "is why we keep it out loud.\p"
	.string "The ancient ones did not build the\n"
	.string "seals to imprison the legendaries. They\l"
	.string "built them to PROTECT them - from\l"
	.string "people like the ones who came later.$"

PrimalisElderHallInterior_Text_History2:
	.string "But they were not fools. They knew a\n"
	.string "seal can be turned into a leash. So\l"
	.string "they built one last thing.\p"
	.string "A kill switch.\p"
	.string "If the seals were ever bent toward\n"
	.string "weaponizing what they held - the whole\l"
	.string "network could be ended at once. The\l"
	.string "legendaries destroyed, rather than\l"
	.string "enslaved.$"

PrimalisElderHallInterior_Text_History3:
	.string "There is one place the switch can be\n"
	.string "thrown. One central node, where every\l"
	.string "seal in Pelagios threads together.\p"
	.string "Haven Isle.\p"
	.string "Your home, child. The caretakers of\n"
	.string "that node have a name. The Wardens.\l"
	.string "Your bloodline. Your inheritance.$"

PrimalisElderHallInterior_Text_HistoryParent:
	.string "Your parent learned this here, in this\n"
	.string "hall, from my own mouth.\p"
	.string "And they wept - not from fear. From\n"
	.string "the weight of it. The knowing that a\l"
	.string "door to the end of everything sat\l"
	.string "under their own house.$"

PrimalisElderHallInterior_Text_HistoryQuestion:
	.string "So I ask you what I never got to ask\n"
	.string "them.\p"
	.string "What did your parent DO with this\n"
	.string "knowledge?$"

PrimalisElderHallInterior_Text_MakoGiveWhistle:
	.string "You cannot answer. I see that. Neither\n"
	.string "could I, in your place.\p"
	.string "Then take what a guardian carries. This\n"
	.string "is a BEAST WHISTLE. The forest will\l"
	.string "open for the one who holds it.$"

PrimalisElderHallInterior_Text_ReceivedWhistle:
	.string "{PLAYER} received the BEAST WHISTLE.$"

PrimalisElderHallInterior_Text_MakoGiveToken:
	.string "And this. My mark, carved in old bone.\p"
	.string "It tells anyone who knows the Zoan that\n"
	.string "you are trusted by us. You will need\l"
	.string "that proof. Where you are going, trust\l"
	.string "is the only currency that holds.$"

PrimalisElderHallInterior_Text_ReceivedToken:
	.string "{PLAYER} received the PRIMAL TOKEN.$"

PrimalisElderHallInterior_Text_MakoCharge:
	.string "Find out what your parent did. Before\n"
	.string "someone else uses the switch.\p"
	.string "Reinforce the Root in the Heartwood,\n"
	.string "past the ruins camp. Then go home,\l"
	.string "child. The answer waits there. It\l"
	.string "always did.$"

PrimalisElderHallInterior_Text_MakoTM:
	.string "One more thing. We have no record of\n"
	.string "OUTRAGE in our keeping - but DRAGON\l"
	.string "CLAW carries the same blood.\p"
	.string "Strike clean. Rage is a poor guardian.$"

PrimalisElderHallInterior_Text_MakoAfter:
	.string "The Heartwood is past the ruins camp,\n"
	.string "north of the route east.\p"
	.string "Go. The Root has waited long enough for\n"
	.string "hands that mean it no harm.$"

PrimalisElderHallInterior_Text_MakoResolvedHall:
	.string "I have told the history twice in my\n"
	.string "life and watched it change nothing.\p"
	.string "You heard it and went and DID\n"
	.string "something. That is the difference\l"
	.string "between keeping a thing and being it.\p"
	.string "Go home, Warden's child. Your house has\n"
	.string "answers in it.$"

@ ===================================================================
@ Text - Elder Two
@ ===================================================================
PrimalisElderHallInterior_Text_Elder2:
	.string "I have kept this history beside MAKO for\n"
	.string "forty years.\p"
	.string "There are nights I wish I did not know\n"
	.string "it. Then I remember - someone must. Or\l"
	.string "it is lost, and being lost is how a\l"
	.string "thing gets used.$"

PrimalisElderHallInterior_Text_Elder2PostHistory:
	.string "He told you all of it. I watched his\n"
	.string "face. He has not told it like that\l"
	.string "since your parent.\p"
	.string "Carry it carefully. It is heavier than\n"
	.string "it sounds, and it sounds very heavy.$"
