Gildhaven_TheExchange_SealChamber_MapScripts::
	.byte 0

@ ===================================================================
@ Discovery cutscene. Coord triggers at row 14 fire at
@ VAR_GILDHAVEN_PROGRESS == 5 (set by the Serel victory). Ancient
@ stone, total silence, no gold - the deliberate opposite of the
@ gilt Exchange above. Mirath is a barely-visible Fairy/Dark shimmer.
@ Folds in the one-time RALTS CALLBACK: a single Ralts, drawn up to
@ Mirath's leaking energy, scatters when the player nears - the same
@ scene as the Haven Isle ruins, an island and a lifetime ago. This
@ is a pure CUTSCENE (no overworld Ralts object exists in this map's
@ map.json, and the brief frames it as a fleeting appearance, not a
@ catchable battle). Sets FLAG_GILDHAVEN_SEAL_FOUND + progress 6.
@ ===================================================================
GildhavenSealChamber_EventScript_Discovery::
	lockall
	delay 20
	msgbox GildhavenSealChamber_Text_Discovery1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox GildhavenSealChamber_Text_Discovery2, MSGBOX_DEFAULT
	closemessage
	@ Ralts callback - a soft chime, a flicker of movement at the seal.
	delay 30
	playse SE_PIN
	playmoncry SPECIES_RALTS, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	msgbox GildhavenSealChamber_Text_Ralts1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox GildhavenSealChamber_Text_Ralts2, MSGBOX_DEFAULT
	closemessage
	delay 20
	setflag FLAG_GILDHAVEN_SEAL_FOUND
	setvar VAR_GILDHAVEN_PROGRESS, 6
	msgbox GildhavenSealChamber_Text_DiscoveryClose, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Apparatus - there is NO siphon to disable here. Mirath's energy
@ leaks NATURALLY, seeping up through honest old stone with no
@ Covenant machinery bolted over it (unlike Ironhold / Sirocco). The
@ player reinforces the seal DIRECTLY. As it stabilizes, the city's
@ unnatural shimmer begins to dissolve. Sets FLAG_GILDHAVEN_RESOLVED,
@ progress 7, the Sollis call (she reveals she has held the yield map
@ for years), and the three-island Ashenveil gate.
@ ===================================================================
GildhavenSealChamber_EventScript_Apparatus::
	lockall
	goto_if_set FLAG_GILDHAVEN_RESOLVED, GildhavenSealChamber_EventScript_ApparatusResolved
	goto_if_lt VAR_GILDHAVEN_PROGRESS, 6, GildhavenSealChamber_EventScript_ApparatusNotReady
	msgbox GildhavenSealChamber_Text_ApparatusApproach, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, GildhavenSealChamber_EventScript_ApparatusDecline
	closemessage
	@ Reinforce the seal directly - presence, not machinery.
	delay 20
	playse SE_M_SUPERSONIC
	fadescreen FADE_TO_WHITE
	delay 40
	playmoncry SPECIES_GARDEVOIR, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	fadescreen FADE_FROM_WHITE
	msgbox GildhavenSealChamber_Text_Reinforce1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox GildhavenSealChamber_Text_Reinforce2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_GILDHAVEN_RESOLVED
	setvar VAR_GILDHAVEN_PROGRESS, 7
	@ PokeNav call to Sollis - she has had the Covenant yield map for years.
	delay 20
	msgbox GildhavenSealChamber_Text_PokenavBuzz, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox GildhavenSealChamber_Text_SollisCall, MSGBOX_DEFAULT
	setspeaker 0
	msgbox GildhavenSealChamber_Text_SollisReply, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox GildhavenSealChamber_Text_SollisConfession, MSGBOX_DEFAULT
	setspeaker 0
	closemessage
	@ Three-island order-independent Ashenveil gate.
	goto GildhavenSealChamber_EventScript_AshenveilCheck

@ -------------------------------------------------------------------
@ Ashenveil unlock check. Ashenveil opens only when Schism, Thalvern,
@ AND Gildhaven are all resolved. This MIRRORS the identical checks in
@ Schism_SealChamber_North and Thalvern_ThroneRoom (each checks the
@ OTHER two RESOLVED flags), so whichever of the three resolves LAST
@ fires the "opens" branch. Ashenveil is not built yet, so the "opens"
@ branch is still narration only (no warp, no boat-tier change - the
@ Ashenveil sea-chart is future work); it is a clean, real all-three
@ handler so only this one branch needs wiring when Ashenveil exists.
@ -------------------------------------------------------------------
GildhavenSealChamber_EventScript_AshenveilCheck::
	goto_if_unset FLAG_SCHISM_RESOLVED, GildhavenSealChamber_EventScript_AshenveilNotYet
	goto_if_unset FLAG_THALVERN_RESOLVED, GildhavenSealChamber_EventScript_AshenveilNotYet
	msgbox GildhavenSealChamber_Text_AshenveilOpens, MSGBOX_DEFAULT
	setspeaker Pelagios_Speaker_Sollis
	msgbox GildhavenSealChamber_Text_AshenveilSollis, MSGBOX_DEFAULT
	setspeaker 0
	releaseall
	end

