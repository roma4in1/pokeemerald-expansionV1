Gildhaven_MerchantDistrict_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 1 - Glint (Fairy). Street-level trader running a converted
@ market stall. Talk-initiated (TRAINER_TYPE_NONE). Victory sets
@ FLAG_GILDHAVEN_GYM1_CLEAR + progress 2; badge is NARRATIVE-ONLY
@ (engine badge flags exhausted at FLAG_BADGE08_GET). TM: there is no
@ Dazzling Gleam TM in this project's vanilla set, so Glint gives
@ TM Light Screen (a shimmering Fairy-flavor barrier) and lampshades
@ the substitution.
@ ===================================================================
GildhavenMerchantDistrict_EventScript_Glint::
	goto_if_defeated TRAINER_LEADER_GILDHAVEN_GLINT, GildhavenMerchantDistrict_EventScript_GlintDefeated
	setspeaker Pelagios_Speaker_Glint
	trainerbattle_single TRAINER_LEADER_GILDHAVEN_GLINT, GildhavenMerchantDistrict_Text_GlintIntro, GildhavenMerchantDistrict_Text_GlintDefeat, GildhavenMerchantDistrict_EventScript_GlintVictory
	end

GildhavenMerchantDistrict_EventScript_GlintVictory::
	setspeaker Pelagios_Speaker_Glint
	msgbox GildhavenMerchantDistrict_Text_GlintPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message GildhavenMerchantDistrict_Text_ReceivedGiltBadge
	waitmessage
	waitfanfare
	setflag FLAG_GILDHAVEN_GYM1_CLEAR
	setvar VAR_GILDHAVEN_PROGRESS, 2
	setspeaker Pelagios_Speaker_Glint
	msgbox GildhavenMerchantDistrict_Text_GlintDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_LIGHT_SCREEN
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Glint
	msgbox GildhavenMerchantDistrict_Text_GlintTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_GlintDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Glint
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenMerchantDistrict_EventScript_GlintResolved
	msgbox GildhavenMerchantDistrict_Text_GlintPostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_GlintResolved::
	msgbox GildhavenMerchantDistrict_Text_GlintResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Gym 2 - Shade (Dark). Black-market enforcer. Hidden alley gym.
@ Gated on FLAG_GILDHAVEN_GYM1_CLEAR. Victory sets
@ FLAG_GILDHAVEN_GYM2_CLEAR + progress 3 (the corruption variants
@ kick in at progress 3+). TM: no Crunch TM exists, so Shade gives
@ TM Thief (Dark-type, same predatory spirit) and lampshades it.
@ ===================================================================
GildhavenMerchantDistrict_EventScript_Shade::
	goto_if_defeated TRAINER_LEADER_GILDHAVEN_SHADE, GildhavenMerchantDistrict_EventScript_ShadeDefeated
	goto_if_unset FLAG_GILDHAVEN_GYM1_CLEAR, GildhavenMerchantDistrict_EventScript_ShadeNotYet
	setspeaker Pelagios_Speaker_Shade
	trainerbattle_single TRAINER_LEADER_GILDHAVEN_SHADE, GildhavenMerchantDistrict_Text_ShadeIntro, GildhavenMerchantDistrict_Text_ShadeDefeat, GildhavenMerchantDistrict_EventScript_ShadeVictory
	end

GildhavenMerchantDistrict_EventScript_ShadeVictory::
	setspeaker Pelagios_Speaker_Shade
	msgbox GildhavenMerchantDistrict_Text_ShadePostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message GildhavenMerchantDistrict_Text_ReceivedShadowBadge
	waitmessage
	waitfanfare
	setflag FLAG_GILDHAVEN_GYM2_CLEAR
	setvar VAR_GILDHAVEN_PROGRESS, 3
	setspeaker Pelagios_Speaker_Shade
	msgbox GildhavenMerchantDistrict_Text_ShadeDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_THIEF
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Shade
	msgbox GildhavenMerchantDistrict_Text_ShadeTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_ShadeNotYet::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Shade
	msgbox GildhavenMerchantDistrict_Text_ShadeNotYet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_ShadeDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Shade
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenMerchantDistrict_EventScript_ShadeResolved
	msgbox GildhavenMerchantDistrict_Text_ShadePostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_ShadeResolved::
	msgbox GildhavenMerchantDistrict_Text_ShadeResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Dagan in a cafe - waves cheerfully, won't engage until after Gym 1
