// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventorySlotWidgetImage.h"
#include "InventoryTabObject.h"
#include "InventorySlotWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"

#define WEAPON_TAB_SLOT_COUNT 12
#define MISC_TAB_SLOT_COUNT 12

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("InvenntoryWidget NativeOnInitailized"));
	//WeaponTabSlotImageArray.Init(nullptr, WEAPON_TAB_SLOT_COUNT);
	WeaponTabSlotWidgetArray.Init(nullptr, WEAPON_TAB_SLOT_COUNT);
	WeaponTabItemCountTextArray.Init(nullptr, WEAPON_TAB_SLOT_COUNT);

	for (int i = 0; i < WeaponTabSlotWidgetArray.Num(); ++i)
	{
		if (WeaponTabSlotWidgetArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("WeaponTabSlotWidget_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(GetWidgetFromName(WidgetName));

		WeaponTabSlotWidgetArray[i] = SlotWidget;
		WeaponTabSlotWidgetArray[i]->GetSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		WeaponTabSlotWidgetArray[i]->GetDragSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		WeaponTabSlotWidgetArray[i]->OnLeftMouseButtonUpDelegate.BindUObject(this, &UInventoryWidget::OnWeaponTabSlotClicked);
		WeaponTabSlotWidgetArray[i]->SetSlotIndex(i);
		WeaponTabSlotWidgetArray[i]->SetTabTypeBelongTo(ETabType::WeaponTab);

		WidgetNameString = FString("WeaponTabItemCount_");
		WidgetNameString.Append(FString::FromInt(i));
		WidgetName = FName(WidgetNameString);

		UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));

		WeaponTabItemCountTextArray[i] = TextBlock;
		WeaponTabItemCountTextArray[i]->SetText(FText());
	}
	
	MiscTabSlotWidgetArray.Init(nullptr, MISC_TAB_SLOT_COUNT);
	MiscTabItemCountTextArray.Init(nullptr, MISC_TAB_SLOT_COUNT);

	for (int i = 0; i < MiscTabSlotWidgetArray.Num(); ++i)
	{
		if (MiscTabSlotWidgetArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("MiscTabSlotWidget_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(GetWidgetFromName(WidgetName));

		MiscTabSlotWidgetArray[i] = SlotWidget;
		//MiscTabSlotWidgetArray[i]->GetSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		//MiscTabSlotWidgetArray[i]->GetDragSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		MiscTabSlotWidgetArray[i]->OnLeftMouseButtonUpDelegate.BindUObject(this, &UInventoryWidget::OnMiscTabSlotClicked);
		MiscTabSlotWidgetArray[i]->SetSlotIndex(i);
		MiscTabSlotWidgetArray[i]->SetTabTypeBelongTo(ETabType::MiscTab);

		WidgetNameString = FString("MiscTabItemCount_");
		WidgetNameString.Append(FString::FromInt(i));
		WidgetName = FName(WidgetNameString);

		UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));
		MiscTabItemCountTextArray[i] = TextBlock;
		MiscTabItemCountTextArray[i]->SetText(FText());
	}

	/*MiscTabSlotArray.Init(nullptr, 12);

	for (int i = 0; i < MiscTabSlotArray.Num(); ++i)
	{
		if (MiscTabSlotArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("MiscSlot_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UButton* SlotButton = Cast<UButton>(GetWidgetFromName(WidgetName));

		MiscTabSlotArray[i] = SlotButton;
		FButtonStyle ButtonStyle;
		FSlateBrush Brush;

		Brush.SetResourceObject(EmptySlotTexture);
		ButtonStyle.SetNormal(Brush);
		ButtonStyle.SetHovered(Brush);
		ButtonStyle.SetPressed(Brush);

		//MiscTabSlotArray[i]->SetStyle(ButtonStyle);
		MiscTabSlotArray[i]->OnClicked.AddDynamic(this, &UInventoryWidget::OnMiscTabSlotClicked);
	//	MiscTabSlotArray[i]->OnHovered.AddDynamic(this, &UInventoryWidget::OnMiscTabSlotClicked);

		UE_LOG(LogTemp, Warning, TEXT("MiscButtonSlot is Bound? : %d, WidgetName : %s"), MiscTabSlotArray[i]->OnClicked.IsBound(), *MiscTabSlotArray[i]->GetName());
		MiscTabSlotImageArray[i]->SetSlotIndex(i);
	}*/

	//TestSlotImage = Cast<UInventorySlotWidgetImage>(GetWidgetFromName(TEXT("TestSlot")));
	//TestSlotImage->OnClickedDelegate.BindUObject(this, &UInventoryWidget::OnMiscTabSlotClicked);

	//MiscTabSlotImageArray[9]->SetColorAndOpacity(FLinearColor::Black);
	WeaponTabButton->OnClicked.AddDynamic(this, &UInventoryWidget::ConvertToWeaponTab);
	UE_LOG(LogTemp, Warning, TEXT("WeaponTabButton Onclicked is Bound? : %d"), WeaponTabButton->OnClicked.IsBound());
	MiscTabButton->OnClicked.AddDynamic(this, &UInventoryWidget::ConvertToMiscTab);
	UE_LOG(LogTemp, Warning, TEXT("MiscTabButton Onclicked is Bound? : %d"), MiscTabButton->OnClicked.IsBound());
}

TArray<UInventorySlotWidget*> UInventoryWidget::GetSlotWidgetArray(ETabType TabType)
{
	switch (TabType)
	{
	case ETabType::WeaponTab:

		return WeaponTabSlotWidgetArray;

	case ETabType::MiscTab:

		return MiscTabSlotWidgetArray;

	default:

		UE_LOG(LogTemp, Warning, TEXT("GetSlotImageArray Failed.. Return WeaponTabslotImageArray"));
		return WeaponTabSlotWidgetArray;
	}
}

void UInventoryWidget::SetSlotWidgetImageFromTexture(ETabType TabType, int SlotIndex, UTexture2D* SlotTexture)
{
	TArray<UInventorySlotWidget*> SlotWidgetArray;

	switch (TabType)
	{
	case ETabType::WeaponTab:
		SlotWidgetArray = WeaponTabSlotWidgetArray;
		break;
	case ETabType::MiscTab:
		SlotWidgetArray = MiscTabSlotWidgetArray;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("SetSlotImageFromTexture Failed.. WeaponTabslotWidgetArray is modified"));
		SlotWidgetArray = WeaponTabSlotWidgetArray;
		break;
	}

	SetSlotWidgetImageFromTextureInternal(SlotWidgetArray, SlotIndex, SlotTexture);
}

void UInventoryWidget::SetSlotWidgetImageFromTextureInternal(TArray<UInventorySlotWidget*> SlotWidgetArray, int SlotIndex, UTexture2D* SlotTexture)
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

	switch (TabType)
	{
	case ETabType::WeaponTab:
		if (SlotIndex >= WEAPON_TAB_SLOT_COUNT)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Out of Index - WeaponTab"));
			break;
		}

		WeaponTabItemCountTextArray[SlotIndex]->SetText(ItemCountText);
		break;
	case ETabType::MiscTab:
		if (SlotIndex >= MISC_TAB_SLOT_COUNT)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Out of Index - MiscTab"));
			break;
		}

		MiscTabItemCountTextArray[SlotIndex]->SetText(ItemCountText);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Not Available Tab"));
		break;
	}
}

void UInventoryWidget::OnWeaponTabSlotClicked(int SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget : OnWeaponTabSlotClicked"));

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
