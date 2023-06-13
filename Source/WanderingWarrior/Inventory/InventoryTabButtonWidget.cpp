// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTabButtonWidget.h"

UInventoryTabButtonWidget::UInventoryTabButtonWidget()
{
	OnClicked.AddDynamic(this, &UInventoryTabButtonWidget::OnTabButtonClicked);
	UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, UInventoryTabButtonWidget, OnClicked is bound? : %d"), OnClicked.IsBound());
}

void UInventoryTabButtonWidget::InitSlotWidgetArray(int SlotWidgetCount)
{
	check(SlotWidgetCount > 0);
	SlotWidgetArray.Init(nullptr, SlotWidgetCount);
}

void UInventoryTabButtonWidget::InitSlotItemCountTextArray(int SlotItemCountArrayCount)
{
	check(SlotItemCountArrayCount > 0);
	SlotItemCountTextArray.Init(nullptr, SlotItemCountArrayCount);
}

TArray<class UInventorySlotWidget*>& UInventoryTabButtonWidget::GetSlotWidgetArray()
{
	check(SlotWidgetArray.IsValidIndex(0));
	return SlotWidgetArray;
}

TArray<class UTextBlock*>& UInventoryTabButtonWidget::GetSlotItemCountTextArray()
{
	check(SlotItemCountTextArray.IsValidIndex(0));
	return SlotItemCountTextArray;
}

void UInventoryTabButtonWidget::SetTabType(ETabType NewTabType)
{
	TabType = NewTabType;
}

void UInventoryTabButtonWidget::OnTabSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, OnWeaponTabSlotClicked"));
}

void UInventoryTabButtonWidget::OnTabButtonClicked()
{
	if (OnTabButtonClickedDelegate.IsBound())
	{
		OnTabButtonClickedDelegate.Execute(int(TabType));
		UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, OnTabButtonClicked, OnTabButtonClickedDelegate.Execute(int(%d))"), int(TabType));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, OnTabButtonClicked, OnTabButtonClickedDelegate is NOT bound"));
	}
	
}