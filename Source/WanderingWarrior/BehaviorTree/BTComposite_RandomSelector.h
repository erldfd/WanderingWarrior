// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTCompositeNode.h"

#include "BTComposite_RandomSelector.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UBTComposite_RandomSelector : public UBTCompositeNode
{
	GENERATED_BODY()
	
public:

	UBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer);

protected:

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
};
