// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"

#include "AN_PlayRandomSound.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UAN_PlayRandomSound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true), meta = (AllowPrivateAccess))
	TArray<TObjectPtr<USoundBase>> RandomSoundArray;
};
