Sirocco_DaganPalace_Interior2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Sirocco_DaganPalace_Interior2_OnTransition
	.byte 0

@ Miria's post-battle cameo object uses FLAG_TEMP_1 as its hide flag.
@ Temp flags clear on every map load, which would make her visible on
@ entry - so the transition script re-hides her. She exists only for
@ the escape scene's map session (afterward she is back at the
@ Buried City, where her permanent object lives).
Sirocco_DaganPalace_Interior2_OnTransition:
	setflag FLAG_TEMP_1
	end

@ -------------------------------------------------------------------
@ Gym 4 - Dagan Mire (Ground/Rock). Crime lord, refreshingly honest.
@ Talk-initiated. After the battle he gives the Mire Badge and a TM,
@ delivers his exit speech (two optional response beats feed
@ VAR_DAGAN_RELATIONSHIP), and ESCAPES - scripted exit, no arrest,
@ no permanent-defeat flag. His object's hide flag is
@ FLAG_SIROCCO_DAGAN_ESCAPED. He reappears on Gildhaven.
@ NOTE on badges: the engine's badge flags end at FLAG_BADGE08_GET
@ (used by Miria). The MIRE BADGE is narrative-only - fanfare and
@ text play, but no FLAG_BADGEXX exists to set. Flagged for the
@ systems engineer if the badge UI is ever expanded.
@ NOTE on the TM: no TM teaches CRUNCH in this project's vanilla set;
@ Dagan gives TM THIEF instead (Dark-type, and entirely in character).
@ Skipped silently on a full bag - the escape scene must not abort.
@ -------------------------------------------------------------------
SiroccoDaganPalaceInterior2_EventScript_Dagan::
	goto_if_defeated TRAINER_LEADER_SIROCCO_DAGAN, SiroccoDaganPalaceInterior2_EventScript_DaganFallback
	setspeaker Pelagios_Speaker_Dagan
	trainerbattle_single TRAINER_LEADER_SIROCCO_DAGAN, SiroccoDaganPalaceInterior2_Text_DaganIntro, SiroccoDaganPalaceInterior2_Text_DaganDefeat, SiroccoDaganPalaceInterior2_EventScript_DaganVictory
	end

SiroccoDaganPalaceInterior2_EventScript_DaganVictory::
	msgbox SiroccoDaganPalaceInterior2_Text_StandsStraightens, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganNotFighting, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDaganPalaceInterior2_Text_PromptHonesty, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SiroccoDaganPalaceInterior2_EventScript_ToldHimOff
	msgbox SiroccoDaganPalaceInterior2_Text_YouSayNothing, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganNothingToSay, MSGBOX_DEFAULT
	goto SiroccoDaganPalaceInterior2_EventScript_BadgeAndTM

SiroccoDaganPalaceInterior2_EventScript_ToldHimOff::
	addvar VAR_DAGAN_RELATIONSHIP, 1
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganOnHonesty, MSGBOX_DEFAULT
SiroccoDaganPalaceInterior2_EventScript_BadgeAndTM::
	setspeaker 0
	msgbox SiroccoDaganPalaceInterior2_Text_FlipsBadge, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message SiroccoDaganPalaceInterior2_Text_ReceivedMireBadge
	waitmessage
	waitfanfare
	checkitemspace ITEM_TM_THIEF, 1
	call_if_eq VAR_RESULT, TRUE, SiroccoDaganPalaceInterior2_EventScript_GiveTM
	closemessage
	@ --- the window ---
	applymovement LOCALID_SIROCCO_DAGAN, SiroccoDaganPalaceInterior2_Movement_DaganFaceWindow
	waitmovement 0
	delay 30
	msgbox SiroccoDaganPalaceInterior2_Text_WalksToWindow, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganOasisBack, MSGBOX_DEFAULT
	msgbox SiroccoDaganPalaceInterior2_Text_DaganLastQuestion, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDaganPalaceInterior2_Text_PromptAnswer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SiroccoDaganPalaceInterior2_EventScript_AnsweredHim
	msgbox SiroccoDaganPalaceInterior2_Text_SilenceAnswers, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganOnSilence, MSGBOX_DEFAULT
	goto SiroccoDaganPalaceInterior2_EventScript_Escape

