// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "WWGameInstance.h"
#include "ItemData.h"

#include "UObject/NoExportTypes.h"

#include "InventorySlotData.generated.h"

struct FItemDataRow;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotData : public UObject
{
	GENERATED_BODY()

public:

	UInventorySlotData();

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
