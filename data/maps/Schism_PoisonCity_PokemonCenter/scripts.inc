Schism_PoisonCity_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

@ -------------------------------------------------------------------
@ Nurse - standard heal. (Whiteout respawn is the VenomquayPort Inn,
@ the island's south heal location.)
@ -------------------------------------------------------------------
SchismPoisonCityPokemonCenter_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_SCHISM_POISON_CITY_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end
