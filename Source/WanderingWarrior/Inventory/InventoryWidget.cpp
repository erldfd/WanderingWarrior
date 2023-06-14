// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryTabData.h"
#include "InventorySlotWidget.h"
#include "WWConstContainer.h"
#include "WWEnumClassContainer.h"
#include "InventoryTabButtonWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget NativeOnInitailized"));
}

TArray<UInventorySlotWidget*>& UInventoryWidget::GetSlotWidgetArray(ETabType TabType)
{
	return TabButtonArray[FMath::Clamp(int(TabType), 0, TabButtonArray.Num()-1)]->GetSlotWidgetArray();
}

void UInventoryWidget::SetSlotWidgetImageFromTexture(ETabType TabType, int SlotIndex, UTexture2D* SlotTexture)
{
	const TArray<UInventorySlotWidget*>& SlotWidgetArray = TabButtonArray[FMath::Clamp(int(TabType), 0, TabButtonArray.Num())]->GetSlotWidgetArray();

	SetSlotWidgetImageFromTexture(SlotWidgetArray, SlotIndex, SlotTexture);
}

void UInventoryWidget::SetSlotWidgetImageFromTexture(const TArray<UInventorySlotWidget*>& SlotWidgetArray, int SlotIndex, UTexture2D* SlotTexture)
{
	if (SlotIndex < 0 || SlotIndex > SlotWidgetArray.Num() - 1)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget, SetSlotWidgetImageFromTextureInternal SlotIndex - Out of Index"));
		return;
	}

	check(SlotWidgetArray.IsValidIndex(SlotIndex));
	check(SlotWidgetArray[SlotIndex] != nullptr);

	if (SlotTexture == nullptr)
	{
		SlotTexture = EmptySlotTexture;
	}
	
	SlotWidgetArray[SlotIndex]->GetSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotWidgetArray[SlotIndex]->GetDragSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotWidgetArray[SlotIndex]->GetDragSlotImage()->SetDesiredSizeOverride(FVector2D(170, 115));
}

void UInventoryWidget::SetSlotWidgetImageFromTexture(UInventorySlotWidget*& SlotWidget, UTexture2D* SlotTexture)
{
	if (SlotTexture == nullptr)
	{
		SlotTexture = EmptySlotTexture;
	}

	SlotWidget->GetSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotWidget->GetDragSlotImage()->SetBrushFromTexture(SlotTexture);
	SlotWidget->GetDragSlotImage()->SetDesiredSizeOverride(FVector2D(170, 115));
}

void UInventoryWidget::SetSlotItemCountText(int SlotItemCount, int SlotIndex, ETabType TabType)
{
	FText ItemCountText;

	if (SlotItemCount > 0)
	{
		ItemCountText = FText::FromString(FString::FromInt(SlotItemCount));
	}
	else
	{
		ItemCountText = FText();
	}

	TabButtonArray[int(TabType)]->GetSlotItemCountTextArray()[SlotIndex]->SetText(ItemCountText);
}

void UInventoryWidget::InitNullTabButtonArray(int ArrayCount)
{
	TabButtonArray.Init(nullptr, ArrayCount);
}

TArray<UInventoryTabButtonWidget*>& UInventoryWidget::GetTabButtonArray()
{
	return TabButtonArray;
}

