// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"

#include "InventorySlotWidget.h"
#include "InventorySlotWidgetImage.h"
#include "InventoryTabObject.h"

#include "Components/TextBlock.h"

#define QUICK_SLOT_COUNT 8

void UQuickSlotWidget::NativeOnInitialized()
{
	QuickSlotWidgetArray.Init(nullptr, QUICK_SLOT_COUNT);
	QuickSlotItemCountTextArray.Init(nullptr, QUICK_SLOT_COUNT);

	for (int i = 0; i < QuickSlotWidgetArray.Num(); ++i)
	{
		if (QuickSlotWidgetArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("QuickSlotWidget_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(GetWidgetFromName(WidgetName));

		QuickSlotWidgetArray[i] = SlotWidget;
		QuickSlotWidgetArray[i]->GetSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		QuickSlotWidgetArray[i]->GetDragSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		QuickSlotWidgetArray[i]->OnLeftMouseButtonUpDelegate.BindUObject(this, &UQuickSlotWidget::OnQuickSlotClicked);
		QuickSlotWidgetArray[i]->SetSlotIndex(i);
		QuickSlotWidgetArray[i]->SetTabTypeBelongTo(ETabType::QuickSlotTab);

		WidgetNameString = FString("QuickSlotItemCountText_");
		WidgetNameString.Append(FString::FromInt(i));
		WidgetName = FName(WidgetNameString);

		UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));

		QuickSlotItemCountTextArray[i] = TextBlock;
		QuickSlotItemCountTextArray[i]->SetText(FText());
	}
}

void UQuickSlotWidget::SetSlotWidgetImageFromTexture(int SlotIndex, UTexture2D* SlotTexture)
{
	TArray<UInventorySlotWidget*> SlotImageArray;

	SlotImageArray = QuickSlotWidgetArray;

	SetSlotWidgetImageFromTextureInternal(SlotImageArray, SlotIndex, SlotTexture);
}

void UQuickSlotWidget::SetSlotWidgetImageFromTextureInternal(TArray<UInventorySlotWidget*> SlotImageArray, int SlotIndex, UTexture2D* SlotTexture)
{
	if (SlotIndex < 0 || SlotIndex > SlotImageArray.Num() - 1)
	{
		UE_LOG(LogTemp, Error, TEXT("QuickSlotWidget, SetSlotImageFromTextureInternal : SlotIndex - Out of Index"));
		return;
	}

	check(SlotImageArray.IsValidIndex(SlotIndex));
	check(SlotImageArray[SlotIndex] != nullptr);

	if (SlotTexture == nullptr)
	{
		SlotTexture = EmptySlotTexture;
	}

	SlotImageArray[SlotIndex]->GetSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotImageArray[SlotIndex]->GetDragSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotImageArray[SlotIndex]->GetDragSlotImage()->SetBrushSize(FVector2D(170, 115));
	//SlotImageArray[SlotIndex]->SetVisibility(ESlateVisibility::Hidden);
	//SlotImageArray[SlotIndex]->SetVisibility(ESlateVisibility::Visible);//이미지 새로고침이 왜 안될까.. 그래서 임시방편으로 넣음
}

void UQuickSlotWidget::SetSlotItemCountText(int SlotItemCount, int SlotIndex)
{
	if (SlotIndex >= QUICK_SLOT_COUNT)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Out of Index - WeaponTab"));
		return;
	}

	FText ItemCountText;

	if (SlotItemCount > 0)
	{
		ItemCountText = FText::FromString(FString::FromInt(SlotItemCount));
	}
	else
	{
		ItemCountText = FText();
	}

	QuickSlotItemCountTextArray[SlotIndex]->SetText(ItemCountText);
}

TArray<class UInventorySlotWidget*> UQuickSlotWidget::GetQuickSlotWidgetArray()
{
	check(QuickSlotWidgetArray.IsEmpty() == false);
	check(QuickSlotWidgetArray.IsValidIndex(0));
	return QuickSlotWidgetArray;
}

void UQuickSlotWidget::OnQuickSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot Clicked : %d"), SlotIndex);
}
