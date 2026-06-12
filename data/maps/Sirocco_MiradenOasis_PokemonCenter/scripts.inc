Sirocco_MiradenOasis_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Sirocco_MiradenOasis_PokemonCenter_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

@ The island's whiteout respawn stays at the Dustmouth inn - the
@ oasis center heals but doesn't host beds.
Sirocco_MiradenOasis_PokemonCenter_OnTransition:
	setrespawn HEAL_LOCATION_SIROCCO_DUSTMOUTH_PORT
	end

SiroccoMiradenOasisPokemonCenter_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_SIROCCO_OASIS_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end
