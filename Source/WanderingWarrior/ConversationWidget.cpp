// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationWidget.h"

#include "ConversationAnswerButton.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UConversationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NPCConversationTextBlockArray.Reserve(4);

	for (int32 i = 0; i < 4; ++i)
	{
		FString WidgetString(TEXT("NPCConversationTextBlock_"));
		WidgetString.Append(FString::FromInt(i));
		FName WidgetName(WidgetString);

		NPCConversationTextBlockArray.Emplace(Cast<UTextBlock>(GetWidgetFromName(WidgetName)));
		UE_LOG(LogTemp, Warning, TEXT("UConversationWidget, NativeOnInitialized, NPCConversationTextBlockArray %d is Valid index ? : %d"),
			i, NPCConversationTextBlockArray.IsValidIndex(i));
	}

	AnswerTextBlockArray.Reserve(4);

	for (int32 i = 0; i < 4; ++i)
	{
		FString WidgetString(TEXT("AnswerTextBlock_"));
		WidgetString.Append(FString::FromInt(i));
		FName WidgetName(WidgetString);

		AnswerTextBlockArray.Emplace(Cast<UTextBlock>(GetWidgetFromName(WidgetName)));
		UE_LOG(LogTemp, Warning, TEXT("UConversationWidget, NativeOnInitialized, AnswerTextBlockArray %d is Valid index ? : %d"),
			i, AnswerTextBlockArray.IsValidIndex(i));
	}

	AnswerButtonArray.Reserve(4);

	for (int32 i = 0; i < 4; ++i)
	{
		FString WidgetString(TEXT("AnswerButton_"));
		WidgetString.Append(FString::FromInt(i));
		FName WidgetName(WidgetString);

		AnswerButtonArray.Emplace(Cast<UConversationAnswerButton>(GetWidgetFromName(WidgetName)));
		UE_LOG(LogTemp, Warning, TEXT("UConversationWidget, NativeOnInitialized, AnswerButtonArray %d is Valid index ? : %d"),
			i, AnswerButtonArray.IsValidIndex(i));

		AnswerButtonArray[i]->SetButtonIndex(i);
		AnswerButtonArray[i]->OnButtonClickedSignature.BindUObject(this, &UConversationWidget::OnAnswerButtonClicked);
	}
}

FReply UConversationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply rep = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (bIsAnswerWidgetOpened)
	{
		return rep;
	}

	OnNPCConversationPageClickedSignature.Broadcast();

	return rep;
}

void UConversationWidget::OnAnswerButtonClicked(int32 ButtonIndex)
{
	if (bIsAnswerWidgetOpened == false)
	{
		return;
	}

	OpenNPCConversationPage();

	if (OnAnswerButtonClickedSignature.IsBound())
	{
		OnAnswerButtonClickedSignature.Broadcast(ButtonIndex);
	}
}

void UConversationWidget::SetNPCNameText(const FText& InText)
{
	NPCNameTextBlock->SetText(InText);
}

int32 UConversationWidget::SetNPCConversation(const TArray<FString>& InTextArray, int32 StartIndex)
{
	if (StartIndex == -1)
	{
		return -1;
		// NPC Conversation Completed
	}

	int32 InTextArrayNum = InTextArray.Num();
	int32 NPCConversationTextBlockArrayNum = NPCConversationTextBlockArray.Num();

	int32 CurrentIndex = StartIndex;
	int32 i = 0;

	for (i = 0; CurrentIndex < InTextArrayNum && i < NPCConversationTextBlockArrayNum; ++CurrentIndex, ++i)
	{
		NPCConversationTextBlockArray[i]->SetText(FText::FromString(InTextArray[CurrentIndex]));
	}

	for (; i < NPCConversationTextBlockArrayNum; ++i)
	{
		NPCConversationTextBlockArray[i]->SetText(FText());
	}

	if (CurrentIndex >= InTextArrayNum)
	{
		CurrentIndex = -1;
	}

	return CurrentIndex;
}

void UConversationWidget::SetAnswers(const TArray<FString>& InTextArray)
{
	int i = 0;

	for (auto& Text : InTextArray)
	{
		AnswerButtonArray[i]->SetVisibility(ESlateVisibility::Visible);
		AnswerTextBlockArray[i]->SetText(FText::FromString(Text));
		i++;
	}

	for (; i < AnswerTextBlockArray.Num(); ++i)
	{
		AnswerTextBlockArray[i]->SetText(FText());
		AnswerButtonArray[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UConversationWidget::OpenNPCConversationPage()
{
	ConversationWidgetSwitcher->SetActiveWidgetIndex(0);
	bIsAnswerWidgetOpened = false;
}

void UConversationWidget::OpenAnswerPage()
{
	ConversationWidgetSwitcher->SetActiveWidgetIndex(1);
	bIsAnswerWidgetOpened = true;
}
