Ironhold_IronholdCity_MapScripts::
	.byte 0

@ -------------------------------------------------------------------
@ Gym 1 - Petra (Fighting), converted town hall, north end.
@ Talk-initiated. Victory: Iron Badge (FLAG_BADGE02_GET), TM Bulk Up,
@ FLAG_IRONHOLD_GYM1_CLEAR (also stands down the HQ guard object),
@ VAR_IRONHOLD_PROGRESS = 2.
@ -------------------------------------------------------------------
IronholdCity_EventScript_Petra::
	goto_if_defeated TRAINER_LEADER_IRONHOLD_PETRA, IronholdCity_EventScript_PetraDefeated
	trainerbattle_single TRAINER_LEADER_IRONHOLD_PETRA, IronholdCity_Text_PetraIntro, IronholdCity_Text_PetraDefeat, IronholdCity_EventScript_PetraVictory
	end

IronholdCity_EventScript_PetraVictory::
	msgbox IronholdCity_Text_PetraPostBattle, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_BADGE
	message IronholdCity_Text_ReceivedIronBadge
	waitmessage
	waitfanfare
	setflag FLAG_BADGE02_GET
	setflag FLAG_IRONHOLD_GYM1_CLEAR
	setvar VAR_IRONHOLD_PROGRESS, 2
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox IronholdCity_Text_PetraExplainTM, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_PetraDefeated::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdCity_EventScript_PetraResolved
	msgbox IronholdCity_Text_PetraPostBadge, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_PetraResolved::
	msgbox IronholdCity_Text_PetraResolved, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ City NPCs
@ -------------------------------------------------------------------
IronholdCity_EventScript_Elder::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdCity_EventScript_ElderResolved
	msgbox IronholdCity_Text_Elder, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_ElderResolved::
	msgbox IronholdCity_Text_ElderResolved, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_Child::
	lock
	faceplayer
	msgbox IronholdCity_Text_Child, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_Citizen::
	lock
	faceplayer
	goto_if_set FLAG_IRONHOLD_RESOLVED, IronholdCity_EventScript_CitizenResolved
	msgbox IronholdCity_Text_Citizen, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_CitizenResolved::
	msgbox IronholdCity_Text_CitizenResolved, MSGBOX_DEFAULT
	release
	end

@ Covenant officer who recognizes the Warden's name - goes quiet.
IronholdCity_EventScript_CovenantOfficer::
	lock
	faceplayer
	msgbox IronholdCity_Text_CovenantOfficer, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceUp
	waitmovement 0
	release
	end

IronholdCity_EventScript_SoldierPatrolA::
	lock
	faceplayer
	msgbox IronholdCity_Text_SoldierPatrolA, MSGBOX_DEFAULT
	release
	end

IronholdCity_EventScript_SoldierPatrolB::
	lock
	faceplayer
	msgbox IronholdCity_Text_SoldierPatrolB, MSGBOX_DEFAULT
	release
	end

@ Posted outside the contact's house. Hidden once FLAG_IRONHOLD_GYM1_CLEAR
@ is set (rotated north) - that flag is the object's hide flag in map.json.
IronholdCity_EventScript_HQGuard::
	lock
	faceplayer
	msgbox IronholdCity_Text_HQGuard, MSGBOX_DEFAULT
	release
	end

@ -------------------------------------------------------------------
@ Signs
@ -------------------------------------------------------------------
IronholdCity_EventScript_GymSign::
	msgbox IronholdCity_Text_GymSign, MSGBOX_SIGN
	end

IronholdCity_EventScript_ArmorySign::
	msgbox IronholdCity_Text_ArmorySign, MSGBOX_SIGN
	end

IronholdCity_EventScript_CenterSign::
	msgbox IronholdCity_Text_CenterSign, MSGBOX_SIGN
	end

IronholdCity_EventScript_HQSign::
	msgbox IronholdCity_Text_HQSign, MSGBOX_SIGN
	end

@ -------------------------------------------------------------------
@ Text
@ -------------------------------------------------------------------
IronholdCity_Text_PetraIntro:
	.string "You want to pass through IRONHOLD?\n"
	.string "Fine.\p"
	.string "Show me what you've got.\p"
	.string "I don't care who your parent was -\n"
	.string "earn it.$"

IronholdCity_Text_PetraDefeat:
	.string "…Earned.$"

