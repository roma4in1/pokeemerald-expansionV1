Ashenveil_TheMemorial_MapScripts::
	.byte 0

@ ============================================================================
@ THE MEMORIAL - the small maintained garden, the only green on the island.
@ Dorne is hidden by FLAG_ASHENVEIL_DORNE_MET until the scene reveals him.
@ The DorneScene coord trigger (progress 3) is the single most important scene
@ in the game: the full truth, the player's choice, and his quiet departure.
@ ============================================================================

@ Talk fallback. The scene is trigger-only; before it fires Dorne's object is
@ hidden, and after it fires removeobject takes him away. If somehow talked to
@ while present (defensive), replay the offer beat quietly.
AshenveilTheMemorial_EventScript_Dorne::
	lockall
	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_DorneOfferShort, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

@ ----------------------------------------------------------------------------
@ THE DORNE SCENE (progress 3). Reveal the hidden Dorne object, deliver the
@ full truth sequence, take the player's choice, his daughter's name, then he
@ leaves. No music sting. No camera. He just goes.
@ ----------------------------------------------------------------------------
AshenveilTheMemorial_EventScript_DorneScene::
	lockall
	@ Reveal Dorne for the scene (his object is FLAG_ASHENVEIL_DORNE_MET-hidden;
	@ clear it now so addobject shows him, then re-set it after he leaves so he
	@ stays gone on every future load).
	clearflag FLAG_ASHENVEIL_DORNE_MET
	addobject LOCALID_ASHENVEIL_DORNE
	delay 30
	applymovement LOCALID_ASHENVEIL_DORNE, AshenveilTheMemorial_Movement_DorneTurn
	waitmovement 0
	delay 15

	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_Dorne1, MSGBOX_DEFAULT
	msgbox AshenveilTheMemorial_Text_Dorne2, MSGBOX_DEFAULT
	msgbox AshenveilTheMemorial_Text_Dorne3, MSGBOX_DEFAULT
	msgbox AshenveilTheMemorial_Text_Dorne4, MSGBOX_DEFAULT
	@ The Warden's-death truth.
	msgbox AshenveilTheMemorial_Text_DorneTruth1, MSGBOX_DEFAULT
	msgbox AshenveilTheMemorial_Text_DorneTruth2, MSGBOX_DEFAULT
	@ The exact carry line - its own msgbox, verbatim.
	msgbox AshenveilTheMemorial_Text_DorneCarry, MSGBOX_DEFAULT
	@ The offer.
	msgbox AshenveilTheMemorial_Text_DorneOffer, MSGBOX_DEFAULT
	setspeaker 0

	@ The choice: two sequential YESNO prompts (no custom multichoice list -
	@ that is systems-engineer/C domain). Exactly one of the three flags is set.
	msgbox AshenveilTheMemorial_Text_ChoiceStop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, AshenveilTheMemorial_EventScript_ChoiceStop
	msgbox AshenveilTheMemorial_Text_ChoiceHelp, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, AshenveilTheMemorial_EventScript_ChoiceHelp
	goto AshenveilTheMemorial_EventScript_ChoiceDefer

@ "I'll stop you." Dorne respects it - the Warden's answer.
AshenveilTheMemorial_EventScript_ChoiceStop::
	setflag FLAG_DORNE_CHOICE_STOP
	@ Respect, but not maximum. Never lower an already-higher value.
	call AshenveilTheMemorial_EventScript_RaiseDorneRespect
	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_DorneRepliesStop, MSGBOX_DEFAULT
	setspeaker 0
	goto AshenveilTheMemorial_EventScript_DorneFinal

@ "I'll help you." VAR_DORNE_RELATIONSHIP = maximum.
AshenveilTheMemorial_EventScript_ChoiceHelp::
	setflag FLAG_DORNE_CHOICE_HELP
	setvar VAR_DORNE_RELATIONSHIP, 3
	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_DorneRepliesHelp, MSGBOX_DEFAULT
	setspeaker 0
	goto AshenveilTheMemorial_EventScript_DorneFinal

