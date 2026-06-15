Ashenveil_DeadCity_Ruins2_MapScripts::
	.byte 0

@ The real Covenant documents - Cass's copy, deliberately left here.
@ Sets FLAG_ASHENVEIL_COVENANT_DOCS_FOUND.
AshenveilDeadCityRuins2_EventScript_RealDocuments::
	lockall
	msgbox AshenveilDeadCityRuins2_Text_RealDocuments, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_ASHENVEIL_COVENANT_DOCS_FOUND
	msgbox AshenveilDeadCityRuins2_Text_NotDestroyed, MSGBOX_DEFAULT
	releaseall
	end

AshenveilDeadCityRuins2_Text_RealDocuments::
	.string "A folder, recently set here. The\n"
	.string "pages are old, but the dust on them\l"
	.string "is not.\p"
	.string "ASHENVEIL - INTERNAL MEMO\p"
	.string "The geological instability was caused\n"
	.string "by our seal-siphoning apparatus.\p"
	.string "The evacuation boats were deliberately\n"
	.string "under-provisioned.\p"
	.string "The decision was made at the highest\n"
	.string "level.\p"
	.string "The survivors who reached the\n"
	.string "receiving port have been relocated and\l"
	.string "the incident classified.\p"
	.string "The Warden's field reports from this\n"
	.string "site have been suppressed.\p"
	.string "This memo is classified at highest\n"
	.string "level. Destroy after reading.$"

AshenveilDeadCityRuins2_Text_NotDestroyed::
	.string "It was not destroyed.\p"
	.string "Someone copied it by hand and left\n"
	.string "the copy here. Where it would be\l"
	.string "found.$"
