Primalis_ZoanVillage_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 1 - Fern (Grass). Young part-plant Zoan at the training edge.
@ Talk-initiated (TRAINER_TYPE_NONE). Victory sets
@ FLAG_PRIMALIS_GYM1_CLEAR + progress 2; badge NARRATIVE-ONLY (engine
@ badge flags exhausted at FLAG_BADGE08_GET). TM: no Leaf Storm TM in
@ this project's vanilla set, so Fern gives TM Giga Drain (a draining
@ Grass move) and lampshades the substitution. Key item/flag first.
@ ===================================================================
PrimalisZoanVillage_EventScript_Fern::
	goto_if_defeated TRAINER_LEADER_PRIMALIS_FERN, PrimalisZoanVillage_EventScript_FernDefeated
	setspeaker Pelagios_Speaker_Fern
	trainerbattle_single TRAINER_LEADER_PRIMALIS_FERN, PrimalisZoanVillage_Text_FernIntro, PrimalisZoanVillage_Text_FernDefeat, PrimalisZoanVillage_EventScript_FernVictory
	end

PrimalisZoanVillage_EventScript_FernVictory::
	setspeaker Pelagios_Speaker_Fern
	msgbox PrimalisZoanVillage_Text_FernPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message PrimalisZoanVillage_Text_ReceivedVerdantBadge
	waitmessage
	waitfanfare
	setflag FLAG_PRIMALIS_GYM1_CLEAR
	setvar VAR_PRIMALIS_PROGRESS, 2
	setspeaker Pelagios_Speaker_Fern
	msgbox PrimalisZoanVillage_Text_FernDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_GIGA_DRAIN
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Fern
	msgbox PrimalisZoanVillage_Text_FernTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_FernDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Fern
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisZoanVillage_EventScript_FernResolved
	msgbox PrimalisZoanVillage_Text_FernPostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_FernResolved::
	msgbox PrimalisZoanVillage_Text_FernResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Gym 2 - Scale (Dragon). Mid-rank part-dragon Zoan. Gated on Gym1.
@ Victory sets FLAG_PRIMALIS_GYM2_CLEAR + progress 3 (Mako accessible).
@ TM: no Dragon Dance TM exists; Scale gives TM Bulk Up (a setup move
@ in the same spirit) and lampshades it. Flag first, TM last.
@ ===================================================================
PrimalisZoanVillage_EventScript_Scale::
	goto_if_defeated TRAINER_LEADER_PRIMALIS_SCALE, PrimalisZoanVillage_EventScript_ScaleDefeated
	goto_if_unset FLAG_PRIMALIS_GYM1_CLEAR, PrimalisZoanVillage_EventScript_ScaleLocked
	setspeaker Pelagios_Speaker_Scale
	trainerbattle_single TRAINER_LEADER_PRIMALIS_SCALE, PrimalisZoanVillage_Text_ScaleIntro, PrimalisZoanVillage_Text_ScaleDefeat, PrimalisZoanVillage_EventScript_ScaleVictory
	end

PrimalisZoanVillage_EventScript_ScaleLocked::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Scale
	msgbox PrimalisZoanVillage_Text_ScaleLocked, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_ScaleVictory::
	setspeaker Pelagios_Speaker_Scale
	msgbox PrimalisZoanVillage_Text_ScalePostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message PrimalisZoanVillage_Text_ReceivedScaleBadge
	waitmessage
	waitfanfare
	setflag FLAG_PRIMALIS_GYM2_CLEAR
	setvar VAR_PRIMALIS_PROGRESS, 3
	setspeaker Pelagios_Speaker_Scale
	msgbox PrimalisZoanVillage_Text_ScaleDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Scale
	msgbox PrimalisZoanVillage_Text_ScaleTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_ScaleDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Scale
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisZoanVillage_EventScript_ScaleResolved
	msgbox PrimalisZoanVillage_Text_ScalePostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_ScaleResolved::
	msgbox PrimalisZoanVillage_Text_ScaleResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Zoan Guardian Rael (TRAINER_ZOAN_PRIMALIS_3) - sight trainer.
