HavenIsle_ElderHouse_MapScripts::
	.byte 0

HavenIsleElderHouse_EventScript_Elder::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Elder
	goto_if_set FLAG_HAVEN_RUINS_VISITED, HavenIsleElderHouse_EventScript_ElderAfterRuins
	msgbox HavenIsleElderHouse_Text_ElderHistory, MSGBOX_DEFAULT
	release
	end

HavenIsleElderHouse_EventScript_ElderAfterRuins::
	msgbox HavenIsleElderHouse_Text_ElderAfterRuins, MSGBOX_DEFAULT
	release
	end

HavenIsleElderHouse_Text_ElderHistory:
	.string "Haven Isle. Hmph. The name is\n"
	.string "newer than I am.\p"
	.string "The old charts call this place\n"
	.string "FIRSTMOOR - the first mooring.\l"
	.string "The first place anyone in the\l"
	.string "archipelago dropped anchor.\p"
	.string "And those ruins up north? Older\n"
	.string "still. Older than the charts.\l"
	.string "Older than the NAME for charts.\p"
	.string "Folk built this village in their\n"
	.string "shadow and then spent nine\l"
	.string "generations agreeing not to\l"
	.string "wonder about them.\p"
	.string "The WARDEN wondered. Quietly.\n"
	.string "I used to see lantern-light heading\l"
	.string "up that route past midnight.\p"
	.string "Never asked. One regrets the\n"
	.string "questions one was too polite to ask.$"

HavenIsleElderHouse_Text_ElderAfterRuins:
	.string "So. The ruins spoke to you.\p"
	.string "Don't look so startled. The whole\n"
	.string "island felt the shiver, and you're\l"
	.string "the only one who walked TOWARD it.\p"
	.string "The WARDEN's child, sure enough.\p"
	.string "Listen, then. Whatever sleeps under\n"
	.string "Firstmoor has slept through storm\l"
	.string "and war and worse.\p"
	.string "If it's waking now, it isn't waking\n"
	.string "on its own.\p"
	.string "Somebody, somewhere, is pulling\n"
	.string "at threads best left alone.$"
