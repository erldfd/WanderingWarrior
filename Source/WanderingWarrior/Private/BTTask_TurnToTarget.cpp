// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"

#include "WWCharacter.h"
#include "EnemyAIController.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{

}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

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
	if (AnimInstance == nullptr || AnimInstance->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Character->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//Character->SetActorRotation(TargetRot);//¹Ù·ÎÃÄ´Ùº½
	Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;

}
