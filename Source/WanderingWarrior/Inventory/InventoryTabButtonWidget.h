// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"

#include "InventoryTabButtonWidget.generated.h"

enum class ETabType : uint8;

//param : TabType to int
DECLARE_DELEGATE_OneParam(FOnTabButtonClickedDelegate, int);

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryTabButtonWidget : public UButton
{
	GENERATED_BODY()
	
public:

	UInventoryTabButtonWidget();

	//init to nullptr
	void InitSlotWidgetArray(int SlotWidgetCount);

	//init to nullptr
	void InitSlotItemCountTextArray(int SlotItemCountArrayCount);

	TArray<class UInventorySlotWidget*>& GetSlotWidgetArray();
	TArray<class UTextBlock*>& GetSlotItemCountTextArray();

	void SetTabType(ETabType NewTabType);

public:

	//param : TabType to int
	FOnTabButtonClickedDelegate OnTabButtonClickedDelegate;

private:

	void OnTabSlotClicked(int SlotIndex);

	UFUNCTION()
	void OnTabButtonClicked();

private:

	UPROPERTY(EditAnywhere)
	ETabType TabType;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UInventorySlotWidget>> SlotWidgetArray;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UTextBlock>> SlotItemCountTextArray;
};
