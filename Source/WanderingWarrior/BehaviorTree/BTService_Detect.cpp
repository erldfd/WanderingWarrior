// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "Controller/EnemyAIController.h"
#include "Character/WWCharacter.h"
#include "WWAnimInstance.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AWWCharacter* ThisCharacter = Cast<AWWCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ThisCharacter == nullptr)
	{
		return;
	}

	UWWAnimInstance& AnimInstance = ThisCharacter->GetAnimInstance();
	if (AnimInstance.GetIsDead())
	{
		return;
	}

	UWorld* World = ThisCharacter->GetWorld();
	FVector Center = ThisCharacter->GetActorLocation();
	float DetectRadius = 1000;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ThisCharacter);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* Character = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.5);
				DrawDebugPoint(World, Character->GetActorLocation(), 10, FColor::Blue, false, 0.5);
				DrawDebugLine(World, ThisCharacter->GetActorLocation(), Character->GetActorLocation(), FColor::Black, false, 0.5);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.5);
}