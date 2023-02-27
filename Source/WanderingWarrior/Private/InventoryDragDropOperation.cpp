// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDragDropOperation.h"

#include "InventoryTabObject.h"

UInventoryDragDropOperation::UInventoryDragDropOperation() : StartDragSlotIndex(-1)
{

}

int UInventoryDragDropOperation::GetStartDragSlotIndex()
{
	return StartDragSlotIndex;
}

void UInventoryDragDropOperation::SetStartDragSlotIndex(int NewIndex)
{
	StartDragSlotIndex = NewIndex;
}

ETabType UInventoryDragDropOperation::GetTabTypeBelongTo()
{
	return TabTypeBelongTo;
}

void UInventoryDragDropOperation::SetTabTypeBelongTo(ETabType TabType)
{
	TabTypeBelongTo = TabType;
}
