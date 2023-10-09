// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_MusouAttack.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "Components/WarriorSkillComponent.h"
#include "WWAnimInstance.h"

UBTTask_MusouAttack::UBTTask_MusouAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MusouAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Character);

	UWWAnimInstance* AnimInstance = Character->GetAnimInstance();

	if (AnimInstance->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	Character->DoMusouAttack();

	return EBTNodeResult::InProgress;
}

void UBTTask_MusouAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_MusouAttack::TickTask, Character == nullptr"));
		return;
	}

	if (Character->GetSkillComponent()->GetIsChargeAttack1Started() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}