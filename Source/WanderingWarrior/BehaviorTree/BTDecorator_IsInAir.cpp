// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsInAir.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_IsInAir::UBTDecorator_IsInAir()
{
}

bool UBTDecorator_IsInAir::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter& ThisCharacter = *Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (&ThisCharacter == nullptr)
	{
		return false;
	}

	if (ThisCharacter.GetMovementComponent()->IsFalling())
	{
		return true;
	}
	
	return false;
}
