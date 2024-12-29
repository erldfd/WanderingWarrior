// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT (TEXT("/Game/AI/BT_EnemyAI.BT_EnemyAI"));
	if (BT.Succeeded())
	{
		Super::BTEnemyAI = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBData(TEXT("/Game/AI/BB_EnemyAI.BB_EnemyAI"));
	if (BBData.Succeeded())
	{
		Super::BBEnemyAI = BBData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTRush(TEXT("/Game/AI/BT_EnemyRushAI.BT_EnemyRushAI"));
	if (BTRush.Succeeded())
	{
		BTEnemyRushAI = BTRush.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBRushData(TEXT("/Game/AI/BB_EnemyRushAI.BB_EnemyRushAI"));
	if (BBRushData.Succeeded())
	{
		BBEnemyRushAI = BBRushData.Object;
	}*/
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bPreviousWillUseRushAI = bWillUseRushAI;

	if (bWillUseRushAI)
	{
		if (UseBlackboard(BBEnemyRushAI, (UBlackboardComponent*&)Blackboard))
		{
			Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
			if (RunBehaviorTree(BTEnemyRushAI) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
			}
		}

		return;
	}

	if (UseBlackboard(BBEnemyAI, (UBlackboardComponent*&)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (RunBehaviorTree(BTEnemyAI) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWillUseRushAI == bPreviousWillUseRushAI)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController::Tick, bWillUseRushAI == bPreviousWillUseRushAI"));
		return;
	}

	APawn* InPawn = GetPawn();
	if (InPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController::Tick,  == false"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController::Tick,  == true"));

	bPreviousWillUseRushAI = bWillUseRushAI;

	if (bWillUseRushAI)
	{
		if (UseBlackboard(BBEnemyRushAI, (UBlackboardComponent*&)Blackboard))
		{
			Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
			if (RunBehaviorTree(BTEnemyRushAI) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
			}
		}

		return;
	}

	if (UseBlackboard(BBEnemyAI, (UBlackboardComponent*&)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (RunBehaviorTree(BTEnemyAI) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
