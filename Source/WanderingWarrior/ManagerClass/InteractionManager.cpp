// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"

#include "ConversationManager.h"
#include "WWGameInstance.h"
#include "Character/NPCCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "Inventory/CharacterInventory.h"


#include "Kismet/GameplayStatics.h"

UInteractionManager::UInteractionManager()
{

}

void UInteractionManager::AnalyzeInteraction(const TArray<FOverlapResult>& OverlapResults)
{
	int32 OverlapNum = OverlapResults.Num();

	for (int32 i = 0; i < OverlapNum; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, OverlapResults[%d] : %s"), i, *OverlapResults[i].GetActor()->GetName());

		AActor& OverlappedActor = *OverlapResults[i].GetActor();

		if (OverlappedActor.Tags.IsValidIndex(0) == false)
		{
			continue;
		}

		FName& ActorTag = OverlappedActor.Tags[0];

		if (ActorTag == "Marchant")
		{
			ANPCCharacter& Marchant = *Cast<ANPCCharacter>(&OverlappedActor);

			static UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(OverlappedActor.GetWorld()));
			check(&GameInstance);

			GameInstance.GetConversationManager().SetNPCNameText(FText::FromString(Marchant.GetNPCName()));

			OnStartConversationSignature.Broadcast(&Marchant);

			return;
		}
		else if (ActorTag == "Weapon")
		{
			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, FoundWeapon"));
			AWeapon& Weapon = *Cast<AWeapon>(&OverlappedActor);

			if (Weapon.GetbIsFieldItem() == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, Weapon is not Field item"));
				continue;
			}

			FName& WeaponTag = OverlappedActor.Tags[1];

			static APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
			check(PlayerCharacter);

			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, PlayerName : %s"), *PlayerCharacter->GetName());

			UCharacterInventory* PlayerInventory = PlayerCharacter->GetInventory();

			const UEnum* WeaponNameEnum = FindObject<UEnum>(nullptr, TEXT("/Script/WanderingWarrior.EWeaponName"));

			if (!WeaponNameEnum)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, Failed to Find UEnum EWeaponName"));
				return;
			}

			EWeaponName WeaponName = (EWeaponName)WeaponNameEnum->GetValueByName(WeaponTag);

			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, EWeaponName : %d "), (int32)WeaponName);

			PlayerInventory->ObtainItem(WeaponName);
			OverlappedActor.Destroy();

			return;
		}
		else if (ActorTag == "MiscItem")
		{
			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, Found MiscItem"));
			AMiscItem& MiscItem = *Cast<AMiscItem>(&OverlappedActor);

			if (MiscItem.GetbIsFieldItem() == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, MiscItem is not Field item"));
				continue;
			}

			FName& MiscItemTag = OverlappedActor.Tags[1];

			static APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
			check(PlayerCharacter);

			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, PlayerName : %s"), *PlayerCharacter->GetName());

			UCharacterInventory* PlayerInventory = PlayerCharacter->GetInventory();

			const UEnum* MiscItemNameEnum = FindObject<UEnum>(nullptr, TEXT("/Script/WanderingWarrior.EMiscItemName"));

			if (!MiscItemNameEnum)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, Failed to Find UEnum EMiscItemName"));
				return;
			}

			EMiscItemName MiscItemName = (EMiscItemName)MiscItemNameEnum->GetValueByName(MiscItemTag);

			UE_LOG(LogTemp, Warning, TEXT("UInteractionManager::AnalyzeInteraction, EWeaponName : %d "), (int32)MiscItemName);

			PlayerInventory->ObtainItem(MiscItemName);
			OverlappedActor.Destroy();

			return;
		}
	}
}