void UInventoryWidget::InitInventoryWidget(const FInventoryWidgetSettings& Settings)
{
	int TabCount = Settings.TabCount;
	int SlotCount = Settings.SlotCountByTab;
	TArray<ETabType> TabTypeArray = Settings.TabTypeArray;

	TabButtonArray.Init(nullptr, TabCount);

	for (int i = 0; i < TabCount; ++i)
	{
		FString WidgetNameString("TabButton_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		TabButtonArray[i] = Cast<UInventoryTabButtonWidget>(GetWidgetFromName(WidgetName));
		check(TabButtonArray[i]);

		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, InitInventoryWidget, TabButtonArray%d is null? : %d, WidgetName : %s"), i, TabButtonArray[i] == nullptr, *WidgetName.ToString());
		TabButtonArray[i]->InitSlotWidgetArray(SlotCount);
		TabButtonArray[i]->InitSlotItemCountTextArray(SlotCount);
		TabButtonArray[i]->SetTabType(TabTypeArray[i]);

		TArray<UInventorySlotWidget*>& TabSlotWidgetArray = TabButtonArray[i]->GetSlotWidgetArray();
		TArray<UTextBlock*>& TabItemCountTextArray = TabButtonArray[i]->GetSlotItemCountTextArray();

		for (int j = 0; j < TabSlotWidgetArray.Num(); ++j)
		{
			if (TabSlotWidgetArray.IsValidIndex(j) == false)
			{
				break;
			}

			WidgetNameString = FString("SlotWidget_");
			WidgetNameString.Append(FString::FromInt(i));
			WidgetNameString.Append("_");
			WidgetNameString.Append(FString::FromInt(j));
			WidgetName = FName(WidgetNameString);

			UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(GetWidgetFromName(WidgetName));
			check(SlotWidget);

			TabSlotWidgetArray[j] = SlotWidget;
			TabSlotWidgetArray[j]->GetSlotImage()->SetBrushFromTexture(EmptySlotTexture);
			TabSlotWidgetArray[j]->GetDragSlotImage()->SetBrushFromTexture(EmptySlotTexture);
			TabSlotWidgetArray[j]->OnLeftMouseButtonUpDelegate.BindUObject(this, &UInventoryWidget::OnWeaponTabSlotClicked);
			TabSlotWidgetArray[j]->SetSlotIndex(j);
			TabSlotWidgetArray[j]->SetTabTypeBelongTo(TabTypeArray[i]);
			TabSlotWidgetArray[j]->SetInventoryBelongTo(InventoryType);

			WidgetNameString = FString("ItemCount_");
			WidgetNameString.Append(FString::FromInt(i));
			WidgetNameString.Append("_");
			WidgetNameString.Append(FString::FromInt(j));
			WidgetName = FName(WidgetNameString);

			UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));
			check(TextBlock);

			TabItemCountTextArray[j] = TextBlock;
			TabItemCountTextArray[j]->SetText(FText());
		}

		TabButtonArray[i]->OnTabButtonClickedDelegate.BindUObject(this, &UInventoryWidget::ConvertTab);
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, InitInventoryWidget, TabButton_%d Onclicked is Bound? : %d"), i, TabButtonArray[i]->OnTabButtonClickedDelegate.IsBound());
	}

	if (TabButtonArray.Num() > 1)
	{
		InventoryWidgetTabSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("InventoryWidgetTabSwitcher")));
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, InitInventoryWidget, InventoryWidgetTabSwitcher is null? : %d"), InventoryWidgetTabSwitcher == nullptr);
	}
}

void UInventoryWidget::SetInventoryType(EInventory NewInventoryType)
{
	InventoryType = NewInventoryType;
}

void UInventoryWidget::OnWeaponTabSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnWeaponTabSlotClicked"));

	auto WeaponTabSlotWidgetArray = TabButtonArray[int(ETabType::WeaponTab)]->GetSlotWidgetArray();
	auto WeaponTabItemCountTextArray = TabButtonArray[int(ETabType::WeaponTab)]->GetSlotItemCountTextArray();

	if (WeaponTabSlotWidgetArray.IsValidIndex(SlotIndex) == false ||
		WeaponTabSlotWidgetArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnWeaponTabSlotClicked, It is invailed index"));
		return;
	}

	if (OnSlotImageWidgetClickedDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnSlotImageWidgetClickedDelegate is not bound."));
		return;
	}

	 OnSlotImageWidgetClickedDelegate.Execute(SlotIndex);
}

void UInventoryWidget::OnMiscTabSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget : OnMiscTabSlotClicked"));

	auto MiscTabSlotWidgetArray = TabButtonArray[int(ETabType::MiscTab)]->GetSlotWidgetArray();
	auto MiscTabItemCountTextArray = TabButtonArray[int(ETabType::MiscTab)]->GetSlotItemCountTextArray();

	if (MiscTabSlotWidgetArray.IsValidIndex(SlotIndex) == false ||
		MiscTabSlotWidgetArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnMiscTabSlotClicked, It is invailed index"));
		return;
	}

	if (OnSlotImageWidgetClickedDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnSlotImageWidgetClickedDelegate is not bound."));
		return;
	}

	OnSlotImageWidgetClickedDelegate.Execute(SlotIndex);
}

void UInventoryWidget::ConvertTab(int TabNum)
{
	InventoryWidgetTabSwitcher->SetActiveWidgetIndex(TabNum);

	if (OnConvertTabDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnConvertTabDelegate is not bound."));
		return;
	}

	OnConvertTabDelegate.Execute(TabNum);
}

void UInventoryWidget::ConvertToWeaponTab()
{
	InventoryWidgetTabSwitcher->SetActiveWidgetIndex(0);

	if (OnConvertTabDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnConvertTabDelegate is not bound."));
		return;
	}

	OnConvertTabDelegate.Execute(0);
}

void UInventoryWidget::ConvertToMiscTab()
{
	InventoryWidgetTabSwitcher->SetActiveWidgetIndex(1);

	if (OnConvertTabDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnConvertTabDelegate is not bound."));
		return;
	}

	OnConvertTabDelegate.Execute(1);
}