SiroccoDaganPalaceInterior2_EventScript_AnsweredHim::
	addvar VAR_DAGAN_RELATIONSHIP, 1
	msgbox SiroccoDaganPalaceInterior2_Text_StopThem, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganAlmostBelieves, MSGBOX_DEFAULT
	goto SiroccoDaganPalaceInterior2_EventScript_Escape

SiroccoDaganPalaceInterior2_EventScript_GiveTM::
	setspeaker 0
	giveitem ITEM_TM_THIEF
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganExplainTM, MSGBOX_DEFAULT
	return

@ -------------------------------------------------------------------
@ The escape - shared tail. Side door, gone. Then Miria enters.
@ Sets FLAG_SIROCCO_DAGAN_ESCAPED, FLAG_SIROCCO_GYM4_CLEAR,
@ VAR_SIROCCO_PROGRESS = 5 (arms the SealChamber discovery).
@ -------------------------------------------------------------------
SiroccoDaganPalaceInterior2_EventScript_Escape::
	closemessage
	delay 20
	msgbox SiroccoDaganPalaceInterior2_Text_SideDoor, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SIROCCO_DAGAN
	setflag FLAG_SIROCCO_DAGAN_ESCAPED
	playse SE_EXIT
	waitse
	fadescreen FADE_FROM_BLACK
	delay 30
	msgbox SiroccoDaganPalaceInterior2_Text_DoorClicksShut, MSGBOX_DEFAULT
	closemessage
	delay 30
	playse SE_EXIT
	clearflag FLAG_TEMP_1
	addobject LOCALID_SIROCCO_MIRIA_POST
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	setspeaker Pelagios_Speaker_Miria
	msgbox SiroccoDaganPalaceInterior2_Text_MiriaBoatReady, MSGBOX_DEFAULT
	msgbox SiroccoDaganPalaceInterior2_Text_MiriaFixWhatBroke, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SIROCCO_GYM4_CLEAR
	setvar VAR_SIROCCO_PROGRESS, 5
	release
	end

@ Defensive fallback: Dagan beaten but the escape never completed
@ (e.g. a reset mid-scene). One line, then the same escape tail.
SiroccoDaganPalaceInterior2_EventScript_DaganFallback::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dagan
	msgbox SiroccoDaganPalaceInterior2_Text_DaganFallback, MSGBOX_DEFAULT
	goto SiroccoDaganPalaceInterior2_EventScript_Escape

@ -------------------------------------------------------------------
@ Miria - if spoken to during the scene session
@ -------------------------------------------------------------------
SiroccoDaganPalaceInterior2_EventScript_MiriaPost::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Miria
	msgbox SiroccoDaganPalaceInterior2_Text_MiriaPostTalk, MSGBOX_DEFAULT
	release
	end

SiroccoDaganPalaceInterior2_Movement_DaganFaceWindow:
	face_up
	step_end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoDaganPalaceInterior2_Text_DaganIntro:
	.string "The WARDEN's child. I've been\n"
	.string "wondering when you'd show up.\p"
	.string "You want to know something funny?\n"
	.string "I'm not hiding anything.\p"
	.string "The seal is weakening, the water\n"
	.string "will run out, and I'll control\l"
	.string "what's left.\p"
	.string "It's not complicated.\n"
	.string "I just got here first.$"

SiroccoDaganPalaceInterior2_Text_DaganDefeat:
	.string "Well.\n"
	.string "That's that, then.$"

SiroccoDaganPalaceInterior2_Text_StandsStraightens:
	.string "He stands, unhurried, and\n"
	.string "straightens his jacket like a man\l"
	.string "concluding a meeting that ran\l"
	.string "slightly long.$"

SiroccoDaganPalaceInterior2_Text_DaganNotFighting:
	.string "I'm not going to fight about it.\n"
	.string "You won. Fair enough.\p"
	.string "But I want you to know - someone\n"
	.string "else would have done exactly what\l"
	.string "I did.\p"
	.string "I was just honest about it.$"

SiroccoDaganPalaceInterior2_Text_PromptHonesty:
	.string "Tell him honesty doesn't make it\n"
	.string "right?$"

SiroccoDaganPalaceInterior2_Text_YouSayNothing:
	.string "You say nothing.$"

SiroccoDaganPalaceInterior2_Text_DaganNothingToSay:
	.string "Nothing to say? Sensible.\p"
	.string "Words are how men like me win.$"

