// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_IsStunned.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UBTDecorator_IsStunned : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTDecorator_IsStunned();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
