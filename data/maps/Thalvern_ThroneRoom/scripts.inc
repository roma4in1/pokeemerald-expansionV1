Thalvern_ThroneRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_ThroneRoom_OnTransition
	.byte 0

@ ===================================================================
@ Dex's ThroneRoom cameo object (LOCALID_THALVERN_THRONE_DEX) uses
@ FLAG_TEMP_1 as its hide flag. Re-hide it each load; the throne
@ sequence clears it and addobjects him for the scene (PATH B).
@ ===================================================================
Thalvern_ThroneRoom_OnTransition:
	setflag FLAG_TEMP_1
	end

@ ===================================================================
@ THE THRONE SEQUENCE. Coord trigger at (13/14,34), gated
@ VAR_THALVERN_PROGRESS == 5. Branches on the path determinant set at
@ the Interior3 choice:
@   VAR_PELAGIOS_RESERVED_0x410B == 1 -> PATH A (player goes first)
@   VAR_PELAGIOS_RESERVED_0x410B == 2 -> PATH B (Dex goes first)
@ Exactly one path can ever play (one var, one branch). FLAG_DEX_ALIVE
@ is set ONLY inside PATH A's recovery. Both paths end at progress 6
@ with the seal apparatus armed for reinforcement.
@ ===================================================================
ThalvernThroneRoom_EventScript_ThroneSequence::
	lockall
	goto_if_eq VAR_PELAGIOS_RESERVED_0x410B, 2, ThalvernThroneRoom_EventScript_PathB
	goto ThalvernThroneRoom_EventScript_PathA

@ ===================================================================
@ PATH A - player goes first. THE LONGEST VISION IN THE GAME.
@ Full ancient-civilization memory, the sealing, then the flash of the
@ player's PARENT at this same spot. Player collapses. Three-day
@ recovery (rendered in-place over fades - see note below). Player
@ wakes, FLAG_DEX_ALIVE is set, then the seal is theirs to reinforce.
@ ===================================================================
ThalvernThroneRoom_EventScript_PathA::
	setspeaker 0
	msgbox ThalvernThroneRoom_Text_PathAEnter, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ The seal answers. Pelagios's presence (cry placeholder = Kyogre).
	playmoncry SPECIES_KYOGRE, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	@ Vision beat 1 - the world goes white and the memory opens.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 50
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_VisionCity, MSGBOX_DEFAULT
	closemessage
	@ Vision beat 2 - the final meeting, the decision.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 50
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_VisionDecision, MSGBOX_DEFAULT
	closemessage
	@ Vision beat 3 - Pelagios's own perspective.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 55
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_VisionPelagios, MSGBOX_DEFAULT
	closemessage
	@ Vision beat 4 - the Warden network is born.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 50
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_VisionWardens, MSGBOX_DEFAULT
	closemessage
	@ Vision beat 5 - the PARENT FLASH. Young, here, seeing this.
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 60
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_VisionParent, MSGBOX_DEFAULT
	closemessage
	@ Cipher 6 found (PATH A wording - the parent flash is encoded).
	call ThalvernThroneRoom_EventScript_Cipher6
	@ Collapse - the screen goes dark.
	delay 20
	fadescreen FADE_TO_BLACK
	delay 40
	msgbox ThalvernThroneRoom_Text_Collapse, MSGBOX_DEFAULT
	closemessage
	@ -----------------------------------------------------------------
	@ THREE-DAY RECOVERY. Rendered in-place over a held black screen
	@ (the player never sees the ThroneRoom during it). map.json gives
	@ DexCamp no bed object and no recovery hooks, and it is generator-
	@ owned, so the "makeshift bed at DexCamp" is delivered as narration
	@ + Dex's day-by-day dialogue rather than an actual map change. The
	@ day counter is VAR_PELAGIOS_RESERVED_0x410C, advanced 1 -> 2 -> 3,
	@ each day a distinct beat (Dex present throughout).
	@ -----------------------------------------------------------------
	setvar VAR_PELAGIOS_RESERVED_0x410C, 1
	call ThalvernThroneRoom_EventScript_RecoveryDay
	setvar VAR_PELAGIOS_RESERVED_0x410C, 2
	call ThalvernThroneRoom_EventScript_RecoveryDay
	setvar VAR_PELAGIOS_RESERVED_0x410C, 3
	call ThalvernThroneRoom_EventScript_RecoveryDay
	@ Day 3 done. The player stirs.
	delay 30
	msgbox ThalvernThroneRoom_Text_Wake, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernThroneRoom_Text_DexWakeAsk, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	msgbox ThalvernThroneRoom_Text_WakeReply, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernThroneRoom_Text_DexUnderstands, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ DEX SURVIVES. This is the ONLY place FLAG_DEX_ALIVE is ever set.
	setflag FLAG_DEX_ALIVE
	@ Return to the seal. Fade back up into the ThroneRoom (the player
	@ is still standing where they collapsed).
	delay 20
	fadescreen FADE_FROM_BLACK
	delay 20
	msgbox ThalvernThroneRoom_Text_PathAReturn, MSGBOX_DEFAULT
	closemessage
	setvar VAR_THALVERN_PROGRESS, 6
	releaseall
	end

