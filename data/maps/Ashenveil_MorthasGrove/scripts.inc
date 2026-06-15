Ashenveil_MorthasGrove_MapScripts::
	.byte 0

@ ============================================================================
@ MORTHAS GROVE - a cathedral of ash-preserved dead trees. A dark pool at the
@ centre holds Morthas's seal. Cass left the sea chart by the tree line.
@
@ The endurance encounter is NOT a battle. Five turns of vision; each turn the
@ player chooses to Endure or Turn away. Turning away ends the attempt and the
@ player may re-enter to try again (the trigger re-arms at progress 5). Five
@ endured: the final silent vision, the Seal Shard, and Cipher 9 - the last
@ journal entry, the emotional climax of the game. Morthas has no dialogue.
@ ============================================================================

@ Talk fallback. Morthas is approached through the trigger, not by talking; its
@ object is removed once FLAG_MORTHAS_ENCOUNTERED is set. Defensive only.
AshenveilMorthasGrove_EventScript_Morthas::
	lockall
	msgbox AshenveilMorthasGrove_Text_MorthasStill, MSGBOX_DEFAULT
	releaseall
	end

@ ----------------------------------------------------------------------------
@ The endurance encounter (coord trigger, progress 5).
@ VAR_TEMP_1 is the turn counter (0..5). It is a temp var - it resets to 0 on
@ every map load, so a player who turns away and walks out always restarts at
@ turn 1 with no permanent state consumed.
@ ----------------------------------------------------------------------------
AshenveilMorthasGrove_EventScript_MorthasApproach::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	msgbox AshenveilMorthasGrove_Text_PoolStirs, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, 0

AshenveilMorthasGrove_EventScript_EnduranceTurn::
	@ Branch to the vision text for the current turn.
	goto_if_eq VAR_TEMP_1, 0, AshenveilMorthasGrove_EventScript_Vision1
	goto_if_eq VAR_TEMP_1, 1, AshenveilMorthasGrove_EventScript_Vision2
	goto_if_eq VAR_TEMP_1, 2, AshenveilMorthasGrove_EventScript_Vision3
	goto_if_eq VAR_TEMP_1, 3, AshenveilMorthasGrove_EventScript_Vision4
	goto AshenveilMorthasGrove_EventScript_Vision5

AshenveilMorthasGrove_EventScript_Vision1::
	msgbox AshenveilMorthasGrove_Text_Vision1, MSGBOX_DEFAULT
	goto AshenveilMorthasGrove_EventScript_EndurePrompt
AshenveilMorthasGrove_EventScript_Vision2::
	msgbox AshenveilMorthasGrove_Text_Vision2, MSGBOX_DEFAULT
	goto AshenveilMorthasGrove_EventScript_EndurePrompt
AshenveilMorthasGrove_EventScript_Vision3::
	msgbox AshenveilMorthasGrove_Text_Vision3, MSGBOX_DEFAULT
	goto AshenveilMorthasGrove_EventScript_EndurePrompt
AshenveilMorthasGrove_EventScript_Vision4::
	msgbox AshenveilMorthasGrove_Text_Vision4, MSGBOX_DEFAULT
	goto AshenveilMorthasGrove_EventScript_EndurePrompt
AshenveilMorthasGrove_EventScript_Vision5::
	msgbox AshenveilMorthasGrove_Text_Vision5, MSGBOX_DEFAULT
	goto AshenveilMorthasGrove_EventScript_EndurePrompt

AshenveilMorthasGrove_EventScript_EndurePrompt::
	msgbox AshenveilMorthasGrove_Text_EndurePrompt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, AshenveilMorthasGrove_EventScript_TurnAway
	addvar VAR_TEMP_1, 1
	goto_if_ge VAR_TEMP_1, 5, AshenveilMorthasGrove_EventScript_Settles
	goto AshenveilMorthasGrove_EventScript_EnduranceTurn

@ Turn away = fled. The attempt ends; the player may re-enter to retry.
AshenveilMorthasGrove_EventScript_TurnAway::
	closemessage
	msgbox AshenveilMorthasGrove_Text_TurnedAway, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_1, 0
	applymovement OBJ_EVENT_ID_PLAYER, AshenveilMorthasGrove_Movement_StepBack
	waitmovement 0
	releaseall
	end

