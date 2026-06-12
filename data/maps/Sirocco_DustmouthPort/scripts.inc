Sirocco_DustmouthPort_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Arrival cutscene - fires once, when VAR_SIROCCO_PROGRESS == 0.
@ The dock enforcer saunters down from the watch post, runs the
@ "water tax" routine, and waves the player through. The camera then
@ pans north over the dust-choked town toward the dry hills.
@ Sets FLAG_SIROCCO_ARRIVED and VAR_SIROCCO_PROGRESS = 1.
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_Arrival::
	lockall
	delay 16
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	msgbox SiroccoDustmouthPort_Text_ArrivalDust, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_SIROCCO_DOCK_ENFORCER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SIROCCO_DOCK_ENFORCER, SiroccoDustmouthPort_Movement_EnforcerApproach
	waitmovement 0
	setspeaker Pelagios_Speaker_Enforcer
	msgbox SiroccoDustmouthPort_Text_ArrivalWaterTax, MSGBOX_DEFAULT
	setspeaker 0
	msgbox SiroccoDustmouthPort_Text_ArrivalBeat, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Enforcer
	msgbox SiroccoDustmouthPort_Text_ArrivalJustKidding, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SIROCCO_DOCK_ENFORCER, SiroccoDustmouthPort_Movement_EnforcerReturn
	waitmovement 0
	setflag FLAG_SIROCCO_ARRIVED
	delay 20
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoDustmouthPort_Movement_CameraPanUp
	waitmovement 0
	delay 40
	msgbox SiroccoDustmouthPort_Text_ArrivalView, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, SiroccoDustmouthPort_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setvar VAR_SIROCCO_PROGRESS, 1
	releaseall
	end

SiroccoDustmouthPort_Movement_EnforcerApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	face_down
	step_end

SiroccoDustmouthPort_Movement_EnforcerReturn:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

SiroccoDustmouthPort_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SiroccoDustmouthPort_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

@ -------------------------------------------------------------------
@ Dock enforcer (watch post NPC, post-arrival flavor)
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_DockEnforcer::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Enforcer
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPort_EventScript_DockEnforcerResolved
	msgbox SiroccoDustmouthPort_Text_DockEnforcer, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPort_EventScript_DockEnforcerResolved::
	msgbox SiroccoDustmouthPort_Text_DockEnforcerResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Desperate local - explains the water situation, quietly
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_DesperateLocal::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Citizen
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPort_EventScript_LocalResolved
	msgbox SiroccoDustmouthPort_Text_DesperateLocal, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPort_EventScript_LocalResolved::
	msgbox SiroccoDustmouthPort_Text_LocalResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Merchant - the Gilt Claw tariff means he can't even sell to you
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_Merchant::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Merchant
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPort_EventScript_MerchantResolved
	msgbox SiroccoDustmouthPort_Text_Merchant, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPort_EventScript_MerchantResolved::
	msgbox SiroccoDustmouthPort_Text_MerchantResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Sailor - points the player toward Dex's camp
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_Sailor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Sailor
	msgbox SiroccoDustmouthPort_Text_Sailor, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Gilt Claw water distribution point
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_WaterDistributor::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_GiltClaw
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPort_EventScript_DistributorResolved
	msgbox SiroccoDustmouthPort_Text_WaterDistributor, MSGBOX_DEFAULT
	release
	end

SiroccoDustmouthPort_EventScript_DistributorResolved::
	msgbox SiroccoDustmouthPort_Text_DistributorResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_NorthSign::
	msgbox SiroccoDustmouthPort_Text_NorthSign, MSGBOX_SIGN
	end

SiroccoDustmouthPort_EventScript_BerthSign::
	msgbox SiroccoDustmouthPort_Text_BerthSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ The Tennyson - sail back to earlier islands (chained yes/no, per
@ the established harbor pattern: boarding via bg_events only).
@ -------------------------------------------------------------------
SiroccoDustmouthPort_EventScript_Tennyson::
	lockall
	setvar VAR_TEMP_1, PELAGIOS_ISLAND_SIROCCO
	goto_if_set FLAG_SIROCCO_RESOLVED, SiroccoDustmouthPort_EventScript_TennysonResolved
	msgbox SiroccoDustmouthPort_Text_TennysonAtBerth, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

SiroccoDustmouthPort_EventScript_TennysonResolved::
	msgbox SiroccoDustmouthPort_Text_TennysonResolved, MSGBOX_DEFAULT
	goto Pelagios_EventScript_BoardTennyson

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
SiroccoDustmouthPort_Text_ArrivalDust:
	.string "The heat hits you before your boots\n"
	.string "hit the boards.\p"
	.string "Dust on the railings. Dust on the\n"
	.string "barrels. Dust on the people.\p"
	.string "Every water barrel on the dock\n"
	.string "wears the same brand: a gilded\l"
	.string "claw, stamped deep.$"

