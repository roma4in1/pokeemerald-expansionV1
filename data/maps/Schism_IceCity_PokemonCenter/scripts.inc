Schism_IceCity_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

@ -------------------------------------------------------------------
@ Nurse - standard heal. (Whiteout respawn is the FrostmarkPort Inn,
@ the island's north heal location.)
@ -------------------------------------------------------------------
SchismIceCityPokemonCenter_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_SCHISM_ICE_CITY_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end
