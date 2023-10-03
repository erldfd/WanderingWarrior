// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_TurnToTargetDirectly.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_TurnToTargetDirectly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWWAnimInstance* AnimInstance = Character->GetAnimInstance();
	if (AnimInstance->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Character->GetActorLocation();
	LookVector.Z = 0.0f;
	const FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Character->SetActorRotation(TargetRot);// Look Directly

	return EBTNodeResult::Succeeded;
}
