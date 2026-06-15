Primalis_JungleRoute1_MapScripts::
	.byte 0

@ ===================================================================
@ Zoan Guardian Sera (TRAINER_ZOAN_PRIMALIS_1) - sight trainer.
@ Sight trainers MUST keep trainerbattle as the first command, so
@ the intro gets no namebox; setspeaker only before the post-battle
@ msgbox.
@ ===================================================================
PrimalisJungleRoute1_EventScript_Zoan1::
	trainerbattle_single TRAINER_ZOAN_PRIMALIS_1, PrimalisJungleRoute1_Text_SeraIntro, PrimalisJungleRoute1_Text_SeraDefeat, PrimalisJungleRoute1_EventScript_Zoan1Post
	setspeaker Pelagios_Speaker_Sera
	msgbox PrimalisJungleRoute1_Text_SeraPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

PrimalisJungleRoute1_EventScript_Zoan1Post::
	setspeaker Pelagios_Speaker_Sera
	msgbox PrimalisJungleRoute1_Text_SeraPost, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Beast-Whistle gate - OPTIONAL NE reward nook. The undergrowth is
@ baked into the layout as impassable; the actual tile-clearing field
@ effect is DEFERRED to systems (no C field effect exists yet, same
@ as Ironhold's rubble having no clear animation). Until the Beast
@ Whistle is held, these triggers bounce the player south with a
@ "too dense" message. Once held, the player is let through with a
@ brief "you clear the growth" line. Gate keys off the item, not a
@ progress value, so it works the moment the Whistle is obtained.
@ ===================================================================
PrimalisJungleRoute1_EventScript_BeastGate::
	lockall
	checkitem ITEM_BEAST_WHISTLE
	goto_if_eq VAR_RESULT, TRUE, PrimalisJungleRoute1_EventScript_BeastGateOpen
	msgbox PrimalisJungleRoute1_Text_BeastGateBlocked, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PrimalisJungleRoute1_Movement_BounceSouth
	waitmovement 0
	releaseall
	end

PrimalisJungleRoute1_EventScript_BeastGateOpen::
	playse SE_M_GRASSWHISTLE
	msgbox PrimalisJungleRoute1_Text_BeastGateOpen, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

PrimalisJungleRoute1_Movement_BounceSouth:
	walk_down
	step_end

PrimalisJungleRoute1_Text_SeraIntro:
	.string "You are going deeper. I need to know\n"
	.string "you can handle what is deeper.$"

PrimalisJungleRoute1_Text_SeraDefeat:
	.string "All right. All right.$"

PrimalisJungleRoute1_Text_SeraPost:
	.string "You can. Keep going.$"

PrimalisJungleRoute1_Text_BeastGateBlocked:
	.string "The undergrowth here is a living wall -\n"
	.string "vines thick as your arm, knotted past\l"
	.string "any blade.\p"
	.string "It would take more than strength to\n"
	.string "open this. It would take the forest's\l"
	.string "permission.$"

PrimalisJungleRoute1_Text_BeastGateOpen:
	.string "You raise the BEAST WHISTLE and breathe\n"
	.string "a low, carrying note.\p"
	.string "The vines shudder, then draw back like\n"
	.string "an animal making room. A path opens.$"
