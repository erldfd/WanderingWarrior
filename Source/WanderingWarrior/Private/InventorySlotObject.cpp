// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotObject.h"

#include "AItem.h"

#include "Engine/Texture2D.h"

UInventorySlotObject::UInventorySlotObject()
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
	return HoldedItemCount;
}

bool UInventorySlotObject::SetHoldedItemCount(int NewCount)
{
	if (NewCount < 0 || NewCount > SlotItem->GetMaxItemCount())
	{
		return false;
	}

	HoldedItemCount = NewCount;

	return true;
}

bool UInventorySlotObject::UseSlotItem()
{
	if (HoldedItemCount <= 0)
	{
		return false;
	}

	SlotItem->Use();
	HoldedItemCount--;

	if (HoldedItemCount == 0)
	{
		SlotItem == nullptr;
	}

	return true;
}