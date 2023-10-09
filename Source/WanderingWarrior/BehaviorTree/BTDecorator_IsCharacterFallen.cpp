// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsCharacterFallen.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsCharacterFallen::UBTDecorator_IsCharacterFallen()
{
}

bool UBTDecorator_IsCharacterFallen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("ThisCharacter : %d"), ThisCharacter == nullptr);
	if (ThisCharacter == nullptr)
	{
		return false;
	}

	UWWAnimInstance* AnimInstance = ThisCharacter->GetAnimInstance();
	UE_LOG(LogTemp, Warning, TEXT("Dead: %d, IdleOrRun : %d"), AnimInstance->GetIsDead(), AnimInstance->GetIsIdleOrRun());
	if (AnimInstance->GetIsDead() || AnimInstance->GetIsIdleOrRun() == false)
	{
		return true;
	}
	
	return false;
}
