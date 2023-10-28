// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotWidgetData.h"

int32 UInventorySlotWidgetData::GetSlotIndex() const
{
	if (SlotIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidgetData::GetSlotIndex, SlotIndex < 0, Wrong Index Returned."));
	}

	return SlotIndex;
}

void UInventorySlotWidgetData::SetSlotIndex(int32 NewSlotIndex)
{
	if (NewSlotIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidgetData::SetSlotIndex, Slot Index Must be at least 0"));
		return;
	}

	SlotIndex = NewSlotIndex;
}

int32 UInventorySlotWidgetData::GetSlotItemCount() const
{
	return SlotItemCount;
}

void UInventorySlotWidgetData::SetSlotItemCount(int32 NewItemCount)
{
	if (NewItemCount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidgetData::SetSlotItemCount, SlotItemCount Must be at least 0"));
		return;
	}

	SlotItemCount = NewItemCount;
}

TObjectPtr<UTexture2D> UInventorySlotWidgetData::GetSlotTexture() const
{
	return SlotTexture;
}

void UInventorySlotWidgetData::SetSlotTexture(UTexture2D& NewTexture)
{
	SlotTexture = &NewTexture;
}
