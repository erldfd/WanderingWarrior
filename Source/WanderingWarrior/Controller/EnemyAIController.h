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

	virtual void Tick(float DeltaTime) override;

public:

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

protected:

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTEnemyAI;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBEnemyAI;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTEnemyRushAI;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBEnemyRushAI;

	UPROPERTY(BlueprintReadWrite)
	uint8 bWillUseRushAI : 1;

private:

	uint8 bPreviousWillUseRushAI : 1;
};
