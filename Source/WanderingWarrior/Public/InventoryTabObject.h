// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventoryTabObject.generated.h"

enum class ETabType
{
	WeaponTab,
	MiscTab,
	QuickSlotTab
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryTabObject : public UObject
{
	GENERATED_BODY()

public:

	UInventoryTabObject();

	//virtual void PostInitProperties() override;

	int GetSlotCount();
	void SetSlotCount(int NewSlotCount);

	class UInventorySlotObject* GetSlotFromIndex(int Index);

	//SlotIndex is Getted Slot Index, if Failed to Get Slot, SlotIndex is -1.
	class UInventorySlotObject* GetEmptySlot();

	class UInventorySlotObject* GetItemHoldableSlot(class AAItem* Item);

	ETabType GetTabType();
	void SetTabType(ETabType NewTabType);

private:

	int SlotCount;

	UPROPERTY()
	TArray<class UInventorySlotObject*> InventorySlotArray;

	ETabType TabType;
};
