// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_WalkAround.h"

#include "Character/WWCharacter.h"
#include "Controller/EnemyAIController.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnhancedInputComponent.h"

UBTTask_WalkAround::UBTTask_WalkAround()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_WalkAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	WalkTime = 0;
	return EBTNodeResult::InProgress;
}

void UBTTask_WalkAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* Character = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return;
	}

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return;
	}

	UWWAnimInstance& AnimInstance = Character->GetAnimInstance();
	if (AnimInstance.GetIsDead())
	{
		return;
	}

	/*WalkTime += DeltaSeconds;
	UE_LOG(LogTemp, Warning, TEXT("UBTTask_WalkAround::TickTask, WalkTime : %f"), WalkTime);
	if (WalkTime >= 5.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}*/

	FVector LookVector = Target->GetActorLocation() - Character->GetActorLocation();
	LookVector.Z = 0.0f;
	const FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Character->SetActorRotation(TargetRot);// Look Directly
	Character->AddActorWorldOffset(Character->GetActorRightVector() * 100 * DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
