// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"

#include "InventorySlotWidgetImage.h"

#include "Components/TextBlock.h"

#define QUICK_SLOT_COUNT 8

void UQuickSlotWidget::NativeOnInitialized()
{
	QuickSlotImageArray.Init(nullptr, QUICK_SLOT_COUNT);
	QuickSlotItemCountTextArray.Init(nullptr, QUICK_SLOT_COUNT);

	for (int i = 0; i < QuickSlotImageArray.Num(); ++i)
	{
		if (QuickSlotImageArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("QuickSlot_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidgetImage* SlotImage = Cast<UInventorySlotWidgetImage>(GetWidgetFromName(WidgetName));

		QuickSlotImageArray[i] = SlotImage;
		QuickSlotImageArray[i]->SetBrushFromTexture(EmptySlotTexture);
		//QuickSlotImageArray[i]->OnClickedDelegate.BindUObject(this, &UQuickSlotWidget::OnWeaponTabSlotClicked);
		QuickSlotImageArray[i]->SetSlotIndex(i);

		WidgetNameString = FString("QuickSlotItemCountText_");
		WidgetNameString.Append(FString::FromInt(i));
		WidgetName = FName(WidgetNameString);

		UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));

		QuickSlotItemCountTextArray[i] = TextBlock;
		QuickSlotItemCountTextArray[i]->SetText(FText());
	}
}

void UQuickSlotWidget::SetSlotImageFromTexture(int SlotIndex, UTexture2D* SlotTexture)
{
	TArray<UInventorySlotWidgetImage*> SlotImageArray;

	SlotImageArray = QuickSlotImageArray;

	SetSlotImageFromTextureInternal(SlotImageArray, SlotIndex, SlotTexture);
}

void UQuickSlotWidget::SetSlotImageFromTextureInternal(TArray<UInventorySlotWidgetImage*> SlotImageArray, int SlotIndex, UTexture2D* SlotTexture)
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

	SlotImageArray[SlotIndex]->SetBrushFromTexture(SlotTexture);
	SlotImageArray[SlotIndex]->SetVisibility(ESlateVisibility::Hidden);
	SlotImageArray[SlotIndex]->SetVisibility(ESlateVisibility::Visible);//이미지 새로고침이 왜 안될까.. 그래서 임시방편으로 넣음
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