@ (the real meeting is in the BlackMarket). Sprite RICH_BOY.
@ ===================================================================
GildhavenMerchantDistrict_EventScript_DaganCafe::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dagan
	goto_if_set FLAG_GILDHAVEN_DAGAN_MET, GildhavenMerchantDistrict_EventScript_DaganCafeMet
	goto_if_set FLAG_GILDHAVEN_GYM1_CLEAR, GildhavenMerchantDistrict_EventScript_DaganCafePostGym1
	msgbox GildhavenMerchantDistrict_Text_DaganCafePreGym1, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_DaganCafePostGym1::
	msgbox GildhavenMerchantDistrict_Text_DaganCafePostGym1, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_DaganCafeMet::
	msgbox GildhavenMerchantDistrict_Text_DaganCafeMet, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Lace, watching the manor from the district edge. Won't speak until
@ after Gym 2 (her real scene + gym are in the NobleQuarter).
@ ===================================================================
GildhavenMerchantDistrict_EventScript_LaceWatching::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lace
	goto_if_set FLAG_GILDHAVEN_GYM3_CLEAR, GildhavenMerchantDistrict_EventScript_LaceWatchingPostGym3
	goto_if_set FLAG_GILDHAVEN_GYM2_CLEAR, GildhavenMerchantDistrict_EventScript_LaceWatchingPostGym2
	msgbox GildhavenMerchantDistrict_Text_LaceWatchingPre, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_LaceWatchingPostGym2::
	msgbox GildhavenMerchantDistrict_Text_LaceWatchingPostGym2, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_LaceWatchingPostGym3::
	msgbox GildhavenMerchantDistrict_Text_LaceWatchingPostGym3, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Street vendor - transparently overcharging. Corruption variant at
@ progress 3+. Post-resolution: confused, unmoored.
@ ===================================================================
GildhavenMerchantDistrict_EventScript_Vendor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Vendor
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenMerchantDistrict_EventScript_VendorResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenMerchantDistrict_EventScript_VendorCorrupt
	msgbox GildhavenMerchantDistrict_Text_Vendor, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_VendorCorrupt::
	msgbox GildhavenMerchantDistrict_Text_VendorCorrupt, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_VendorResolved::
	msgbox GildhavenMerchantDistrict_Text_VendorResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Child who has absorbed the island's values. Corruption variant at
@ progress 3+. Post-resolution: the father-crying line.
@ ===================================================================
GildhavenMerchantDistrict_EventScript_Child::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Child
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenMerchantDistrict_EventScript_ChildResolved
	compare VAR_GILDHAVEN_PROGRESS, 3
	goto_if_ge GildhavenMerchantDistrict_EventScript_ChildCorrupt
	msgbox GildhavenMerchantDistrict_Text_Child, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_ChildCorrupt::
	msgbox GildhavenMerchantDistrict_Text_ChildCorrupt, MSGBOX_NPC
	setspeaker 0
	release
	end

GildhavenMerchantDistrict_EventScript_ChildResolved::
	msgbox GildhavenMerchantDistrict_Text_ChildResolved, MSGBOX_NPC
	setspeaker 0
	release
	end

@ ===================================================================
@ Signs
@ ===================================================================
GildhavenMerchantDistrict_EventScript_SilkStall::
	lockall
	msgbox GildhavenMerchantDistrict_Text_SilkStall, MSGBOX_SIGN
	releaseall
	end

GildhavenMerchantDistrict_EventScript_GlintGymSign::
	lockall
	msgbox GildhavenMerchantDistrict_Text_GlintGymSign, MSGBOX_SIGN
	releaseall
	end

GildhavenMerchantDistrict_EventScript_ShadeGymSign::
	lockall
	msgbox GildhavenMerchantDistrict_Text_ShadeGymSign, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text - Glint
@ ===================================================================
GildhavenMerchantDistrict_Text_GlintIntro:
	.string "Everyone who comes to GILDHAVEN\n"
	.string "wants something.\p"
	.string "You want to go deeper into the city.\n"
	.string "I want to know if you are worth\l"
	.string "letting through.\p"
	.string "Simple transaction. Let's see.$"

GildhavenMerchantDistrict_Text_GlintDefeat:
	.string "Ha! A fair price, paid in full.$"

