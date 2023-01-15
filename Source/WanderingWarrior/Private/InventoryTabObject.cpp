// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTabObject.h"

#include "InventorySlotObject.h"
#include "AItem.h"

UInventoryTabObject::UInventoryTabObject() : SlotCount(12)
{
	InventorySlotArray.Init(nullptr, SlotCount);

	FString SlotNameString("Slot");

	for (int i = 0; i < SlotCount; ++i)
	{
		if (InventorySlotArray.IsValidIndex(i))
		{
			SlotNameString.Append(FString::FromInt(i));
			FName SlotName(*SlotNameString);

			InventorySlotArray[i] = CreateDefaultSubobject<UInventorySlotObject>(SlotName);
			InventorySlotArray[i]->SetSlotIndex(i);
			UE_LOG(LogTemp, Warning, TEXT("InvnetoryTabobject SetSlotIndexNumber : %d"), InventorySlotArray[i]->GetSlotIndex());
		}
	}
}

int UInventoryTabObject::GetSlotCount()
{
	return SlotCount;
}

UInventorySlotObject* UInventoryTabObject::GetSlotFromIndex(int Index)
{
	ensure(InventorySlotArray.IsValidIndex(Index));
	return InventorySlotArray[Index];
}

UInventorySlotObject* UInventoryTabObject::GetEmptySlot()
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

UInventorySlotObject* UInventoryTabObject::GetItemHoldableSlot(AAItem* Item)
{
	for (int i = 0; i < InventorySlotArray.Num(); ++i)
	{
		if (InventorySlotArray.IsValidIndex(i) == false)
		{
			continue;
		}

		if (InventorySlotArray[i]->IsEmpty() ||
			InventorySlotArray[i]->GetSlotItem()->GetMaxItemCount() > InventorySlotArray[i]->GetHoldedItemCount())
		{
			return InventorySlotArray[i];
		}
	}

	return nullptr;
}

ETabType UInventoryTabObject::GetTabType()
{
	return TabType;
}

void UInventoryTabObject::SetTabType(ETabType NewTabType)
{
	TabType = NewTabType;
	UE_LOG(LogTemp, Warning, TEXT("TabType : %d"), TabType);
}