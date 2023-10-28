// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/MyBTTask_MyMoveTo.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"
#include "Components/SkillComponentBase.h"

#include "AISystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"
#include "Tasks/AITask_MoveTo.h"

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
	//SearchAgainInterval = 1;
}

EBTNodeResult::Type UMyBTTask_MyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	return NodeResult;
}

void UMyBTTask_MyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;
	
	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ThisCharacter == nullptr)
	{
		return;
	}

	UWWAnimInstance* AnimInstance = ThisCharacter->GetAnimInstance();
	if (AnimInstance->GetIsDead())
	{
		return;
	}

	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return;
	}

	USkillComponentBase* SkillComp = ThisCharacter->GetSkillComponent();
	if (SkillComp && SkillComp->IsSkillStarted())
	{
		//ElapsedTime = 0;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//if (ElapsedTime >= SearchAgainInterval /*&& Speed < 20.0f*/)
	//{
	//	ElapsedTime = 0;
	//	//bShouldChaseTargetPos = true;
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	//bool bShouldSurroundStart = ShouldSurroundTarget(OwnerComp);
	
	//if (bShouldSurroundStart && bShouldChaseTargetPos == false)
	//{
	//	//ElapsedTime = 0;
	//	//UE_LOG(LogTemp, Warning, TEXT("bShouldChaseTargetPos : %f"), bShouldChaseTargetPos);
	//	bShouldChaseTargetPos = true;
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}

	float DistanceToTarget = Target->GetDistanceTo(ThisCharacter);
	bool bResult = (DistanceToTarget <= AttackRange);

	if (bResult || AnimInstance->GetBeingStunned() || AnimInstance->GetIsDead() || AnimInstance->GetIsIdleOrRun() == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hoow did you.......bShouldChaseTargetPos : %d"), bResult && bShouldChaseTargetPos);
		//ElapsedTime = 0;
		//bShouldChaseTargetPos = false;
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

//EBTNodeResult::Type UMyBTTask_MyMoveTo::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() || TargetPos.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() || bShouldChaseTargetPos == false)
//	{
//		return Super::PerformMoveTask(OwnerComp, NodeMemory);
//	}
//	
//	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
//	FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
//	AAIController* MyController = OwnerComp.GetAIOwner();
//
//	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
//	if (MyController && MyBlackboard)
//	{
//		FAIMoveRequest MoveReq;
//		MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : MyController->GetDefaultNavigationFilterClass());
//		MoveReq.SetAllowPartialPath(bAllowPartialPath);
//		MoveReq.SetAcceptanceRadius(AcceptableRadius);
//		MoveReq.SetCanStrafe(bAllowStrafe);
//		MoveReq.SetReachTestIncludesAgentRadius(bReachTestIncludesAgentRadius);
//		MoveReq.SetReachTestIncludesGoalRadius(bReachTestIncludesGoalRadius);
//		MoveReq.SetProjectGoalLocation(bProjectGoalLocation);
//		MoveReq.SetUsePathfinding(bUsePathfinding);
//
//		/*if (bShouldSurroundStart == false)
//		{
//			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
//			AActor* TargetActor = Cast<AActor>(KeyValue);
//			if (TargetActor)
//			{
//				if (bTrackMovingGoal)
//				{
//					MoveReq.SetGoalActor(TargetActor);
//				}
//				else
//				{
//					MoveReq.SetGoalLocation(TargetActor->GetActorLocation());
//				}
//			}
//			else
//			{
//				UE_VLOG(MyController, LogBehaviorTree, Warning, TEXT("UMyBTTask_MyMoveTo::ExecuteTask tried to go to actor while BB %s entry was empty"), *BlackboardKey.SelectedKeyName.ToString());
//			}
//		}
//		else *//*if (bShouldSurroundStart)
//		{
//			const FVector TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(TargetPos.GetSelectedKeyID());
//			MoveReq.SetGoalLocation(TargetLocation);
//
//			MyMemory->PreviousGoalLocation = TargetLocation;
//		}*/
//		
//		const FVector TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(TargetPos.GetSelectedKeyID());
//		MoveReq.SetGoalLocation(TargetLocation);
//
//		MyMemory->PreviousGoalLocation = TargetLocation;
//		
//		if (MoveReq.IsValid())
//		{
//			if (GET_AI_CONFIG_VAR(bEnableBTAITasks))
//			{
//				UAITask_MoveTo* MoveTask = MyMemory->Task.Get();
//				const bool bReuseExistingTask = (MoveTask != nullptr);
//
//				MoveTask = PrepareMoveTask(OwnerComp, MoveTask, MoveReq);
//				if (MoveTask)
//				{
//					MyMemory->bObserverCanFinishTask = false;
//
//					if (bReuseExistingTask)
//					{
//						if (MoveTask->IsActive())
//						{
//							UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' reusing AITask %s"), *GetNodeName(), *MoveTask->GetName());
//							MoveTask->ConditionalPerformMove();
//						}
//						else
//						{
//							UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' reusing AITask %s, but task is not active - handing over move performing to task mechanics"), *GetNodeName(), *MoveTask->GetName());
//						}
//					}
//					else
//					{
//						MyMemory->Task = MoveTask;
//						UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' task implementing move with task %s"), *GetNodeName(), *MoveTask->GetName());
//						MoveTask->ReadyForActivation();
//					}
//
//					MyMemory->bObserverCanFinishTask = true;
//					NodeResult = (MoveTask->GetState() != EGameplayTaskState::Finished) ? EBTNodeResult::InProgress :
//						MoveTask->WasMoveSuccessful() ? EBTNodeResult::Succeeded :
//						EBTNodeResult::Failed;
//				}
//			}
//			else
//			{
//				const FPathFollowingRequestResult RequestResult = MyController->MoveTo(MoveReq);
//				if (RequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
//				{
//					MyMemory->MoveRequestID = RequestResult.MoveId;
//					WaitForMessage(OwnerComp, UBrainComponent::AIMessage_MoveFinished, RequestResult.MoveId);
//					WaitForMessage(OwnerComp, UBrainComponent::AIMessage_RepathFailed);
//
//					NodeResult = EBTNodeResult::InProgress;
//				}
//				else if (RequestResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
//				{
//					NodeResult = EBTNodeResult::Succeeded;
//				}
//			}
//		}
//	}
//
//	return NodeResult;
//}
//
//bool UMyBTTask_MyMoveTo::ShouldSurroundTarget(const UBehaviorTreeComponent& OwnerComp)
//{
//	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
//	if (ThisCharacter == nullptr)
//	{
//		return false;
//	}
//
//	AWWCharacter* Target = Cast<AWWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
//	if (Target == nullptr)
//	{
//		return false;
//	}
//
//	float DistanceToTarget = Target->GetDistanceTo(ThisCharacter);
//	bool bShouldSurroundStart = (DistanceToTarget <= SurroundingStartRange);
//
//	return bShouldSurroundStart;
//}
