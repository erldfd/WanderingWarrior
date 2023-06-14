// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "WWAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character != nullptr);

	UWWAnimInstance& AnimInstance = Character->GetAnimInstance();

	if (AnimInstance.GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	Character->Attack(1);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character != nullptr);

	UWWAnimInstance& AnimInstance = Character->GetAnimInstance();

	if (AnimInstance.GetIsAttacking())
	{
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
