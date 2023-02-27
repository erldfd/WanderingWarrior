// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

#include "CharacterStatComponent.h"
#include "QuickSlotWidget.h"

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

UQuickSlotWidget* UInGameWidget::GetQuickSlotWidget()
{
	check(QuickSlotWidget != nullptr);
	return QuickSlotWidget;
}

UInventoryWidget* UInGameWidget::GetInventoryWidget()
{
	check(InventoryWidget != nullptr);
	return InventoryWidget;
}

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ensure(MyHPBar != nullptr);
	MyHPBar->SetPercent(1);

	HideEnemyHPAndNameWidget();
}

void UInGameWidget::HideEnemyHPAndNameWidget()
{
	EnemyNameTextBlock->SetVisibility(ESlateVisibility::Hidden); 
	EnemyHPBar->SetVisibility(ESlateVisibility::Hidden);
}