// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventoryManager.generated.h"

enum class EInventory : uint8;
enum class ETabType : uint8;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:

	UInventoryManager();

	void ExchangeOrMoveInventoryItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, 
		int32 DragStartInventory, int32 DragEndInventory, 
		int32 DragStartSlotTabType, int32 DragEndSlotTabType);

	void AddToInventoryArray(EInventory InventoryType, class UAInventory* Inventory);
	void InitManager();

	void BindFunctionToDragDropDelegate(EInventory InventoryType, ETabType InventoryTabType);

private:

	UFUNCTION()
	void OnDragDropInventoryItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, 
		int32 DragStartInventory, int32 DragEndInventory, 
		int32 DragStartSlotTabType, int32 DragEndSlotTabType);

	void ExchangeOrMoveInventoryItemInternal(class UInventorySlotData& DragStartSlot, class UInventorySlotData& DragEndSlot,
		EInventory DragStartInventory, EInventory DragEndInventory,
		ETabType DragStartSlotTabType, ETabType DragEndSlotTabType);

private:

	UPROPERTY()
	TArray<TObjectPtr<class UAInventory>> InventoryArray;

	TObjectPtr<class UInventorySlotData> TempSwapSlot;
};
