Aetheron_CovenantInstallation_Interior_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, AetheronCovenantInstallationInterior_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, AetheronCovenantInstallationInterior_OnTransition
	.byte 0

@ ON_LOAD: once Voss has fallen and the defection has played
@ (FLAG_AETHERON_GYM3_CLEAR set), remove the Voss object so the room
@ stays empty on every return. The defection Cass object is flag-hidden
@ by FLAG_AETHERON_GYM3_CLEAR automatically.
AetheronCovenantInstallationInterior_OnLoad::
	goto_if_set FLAG_AETHERON_GYM3_CLEAR, AetheronCovenantInstallationInterior_EventScript_HideVoss
	end

AetheronCovenantInstallationInterior_EventScript_HideVoss::
	removeobject LOCALID_AETHERON_VOSS
	return

@ ON_TRANSITION: until the defection has fired, keep the Cass-defection
@ object hidden. Her object's hide flag is FLAG_AETHERON_GYM3_CLEAR,
@ which is CLEAR before the scene (so the engine would draw her early).
@ Removing her on every pre-defection load keeps her out of sight until
@ the Voss victory script reveals her with clearflag/addobject.
AetheronCovenantInstallationInterior_OnTransition::
	goto_if_set FLAG_AETHERON_GYM3_CLEAR, AetheronCovenantInstallationInterior_EventScript_TransDone
	removeobject LOCALID_AETHERON_CASS_DEFECT
	end

AetheronCovenantInstallationInterior_EventScript_TransDone::
	end

@ ===================================================================
@ Gym 3 - Commander Voss (Electric/Steel). Talk-initiated. Gated on
@ FLAG_AETHERON_GYM2_CLEAR (Arc must hand over the override first).
@ Victory flows DIRECTLY into the Cass defection scene - there is no
@ free-roam beat between Voss's last line and Cass in the doorway.
@ Badge NARRATIVE-ONLY (TM Thunder, as briefed). FLAG_AETHERON_GYM3_CLEAR
@ + progress are set at the END of the defection, not here.
@ ===================================================================
AetheronCovenantInstallationInterior_EventScript_Voss::
	goto_if_set FLAG_AETHERON_GYM3_CLEAR, AetheronCovenantInstallationInterior_EventScript_VossGone
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Voss
	goto_if_unset FLAG_AETHERON_GYM2_CLEAR, AetheronCovenantInstallationInterior_EventScript_VossNotYet
	msgbox AetheronCovenantInstallationInterior_Text_VossIntro, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_LEADER_AETHERON_VOSS, AetheronCovenantInstallationInterior_Text_VossDefeat
	goto AetheronCovenantInstallationInterior_EventScript_VossVictory
	end

AetheronCovenantInstallationInterior_EventScript_VossNotYet::
	msgbox AetheronCovenantInstallationInterior_Text_VossNotYet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCovenantInstallationInterior_EventScript_VossVictory::
	setspeaker Pelagios_Speaker_Voss
	msgbox AetheronCovenantInstallationInterior_Text_VossPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message AetheronCovenantInstallationInterior_Text_ReceivedCommandBadge
	waitmessage
	waitfanfare
	@ TM Thunder, as briefed. Given BEFORE the defection so a full bag
	@ can never interrupt the scene that follows.
	giveitem ITEM_TM_THUNDER
	@ (No bag-full abort here - if it's full the TM is forfeit, but the
	@  defection MUST still play. Re-check below only for the TM line.)
	copyvar VAR_TEMP_2, VAR_RESULT
	@ He looks toward the doorway. The path forward has been decided.
	setspeaker Pelagios_Speaker_Voss
	msgbox AetheronCovenantInstallationInterior_Text_VossLooksToDoor, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	goto AetheronCovenantInstallationInterior_EventScript_CassDefection
	end

AetheronCovenantInstallationInterior_EventScript_VossGone::
	end