IronholdCity_Text_PetraPostBattle:
	.string "Not bad.\p"
	.string "Honest answer? I'm glad someone's\n"
	.string "pushing back.\p"
	.string "Just… be careful who you say that\n"
	.string "to around here.$"

IronholdCity_Text_ReceivedIronBadge:
	.string "{PLAYER} received the IRON BADGE\n"
	.string "from PETRA!$"

IronholdCity_Text_PetraExplainTM:
	.string "That's BULK UP. Covenant drill\n"
	.string "manual, page one.\p"
	.string "It's the only page of that manual\n"
	.string "worth keeping.$"

IronholdCity_Text_PetraPostBadge:
	.string "The ARMORY smith - FORGE - he's\n"
	.string "the next one to convince.\p"
	.string "He's not a bad man. He's just\n"
	.string "been not-asking questions for a\l"
	.string "very long time.$"

IronholdCity_Text_PetraResolved:
	.string "The siphon's offline. I can feel\n"
	.string "it - the POKéMON in the city are\l"
	.string "calmer already.\p"
	.string "Whatever you did up there…\p"
	.string "She doesn't finish the sentence.\n"
	.string "She just bows her head, briefly,\l"
	.string "like an apology.$"

IronholdCity_Text_Elder:
	.string "This city had a different name\n"
	.string "before the Covenant arrived.\p"
	.string "We don't use it anymore. It's\n"
	.string "easier that way.\p"
	.string "Your parent knew the old name.\n"
	.string "I wonder if they told you.$"

IronholdCity_Text_ElderResolved:
	.string "Something changed on the summit\n"
	.string "today.\p"
	.string "The old name for this city was\n"
	.string "FERRATH'S REST.\p"
	.string "Now you know why.$"

IronholdCity_Text_Child:
	.string "One day I'm going to be a\n"
	.string "Covenant soldier!\p"
	.string "They get to carry real weapons!\p"
	.string "…\p"
	.string "…My dad says that's not something\n"
	.string "to want.\p"
	.string "But he says it quietly.$"

IronholdCity_Text_Citizen:
	.string "He leans in, voice low.\p"
	.string "Don't look at the summit at\n"
	.string "night. The glow started three\l"
	.string "months ago.\p"
	.string "Covenant says it's equipment\n"
	.string "testing.\p"
	.string "Equipment doesn't pulse like\n"
	.string "that.$"

IronholdCity_Text_CitizenResolved:
	.string "The summit went dark last night.\p"
	.string "First night I slept through since\n"
	.string "spring.\p"
	.string "…I'm not asking how. Don't tell\n"
	.string "me. Just - thank you.$"

IronholdCity_Text_CovenantOfficer:
	.string "Warden?\p"
	.string "I… that name is in our records.\p"
	.string "I shouldn't say anything else.$"

IronholdCity_Text_SoldierPatrolA:
	.string "Sector clear. Sector is always\n"
	.string "clear.\p"
	.string "We don't patrol for trouble.\n"
	.string "We patrol so you remember we're\l"
	.string "here.$"

IronholdCity_Text_SoldierPatrolB:
	.string "Keep moving, citizen.\p"
	.string "…You're not a citizen. Keep\n"
	.string "moving anyway.$"

IronholdCity_Text_HQGuard:
	.string "This building is under Covenant\n"
	.string "observation.\p"
	.string "No entry. No questions.\p"
	.string "Especially no questions.$"

IronholdCity_Text_GymSign:
	.string "IRONHOLD TOWN HALL\p"
	.string "(The word “HALL” has been struck\n"
	.string "through and replaced:)\p"
	.string "POKéMON GYM 1 - LEADER: PETRA\n"
	.string "“Earn it.”$"

IronholdCity_Text_ArmorySign:
	.string "IRONHOLD ARMORY\p"
	.string "POKéMON GYM 2 - LEADER: FORGE\n"
	.string "“What is made here, holds.”\p"
	.string "Munitions output requisitioned\n"
	.string "by Covenant order 112-C.$"

IronholdCity_Text_CenterSign:
	.string "POKéMON CENTER\p"
	.string "Heal your POKéMON.\n"
	.string "(Visits may be logged.)$"

IronholdCity_Text_HQSign:
	.string "COVENANT GARRISON HQ - IRONHOLD\n"
	.string "COMMAND\p"
	.string "Commander: H. SEVER\p"
	.string "All inquiries in writing.\n"
	.string "All writing is reviewed.$"
