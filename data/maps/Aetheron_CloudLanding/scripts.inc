Aetheron_CloudLanding_MapScripts::
	.byte 0

@ ===================================================================
@ Dockhand - the cloud-pier. First outsiders in three years. Wary at
@ first, then welcoming once he reads the player. Talk-initiated.
@ Post-resolution variant: the storm changed this morning.
@ ===================================================================
AetheronCloudLanding_EventScript_Dockhand::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dockhand
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronCloudLanding_EventScript_DockhandResolved
	msgbox AetheronCloudLanding_Text_Dockhand, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCloudLanding_EventScript_DockhandResolved::
	msgbox AetheronCloudLanding_Text_DockhandResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Community member - they can see the whole world and chose not to be
@ part of it.
@ ===================================================================
AetheronCloudLanding_EventScript_CommunityMember::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Villager
	goto_if_set FLAG_AETHERON_RESOLVED, AetheronCloudLanding_EventScript_CommunityResolved
	msgbox AetheronCloudLanding_Text_Community, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

AetheronCloudLanding_EventScript_CommunityResolved::
	msgbox AetheronCloudLanding_Text_CommunityResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Arrival coord trigger. The ascent (KnockUpStream) already sets
@ VAR_AETHERON_PROGRESS to 1 BEFORE warping the player onto this map,
@ so this progress==0 trigger can never fire during normal play. It is
@ left as a safe no-op so a stale/edge save can't strand the player.
@ The arrival atmosphere lives in the dockhand's first-talk above.
@ ===================================================================
AetheronCloudLanding_EventScript_Arrival::
	end

@ Boat-boarding wiring (map-builder owns this; NOT dialogue).
AetheronCloudLanding_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_AETHERON
	goto Pelagios_EventScript_BoardTennyson
	end

@ ===================================================================
@ Text
@ ===================================================================
AetheronCloudLanding_Text_Dockhand:
	.string "You came up the Stream.\p"
	.string "First outsiders in three years. The\n"
	.string "last ones were Covenant.\p"
	.string "…You're not Covenant.\p"
	.string "Come in, then. Mind your footing - the\n"
	.string "cloud holds, but it takes some getting\l"
	.string "used to.$"

AetheronCloudLanding_Text_DockhandResolved:
	.string "The storm changed this morning.\p"
	.string "Still here. But different. Lighter,\n"
	.string "somehow.\p"
	.string "Is that what you did up there?$"

AetheronCloudLanding_Text_Community:
	.string "We can see the whole world from here.\p"
	.string "The islands. The sea. The Covenant\n"
	.string "ships, when they move.\p"
	.string "We chose not to be part of it.\p"
	.string "The Covenant didn't ask us before they\n"
	.string "came.$"

AetheronCloudLanding_Text_CommunityResolved:
	.string "Their installation's gone quiet. We can\n"
	.string "feel it - the air doesn't hum the way\l"
	.string "it did.\p"
	.string "Strange. We spent two years hating that\n"
	.string "sound, and now that it's gone I keep\l"
	.string "listening for it.$"
