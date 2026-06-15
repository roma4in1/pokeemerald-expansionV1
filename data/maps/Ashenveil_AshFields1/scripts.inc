Ashenveil_AshFields1_MapScripts::
	.byte 0

@ Phantom Lantern gate. The ash hangs thick enough to hide the path north to
@ the dead city. With the lantern the way reveals itself (and the player has
@ reached the dead city - progress 3). Without it, the player is turned back.
@ NOTE (deferred to systems): a true field effect that LIFTS the impassable
@ OVERGROW/ASHWALL tiles is not built; the impassable layout tiles plus this
@ trigger are the gate. The lantern is obtained at the outpost (progress 2)
@ BEFORE this point, so the no-lantern branch is defensive only - no softlock.
AshenveilAshFields1_EventScript_LanternGate::
	lockall
	checkitem ITEM_PHANTOM_LANTERN
	goto_if_eq VAR_RESULT, FALSE, AshenveilAshFields1_EventScript_LanternGateBlocked
	msgbox AshenveilAshFields1_Text_LanternReveals, MSGBOX_DEFAULT
	closemessage
	setvar VAR_ASHENVEIL_PROGRESS, 3
	releaseall
	end

AshenveilAshFields1_EventScript_LanternGateBlocked::
	msgbox AshenveilAshFields1_Text_AshTooThick, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, AshenveilAshFields1_Movement_BounceBack
	waitmovement 0
	releaseall
	end

AshenveilAshFields1_EventScript_ChildShoe::
	lockall
	msgbox AshenveilAshFields1_Text_ChildShoe, MSGBOX_DEFAULT
	releaseall
	end

AshenveilAshFields1_EventScript_DoorHandle::
	lockall
	msgbox AshenveilAshFields1_Text_DoorHandle, MSGBOX_DEFAULT
	releaseall
	end

AshenveilAshFields1_EventScript_StreetSign::
	lockall
	msgbox AshenveilAshFields1_Text_StreetSign, MSGBOX_DEFAULT
	releaseall
	end

AshenveilAshFields1_EventScript_Footprints::
	lockall
	msgbox AshenveilAshFields1_Text_Footprints, MSGBOX_DEFAULT
	releaseall
	end

AshenveilAshFields1_Movement_BounceBack:
	walk_down
	step_end

AshenveilAshFields1_Text_LanternReveals::
	.string "You raise the Phantom Lantern.\p"
	.string "The ash thins where its light falls.\n"
	.string "A path opens ahead - the road into\l"
	.string "the city that was here.$"

AshenveilAshFields1_Text_AshTooThick::
	.string "The ash is too thick. You can't see\n"
	.string "where the safe path is.$"

AshenveilAshFields1_Text_ChildShoe::
	.string "A small shoe. Red. The kind that\n"
	.string "wears out fast because children run.$"

AshenveilAshFields1_Text_DoorHandle::
	.string "A brass handle. Someone polished\n"
	.string "this regularly. Force of habit.$"

AshenveilAshFields1_Text_StreetSign::
	.string "RIVER LANE. There is no river here.\n"
	.string "There wasn't one when this was built\l"
	.string "either. Someone named it hopefully.$"

AshenveilAshFields1_Text_Footprints::
	.string "Fresh tracks in the ash. Heading\n"
	.string "toward the memorial. Purposeful.\l"
	.string "Not lost.$"
