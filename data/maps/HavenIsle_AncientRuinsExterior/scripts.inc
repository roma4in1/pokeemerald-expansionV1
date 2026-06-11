HavenIsle_AncientRuinsExterior_MapScripts::
	.byte 0

@ The seal disturbance + Ralts scene play as ON_FRAME entry scripts on
@ HavenIsle_AncientRuinsInterior1 (guaranteed to fire inside the ruins).

AncientRuinsExterior_EventScript_Sign::
	msgbox AncientRuinsExterior_Text_Sign, MSGBOX_SIGN
	end

AncientRuinsExterior_Text_Sign:
	.string "The letters carved here are older\n"
	.string "than the village.\p"
	.string "Most have worn away. What remains\n"
	.string "reads:\p"
	.string "“…SLEEPS BENEATH. LET IT.”$"
