Ashenveil_Greyport_Outpost_Interior_MapScripts::
	.byte 0

@ The tea cup. Cass was here, recently. Never stated. Narration only.
AshenveilGreyportOutpostInterior_EventScript_TeaCup::
	lockall
	msgbox AshenveilGreyportOutpostInterior_Text_TeaCup, MSGBOX_DEFAULT
	releaseall
	end

AshenveilGreyportOutpostInterior_Text_TeaCup::
	.string "A half-finished cup of tea.\p"
	.string "Still faintly warm. Someone was here\n"
	.string "recently.$"
