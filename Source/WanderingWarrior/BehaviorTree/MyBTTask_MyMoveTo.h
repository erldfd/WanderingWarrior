// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "MyBTTask_MyMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UMyBTTask_MyMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:

	UMyBTTask_MyMoveTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	float AttackRange;
};
