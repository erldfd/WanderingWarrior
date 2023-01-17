// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//if SlotTexture is null, SlotTexture set to EmptySlotTexture
	void SetSlotImageFromTexture(int SlotIndex, class UTexture2D* SlotTexture = nullptr);

	void SetSlotItemCountText(int SlotItemCount, int SlotIndex);

protected:

	virtual void NativeOnInitialized() override;

private:

	void SetSlotImageFromTextureInternal(TArray<class UInventorySlotWidgetImage*> SlotImageArray, int SlotIndex, class UTexture2D* SlotTexture = nullptr);

private:

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidgetImage>> QuickSlotImageArray;

	UPROPERTY()
	TArray<TObjectPtr<class UTextBlock>> QuickSlotItemCountTextArray;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> EmptySlotTexture;
};
