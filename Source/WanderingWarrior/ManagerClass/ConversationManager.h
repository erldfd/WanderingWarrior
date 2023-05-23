// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"

#include "ConversationManager.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UConversationManager : public UObject
{
	GENERATED_BODY()
	
public:

	UConversationManager();

	void SetConversationWidget(class UConversationWidget* InConversationWidget);

	void SetNPCNameText(const FText& InText);

	void SetNPCConversation(int32 ConversationIndex);

	void SetPlayerAnswers(int32 InCurrentConversationIndex);

	void SetConversationScriptDataArray(const TArray<struct FConversationScriptDataRow*>& InConversationScriptDataArray);

	void OpenConversationWidget();

	void CloseConversationWidget();

	void OpenNPCConversationWidget();

	void BindConversationWidgetSignature();	

	void FinishConversation();

private:

	void SetNPCConversationInternal(int32 ConversationIndex);

	UFUNCTION()
	void OnNPCConversationPageClicked();

	UFUNCTION()
	void OnAnswerButtonClickedSignature(int32 ButtonIndex);

private:

	UPROPERTY()
	TObjectPtr<class UConversationWidget> ConversationWidget;

	TArray<struct FConversationScriptDataRow*> ConversationScriptDataArray;


	int8 bIsConversationWidgetOpened : 1;

	int32 NPCScriptIndex;

	int32 CurrentConversationIndex;
};
