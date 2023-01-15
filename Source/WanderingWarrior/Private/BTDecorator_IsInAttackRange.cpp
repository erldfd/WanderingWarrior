// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"

#include "EnemyAIController.h"
#include "WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{

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
	if (AnimInstance == nullptr || AnimInstance->GetIsDead())
	{
		return false;
	}

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ThisCharacter) <= 200);

	return bResult;
}