// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsHit.h"

#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "AIController.h"

bool UBTDecorator_IsHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ThisCharacter == nullptr)
	{
		return false;
	}

	UWWAnimInstance* AnimInstance = ThisCharacter->GetAnimInstance();

	return AnimInstance->GetIsPlayingCharacterHitMontage();
}