@ The recovery-day sub-scene. One held-black beat per day, branched on
@ the day counter. Dex's line for that day, then a short hold.
ThalvernThroneRoom_EventScript_RecoveryDay::
	delay 30
	goto_if_eq VAR_PELAGIOS_RESERVED_0x410C, 1, ThalvernThroneRoom_EventScript_RecoveryDay1
	goto_if_eq VAR_PELAGIOS_RESERVED_0x410C, 2, ThalvernThroneRoom_EventScript_RecoveryDay2
	goto ThalvernThroneRoom_EventScript_RecoveryDay3

ThalvernThroneRoom_EventScript_RecoveryDay1::
	msgbox ThalvernThroneRoom_Text_RecoveryDay1Scene, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernThroneRoom_Text_RecoveryDay1Dex, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	return

ThalvernThroneRoom_EventScript_RecoveryDay2::
	msgbox ThalvernThroneRoom_Text_RecoveryDay2Scene, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernThroneRoom_Text_RecoveryDay2Dex, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	return

ThalvernThroneRoom_EventScript_RecoveryDay3::
	msgbox ThalvernThroneRoom_Text_RecoveryDay3Scene, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dex
	msgbox ThalvernThroneRoom_Text_RecoveryDay3Dex, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	return

@ ===================================================================
@ PATH B - Dex goes first. He enters ahead, a silence, then he
@ collapses. The player finds him with the decoded notes in hand.
@ Psalm arrives with a wrong diagnosis. ITEM_DEX_NOTES is added.
@ FLAG_DEX_ALIVE STAYS FALSE - it is never touched on this path.
@ ===================================================================
ThalvernThroneRoom_EventScript_PathB::
	setspeaker 0
	msgbox ThalvernThroneRoom_Text_PathBEnter, MSGBOX_DEFAULT
	closemessage
	@ Dex walks ahead toward the seal.
	clearflag FLAG_TEMP_1
	addobject LOCALID_THALVERN_THRONE_DEX
	delay 20
	applymovement LOCALID_THALVERN_THRONE_DEX, ThalvernThroneRoom_Movement_DexAdvance
	waitmovement 0
	delay 30
	@ The seal answers him. Pelagios's presence.
	playmoncry SPECIES_KYOGRE, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 55
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_PathBSilence, MSGBOX_DEFAULT
	closemessage
	@ He collapses.
	applymovement LOCALID_THALVERN_THRONE_DEX, ThalvernThroneRoom_Movement_DexCollapse
	waitmovement 0
	delay 20
	msgbox ThalvernThroneRoom_Text_PathBCollapse, MSGBOX_DEFAULT
	closemessage
	@ Cipher 6 found (same entry; here it comes from Dex's decoded
	@ notes scattered around him).
	call ThalvernThroneRoom_EventScript_Cipher6
	delay 20
	msgbox ThalvernThroneRoom_Text_PathBNotes, MSGBOX_DEFAULT
	closemessage
	@ Psalm arrives - wrong diagnosis.
	delay 30
	msgbox ThalvernThroneRoom_Text_PathBPsalmArrives, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Psalm
	msgbox ThalvernThroneRoom_Text_PathBPsalm, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	@ Dex's decoded notes become the player's research tool.
	giveitem ITEM_DEX_NOTES
	msgbox ThalvernThroneRoom_Text_PathBTakeNotes, MSGBOX_DEFAULT
	closemessage
	@ FLAG_DEX_ALIVE is NOT set or touched anywhere on PATH B.
	setvar VAR_THALVERN_PROGRESS, 6
	releaseall
	end

ThalvernThroneRoom_Movement_DexAdvance:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

ThalvernThroneRoom_Movement_DexCollapse:
	walk_in_place_faster_up
	delay_16
	set_invisible
	step_end

