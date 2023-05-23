// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationAnswerButton.h"

UConversationAnswerButton::UConversationAnswerButton()
{
	OnClicked.AddDynamic(this, &UConversationAnswerButton::OnAnswerButtonClicked);
}

void UConversationAnswerButton::SetButtonIndex(int32 InIndex)
{
	ButtonIndex = InIndex;
}

void UConversationAnswerButton::OnAnswerButtonClicked()
{
	if(OnButtonClickedSignature.IsBound())
	{
		OnButtonClickedSignature.Execute(ButtonIndex);
	}
}

