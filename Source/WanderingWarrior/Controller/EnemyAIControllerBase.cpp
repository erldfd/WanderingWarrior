// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIControllerBase::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIControllerBase::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIControllerBase::TargetKey(TEXT("Target"));

void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBEnemyAI, (UBlackboardComponent*&)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (RunBehaviorTree(BTEnemyAI) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
