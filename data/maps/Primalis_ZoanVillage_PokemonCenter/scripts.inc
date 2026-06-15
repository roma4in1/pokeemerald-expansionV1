Primalis_ZoanVillage_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

@ ===================================================================
@ Nurse - standard heal. (Whiteout respawn is the Verdant Landing
@ Inn, the island's only heal location.)
@ ===================================================================
PrimalisZoanVillagePokemonCenter_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_PRIMALIS_VILLAGE_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end
