// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Controller/EnemyAIControllerBase.h"

#include "WizardAIController.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AWizardAIController : public AEnemyAIControllerBase
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTree> BTEnemyAIPhase1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTree> BTEnemyAIPhase2;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTree> BTEnemyAIPhase3;
	
};
