// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "StoreManager.generated.h"

struct BuyAndSellArgs
{
	class UInventorySlotData& BuyerSlot;
	class UInventorySlotWidget& BuyerSlotWidget;
	int32& BuyerCredits;

	class UInventorySlotData& SellerSlot;
	class UInventorySlotWidget& SellerSlotWidget;
	int32& SellerCredits;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UStoreManager : public UObject
{
	GENERATED_BODY()
	
public:

	UStoreManager();

	bool BuyItem(BuyAndSellArgs& Args);

	bool SellItem(BuyAndSellArgs& Args);

	void SetStoreWidget(class UInventoryWidget* NewStoreWidget);

	void OpenStore(UInventoryWidget* InStoreWidget);
	//void SetStore()

private:

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> StoreWidget;

};
