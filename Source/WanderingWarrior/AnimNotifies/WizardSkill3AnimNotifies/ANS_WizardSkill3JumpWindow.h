// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "ANS_WizardSkill3JumpWindow.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UANS_WizardSkill3JumpWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float JumpDuration;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float JumpHeight;

	float JumpElapsedTime;
	float JumpGoalHeight;
	float StartHeight;
};
