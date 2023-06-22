
#pragma once

UENUM()
enum class ETabType : uint8
{
	WeaponTab,
	MiscTab,
	QuickSlotTab = 0,
	AllTab = 0
};

UENUM()
enum class EItemType : uint8
{
	Weapon,
	Misc
};

UENUM()
enum class EWeaponName : uint8
{
	BlackSword UMETA(DisplayName = "BlackSword"),
	WhiteSword UMETA(DisplayName = "WhiteSword")
};

UENUM()
enum class EMiscItemName : uint8
{
	HPPotion UMETA(DisplayName = "HPPotion"),
	MPPotion UMETA(DisplayName = "MPPotion")
};

UENUM()
enum class EInventory : uint8
{
	CharacterInventory,
	CharacterQuickSlot,
	MarchantInventory,
	EmptySpace = 255,
};

UENUM()
enum class ECreditOwner : uint8
{
	Player,
	Marchant1
};

UENUM()
enum class EDropItem : uint8
{
	None,
	BlackSword,
	WhiteSword,
	HPPotion
};