GildhavenSealChamber_EventScript_AshenveilNotYet::
	msgbox GildhavenSealChamber_Text_AshenveilNotYet, MSGBOX_DEFAULT
	releaseall
	end

GildhavenSealChamber_EventScript_ApparatusResolved::
	msgbox GildhavenSealChamber_Text_ApparatusResolved, MSGBOX_DEFAULT
	releaseall
	end

GildhavenSealChamber_EventScript_ApparatusNotReady::
	msgbox GildhavenSealChamber_Text_ApparatusNotReady, MSGBOX_DEFAULT
	releaseall
	end

GildhavenSealChamber_EventScript_ApparatusDecline::
	msgbox GildhavenSealChamber_Text_ApparatusDecline, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Inscription - the WARDEN's Journal cipher 7. Includes the encoded
@ "Lace Vane is twelve years old" passage. Sets FLAG_CIPHER_7_FOUND
@ (cross-island) + FLAG_GILDHAVEN_CIPHER_FOUND (island).
@ ===================================================================
GildhavenSealChamber_EventScript_Inscription::
	lockall
	goto_if_set FLAG_GILDHAVEN_CIPHER_FOUND, GildhavenSealChamber_EventScript_InscriptionAgain
	msgbox GildhavenSealChamber_Text_Inscription1, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox GildhavenSealChamber_Text_CipherDecode, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_CIPHER_7_FOUND
	setflag FLAG_GILDHAVEN_CIPHER_FOUND
	msgbox GildhavenSealChamber_Text_CipherLogged, MSGBOX_DEFAULT
	releaseall
	end

GildhavenSealChamber_EventScript_InscriptionAgain::
	msgbox GildhavenSealChamber_Text_InscriptionAgain, MSGBOX_DEFAULT
	releaseall
	end

@ ===================================================================
@ Text - discovery + Ralts callback
@ ===================================================================
GildhavenSealChamber_Text_Discovery1:
	.string "The stair ends, and the city ends\n"
	.string "with it.\p"
	.string "No gold. No glass. No polish. Just\n"
	.string "stone older than GILDHAVEN's first\l"
	.string "coin, and a silence with weight to\l"
	.string "it.$"

GildhavenSealChamber_Text_Discovery2:
	.string "At the chamber's heart, a shimmer -\n"
	.string "soft, shifting, neither light nor\l"
	.string "dark but somehow both.\p"
	.string "MIRATH. Barely there. Leaking quietly\n"
	.string "upward, the way it has for forty\l"
	.string "years, into the city that grew rich\l"
	.string "on the wanting.$"

GildhavenSealChamber_Text_Ralts1:
	.string "Something small stirs in the\n"
	.string "shimmer.\p"
	.string "A RALTS. Tiny, wide-eyed, drawn up\n"
	.string "out of the dark by the seal's leaking\l"
	.string "warmth.$"

GildhavenSealChamber_Text_Ralts2:
	.string "It looks at you. You have done this\n"
	.string "before - an island and a lifetime\l"
	.string "ago, in a ruined hall on HAVEN ISLE.\p"
	.string "Then, like then, it turns and\n"
	.string "scatters into the stone, gone before\l"
	.string "you can move.\p"
	.string "Some things follow the WARDEN's blood\n"
	.string "wherever it goes.$"

GildhavenSealChamber_Text_DiscoveryClose:
	.string "The shimmer waits. Whatever is going\n"
	.string "to happen here, you are the one who\l"
	.string "has to start it.$"

@ ===================================================================
@ Text - apparatus / reinforcement
@ ===================================================================
GildhavenSealChamber_Text_ApparatusApproach:
	.string "The seal lies bare in the stone - no\n"
	.string "machine clamped over it, nothing to\l"
	.string "tear loose first.\p"
	.string "Just the leak, and the worn old\n"
	.string "binding behind it, waiting to be\l"
	.string "made whole.\p"
	.string "Reinforce MIRATH's seal?$"

GildhavenSealChamber_Text_Reinforce1:
	.string "You set your hand to the binding and\n"
	.string "the chamber fills with a still, white\l"
	.string "light.\p"
	.string "The leak draws inward. The shimmer\n"
	.string "settles. The wanting in the air -\l"
	.string "the thing you have felt since the\l"
	.string "harbor - simply stops.$"

GildhavenSealChamber_Text_Reinforce2:
	.string "Above you, in the city built over\n"
	.string "this chamber, forty years of\l"
	.string "artificial prosperity begins to\l"
	.string "dissolve.\p"
	.string "Somewhere a ledger stops making\n"
	.string "sense to the hand that keeps it.\p"
	.string "It will be a hard season. It will\n"
	.string "also be the first honest one\l"
	.string "GILDHAVEN has had in a lifetime.$"

