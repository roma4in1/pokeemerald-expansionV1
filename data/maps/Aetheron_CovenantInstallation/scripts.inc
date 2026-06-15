Aetheron_CovenantInstallation_MapScripts::
	.byte 0

@ ===================================================================
@ Covenant Officer Mael (TRAINER_COVENANT_AETHERON_1) - sight trainer.
@ Sight trainers keep trainerbattle as the first command (no namebox on
@ the intro); setspeaker only before the post-battle msgbox.
@ ===================================================================
AetheronCovenantInstallation_EventScript_Officer1::
	trainerbattle_single TRAINER_COVENANT_AETHERON_1, AetheronCovenantInstallation_Text_MaelIntro, AetheronCovenantInstallation_Text_MaelDefeat, AetheronCovenantInstallation_EventScript_Officer1Post
	setspeaker Pelagios_Speaker_Officer
	msgbox AetheronCovenantInstallation_Text_MaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCovenantInstallation_EventScript_Officer1Post::
	setspeaker Pelagios_Speaker_Officer
	msgbox AetheronCovenantInstallation_Text_MaelPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Outer-gate officer - talk-flavor guard (not a battle). Bristles at
@ the breach. Post-resolution he is simply gone (the installation went
@ quiet); the line reflects that.
@ ===================================================================
AetheronCovenantInstallation_EventScript_Officer2::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Officer
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronCovenantInstallation_EventScript_OuterGateResolved
	msgbox AetheronCovenantInstallation_Text_OuterGate, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCovenantInstallation_EventScript_OuterGateResolved::
	msgbox AetheronCovenantInstallation_Text_OuterGateResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Approach trigger (10,16)/(11,16) at VAR_AETHERON_PROGRESS == 2 (only
@ between Gym 1 and Gym 2; Gym 2 advances to progress 3 and disarms it).
@ First sight of the facility up close. Narration only - the panel
@ examine inside is what sets FLAG_AETHERON_INSTALLATION_FOUND.
@ ===================================================================
AetheronCovenantInstallation_EventScript_Approach::
	lockall
	setspeaker 0
	delay 20
	msgbox AetheronCovenantInstallation_Text_Approach, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronCovenantInstallation_Text_MaelIntro:
	.string "Installation is restricted.\n"
	.string "Covenant personnel only.$"

AetheronCovenantInstallation_Text_MaelDefeat:
	.string "Unauthorized. Unauthorized.$"

AetheronCovenantInstallation_Text_MaelPost:
	.string "…Breach logged.$"

AetheronCovenantInstallation_Text_OuterGate:
	.string "You don't have clearance for this site.\p"
	.string "I'm required to log your presence and\n"
	.string "report it up the chain.\p"
	.string "Whether anyone up the chain still\n"
	.string "answers… that's above my pay grade.$"

AetheronCovenantInstallation_Text_OuterGateResolved:
	.string "The post is empty. A logbook lies open\n"
	.string "on the rail, its last entry half\l"
	.string "finished.\p"
	.string "Whoever stood guard here left in a\n"
	.string "hurry.$"

AetheronCovenantInstallation_Text_Approach:
	.string "The installation rises ahead of you -\n"
	.string "grey panels, humming conduits, a low\l"
	.string "electric drone you can feel in your\l"
	.string "teeth.\p"
	.string "It was built to take something from this\n"
	.string "place. You can tell that just by\l"
	.string "looking at it.$"
