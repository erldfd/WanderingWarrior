// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryTabObject.h"
#include "InventorySlotWidget.h"
#include "WWConstContainer.h"
#include "WWEnumClassContainer.h"
#include "InventoryTabButton.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget NativeOnInitailized"));
	/*TabButtonArray.Emplace(Cast<UInventoryTabButton>(GetWidgetFromName(TEXT("WeaponTabButton"))));
	TabButtonArray[int(ETabType::WeaponTab)]->InitSlotWidgetArray(SlotCount::WEAPON_TAB_SLOT_COUNT);
	TabButtonArray[int(ETabType::WeaponTab)]->InitSlotItemCountTextArray(SlotCount::WEAPON_TAB_SLOT_COUNT);

	TArray<UInventorySlotWidget*>& WeaponTabSlotWidgetArray = TabButtonArray[int(ETabType::WeaponTab)]->GetSlotWidgetArray();
	TArray<UTextBlock*>& WeaponTabItemCountTextArray = TabButtonArray[int(ETabType::WeaponTab)]->GetSlotItemCountTextArray();

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
	}*/
	/*WeaponTabSlotWidgetArray.Init(nullptr, SlotCount::WEAPON_TAB_SLOT_COUNT);
	WeaponTabItemCountTextArray.Init(nullptr, SlotCount::WEAPON_TAB_SLOT_COUNT);*/

	/*for (int i = 0; i < WeaponTabSlotWidgetArray.Num(); ++i)
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
	}*/
	
	/*TabButtonArray.Emplace(Cast<UInventoryTabButton>(GetWidgetFromName(TEXT("MiscTabButton"))));
	TabButtonArray[int(ETabType::MiscTab)]->InitSlotWidgetArray(SlotCount::WEAPON_TAB_SLOT_COUNT);
	TabButtonArray[int(ETabType::MiscTab)]->InitSlotItemCountTextArray(SlotCount::WEAPON_TAB_SLOT_COUNT);
	
	TArray<UInventorySlotWidget*>& MiscTabSlotWidgetArray = TabButtonArray[int(ETabType::MiscTab)]->GetSlotWidgetArray();
	TArray<UTextBlock*>& MiscTabItemCountTextArray = TabButtonArray[int(ETabType::MiscTab)]->GetSlotItemCountTextArray();

	MiscTabSlotWidgetArray.Init(nullptr, SlotCount::MISC_TAB_SLOT_COUNT);
	MiscTabItemCountTextArray.Init(nullptr, SlotCount::MISC_TAB_SLOT_COUNT);

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
		MiscTabSlotWidgetArray[i]->GetSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		MiscTabSlotWidgetArray[i]->GetDragSlotImage()->SetBrushFromTexture(EmptySlotTexture);
		MiscTabSlotWidgetArray[i]->OnLeftMouseButtonUpDelegate.BindUObject(this, &UInventoryWidget::OnMiscTabSlotClicked);
		MiscTabSlotWidgetArray[i]->SetSlotIndex(i);
		MiscTabSlotWidgetArray[i]->SetTabTypeBelongTo(ETabType::MiscTab);

		WidgetNameString = FString("MiscTabItemCount_");
		WidgetNameString.Append(FString::FromInt(i));
		WidgetName = FName(WidgetNameString);

		UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(WidgetName));
		MiscTabItemCountTextArray[i] = TextBlock;
		MiscTabItemCountTextArray[i]->SetText(FText());
	}*/

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
	/*if (TabButtonArray.Num() > 1)
	{
		InventoryWidgetTabSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("InventoryWidgetTabSwitcher")));

		TabButtonArray[int(ETabType::WeaponTab)]->OnClicked.AddDynamic(this, &UInventoryWidget::ConvertToWeaponTab);
		UE_LOG(LogTemp, Warning, TEXT("WeaponTabButton Onclicked is Bound? : %d"), TabButtonArray[int(ETabType::WeaponTab)]->OnClicked.IsBound());
		TabButtonArray[int(ETabType::MiscTab)]->OnClicked.AddDynamic(this, &UInventoryWidget::ConvertToMiscTab);
		UE_LOG(LogTemp, Warning, TEXT("MiscTabButton Onclicked is Bound? : %d"), TabButtonArray[int(ETabType::MiscTab)]->OnClicked.IsBound());
	}*/
}

TArray<UInventorySlotWidget*> UInventoryWidget::GetSlotWidgetArray(ETabType TabType)
{
	return TabButtonArray[FMath::Clamp(int(TabType), 0, TabButtonArray.Num()-1)]->GetSlotWidgetArray();

	/*switch (TabType)
	{
	case ETabType::WeaponTab:

		return WeaponTabSlotWidgetArray;

	case ETabType::MiscTab:

		return MiscTabSlotWidgetArray;

	default:

		UE_LOG(LogTemp, Warning, TEXT("GetSlotImageArray Failed.. Return WeaponTabslotImageArray"));
		return WeaponTabSlotWidgetArray;
	}*/
}

void UInventoryWidget::SetSlotWidgetImageFromTexture(ETabType TabType, int SlotIndex, UTexture2D* SlotTexture)
{
	const TArray<UInventorySlotWidget*>& SlotWidgetArray = TabButtonArray[FMath::Clamp(int(TabType), 0, TabButtonArray.Num())]->GetSlotWidgetArray();

	/*switch (TabType)
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
	}*/

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
	/*switch (TabType)
	{
	case ETabType::WeaponTab:
		if (SlotIndex >= SlotCount::WEAPON_TAB_SLOT_COUNT)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Out of Index - WeaponTab"));
			break;
		}

		WeaponTabItemCountTextArray[SlotIndex]->SetText(ItemCountText);
		break;
	case ETabType::MiscTab:
		if (SlotIndex >= SlotCount::MISC_TAB_SLOT_COUNT)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Out of Index - MiscTab"));
			break;
		}

		MiscTabItemCountTextArray[SlotIndex]->SetText(ItemCountText);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, SetSlotItemCountText, Not Available Tab"));
		break;
	}*/
}

void UInventoryWidget::InitNullTabButtonArray(int ArrayCount)
{
	TabButtonArray.Init(nullptr, ArrayCount);
}

TArray<UInventoryTabButton*> UInventoryWidget::GetTabButtonArray()
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
		TabButtonArray[i] = Cast<UInventoryTabButton>(GetWidgetFromName(WidgetName));
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
