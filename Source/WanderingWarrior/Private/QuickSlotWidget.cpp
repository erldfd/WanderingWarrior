// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"

#include "InventorySlotWidgetImage.h"

void UQuickSlotWidget::NativeOnInitialized()
{
	QuickSlotImageArray.Init(nullptr, 8);

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
}
