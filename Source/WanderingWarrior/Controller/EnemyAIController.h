// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

public:

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:

	UPROPERTY()
	class UBehaviorTree* BTEnemyAI;

	UPROPERTY()
	class UBlackboardData* BBEnemyAI;
};