@ ===================================================================
@ Cipher 6 - Warden's Journal entry. Shared by BOTH paths (same text,
@ same flags). Sets FLAG_CIPHER_6_FOUND + FLAG_THALVERN_CIPHER_FOUND.
@ The encoded section about seeing the player's parent (successor) in
@ the vision is included on both paths per the task.
@ ===================================================================
ThalvernThroneRoom_EventScript_Cipher6::
	goto_if_set FLAG_THALVERN_CIPHER_FOUND, ThalvernThroneRoom_EventScript_Cipher6Done
	setflag FLAG_CIPHER_6_FOUND
	setflag FLAG_THALVERN_CIPHER_FOUND
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox ThalvernThroneRoom_Text_Cipher6, MSGBOX_DEFAULT
	closemessage
ThalvernThroneRoom_EventScript_Cipher6Done:
	return

@ ===================================================================
@ Seal apparatus - reinforcement + island resolution. Armed once the
@ throne sequence has run (progress >= 6). Pelagios gives a psychic
@ ACKNOWLEDGMENT (presence, not a vision), the ruins stop sinking, the
@ PokeNav call to Sollis fires (two variants on FLAG_DEX_ALIVE), the
@ Seal Shard is awarded LAST, then the Ashenveil three-island stub.
@ ===================================================================
ThalvernThroneRoom_EventScript_Apparatus::
	lockall
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernThroneRoom_EventScript_ApparatusResolved
	goto_if_lt VAR_THALVERN_PROGRESS, 6, ThalvernThroneRoom_EventScript_ApparatusNotReady
	msgbox ThalvernThroneRoom_Text_ApparatusApproach, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ThalvernThroneRoom_EventScript_ApparatusDecline
	closemessage
	@ Reinforce the seal. Pelagios acknowledges - presence only.
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 30
	playmoncry SPECIES_KYOGRE, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	fadescreen FADE_FROM_WHITE
	msgbox ThalvernThroneRoom_Text_Acknowledgment, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox ThalvernThroneRoom_Text_RuinsStabilize, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_THALVERN_SEAL_FOUND
	@ PokeNav call to Sollis - two variants on whether Dex survived.
	delay 20
	msgbox ThalvernThroneRoom_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox ThalvernThroneRoom_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	goto_if_set FLAG_DEX_ALIVE, ThalvernThroneRoom_EventScript_SollisAlive
	@ Dex gone.
	msgbox ThalvernThroneRoom_Text_SollisReplyGone, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox ThalvernThroneRoom_Text_SollisGone, MSGBOX_DEFAULT
	setspeaker 0
	goto ThalvernThroneRoom_EventScript_ApparatusFinish

ThalvernThroneRoom_EventScript_SollisAlive::
	msgbox ThalvernThroneRoom_Text_SollisReplyAlive, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox ThalvernThroneRoom_Text_SollisAlive, MSGBOX_DEFAULT
	setspeaker 0
	goto ThalvernThroneRoom_EventScript_ApparatusFinish

