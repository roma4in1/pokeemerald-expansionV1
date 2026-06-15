Ashenveil_DeadCity_Ruins1_MapScripts::
	.byte 0

@ The official, sanitized Covenant evacuation report.
AshenveilDeadCityRuins1_EventScript_OfficialReport::
	lockall
	msgbox AshenveilDeadCityRuins1_Text_OfficialReport, MSGBOX_DEFAULT
	releaseall
	end

AshenveilDeadCityRuins1_Text_OfficialReport::
	.string "A Covenant placard, bolted to the\n"
	.string "wall and untouched for twenty years.\p"
	.string "ASHENVEIL INCIDENT - CLASSIFIED\p"
	.string "Cause: unexpected geological\n"
	.string "instability.\p"
	.string "Evacuation: completed, all residents\n"
	.string "accounted for.\p"
	.string "Casualties: none.\p"
	.string "Status: uninhabitable, access\n"
	.string "restricted.$"
