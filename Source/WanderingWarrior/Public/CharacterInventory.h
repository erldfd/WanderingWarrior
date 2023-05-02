// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AInventory.h"

#include "CharacterInventory.generated.h"

//enum class EWeaponName;
//enum class EMiscItemName;
//enum class ETabType;

//param : is inventory opened?
DECLARE_DELEGATE_OneParam(FOnOpenAndCloseInventoryDelegate, bool);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UCharacterInventory : public UAInventory
{
	GENERATED_BODY()

public:

	UCharacterInventory();

	virtual bool ObtainItem(EWeaponName WeaponName) override;
	virtual bool ObtainItem(EMiscItemName MiscItemName) override;
	virtual bool ObtainItem(const struct FItemDataRow& NewItemData, class UInventoryTabObject* Tab) override;

	virtual bool UseSlotItemFormSlotIndex(int Index) override;
	virtual bool UseSlotItemFromSlot(class UInventorySlotObject*& Slot) override;

	void OpenAndCloseInventory();

	bool IsInventoryVisible();

	virtual void ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType) override;

public:

	FOnOpenAndCloseInventoryDelegate OnOpenAndCloseInventoryDelegate;

protected:

	virtual void OnComponentCreated() override;
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OnSlotImageWidgetClicked(int SlotIndex);

	void OnConvertTab(int TabIndex);

	void OnDragDropInventorySlot(int DragStartSlotIndex, int DragEndSlotIndex, int DragStartSlotTabType, int DragEndSlotTabType);

private:

	UPROPERTY()
	TObjectPtr<class UInventorySlotObject> TempSwapSlot;
};
