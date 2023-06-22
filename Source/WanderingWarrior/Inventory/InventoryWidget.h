// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

enum class ETabType : uint8;
enum class EInventory : uint8;

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnSlotImageWigetClickedDelegate, int32);
//param : TabIndex, 0= weapontab, 1 = misctab
DECLARE_DELEGATE_OneParam(FOnConvertTabDelegate, int32);
/**
 * 
 */

USTRUCT(BlueprintType)
struct FInventoryWidgetSettings
{
	GENERATED_BODY()

public:

	int32 TabCount;
	int32 SlotCountByTab;

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
	void SetSlotWidgetImageFromTexture(ETabType TabType, int32 SlotIndex, class UTexture2D* SlotTexture = nullptr);
	void SetSlotWidgetImageFromTexture(const TArray<UInventorySlotWidget*>& SlotImageArray, int32 SlotIndex, class UTexture2D* SlotTexture = nullptr);
	void SetSlotWidgetImageFromTexture(UInventorySlotWidget& SlotWidget, class UTexture2D* SlotTexture = nullptr);
	void SetSlotItemCountText(int32 SlotItemCount, int32 SlotIndex, ETabType TabType);

	void InitNullTabButtonArray(int32 ArrayCount);

	TArray<class UInventoryTabButtonWidget*>& GetTabButtonArray();

	void InitInventoryWidget(const FInventoryWidgetSettings& Settings);

	void SetInventoryType(EInventory NewInventoryType);

public:

	////param : SlotIndex
	FOnSlotImageWigetClickedDelegate OnSlotImageWidgetClickedDelegate;
	FOnConvertTabDelegate OnConvertTabDelegate;

protected:

	virtual void NativeOnInitialized() override;

private:

	void OnWeaponTabSlotClicked(int32 SlotIndex);
	void OnMiscTabSlotClicked(int32 SlotIndex);

	UFUNCTION()
	void ConvertTab(int32 TabNum);

	UFUNCTION()
	void ConvertToWeaponTab();

	UFUNCTION()
	void ConvertToMiscTab();

private:

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UInventoryTabButtonWidget>> TabButtonArray;


	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetSwitcher> InventoryWidgetTabSwitcher;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> EmptySlotTexture;

	EInventory InventoryType;
};
