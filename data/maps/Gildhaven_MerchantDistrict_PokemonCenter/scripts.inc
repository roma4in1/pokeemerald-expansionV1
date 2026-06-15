Gildhaven_MerchantDistrict_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

@ -------------------------------------------------------------------
@ Nurse - standard heal. (Whiteout respawn is the Goldport Inn, the
@ island's only heal location.)
@ -------------------------------------------------------------------
GildhavenMerchantDistrictPokemonCenter_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_GILDHAVEN_CITY_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

@ -------------------------------------------------------------------
@ Counter shopkeeper - sells at honest prices. Lampshades the contrast
@ with the street stalls outside.
@ -------------------------------------------------------------------
GildhavenMerchantDistrictPokemonCenter_EventScript_Shopkeeper::
	lock
	faceplayer
	setspeaker Pelagios_Speaker_Clerk
	message GildhavenMerchantDistrictPokemonCenter_Text_ShopGreeting
	waitmessage
	pokemart GildhavenMerchantDistrictPokemonCenter_Pokemart
	setspeaker Pelagios_Speaker_Clerk
	msgbox GildhavenMerchantDistrictPokemonCenter_Text_ShopFarewell, MSGBOX_DEFAULT
	setspeaker 0
	release
	end

	.align 2
GildhavenMerchantDistrictPokemonCenter_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_NONE
	release
	end

GildhavenMerchantDistrictPokemonCenter_Text_ShopGreeting:
	.string "Counter prices. Fixed. Posted.\p"
	.string "No haggling, no genuine artifacts,\n"
	.string "no surprises.\p"
	.string "I know. After the stalls out there it\n"
	.string "feels almost suspicious, doesn't it.$"

GildhavenMerchantDistrictPokemonCenter_Text_ShopFarewell:
	.string "Fair value, every time. Wild concept\n"
	.string "for this city.$"
