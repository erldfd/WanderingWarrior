// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIControllerBase.h"

#include "Character/WWCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIControllerBase::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIControllerBase::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIControllerBase::TargetKey(TEXT("Target"));

void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AWWCharacter* PossessedCharacter = Cast<AWWCharacter>(InPawn);
	if (PossessedCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyAIControllerBase::OnPossess, PossessedCharacter == nullptr"));
		return;
	}

	PossessedCharacter->SetIsPlayer(false);

	RunBT();
}

void AEnemyAIControllerBase::RunBT()
{
	if (UseBlackboard(BBEnemyAI, (UBlackboardComponent*&)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (RunBehaviorTree(BTEnemyAI) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
