// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void OnPossess(APawn* InPawn) override;

public:

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

protected:

	virtual void RunBT();

protected:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTree> BTEnemyAI;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBlackboardData> BBEnemyAI;
};