GildhavenMerchantDistrict_Text_GlintPostBattle:
	.string "Good. That is the most honest thing\n"
	.string "that has happened to me all week.$"

GildhavenMerchantDistrict_Text_ReceivedGiltBadge:
	.string "{PLAYER} received the GILT BADGE.$"

GildhavenMerchantDistrict_Text_GlintDirections:
	.string "The black market is three alleys\n"
	.string "back, behind the silk stall.\p"
	.string "Tell them GLINT sent you.\p"
	.string "And find DAGAN. He has been asking\n"
	.string "about you. Cheerfully. Which is\l"
	.string "somehow worse.$"

GildhavenMerchantDistrict_Text_GlintTM:
	.string "Take this, too. It is a TM - LIGHT\n"
	.string "SCREEN.\p"
	.string "I know, I know, you wanted something\n"
	.string "that sparkles and hits hard. We\l"
	.string "fairy types make do with what the\l"
	.string "old TM presses still stamp.\p"
	.string "A good shimmer is half the battle\n"
	.string "anyway.$"

GildhavenMerchantDistrict_Text_GlintPostBadge:
	.string "Go on, then. The city is waiting to\n"
	.string "charge you for something.$"

GildhavenMerchantDistrict_Text_GlintResolved:
	.string "Business is down forty percent and I\n"
	.string "have never slept better.\p"
	.string "Turns out half my customers only\n"
	.string "wanted things because the air told\l"
	.string "them to. Strange feeling, the air\l"
	.string "going quiet.$"

@ ===================================================================
@ Text - Shade
@ ===================================================================
GildhavenMerchantDistrict_Text_ShadeNotYet:
	.string "New here. I can tell.\p"
	.string "Come back when you have made enough\n"
	.string "noise that someone with money knows\l"
	.string "your name. Then we will talk.$"

GildhavenMerchantDistrict_Text_ShadeIntro:
	.string "I work for whoever pays.\p"
	.string "Right now that is VANE. Tomorrow,\n"
	.string "who knows.\p"
	.string "Beat me and I will consider it a job\n"
	.string "offer.$"

GildhavenMerchantDistrict_Text_ShadeDefeat:
	.string "...Consider it considered.$"

GildhavenMerchantDistrict_Text_ShadePostBattle:
	.string "You hit harder than the people who\n"
	.string "sign my checks. That is rare.$"

GildhavenMerchantDistrict_Text_ReceivedShadowBadge:
	.string "{PLAYER} received the SHADOW BADGE.$"

GildhavenMerchantDistrict_Text_ShadeDirections:
	.string "VANE's daughter is in the noble\n"
	.string "quarter. She has been watching the\l"
	.string "manor for a week.\p"
	.string "She is not watching it because she\n"
	.string "loves her father.\p"
	.string "Talk to her before you go further.$"

GildhavenMerchantDistrict_Text_ShadeTM:
	.string "Take this. TM - THIEF.\p"
	.string "You wanted CRUNCH, I would guess.\n"
	.string "Big teeth, big bite. We are fresh\l"
	.string "out.\p"
	.string "THIEF is more my style anyway. Take\n"
	.string "what you need and leave the rest\l"
	.string "lighter than you found it.$"

GildhavenMerchantDistrict_Text_ShadePostBadge:
	.string "Still thinking about that job offer.\n"
	.string "You would be a terrible employee.\l"
	.string "Too principled.$"

GildhavenMerchantDistrict_Text_ShadeResolved:
	.string "Vane stopped paying. Vane stopped\n"
	.string "everything.\p"
	.string "I am out of a job and oddly fine\n"
	.string "with it. First time in years I want\l"
	.string "to work for something instead of\l"
	.string "someone.$"

@ ===================================================================
@ Text - Dagan cafe
@ ===================================================================
GildhavenMerchantDistrict_Text_DaganCafePreGym1:
	.string "(A familiar figure at a cafe table\n"
	.string "lifts a cup in salute and grins.)\p"
	.string "Look who washed up. Win a gym battle\n"
	.string "or two first. Then come find me\l"
	.string "where the lighting is worse.\p"
	.string "I will be the one enjoying himself.$"

GildhavenMerchantDistrict_Text_DaganCafePostGym1:
	.string "There you are. Good showing against\n"
	.string "GLINT.\p"
	.string "I have moved my office. Behind the\n"
	.string "silk stall, down where the city\l"
	.string "pretends it does not look.\p"
	.string "Come down. I have been saving you a\n"
	.string "story.$"

