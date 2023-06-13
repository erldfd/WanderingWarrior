// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

#include "Components/CharacterStatComponent.h"
#include "Inventory/InventoryWidget.h"
#include "WWConstContainer.h"
#include "WWEnumClassContainer.h"
#include "ConversationWidget.h"
#include "Inventory/InventoryItemInfoWidget.h"
#include "Inventory/InventoryTabButtonWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UInGameWidget::SetEnemyNameTextBlock(FText NewText)
{
	check(EnemyNameTextBlock != nullptr);
	EnemyNameTextBlock->SetText(NewText);

	EnemyNameTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UInGameWidget::SetEnemyHPBarPercent(float Percent)
{
	check(EnemyHPBar != nullptr);
	EnemyHPBar->SetPercent(Percent);

	EnemyHPBar->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(HideEnemyHPAndNameWidgetTimerHandle, FTimerDelegate::CreateUObject(this, &UInGameWidget::HideEnemyHPAndNameWidget), 3, false);
}

void UInGameWidget::SetMyHPBarPercent(float Percent)
{
	check(MyHPBar != nullptr);
	MyHPBar->SetPercent(Percent);
}

UInventoryWidget* UInGameWidget::GetQuickSlotWidget()
{
	check(QuickSlotWidget != nullptr);
	return QuickSlotWidget;
}

UInventoryWidget* UInGameWidget::GetInventoryWidget()
{
	check(InventoryWidget != nullptr);
	return InventoryWidget;
}

UInventoryWidget* UInGameWidget::GetMarchantInventoryWidget()
{
	check(MarchantInventoryWidget);
	return MarchantInventoryWidget;
}

UConversationWidget* UInGameWidget::GetConversationWidget()
{
	check(ConversationWidget);
	return ConversationWidget;
}

UInventoryItemInfoWidget* UInGameWidget::GetInventoryItemInfoWidget()
{
	check(InventoryItemInfoWidget);
	return InventoryItemInfoWidget;
}

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ensure(MyHPBar != nullptr);
	MyHPBar->SetPercent(1);

	FInventoryWidgetSettings Settings;
	Settings.TabCount = TabCount::INVENTORY_TAB_COUNT;
	Settings.SlotCountByTab = SlotCount::WEAPON_TAB_SLOT_COUNT;
	Settings.InventoryType = EInventory::CharacterInventory;
	Settings.TabTypeArray.Init(ETabType::WeaponTab, TabCount::INVENTORY_TAB_COUNT);
	
	Settings.TabTypeArray[1] = ETabType::MiscTab;

	ensure(InventoryWidget != nullptr);
	InventoryWidget->SetInventoryType(EInventory::CharacterInventory);
	InventoryWidget->InitInventoryWidget(Settings);

	Settings.TabCount = TabCount::QUICKSLOT_TAB_COUNT;
	Settings.SlotCountByTab = SlotCount::QUICK_SLOT_COUNT;
	Settings.InventoryType = EInventory::CharacterQuickSlot;
	Settings.TabTypeArray.Init(ETabType::QuickSlotTab, TabCount::QUICKSLOT_TAB_COUNT);

	ensure(QuickSlotWidget != nullptr);
	QuickSlotWidget->SetInventoryType(EInventory::CharacterQuickSlot);
	QuickSlotWidget->InitInventoryWidget(Settings);

	Settings.TabCount = TabCount::DEFUALT_TAB_COUNT;
	Settings.SlotCountByTab = SlotCount::MARCHANT_SLOT_COUNT;
	Settings.InventoryType = EInventory::MarchantInventory;
	Settings.TabTypeArray.Init(ETabType::QuickSlotTab, TabCount::QUICKSLOT_TAB_COUNT);

	ensure(MarchantInventoryWidget != nullptr);
	MarchantInventoryWidget->SetInventoryType(EInventory::MarchantInventory);
	MarchantInventoryWidget->InitInventoryWidget(Settings);

	HideEnemyHPAndNameWidget();

	FWidgetTransform Transform;
	Transform.Translation = FVector2D(100, 100);
	check(InventoryItemInfoWidget);
	InventoryItemInfoWidget->SetRenderTransform(Transform);

	//InventoryWidget->GetTabButtonArray()[0]->GetSlotWidgetArray();
}

void UInGameWidget::HideEnemyHPAndNameWidget()
{
	EnemyNameTextBlock->SetVisibility(ESlateVisibility::Hidden); 
	EnemyHPBar->SetVisibility(ESlateVisibility::Hidden);
}