@ "I need time." He waits.
AshenveilTheMemorial_EventScript_ChoiceDefer::
	setflag FLAG_DORNE_CHOICE_DEFER
	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_DorneRepliesDefer, MSGBOX_DEFAULT
	setspeaker 0
	goto AshenveilTheMemorial_EventScript_DorneFinal

@ Raise Dorne toward respect (2) for the STOP path without lowering a higher
@ prior value. (HELP sets 3 directly; DEFER leaves the var as it was.)
AshenveilTheMemorial_EventScript_RaiseDorneRespect::
	goto_if_ge VAR_DORNE_RELATIONSHIP, 2, AshenveilTheMemorial_EventScript_RaiseDorneRespectDone
	setvar VAR_DORNE_RELATIONSHIP, 2
AshenveilTheMemorial_EventScript_RaiseDorneRespectDone::
	return

@ His daughter's name, then he leaves. No dramatic exit. Just leaves.
AshenveilTheMemorial_EventScript_DorneFinal::
	setspeaker Pelagios_Speaker_Dorne
	msgbox AshenveilTheMemorial_Text_DorneDaughter, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 30
	applymovement LOCALID_ASHENVEIL_DORNE, AshenveilTheMemorial_Movement_DorneLeave
	waitmovement 0
	removeobject LOCALID_ASHENVEIL_DORNE
	@ Re-assert the hide flag so he is permanently gone on future loads, and
	@ mark the scene done. Advance progress 3 -> 4 (Dorne met).
	setflag FLAG_ASHENVEIL_DORNE_MET
	setflag FLAG_ASHENVEIL_VISITED
	setvar VAR_ASHENVEIL_PROGRESS, 4
	releaseall
	end

@ ----------------------------------------------------------------------------
@ Grave markers. Quiet. The newest is Dorne's daughter - nothing follows it.
@ ----------------------------------------------------------------------------
AshenveilTheMemorial_EventScript_Grave1::
	lockall
	msgbox AshenveilTheMemorial_Text_Grave1, MSGBOX_DEFAULT
	releaseall
	end

AshenveilTheMemorial_EventScript_Grave2::
	lockall
	msgbox AshenveilTheMemorial_Text_Grave2, MSGBOX_DEFAULT
	releaseall
	end

AshenveilTheMemorial_EventScript_Grave3::
	lockall
	msgbox AshenveilTheMemorial_Text_Grave3, MSGBOX_DEFAULT
	releaseall
	end

AshenveilTheMemorial_EventScript_GraveNewest::
	lockall
	msgbox AshenveilTheMemorial_Text_GraveNewest, MSGBOX_DEFAULT
	releaseall
	end

@ ----------------------------------------------------------------------------
@ Movements
@ ----------------------------------------------------------------------------
AshenveilTheMemorial_Movement_DorneTurn:
	walk_in_place_fast_down
	step_end

@ He walks off, unhurried. removeobject takes him the rest of the way.
AshenveilTheMemorial_Movement_DorneLeave:
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

@ ----------------------------------------------------------------------------
@ Text
@ ----------------------------------------------------------------------------

@ Dorne turns from the grave and begins. The truth, in order.
AshenveilTheMemorial_Text_Dorne1::
	.string "You came.\p"
	.string "I thought you might. You have your\n"
	.string "parent's habit of walking toward the\l"
	.string "thing everyone else walks away from.$"

AshenveilTheMemorial_Text_Dorne2::
	.string "Twenty years ago the Covenant let\n"
	.string "this island die.\p"
	.string "Eight hundred and forty-seven people.\n"
	.string "Boats for four hundred.\l"
	.string "They knew the numbers before they\l"
	.string "sent them.\p"
	.string "Then they wrote a report that said\n"
	.string "everyone got out, and they classified\l"
	.string "the truth, and they kept going.$"

