// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_ComboAttack.h"

#include "Character/WWCharacter.h"
#include "Controller/BossAIController.h"
#include "WWAnimInstance.h"

UBTTask_ComboAttack::UBTTask_ComboAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character);

	UWWAnimInstance* AnimInstance = Character->GetAnimInstance();

	if (AnimInstance->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	Character->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character);

	if (Character->GetComboCount() > 0)
	{
		Character->Attack();
		return;
	}

	if (Character->GetIsAttacking())
	{
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
