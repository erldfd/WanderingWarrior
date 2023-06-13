// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AInventory.h"

#include "CharacterInventory.generated.h"

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

	virtual void RemoveAllItem(int32 SlotIndex) override;

	virtual bool UseSlotItemFormSlotIndex(int32 Index) override;
	virtual bool UseSlotItemFromSlot(class UInventorySlotObject*& Slot) override;

	void OpenAndCloseInventory();

	bool IsInventoryVisible();

	virtual void ExchangeOrMoveSlotItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, ETabType DragSlotTabType) override;

	void SetItemInfoWidget(class UInventoryItemInfoWidget& InItemInfoWidget);

public:

	FOnOpenAndCloseInventoryDelegate OnOpenAndCloseInventoryDelegate;

protected:

	virtual void OnComponentCreated() override;
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OnSlotImageWidgetClicked(int32 SlotIndex);

	void OnConvertTab(int32 TabIndex);

	void OnMouseEnterToSlotWidget(int32 SlotIndex);

	void OnMouseLeaveToSlotWidget(int32 SlotIndex);

	void ShowItemInfoWidget(const struct FItemDataRow& ItemData);

private:

	UPROPERTY()
	TObjectPtr<class UInventorySlotObject> TempSwapSlot;

	uint8 bIsMouseEnterToSlotWidget : 1;

	FTimerHandle ItemInfoWidgetTimeHandler;

	TObjectPtr<class UInventoryItemInfoWidget> InventoryItemInfoWidget;
};
