// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ConversationWidget.generated.h"

//int32 : ButtonIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnswerButtonClickedSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnNPCConversationPageClickedSignature);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UConversationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetNPCNameText(const FText& InText);

	// return : Currnet Conversation Index
	int32 SetNPCConversation(const TArray<FString>& InTextArray, int32 StartIndex);

	void SetAnswers(const TArray<FString>& InTextArray);

	void OpenNPCConversationPage();
	void OpenAnswerPage();

public:

	FOnAnswerButtonClickedSignature OnAnswerButtonClickedSignature;
	FOnNPCConversationPageClickedSignature OnNPCConversationPageClickedSignature;

private:

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void OnAnswerButtonClicked(int32 ButtonIndex);


private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NPCNameTextBlock;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UTextBlock>> NPCConversationTextBlockArray;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UTextBlock>> AnswerTextBlockArray;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UConversationAnswerButton>> AnswerButtonArray;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> ConversationWidgetSwitcher;

	int8 bIsAnswerWidgetOpened : 1;
};
