// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InventorySlotWidgetData.generated.h"

// params : UInventorySlotWidgetData* UpdatedInventorySlotWidgetData, UInventorySlotWidget* EntryWidget
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTileViewItemUpdateSignature, class UInventorySlotWidgetData* /*UpdatedInventorySlotWidgetData*/, class UInventorySlotWidget* /*EntryWidget*/);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotWidgetData : public UObject
{
	GENERATED_BODY()
	
public:

	int32 GetSlotIndex() const;
	void SetSlotIndex(int32 NewSlotIndex);

	int32 GetSlotItemCount() const;
	void SetSlotItemCount(int32 NewItemCount);

	TObjectPtr<UTexture2D> GetSlotTexture() const;
	void SetSlotTexture(class UTexture2D& NewTexture);

public:

	// params : UInventorySlotWidgetData* UpdatedInventorySlotWidgetData, UInventorySlotWidget* EntryWidget
	FOnTileViewItemUpdateSignature OnTileViewItemUpdateSignature;

private:

	int32 SlotIndex;

	int32 SlotItemCount;

	UPROPERTY()
	TObjectPtr<class UTexture2D> SlotTexture;
	
};
