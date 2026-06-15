Primalis_Heartwood_SealChamber_MapScripts::
	.byte 0

@ ===================================================================
@ Discovery cutscene. Coord triggers at row 14 fire at
@ VAR_PRIMALIS_PROGRESS == 5 (set by the oral history). The seal
@ chamber beneath the root-knot - the deepest, oldest stone on
@ Primalis. Verdath is a vast Grass/Dragon shimmer behind the
@ apparatus. Sets FLAG_PRIMALIS_SEAL_FOUND. (Verdath cry placeholder =
@ SPECIES_RAYQUAZA, an ancient Dragon legendary - swap when a custom
@ Verdath species exists.)
@ ===================================================================
PrimalisSealChamber_EventScript_Discovery::
	lockall
	delay 20
	msgbox PrimalisSealChamber_Text_Discovery1, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_SUDOWOODO_SHAKE
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	msgbox PrimalisSealChamber_Text_Discovery2, MSGBOX_DEFAULT
	closemessage
	delay 20
	setflag FLAG_PRIMALIS_SEAL_FOUND
	msgbox PrimalisSealChamber_Text_DiscoveryClose, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Apparatus - the seal reinforcement. There is no Covenant siphon
@ here; the seal is ancient and the wear is honest - the destabilizing
@ transformations come from the seal weakening, not from a machine
@ draining it. The player reinforces it directly. As it stabilizes,
@ Verdath answers, and the fully-transformed Zoan begin to recover.
@ Mako's ONE line lands at the end: "The Root remembers us." - and
@ NOTHING follows it in that beat. Sets FLAG_PRIMALIS_RESOLVED +
@ progress 7, then the Sollis call.
@ ===================================================================
PrimalisSealChamber_EventScript_Apparatus::
	lockall
	goto_if_set FLAG_PRIMALIS_RESOLVED, PrimalisSealChamber_EventScript_ApparatusResolved
	goto_if_lt VAR_PRIMALIS_PROGRESS, 6, PrimalisSealChamber_EventScript_ApparatusNotReady
	msgbox PrimalisSealChamber_Text_ApparatusApproach, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PrimalisSealChamber_EventScript_ApparatusDecline
	closemessage
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 45
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	fadescreen FADE_FROM_WHITE
	msgbox PrimalisSealChamber_Text_Reinforce1, MSGBOX_DEFAULT
	closemessage
	delay 20
	@ The recovery - the fully-transformed begin to come back.
	msgbox PrimalisSealChamber_Text_Reinforce2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PRIMALIS_RESOLVED
	setvar VAR_PRIMALIS_PROGRESS, 7
	delay 40
	@ Mako's single line. Nothing after it in this beat.
	setspeaker Pelagios_Speaker_Mako
	msgbox PrimalisSealChamber_Text_MakoLine, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	delay 50
	goto PrimalisSealChamber_EventScript_SollisCall

@ -------------------------------------------------------------------
@ Resolution - the Sollis PokeNav call. She reveals she has known
@ about the kill switch since before the player was born. Ends on
@ the exact line: "Come home when you can. There are things I should
@ tell you in person." (No boat-tier change - Galleon is the cap;
@ Ashenveil uses a future sea-chart, and is gated by Schism/Thalvern/
@ Gildhaven, not Primalis.) Full-bag-safe (no items given here).
@ -------------------------------------------------------------------
PrimalisSealChamber_EventScript_SollisCall::
	lockall
	delay 20
	msgbox PrimalisSealChamber_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox PrimalisSealChamber_Text_SollisOpen, MSGBOX_DEFAULT
	setspeaker 0
	msgbox PrimalisSealChamber_Text_PlayerReply, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox PrimalisSealChamber_Text_SollisConfession, MSGBOX_DEFAULT
	closemessage
	delay 30
	setspeaker Pelagios_Speaker_Sollis
	msgbox PrimalisSealChamber_Text_SollisClose, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

PrimalisSealChamber_EventScript_ApparatusResolved::
	msgbox PrimalisSealChamber_Text_ApparatusResolved, MSGBOX_DEFAULT
	releaseall
	end

PrimalisSealChamber_EventScript_ApparatusNotReady::
	msgbox PrimalisSealChamber_Text_ApparatusNotReady, MSGBOX_DEFAULT
	releaseall
	end

PrimalisSealChamber_EventScript_ApparatusDecline::
	msgbox PrimalisSealChamber_Text_ApparatusDecline, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Inscription - the WARDEN's Journal, CIPHER 8. The full entry: the
@ kill switch, the THIRD OPTION the Warden found, and Dorne. Sets
@ FLAG_CIPHER_8_FOUND (cross-island) + FLAG_PRIMALIS_CIPHER_FOUND
@ (island).
@ ===================================================================
PrimalisSealChamber_EventScript_Inscription::
	lockall
	goto_if_set FLAG_PRIMALIS_CIPHER_FOUND, PrimalisSealChamber_EventScript_InscriptionAgain
	msgbox PrimalisSealChamber_Text_Inscription1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox PrimalisSealChamber_Text_Cipher1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox PrimalisSealChamber_Text_Cipher2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_CIPHER_8_FOUND
	setflag FLAG_PRIMALIS_CIPHER_FOUND
	msgbox PrimalisSealChamber_Text_CipherLogged, MSGBOX_DEFAULT
	releaseall
	end

