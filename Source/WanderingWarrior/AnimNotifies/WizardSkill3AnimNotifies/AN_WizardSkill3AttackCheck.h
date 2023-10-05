// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify.h"

#include "AN_WizardSkill3AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UAN_WizardSkill3AttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
