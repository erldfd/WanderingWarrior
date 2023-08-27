// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/MyBTTask_MyMoveTo.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"

//UMyBTTask_MyMoveTo::UMyBTTask_MyMoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//	bNotifyTick = true;
//
//	//INIT_TASK_NODE_NOTIFY_FLAGS(true);
//}

UMyBTTask_MyMoveTo::UMyBTTask_MyMoveTo()
{
	bNotifyTick = true;
	AttackRange = 200;
}

EBTNodeResult::Type UMyBTTask_MyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	return NodeResult;
}

void UMyBTTask_MyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter& ThisCharacter = *Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (&ThisCharacter == nullptr)
	{
		return;
	}

	UWWAnimInstance& AnimInstance = ThisCharacter.GetAnimInstance();
	if (AnimInstance.GetIsDead())
	{
		return;
	}

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return;
	}

	//float AttackRange = 100;
	float DistanceToTarget = Target->GetDistanceTo(&ThisCharacter);
	bool bResult = (DistanceToTarget <= AttackRange);
	
	if (bResult || AnimInstance.GetBeingStunned() || AnimInstance.GetIsDead() || AnimInstance.GetIsIdleOrRun() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
