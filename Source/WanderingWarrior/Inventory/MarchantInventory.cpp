// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchantInventory.h"

#include "WanderingWarrior/WWEnumClassContainer.h"
#include "InventoryComponent.h"
#include "WanderingWarrior/WWConstContainer.h"
#include "InventoryTabObject.h"
#include "InventoryWidget.h"
#include "InventorySlotObject.h"
#include "InventorySlotWidget.h"
#include "WanderingWarrior/WWGameInstance.h"

#include "Kismet/GameplayStatics.h"

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
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);

	FItemDataRow* ItemData = GameInstance->GetWeaponData(WeaponName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMarchantInventory::ObtainItem, ItemData == nullptr"));
		return false;
	}

	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[int(ETabType::WeaponTab)]);
}

bool UMarchantInventory::ObtainItem(EMiscItemName MiscItemName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);

	FItemDataRow* ItemData = GameInstance->GetMiscItemData(MiscItemName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMarchantInventory::ObtainItem, ItemData == nullptr"));
		return false;
	}


	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[int(ETabType::MiscTab)]);
}

bool UMarchantInventory::ObtainItem(const FItemDataRow& NewItemData, class UInventoryTabObject* Tab)
{
	check(InventoryWidget);

	UInventorySlotObject* Slot = Tab->GetHoldableItemSlot();

	ETabType TabType = Tab->GetTabType();

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMarchantInventory::ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot->SetSlotItemData(NewItemData);

	int SlotIndex = Slot->GetSlotIndex();
	InventoryWidget->SetSlotWidgetImageFromTexture(TabType, SlotIndex, NewItemData.SlotTexture);

	Slot->SetHeldItemCount(Slot->GetHeldItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), SlotIndex, TabType);

	TArray<UInventorySlotWidget*>& SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(TabType);
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

void UMarchantInventory::RemoveAllItem(int32 SlotIndex)
{
	UInventorySlotObject*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(SlotIndex);

	Slot->ClearSlotItem();
}

bool UMarchantInventory::UseSlotItemFormSlotIndex(int Index)
{
	UInventorySlotObject*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

bool UMarchantInventory::UseSlotItemFromSlot(UInventorySlotObject*& Slot)
{
	if (Slot == nullptr || Slot->GetHeldItemCount() == 0)
	{
		return false;
	}

	UWorld* World = GetWorld();
	check(World);

	Slot->UseSlotItem(*World);

	ETabType CurrentActivatedTabType = InventoryComponent->GetCurrentActivatedTab()->GetTabType();

	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), Slot->GetSlotIndex(), CurrentActivatedTabType);

	if (Slot->GetHeldItemCount() == 0)
	{
		int SlotIndex = Slot->GetSlotIndex();

		check(InventoryWidget);
		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, Slot->GetSlotIndex());

		TArray<UInventorySlotWidget*>& SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
		SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(true);
	}

	return true;
}

void UMarchantInventory::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType)
{

}