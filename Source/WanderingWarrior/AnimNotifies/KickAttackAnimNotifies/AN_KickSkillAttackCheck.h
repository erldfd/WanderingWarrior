// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify.h"

#include "AN_KickSkillAttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UAN_KickSkillAttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
