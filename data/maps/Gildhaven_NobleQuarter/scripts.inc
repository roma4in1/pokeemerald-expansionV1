Gildhaven_NobleQuarter_MapScripts::
	.byte 0

@ ===================================================================
@ Cass noble-quarter appearance (one-time). The coord triggers at
@ (11,8)/(12,8) arm on VAR_TEMP_2 == 0 (transient: armed every entry),
@ so the FLAG_CASS_GILDHAVEN_SEEN guard makes the SCENE fire exactly
@ once. The Cass object's map.json hide flag is FLAG_CASS_GILDHAVEN_SEEN,
@ so once the scene sets that flag she is gone on every later entry.
@ (The harbor glimpse - scene 1 - deliberately does NOT set this flag;
@ it removes its own object via progress instead, so this flag is the
@ clean once-guard for scene 2 alone.)
@ ===================================================================
GildhavenNobleQuarter_EventScript_CassScene::
	lockall
	goto_if_set FLAG_CASS_GILDHAVEN_SEEN, GildhavenNobleQuarter_EventScript_CassSceneDone
	setvar VAR_TEMP_2, 1
	delay 20
	@ Cass emerges from the manor and freezes when she sees the player.
	applymovement LOCALID_GILDHAVEN_CASS_NOBLE, GildhavenNobleQuarter_Movement_CassEmerge
	waitmovement 0
	delay 20
	playse SE_PIN
	applymovement LOCALID_GILDHAVEN_CASS_NOBLE, Common_Movement_ExclamationMark
	waitmovement 0
	delay 20
	setspeaker Pelagios_Speaker_Cass
	msgbox GildhavenNobleQuarter_Text_CassLeave, MSGBOX_DEFAULT
	@ The player tries to ask why.
	setspeaker 0
	msgbox GildhavenNobleQuarter_Text_CassWhyPrompt, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox GildhavenNobleQuarter_Text_CassWhy, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	@ She walks away quickly, back inside the manor.
	applymovement LOCALID_GILDHAVEN_CASS_NOBLE, GildhavenNobleQuarter_Movement_CassLeave
	waitmovement 0
	removeobject LOCALID_GILDHAVEN_CASS_NOBLE
	setflag FLAG_CASS_GILDHAVEN_SEEN
	delay 20
	msgbox GildhavenNobleQuarter_Text_CassGone, MSGBOX_DEFAULT
	releaseall
	end

GildhavenNobleQuarter_EventScript_CassSceneDone::
	releaseall
	end

GildhavenNobleQuarter_Movement_CassEmerge:
	walk_down
	walk_down
	face_down
	step_end

GildhavenNobleQuarter_Movement_CassLeave:
	walk_up
	walk_up
	walk_up
	step_end

@ Defensive fallback talk (the object is removed during the scene).
GildhavenNobleQuarter_EventScript_CassWarning::
	lockall
	releaseall
	end

@ ===================================================================
@ Gym 3 - Lace Vane (Fairy/Dark). The moral midpoint. Garden arena at
@ the manor's edge. Gated on FLAG_GILDHAVEN_GYM2_CLEAR. Victory gives
@ ITEM_VANE_MANOR_KEY + FLAG_GILDHAVEN_MANOR_ACCESS BEFORE the TM (so a
@ full bag cannot strand the player), the Vane Badge (narrative-only),
@ progress 4, and her extended post-battle scene (FLAG_GILDHAVEN_LACE_TALKED).
@ TM: no Play Rough TM exists -> TM Torment (Dark-flavor) lampshaded.
@ ===================================================================
GildhavenNobleQuarter_EventScript_Lace::
	goto_if_defeated TRAINER_LEADER_GILDHAVEN_LACE, GildhavenNobleQuarter_EventScript_LaceDefeated
	goto_if_unset FLAG_GILDHAVEN_GYM2_CLEAR, GildhavenNobleQuarter_EventScript_LaceNotYet
	setspeaker Pelagios_Speaker_Lace
	trainerbattle_single TRAINER_LEADER_GILDHAVEN_LACE, GildhavenNobleQuarter_Text_LaceIntro, GildhavenNobleQuarter_Text_LaceDefeat, GildhavenNobleQuarter_EventScript_LaceVictory
	end

