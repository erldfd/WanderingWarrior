// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_DrinkingWindow.h"

#include "Character/WWCharacter.h"
#include "Item/MiscItem.h"
#include "WWEnumClassContainer.h"
#include "Components/CharacterStatComponent.h"

void UANS_DrinkingWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_DrinkingWindow::NotifyEnd, OwnerCharacter == nullptr"));
		return;
	}

	AMiscItem* DrinkingItem = OwnerCharacter->GetCurrentDrinkingItem();
	if (DrinkingItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_DrinkingWindow::NotifyEnd, DrinkingItem == nullptr"));
		return;
	}

	if (DrinkingItem->GetMiscItemName() == EMiscItemName::HPPotion)
	{
		UCharacterStatComponent* StatComp = OwnerCharacter->GetCharacterStatComponent();
		if (StatComp == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UANS_DrinkingWindow::NotifyEnd, StatComp == nullptr"));
			return;
		}

		StatComp->SetHP(StatComp->GetHP() + 50);
		DrinkingItem->Destroy();
		OwnerCharacter->SetIsDrinking(false);
	}
}
