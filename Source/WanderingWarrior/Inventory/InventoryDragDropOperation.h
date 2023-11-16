// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/DragDropOperation.h"

#include "InventoryDragDropOperation.generated.h"

enum class EInventory : uint8;
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
	
public:

	EInventory InventoryType;
	int32 SlotIndex;
};
