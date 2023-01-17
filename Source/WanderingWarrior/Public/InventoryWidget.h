// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnSlotImageWigetClickedDelegate, int);
//param : TabIndex, 0= weapontab, 1 = misctab
DECLARE_DELEGATE_OneParam(FOnConvertTabDelegate, int);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	TArray<class UInventorySlotWidgetImage*> GetSlotImageArray(enum class ETabType TabType);

	//if SlotTexture is null, SlotTexture set to EmptySlotTexture
	void SetSlotImageFromTexture(enum class ETabType TabType, int SlotIndex, class UTexture2D* SlotTexture = nullptr);

	void SetSlotItemCountText(int SlotItemCount, int SlotIndex, ETabType TabType);

public:

	//param : SlotIndex
	FOnSlotImageWigetClickedDelegate OnSlotImageWidgetClickedDelegate;
	FOnConvertTabDelegate OnConvertTabDelegate;

protected:

	virtual void NativeOnInitialized() override;

private:

	void SetSlotImageFromTextureInternal(TArray<UInventorySlotWidgetImage*> SlotImageArray, int SlotIndex, class UTexture2D* SlotTexture = nullptr);

	void OnWeaponTabSlotClicked();
	void OnMiscTabSlotClicked();

	UFUNCTION()
	void ConvertToWeaponTab();

	UFUNCTION()
	void ConvertToMiscTab();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponTabButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MiscTabButton;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidgetImage>> WeaponTabSlotImageArray;

	UPROPERTY();
	TArray<TObjectPtr<class UTextBlock>> WeaponTabItemCountTextArray;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidgetImage>> MiscTabSlotImageArray;

	TArray<TObjectPtr<class UTextBlock>> MiscTabItemCountTextArray;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> EmptySlotTexture;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InventoryWidgetTabSwitcher;
};
