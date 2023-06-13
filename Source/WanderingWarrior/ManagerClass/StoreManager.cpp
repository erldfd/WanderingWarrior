// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerClass/StoreManager.h"

#include "CreditManager.h"

#include "Inventory/CharacterInventory.h"
#include "Inventory/MarchantInventory.h"
#include "Inventory/InventoryTabObject.h"
#include "Inventory/InventorySlotObject.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventorySlotWidget.h"

#include "WWEnumClassContainer.h"
#include "WWGameInstance.h"

#include "Kismet/GameplayStatics.h"


UStoreManager::UStoreManager()
{
}

bool UStoreManager::BuyItem(BuyAndSellArgs& Args)
{
	if (Args.BuyerSlot.IsEmpty() == false || Args.SellerSlot.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UStoreManager::BuyItem, BuyFailed because of BuyersSlot is NOT Empty, or SellerSlot is Empty"));
		return false;
	}

	const FItemDataRow& ItemData = Args.SellerSlot.GetSlotItemData();

	if (Args.BuyerCredits < ItemData.BuyPrice)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStoreManager::BuyItem, Not Enough Credits"));
		return false;
	}

	Args.BuyerSlot.SetSlotItemData(ItemData);
	Args.BuyerSlot.SetHeldItemCount(Args.BuyerSlot.GetHeldItemCount() + 1);

	return true;
}

bool UStoreManager::SellItem(BuyAndSellArgs& Args)
{
	return BuyItem(Args);
}

void UStoreManager::SetStoreWidget(UInventoryWidget* NewStoreWidget)
{
	StoreWidget = NewStoreWidget;
}

void UStoreManager::OpenStore()
{
	StoreWidget->SetVisibility(ESlateVisibility::Visible);
	//StoreWidget->GetSlotWidgetArray(ETabType::AllTab);
}



