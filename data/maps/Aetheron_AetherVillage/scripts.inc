Aetheron_AetherVillage_MapScripts::
	.byte 0

@ ===================================================================
@ Gym 1 - Gale (Flying). Community elder. Talk-initiated
@ (TRAINER_TYPE_NONE). Victory sets FLAG_AETHERON_GYM1_CLEAR + progress
@ 2; badge NARRATIVE-ONLY (engine badge flags exhausted at
@ FLAG_BADGE08_GET) - fanfare + "received the STORM BADGE" text + flag
@ only, NO FLAG_BADGE*_GET. TM: no Air Slash TM in this project's set,
@ so Gale gives TM Aerial Ace (a Flying move) and lampshades the swap.
@ ===================================================================
AetheronAetherVillage_EventScript_Gale::
	goto_if_defeated TRAINER_LEADER_AETHERON_GALE, AetheronAetherVillage_EventScript_GaleDefeated
	setspeaker Pelagios_Speaker_Gale
	trainerbattle_single TRAINER_LEADER_AETHERON_GALE, AetheronAetherVillage_Text_GaleIntro, AetheronAetherVillage_Text_GaleDefeat, AetheronAetherVillage_EventScript_GaleVictory
	end

AetheronAetherVillage_EventScript_GaleVictory::
	setspeaker Pelagios_Speaker_Gale
	msgbox AetheronAetherVillage_Text_GalePostBattle, MSGBOX_DEFAULT
	setspeaker 0
	playfanfare MUS_OBTAIN_BADGE
	message AetheronAetherVillage_Text_ReceivedStormBadge
	waitmessage
	waitfanfare
	setflag FLAG_AETHERON_GYM1_CLEAR
	setvar VAR_AETHERON_PROGRESS, 2
	setspeaker Pelagios_Speaker_Gale
	msgbox AetheronAetherVillage_Text_GaleDirections, MSGBOX_DEFAULT
	setspeaker 0
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Gale
	msgbox AetheronAetherVillage_Text_GaleTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronAetherVillage_EventScript_GaleDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Gale
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronAetherVillage_EventScript_GaleResolved
	msgbox AetheronAetherVillage_Text_GalePostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronAetherVillage_EventScript_GaleResolved::
	msgbox AetheronAetherVillage_Text_GaleResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Gym 2 - Arc (Electric). Covenant defector who stayed to watch from
@ the inside. Talk-initiated. Gated on FLAG_AETHERON_GYM1_CLEAR (he
@ will not waste your time before Gale). Pre-battle = community-defector
@ confession. "Hands player a schematic" after battle is LORE FLAVOR
@ ONLY (no item given - dialogue/narration). Victory sets
@ FLAG_AETHERON_GYM2_CLEAR + progress 3; TM Thunderbolt (as briefed).
@ ===================================================================
AetheronAetherVillage_EventScript_Arc::
	goto_if_defeated TRAINER_LEADER_AETHERON_ARC, AetheronAetherVillage_EventScript_ArcDefeated
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Arc
	goto_if_unset FLAG_AETHERON_GYM1_CLEAR, AetheronAetherVillage_EventScript_ArcNotYet
	msgbox AetheronAetherVillage_Text_ArcPreamble, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_LEADER_AETHERON_ARC, AetheronAetherVillage_Text_ArcDefeat
	goto AetheronAetherVillage_EventScript_ArcVictory
	end

AetheronAetherVillage_EventScript_ArcNotYet::
	msgbox AetheronAetherVillage_Text_ArcNotYet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronAetherVillage_EventScript_ArcVictory::
	setspeaker Pelagios_Speaker_Arc
	msgbox AetheronAetherVillage_Text_ArcPostBattle, MSGBOX_DEFAULT
	setspeaker 0
	@ The schematic - lore flavor only, no actual item.
	msgbox AetheronAetherVillage_Text_ArcSchematic, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message AetheronAetherVillage_Text_ReceivedVoltBadge
	waitmessage
	waitfanfare
	setflag FLAG_AETHERON_GYM2_CLEAR
	setvar VAR_AETHERON_PROGRESS, 3
	giveitem ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setspeaker Pelagios_Speaker_Arc
	msgbox AetheronAetherVillage_Text_ArcTM, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronAetherVillage_EventScript_ArcDefeated::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Arc
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronAetherVillage_EventScript_ArcResolved
	msgbox AetheronAetherVillage_Text_ArcPostBadge, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronAetherVillage_EventScript_ArcResolved::
	msgbox AetheronAetherVillage_Text_ArcResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ First Cass sighting. Talk-script on the village-square Cass object