@ ----------------------------------------------------------------------------
@ Five endured. Morthas settles. The final vision - in silence.
@ ----------------------------------------------------------------------------
AshenveilMorthasGrove_EventScript_Settles::
	closemessage
	msgbox AshenveilMorthasGrove_Text_Settles, MSGBOX_DEFAULT
	closemessage
	delay 20

	@ Stop the music completely for the final vision. fadeoutbgm produces the
	@ silence the brief requires; fadedefaultbgm restores the grove track after.
	fadeoutbgm 4
	delay 40

	msgbox AshenveilMorthasGrove_Text_FinalVision1, MSGBOX_DEFAULT
	delay 30
	msgbox AshenveilMorthasGrove_Text_FinalVision2, MSGBOX_DEFAULT
	delay 30
	msgbox AshenveilMorthasGrove_Text_FinalVision3, MSGBOX_DEFAULT
	closemessage
	@ Roughly six seconds of stillness before it ends.
	delay 90

	@ Then it's over. Restore the music.
	fadedefaultbgm
	delay 30

	@ Resolution. Set all permanent state BEFORE the (full-bag-safe) item give
	@ so nothing critical can be skipped by a full bag.
	setflag FLAG_MORTHAS_ENCOUNTERED
	setflag FLAG_ASHENVEIL_VISITED
	setvar VAR_ASHENVEIL_PROGRESS, 6
	removeobject LOCALID_ASHENVEIL_MORTHAS

	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox AshenveilMorthasGrove_Text_SealShard, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_SEAL_SHARD_ASHENVEIL

	@ Cipher 9 - the final journal entry. The emotional climax. It is the last
	@ thing the scene says. Nothing follows the two closing lines.
	goto AshenveilMorthasGrove_EventScript_Cipher9

AshenveilMorthasGrove_EventScript_Cipher9::
	delay 20
	msgbox AshenveilMorthasGrove_Text_JournalOpens, MSGBOX_DEFAULT
	setflag FLAG_CIPHER_9_FOUND
	setflag FLAG_ASHENVEIL_CIPHER_FOUND
	setflag FLAG_TRUE_ENDING_UNLOCKED
	setvar VAR_ASHENVEIL_PROGRESS, 7
	msgbox AshenveilMorthasGrove_Text_Cipher9, MSGBOX_DEFAULT
	releaseall
	end

@ ----------------------------------------------------------------------------
@ Sea chart stone (bg_event examine). Cass hid the chart here.
@ ----------------------------------------------------------------------------
AshenveilMorthasGrove_EventScript_SeaChartStone::
	lockall
	goto_if_set FLAG_SEA_CHART_FOUND, AshenveilMorthasGrove_EventScript_SeaChartTaken
	msgbox AshenveilMorthasGrove_Text_SeaChartFind, MSGBOX_DEFAULT
	closemessage
	@ Set the flag and advance BEFORE the give, so a full bag can never strand
	@ the chart unfound. FLAG_SEA_CHART_FOUND is what activates Aetheron.
	setflag FLAG_SEA_CHART_FOUND
	goto_if_lt VAR_ASHENVEIL_PROGRESS, 5, AshenveilMorthasGrove_EventScript_SeaChartSetProgress
	goto AshenveilMorthasGrove_EventScript_SeaChartGive
AshenveilMorthasGrove_EventScript_SeaChartSetProgress::
	setvar VAR_ASHENVEIL_PROGRESS, 5
AshenveilMorthasGrove_EventScript_SeaChartGive::
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox AshenveilMorthasGrove_Text_SeaChartGot, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_SEA_CHART
	msgbox AshenveilMorthasGrove_Text_SeaChartAetheron, MSGBOX_DEFAULT
	releaseall
	end

AshenveilMorthasGrove_EventScript_SeaChartTaken::
	msgbox AshenveilMorthasGrove_Text_SeaChartEmpty, MSGBOX_DEFAULT
	releaseall
	end

@ ----------------------------------------------------------------------------
@ Movements
@ ----------------------------------------------------------------------------
AshenveilMorthasGrove_Movement_StepBack:
	walk_down
	step_end

@ ----------------------------------------------------------------------------
@ Text
@ ----------------------------------------------------------------------------

AshenveilMorthasGrove_Text_MorthasStill::
	.string "The pool is dark and still. Something\n"
	.string "vast rests just beneath the surface,\l"
	.string "neither asleep nor awake.$"

AshenveilMorthasGrove_Text_PoolStirs::
	.string "The pool darkens as you near it.\p"
	.string "The cold comes first. Then the\n"
	.string "memories - not yours.$"

AshenveilMorthasGrove_Text_Vision1::
	.string "A boat, too full. Hands reaching from\n"
	.string "the water for a gunwale already gone.$"

AshenveilMorthasGrove_Text_Vision2::
	.string "A harbour bell ringing and ringing\n"
	.string "with no one left to answer it.$"