GildhavenNobleQuarter_EventScript_LaceVictory::
	@ Quiet, to herself - the player overhears.
	setspeaker Pelagios_Speaker_Lace
	msgbox GildhavenNobleQuarter_Text_LaceOverheard, MSGBOX_DEFAULT
	@ Then directly to the player - the extended decision.
	msgbox GildhavenNobleQuarter_Text_LacePostBattle1, MSGBOX_DEFAULT
	msgbox GildhavenNobleQuarter_Text_LacePostBattle2, MSGBOX_DEFAULT
	setspeaker 0
	@ Key item + access FIRST (never gated behind a full bag).
	giveitem ITEM_VANE_MANOR_KEY
	setflag FLAG_GILDHAVEN_MANOR_ACCESS
	setflag FLAG_GILDHAVEN_LACE_TALKED
	setspeaker Pelagios_Speaker_Lace
	msgbox GildhavenNobleQuarter_Text_LaceGivesKey, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message GildhavenNobleQuarter_Text_ReceivedVaneBadge
	waitmessage
	waitfanfare
	setflag FLAG_GILDHAVEN_GYM3_CLEAR
	setvar VAR_GILDHAVEN_PROGRESS, 4
	giveitem ITEM_TM_TORMENT
	goto_if_eq VAR_RESULT, FALSE, GildhavenNobleQuarter_EventScript_LaceFinish
	setspeaker Pelagios_Speaker_Lace
	msgbox GildhavenNobleQuarter_Text_LaceTM, MSGBOX_DEFAULT
	setspeaker 0
GildhavenNobleQuarter_EventScript_LaceFinish::
	release
	end

GildhavenNobleQuarter_EventScript_LaceNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lace
	msgbox GildhavenNobleQuarter_Text_LaceNotYet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_LaceDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lace
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenNobleQuarter_EventScript_LaceResolved
	msgbox GildhavenNobleQuarter_Text_LacePostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_LaceResolved::
	msgbox GildhavenNobleQuarter_Text_LaceResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Covenant officers in the noble quarter - presence feels wrong.
@ ===================================================================
GildhavenNobleQuarter_EventScript_CovenantOfficer1::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Officer
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenNobleQuarter_EventScript_CovenantOfficer1Resolved
	msgbox GildhavenNobleQuarter_Text_CovenantOfficer1, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_CovenantOfficer1Resolved::
	msgbox GildhavenNobleQuarter_Text_CovenantOfficer1Resolved, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_CovenantOfficer2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Officer
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenNobleQuarter_EventScript_CovenantOfficer2Resolved
	msgbox GildhavenNobleQuarter_Text_CovenantOfficer2, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_CovenantOfficer2Resolved::
	msgbox GildhavenNobleQuarter_Text_CovenantOfficer2Resolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Wealthy resident - corruption variant at progress 3+.
@ ===================================================================
GildhavenNobleQuarter_EventScript_Resident::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Resident
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenNobleQuarter_EventScript_ResidentResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenNobleQuarter_EventScript_ResidentCorrupt
	msgbox GildhavenNobleQuarter_Text_Resident, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_ResidentCorrupt::
	msgbox GildhavenNobleQuarter_Text_ResidentCorrupt, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenNobleQuarter_EventScript_ResidentResolved::
	msgbox GildhavenNobleQuarter_Text_ResidentResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Signs
@ ===================================================================
GildhavenNobleQuarter_EventScript_ManorSign::
	lockall
	msgbox GildhavenNobleQuarter_Text_ManorSign, MSGBOX_SIGN
	releaseall
	end

GildhavenNobleQuarter_EventScript_GardenGymSign::
	lockall
	msgbox GildhavenNobleQuarter_Text_GardenGymSign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text - Cass
@ ===================================================================
GildhavenNobleQuarter_Text_CassLeave:
	.string "...\p"
	.string "Leave. Please.$"

GildhavenNobleQuarter_Text_CassWhyPrompt:
	.string "You ask her why.$"

GildhavenNobleQuarter_Text_CassWhy:
	.string "Because I am here and you are here.\p"
	.string "And if anyone connects those two\n"
	.string "things -\p"
	.string "...\p"
	.string "Just leave. Please. I cannot explain\n"
	.string "it here. I am not even sure I am\l"
	.string "allowed to explain it anywhere.$"

GildhavenNobleQuarter_Text_CassGone:
	.string "Cass disappears back into the manor\n"
	.string "before you can say a word.\p"
	.string "Whatever she is doing on this island,\n"
	.string "she did not want you to see it.$"

@ ===================================================================
@ Text - Lace (Gym 3)
@ ===================================================================
GildhavenNobleQuarter_Text_LaceNotYet:
	.string "Not yet. Beat SHADE first - prove you\n"
	.string "can survive the parts of this city\l"
	.string "that bite.\p"
	.string "Then come to the garden. I will be\n"
	.string "ready. I think.$"

GildhavenNobleQuarter_Text_LaceIntro:
	.string "My father built his fortune on this\n"
	.string "island.\p"
	.string "He believes in it. In GILDHAVEN. In\n"
	.string "what it represents.\p"
	.string "I used to believe in him.\p"
	.string "I am going to battle you because I\n"
	.string "need to understand something about\l"
	.string "myself.\p"
	.string "Whether I am still protecting him. Or\n"
	.string "letting go.$"

GildhavenNobleQuarter_Text_LaceDefeat:
	.string "...There. Now I know.$"

GildhavenNobleQuarter_Text_LaceOverheard:
	.string "(Quietly, almost to herself -)\p"
	.string "I wonder if he ever actually loved\n"
	.string "this place.\p"
	.string "Or just what it gave him.$"

