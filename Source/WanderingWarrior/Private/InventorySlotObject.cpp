// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotObject.h"

#include "AItem.h"

#include "Engine/Texture2D.h"

UInventorySlotObject::UInventorySlotObject() : HeldItemCount(0)
{
}

AAItem* UInventorySlotObject::GetSlotItem()
{
	return SlotItem;
}

void UInventorySlotObject::SetSlotItem(AAItem* NewItem)
{
	SlotItem = NewItem;
}

bool UInventorySlotObject::IsEmpty()
{
	return (SlotItem == nullptr);
}

int UInventorySlotObject::GetSlotIndex()
{
	return SlotIndex;
}

void UInventorySlotObject::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

int UInventorySlotObject::GetHoldedItemCount()
{
	return HeldItemCount;
}

bool UInventorySlotObject::SetHoldedItemCount(int NewCount)
{
	if (NewCount < 0 || NewCount > SlotItem->GetMaxItemCount())
	{
		return false;
	}

	HeldItemCount = NewCount;

	return true;
}

bool UInventorySlotObject::UseSlotItem()
{
	if (HeldItemCount <= 0 || SlotItem == nullptr)
	{
		return false;
	}

 	SlotItem->Use();
	HeldItemCount--;
	UE_LOG(LogTemp, Warning, TEXT("HOldedItemCount : %d"), HeldItemCount);
	if (HeldItemCount == 0)
	{
		SlotItem = nullptr;
	}

	return true;
}