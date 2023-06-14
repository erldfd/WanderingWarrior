// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTabData.h"

#include "InventorySlotData.h"
#include "Item/AItem.h"
#include "ItemData.h"
#include "WWConstContainer.h"

UInventoryTabData::UInventoryTabData() : SlotCount(0)
{

}

int UInventoryTabData::GetSlotCount()
{
	return SlotCount;
}

void UInventoryTabData::InitSlots(int32 NewSlotCount)
{
	SlotCount = NewSlotCount;

	InventorySlotArray.Init(nullptr, SlotCount);

	FString SlotNameString("Slot");

	for (int i = 0; i < SlotCount; ++i)
	{
		if (InventorySlotArray.IsValidIndex(i))
		{
			SlotNameString.Append(FString::FromInt(i));
			FName SlotName(*SlotNameString);

			InventorySlotArray[i] = NewObject<UInventorySlotData>();
			InventorySlotArray[i]->SetSlotIndex(i);
		}
	}
}

UInventorySlotData*& UInventoryTabData::GetSlotFromIndex(int32 Index)
{
	return InventorySlotArray[Index];
}

UInventorySlotData* UInventoryTabData::GetEmptySlot()
{
	for (int i = 0; i < InventorySlotArray.Num(); ++i)
	{
		if (InventorySlotArray.IsValidIndex(i) == false || InventorySlotArray[i]->IsEmpty() == false)
		{
			continue;
		}

		return InventorySlotArray[i];
	}

	return nullptr;
}

UInventorySlotData* UInventoryTabData::GetHoldableItemSlot()
{
	for (int i = 0; i < InventorySlotArray.Num(); ++i)
	{
		if (InventorySlotArray.IsValidIndex(i) == false)
		{
			continue;
		}

		if (InventorySlotArray[i]->IsEmpty() ||
			InventorySlotArray[i]->GetSlotItemData().MaxItemCount > InventorySlotArray[i]->GetHeldItemCount())
		{
			return InventorySlotArray[i];
		}
	}

	return nullptr;
}

ETabType UInventoryTabData::GetTabType()
{
	return TabType;
}

void UInventoryTabData::SetTabType(ETabType NewTabType)
{
	TabType = NewTabType;
	UE_LOG(LogTemp, Warning, TEXT("TabType : %d"), TabType);
}