// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BossAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

//const FName AEnemyAIController::HomePosKey(TEXT("HomePos"));
//const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
//const FName AEnemyAIController::TargetKey(TEXT("Target"));

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_BossAI.BT_BossAI"));
	if (BT.Succeeded())
	{
		BTEnemyAI = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBData(TEXT("/Game/AI/BB_BossAI.BB_BossAI"));
	if (BBData.Succeeded())
	{
		BBEnemyAI = BBData.Object;
	}
}
