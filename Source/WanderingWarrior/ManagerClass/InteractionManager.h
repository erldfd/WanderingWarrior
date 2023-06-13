// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InteractionManager.generated.h"

// AActor : InteractionNPC
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartConversationSignature, class ANPCCharacter*);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInteractionManager : public UObject
{
	GENERATED_BODY()
	
public:

	UInteractionManager();

	UFUNCTION()
	void AnalyzeInteraction(const TArray<FOverlapResult>& OverlapResults);

public:

	FOnStartConversationSignature OnStartConversationSignature;
};
