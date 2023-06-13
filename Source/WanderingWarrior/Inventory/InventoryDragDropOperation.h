// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/DragDropOperation.h"

#include "InventoryDragDropOperation.generated.h"

enum class ETabType : uint8;
enum class EInventory : uint8;
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UInventoryDragDropOperation();

	int GetStartDragSlotIndex();
	void SetStartDragSlotIndex(int NewIndex);

	ETabType GetTabTypeBelongTo();
	void SetTabTypeBelongTo(ETabType TabType);
	EInventory GetInventoryBelongTo();
	void SetInventoryBelongTo(EInventory NewInventory);

private:

	int StartDragSlotIndex;
	ETabType TabTypeBelongTo;
	EInventory InventoryBelongTo;
};
