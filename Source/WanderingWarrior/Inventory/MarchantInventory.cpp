// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchantInventory.h"

#include "WanderingWarrior/WWEnumClassContainer.h"
#include "InventoryComponent.h"
#include "WanderingWarrior/WWConstContainer.h"
#include "InventoryTabObject.h"

UMarchantInventory::UMarchantInventory()
{
	InventoryType = EInventory::MarchantInventory;

	InventoryComponent->InitTabArray(TabCount::QUICKSLOT_TAB_COUNT);

	TArray<UInventoryTabObject*>& TabArray = InventoryComponent->GetTabArray();

	TabArray[0] = CreateDefaultSubobject<UInventoryTabObject>(TEXT("AllTab"));
	check(TabArray[0] != nullptr);
	TabArray[0]->InitSlots(SlotCount::MARCHANT_SLOT_COUNT);
	TabArray[0]->SetTabType(ETabType::AllTab);

	InventoryComponent->SetCurrentActivatedTab(TabArray[0]);
}

bool UMarchantInventory::ObtainItem(EWeaponName WeaponName)
{
	return false;
}

bool UMarchantInventory::ObtainItem(EMiscItemName MiscItemName)
{
	return false;
}

bool UMarchantInventory::ObtainItem(const struct FItemDataRow& NewItemData, class UInventoryTabObject* Tab)
{
	return false;
}

bool UMarchantInventory::UseSlotItemFormSlotIndex(int Index)
{
	return false;
}

bool UMarchantInventory::UseSlotItemFromSlot(class UInventorySlotObject*& Slot)
{
	return false;
}

void UMarchantInventory::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType)
{

}