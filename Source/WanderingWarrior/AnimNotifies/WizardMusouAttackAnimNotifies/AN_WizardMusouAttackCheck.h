// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify.h"

#include "AN_WizardMusouAttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UAN_WizardMusouAttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
