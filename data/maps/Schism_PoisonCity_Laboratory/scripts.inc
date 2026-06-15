Schism_PoisonCity_Laboratory_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Lab assistant - foyer. Points to Murk, hints at Drenn upstairs.
@ -------------------------------------------------------------------
SchismPoisonCityLaboratory_EventScript_LabAssistant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Researcher
	goto_if_set FLAG_SCHISM_GYM3_CLEAR, SchismPoisonCityLaboratory_EventScript_LabAssistantBeaten
	msgbox SchismPoisonCityLaboratory_Text_LabAssistant, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratory_EventScript_LabAssistantBeaten::
	msgbox SchismPoisonCityLaboratory_Text_LabAssistantBeaten, MSGBOX_DEFAULT
	release
	end

SchismPoisonCityLaboratory_Text_LabAssistant:
	.string "MURK runs the research floor.\n"
	.string "Mind the vats - the platforms are\l"
	.string "there for a reason.\p"
	.string "DRENN's office is upstairs. You\n"
	.string "can see him through the glass.\p"
	.string "He's been at those documents for\n"
	.string "weeks. Barely sleeps.$"

SchismPoisonCityLaboratory_Text_LabAssistantBeaten:
	.string "You got past MURK. Few do.\p"
	.string "DRENN's waiting upstairs.\n"
	.string "Whatever you say to him - say it\l"
	.string "carefully.\p"
	.string "He's been alone with DORNE's\n"
	.string "documents too long.$"
