Thalvern_DexCamp_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thalvern_DexCamp_OnTransition
	.byte 0

@ ===================================================================
@ If the island is resolved and Dex did NOT survive (PATH B), his
@ camp object is removed every load - the camp is here, the notes
@ are here, but Dex is not. His DexCamp object has no hide flag in
@ map.json, so a per-load removeobject is how we make his absence
@ permanent. (FLAG_DEX_ALIVE is set ONLY on PATH A's recovery.)
@ ===================================================================
Thalvern_DexCamp_OnTransition:
	goto_if_unset FLAG_THALVERN_RESOLVED, Thalvern_DexCamp_OnTransition_Done
	goto_if_set FLAG_DEX_ALIVE, Thalvern_DexCamp_OnTransition_Done
	removeobject LOCALID_THALVERN_DEX
Thalvern_DexCamp_OnTransition_Done:
	end

@ ===================================================================
@ Dex - multi-stage. First meeting sets FLAG_THALVERN_DEX_MET, with a
@ journal-reaction branch if the Warden's Journal is in the bag.
@ Stages:
@   - pre-met: full first-meeting enthusiasm (+ journal variant)
@   - post-met, pre-mural: research update
@   - post-mural (cipher found): the journal has cracked everything
@   - pre-Throne (Gym 3 clear): the Throne Room is within reach
@   - post-resolution + alive: changed, quieter, grateful
@ Dex is WARM AND ALIVE - no line foreshadows his death, ever.
@ ===================================================================
ThalvernDexCamp_EventScript_Dex::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Dex
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernDexCamp_EventScript_DexResolved
	goto_if_set FLAG_THALVERN_GYM3_CLEAR, ThalvernDexCamp_EventScript_DexPreThrone
	goto_if_set FLAG_THALVERN_CIPHER_FOUND, ThalvernDexCamp_EventScript_DexPostMural
	goto_if_set FLAG_THALVERN_DEX_MET, ThalvernDexCamp_EventScript_DexMet
	@ First meeting.
	msgbox ThalvernDexCamp_Text_DexFirst, MSGBOX_DEFAULT
	checkitem ITEM_WARDENS_JOURNAL
	goto_if_eq VAR_RESULT, TRUE, ThalvernDexCamp_EventScript_DexFirstJournal
	closemessage
	setflag FLAG_THALVERN_DEX_MET
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_DexFirstJournal::
	msgbox ThalvernDexCamp_Text_DexFirstJournal, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_THALVERN_DEX_MET
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_DexMet::
	msgbox ThalvernDexCamp_Text_DexMet, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_DexPostMural::
	msgbox ThalvernDexCamp_Text_DexPostMural, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_DexPreThrone::
	msgbox ThalvernDexCamp_Text_DexPreThrone, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_DexResolved::
	@ Only reachable if Dex is alive (PATH A) - if he died, the
	@ OnTransition removed his object, so this script never runs.
	msgbox ThalvernDexCamp_Text_DexResolved, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ The Lens at Dex's camp - appears once FLAG_THALVERN_LENS_DEFECTED
@ is set (his object's hide flag is that flag, so he is here only
@ after defecting). His "why I left the Covenant" speech.
@ ===================================================================
ThalvernDexCamp_EventScript_LensAtCamp::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Lens
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernDexCamp_EventScript_LensResolved
	msgbox ThalvernDexCamp_Text_LensCamp, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_LensResolved::
	goto_if_set FLAG_DEX_ALIVE, ThalvernDexCamp_EventScript_LensResolvedAlive
	msgbox ThalvernDexCamp_Text_LensResolvedGone, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

ThalvernDexCamp_EventScript_LensResolvedAlive::
	msgbox ThalvernDexCamp_Text_LensResolvedAlive, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

@ ===================================================================
@ Research notes / equipment - examinable lore. Post-resolution the
@ notes read differently depending on whether Dex survived.
@ ===================================================================
ThalvernDexCamp_EventScript_ResearchNotes1::
	lockall
	goto_if_set FLAG_THALVERN_RESOLVED, ThalvernDexCamp_EventScript_Notes1Resolved
	msgbox ThalvernDexCamp_Text_Notes1, MSGBOX_SIGN
	releaseall
	end

ThalvernDexCamp_EventScript_Notes1Resolved::
	goto_if_set FLAG_DEX_ALIVE, ThalvernDexCamp_EventScript_Notes1Alive
	msgbox ThalvernDexCamp_Text_Notes1Gone, MSGBOX_SIGN
	releaseall
	end

ThalvernDexCamp_EventScript_Notes1Alive::
	msgbox ThalvernDexCamp_Text_Notes1Alive, MSGBOX_SIGN
	releaseall
	end

ThalvernDexCamp_EventScript_ResearchNotes2::
	lockall
	msgbox ThalvernDexCamp_Text_Notes2, MSGBOX_SIGN
	releaseall
	end

ThalvernDexCamp_EventScript_EquipmentCrate::
	lockall
	msgbox ThalvernDexCamp_Text_Equipment, MSGBOX_SIGN
	releaseall
	end

@ ===================================================================
@ Text
@ ===================================================================
ThalvernDexCamp_Text_DexFirst:
	.string "You found me! The WARDEN's child -\n"
	.string "yes, I know who you are.\p"
	.string "Your parent and I worked together,\n"
	.string "years ago. They were better at the\l"
	.string "cipher than me. Far better.\p"
	.string "I've decoded three panels this\n"
	.string "week. Same civilization as Haven\l"
	.string "Isle, Sirocco, all of it.\p"
	.string "And THALVERN is their CENTER.\p"
	.string "PELAGIOS predates the civilization\n"
	.string "entirely. They built their whole\l"
	.string "culture around keeping its seal.\p"
	.string "Two thousand years devoted to a\n"
	.string "single act of care.\p"
	.string "Remarkable, isn't it?$"

ThalvernDexCamp_Text_DexFirstJournal:
	.string "Is that - your parent's journal?\p"
	.string "The cipher. May I see it?\p"
	.string "...$"
	.string "This is it. This is the key.\p"
	.string "Between your journal and my panels,\n"
	.string "we can decode everything. All of\l"
	.string "it. The whole story.\p"
	.string "Stay close. We're going to learn\n"
	.string "things no one has known in two\l"
	.string "thousand years.$"

ThalvernDexCamp_Text_DexMet:
	.string "Psalm's in the first ruin chamber.\n"
	.string "She's been reading one inscription\l"
	.string "for weeks. Brilliant. A little\l"
	.string "lost in it, maybe.\p"
	.string "The LENS - Covenant archaeologist,\n"
	.string "young - he's been slipping me\l"
	.string "information. I think he's having\l"
	.string "second thoughts about who he works\l"
	.string "for.$"

ThalvernDexCamp_Text_DexPostMural:
	.string "You touched the central mural.\n"
	.string "I can see it on your face.\p"
	.string "I've read the cipher of it a\n"
	.string "hundred times - but reading isn't\l"
	.string "seeing, is it.\p"
	.string "The journal confirmed the last\n"
	.string "fragment. The Throne Room is real,\l"
	.string "and it's close.$"

ThalvernDexCamp_Text_DexPreThrone:
	.string "The Throne Room. I've been trying\n"
	.string "to reach it for weeks. The Covenant\l"
	.string "kept blocking me.\p"
	.string "Now the LENS has given us a way\n"
	.string "through.\p"
	.string "I should go first. I've prepared\n"
	.string "for this my whole life.\p"
	.string "But something in PSALM's eyes when\n"
	.string "she came back... I don't know.\l"
	.string "I don't know.$"

ThalvernDexCamp_Text_DexResolved:
	.string "I'm still putting it in order. The\n"
	.string "whole history, end to end.\p"
	.string "I keep thinking about your parent.\n"
	.string "Sitting where I would have sat.\l"
	.string "Seeing what you saw.\p"
	.string "...Thank you. For going first.\p"
	.string "I don't think I understood what I\n"
	.string "was asking to carry.$"

ThalvernDexCamp_Text_LensCamp:
	.string "I worked for the Covenant for four\n"
	.string "years. I believed in institutional\l"
	.string "research.\p"
	.string "Then I read what DEX found, and I\n"
	.string "understood - institutions don't\l"
	.string "care what they find. Only what they\l"
	.string "can use.\p"
	.string "I care what I find.\p"
	.string "That distinction cost me my career.\n"
	.string "I think it was the right trade.$"

ThalvernDexCamp_Text_LensResolvedAlive:
	.string "DEX is going to need help keeping\n"
	.string "up with himself. He always does.\p"
	.string "I'll stay. There's work here worth\n"
	.string "a life.$"

ThalvernDexCamp_Text_LensResolvedGone:
	.string "Someone has to finish what DEX\n"
	.string "started.\p"
	.string "I have his notes. I have the time\n"
	.string "I owe him.\p"
	.string "I'll carry it from here.$"

ThalvernDexCamp_Text_Notes1:
	.string "Stacks of decoded panels in a\n"
	.string "careful hand.\p"
	.string "...the same glyph-system as the\n"
	.string "WARDEN's cipher. He's nearly there.$"

ThalvernDexCamp_Text_Notes1Alive:
	.string "Fresh notes, still being added to.\p"
	.string "The history, written down at last -\n"
	.string "by someone who lived to write it.$"

ThalvernDexCamp_Text_Notes1Gone:
	.string "His final notes, fully decoded.\p"
	.string "Everything he spent his life\n"
	.string "chasing, finished in a single\l"
	.string "moment.\p"
	.string "He isn't here to read them back.$"

ThalvernDexCamp_Text_Notes2:
	.string "A map of the ruins, hand-drawn,\n"
	.string "annotated obsessively.\p"
	.string "Every chamber labeled. Every water\n"
	.string "level dated. The sea is winning.$"

ThalvernDexCamp_Text_Equipment:
	.string "Survey gear, lamps, rope, a crate\n"
	.string "of dry notebooks.\p"
	.string "Someone has been living here for a\n"
	.string "long time.$"