GildhavenMerchantDistrict_Text_DaganCafeMet:
	.string "Up here for the coffee. It is\n"
	.string "extortionate and excellent.\p"
	.string "Everything you need is downstairs.\n"
	.string "You know the way now.$"

@ ===================================================================
@ Text - Lace watching
@ ===================================================================
GildhavenMerchantDistrict_Text_LaceWatchingPre:
	.string "(A young woman stands at the quarter's\n"
	.string "edge, watching the manor on the hill.\l"
	.string "She does not turn.)\p"
	.string "Not now. I am thinking.\p"
	.string "If you must talk to someone, the\n"
	.string "gyms are that way.$"

GildhavenMerchantDistrict_Text_LaceWatchingPostGym2:
	.string "You beat SHADE. Word travels here\n"
	.string "faster than ships.\p"
	.string "Come up to the quarter. The garden\n"
	.string "at the manor's edge. I will be there.\p"
	.string "There is something I need to do, and\n"
	.string "I think you are the reason I can\l"
	.string "finally do it.$"

GildhavenMerchantDistrict_Text_LaceWatchingPostGym3:
	.string "I do not stand at this corner\n"
	.string "anymore.\p"
	.string "I used to watch the manor and wait\n"
	.string "for it to feel like home again.\p"
	.string "It will not. I know that now. That\n"
	.string "is its own kind of freedom.$"

@ ===================================================================
@ Text - Vendor (corruption variants)
@ ===================================================================
GildhavenMerchantDistrict_Text_Vendor:
	.string "Only the best goods. Genuine\n"
	.string "artifacts from the Sunken Kingdom.\p"
	.string "...All authentic. Very reasonable\n"
	.string "prices for genuine artifacts.\p"
	.string "Did I mention they are genuine?$"

GildhavenMerchantDistrict_Text_VendorCorrupt:
	.string "The market was good today. Better\n"
	.string "than yesterday.\p"
	.string "Tomorrow will be better.\p"
	.string "I made four hundred credits today.\n"
	.string "I need five hundred tomorrow. I do\l"
	.string "not know why. I only know the number\l"
	.string "has to go up.$"

GildhavenMerchantDistrict_Text_VendorResolved:
	.string "I had the most extraordinary morning.\p"
	.string "I sat down with my ledger and looked\n"
	.string "at the numbers and could not\l"
	.string "remember why they mattered.\p"
	.string "An hour ago I would have called that\n"
	.string "impossible. Now I am wondering what\l"
	.string "else I want. Besides more.\p"
	.string "I am not sure. It has been a long\n"
	.string "time.$"

@ ===================================================================
@ Text - Child (corruption variants)
@ ===================================================================
GildhavenMerchantDistrict_Text_Child:
	.string "My dad says everything has a price.\p"
	.string "Even the things that do not look\n"
	.string "like they do. Even people.\p"
	.string "Is that right? That seems wrong.$"

GildhavenMerchantDistrict_Text_ChildCorrupt:
	.string "I am saving up. I do not know for\n"
	.string "what yet.\p"
	.string "Dad says it does not matter what. He\n"
	.string "says you save first and want later.\p"
	.string "That sounds backwards. But everyone\n"
	.string "here does it.$"

GildhavenMerchantDistrict_Text_ChildResolved:
	.string "Dad sat down at breakfast and\n"
	.string "started crying.\p"
	.string "He said he did not know why.\p"
	.string "I think I know why. I think\n"
	.string "something lifted.$"

@ ===================================================================
@ Text - Signs
@ ===================================================================
GildhavenMerchantDistrict_Text_SilkStall:
	.string "A silk stall, draped floor to ceiling.\p"
	.string "There is a narrow gap behind it - and\n"
	.string "a draft that smells of somewhere\l"
	.string "less polished.$"

GildhavenMerchantDistrict_Text_GlintGymSign:
	.string "GLINT'S MARKET ARENA\n"
	.string "Fairy specialist.\l"
	.string "Step right up. Everything is for\l"
	.string "sale, even a way through.$"

GildhavenMerchantDistrict_Text_ShadeGymSign:
	.string "(No sign. Just an alley, and the\n"
	.string "sense that someone in it is watching\l"
	.string "you decide whether to enter.)$"
