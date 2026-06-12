@ ---------------------------------------------------------------------------
@ Pelagios speaker-name strings for the dialogue namebox.
@ Usage in any scripts.inc:
@     setspeaker Pelagios_Speaker_Sollis   @ next msgbox shows a "SOLLIS" namebox
@     msgbox SomeText, MSGBOX_DEFAULT
@     setspeaker 0                         @ clears the namebox (narration follows
@                                          @ in the same open message box)
@ The buffered speaker persists while the message box stays open and is
@ cleared automatically by closemessage / script end (HideFieldMessageBox).
@ Names render in FONT_SMALL, max 8 tiles (64 px) wide - keep them short.
@ ---------------------------------------------------------------------------

@ Story characters
Pelagios_Speaker_Mom::
	.string "MOM$"
Pelagios_Speaker_Sollis::
	.string "SOLLIS$"
Pelagios_Speaker_Cass::
	.string "CASS$"
Pelagios_Speaker_Sever::
	.string "SEVER$"

@ Gym leaders
Pelagios_Speaker_Petra::
	.string "PETRA$"
Pelagios_Speaker_Forge::
	.string "FORGE$"
Pelagios_Speaker_Rook::
	.string "ROOK$"
Pelagios_Speaker_Cinder::
	.string "CINDER$"
Pelagios_Speaker_Slag::
	.string "SLAG$"
Pelagios_Speaker_Vex::
	.string "VEX$"
Pelagios_Speaker_Solace::
	.string "SOLACE$"

@ Named minor trainers
Pelagios_Speaker_Jay::
	.string "JAY$"
Pelagios_Speaker_OldBren::
	.string "OLD BREN$"
Pelagios_Speaker_Mace::
	.string "MACE$"
Pelagios_Speaker_Finn::
	.string "FINN$"
Pelagios_Speaker_Crag::
	.string "CRAG$"
Pelagios_Speaker_Venn::
	.string "VENN$"
Pelagios_Speaker_Hale::
	.string "HALE$"
Pelagios_Speaker_Bryn::
	.string "BRYN$"
Pelagios_Speaker_Karst::
	.string "KARST$"
Pelagios_Speaker_Ostin::
	.string "OSTIN$"
Pelagios_Speaker_Wrenna::
	.string "WRENNA$"
Pelagios_Speaker_Vael::
	.string "VAEL$"
Pelagios_Speaker_Senna::
	.string "SENNA$"

@ Emberveil cult trainers
Pelagios_Speaker_Sera::
	.string "SERA$"
Pelagios_Speaker_Bren::
	.string "BREN$"
Pelagios_Speaker_Mira::
	.string "MIRA$"
Pelagios_Speaker_Rem::
	.string "REM$"
Pelagios_Speaker_Voss::
	.string "VOSS$"
Pelagios_Speaker_Tane::
	.string "TANE$"
Pelagios_Speaker_Horne::
	.string "HORNE$"
Pelagios_Speaker_Cael::
	.string "CAEL$"

@ Generic role names (unnamed NPCs whose lines are pure direct speech)
Pelagios_Speaker_Guard::
	.string "GUARD$"
Pelagios_Speaker_Soldier::
	.string "SOLDIER$"
Pelagios_Speaker_Officer::
	.string "OFFICER$"
Pelagios_Speaker_Sailor::
	.string "SAILOR$"
Pelagios_Speaker_Fisherman::
	.string "FISHERMAN$"
Pelagios_Speaker_Merchant::
	.string "MERCHANT$"
Pelagios_Speaker_Clerk::
	.string "CLERK$"
Pelagios_Speaker_Innkeeper::
	.string "INNKEEPER$"
Pelagios_Speaker_Traveler::
	.string "TRAVELER$"
Pelagios_Speaker_Apprentice::
	.string "APPRENTICE$"
Pelagios_Speaker_Elder::
	.string "ELDER$"
Pelagios_Speaker_OldWoman::
	.string "OLD WOMAN$"
Pelagios_Speaker_Woman::
	.string "WOMAN$"
Pelagios_Speaker_Child::
	.string "CHILD$"
Pelagios_Speaker_Boy::
	.string "BOY$"
Pelagios_Speaker_Citizen::
	.string "CITIZEN$"
Pelagios_Speaker_CassMom::
	.string "CASS'S MOM$"
Pelagios_Speaker_Devotee::
	.string "DEVOTEE$"
Pelagios_Speaker_Worshipper::
	.string "WORSHIPPER$"
Pelagios_Speaker_Attendant::
	.string "ATTENDANT$"
Pelagios_Speaker_Historian::
	.string "HISTORIAN$"

@ Sirocco Isle - story characters and gym leaders
@ (CRAG, SERA, VOSS reused from the blocks above - do not redefine.)
Pelagios_Speaker_Dex::
	.string "DEX$"
Pelagios_Speaker_Dagan::
	.string "DAGAN$"
Pelagios_Speaker_Silt::
	.string "SILT$"
Pelagios_Speaker_Miria::
	.string "MIRIA$"

@ Sirocco Isle - named trainers
Pelagios_Speaker_Rael::
	.string "RAEL$"
Pelagios_Speaker_Yoss::
	.string "YOSS$"
Pelagios_Speaker_Wex::
	.string "WEX$"
Pelagios_Speaker_Pell::
	.string "PELL$"
Pelagios_Speaker_Mave::
	.string "MAVE$"
Pelagios_Speaker_Thorn::
	.string "THORN$"
Pelagios_Speaker_Fen::
	.string "FEN$"
Pelagios_Speaker_Rusk::
	.string "RUSK$"
Pelagios_Speaker_Lace::
	.string "LACE$"

@ Sirocco Isle - generic roles
Pelagios_Speaker_Enforcer::
	.string "ENFORCER$"
Pelagios_Speaker_GiltClaw::
	.string "GILT CLAW$"
Pelagios_Speaker_Scholar::
	.string "SCHOLAR$"
Pelagios_Speaker_Broker::
	.string "BROKER$"
