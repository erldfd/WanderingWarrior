// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/WWCharacter.h"

#include "WizardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AWizardCharacter : public AWWCharacter
{
	GENERATED_BODY()
	
public: 

	AWizardCharacter();

	virtual void AttackCheck() override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMagicBall> NormalAttackMagicBall;
};