GildhavenSealChamber_Text_PokenavBuzz:
	.string "...Your POKéNAV buzzes.$"

GildhavenSealChamber_Text_SollisCall:
	.string "GILDHAVEN readings just stabilized.\n"
	.string "MIRATH's seal is holding.\p"
	.string "What did you find down there?$"

GildhavenSealChamber_Text_SollisReply:
	.string "You tell her about the map. Every\n"
	.string "island. Every yield. The COVENANT's\l"
	.string "whole plan, framed on a wall like a\l"
	.string "thing to be proud of.$"

GildhavenSealChamber_Text_SollisConfession:
	.string "...The yield map. You found it.\p"
	.string "Your parent found a copy of that map\n"
	.string "eight years ago.\p"
	.string "I have it. I have had it the whole\n"
	.string "time.\p"
	.string "I should have shown you from the\n"
	.string "beginning. I told myself I was\l"
	.string "protecting you. I am not sure that\l"
	.string "was the whole truth.\p"
	.string "I am sorry.$"

GildhavenSealChamber_Text_ApparatusResolved:
	.string "The seal is whole. The chamber is\n"
	.string "quiet.\p"
	.string "The only shimmer left is the ordinary\n"
	.string "kind, on still water.$"

GildhavenSealChamber_Text_ApparatusNotReady:
	.string "The shimmer pulls at the edge of your\n"
	.string "thoughts. Not yet. You came down too\l"
	.string "fast - you have not even seen what\l"
	.string "this place is.$"

GildhavenSealChamber_Text_ApparatusDecline:
	.string "You stay your hand. The leak goes on,\n"
	.string "patient, upward, into the wanting\l"
	.string "city.$"

@ ===================================================================
@ Text - Ashenveil gate
@ ===================================================================
GildhavenSealChamber_Text_AshenveilOpens:
	.string "On the WARDEN's charts, the last of\n"
	.string "three rings around a darker mark goes\l"
	.string "still.\p"
	.string "SCHISM. THALVERN. GILDHAVEN. All three\n"
	.string "quiet now.\p"
	.string "The way to the dead island - ASHENVEIL\n"
	.string "- is open at last.$"

GildhavenSealChamber_Text_AshenveilSollis:
	.string "All three seals are holding. That\n"
	.string "means the charts will clear a path I\l"
	.string "hoped you would never need.\p"
	.string "The dead island. ASHENVEIL.\p"
	.string "Your parent went there once and would\n"
	.string "not talk about it for a month.\p"
	.string "When the sea-chart reaches you - be\n"
	.string "careful. Promise me.$"

GildhavenSealChamber_Text_AshenveilNotYet:
	.string "On the WARDEN's charts, three islands\n"
	.string "ring a fourth, darker mark.\p"
	.string "GILDHAVEN is quiet now. But the other\n"
	.string "rings still stir.\p"
	.string "The way to the dead island stays\n"
	.string "closed until all three are stilled.$"

@ ===================================================================
@ Text - inscription / cipher 7
@ ===================================================================
GildhavenSealChamber_Text_Inscription1:
	.string "An inscription, worn but legible,\n"
	.string "circles the base of the seal.\p"
	.string "Beside it, in a hand you know better\n"
	.string "than your own, the WARDEN has left a\l"
	.string "note.$"

GildhavenSealChamber_Text_CipherDecode:
	.string "GILDHAVEN is the most honest\n"
	.string "dishonest place I have ever been.\p"
	.string "Everyone knows the prosperity is\n"
	.string "wrong. Nobody says it. It is the\l"
	.string "island's open secret - like a family\l"
	.string "that knows one of them is sick and\l"
	.string "pretends at dinner that everything is\l"
	.string "fine.\p"
	.string "MIRATH does not force anyone to want\n"
	.string "things. It only amplifies what is\l"
	.string "already there.\p"
	.string "That is the terrifying part.\p"
	.string "...LACE VANE is twelve years old. She\n"
	.string "brought me tea and asked what I was\l"
	.string "researching. I did not tell her.\p"
	.string "I hope by the time this matters she\n"
	.string "has grown into someone better than\l"
	.string "her father.\p"
	.string "It is possible. It seemed possible.$"

GildhavenSealChamber_Text_CipherLogged:
	.string "Cipher 7 was added to the WARDEN's\n"
	.string "Journal.$"

GildhavenSealChamber_Text_InscriptionAgain:
	.string "The WARDEN's note circles the seal.\p"
	.string "(LACE VANE is twelve years old.) She\n"
	.string "is older now. She turned out better\l"
	.string "than he feared. The WARDEN would have\l"
	.string "been glad.$"