ThalvernThroneRoom_EventScript_ApparatusFinish::
	closemessage
	setflag FLAG_THALVERN_RESOLVED
	setvar VAR_THALVERN_PROGRESS, 7
	@ Award the Feraligatr Seal Shard LAST, so a full bag can never
	@ block resolution (it is a key item; key items always fit, but
	@ guard anyway and continue regardless).
	giveitem ITEM_SEAL_SHARD_THALVERN
	msgbox ThalvernThroneRoom_Text_SealShard, MSGBOX_DEFAULT
	closemessage
	@ Ashenveil three-island order-independent gate (stub; mirrors
	@ Schism's AshenveilCheck/NotYet). Gildhaven is not built yet.
	goto ThalvernThroneRoom_EventScript_AshenveilCheck

ThalvernThroneRoom_EventScript_AshenveilCheck::
	goto_if_unset FLAG_SCHISM_RESOLVED, ThalvernThroneRoom_EventScript_AshenveilNotYet
	goto_if_unset FLAG_GILDHAVEN_RESOLVED, ThalvernThroneRoom_EventScript_AshenveilNotYet
	msgbox ThalvernThroneRoom_Text_AshenveilOpens, MSGBOX_DEFAULT
	releaseall
	end

ThalvernThroneRoom_EventScript_AshenveilNotYet::
	msgbox ThalvernThroneRoom_Text_AshenveilNotYet, MSGBOX_DEFAULT
	releaseall
	end

ThalvernThroneRoom_EventScript_ApparatusResolved::
	msgbox ThalvernThroneRoom_Text_ApparatusResolved, MSGBOX_DEFAULT
	releaseall
	end

ThalvernThroneRoom_EventScript_ApparatusNotReady::
	msgbox ThalvernThroneRoom_Text_ApparatusNotReady, MSGBOX_DEFAULT
	releaseall
	end

ThalvernThroneRoom_EventScript_ApparatusDecline::
	msgbox ThalvernThroneRoom_Text_ApparatusDecline, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Dex fallback talk (PATH B cameo object, normally invisible).
@ ===================================================================
ThalvernThroneRoom_EventScript_Dex::
	lockall
	releaseall
	end

@ ===================================================================
@ Inscription sign - lore.
@ ===================================================================
ThalvernThroneRoom_EventScript_Inscription::
	lockall
	msgbox ThalvernThroneRoom_Text_Inscription, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text - vision (PATH A)
@ ===================================================================
ThalvernThroneRoom_Text_PathAEnter:
	.string "The deepest chamber. The water is\n"
	.string "gone here - or beneath you,\l"
	.string "everywhere, held back by something\l"
	.string "you can feel more than see.\p"
	.string "PELAGIOS. Vast. Ancient. Awake.\p"
	.string "It turns its attention to you, and\n"
	.string "the world dissolves into white.$"

ThalvernThroneRoom_Text_VisionCity:
	.string "...You are standing in the capital,\n"
	.string "whole, at noon.\p"
	.string "The smell of salt and bread. A bell.\n"
	.string "Someone laughing two streets over.\p"
	.string "It is not a picture. You are HERE.\n"
	.string "You could name the day.$"

ThalvernThroneRoom_Text_VisionDecision:
	.string "...A hall, lamplit. The leaders of\n"
	.string "the civilization, gathered.\p"
	.string "They have done the mathematics of\n"
	.string "the rising sea. They know there is\l"
	.string "no escape that does not abandon the\l"
	.string "lock.\p"
	.string "So they choose, together, to stay.\n"
	.string "To bind PELAGIOS and be bound to it.$"

ThalvernThroneRoom_Text_VisionPelagios:
	.string "...And then you are not yourself.\p"
	.string "You are PELAGIOS. You feel the\n"
	.string "binding settle, willingly accepted.\p"
	.string "You feel grief that is patient\n"
	.string "rather than sharp - the grief of\l"
	.string "something that will outlast every\l"
	.string "face it loves, and remember them\l"
	.string "all.$"

ThalvernThroneRoom_Text_VisionWardens:
	.string "...Generations pass in a heartbeat.\p"
	.string "The capital drowns. But the lock\n"
	.string "holds, because hands keep finding\l"
	.string "their way to it.\p"
	.string "A network of them, island to\n"
	.string "island, each keeping one bright\l"
	.string "seal turned. The WARDENS.$"

ThalvernThroneRoom_Text_VisionParent:
	.string "...And then a face you know.\p"
	.string "Young. Determined. Standing exactly\n"
	.string "where you stand now, seeing exactly\l"
	.string "what you see.\p"
	.string "Your parent. The WARDEN. Here,\n"
	.string "before you were born.\p"
	.string "For one impossible moment, across\n"
	.string "all that time, you are looking at\l"
	.string "each other.$"

ThalvernThroneRoom_Text_Collapse:
	.string "The white folds into black.\p"
	.string "Your knees give. The last thing you\n"
	.string "feel is someone catching you.$"

@ ===================================================================
@ Text - three-day recovery (PATH A)
@ ===================================================================
ThalvernThroneRoom_Text_RecoveryDay1Scene:
	.string "...A makeshift bed at DEX's camp.\n"
	.string "Lamplight. Rain on canvas. You\l"
	.string "drift near the surface of waking.$"

ThalvernThroneRoom_Text_RecoveryDay1Dex:
	.string "Easy. Easy. You're at the camp.\p"
	.string "You've been out a day. Don't try to\n"
	.string "talk yet. Take your time.\p"
	.string "I'm not going anywhere.$"

ThalvernThroneRoom_Text_RecoveryDay2Scene:
	.string "...The second day. The fever-light\n"
	.string "of the vision fades a little. You\l"
	.string "almost surface, then sink again.$"

ThalvernThroneRoom_Text_RecoveryDay2Dex:
	.string "PSALM came by. She says what you're\n"
	.string "going through is normal. Ish.\p"
	.string "She also said she's never seen\n"
	.string "anyone come back up at all. So.\l"
	.string "Mixed news.\p"
	.string "Rest.$"

ThalvernThroneRoom_Text_RecoveryDay3Scene:
	.string "...The third day. Warmth. Voices,\n"
	.string "low and easy. The smell of food.\l"
	.string "You are nearly here.$"

ThalvernThroneRoom_Text_RecoveryDay3Dex:
	.string "The LENS brought food. PSALM's\n"
	.string "outside. We're all here.\p"
	.string "We just kept the lamps lit and\n"
	.string "waited for you. That's all anyone\l"
	.string "could do.$"

ThalvernThroneRoom_Text_Wake:
	.string "...Your eyes open. The canvas\n"
	.string "ceiling. The rain has stopped.\p"
	.string "You are awake.$"

ThalvernThroneRoom_Text_DexWakeAsk:
	.string "There you are. There you ARE.\p"
	.string "...What did you see?$"

ThalvernThroneRoom_Text_WakeReply:
	.string "You try to find the words for the\n"
	.string "whole drowned world, the faces, the\l"
	.string "one face that was your parent's.\p"
	.string "There aren't enough words. DEX\n"
	.string "watches you try, and nods like he\l"
	.string "already knows.$"

ThalvernThroneRoom_Text_DexUnderstands:
	.string "I think I understand now why your\n"
	.string "parent sat with their work the way\l"
	.string "they did. Why they never tried to\l"
	.string "stop any of it.\p"
	.string "They saw this. They knew.\p"
	.string "...Come on. There's still a seal to\n"
	.string "mend. And this time you won't do it\l"
	.string "alone.$"

ThalvernThroneRoom_Text_PathAReturn:
	.string "You return to the Throne Room\n"
	.string "steadier than you left it.\p"
	.string "PELAGIOS waits. The seal waits.\n"
	.string "Approach the apparatus to reinforce\l"
	.string "it.$"

@ ===================================================================
@ Text - PATH B (Dex goes first)
@ ===================================================================
ThalvernThroneRoom_Text_PathBEnter:
	.string "DEX steadies his breath at the\n"
	.string "threshold.\p"
	.string "This is everything he has worked\n"
	.string "toward. He goes ahead.$"

ThalvernThroneRoom_Text_PathBSilence:
	.string "...He reaches the seal. He goes\n"
	.string "still.\p"
	.string "The white light takes him, gentle\n"
	.string "as a tide. For a long moment there\l"
	.string "is only silence.$"

ThalvernThroneRoom_Text_PathBCollapse:
	.string "Then DEX folds quietly to the\n"
	.string "floor.\p"
	.string "He does not cry out. He simply lies\n"
	.string "down, as if the vision were a place\l"
	.string "he chose to stay.$"

ThalvernThroneRoom_Text_PathBNotes:
	.string "You reach him. Notebooks are\n"
	.string "scattered around his hand - and\l"
	.string "every page is decoded. Complete.\p"
	.string "Everything he spent his life\n"
	.string "chasing, finished in the single\l"
	.string "moment it cost him.\p"
	.string "He won't wake. You can't wake him.$"

ThalvernThroneRoom_Text_PathBPsalmArrives:
	.string "Footsteps behind you. PSALM, out of\n"
	.string "breath, the LENS a step behind her.$"

ThalvernThroneRoom_Text_PathBPsalm:
	.string "...This. This is what I was afraid\n"
	.string "of.\p"
	.string "He'll live. But he won't wake up.\n"
	.string "I've seen it once before - someone\l"
	.string "who stayed too long in the vision.\p"
	.string "We'll care for him. We'll be here\n"
	.string "when he comes back.$"

ThalvernThroneRoom_Text_PathBTakeNotes:
	.string "You gather DEX's decoded notes.\p"
	.string "It feels wrong to take them. It\n"
	.string "would feel worse to leave his life's\l"
	.string "work on a flooding floor.\p"
	.string "It's what he'd want. It's all he'd\n"
	.string "want.$"

@ ===================================================================
@ Text - cipher 6 (both paths)
@ ===================================================================
ThalvernThroneRoom_Text_Cipher6:
	.string "The WARDEN's cipher resolves\n"
	.string "another fragment. Journal entry six:\p"
	.string "I saw it. The whole thing. I\n"
	.string "understand now why the WARDENS\l"
	.string "before me never wrote this down -\l"
	.string "there aren't words for the last\l"
	.string "day. There are only faces.\p"
	.string "PELAGIOS remembered every one of\n"
	.string "them. It still does.\p"
	.string "...I saw my successor in the vision.\n"
	.string "Young. Determined. I didn't know it\l"
	.string "was a vision at the time. I thought\l"
	.string "it was a dream.\p"
	.string "I hope they're ready. I hope I\n"
	.string "prepared enough.$"

@ ===================================================================
@ Text - reinforcement / resolution
@ ===================================================================
ThalvernThroneRoom_Text_ApparatusApproach:
	.string "The ancient mechanism that tends\n"
	.string "the seal stands open to you, worn\l"
	.string "smooth by two thousand years of\l"
	.string "caretakers' hands.\p"
	.string "Reinforce PELAGIOS's seal?$"

ThalvernThroneRoom_Text_Acknowledgment:
	.string "You set your hands where countless\n"
	.string "hands have rested. The lock turns.\p"
	.string "PELAGIOS does not show you a vision\n"
	.string "this time. It simply... notices you.\p"
	.string "A vast, quiet awareness, brushing\n"
	.string "past like a current. Recognition.\l"
	.string "Gratitude. Then it withdraws,\l"
	.string "patient again.$"

ThalvernThroneRoom_Text_RuinsStabilize:
	.string "Far above, the water that has\n"
	.string "swallowed the ruins for a hundred\l"
	.string "years stops rising.\p"
	.string "It will not give back what it took.\n"
	.string "But it will take no more, for now.$"

ThalvernThroneRoom_Text_PokenavBuzz:
	.string "...Your PokeNav buzzes.$"

ThalvernThroneRoom_Text_SollisCall:
	.string "THALVERN's readings just stabilized\n"
	.string "across the board.\p"
	.string "PELAGIOS's seal is holding.\p"
	.string "...What did you find in the Throne\n"
	.string "Room?$"

ThalvernThroneRoom_Text_SollisReplyAlive:
	.string "You tell her about the vision. The\n"
	.string "drowned world. The faces.\p"
	.string "And the one that was your parent's.$"

ThalvernThroneRoom_Text_SollisAlive:
	.string "...The vision. Your parent wrote\n"
	.string "about it.\p"
	.string "I always thought they were being\n"
	.string "poetic.\p"
	.string "They weren't being poetic.\p"
	.string "Come home when you can. And tell\n"
	.string "DEX I'm glad - I'm so glad - he's\l"
	.string "still arguing with the rest of us.$"

ThalvernThroneRoom_Text_SollisReplyGone:
	.string "You tell her about the vision. And\n"
	.string "you tell her about DEX.$"

ThalvernThroneRoom_Text_SollisGone:
	.string "...DEX.\p"
	.string "I knew him. We all knew him.\p"
	.string "His notes - use them. Decode the\n"
	.string "rest. That's what he'd want.\p"
	.string "That's all he'd want.\p"
	.string "...Come home, {PLAYER}.$"

ThalvernThroneRoom_Text_SealShard:
	.string "Set into the apparatus, loosed by\n"
	.string "the lock's turning, is a shard of\l"
	.string "blue light.\p"
	.string "{PLAYER} received the Seal Shard.\p"
	.string "It hums with the deep - the bond of\n"
	.string "a great water beast, waiting.$"

ThalvernThroneRoom_Text_AshenveilNotYet:
	.string "On the WARDEN's charts, three\n"
	.string "islands ring a fourth, darker mark:\l"
	.string "ASHENVEIL.\p"
	.string "THALVERN is quiet now. But the\n"
	.string "other rings still stir.\p"
	.string "The way to the dead island stays\n"
	.string "closed until all three are stilled.$"

ThalvernThroneRoom_Text_AshenveilOpens:
	.string "On the WARDEN's charts, the last of\n"
	.string "three rings around ASHENVEIL goes\l"
	.string "still.\p"
	.string "The way to the dead island is open\n"
	.string "at last.$"

ThalvernThroneRoom_Text_ApparatusResolved:
	.string "The seal is reinforced. The lock\n"
	.string "rests easy under your hand.$"

ThalvernThroneRoom_Text_ApparatusNotReady:
	.string "The mechanism is silent. Something\n"
	.string "must happen here first.$"

ThalvernThroneRoom_Text_ApparatusDecline:
	.string "You leave the lock untouched. It\n"
	.string "waits, patient as the deep.$"

ThalvernThroneRoom_Text_Inscription:
	.string "Carved beside the seal, in the\n"
	.string "oldest hand of all:\p"
	.string "We could not stay forever. So we\n"
	.string "asked it to remember for us. It\l"
	.string "agreed.$"
