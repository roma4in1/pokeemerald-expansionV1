HavenIsle_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HavenIsle_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

HavenIsle_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_TIDEMARK_VILLAGE
	end

HavenIslePokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_TIDEMARK_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

HavenIslePokemonCenter_1F_EventScript_Boy::
	setspeaker Pelagios_Speaker_Boy
	msgbox HavenIslePokemonCenter_1F_Text_Boy, MSGBOX_NPC
	end

HavenIslePokemonCenter_1F_Text_Boy:
	.string "The nurse here patched up a wild\n"
	.string "WINGULL once. Just because.\p"
	.string "It still visits. Sits on the roof\n"
	.string "and yells until she waves.$"
