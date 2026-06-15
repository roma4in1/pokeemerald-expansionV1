Thalvern_CovenantSite_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_CovenantSite_OnTransition
	.byte 0

@ ===================================================================
@ Once Numa Vess has been confronted, she has left the site (unhurried,
@ unimpressed). Her object has no hide flag in map.json, so remove it
@ every load after FLAG_NUMA_VESS_CONFRONTED.
@ ===================================================================
Thalvern_CovenantSite_OnTransition:
	goto_if_unset FLAG_NUMA_VESS_CONFRONTED, Thalvern_CovenantSite_OnTransition_Done
	removeobject LOCALID_THALVERN_NUMA_VESS
Thalvern_CovenantSite_OnTransition_Done:
	end

@ ===================================================================
@ Director Numa Vess - optional confrontation. She REFUSES to battle,
@ delivers her speech, and leaves unhurried. NO trainerbattle. Sets
@ FLAG_NUMA_VESS_CONFRONTED and walks off (then removed by transition).
@ ===================================================================
ThalvernCovenantSite_EventScript_NumaVess::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Numa
	goto_if_set FLAG_NUMA_VESS_CONFRONTED, ThalvernCovenantSite_EventScript_NumaGone
	msgbox ThalvernCovenantSite_Text_Numa, MSGBOX_DEFAULT
	closemessage
	setspeaker 0
	setflag FLAG_NUMA_VESS_CONFRONTED
	@ She leaves - walks away and out of sight. No hurry.
	applymovement LOCALID_THALVERN_NUMA_VESS, ThalvernCovenantSite_Movement_NumaLeave
	waitmovement 0
	removeobject LOCALID_THALVERN_NUMA_VESS
	release
	end

ThalvernCovenantSite_EventScript_NumaGone::
	@ Safety fallback (object should be removed by transition).
	setspeaker 0
	release
	end

ThalvernCovenantSite_Movement_NumaLeave:
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	set_invisible
	step_end

@ ===================================================================
@ Covenant researchers - flavor.
@ ===================================================================
ThalvernCovenantSite_EventScript_Researcher1::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	msgbox ThalvernCovenantSite_Text_Researcher1, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernCovenantSite_EventScript_Researcher2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	msgbox ThalvernCovenantSite_Text_Researcher2, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Covenant documents - examinable lore about the Covenant's true plans.
@ ===================================================================
ThalvernCovenantSite_EventScript_Documents1::
	lockall
	msgbox ThalvernCovenantSite_Text_Documents1, MSGBOX_SIGN
	releaseall
	end

ThalvernCovenantSite_EventScript_Documents2::
	lockall
	msgbox ThalvernCovenantSite_Text_Documents2, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernCovenantSite_Text_Numa:
	.string "You're the WARDEN's child.\p"
	.string "I know your file.\p"
	.string "I'm not going to fight you - that's\n"
	.string "not how I work.\p"
	.string "I don't dirty my hands. I file\n"
	.string "reports. I commission studies. I\l"
	.string "wait.\p"
	.string "I waited three years for this site\n"
	.string "to become accessible. I can wait\l"
	.string "longer.\p"
	.string "The seal will weaken eventually.\n"
	.string "Everything does.$"

ThalvernCovenantSite_Text_Researcher1:
	.string "The Director files everything. Every\n"
	.string "reading, every setback.\p"
	.string "She says a failure on paper is just\n"
	.string "a study that hasn't concluded yet.$"

ThalvernCovenantSite_Text_Researcher2:
	.string "We were so close to a containment\n"
	.string "protocol.\p"
	.string "If you reinforce that seal, three\n"
	.string "years of work goes in a drawer.\p"
	.string "...The Director won't even raise\n"
	.string "her voice. That's the unsettling\l"
	.string "part.$"

ThalvernCovenantSite_Text_Documents1:
	.string "A Covenant brief: PROJECT TIDEHOLD.\p"
	.string "...objective: controlled extraction\n"
	.string "of seal-energy from the THALVERN\l"
	.string "site for application elsewhere...\p"
	.string "The same thing that killed\n"
	.string "ASHENVEIL, dressed in a project\l"
	.string "name.$"

ThalvernCovenantSite_Text_Documents2:
	.string "A signed memo: ...the legendary's\n"
	.string "cooperation is not required. Its\l"
	.string "containment is.\p"
	.string "Filed. Initialed. N.V.$"
