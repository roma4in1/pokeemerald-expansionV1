Convergence_Epilogue_MapScripts::
	.byte 0

@ ===========================================================================
@ THE EPILOGUE - the last thing the player sees before the credits.
@
@ The player warps in from the kill switch chamber with
@ VAR_CONVERGENCE_PROGRESS == 5. Walking north crosses the credits trigger
@ at (11,12)/(12,12), which branches on the FLAG_ENDING_*_PLAYED marker the
@ chamber set, plays the matching epilogue, then rolls the credits.
@
@ The named NPC objects on this map are gated behind FLAG_CONVERGENCE_COMPLETE,
@ so they are HIDDEN during the epilogue cutscene and APPEAR for post-game
@ once the flag is set (just before GameClear). The cutscene itself is told
@ entirely through narration and speech - no object movement is required, so
@ the gated objects never need to be present mid-scene.
@
@ CREDITS MECHANISM: vanilla `special GameClear` (the Hall-of-Fame / end-game
@ path used by EverGrandeCity_HallOfFame). It heals the party, records the
@ Hall of Fame, rolls the FRLG credits sequence, sets FLAG_SYS_GAME_CLEAR, and
@ returns the player to their last heal location for post-game. The Storm
@ Compass already works from the bag, so post-game island travel is enabled.
@ FLAG_CONVERGENCE_COMPLETE is set immediately BEFORE GameClear so it persists.
@ ===========================================================================

ConvergenceEpilogue_EventScript_Credits::
	lockall
	goto_if_set FLAG_ENDING_TRUE_PLAYED, ConvergenceEpilogue_EventScript_EpilogueTrue
	goto_if_set FLAG_ENDING_HELP_PLAYED, ConvergenceEpilogue_EventScript_EpilogueHelp
	goto ConvergenceEpilogue_EventScript_EpilogueStop

@ ===========================================================================
@ EPILOGUE 1 - STOP. Haven Isle. Sollis is there. The player gives no answer.
@ ===========================================================================
ConvergenceEpilogue_EventScript_EpilogueStop::
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_StopSetting, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceEpilogue_Text_StopSollis, MSGBOX_DEFAULT
	closemessage
	@ The player says nothing.
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_StopSilence, MSGBOX_DEFAULT
	msgbox ConvergenceEpilogue_Text_StopHarbor, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceEpilogue_EventScript_RollCredits

@ ===========================================================================
@ EPILOGUE 2 - HELP. The world changed. The legendaries free in the distance.
@ ===========================================================================
ConvergenceEpilogue_EventScript_EpilogueHelp::
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_HelpSetting, MSGBOX_DEFAULT
	closemessage
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceEpilogue_Text_HelpDorneDifferent, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceEpilogue_Text_HelpCassWrong, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceEpilogue_Text_HelpDorneQuieter, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_HelpDistance, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceEpilogue_EventScript_RollCredits

@ ===========================================================================
@ EPILOGUE 3 - TRUE. Every returning character. A final line each.
@ Cass's line is the LAST dialogue in the game. NOTHING follows it.
@ ===========================================================================
ConvergenceEpilogue_EventScript_EpilogueTrue::
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_TrueSetting, MSGBOX_DEFAULT
	closemessage

	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceEpilogue_Text_TrueSollis, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceEpilogue_Text_TrueCass, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceEpilogue_Text_TrueDorne, MSGBOX_DEFAULT
	closemessage

	@ Eira says nothing. She nods once. (narration - no namebox.)
	setspeaker 0
	msgbox ConvergenceEpilogue_Text_TrueEira, MSGBOX_DEFAULT
	closemessage

	setspeaker Pelagios_Speaker_Mako
	msgbox ConvergenceEpilogue_Text_TrueMako, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Lens
	msgbox ConvergenceEpilogue_Text_TrueLens, MSGBOX_DEFAULT
	closemessage

	@ Dex - only if alive.
	goto_if_set FLAG_DEX_ALIVE, ConvergenceEpilogue_EventScript_TrueDex
	goto ConvergenceEpilogue_EventScript_TrueAfterDex
ConvergenceEpilogue_EventScript_TrueDex::
	setspeaker Pelagios_Speaker_Dex
	msgbox ConvergenceEpilogue_Text_TrueDex, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceEpilogue_EventScript_TrueAfterDex

ConvergenceEpilogue_EventScript_TrueAfterDex::
	@ Drenn - only if alive.
	goto_if_set FLAG_DRENN_ALIVE, ConvergenceEpilogue_EventScript_TrueDrenn
	goto ConvergenceEpilogue_EventScript_TrueAfterDrenn
ConvergenceEpilogue_EventScript_TrueDrenn::
	setspeaker Pelagios_Speaker_Drenn
	msgbox ConvergenceEpilogue_Text_TrueDrenn, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceEpilogue_EventScript_TrueAfterDrenn