GildhavenNobleQuarter_Text_LacePostBattle1:
	.string "...Letting go. I think.\p"
	.string "He was a good father when I was\n"
	.string "small. Before the EXCHANGE. Before\l"
	.string "all of this.\p"
	.string "I do not know when it changed. I do\n"
	.string "not think he knows either.$"

GildhavenNobleQuarter_Text_LacePostBattle2:
	.string "Whatever is under that building, it\n"
	.string "took my father a piece at a time, so\l"
	.string "slowly that none of us saw a single\l"
	.string "piece go.\p"
	.string "If you can stop it taking anyone\n"
	.string "else - do it. Please.$"

GildhavenNobleQuarter_Text_LaceGivesKey:
	.string "Here. The manor key.\p"
	.string "The passage to the EXCHANGE is\n"
	.string "through his study. I found it last\l"
	.string "year.\p"
	.string "I never used it.\p"
	.string "I am using it now.$"

GildhavenNobleQuarter_Text_ReceivedVaneBadge:
	.string "{PLAYER} received the VANE BADGE.$"

GildhavenNobleQuarter_Text_LaceTM:
	.string "And take this. TM - TORMENT.\p"
	.string "I know it is not the prettiest move.\n"
	.string "PLAY ROUGH is what people expect from\l"
	.string "my type, but the presses here never\l"
	.string "cut that one.\p"
	.string "TORMENT suits the family better\n"
	.string "anyway. We are very good at not\l"
	.string "letting things rest.$"

GildhavenNobleQuarter_Text_LacePostBadge:
	.string "The study is open to you now. Go down.\p"
	.string "I will wait up here. I do not think I\n"
	.string "can watch what happens next.$"

GildhavenNobleQuarter_Text_LaceResolved:
	.string "The shimmer is gone from the air. I\n"
	.string "can feel the difference standing\l"
	.string "here.\p"
	.string "Thank you. For all of it.\p"
	.string "I am going to start over. Honestly,\n"
	.string "this time. No invisible thumb on the\l"
	.string "scale.$"

@ ===================================================================
@ Text - Covenant officers
@ ===================================================================
GildhavenNobleQuarter_Text_CovenantOfficer1:
	.string "Covenant business. Move along.\p"
	.string "We are here to protect an asset of\n"
	.string "regional importance.\p"
	.string "I am not permitted to tell you which\n"
	.string "asset. You would not believe me if I\l"
	.string "did.$"

GildhavenNobleQuarter_Text_CovenantOfficer1Resolved:
	.string "The asset is... gone. Or it was\n"
	.string "never an asset. The orders contradict\l"
	.string "themselves now.\p"
	.string "We are being recalled. Nobody will\n"
	.string "say where.$"

GildhavenNobleQuarter_Text_CovenantOfficer2:
	.string "You did not see a grey-coated agent\n"
	.string "go into that manor.\p"
	.string "There is no grey-coated agent.\n"
	.string "There is no manor business.\p"
	.string "Enjoy your visit to GILDHAVEN.$"

GildhavenNobleQuarter_Text_CovenantOfficer2Resolved:
	.string "Whatever we were guarding here, the\n"
	.string "value just dropped to nothing\l"
	.string "overnight.\p"
	.string "Strange. The COVENANT does not\n"
	.string "usually misjudge a yield.$"

@ ===================================================================
@ Text - Resident
@ ===================================================================
GildhavenNobleQuarter_Text_Resident:
	.string "The noble quarter. Old money, they\n"
	.string "say. Though nothing here is older\l"
	.string "than the EXCHANGE.\p"
	.string "Everyone wealthy on this island got\n"
	.string "wealthy at the same time. Funny, that.\l"
	.string "Nobody finds it funny.$"

GildhavenNobleQuarter_Text_ResidentCorrupt:
	.string "Do you know what my estate is worth?\p"
	.string "I do. To the credit. I check it three\n"
	.string "times a day. It is never enough.\p"
	.string "It is the most magnificent house on\n"
	.string "the hill and I lie awake wanting the\l"
	.string "next one.$"

GildhavenNobleQuarter_Text_ResidentResolved:
	.string "I walked through my own home this\n"
	.string "morning like a stranger.\p"
	.string "Half these rooms I bought to have, not\n"
	.string "to use. I cannot think why now.\p"
	.string "I opened a window. First time in\n"
	.string "years.$"

@ ===================================================================
@ Text - Signs
@ ===================================================================
GildhavenNobleQuarter_Text_ManorSign:
	.string "VANE MANOR\n"
	.string "Private residence of Merchant Lord\l"
	.string "SEREL VANE.\p"
	.string "The gate is locked. It looks like it\n"
	.string "is always locked.$"

GildhavenNobleQuarter_Text_GardenGymSign:
	.string "A garden arena at the manor's edge.\n"
	.string "Elegant, improvised, recent.\p"
	.string "Someone built a battlefield here in a\n"
	.string "hurry, and meant it.$"
