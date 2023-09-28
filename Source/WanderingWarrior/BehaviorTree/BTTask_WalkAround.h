// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_WalkAround.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UBTTask_WalkAround : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_WalkAround();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:

	float WalkTime;
};
