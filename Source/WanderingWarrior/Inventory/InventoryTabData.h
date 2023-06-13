// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventoryTabData.generated.h"

enum class ETabType : uint8;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryTabData : public UObject
{
	GENERATED_BODY()

public:

	UInventoryTabData();

	//virtual void PostInitProperties() override;

	int GetSlotCount();
	void InitSlots(int NewSlotCount);

	class UInventorySlotData*& GetSlotFromIndex(int Index);

	//SlotIndex is Getted Slot Index, if Failed to Get Slot, SlotIndex is -1.
	class UInventorySlotData* GetEmptySlot();

	class UInventorySlotData* GetHoldableItemSlot();

	ETabType GetTabType();
	void SetTabType(ETabType NewTabType);

private:

	int SlotCount;

	UPROPERTY()
	TArray<class UInventorySlotData*> InventorySlotArray;

	ETabType TabType;
};
