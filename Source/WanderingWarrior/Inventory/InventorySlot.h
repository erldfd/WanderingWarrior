// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "ItemData.h"

#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlot : public UObject
{
	GENERATED_BODY()
	
public:

	int32 GetSlotIndex() const;
	void SetSlotIndex(int32 NewSlotIndex);

	void UseSlotItem();

	const FItemDataRow& GetSlotItemData() const;
	void SetSlotItemData(const FItemDataRow& NewSlotItemData);

	bool IsEmpty() const;

	void SetOwner(class AWWCharacter* NewOwner);

	int32 GetSlotItemCount() const;
	void SetSlotItemCount(int32 NewSlotItemCount);

private:

	void UseWeaponItem();
	void UseMiscItem();

private:

	UPROPERTY()
	int32 SlotIndex;

	int32 SlotItemCount;

	FItemDataRow SlotItemData;

	UPROPERTY()
	TObjectPtr<class AWWCharacter> Owner;
};
