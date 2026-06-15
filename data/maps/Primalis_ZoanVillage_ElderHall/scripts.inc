Primalis_ZoanVillage_ElderHall_MapScripts::
	.byte 0

@ ===================================================================
@ Hall attendant - guards the foyer of the Elder Hall. Doubles as the
@ community historian's helper; gates nothing (the real gating is the
@ Mako scene inside), but colors the threshold.
@ ===================================================================
PrimalisElderHall_EventScript_Attendant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Attendant
	goto_if_set FLAG_PRIMALIS_ORAL_HISTORY_HEARD, PrimalisElderHall_EventScript_AttendantPostHistory
	goto_if_set FLAG_PRIMALIS_GYM2_CLEAR, PrimalisElderHall_EventScript_AttendantTrusting
	msgbox PrimalisElderHall_Text_Attendant, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHall_EventScript_AttendantTrusting::
	msgbox PrimalisElderHall_Text_AttendantTrusting, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHall_EventScript_AttendantPostHistory::
	msgbox PrimalisElderHall_Text_AttendantPostHistory, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisElderHall_Text_Attendant:
	.string "This is the Elder Hall. The history is\n"
	.string "kept here.\p"
	.string "Not on paper. In MAKO. In the elders\n"
	.string "before him, back and back.\p"
	.string "He does not tell it to outsiders. Do\n"
	.string "not take it personally.$"

PrimalisElderHall_Text_AttendantTrusting:
	.string "SCALE sent you up? Then go in.\p"
	.string "MAKO will weigh you. He weighs\n"
	.string "everyone. He has been wrong before -\l"
	.string "but not often, and never kindly.$"

PrimalisElderHall_Text_AttendantPostHistory:
	.string "He told you. The whole of it.\p"
	.string "I have served in this hall for thirty\n"
	.string "years and I have heard him tell it\l"
	.string "twice. You are the third.$"
