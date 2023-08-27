// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "ANS_AttackMovementWindow.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UANS_AttackMovementWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UANS_AttackMovementWindow();

	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

private:

	UPROPERTY(EditAnywhere, Category = SkillMoveSpeed)
	float MoveSpeed;

	float CurrentMoveSpeed;
};
