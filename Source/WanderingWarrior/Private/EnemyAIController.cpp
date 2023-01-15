// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::TargetKey(TEXT("Target"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT (TEXT("/Game/AI/BT_EnemyAI.BT_EnemyAI"));
	if (BT.Succeeded())
	{
		BTEnemyAI = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBData(TEXT("/Game/AI/BB_EnemyAI.BB_EnemyAI"));
	if (BBData.Succeeded())
	{
		BBEnemyAI = BBData.Object;
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBEnemyAI, (UBlackboardComponent*&)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTEnemyAI))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}