// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AInventory.h"

#include "CharacterQuickSlot.generated.h"

enum class EWeaponName;
enum class EMiscItemName;
enum class ETabType;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UCharacterQuickSlot : public UAInventory
{
	GENERATED_BODY()
	
public:

	UCharacterQuickSlot();

	virtual bool ObtainItem(EWeaponName WeaponName) override;
	virtual bool ObtainItem(EMiscItemName MiscItemName) override;
	virtual bool ObtainItem(const struct FItemDataRow& NewItemData, class UInventoryTabObject* Tab) override;

	virtual bool UseSlotItemFormSlotIndex(int Index) override;
	virtual bool UseSlotItemFromSlot(class UInventorySlotObject*& Slot) override;

	virtual void ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OnSlotImageWidgetClicked(int SlotIndex);

private:

	UPROPERTY()
	TObjectPtr<class UInventorySlotObject> TempSwapSlot;
};