@ ===================================================================
PrimalisZoanVillage_EventScript_Zoan3::
	trainerbattle_single TRAINER_ZOAN_PRIMALIS_3, PrimalisZoanVillage_Text_RaelIntro, PrimalisZoanVillage_Text_RaelDefeat, PrimalisZoanVillage_EventScript_Zoan3Post
	setspeaker Pelagios_Speaker_Rael
	msgbox PrimalisZoanVillage_Text_RaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_Zoan3Post::
	setspeaker Pelagios_Speaker_Rael
	msgbox PrimalisZoanVillage_Text_RaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Zoan Guardian Cael (TRAINER_ZOAN_PRIMALIS_4) - sight trainer.
@ The last of the four; his defeat earns a grudging welcome.
@ ===================================================================
PrimalisZoanVillage_EventScript_Zoan4::
	trainerbattle_single TRAINER_ZOAN_PRIMALIS_4, PrimalisZoanVillage_Text_CaelIntro, PrimalisZoanVillage_Text_CaelDefeat, PrimalisZoanVillage_EventScript_Zoan4Post
	setspeaker Pelagios_Speaker_Cael
	msgbox PrimalisZoanVillage_Text_CaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_Zoan4Post::
	setspeaker Pelagios_Speaker_Cael
	msgbox PrimalisZoanVillage_Text_CaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Fully-transformed Zoan member. Pre-resolution there is NO dialogue -
@ examine-only narration; she just watches, recognition stuck behind
@ her eyes. POST-RESOLUTION this is the recovery scene - the emotional
@ gut-punch. It needs a PAUSE before AND after the line so it lands.
@ One-time: FLAG_TEMP_2 marks it played this session; it can replay
@ across sessions (harmless - it is a gift, not a gate).
@ ===================================================================
PrimalisZoanVillage_EventScript_Villager::
	lock
	faceplayer
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisZoanVillage_EventScript_VillagerRecovering
	@ Pre-resolution: pure narration, no namebox, she does not speak.
	msgbox PrimalisZoanVillage_Text_TransformedWatch, MSGBOX_DEFAULT
	release
	end

PrimalisZoanVillage_EventScript_VillagerRecovering::
	delay 30
	setspeaker Pelagios_Speaker_Villager
	msgbox PrimalisZoanVillage_Text_RecoveryLine, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 40
	msgbox PrimalisZoanVillage_Text_RecoveryAfter, MSGBOX_DEFAULT
	release
	end

@ ===================================================================
@ Village child.
@ ===================================================================
PrimalisZoanVillage_EventScript_Child::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisZoanVillage_EventScript_ChildResolved
	msgbox PrimalisZoanVillage_Text_Child, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisZoanVillage_EventScript_ChildResolved::
	msgbox PrimalisZoanVillage_Text_ChildResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Signs
@ ===================================================================
PrimalisZoanVillage_EventScript_ElderHallSign::
	lockall
	msgbox PrimalisZoanVillage_Text_ElderHallSign, MSGBOX_DEFAULT
	releaseall
	end

PrimalisZoanVillage_EventScript_CenterSign::
	lockall
	msgbox PrimalisZoanVillage_Text_CenterSign, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - Fern (Gym 1)
@ ===================================================================
PrimalisZoanVillage_Text_FernIntro:
	.string "You want to go deeper. Elder MAKO said\n"
	.string "outsiders are not welcome.\p"
	.string "But he also said to evaluate anyone who\n"
	.string "made it this far.\p"
	.string "I am evaluating.$"

PrimalisZoanVillage_Text_FernDefeat:
	.string "Oh. You are - you are good.$"

PrimalisZoanVillage_Text_FernPostBattle:
	.string "You move like you know what you are\n"
	.string "protecting.\p"
	.string "Elder MAKO will see you. Beat SCALE\n"
	.string "first.$"

PrimalisZoanVillage_Text_ReceivedVerdantBadge:
	.string "{PLAYER} received the VERDANT BADGE.$"

PrimalisZoanVillage_Text_FernDirections:
	.string "SCALE holds the village center. She is\n"
	.string "harder than me. She is supposed to be.$"

PrimalisZoanVillage_Text_FernTM:
	.string "Take this. We do not have a record of\n"
	.string "LEAF STORM the way the mainland does -\l"
	.string "but GIGA DRAIN carries the same idea.\p"
	.string "Take what the forest gives. Give it\n"
	.string "back. That is the whole lesson.$"

PrimalisZoanVillage_Text_FernPostBadge:
	.string "Go on. SCALE is waiting, and she does\n"
	.string "not wait gracefully.$"