SiroccoDustmouthPort_Text_ArrivalWaterTax:
	.string "Gilt Claw port. Water tax is three\n"
	.string "hundred for non-members.\p"
	.string "Don't have it? The harbor's that\n"
	.string "way.$"

SiroccoDustmouthPort_Text_ArrivalBeat:
	.string "He lets the silence sit exactly\n"
	.string "as long as it takes your hand to\l"
	.string "move toward your bag.$"

SiroccoDustmouthPort_Text_ArrivalJustKidding:
	.string "Just kidding. We let travelers\n"
	.string "through.\p"
	.string "DAGAN says a dry mouth tells no\n"
	.string "tales.\p"
	.string "Heh. Welcome to DUSTMOUTH.$"

SiroccoDustmouthPort_Text_ArrivalView:
	.string "Beyond the town's sagging rooftops,\n"
	.string "the desert climbs away in waves of\l"
	.string "cracked earth.\p"
	.string "Somewhere past those dunes lies\n"
	.string "MIRADEN OASIS - the water this\l"
	.string "whole island hangs from.\p"
	.string "From here, you can't see anything\n"
	.string "green at all.$"

SiroccoDustmouthPort_Text_DockEnforcer:
	.string "Still here? Still watched.\p"
	.string "Don't take it personally. We watch\n"
	.string "everyone. It's the only thing\l"
	.string "that's free on this island.$"

SiroccoDustmouthPort_Text_DockEnforcerResolved:
	.string "No boss. No orders. No water tax.\p"
	.string "…You'd think that would feel like\n"
	.string "a holiday.\p"
	.string "Mostly it feels like standing\n"
	.string "around in the sun.$"

SiroccoDustmouthPort_Text_DesperateLocal:
	.string "Don't drink the free water they\n"
	.string "give out.\p"
	.string "It's fine - I think it's fine -\n"
	.string "but there's never quite enough.\p"
	.string "There's never quite enough of\n"
	.string "anything anymore.$"

SiroccoDustmouthPort_Text_LocalResolved:
	.string "They say the oasis is rising.\n"
	.string "Actually rising.\p"
	.string "I filled a jug this morning and\n"
	.string "nobody stopped me.\p"
	.string "I keep waiting for someone to\n"
	.string "stop me.$"

SiroccoDustmouthPort_Text_Merchant:
	.string "Gilt Claw Supply Fee. Twenty\n"
	.string "percent over standard.\p"
	.string "Not negotiable. DAGAN's price,\n"
	.string "not mine.\p"
	.string "And without a member card I can't\n"
	.string "sell you so much as a rope's end.\p"
	.string "…I'd leave if I knew where to go.$"

SiroccoDustmouthPort_Text_MerchantResolved:
	.string "The fee's gone. Just like that.\n"
	.string "One morning it didn't apply.\p"
	.string "I'm restocking everything.\n"
	.string "EVERYTHING. Come back in a week\l"
	.string "and this stall will be a SHOP.$"

SiroccoDustmouthPort_Text_Sailor:
	.string "Some scholar set up a dig camp\n"
	.string "east of the main route.\p"
	.string "Gilt Claw's been trying to shut\n"
	.string "him down for months.\p"
	.string "He keeps finding things they\n"
	.string "don't want found.$"

SiroccoDustmouthPort_Text_WaterDistributor:
	.string "Ration line forms at dawn.\n"
	.string "One measure per person.\p"
	.string "Don't argue about the amount.\n"
	.string "Don't ask what happens when it\l"
	.string "runs out.\p"
	.string "Those aren't questions I'm\n"
	.string "allowed to answer.$"

SiroccoDustmouthPort_Text_DistributorResolved:
	.string "No more ration line.\p"
	.string "I keep showing up at dawn anyway.\n"
	.string "Old habit.\p"
	.string "Mostly I just watch people walk\n"
	.string "past without needing me.\l"
	.string "It's better than it sounds.$"

SiroccoDustmouthPort_Text_NorthSign:
	.string "MIRADEN OASIS - 2 DAYS WALK\p"
	.string "GILT CLAW WATER ROUTE -\n"
	.string "PERMIT REQUIRED.$"

SiroccoDustmouthPort_Text_BerthSign:
	.string "VISITOR BERTH\p"
	.string "Mooring fees waived for vessels\n"
	.string "carrying trade goods or water.\p"
	.string "(Someone has scratched out\n"
	.string "“waived” and written “doubled”.)$"

SiroccoDustmouthPort_Text_TennysonAtBerth:
	.string "The TENNYSON sits at the berth,\n"
	.string "her brigantine rigging creaking\l"
	.string "in the hot wind.\p"
	.string "A thin film of red dust has\n"
	.string "already settled on her deck.$"

SiroccoDustmouthPort_Text_TennysonResolved:
	.string "The TENNYSON rides easy at the\n"
	.string "berth.\p"
	.string "Even the dust on her deck seems\n"
	.string "lighter, now that the island\l"
	.string "underneath her is breathing\l"
	.string "again.$"