ConvergenceEpilogue_EventScript_TrueAfterDrenn::
	setspeaker Pelagios_Speaker_Lace
	msgbox ConvergenceEpilogue_Text_TrueLace, MSGBOX_DEFAULT
	closemessage

	@ Dagan - always funny, always charming, always slightly out of place.
	setspeaker Pelagios_Speaker_Dagan
	goto_if_eq VAR_DAGAN_RELATIONSHIP, 0, ConvergenceEpilogue_EventScript_TrueDagan0
	goto_if_eq VAR_DAGAN_RELATIONSHIP, 1, ConvergenceEpilogue_EventScript_TrueDagan1
	goto ConvergenceEpilogue_EventScript_TrueDagan2
ConvergenceEpilogue_EventScript_TrueDagan0::
	msgbox ConvergenceEpilogue_Text_TrueDagan0, MSGBOX_DEFAULT
	goto ConvergenceEpilogue_EventScript_TrueCassFinal
ConvergenceEpilogue_EventScript_TrueDagan1::
	msgbox ConvergenceEpilogue_Text_TrueDagan1, MSGBOX_DEFAULT
	goto ConvergenceEpilogue_EventScript_TrueCassFinal
ConvergenceEpilogue_EventScript_TrueDagan2::
	msgbox ConvergenceEpilogue_Text_TrueDagan2, MSGBOX_DEFAULT
	goto ConvergenceEpilogue_EventScript_TrueCassFinal

@ THE FINAL LINE OF THE GAME. Nothing follows it. Credits roll immediately.
ConvergenceEpilogue_EventScript_TrueCassFinal::
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceEpilogue_Text_TrueCassFinal, MSGBOX_DEFAULT
	closemessage
	goto ConvergenceEpilogue_EventScript_RollCredits

@ ===========================================================================
@ SHARED CREDITS TAIL.
@ Set FLAG_CONVERGENCE_COMPLETE (post-game NPCs appear; island resolved),
@ fade out, and roll the vanilla end-game credits via `special GameClear`.
@ ===========================================================================
ConvergenceEpilogue_EventScript_RollCredits::
	setvar VAR_CONVERGENCE_PROGRESS, 6
	setflag FLAG_CONVERGENCE_COMPLETE
	fadescreenspeed FADE_TO_BLACK, 24
	special GameClear
	releaseall
	end

@ ===========================================================================
@ POST-GAME FLAVOR TALK (objects appear once FLAG_CONVERGENCE_COMPLETE is set).
@ ===========================================================================
ConvergenceEpilogue_EventScript_Sollis::
	lockall
	setspeaker Pelagios_Speaker_Sollis
	msgbox ConvergenceEpilogue_Text_PostSollis, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Cass::
	lockall
	setspeaker Pelagios_Speaker_Cass
	msgbox ConvergenceEpilogue_Text_PostCass, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Dorne::
	lockall
	setspeaker Pelagios_Speaker_Dorne
	msgbox ConvergenceEpilogue_Text_PostDorne, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Eira::
	lockall
	setspeaker Pelagios_Speaker_Eira
	msgbox ConvergenceEpilogue_Text_PostEira, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Mako::
	lockall
	setspeaker Pelagios_Speaker_Mako
	msgbox ConvergenceEpilogue_Text_PostMako, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Lens::
	lockall
	setspeaker Pelagios_Speaker_Lens
	msgbox ConvergenceEpilogue_Text_PostLens, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Drenn::
	lockall
	setspeaker Pelagios_Speaker_Drenn
	msgbox ConvergenceEpilogue_Text_PostDrenn, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Lace::
	lockall
	setspeaker Pelagios_Speaker_Lace
	msgbox ConvergenceEpilogue_Text_PostLace, MSGBOX_DEFAULT
	releaseall
	end

ConvergenceEpilogue_EventScript_Dagan::
	lockall
	setspeaker Pelagios_Speaker_Dagan
	msgbox ConvergenceEpilogue_Text_PostDagan, MSGBOX_DEFAULT
	releaseall
	end

@ ===========================================================================
@ TEXT - EPILOGUE 1 (STOP)
@ ===========================================================================
ConvergenceEpilogue_Text_StopSetting:
	.string "Haven Isle. Home.\p"
	.string "The sea is the same as it always was.\n"
	.string "Sollis is waiting where the path meets\l"
	.string "the water.$"

ConvergenceEpilogue_Text_StopSollis:
	.string "You stopped him. The seals are stable.\p"
	.string "Is that enough?$"

ConvergenceEpilogue_Text_StopSilence:
	.string "The player doesn't answer.\p"
	.string "Some questions don't have one. Not yet.$"

ConvergenceEpilogue_Text_StopHarbor:
	.string "The player walks down to the harbor\n"
	.string "alone and looks out at the sea, the way\l"
	.string "the Warden used to.\p"
	.string "Somewhere out there, the work goes on.$"

@ ===========================================================================
@ TEXT - EPILOGUE 2 (HELP)
@ ===========================================================================
ConvergenceEpilogue_Text_HelpSetting:
	.string "The world is changed.\p"
	.string "Out past the island, in the sky and the\n"
	.string "deep water, vast shapes move free - the\l"
	.string "legendaries, loosed into a world that no\l"
	.string "longer holds them.\p"
	.string "Dorne stands at the mouth of the empty\n"
	.string "seal chamber, looking in.$"

