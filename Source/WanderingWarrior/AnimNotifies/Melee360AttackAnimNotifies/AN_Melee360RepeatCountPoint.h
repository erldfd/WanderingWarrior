// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify.h"

#include "AN_Melee360RepeatCountPoint.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UAN_Melee360RepeatCountPoint : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UAN_Melee360RepeatCountPoint();

	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	UPROPERTY()
	int32 CurrentRepeatCount;
};
