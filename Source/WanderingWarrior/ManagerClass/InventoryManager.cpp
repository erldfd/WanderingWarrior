// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "WWEnumClassContainer.h"
#include "WWConstContainer.h"
#include "Inventory/AInventory.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventorySlotWidget.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/InventorySlotData.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryTabData.h"

#include "Components/Image.h"

UInventoryManager::UInventoryManager()
{
	if (InventoryArray.Num() < 1)
	{
		InventoryArray.Init(nullptr, 3);
	}
}

void UInventoryManager::InitManager()
{
	TempSwapSlot = NewObject<UInventorySlotData>();
}

void UInventoryManager::BindFunctionToDragDropDelegate(EInventory InventoryType, ETabType InventoryTabType)
{
	TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryArray[int(InventoryType)]->GetInventoryWidget()->GetSlotWidgetArray(InventoryTabType);

	for (int i = 0; i < SlotWidgetArray.Num(); ++i)
	{
		SlotWidgetArray[i]->OnDragDropInventoryItemDelegate.BindUObject(this, &UInventoryManager::OnDragDropInventoryItem);
		
		UE_LOG(LogTemp, Warning, TEXT("InventoryManager, BindFunctionToDragDropDelegate, OnDragDropInventoryItemDelegate is bound? : %d"), SlotWidgetArray[i]->OnDragDropInventoryItemDelegate.IsBound());
	}
}

void UInventoryManager::OnDragDropInventoryItem(int DragStartSlotIndex, int DragEndSlotIndex,
	int DragStartInventory, int DragEndInventory, int DragStartSlotTabType, int DragEndSlotTabType)
{
	ExchangeOrMoveInventoryItem(DragStartSlotIndex, DragEndSlotIndex, DragStartInventory, DragEndInventory, DragStartSlotTabType, DragEndSlotTabType);
}

void UInventoryManager::ExchangeOrMoveInventoryItem(int DragStartSlotIndex, int DragEndSlotIndex,
	int DragStartInventory, int DragEndInventory, int DragStartSlotTabType,  int DragEndSlotTabType)
{
	if (DragEndInventory == (int)EInventory::EmptySpace)
	{
		//슬롯 그림 원상복구 후 종료
		return;
	}

	if (DragStartInventory == DragEndInventory)
	{
		if (DragStartSlotIndex == DragEndSlotIndex)
		{
			return;
		}

		InventoryArray[DragStartInventory]->ExchangeOrMoveSlotItem(DragStartSlotIndex, DragEndSlotIndex, (ETabType)DragEndSlotTabType);
		return;
	}

	UInventorySlotData* DragStartSlot =
		InventoryArray[DragStartInventory]->GetInventoryComponent()->GetTabArray()[DragStartSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
	UInventorySlotData* DragEndSlot =
		InventoryArray[DragEndInventory]->GetInventoryComponent()->GetTabArray()[DragEndSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);
	check(DragStartSlot != nullptr);
	check(DragEndSlot != nullptr);

	if (DragEndInventory != (int)EInventory::CharacterInventory ||
		(DragEndSlotTabType == (int)ETabType::WeaponTab && DragStartSlot->GetSlotItemData().ItemTypeTag.GetTagName() == "Weapon") ||
		(DragEndSlotTabType == (int)ETabType::MiscTab && DragStartSlot->GetSlotItemData().ItemTypeTag.GetTagName() == "Misc"))
	{
		ExchangeOrMoveInventoryItemInternal(DragStartSlot, DragEndSlot, 
			EInventory(DragStartInventory), EInventory(DragEndInventory), 
			ETabType(DragStartSlotTabType), ETabType(DragEndSlotTabType));

		return;
	}
}

void UInventoryManager::ExchangeOrMoveInventoryItemInternal(class UInventorySlotData* DragStartSlot, class UInventorySlotData* DragEndSlot,
	EInventory DragStartInventory, EInventory DragEndInventory,
	ETabType DragStartSlotTabType, ETabType DragEndSlotTabType)
{
	UInventoryWidget* DragStartInventoryWidget = InventoryArray[(int)DragStartInventory]->GetInventoryWidget();
	UInventoryWidget* DragEndInventoryWidget = InventoryArray[(int)DragEndInventory]->GetInventoryWidget();

	UInventorySlotWidget* DragStartSlotWidget =
		DragStartInventoryWidget->GetSlotWidgetArray(DragStartSlotTabType)[DragStartSlot->GetSlotIndex()];
	UInventorySlotWidget* DragEndSlotWidget =
		DragEndInventoryWidget->GetSlotWidgetArray(DragEndSlotTabType)[DragEndSlot->GetSlotIndex()];

	if (DragEndSlot->IsEmpty())
	{
		DragEndSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());

		DragStartSlot->SetHeldItemCount(0);

		DragStartInventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotWidget);
		DragStartInventoryWidget->SetSlotItemCountText(0, DragStartSlot->GetSlotIndex(), DragStartSlotTabType);
		DragStartInventoryWidget->GetSlotWidgetArray(DragStartSlotTabType)[DragStartSlot->GetSlotIndex()]->SetIsEmptySlotImage(true);

		DragEndInventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotWidget, DragEndSlot->GetSlotItemData().SlotTexture);
		DragEndInventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlot->GetSlotIndex(), DragEndSlotTabType);
		DragEndInventoryWidget->GetSlotWidgetArray(DragEndSlotTabType)[DragEndSlot->GetSlotIndex()]->SetIsEmptySlotImage(false);
	}
	else if((DragEndSlot->GetSlotItemData().ItemTypeTag.ToString() == "Misc" && DragStartSlotTabType == ETabType::MiscTab) ||
		(DragEndSlot->GetSlotItemData().ItemTypeTag.ToString() == "Weapon" && DragStartSlotTabType == ETabType::WeaponTab))
	{
		
		TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());

		DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
		DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());

		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(TempSwapSlot->GetHeldItemCount());

		DragStartInventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotWidget, DragStartSlot->GetSlotItemData().SlotTexture);
		DragStartInventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlot->GetSlotIndex(), DragStartSlotTabType);

		DragEndInventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotWidget, DragEndSlot->GetSlotItemData().SlotTexture);
		DragEndInventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlot->GetSlotIndex(), DragEndSlotTabType);
	}
}

void UInventoryManager::AddToInventoryArray(EInventory InventoryType, UAInventory* Inventory)
{
	if (InventoryArray.IsValidIndex(int(InventoryType)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManager, AddToInventoryArray, %d is NOT Valid Index"), int(InventoryType));
		return;
	}

	if (InventoryArray.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManager, AddToInventoryArray, InventoryArray is Empty"));
		return;
	}

	InventoryArray[int(InventoryType)] = Inventory;
}