// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTComposite_RandomSelector.h"

UBTComposite_RandomSelector::UBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "RandomSelector";
}

int32 UBTComposite_RandomSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	// success = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized || LastResult == EBTNodeResult::Failed)
	{
		int32 ChildrenNum = GetChildrenNum();
		int32 RandomIndex = FMath::RandRange(0, ChildrenNum - 1);
		UE_LOG(LogTemp, Warning, TEXT("UBTComposite_RandomSelector::GetNextChildHandler, Child : %d, RandomIndex : %d"), ChildrenNum, RandomIndex);
		NextChildIdx = RandomIndex;
	}

	return NextChildIdx;
}