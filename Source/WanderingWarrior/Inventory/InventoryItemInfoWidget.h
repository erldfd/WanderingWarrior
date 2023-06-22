// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetItemNameText(const FText& InText);
	void SetBuyPriceText(const FText& InText);
	void SetSellPriceText(const FText& InText);
	void SetAttackDamageText(const FText& InText);

private:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BuyPriceTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SellPriceTextBlock;
};
