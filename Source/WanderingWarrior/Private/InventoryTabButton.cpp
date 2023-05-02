// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTabButton.h"

UInventoryTabButton::UInventoryTabButton()
{
	OnClicked.AddDynamic(this, &UInventoryTabButton::OnTabButtonClicked);
	UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, UInventoryTabButton, OnClicked is bound? : %d"), OnClicked.IsBound());
}

void UInventoryTabButton::InitSlotWidgetArray(int SlotWidgetCount)
{
	check(SlotWidgetCount > 0);
	SlotWidgetArray.Init(nullptr, SlotWidgetCount);
}

void UInventoryTabButton::InitSlotItemCountTextArray(int SlotItemCountArrayCount)
{
	check(SlotItemCountArrayCount > 0);
	SlotItemCountTextArray.Init(nullptr, SlotItemCountArrayCount);
}

TArray<class UInventorySlotWidget*>& UInventoryTabButton::GetSlotWidgetArray()
{
	check(SlotWidgetArray.IsValidIndex(0));
	return SlotWidgetArray;
}

TArray<class UTextBlock*>& UInventoryTabButton::GetSlotItemCountTextArray()
{
	check(SlotItemCountTextArray.IsValidIndex(0));
	return SlotItemCountTextArray;
}

void UInventoryTabButton::SetTabType(ETabType NewTabType)
{
	TabType = NewTabType;
}

void UInventoryTabButton::OnTabSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, OnWeaponTabSlotClicked"));

	/*if (OnSlotImageWidgetClickedDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryTabButton, OnSlotImageWidgetClickedDelegate is not bound."));
		return;
	}

	OnSlotImageWidgetClickedDelegate.Execute(SlotIndex);*/
}

void UInventoryTabButton::OnTabButtonClicked()
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