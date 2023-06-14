// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AInventory.h"

#include "MarchantInventory.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UMarchantInventory : public UAInventory
{
	GENERATED_BODY()
	
public:

	UMarchantInventory();

	virtual bool ObtainItem(EWeaponName WeaponName) override;
	virtual bool ObtainItem(EMiscItemName MiscItemName) override;
	virtual bool ObtainItem(const struct FItemDataRow& NewItemData, class UInventoryTabData* Tab) override;

	virtual void RemoveAllItem(int32 SlotIndex) override;

	virtual bool UseSlotItemFormSlotIndex(int32 Index) override;
	virtual bool UseSlotItemFromSlot(class UInventorySlotData*& Slot) override;

	virtual void ExchangeOrMoveSlotItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, ETabType DragSlotTabType) override;
};
