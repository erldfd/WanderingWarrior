// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerBox.h"
#include "LevelSequence/Public/LevelSequence.h"

#include "SequencePlayerTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API ASequencePlayerTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	ASequencePlayerTriggerBox();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), BlueprintReadWrite)
	TObjectPtr<class ULevelSequence> LevelSequence;
};
