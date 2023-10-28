// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ComboCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UComboCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UComboCountWidget(const FObjectInitializer& ObjectInitializer);

public:

	void SetComboCount(int32 NewComboCount);
	void ChangeAndShowComboCountText(int32 NewComboCount);

	// ComboCount + 1
	void AddAndShowComboCount();

	// Combo Count Section
private:

	UFUNCTION()
	void InitComboCount();

	void ExtendComboCountTextShowTime(float ExtendingTime);
	
private:

	int32 ComboCount;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ComboCountTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ComboTextBlock;

	FTimerHandle ShowComboTimerHandle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float ComboCountShowTime;

	//kill count section
public:

	// KillCount + 1
	void AddKillCount();

	void ChangeKillCountText(int32 NewKillCount);

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> KillCountTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> KillTextBlock;

	int32 KillCount;

};
