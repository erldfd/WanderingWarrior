// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsDead.h"

#include "EnemyAIController.h"
#include "WWCharacter.h"
#include "WWAnimInstance.h"

UBTDecorator_IsDead::UBTDecorator_IsDead()
{
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ThisCharacter == nullptr)
	{
		return true;
	}

	UWWAnimInstance* AnimInstance = ThisCharacter->GetAnimInstance();
	if (AnimInstance == nullptr || AnimInstance->GetIsDead())
	{
		return true;
	}

	return false;
}