AshenveilTheMemorial_Text_Dorne3::
	.string "The seal-siphoning did this. The same\n"
	.string "thing they are still doing, island by\l"
	.string "island, right now.\p"
	.string "The people who built the seals were\n"
	.string "not fools. They knew their network\l"
	.string "could be turned into a weapon.\p"
	.string "So they built a failsafe. A way to\n"
	.string "shut the whole thing down for good.\l"
	.string "A kill switch.$"

AshenveilTheMemorial_Text_Dorne4::
	.string "It was meant for exactly this. For a\n"
	.string "power that learned to feed on the\l"
	.string "seals and would not stop.\p"
	.string "I mean to use it.$"

@ The Warden's-death truth.
AshenveilTheMemorial_Text_DorneTruth1::
	.string "Your parent didn't die because of me.\p"
	.string "We disagreed. They refused. I respected\n"
	.string "that.\p"
	.string "Three months later they were dead.$"

AshenveilTheMemorial_Text_DorneTruth2::
	.string "The Covenant intercepted their message\n"
	.string "about stopping my plan.\p"
	.string "A living Warden who knew what they knew\n"
	.string "was more dangerous to the Covenant than\l"
	.string "I was.$"

@ The exact carry line. Verbatim. Its own msgbox.
AshenveilTheMemorial_Text_DorneCarry::
	.string "I didn't kill your parent.\p"
	.string "But I knew it was coming and I didn't\n"
	.string "warn them.\p"
	.string "That's what I carry.$"

@ The offer.
AshenveilTheMemorial_Text_DorneOffer::
	.string "I'm going to use the kill switch.\p"
	.string "Either you're there to make sure I do\n"
	.string "it right, or you're there to stop me.\p"
	.string "Either way ends the Covenant's ability\n"
	.string "to exploit.\p"
	.string "The difference is what happens to the\n"
	.string "legendaries.$"

@ Choice prompts.
AshenveilTheMemorial_Text_ChoiceStop::
	.string "Tell him: I'll stop you.$"

AshenveilTheMemorial_Text_ChoiceHelp::
	.string "Tell him: I'll help you.$"

@ Replies.
AshenveilTheMemorial_Text_DorneRepliesStop::
	.string "I know.\p"
	.string "That's what your parent would have done.\n"
	.string "I still respect it.$"

AshenveilTheMemorial_Text_DorneRepliesHelp::
	.string "Good.\p"
	.string "The kill switch requires a Warden's\n"
	.string "activation. I couldn't have used it\l"
	.string "alone.$"

AshenveilTheMemorial_Text_DorneRepliesDefer::
	.string "Take it.\p"
	.string "I'll be at Haven Isle. I've waited\n"
	.string "twenty years. I can wait longer.$"

@ Daughter lines. Verbatim. Nothing dramatic follows; he leaves.
AshenveilTheMemorial_Text_DorneDaughter::
	.string "Her name was Vael. Same as mine.\n"
	.string "Her mother's idea.\p"
	.string "She was four years old.$"

@ Defensive talk fallback (Dorne should normally be gone by talk time).
AshenveilTheMemorial_Text_DorneOfferShort::
	.string "Make your choice when you're ready.\p"
	.string "I'll be at the grove, or at Haven.$"

@ Graves. The older markers - quiet, worn.
AshenveilTheMemorial_Text_Grave1::
	.string "A worn marker. The name has gone soft\n"
	.string "with weather. Someone has kept the moss\l"
	.string "cut back from it anyway.$"

AshenveilTheMemorial_Text_Grave2::
	.string "Two names share this stone. A shared\n"
	.string "date below them. They went together.$"

AshenveilTheMemorial_Text_Grave3::
	.string "No name on this one. Only a line:\p"
	.string "“For the ones the boats couldn't take.”$"

@ The newest marker. Nothing follows. Ends at twenty-three.
AshenveilTheMemorial_Text_GraveNewest::
	.string "VAEL DORNE (JUNIOR)\n"
	.string "Born on this island.\l"
	.string "She would have been twenty-three.$"
