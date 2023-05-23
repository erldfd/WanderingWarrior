// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_FindPatrolPosition.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UBTTask_FindPatrolPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_FindPatrolPosition();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
