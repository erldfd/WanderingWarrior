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

protected:

	virtual void NativeOnInitialized() override;

private:

	void HideEnemyHPAndNameWidget();

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnemyNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EnemyHPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MyHPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MyMPBar;

	FTimerHandle HideEnemyHPAndNameWidgetTimerHandle = { };
};
