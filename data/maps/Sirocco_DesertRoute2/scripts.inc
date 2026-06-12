Sirocco_DesertRoute2_MapScripts::
	.byte 0

@ Enforcer Wex - TRAINER_GILTCLAW_SIROCCO_2 (sight-initiated).
@ The brief's "blocks access until Gym 1" is narrative: he guards the
@ ruins road, and once beaten he stays beaten - Dex's vouching is what
@ keeps the Gilt Claw from replacing him.
SiroccoDesertRoute2_EventScript_EnforcerWex::
	trainerbattle_single TRAINER_GILTCLAW_SIROCCO_2, SiroccoDesertRoute2_Text_WexIntro, SiroccoDesertRoute2_Text_WexDefeat
	setspeaker Pelagios_Speaker_Wex
	msgbox SiroccoDesertRoute2_Text_WexPost, MSGBOX_AUTOCLOSE
	end

SiroccoDesertRoute2_Text_WexIntro:
	.string "The ruins are Gilt Claw property.\n"
	.string "DAGAN's orders.\p"
	.string "Nobody goes past here without\n"
	.string "clearance.$"

SiroccoDesertRoute2_Text_WexDefeat:
	.string "…Huh.$"

SiroccoDesertRoute2_Text_WexPost:
	.string "You have clearance now,\n"
	.string "I suppose.$"