SiroccoDaganPalaceInterior2_Text_DaganOnHonesty:
	.string "No. It doesn't.\p"
	.string "But it makes me the only man on\n"
	.string "this island you can take at his\l"
	.string "word.\p"
	.string "Remember that when you meet the\n"
	.string "Covenant's smiling people.$"

SiroccoDaganPalaceInterior2_Text_FlipsBadge:
	.string "He flips something small and\n"
	.string "bright across the room. You catch\l"
	.string "it without thinking.$"

SiroccoDaganPalaceInterior2_Text_ReceivedMireBadge:
	.string "{PLAYER} received the MIRE BADGE\n"
	.string "from DAGAN!$"

SiroccoDaganPalaceInterior2_Text_DaganExplainTM:
	.string "And that TM is THIEF. I'd have\n"
	.string "given you CRUNCH, but apparently\l"
	.string "nobody prints it anymore.\p"
	.string "THIEF suits the occasion better\n"
	.string "anyway. Consider it a craftsman's\l"
	.string "keepsake.$"

SiroccoDaganPalaceInterior2_Text_WalksToWindow:
	.string "He turns to the tall window and\n"
	.string "looks out over his fountains, his\l"
	.string "gates, his desert.$"

SiroccoDaganPalaceInterior2_Text_DaganOasisBack:
	.string "The oasis will come back now,\n"
	.string "I suppose.\p"
	.string "Good for MIRADEN.\p"
	.string "I'll find another opportunity.$"

SiroccoDaganPalaceInterior2_Text_DaganLastQuestion:
	.string "One question, before I go.\p"
	.string "The Covenant will come for that\n"
	.string "seal eventually. They come for\l"
	.string "all of them - ask your parent.\p"
	.string "Ah. Apologies.\p"
	.string "…When they come.\n"
	.string "What will you do?$"

SiroccoDaganPalaceInterior2_Text_PromptAnswer:
	.string "Answer him?$"

SiroccoDaganPalaceInterior2_Text_SilenceAnswers:
	.string "You let the silence answer.$"

SiroccoDaganPalaceInterior2_Text_DaganOnSilence:
	.string "Smart. Never tell a stranger your\n"
	.string "plans.\p"
	.string "We're more alike than you'd enjoy\n"
	.string "hearing.$"

SiroccoDaganPalaceInterior2_Text_StopThem:
	.string "“Stop them,” you say.$"

SiroccoDaganPalaceInterior2_Text_DaganAlmostBelieves:
	.string "…Hm.\p"
	.string "I almost believe you. That's\n"
	.string "rarer than you'd think.\p"
	.string "I'll be watching from somewhere\n"
	.string "comfortable.$"

SiroccoDaganPalaceInterior2_Text_SideDoor:
	.string "He crosses to a side door you\n"
	.string "had taken for paneling, and opens\l"
	.string "it without a key.$"

SiroccoDaganPalaceInterior2_Text_DoorClicksShut:
	.string "The door clicks shut behind him.\p"
	.string "The room is suddenly just a room:\n"
	.string "expensive, quiet, and empty.$"

SiroccoDaganPalaceInterior2_Text_MiriaBoatReady:
	.string "He told me this would happen\n"
	.string "eventually.\p"
	.string "He had a boat ready.\p"
	.string "He always has a boat ready.$"

SiroccoDaganPalaceInterior2_Text_MiriaFixWhatBroke:
	.string "…Might as well fix what he broke.\p"
	.string "His machine is in the deep\n"
	.string "chamber under the BURIED CITY,\l"
	.string "past the mural hall. The way down\l"
	.string "is open.\p"
	.string "Go shut it off. I'll see that\n"
	.string "nobody follows you.$"

SiroccoDaganPalaceInterior2_Text_MiriaPostTalk:
	.string "The deep chamber. Under the\n"
	.string "BURIED CITY.\p"
	.string "I'll handle the Claw. They take\n"
	.string "orders from competence, and I'm\l"
	.string "what's left of it here.$"

SiroccoDaganPalaceInterior2_Text_DaganFallback:
	.string "Still here? We concluded our\n"
	.string "business, I believe.\p"
	.string "Watch the door on your way out.\n"
	.string "I intend to use the other one.$"
