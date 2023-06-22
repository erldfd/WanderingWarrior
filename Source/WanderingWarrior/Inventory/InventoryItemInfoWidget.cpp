// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInfoWidget.h"

#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryItemInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemInfoWidget::SetItemNameText(const FText& InText)
{
	ItemNameTextBlock->SetText(InText);
}

void UInventoryItemInfoWidget::SetBuyPriceText(const FText& InText)
{
	BuyPriceTextBlock->SetText(InText);
}

void UInventoryItemInfoWidget::SetSellPriceText(const FText& InText)
{
	SellPriceTextBlock->SetText(InText);
}

void UInventoryItemInfoWidget::SetAttackDamageText(const FText& InText)
{
	DamageTextBlock->SetText(InText);
}
