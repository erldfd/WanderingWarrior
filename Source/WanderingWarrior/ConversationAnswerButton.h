// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"

#include "ConversationAnswerButton.generated.h"

// int32 : ButtonIndex
DECLARE_DELEGATE_OneParam(FOnButtonClickedSignature, int32);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UConversationAnswerButton : public UButton
{
	GENERATED_BODY()
	
public:

	UConversationAnswerButton();

	void SetButtonIndex(int32 InIndex);

public:

	FOnButtonClickedSignature OnButtonClickedSignature;

private:

	UFUNCTION()
	void OnAnswerButtonClicked();

private:

	int32 ButtonIndex;
};
