// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetEnemyNameTextBlock(FText NewText);
	void SetEnemyHPBarPercent(float Percent);
	void SetMyHPBarPercent(float Percent);
	void SetMyMPBarPercent(float Percent);

	class UInventoryWidget* GetQuickSlotWidget();
	class UInventoryWidget* GetInventoryWidget();
	class UInventoryWidget* GetMarchantInventoryWidget();

	class UConversationWidget* GetConversationWidget();

	class UInventoryItemInfoWidget* GetInventoryItemInfoWidget();

protected:

	virtual void NativeOnInitialized() override;

private:

	void HideEnemyHPAndNameWidget();

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> EnemyNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> EnemyHPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> MyHPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> MyMPBar;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UInventoryWidget> QuickSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> MarchantInventoryWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UConversationWidget> ConversationWidget;*/

	FTimerHandle HideEnemyHPAndNameWidgetTimerHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryItemInfoWidget> InventoryItemInfoWidget;
};
