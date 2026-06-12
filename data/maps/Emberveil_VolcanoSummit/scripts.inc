Emberveil_VolcanoSummit_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ The Solace confrontation. Coord triggers sit on the ritual
@ platform's ONLY entrance - the two-tile gap at (9,3)/(10,3) - and
@ are armed at VAR_EMBERVEIL_PROGRESS == 4 (Vex beaten, Solace not
@ yet confronted).
@
@ State walkthrough (verified against the trigger/branch table):
@   progress < 4               - unreachable (Vex/lava gates below),
@                                triggers disarmed either way
@   progress 4, notes UNSET    - Path A: her speech, battle, choice
@                                (YES = FLAG_SOLACE_TOLD_TRUTH,
@                                 NO = FLAG_SOLACE_ALT_ENDING),
@                                badge, progress = 5
@   progress 4, notes SET      - Path B prompt. Decline = retreat two
@                                tiles north (off the trigger; it
@                                re-arms). Accept = commune ritual,
@                                NO battle, FLAG_EMBERVEIL_PATH_B +
@                                badge, progress = 5
@   battle loss                - whiteout; progress stays 4; the
@                                scene re-fires on return
@   progress 5+                - triggers disarmed; Solace's talk
@                                script (below) carries her state
@
@ The player is walked only to row 4 - never onto the SealChamber
@ warp tiles at (9,5)/(10,5).
@ The TM is given WITHOUT a bag-full branch: this key scene must not
@ abort mid-cutscene (same decision as Sever's scene on Ironhold).
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_EventScript_SolaceScene::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilVolcanoSummit_Movement_PlayerEnter
	waitmovement 0
	delay 20
	msgbox EmberveilVolcanoSummit_Text_RitualInProgress, MSGBOX_DEFAULT
	closemessage
	playse SE_M_SUPERSONIC
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 0  @ horizontal pan
	setvar VAR_0x8006, 3  @ num shakes
	setvar VAR_0x8007, 6  @ shake delay
	special ShakeCamera
	waitstate
	msgbox EmberveilVolcanoSummit_Text_SealPulses, MSGBOX_DEFAULT
	closemessage
	goto_if_set FLAG_WARDEN_NOTES_EMBERVEIL, EmberveilVolcanoSummit_EventScript_PathB
	goto EmberveilVolcanoSummit_EventScript_PathA

@ ----------------------------- PATH A ------------------------------
EmberveilVolcanoSummit_EventScript_PathA::
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceAKnowsYou, MSGBOX_DEFAULT
	msgbox EmberveilVolcanoSummit_Text_SolaceAParent, MSGBOX_DEFAULT
	closemessage
	delay 30
	applymovement LOCALID_EMBERVEIL_SOLACE_SUMMIT, Common_Movement_FaceUp
	waitmovement 0
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceAResolve, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_LEADER_EMBERVEIL_SOLACE, EmberveilVolcanoSummit_Text_SolaceDefeat
	msgbox EmberveilVolcanoSummit_Text_AfterBattle, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceAccepts, MSGBOX_DEFAULT
	msgbox EmberveilVolcanoSummit_Text_SolaceWasIWrong, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EmberveilVolcanoSummit_EventScript_ChoiceMethod
	@ YES - "Yes. The volcano would have erupted."
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_ChoiceTruth, MSGBOX_DEFAULT
	setflag FLAG_SOLACE_TOLD_TRUTH
	goto EmberveilVolcanoSummit_EventScript_PathAClose

EmberveilVolcanoSummit_EventScript_ChoiceMethod::
	@ NO - "About the method. Not about Pyrath."
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_ChoiceMethod, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceMethodReply, MSGBOX_DEFAULT
	setflag FLAG_SOLACE_ALT_ENDING
	goto EmberveilVolcanoSummit_EventScript_PathAClose

EmberveilVolcanoSummit_EventScript_PathAClose::
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_BadgeWordless, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message EmberveilVolcanoSummit_Text_ReceivedPyrathBadge
	waitmessage
	waitfanfare
	closemessage
	setflag FLAG_EMBERVEIL_GYM4_CLEAR
	setvar VAR_EMBERVEIL_PROGRESS, 5
	giveitem ITEM_TM_FIRE_BLAST
	msgbox EmberveilVolcanoSummit_Text_PathAEnd, MSGBOX_DEFAULT
	releaseall
	end

@ ----------------------------- PATH B ------------------------------
EmberveilVolcanoSummit_EventScript_PathB::
	msgbox EmberveilVolcanoSummit_Text_NotesInPack, MSGBOX_DEFAULT
	msgbox EmberveilVolcanoSummit_Text_ShowNotesPrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EmberveilVolcanoSummit_EventScript_PathBDecline
	msgbox EmberveilVolcanoSummit_Text_HoldUpNotes, MSGBOX_DEFAULT
	closemessage
	delay 30
	applymovement LOCALID_EMBERVEIL_SOLACE_SUMMIT, Common_Movement_FaceUp
	waitmovement 0
	msgbox EmberveilVolcanoSummit_Text_RitualStops, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceBReads, MSGBOX_DEFAULT
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_LongPause, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceBThirtyYears, MSGBOX_DEFAULT
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_LongerPause, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceBShowMe, MSGBOX_DEFAULT
	closemessage
	@ --- The commune ritual ---
	msgbox EmberveilVolcanoSummit_Text_CommuneBegin, MSGBOX_DEFAULT
	closemessage
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 40
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER  @ placeholder cry for Pyrath
	waitmoncry
	delay 40
	fadescreen FADE_FROM_WHITE
	msgbox EmberveilVolcanoSummit_Text_CommunePyrath, MSGBOX_DEFAULT
	msgbox EmberveilVolcanoSummit_Text_CommuneStillness, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 6  @ shake delay
	special ShakeCamera
	waitstate
	msgbox EmberveilVolcanoSummit_Text_SealStabilizes, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceBAfter, MSGBOX_DEFAULT
	msgbox EmberveilVolcanoSummit_Text_SolaceBHeardUs, MSGBOX_DEFAULT
	setspeaker 0
	msgbox EmberveilVolcanoSummit_Text_BadgeWarm, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message EmberveilVolcanoSummit_Text_ReceivedPyrathBadge
	waitmessage
	waitfanfare
	closemessage
	setflag FLAG_EMBERVEIL_GYM4_CLEAR
	setflag FLAG_EMBERVEIL_PATH_B
	setvar VAR_EMBERVEIL_PROGRESS, 5
	giveitem ITEM_TM_FIRE_BLAST
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_PathBEnd, MSGBOX_DEFAULT
	releaseall
	end

EmberveilVolcanoSummit_EventScript_PathBDecline::
	msgbox EmberveilVolcanoSummit_Text_PathBDecline, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, EmberveilVolcanoSummit_Movement_PlayerRetreat
	waitmovement 0
	releaseall
	end

EmberveilVolcanoSummit_Movement_PlayerEnter:
	walk_down
	step_end

EmberveilVolcanoSummit_Movement_PlayerRetreat:
	walk_up
	walk_up
	step_end

@ -------------------------------------------------------------------
@ Solace's talk script - state-carried by flags after the scene.
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_EventScript_Solace::
	lock
	faceplayer
	goto_if_unset FLAG_EMBERVEIL_GYM4_CLEAR, EmberveilVolcanoSummit_EventScript_SolaceRitual
	goto_if_set FLAG_EMBERVEIL_RESOLVED, EmberveilVolcanoSummit_EventScript_SolaceResolved
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilVolcanoSummit_EventScript_SolaceWaitB
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceWaitA, MSGBOX_DEFAULT
	release
	end

EmberveilVolcanoSummit_EventScript_SolaceRitual::
	msgbox EmberveilVolcanoSummit_Text_SolaceLostInRitual, MSGBOX_DEFAULT
	release
	end

EmberveilVolcanoSummit_EventScript_SolaceWaitB::
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceWaitB, MSGBOX_DEFAULT
	release
	end

EmberveilVolcanoSummit_EventScript_SolaceResolved::
	goto_if_set FLAG_EMBERVEIL_PATH_B, EmberveilVolcanoSummit_EventScript_SolaceResolvedB
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceResolvedA, MSGBOX_DEFAULT
	release
	end

EmberveilVolcanoSummit_EventScript_SolaceResolvedB::
	setspeaker Pelagios_Speaker_Solace
	msgbox EmberveilVolcanoSummit_Text_SolaceResolvedB, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Text - shared opening
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_Text_RitualInProgress:
	.string "The summit opens around a\n"
	.string "platform of black stone.\p"
	.string "Set into the rock beyond it, a\n"
	.string "massive formation glows from\l"
	.string "within - PYRATH's seal, veins of\l"
	.string "light running through it like a\l"
	.string "heartbeat made visible.\p"
	.string "HIGH EMBER SOLACE stands before\n"
	.string "it, mid-ritual. She does not\l"
	.string "stop.\p"
	.string "She does not need to turn around\n"
	.string "to know you're here.$"

EmberveilVolcanoSummit_Text_SealPulses:
	.string "Her voice rises and falls. With\n"
	.string "every cycle of the chant, the\l"
	.string "seal pulses-\p"
	.string "-and each pulse is a little\n"
	.string "dimmer than the last.$"

@ -------------------------------------------------------------------
@ Text - Path A
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_Text_SolaceAKnowsYou:
	.string "You've come to stop the ritual.\n"
	.string "I know.\p"
	.string "The WARDEN's child - yes, I know\n"
	.string "who you are.$"

EmberveilVolcanoSummit_Text_SolaceAParent:
	.string "Your parent visited this summit\n"
	.string "years ago. They stood where\l"
	.string "you're standing.\p"
	.string "They didn't stop me either.\p"
	.string "They sat with me for an hour,\n"
	.string "and then they left.\p"
	.string "I've wondered about that for six\n"
	.string "years.$"

EmberveilVolcanoSummit_Text_SolaceAResolve:
	.string "I won't stop the ritual.\p"
	.string "PYRATH's awakening will purify\n"
	.string "this island. I have seen it.\p"
	.string "If you want to stop me, you'll\n"
	.string "have to battle me.$"

EmberveilVolcanoSummit_Text_SolaceDefeat:
	.string "…So that's what they were\n"
	.string "waiting for.$"

EmberveilVolcanoSummit_Text_AfterBattle:
	.string "The ritual fire gutters low.\p"
	.string "SOLACE folds her hands and\n"
	.string "watches her POKéMON return to\l"
	.string "their balls with complete\l"
	.string "serenity.$"

EmberveilVolcanoSummit_Text_SolaceAccepts:
	.string "You're stronger. That's clear.\p"
	.string "Do what you came to do.$"

EmberveilVolcanoSummit_Text_SolaceWasIWrong:
	.string "…Was I wrong? About all of it?$"

EmberveilVolcanoSummit_Text_ChoiceTruth:
	.string "Yes, you tell her. The volcano\n"
	.string "would have erupted. The\l"
	.string "awakening would have killed\l"
	.string "everyone it was meant to save.\p"
	.string "SOLACE closes her eyes.\p"
	.string "She says nothing more.$"

EmberveilVolcanoSummit_Text_ChoiceMethod:
	.string "About the method, you tell her.\n"
	.string "Not about PYRATH.\p"
	.string "SOLACE opens her eyes. Something\n"
	.string "shifts behind them.$"

EmberveilVolcanoSummit_Text_SolaceMethodReply:
	.string "Then perhaps there's more to\n"
	.string "understand.$"

EmberveilVolcanoSummit_Text_BadgeWordless:
	.string "Without ceremony, she presses\n"
	.string "something into your hand.$"

EmberveilVolcanoSummit_Text_PathAEnd:
	.string "She turns back to the dimming\n"
	.string "seal and does not watch you go.\p"
	.string "Past the cold ritual fire, stone\n"
	.string "steps lead down beneath the\l"
	.string "platform - toward the seal\l"
	.string "itself.$"

@ -------------------------------------------------------------------
@ Text - Path B
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_Text_NotesInPack:
	.string "The WARDEN's FIELD NOTES press\n"
	.string "against your pack like a held\l"
	.string "breath.\p"
	.string "Communion without breaking. The\n"
	.string "correction, written in your\l"
	.string "parent's hand.$"

EmberveilVolcanoSummit_Text_ShowNotesPrompt:
	.string "Show HIGH EMBER SOLACE the\n"
	.string "notes?$"

EmberveilVolcanoSummit_Text_HoldUpNotes:
	.string "You step toward the ritual fire\n"
	.string "and hold the notes up into the\l"
	.string "light of the seal.$"

EmberveilVolcanoSummit_Text_RitualStops:
	.string "The chant stops.\p"
	.string "For the first time in thirty\n"
	.string "years, mid-cycle, it stops.\p"
	.string "SOLACE crosses the platform and\n"
	.string "takes the pages as if they might\l"
	.string "be warm.\p"
	.string "She reads. The wind turns the\n"
	.string "ash around her feet.$"

EmberveilVolcanoSummit_Text_SolaceBReads:
	.string "These are your parent's notes.\p"
	.string "A way to commune with PYRATH\n"
	.string "without…\p"
	.string "…without weakening the seal.$"

EmberveilVolcanoSummit_Text_LongPause:
	.string "She reads them again, from the\n"
	.string "beginning.$"

EmberveilVolcanoSummit_Text_SolaceBThirtyYears:
	.string "I've been performing the ritual\n"
	.string "for thirty years.\p"
	.string "My mother performed it before\n"
	.string "me. Her mother before her.\p"
	.string "We thought we were calling to\n"
	.string "PYRATH.\p"
	.string "We were - we were maintaining\n"
	.string "a seal.$"

EmberveilVolcanoSummit_Text_LongerPause:
	.string "The seal pulses, dim, behind\n"
	.string "her. She watches it for a long\l"
	.string "time, and when she turns back,\l"
	.string "her face is decided.$"

EmberveilVolcanoSummit_Text_SolaceBShowMe:
	.string "Show me. Show me how the\n"
	.string "commune ritual works.\p"
	.string "If it's real - if your parent\n"
	.string "found a real path - I want to\l"
	.string "walk it.\p"
	.string "Not the one that kills my\n"
	.string "people.$"

EmberveilVolcanoSummit_Text_CommuneBegin:
	.string "Together, you trace the\n"
	.string "diagram's steps.\p"
	.string "SOLACE takes up the chant again -\n"
	.string "but slower now, lower, the\l"
	.string "rhythm your parent marked in\l"
	.string "the margins.\p"
	.string "The light of the seal deepens to\n"
	.string "warm amber-$"

EmberveilVolcanoSummit_Text_CommunePyrath:
	.string "-and through the glowing stone,\n"
	.string "something turns toward you.\p"
	.string "PYRATH. Vast beyond the telling\n"
	.string "of it. Neither imprisoned nor\l"
	.string "free.\p"
	.string "Aware.$"

EmberveilVolcanoSummit_Text_CommuneStillness:
	.string "For one held moment, the\n"
	.string "mountain, the woman, and the\l"
	.string "child of the WARDEN stand in a\l"
	.string "single warmth - acknowledged,\l"
	.string "all three.\p"
	.string "Then the moment sets you gently\n"
	.string "down.$"

EmberveilVolcanoSummit_Text_SealStabilizes:
	.string "The seal's pulse settles - slow,\n"
	.string "even, strong.\p"
	.string "No dimmer with each cycle.\n"
	.string "Not anymore.$"

EmberveilVolcanoSummit_Text_SolaceBAfter:
	.string "Thirty years. My mother's\n"
	.string "lifetime. Her mother's lifetime.\p"
	.string "All of it - not wrong. Just\n"
	.string "incomplete.\p"
	.string "We were maintaining something\n"
	.string "sacred. We just didn't know\l"
	.string "what.$"

EmberveilVolcanoSummit_Text_SolaceBHeardUs:
	.string "PYRATH heard us. All those\n"
	.string "years, PYRATH heard us.\p"
	.string "That's enough.\p"
	.string "That's more than enough.$"

EmberveilVolcanoSummit_Text_BadgeWarm:
	.string "She presses something into your\n"
	.string "hand - and laughs at herself,\l"
	.string "soft, surprised by the sound.$"

EmberveilVolcanoSummit_Text_PathBEnd:
	.string "Your parent's TM, too - FIRE\n"
	.string "BLAST. They left it with me. I\l"
	.string "think I finally understand why.\p"
	.string "The chamber below is open,\n"
	.string "child. Go and see what we kept\l"
	.string "alive without knowing.$"

EmberveilVolcanoSummit_Text_PathBDecline:
	.string "You keep the notes in your pack.\p"
	.string "Not here. Not shouted over a\n"
	.string "ritual fire, in front of all her\l"
	.string "people.\p"
	.string "Some truths need a quieter\n"
	.string "moment - or a stronger argument.$"

@ -------------------------------------------------------------------
@ Text - badge (shared) and talk script
@ -------------------------------------------------------------------
EmberveilVolcanoSummit_Text_ReceivedPyrathBadge:
	.string "{PLAYER} received the PYRATH BADGE\n"
	.string "from HIGH EMBER SOLACE!$"

EmberveilVolcanoSummit_Text_SolaceLostInRitual:
	.string "She is deep inside the chant.\p"
	.string "The words climb and fall, climb\n"
	.string "and fall, and the seal answers\l"
	.string "every line.$"

EmberveilVolcanoSummit_Text_SolaceWaitA:
	.string "The chamber is open.\p"
	.string "Do what you came to do.$"

EmberveilVolcanoSummit_Text_SolaceWaitB:
	.string "Go down, child. PYRATH is\n"
	.string "waiting to be seen.\p"
	.string "Two centuries, and no one ever\n"
	.string "simply went and LOOKED.$"

EmberveilVolcanoSummit_Text_SolaceResolvedA:
	.string "…It's done, then.\p"
	.string "The mountain is quiet. My\n"
	.string "people are quiet.\p"
	.string "I find I don't know what to do\n"
	.string "with so much quiet.$"

EmberveilVolcanoSummit_Text_SolaceResolvedB:
	.string "The island is breathing normally\n"
	.string "again. I can feel it.\p"
	.string "PYRATH is… calm.\p"
	.string "We'll keep the flame burning.\n"
	.string "Properly, now. A kept watch,\l"
	.string "not a countdown.\p"
	.string "Thank you. Your parent would\n"
	.string "have liked how this ended.$"