AshenveilMorthasGrove_Text_Vision3::
	.string "A list of names in a careful hand,\n"
	.string "and a second list, longer, that no\l"
	.string "one was meant to keep.$"

AshenveilMorthasGrove_Text_Vision4::
	.string "Ash settling over a doorstep where\n"
	.string "someone always swept. No one sweeps\l"
	.string "it now.$"

AshenveilMorthasGrove_Text_Vision5::
	.string "The weight of being remembered by\n"
	.string "the only thing left that can.\p"
	.string "It is asking if you can hold it.$"

AshenveilMorthasGrove_Text_EndurePrompt::
	.string "Endure it?$"

AshenveilMorthasGrove_Text_TurnedAway::
	.string "You look away. The cold lets go of\n"
	.string "you, and the pool goes still again.\p"
	.string "It is still waiting.$"

AshenveilMorthasGrove_Text_Settles::
	.string "You hold it. All of it.\p"
	.string "The pool stops fighting you. Whatever\n"
	.string "is beneath it settles, and for the\l"
	.string "first time it lets you see clearly.$"

@ The final vision. Shown in silence (fadeoutbgm). Paced with delays so it
@ lasts about six seconds. Verbatim from the brief.
AshenveilMorthasGrove_Text_FinalVision1::
	.string "You see the island before it died.\p"
	.string "Not ruins. A city. Morning light.$"

AshenveilMorthasGrove_Text_FinalVision2::
	.string "People going about their day - a baker\n"
	.string "opening a stall, two children arguing\l"
	.string "over something small and important,\l"
	.string "a fisherman checking his nets.\p"
	.string "A woman hanging laundry and humming a\n"
	.string "song you don't recognize.$"

AshenveilMorthasGrove_Text_FinalVision3::
	.string "None of them know what is coming.\p"
	.string "The vision lasts six seconds.\n"
	.string "There is no music.\p"
	.string "Then it's over.$"

AshenveilMorthasGrove_Text_SealShard::
	.string "Where the pool was darkest, a shard of\n"
	.string "something cold and still has settled\l"
	.string "at the water's edge.\p"
	.string "You took the SEAL SHARD.$"

@ Cipher 9 - the final journal entry. Ends on the two closing lines. Nothing
@ follows them. Do not change them.
AshenveilMorthasGrove_Text_JournalOpens::
	.string "The shard's cold settles into the\n"
	.string "Warden's journal. The last cipher\l"
	.string "gives way. Their hand fills the page.$"

AshenveilMorthasGrove_Text_Cipher9::
	.string "If you're reading this, you decoded\n"
	.string "everything. That means you're ready.\p"
	.string "The kill switch has a third option.\n"
	.string "Dorne doesn't know. I found it in\l"
	.string "Thalvern's Throne Room - in the vision.\p"
	.string "The civilization built the kill switch\n"
	.string "to destroy the legendary network. But\l"
	.string "they also built a second function.\l"
	.string "A release.\p"
	.string "Not destruction. Liberation.\p"
	.string "If the seals are reinforced - all of\n"
	.string "them, stable - the kill switch doesn't\l"
	.string "destroy. It frees.\p"
	.string "The legendaries remain in the world but\n"
	.string "the seal network dissolves. No one can\l"
	.string "exploit them again. No one can use the\l"
	.string "kill switch against them again.\p"
	.string "I couldn't tell Dorne. He would have\n"
	.string "used it before the seals were ready.\p"
	.string "I couldn't tell anyone.\n"
	.string "I'm telling you now.\p"
	.string "The Covenant is moving faster than I\n"
	.string "expected. I don't have time to do this\l"
	.string "myself.\p"
	.string "I'm sorry I'm leaving it to you.\p"
	.string "I love you. I should have said that\n"
	.string "more.\p"
	.string "I love you.$"

@ Sea chart stone.
AshenveilMorthasGrove_Text_SeaChartFind::
	.string "A waterproof case is wedged into the\n"
	.string "stone at the tree line.\p"
	.string "Inside: a sea chart. “The stream rises\n"
	.string "here.” An X on the open sea.\p"
	.string "Below, in different handwriting:\p"
	.string "“In case something happens to me. -- C”$"

AshenveilMorthasGrove_Text_SeaChartGot::
	.string "You took the SEA CHART.$"

AshenveilMorthasGrove_Text_SeaChartAetheron::
	.string "The X marks where the stream rises.\n"
	.string "Aetheron is above it - the sky island.\p"
	.string "The Tennyson can reach it now.$"

AshenveilMorthasGrove_Text_SeaChartEmpty::
	.string "The empty case rests in the stone.\n"
	.string "You have the chart it held.$"
