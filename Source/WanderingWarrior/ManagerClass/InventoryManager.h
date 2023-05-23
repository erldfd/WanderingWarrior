// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventoryManager.generated.h"

enum class EInventory;
enum class ETabType;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:

	UInventoryManager();

	void ExchangeOrMoveInventoryItem(int DragStartSlotIndex, int DragEndSlotIndex, 
		int DragStartInventory, int DragEndInventory, 
		int DragStartSlotTabType, int DragEndSlotTabType);

	void AddToInventoryArray(EInventory InventoryType, class UAInventory* Inventory);
	void InitManager();

	void BindFunctionToDragDropDelegate(EInventory InventoryType, ETabType InventoryTabType);

private:

	UFUNCTION()
	void OnDragDropInventoryItem(int DragStartSlotIndex, int DragEndSlotIndex, 
		int DragStartInventory, int DragEndInventory, 
		int DragStartSlotTabType, int DragEndSlotTabType);

	void ExchangeOrMoveInventoryItemInternal(class UInventorySlotObject* DragStartSlot, class UInventorySlotObject* DragEndSlot, 
		EInventory DragStartInventory, EInventory DragEndInventory,
		ETabType DragStartSlotTabType, ETabType DragEndSlotTabType);

private:

	UPROPERTY()
	TArray<TObjectPtr<class UAInventory>> InventoryArray;

	TObjectPtr<class UInventorySlotObject> TempSwapSlot;
};
