// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"

#include "InventorySlotWidgetImage.generated.h"

DECLARE_DELEGATE(FOnClickedDelelgate);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotWidgetImage : public UImage
{
	GENERATED_BODY()

public:

	UInventorySlotWidgetImage(const FObjectInitializer& ObjectInitializer);

	int GetSlotIndex();
	void SetSlotIndex(int NewIndex);

	bool GetIsClicked();
	void SetIsClicked(bool bNewIsClicked);

public:

	FOnClickedDelelgate OnClickedDelegate;

private:

	UFUNCTION()
	void OnClicked();

private:

	int SlotIndex;
	bool bIsClicked = false;
};
