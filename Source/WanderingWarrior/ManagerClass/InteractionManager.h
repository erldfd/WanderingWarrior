// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "InteractionManager.generated.h"

// int32 : Conversation Index
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartConversationSignature, int32);
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
