
#pragma once

UENUM()
enum class ETabType
{
	WeaponTab,
	MiscTab,
	QuickSlotTab = 0,
	AllTab = 0
};

UENUM()
enum class EItemType
{
	Weapon,
	Misc
};

UENUM()
enum class EWeaponName
{
	BlackSword,
	WhiteSword
};

UENUM()
enum class EMiscItemName
{
	HPPotion,
	MPPotion
};

UENUM()
enum class EInventory
{
	CharacterInventory,
	CharacterQuickSlot,
	MarchantInventory,
	EmptySpace = 255,
};

UENUM()
enum class ECreditOwner
{
	Player,
	Marchant1
};