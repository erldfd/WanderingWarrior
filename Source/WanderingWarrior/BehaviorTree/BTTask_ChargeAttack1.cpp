// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_ChargeAttack1.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "Components/WarriorSkillComponent.h"
#include "WWAnimInstance.h"

UBTTask_ChargeAttack1::UBTTask_ChargeAttack1()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ChargeAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character);

	UWWAnimInstance* AnimInstance = Character->GetAnimInstance();

	if (AnimInstance->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	Character->DoChargeAttack();

	return EBTNodeResult::InProgress;
}

void UBTTask_ChargeAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChargeAttack1::TickTask, Character == nullptr"));
		return;
	}

	if (Character->GetSkillComponent()->GetIsChargeAttack1Started() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}