// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

enum class ETabType : uint8;
enum class EInventory : uint8;

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnSlotImageWigetClickedDelegate, int);
//param : TabIndex, 0= weapontab, 1 = misctab
DECLARE_DELEGATE_OneParam(FOnConvertTabDelegate, int);
/**
 * 
 */

USTRUCT(BlueprintType)
struct FInventoryWidgetSettings
{
	GENERATED_BODY()

public:

	int TabCount;
	int SlotCountByTab;

	EInventory InventoryType;
	TArray<ETabType> TabTypeArray;
};

UCLASS()
class WANDERINGWARRIOR_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	TArray<class UInventorySlotWidget*>& GetSlotWidgetArray(ETabType TabType);

	//if SlotTexture is null, SlotTexture set to EmptySlotTexture
	void SetSlotWidgetImageFromTexture(ETabType TabType, int SlotIndex, class UTexture2D* SlotTexture = nullptr);
	void SetSlotWidgetImageFromTexture(const TArray<UInventorySlotWidget*>& SlotImageArray, int SlotIndex, class UTexture2D* SlotTexture = nullptr);
	void SetSlotWidgetImageFromTexture(UInventorySlotWidget*& SlotWidget, class UTexture2D* SlotTexture = nullptr);
	void SetSlotItemCountText(int SlotItemCount, int SlotIndex, ETabType TabType);

	void InitNullTabButtonArray(int ArrayCount);

	TArray<class UInventoryTabButton*>& GetTabButtonArray();

	void InitInventoryWidget(const FInventoryWidgetSettings& Settings);

	void SetInventoryType(EInventory NewInventoryType);

public:

	////param : SlotIndex
	FOnSlotImageWigetClickedDelegate OnSlotImageWidgetClickedDelegate;
	FOnConvertTabDelegate OnConvertTabDelegate;

protected:

	virtual void NativeOnInitialized() override;

private:

	void OnWeaponTabSlotClicked(int SlotIndex);
	void OnMiscTabSlotClicked(int SlotIndex);

	UFUNCTION()
	void ConvertTab(int TabNum);

	UFUNCTION()
	void ConvertToWeaponTab();

	UFUNCTION()
	void ConvertToMiscTab();

private:

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UInventoryTabButton>> TabButtonArray;

	/*UPROPERTY(EditAnywhere)
	TArray<> SlotWidgetArrays;*/

	/*UPROPERTY(EditAnywhere)
	TArray<TArray<class UTextBlock>> SlotItemCountTextArrays;*/

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetSwitcher> InventoryWidgetTabSwitcher;

	/*UPROPERTY(EditAnywhere)
	TObjectPtr<class UButton> WeaponTabButton;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UButton> MiscTabButton;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidget>> WeaponTabSlotWidgetArray;

	UPROPERTY()
	TArray<TObjectPtr<class UTextBlock>> WeaponTabItemCountTextArray;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidget>> MiscTabSlotWidgetArray;

	UPROPERTY()
	TArray<TObjectPtr<class UTextBlock>> MiscTabItemCountTextArray;*/

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> EmptySlotTexture;

	/*UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InventoryWidgetTabSwitcher;*/

	EInventory InventoryType;
};