@ ===================================================================
@ THE CASS DEFECTION SCENE. The emotional turn of the rival arc.
@ EXACT timing is honored: stop music (fadeoutbgm 4); the silence
@ between Voss's order and Cass's first word is delay 60 (NON-
@ NEGOTIABLE) - Cass's final line is exactly "Okay." (delay 20) "Let's
@ go." Music resumes (fadedefaultbgm) before her "Can I come with you?"
@ FLAG_CASS_DEFECTED set; VAR_CASS_RELATIONSHIP HARD-SET to 3.
@ Chained from the Voss victory (player still locked, never walks).
@ ===================================================================
AetheronCovenantInstallationInterior_EventScript_CassDefection::
	@ Reveal Cass in the north doorway (her hide flag is GYM3_CLEAR,
	@ which is still clear here, so clearflag + addobject is safe).
	clearflag FLAG_AETHERON_GYM3_CLEAR
	addobject LOCALID_AETHERON_CASS_DEFECT
	delay 20
	@ SILENCE. The music stops and does not return until "Can I come?"
	fadeoutbgm 4
	delay 30
	@ Voss gives the order.
	setspeaker Pelagios_Speaker_Voss
	msgbox AetheronCovenantInstallationInterior_Text_DefVossOrder, MSGBOX_DEFAULT
	closemessage
	@ Cass does NOT move. The most important beat - do not shorten.
	delay 60
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronCovenantInstallationInterior_Text_DefCassCommander, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox AetheronCovenantInstallationInterior_Text_DefCassDocuments, MSGBOX_DEFAULT
	closemessage
	delay 40
	msgbox AetheronCovenantInstallationInterior_Text_DefCassCarrying, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox AetheronCovenantInstallationInterior_Text_DefCassNoExplanation, MSGBOX_DEFAULT
	closemessage
	@ Voss starts to speak - Cass cuts him off.
	setspeaker Pelagios_Speaker_Voss
	msgbox AetheronCovenantInstallationInterior_Text_DefVossCutOff, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronCovenantInstallationInterior_Text_DefCassDone, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ Cass steps fully into the room and faces the player.
	setspeaker 0
	applymovement LOCALID_AETHERON_CASS_DEFECT, AetheronCovenantInstallationInterior_Movement_CassStepIn
	waitmovement 0
	delay 20
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronCovenantInstallationInterior_Text_DefCassSorry, MSGBOX_DEFAULT
	closemessage
	delay 40
	msgbox AetheronCovenantInstallationInterior_Text_DefCassComeWithYou, MSGBOX_DEFAULT
	closemessage
	@ Music resumes.
	fadedefaultbgm
	delay 20
	@ Player response - the answer is always yes.
	setspeaker 0
	msgbox AetheronCovenantInstallationInterior_Text_DefPlayerYes, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ Cass's final line: "Okay." (delay 20 beat) "Let's go."
	setspeaker Pelagios_Speaker_Cass
	msgbox AetheronCovenantInstallationInterior_Text_DefCassOkay, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox AetheronCovenantInstallationInterior_Text_DefCassLetsGo, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	delay 20
	@ Voss exits quietly - no speech, just turns and walks out.
	applymovement LOCALID_AETHERON_VOSS, AetheronCovenantInstallationInterior_Movement_VossExit
	waitmovement 0
	removeobject LOCALID_AETHERON_VOSS
	delay 20
	@ Cass leaves this map with the player (she reappears on StormPeak).
	removeobject LOCALID_AETHERON_CASS_DEFECT
	@ State: gym cleared, Cass defected, relationship HARD-SET to max.
	setflag FLAG_AETHERON_GYM3_CLEAR
	setflag FLAG_CASS_DEFECTED
	setvar VAR_CASS_RELATIONSHIP, 3
	setvar VAR_AETHERON_PROGRESS, 4
	@ Deliver the forfeited-TM note only if the bag was full earlier.
	goto_if_eq VAR_TEMP_2, FALSE, AetheronCovenantInstallationInterior_EventScript_DefectionEndBagFull
	releaseall
	end

AetheronCovenantInstallationInterior_EventScript_DefectionEndBagFull::
	setspeaker 0
	msgbox AetheronCovenantInstallationInterior_Text_TMForfeit, MSGBOX_DEFAULT
	releaseall
	end

@ Cass steps down 2 tiles from the doorway (13,2)->(13,4); (13,5) is a
@ wall, so she stops there and faces down into the room.
AetheronCovenantInstallationInterior_Movement_CassStepIn:
	walk_down
	walk_down
	face_down
	step_end

