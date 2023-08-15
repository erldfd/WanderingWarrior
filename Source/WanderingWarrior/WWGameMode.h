// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "WWAnimInstance.h"

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

	UWWAnimInstance* GetPlayerAnimInstance();
	void SetPlayerAnimInstance(class UWWAnimInstance* NewAnimInstance);

private:

	UPROPERTY()
	TObjectPtr<class UWWAnimInstance> PlayerAnimInstance;
};
