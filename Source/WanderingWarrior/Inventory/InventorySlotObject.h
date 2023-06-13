// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "WWGameInstance.h"
#include "ItemData.h"

#include "UObject/NoExportTypes.h"

#include "InventorySlotObject.generated.h"

struct FItemDataRow;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotObject : public UObject
{
	GENERATED_BODY()

public:

	UInventorySlotObject();

	const FItemDataRow& GetSlotItemData() const;
	void SetSlotItemData(const FItemDataRow& NewItemData);

	void ClearSlotItem();
	bool IsEmpty();

	const int& GetSlotIndex() const;
	void SetSlotIndex(int NewIndex);

	const int& GetHeldItemCount() const;
	bool SetHeldItemCount(int NewCount);

	bool UseSlotItem(const UWorld& World);

private:

	UPROPERTY()
	TObjectPtr<class AAItem> SlotItem;

	int SlotIndex;
	int HeldItemCount;

	FItemDataRow SlotItemData;
};