@ (hide flag FLAG_AETHERON_CASS_SEEN). If the player somehow walks up
@ and talks before the coord-trigger scene fires, route to the same
@ scene so it can't be examined as a normal NPC.
@ ===================================================================
AetheronAetherVillage_EventScript_CassSighting::
	goto AetheronAetherVillage_EventScript_CassSightingScene
	end

@ ===================================================================
@ Cass sighting scene - coord triggers (11,17)/(12,17) at progress 1.
@ FIRES ONCE: Cass is across the square in Covenant uniform, looks at
@ the player deliberately, then turns and walks into the alley and is
@ gone. No dialogue - just the sighting. removeobject + setflag
@ FLAG_AETHERON_CASS_SEEN (the object's hide flag) so she never returns
@ here. The progress==1 gate plus the hide flag make it strictly once.
@ ===================================================================
AetheronAetherVillage_EventScript_CassSightingScene::
	lockall
	setspeaker 0
	delay 30
	@ She notices you - turns to face down across the square.
	applymovement LOCALID_AETHERON_CASS_VILLAGE, AetheronAetherVillage_Movement_CassFacePlayer
	waitmovement 0
	delay 50
	@ A long look. Then the player notices her too.
	applymovement OBJ_EVENT_ID_PLAYER, AetheronAetherVillage_Movement_PlayerLookUp
	waitmovement 0
	delay 50
	msgbox AetheronAetherVillage_Text_CassSighting, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ She turns away and walks into the alley.
	applymovement LOCALID_AETHERON_CASS_VILLAGE, AetheronAetherVillage_Movement_CassToAlley
	waitmovement 0
	removeobject LOCALID_AETHERON_CASS_VILLAGE
	setflag FLAG_AETHERON_CASS_SEEN
	delay 20
	msgbox AetheronAetherVillage_Text_CassSightingAfter, MSGBOX_DEFAULT
	releaseall
	end

AetheronAetherVillage_Movement_CassFacePlayer:
	face_down
	step_end

AetheronAetherVillage_Movement_PlayerLookUp:
	face_up
	step_end

@ From (20,5) up the alley (north). (19,5) is wall, so (20,x) is a
@ corridor; two steps north then she is off behind the buildings.
AetheronAetherVillage_Movement_CassToAlley:
	walk_up
	walk_up
	step_end

@ ===================================================================
@ Installation-view sign - the Covenant facility on the horizon, wrong
@ and angular against the cloud.
@ ===================================================================
AetheronAetherVillage_EventScript_InstallationView::
	lockall
	setspeaker 0
	msgbox AetheronAetherVillage_Text_InstallationView, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - Gale
@ ===================================================================
AetheronAetherVillage_Text_GaleIntro:
	.string "You came up the Stream. That alone\n"
	.string "tells me something about you.\p"
	.string "The Covenant came up the Stream too.\p"
	.string "The difference between you matters.\p"
	.string "Show me the difference.$"

AetheronAetherVillage_Text_GaleDefeat:
	.string "There it is.$"

AetheronAetherVillage_Text_GalePostBattle:
	.string "There it is.\p"
	.string "The Installation is east. You'll need\n"
	.string "to go through it to reach the peak.\p"
	.string "Commander VOSS won't step aside.\p"
	.string "But there's someone up there who\n"
	.string "might.$"

AetheronAetherVillage_Text_ReceivedStormBadge:
	.string "{PLAYER} received the STORM BADGE.$"

AetheronAetherVillage_Text_GaleDirections:
	.string "Take this with you. It's a recording of\n"
	.string "the sky's own strike.$"

AetheronAetherVillage_Text_GaleTM:
	.string "There's no AIR SLASH on a disc up here,\n"
	.string "so this is AERIAL ACE - the cut a bird\l"
	.string "makes through the wind. Same idea.\p"
	.string "It never misses. Nothing up here does.$"

AetheronAetherVillage_Text_GalePostBadge:
	.string "Go on east. The longer you stand here,\n"
	.string "the longer VOSS has to prepare.$"

AetheronAetherVillage_Text_GaleResolved:
	.string "The crackling eased this morning. The\n"
	.string "children noticed before any of us did.\p"
	.string "You gave the sky back some of its\n"
	.string "quiet. We won't forget that.$"

@ ===================================================================
@ Text - Arc
@ ===================================================================
AetheronAetherVillage_Text_ArcNotYet:
	.string "Beat GALE first. He's the heart of this\n"
	.string "place - if you can't get past him, the\l"
	.string "Installation will eat you alive.\p"
	.string "Come back when you've earned the right\n"
	.string "to ask me for help.$"

AetheronAetherVillage_Text_ArcPreamble:
	.string "I was a technician. Installation team.\p"
	.string "I helped run the cables, calibrate the\n"
	.string "extractors. I knew what we were doing\l"
	.string "to STORMVEIL.\p"
	.string "I told myself the yield numbers\n"
	.string "justified it.\p"
	.string "The yield numbers are very large.\p"
	.string "The justification got harder every\n"
	.string "month.\p"
	.string "I built the installation. I know every\n"
	.string "wire, every extraction point.\p"
	.string "I also know what it's doing.\p"
	.string "Beat me. Then I'll tell you everything\n"
	.string "you need to know to shut it down.$"

AetheronAetherVillage_Text_ArcDefeat:
	.string "Good. You're ready to hear it.$"

AetheronAetherVillage_Text_ArcPostBattle:
	.string "The main extraction core is on the\n"
	.string "peak.\p"
	.string "The apparatus has a manual override -\n"
	.string "it'll destroy itself if triggered from\l"
	.string "inside.\p"
	.string "VOSS knows this. He'll stop you.$"

AetheronAetherVillage_Text_ArcSchematic:
	.string "ARC sketched the core's layout on the\n"
	.string "back of an old yield sheet and pressed\l"
	.string "it into your hand.\p"
	.string "(The override is where the cables\n"
	.string "converge. You'll know it when you see\l"
	.string "it.)$"

AetheronAetherVillage_Text_ReceivedVoltBadge:
	.string "{PLAYER} received the VOLT BADGE.$"

AetheronAetherVillage_Text_ArcTM:
	.string "And take this. THUNDERBOLT - clean\n"
	.string "current, no waste. I tuned it myself,\l"
	.string "back when I still tuned things for\l"
	.string "them.$"

AetheronAetherVillage_Text_ArcPostBadge:
	.string "The override's in your head now. Go up\n"
	.string "there and end it.\p"
	.string "I'll be here. Watching, like always.\n"
	.string "Only this time I want to see it stop.$"

AetheronAetherVillage_Text_ArcResolved:
	.string "Two years I sat inside that thing,\n"
	.string "memorizing how to break it, never\l"
	.string "quite daring to.\p"
	.string "You walked in and broke it.\p"
	.string "I think I can go home now. I'd\n"
	.string "forgotten the village was still up\l"
	.string "there waiting.$"

@ ===================================================================
@ Text - Cass sighting (narration)
@ ===================================================================
AetheronAetherVillage_Text_CassSighting:
	.string "Across the square, a figure in a grey\n"
	.string "Covenant coat stands very still.\p"
	.string "Cass.\p"
	.string "She sees you. She holds your eyes for\n"
	.string "a long moment - long enough that it\l"
	.string "means something.\p"
	.string "Then she looks away. On purpose.$"

AetheronAetherVillage_Text_CassSightingAfter:
	.string "She slips into the alley between the\n"
	.string "low houses and is gone.\p"
	.string "She didn't run. She just… left.$"

@ ===================================================================
@ Text - installation view
@ ===================================================================
AetheronAetherVillage_Text_InstallationView:
	.string "East, on the cloud-horizon, the\n"
	.string "Covenant installation hunches against\l"
	.string "the storm.\p"
	.string "Every line of it is angular and hard -\n"
	.string "wrong, somehow, against a sky that has\l"
	.string "no straight edges of its own.$"
