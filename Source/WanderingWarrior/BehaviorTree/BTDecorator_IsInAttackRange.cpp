// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	AttackRange = 300;
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ThisCharacter == nullptr)
	{
		return false;
	}

	UWWAnimInstance* AnimInstance = ThisCharacter->GetAnimInstance();
	if (AnimInstance->GetIsDead())
	{
		return false;
	}

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return false;
	}

	float DistanceToTarget = Target->GetDistanceTo(ThisCharacter);
	bResult = (DistanceToTarget <= AttackRange);

	return bResult;
}