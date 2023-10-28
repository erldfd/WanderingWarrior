// Fill out your copyright notice in the Description page of Project Settings.


#include "MiscItem.h"

#include "Components/CharacterStatComponent.h"
#include "Controller/WWPlayerController.h"
#include "Character/PlayerCharacter.h"
#include "WWEnumClassContainer.h"

AMiscItem::AMiscItem()
{
	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComponent"));
	RootComponent = ItemStaticMeshComponent;


}

void AMiscItem::Use(AWWCharacter* ItemUser)
{
	UCharacterStatComponent* CharacterStat = Cast<UCharacterStatComponent>(ItemUser->GetCharacterStatComponent());
	if (CharacterStat == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMiscItem::Use, CharacterStat == nullptr"));
		return;
	}

	switch (MiscItemName)
	{
	case EMiscItemName::HPPotion:
		CharacterStat->SetHP(CharacterStat->GetHP() + 5);
		break;
	case EMiscItemName::MPPotion:
		break;
	default:
		break;
	}
}

void AMiscItem::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}