@ Voss walks from (8,5) south to the exit warp at (8,9) and is gone.
AetheronCovenantInstallationInterior_Movement_VossExit:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ ===================================================================
@ Officer Sera (TRAINER_COVENANT_AETHERON_2) - interior sight trainer.
@ Sight trainer (trainerbattle first command). Post-defection she is
@ gone with the rest of the installation staff (flag-independent; her
@ object simply isn't re-approached once Voss falls and the player
@ leaves up the stairs - acceptable, same as other islands' staff).
@ ===================================================================
AetheronCovenantInstallationInterior_EventScript_Officer2::
	trainerbattle_single TRAINER_COVENANT_AETHERON_2, AetheronCovenantInstallationInterior_Text_SeraIntro, AetheronCovenantInstallationInterior_Text_SeraDefeat, AetheronCovenantInstallationInterior_EventScript_Officer2Post
	setspeaker Pelagios_Speaker_Officer
	msgbox AetheronCovenantInstallationInterior_Text_SeraPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCovenantInstallationInterior_EventScript_Officer2Post::
	setspeaker Pelagios_Speaker_Officer
	msgbox AetheronCovenantInstallationInterior_Text_SeraPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Control panel (bg_event examine, sign at (11,3)). The yield log -
@ verbatim from AETHERON_BRIEF.md. Sets FLAG_AETHERON_INSTALLATION_FOUND.
@ Narration (no namebox).
@ ===================================================================
AetheronCovenantInstallationInterior_EventScript_ControlPanel::
	lockall
	setspeaker 0
	msgbox AetheronCovenantInstallationInterior_Text_ControlPanel, MSGBOX_DEFAULT
	setflag FLAG_AETHERON_INSTALLATION_FOUND
	releaseall
	end

@ ===================================================================
@ Text - Voss (gym)
@ ===================================================================
AetheronCovenantInstallationInterior_Text_VossNotYet:
	.string "Warden's child. I know why you're here.\p"
	.string "But you haven't earned your way to me\n"
	.string "yet. Deal with ARC. Then we'll see what\l"
	.string "you're made of.$"

AetheronCovenantInstallationInterior_Text_VossDefeat:
	.string "…Above my authority.$"

AetheronCovenantInstallationInterior_Text_VossPostBattle:
	.string "…You're stronger than the file\n"
	.string "suggested.\p"
	.string "I've done my duty. What happens next is\n"
	.string "above my authority.$"

AetheronCovenantInstallationInterior_Text_ReceivedCommandBadge:
	.string "{PLAYER} received the COMMAND BADGE.$"

AetheronCovenantInstallationInterior_Text_VossLooksToDoor:
	.string "Though I think you'll find the path\n"
	.string "forward has already been decided for\l"
	.string "you.$"

@ Pre-battle intro is delivered through trainerbattle_no_intro's flow
@ as a standalone preamble msgbox before the fight.
AetheronCovenantInstallationInterior_Text_VossIntro:
	.string "Warden's child. I've read the file.\p"
	.string "You've been very busy. Eight islands.\n"
	.string "Eight seals reinforced.\p"
	.string "Do you understand what you're undoing?\p"
	.string "The Covenant has kept this region\n"
	.string "stable for forty years by managing the\l"
	.string "seal network.\p"
	.string "You're dismantling forty years of work\n"
	.string "because a dead man left you a journal.\p"
	.string "I can't let you proceed.$"

@ ===================================================================
@ Text - the defection (verbatim where the brief provides it)
@ ===================================================================
AetheronCovenantInstallationInterior_Text_DefVossOrder:
	.string "Agent Vell. Stop them.$"

AetheronCovenantInstallationInterior_Text_DefCassCommander:
	.string "Commander.$"

AetheronCovenantInstallationInterior_Text_DefCassDocuments:
	.string "I found documents on Ashenveil.\p"
	.string "The evacuation records. The real ones.\p"
	.string "847 people. 400 boat capacity.\p"
	.string "401 survivors recorded.$"

AetheronCovenantInstallationInterior_Text_DefCassCarrying:
	.string "I've been carrying them since\n"
	.string "Gildhaven.\p"
	.string "I kept thinking there would be an\n"
	.string "explanation.$"

AetheronCovenantInstallationInterior_Text_DefCassNoExplanation:
	.string "There isn't one.$"

AetheronCovenantInstallationInterior_Text_DefVossCutOff:
	.string "Agent Vell-$"

AetheronCovenantInstallationInterior_Text_DefCassDone:
	.string "I'm done.$"

AetheronCovenantInstallationInterior_Text_DefCassSorry:
	.string "{PLAYER}.\p"
	.string "I'm sorry it took me this long.$"

AetheronCovenantInstallationInterior_Text_DefCassComeWithYou:
	.string "Can I come with you?$"

AetheronCovenantInstallationInterior_Text_DefPlayerYes:
	.string "(There was never any other answer.)\p"
	.string "Of course.$"

AetheronCovenantInstallationInterior_Text_DefCassOkay:
	.string "Okay.$"

AetheronCovenantInstallationInterior_Text_DefCassLetsGo:
	.string "Let's go.$"

AetheronCovenantInstallationInterior_Text_TMForfeit:
	.string "(Your bag was too full to hold the disc\n"
	.string "VOSS left behind. There's no going back\l"
	.string "for it now.)$"

@ ===================================================================
@ Text - Officer Sera
@ ===================================================================
AetheronCovenantInstallationInterior_Text_SeraIntro:
	.string "Commander VOSS is expecting you.\n"
	.string "He's not pleased.$"

AetheronCovenantInstallationInterior_Text_SeraDefeat:
	.string "This is going in the report.$"

AetheronCovenantInstallationInterior_Text_SeraPost:
	.string "He'll be less pleased when I report\n"
	.string "this.$"

@ ===================================================================
@ Text - control panel yield log (verbatim)
@ ===================================================================
AetheronCovenantInstallationInterior_Text_ControlPanel:
	.string "STORMVEIL EXTRACTION ARRAY - YIELD LOG\p"
	.string "Month 1: 847 terawatt-hours\n"
	.string "Month 6: 1,204 TWh (seal degradation: 12%)\n"
	.string "Month 12: 1,891 TWh (seal degradation: 31%)\n"
	.string "Month 18: 2,847 TWh (seal degradation: 53%)\n"
	.string "Month 24: (REDACTED) (seal degradation: 71%)\p"
	.string "PROJECTED SEAL FAILURE: (REDACTED)\p"
	.string "NOTE: Yield increase corresponds\n"
	.string "directly to seal degradation rate.\l"
	.string "Extraction is accelerating.\l"
	.string "Do not distribute externally.$"