PrimalisZoanVillage_Text_FernResolved:
	.string "The plant in me settled the night you\n"
	.string "reinforced the Root.\p"
	.string "I had stopped being sure where I ended\n"
	.string "and the vine began. Now I am sure\l"
	.string "again. Thank you.$"

@ ===================================================================
@ Text - Scale (Gym 2)
@ ===================================================================
PrimalisZoanVillage_Text_ScaleLocked:
	.string "FERN first. That is the order.\p"
	.string "If you cannot get past the youngest of\n"
	.string "us, you have no business standing in\l"
	.string "front of me.$"

PrimalisZoanVillage_Text_ScaleIntro:
	.string "The fully-transformed ones - you saw\n"
	.string "them.\p"
	.string "They were friends. Whatever is happening\n"
	.string "started three months ago. Same time the\l"
	.string "forest felt wrong.\p"
	.string "If you understand why, tell me after.$"

PrimalisZoanVillage_Text_ScaleDefeat:
	.string "Good. That was good.$"

PrimalisZoanVillage_Text_ScalePostBattle:
	.string "Elder MAKO's hall. He will see you now.\p"
	.string "He will not trust you yet. That comes\n"
	.string "after.$"

PrimalisZoanVillage_Text_ReceivedScaleBadge:
	.string "{PLAYER} received the SCALE BADGE.$"

PrimalisZoanVillage_Text_ScaleDirections:
	.string "The hall is north, past the great doors.\p"
	.string "Speak plainly to him. He has spent his\n"
	.string "whole life listening for the lie under\l"
	.string "the words.$"

PrimalisZoanVillage_Text_ScaleTM:
	.string "We have no DRAGON DANCE in our keeping -\n"
	.string "but BULK UP teaches the same patience.\p"
	.string "Build before you strike. Dragons do not\n"
	.string "rush. They wait until rushing is\l"
	.string "unnecessary.$"

PrimalisZoanVillage_Text_ScalePostBadge:
	.string "MAKO is expecting you. Do not keep an\n"
	.string "old shark waiting.$"

PrimalisZoanVillage_Text_ScaleResolved:
	.string "The scales along my arms stopped\n"
	.string "spreading. I can feel my own hands\l"
	.string "again.\p"
	.string "I did not say it before, so I will say\n"
	.string "it now. You understood. After all.$"

@ ===================================================================
@ Text - guardians Rael / Cael
@ ===================================================================
PrimalisZoanVillage_Text_RaelIntro:
	.string "The village is close. The elder does\n"
	.string "not want visitors. I enforce that.$"

PrimalisZoanVillage_Text_RaelDefeat:
	.string "Tch. Fine.$"

PrimalisZoanVillage_Text_RaelPost:
	.string "The elder will decide for himself.$"

PrimalisZoanVillage_Text_CaelIntro:
	.string "Four of us guard the approaches.\p"
	.string "You have beaten three.$"

PrimalisZoanVillage_Text_CaelDefeat:
	.string "And now four.$"

PrimalisZoanVillage_Text_CaelPost:
	.string "Welcome to Primalis.$"

@ ===================================================================
@ Text - transformed member (examine) + recovery scene
@ ===================================================================
PrimalisZoanVillage_Text_TransformedWatch:
	.string "They look at you with recognition.\p"
	.string "Something in their eyes is still there.\n"
	.string "Just stuck.$"

PrimalisZoanVillage_Text_RecoveryLine:
	.string "I...\p"
	.string "...I remember. I remember who I was.\p"
	.string "I am still in here. I am still in here.$"

PrimalisZoanVillage_Text_RecoveryAfter:
	.string "Their hand finds yours. The grip is\n"
	.string "human. Shaking. Theirs.$"

@ ===================================================================
@ Text - child + signs
@ ===================================================================
PrimalisZoanVillage_Text_Child:
	.string "Mama says I am too little to start\n"
	.string "changing yet. She says it like it is\l"
	.string "good news.\p"
	.string "She did not used to say it like that.$"

PrimalisZoanVillage_Text_ChildResolved:
	.string "Mama smiled today. A real one. I am\n"
	.string "going to remember it forever in case\l"
	.string "she forgets how.$"

PrimalisZoanVillage_Text_ElderHallSign:
	.string "ELDER HALL.\p"
	.string "Where the history is kept - not in\n"
	.string "books, but in the keeping of those who\l"
	.string "remember.$"

PrimalisZoanVillage_Text_CenterSign:
	.string "A carved post marks the healing house.\n"
	.string "Even guardians need tending.$"
