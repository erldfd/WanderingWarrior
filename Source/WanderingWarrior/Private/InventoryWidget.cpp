// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventorySlotWidgetImage.h"
#include "InventoryTabObject.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Styling/SlateBrush.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Warning, TEXT("InvenntoryWidget NativeOnInitailized"));
	WeaponTabSlotImageArray.Init(nullptr, 12);

	for (int i = 0; i < WeaponTabSlotImageArray.Num(); ++i)
	{
		if (WeaponTabSlotImageArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("WeaponTabSlotImage_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidgetImage* SlotImage = Cast<UInventorySlotWidgetImage>(GetWidgetFromName(WidgetName));

		WeaponTabSlotImageArray[i] = SlotImage;
		WeaponTabSlotImageArray[i]->SetBrushFromTexture(EmptySlotTexture);
		WeaponTabSlotImageArray[i]->OnClickedDelegate.BindUObject(this, &UInventoryWidget::OnWeaponTabSlotClicked);
		WeaponTabSlotImageArray[i]->SetSlotIndex(i);
	}
	
	MiscTabSlotImageArray.Init(nullptr, 12);
	
	for (int i = 0; i < MiscTabSlotImageArray.Num(); ++i)
	{
		if (MiscTabSlotImageArray.IsValidIndex(i) == false)
		{
			break;
		}

		FString WidgetNameString("MiscTabSlotImage_");
		WidgetNameString.Append(FString::FromInt(i));
		FName WidgetName(WidgetNameString);

		UInventorySlotWidgetImage* SlotImage = Cast<UInventorySlotWidgetImage>(GetWidgetFromName(WidgetName));

		MiscTabSlotImageArray[i] = SlotImage;
		MiscTabSlotImageArray[i]->SetBrushFromTexture(EmptySlotTexture);
		MiscTabSlotImageArray[i]->OnClickedDelegate.BindUObject(this, &UInventoryWidget::OnMiscTabSlotClicked);

		UE_LOG(LogTemp, Warning, TEXT("OnMiscTabSlotclicked is Bound? : %d, WidgetName : %s"), MiscTabSlotImageArray[i]->OnClickedDelegate.IsBound(), *MiscTabSlotImageArray[i]->GetName());
		MiscTabSlotImageArray[i]->SetSlotIndex(i);
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

TArray<UInventorySlotWidgetImage*> UInventoryWidget::GetSlotImageArray(ETabType TabType)
{
	switch (TabType)
	{
	case ETabType::WeaponTab:

		return WeaponTabSlotImageArray;

	case ETabType::MiscTab:

		return MiscTabSlotImageArray;

	default:

		UE_LOG(LogTemp, Warning, TEXT("GetSlotImageArray Failed.. Return WeaponTabslotImageArray"));
		return WeaponTabSlotImageArray;
	}
}

void UInventoryWidget::SetSlotImageFromTexture(ETabType TabType, int SlotIndex, UTexture2D* SlotTexture)
{
	TArray<UInventorySlotWidgetImage*> SlotImageArray;

	switch (TabType)
	{
	case ETabType::WeaponTab:
		SlotImageArray = WeaponTabSlotImageArray;
		break;
	case ETabType::MiscTab:
		SlotImageArray = MiscTabSlotImageArray;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("SetSlotImageFromTexture Failed.. WeaponTabslotImageArray is modified"));
		SlotImageArray = WeaponTabSlotImageArray;
		break;
	}

	SetSlotImageFromTextureInternal(SlotImageArray, SlotIndex, SlotTexture);
}

void UInventoryWidget::SetSlotImageFromTextureInternal(TArray<UInventorySlotWidgetImage*> SlotImageArray, int SlotIndex, UTexture2D* SlotTexture)
{
	if (SlotIndex < 0 || SlotIndex > SlotImageArray.Num() - 1)
	{
		UE_LOG(LogTemp, Error, TEXT("SlotIndex - Out of Index"));
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

void UInventoryWidget::OnWeaponTabSlotClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget : OnWeaponTabSlotClicked"));
	for (int i = 0; i < WeaponTabSlotImageArray.Num(); ++i)
	{
		if (WeaponTabSlotImageArray.IsValidIndex(i) == false ||
			WeaponTabSlotImageArray[i] == nullptr ||
			WeaponTabSlotImageArray[i]->GetIsClicked() == false)
		{
			continue;
		}

		WeaponTabSlotImageArray[i]->SetIsClicked(false);

		if (OnSlotImageWidgetClickedDelegate.IsBound() == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnSlotImageWidgetClickedDelegate is not bound."));
			return;
		}

		OnSlotImageWidgetClickedDelegate.Execute(WeaponTabSlotImageArray[i]->GetSlotIndex());
	}
}

void UInventoryWidget::OnMiscTabSlotClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget : OnMiscTabSlotClicked"));
	for (int i = 0; i < MiscTabSlotImageArray.Num(); ++i)
	{
		if (MiscTabSlotImageArray.IsValidIndex(i) == false ||
			MiscTabSlotImageArray[i] == nullptr ||
			MiscTabSlotImageArray[i]->GetIsClicked() == false)
		{
			continue;
		}

		MiscTabSlotImageArray[i]->SetIsClicked(false);

		if (OnSlotImageWidgetClickedDelegate.IsBound() == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget, OnSlotImageWidgetClickedDelegate is not bound."));
			return;
		}

		OnSlotImageWidgetClickedDelegate.Execute(MiscTabSlotImageArray[i]->GetSlotIndex());
	}
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