// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_ChargeAttack3.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "Components/WarriorSkillComponent.h"
#include "WWAnimInstance.h"

UBTTask_ChargeAttack3::UBTTask_ChargeAttack3()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ChargeAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

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

void UBTTask_ChargeAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast <AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChargeAttack3::TickTask, Character == nullptr"));
		return;
	}

	if (Character->GetComboCount() < 2)
	{
		Character->Attack();
		//FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
	else if (Character->GetWillPlayChargeAttack3() == false)
	{
		Character->DoChargeAttack();
	//	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
	else if (Character->GetSkillComponent()->GetIsChargeAttack3Started() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
