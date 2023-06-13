// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"

#include "ConversationManager.h"
#include "WanderingWarrior/WWGameInstance.h"
#include "WanderingWarrior/Character/NPCCharacter.h"

#include "Kismet/GameplayStatics.h"

UInteractionManager::UInteractionManager()
{

}

void UInteractionManager::AnalyzeInteraction(const TArray<FOverlapResult>& OverlapResults)
{
	int32 OverlapNum = OverlapResults.Num();

	for (int32 i = 0; i < OverlapNum; ++i)
	{
		if (OverlapResults[i].GetActor()->Tags.IsValidIndex(0) == false)
		{
			continue;
		}

		AActor* OverlappedActor = OverlapResults[i].GetActor();
		FName& ActorTag = OverlappedActor->Tags[0];

		if (ActorTag == "Marchant")
		{
			ANPCCharacter* Marchant = Cast<ANPCCharacter>(OverlappedActor);

			UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(OverlapResults[i].GetActor()->GetWorld()));
			check(GameInstance);

			GameInstance->GetConversationManager()->SetNPCNameText(FText::FromString(Marchant->GetNPCName()));

			OnStartConversationSignature.Broadcast(Marchant);

			return;
		}
	}
}