ConvergenceEpilogue_Text_HelpDorneDifferent:
	.string "I thought this would feel different.$"

ConvergenceEpilogue_Text_HelpCassWrong:
	.string "Does it feel wrong?$"

ConvergenceEpilogue_Text_HelpDorneQuieter:
	.string "No.\p"
	.string "Just quieter than I expected.$"

ConvergenceEpilogue_Text_HelpDistance:
	.string "Far off, the legendaries crest once above\n"
	.string "the horizon - free, vast, unreachable.\p"
	.string "Then they are gone, into the world.$"

@ ===========================================================================
@ TEXT - EPILOGUE 3 (TRUE)
@ ===========================================================================
ConvergenceEpilogue_Text_TrueSetting:
	.string "The seals are gone, and nothing fell\n"
	.string "apart. The islands stand quiet and whole\l"
	.string "under an open sky.\p"
	.string "Everyone who made it here is gathered on\n"
	.string "the shore.$"

ConvergenceEpilogue_Text_TrueSollis:
	.string "Your parent would have loved this ending.$"

ConvergenceEpilogue_Text_TrueCass:
	.string "I almost didn't make it in time.$"

ConvergenceEpilogue_Text_TrueDorne:
	.string "Twenty years. A wrong answer and a right\n"
	.string "one.\p"
	.string "I'm glad I didn't get to the wrong one\n"
	.string "first.$"

ConvergenceEpilogue_Text_TrueEira:
	.string "Eira says nothing.\p"
	.string "She nods once. It's enough.$"

ConvergenceEpilogue_Text_TrueMako:
	.string "The Root will remember this too.$"

ConvergenceEpilogue_Text_TrueLens:
	.string "I have so much to write down.$"

ConvergenceEpilogue_Text_TrueDex:
	.string "I need to see all of it. Every island.\p"
	.string "Start over with what we know now.$"

ConvergenceEpilogue_Text_TrueDrenn:
	.string "Don't tell Eira I'm crying.$"

ConvergenceEpilogue_Text_TrueLace:
	.string "What happens now?$"

@ Dagan - three relationship variants, all funny, all ending on the same line.
ConvergenceEpilogue_Text_TrueDagan0:
	.string "I'll be honest, I followed the crowd and\n"
	.string "I'm still not entirely sure whose boat I\l"
	.string "arrived on.\p"
	.string "I have absolutely no idea what happens\n"
	.string "now.\p"
	.string "It's the most interesting I've felt in\n"
	.string "years.$"

ConvergenceEpilogue_Text_TrueDagan1:
	.string "Don't look at me like that - I was\n"
	.string "invited. Mostly. The door was open.\p"
	.string "I have absolutely no idea what happens\n"
	.string "now.\p"
	.string "It's the most interesting I've felt in\n"
	.string "years.$"

ConvergenceEpilogue_Text_TrueDagan2:
	.string "I want it on record that I called this.\n"
	.string "Roughly. In spirit. From a very good seat\l"
	.string "I was not strictly assigned.\p"
	.string "And I have absolutely no idea what\n"
	.string "happens now.\p"
	.string "It's the most interesting I've felt in\n"
	.string "years.$"

@ THE LAST DIALOGUE IN THE GAME. Nothing follows. Credits roll immediately.
ConvergenceEpilogue_Text_TrueCassFinal:
	.string "{PLAYER}. Come on.\p"
	.string "Let's go home.$"

@ ===========================================================================
@ TEXT - POST-GAME FLAVOR
@ ===========================================================================
ConvergenceEpilogue_Text_PostSollis:
	.string "Take your time out there. The islands\n"
	.string "aren't going anywhere now.\p"
	.string "Not anymore.$"

ConvergenceEpilogue_Text_PostCass:
	.string "Whenever you're ready for the next one,\n"
	.string "I'm coming with you.\p"
	.string "Try to keep up.$"

ConvergenceEpilogue_Text_PostDorne:
	.string "I have a great deal of quiet ahead of\n"
	.string "me, and no idea what to do with it.\p"
	.string "I think I'll learn.$"

ConvergenceEpilogue_Text_PostEira:
	.string "Eira watches the horizon, calm for the\n"
	.string "first time in a long while.$"

ConvergenceEpilogue_Text_PostMako:
	.string "The Root grows differently now. Toward\n"
	.string "the light, instead of away from it.$"

ConvergenceEpilogue_Text_PostLens:
	.string "Nine ciphers, eleven islands, and one\n"
	.string "ending nobody had read in two thousand\l"
	.string "years.\p"
	.string "I will be writing for the rest of my\n"
	.string "life.$"

ConvergenceEpilogue_Text_PostDrenn:
	.string "I'm fine. It's the sea air. It does this\n"
	.string "to my eyes.$"

ConvergenceEpilogue_Text_PostLace:
	.string "I still don't know what happens now.\p"
	.string "But it's ours to figure out. That's new.$"

ConvergenceEpilogue_Text_PostDagan:
	.string "Post-game, they're calling it. As if the\n"
	.string "interesting part is over.\p"
	.string "They clearly haven't met me.$"
