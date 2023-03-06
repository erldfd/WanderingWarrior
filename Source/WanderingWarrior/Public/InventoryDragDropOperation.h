// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/DragDropOperation.h"

#include "InventoryDragDropOperation.generated.h"

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

	enum class ETabType GetTabTypeBelongTo();
	void SetTabTypeBelongTo(enum class ETabType TabType);

private:

	int StartDragSlotIndex;
	enum class ETabType TabTypeBelongTo;
};
