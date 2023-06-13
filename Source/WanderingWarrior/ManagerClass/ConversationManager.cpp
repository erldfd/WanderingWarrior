// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationManager.h"

#include "ConversationScriptData.h"
#include "WanderingWarrior/ConversationWidget.h"
#include "WanderingWarrior/Controller/WWPlayerController.h"
#include "WanderingWarrior/Character/NPCCharacter.h"

#include "Kismet/GameplayStatics.h"

UConversationManager::UConversationManager() : bIsConversationWidgetOpened(false), NPCScriptIndex(0)
{
	
}

void UConversationManager::SetConversationWidget(UConversationWidget* InConversationWidget)
{
	ConversationWidget = InConversationWidget;
}

void UConversationManager::SetNPCNameText(const FText& InText)
{
	check(ConversationWidget);
	ConversationWidget->SetNPCNameText(InText);
}

void UConversationManager::SetNPCConversation(int32 ConversationIndex)
{
	NPCScriptIndex = 0;
	CurrentConversationIndex = ConversationIndex;
	SetNPCConversationInternal(CurrentConversationIndex);
}

void UConversationManager::SetBehavior(EBehavior Behavior)
{
	switch (Behavior)
	{
	case EBehavior::OpenStore:

		break;
	case EBehavior::JustClose:
		FinishConversation();
		break;
	case EBehavior::NoBehavior:
		break;
	default:
		break;
	}
}

void UConversationManager::SetNPCConversationInternal(int32 ConversationIndex)
{
	check(ConversationWidget);
	NPCScriptIndex = ConversationWidget->SetNPCConversation(ConversationScriptDataArray[ConversationIndex]->NPCScriptArray, NPCScriptIndex);
	UE_LOG(LogTemp, Warning, TEXT("UConversationManager, SetNPCConversationInternal,  NPCScriptIndex : %d"), NPCScriptIndex);
	CurrentConversationIndex = ConversationIndex;
}

void UConversationManager::SetPlayerAnswers(int32 InCurrentConversationIndex)
{
	ConversationWidget->SetAnswers(ConversationScriptDataArray[InCurrentConversationIndex]->PlayerScriptArray);
}

void UConversationManager::SetConversationScriptDataArray(const TArray<FConversationScriptDataRow*>& InConversationScriptDataArray)
{
	ConversationScriptDataArray = InConversationScriptDataArray;
	if (ensure(ConversationScriptDataArray.Num() > 0) == false) return;

	for (int i = 0; i < ConversationScriptDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("UConversationManager, SetConversationScriptDataArray, %d"), ConversationScriptDataArray[i]->Index);
	}
}

void UConversationManager::SetConversationNPC(ANPCCharacter* NPC)
{
	ConversationNPC = NPC;
}

void UConversationManager::OpenConversationWidget()
{
	check(ConversationWidget);
	ConversationWidget->SetVisibility(ESlateVisibility::Visible);

	bIsConversationWidgetOpened = true;
}

void UConversationManager::CloseConversationWidget()
{
	check(ConversationWidget);
	ConversationWidget->SetVisibility(ESlateVisibility::Hidden);

	bIsConversationWidgetOpened = false;
}

void UConversationManager::OpenNPCConversationWidget()
{
	check(ConversationWidget);
	ConversationWidget->OpenNPCConversationPage();
}

void UConversationManager::BindConversationWidgetSignature()
{
	ConversationWidget->OnAnswerButtonClickedSignature.AddUObject(this, &UConversationManager::OnAnswerButtonClickedSignature);
	ConversationWidget->OnNPCConversationPageClickedSignature.AddUObject(this, &UConversationManager::OnNPCConversationPageClicked);
}

void UConversationManager::FinishConversation()
{
	CloseConversationWidget();

	auto Outer = GetOuter();
	check(Outer);

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(UGameplayStatics::GetPlayerController(Outer, 0));
	check(PlayerController);

	PlayerController->SetGameModeGameOnly();
}

void UConversationManager::OnNPCConversationPageClicked()
{
	if (NPCScriptIndex == -1)
	{
		if (ConversationScriptDataArray[CurrentConversationIndex]->PlayerAnswerIndexArray.Num() > 0)
		{
			ConversationWidget->OpenAnswerPage();
			SetPlayerAnswers(CurrentConversationIndex);
			UE_LOG(LogTemp, Warning, TEXT("UConversationManager::OnNPCConversationPageClicked, Entered"));
		}
		else
		{
			SetBehavior(EBehavior::JustClose);
			//TODO :: 상점 열기 등 행동
			UE_LOG(LogTemp, Warning, TEXT("UConversationManager::OnNPCConversationPageClicked, ELSE"));
		}
	}
	else
	{
		SetNPCConversationInternal(CurrentConversationIndex);
	}
}

void UConversationManager::OnAnswerButtonClickedSignature(int32 ButtonIndex)
{
	bool bHasBehavior = (ConversationScriptDataArray[CurrentConversationIndex]->BehaviorArray.Num() > 0);
	CurrentConversationIndex = ConversationScriptDataArray[CurrentConversationIndex]->PlayerAnswerIndexArray[ButtonIndex];
	
	if (CurrentConversationIndex == 0)
	{
		SetBehavior(EBehavior::JustClose);
		return;
	}

	OpenNPCConversationWidget();
	SetNPCConversation(CurrentConversationIndex);

	// TODO : 행동을 정해야 한다..........
	// 그리고 INDEX 설정에 대해 고민해야한다
	// enum으로 할까
}
