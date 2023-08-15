// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_IsInAir.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UBTDecorator_IsInAir : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTDecorator_IsInAir();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
