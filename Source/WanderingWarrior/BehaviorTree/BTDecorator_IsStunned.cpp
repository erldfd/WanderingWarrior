// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsStunned.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsStunned::UBTDecorator_IsStunned()
{

}

bool UBTDecorator_IsStunned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter& ThisCharacter = *Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (&ThisCharacter && ThisCharacter.GetAnimInstance().GetBeingStunned())
	{
		return true;
	}

	return false;
}