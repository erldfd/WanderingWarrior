// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPosition.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

//#include "BehaviorTree/BehaviorTreeTypes.h"

UBTTask_FindPatrolPosition::UBTTask_FindPatrolPosition()
{
	NodeName = TEXT("FindPatrolPosition");
}

EBTNodeResult::Type UBTTask_FindPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AWWCharacter& ThisCharacter = *Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (&ThisCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWWAnimInstance& AnimInstance = ThisCharacter.GetAnimInstance();
	if (AnimInstance.GetIsDead())
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ThisCharacter.GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::HomePosKey);
	FNavLocation NextPetrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500, NextPetrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::PatrolPosKey, NextPetrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}