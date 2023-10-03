// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Controller/EnemyAIControllerBase.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AEnemyAIController : public AEnemyAIControllerBase
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTree> BTEnemyRushAI;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBlackboardData> BBEnemyRushAI;

	UPROPERTY(BlueprintReadWrite)
	uint8 bWillUseRushAI : 1;

private:

	uint8 bPreviousWillUseRushAI : 1;
};
