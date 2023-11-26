// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"


#include "GameFramework/GameModeBase.h"

#include "WWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AWWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AWWGameMode();

	virtual void BeginPlay() override;

	void AddAndShowComboCount();

	void AddKillCount();

	void SetHideComboCountWidget(bool bShouldHide);
	
private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UComboCountWidget> ComboCountWidgetClass;

	UPROPERTY()
	TObjectPtr<class UComboCountWidget> ComboCountWidget;
	
};
