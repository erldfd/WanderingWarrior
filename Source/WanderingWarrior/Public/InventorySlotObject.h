// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventorySlotObject.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotObject : public UObject
{
	GENERATED_BODY()

public:

	UInventorySlotObject();

	class AAItem* GetSlotItem();
	void SetSlotItem(class AAItem* NewItem);

	bool IsEmpty();

	int GetSlotIndex();
	void SetSlotIndex(int NewIndex);

	int GetHoldedItemCount();
	bool SetHoldedItemCount(int NewCount);

	bool UseSlotItem();

private:

	UPROPERTY()
	TObjectPtr<class AAItem> SlotItem;

	int SlotIndex;
	int HeldItemCount;
};