PrimalisSealChamber_EventScript_InscriptionAgain::
	msgbox PrimalisSealChamber_Text_InscriptionAgain, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - discovery
@ ===================================================================
PrimalisSealChamber_Text_Discovery1:
	.string "The stair ends in a chamber that is not\n"
	.string "carved at all - it is GROWN. Living root\l"
	.string "and ancient stone, fused past telling\l"
	.string "where one ends and the other begins.$"

PrimalisSealChamber_Text_Discovery2:
	.string "Behind a lattice of light, something\n"
	.string "enormous turns in its long sleep.\p"
	.string "Verdath. The Root. Older than the seal,\n"
	.string "older than the people who made it,\l"
	.string "older than the word for old.\p"
	.string "And the lattice that holds it is\n"
	.string "thinning. You can see straight through\l"
	.string "the gaps.$"

PrimalisSealChamber_Text_DiscoveryClose:
	.string "The apparatus stands before the seal -\n"
	.string "no Covenant machine, only the ancient\l"
	.string "keeping-stone the first guardians left.\p"
	.string "It is waiting for a hand that means it\n"
	.string "well.$"

@ ===================================================================
@ Text - apparatus / reinforcement
@ ===================================================================
PrimalisSealChamber_Text_ApparatusApproach:
	.string "Reinforce Verdath's seal?$"

PrimalisSealChamber_Text_Reinforce1:
	.string "You lay your hands on the keeping-stone\n"
	.string "and the chamber fills with green light.\p"
	.string "The thinning lattice draws tight again,\n"
	.string "strand by strand, until it is whole -\l"
	.string "and Verdath's sleep goes deep and\l"
	.string "untroubled.$"

PrimalisSealChamber_Text_Reinforce2:
	.string "Far above, in the village, the\n"
	.string "fully-transformed ones feel it first.\p"
	.string "The thing that was pulling them past\n"
	.string "themselves - past their own names -\l"
	.string "loosens its grip.\p"
	.string "They begin, slowly, to come back.$"

PrimalisSealChamber_Text_MakoLine:
	.string "The Root remembers us.$"

PrimalisSealChamber_Text_PokenavBuzz:
	.string "...Your POKéNAV buzzes.$"

PrimalisSealChamber_Text_SollisOpen:
	.string "Did you speak to Elder MAKO?$"

PrimalisSealChamber_Text_PlayerReply:
	.string "You tell her. The kill switch. Haven\n"
	.string "Isle. The central node under your own\l"
	.string "home.$"

PrimalisSealChamber_Text_SollisConfession:
	.string "...\p"
	.string "I know.\p"
	.string "I have known since before you were\n"
	.string "born. Your parent told me. We argued\l"
	.string "about it for a year.$"

PrimalisSealChamber_Text_SollisClose:
	.string "Come home when you can. There are\n"
	.string "things I should tell you in person.$"

PrimalisSealChamber_Text_ApparatusResolved:
	.string "The seal holds, whole and bright. The\n"
	.string "Root sleeps as it was meant to.$"

PrimalisSealChamber_Text_ApparatusNotReady:
	.string "The lattice thins and brightens, but it\n"
	.string "is not yet yours to touch. There is\l"
	.string "more to see in this chamber first.$"

PrimalisSealChamber_Text_ApparatusDecline:
	.string "You draw your hand back. The light\n"
	.string "waits. It has waited a long time. It\l"
	.string "can wait a moment more.$"

@ ===================================================================
@ Text - inscription / Cipher 8
@ ===================================================================
PrimalisSealChamber_Text_Inscription1:
	.string "Tucked into a seam of the keeping-stone\n"
	.string "is a folded page in a hand you know\l"
	.string "better than your own. The Warden's.$"

PrimalisSealChamber_Text_Cipher1:
	.string "MAKO told me everything. I wept.\p"
	.string "The kill switch was built by people who\n"
	.string "loved this world enough to burn it down\l"
	.string "rather than let it be weaponized. I\l"
	.string "understand that impulse.\p"
	.string "But there is a third option. I found it.\p"
	.string "(The next lines are encoded - a careful,\n"
	.string "patient cipher, written for no one but\l"
	.string "the next Warden.)$"

PrimalisSealChamber_Text_Cipher2:
	.string "DORNE does not know about the third\n"
	.string "option. If he did, none of this would\l"
	.string "be necessary.\p"
	.string "I need to find a way to tell him that\n"
	.string "does not end with one of us dead.\p"
	.string "(Encoded again. Then, in plain hand, a\n"
	.string "single line:)\p"
	.string "I do not think I will find that way in\n"
	.string "time.$"

PrimalisSealChamber_Text_CipherLogged:
	.string "The Warden's Journal stirs in your bag.\n"
	.string "The eighth cipher settles into place.\p"
	.string "Eight of nine. And the last of it points\n"
	.string "home.$"

PrimalisSealChamber_Text_InscriptionAgain:
	.string "I do not think I will find that way in\n"
	.string "time.\p"
	.string "The Warden's hand, steady to the last\n"
	.string "word.$"
