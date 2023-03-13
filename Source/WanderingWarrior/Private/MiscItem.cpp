// Fill out your copyright notice in the Description page of Project Settings.


#include "MiscItem.h"

#include "CharacterStatComponent.h"
#include "WWPlayerController.h"
#include "PlayerCharacter.h"

AMiscItem::AMiscItem()
{
	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComponent"));
	RootComponent = ItemStaticMeshComponent;
}

void AMiscItem::Use(const UWorld& World)
{
	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(World.GetFirstPlayerController());
	check(PlayerController != nullptr);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	check(PlayerCharacter != nullptr);

	UCharacterStatComponent* CharacterStat = Cast<UCharacterStatComponent>(PlayerCharacter->GetCharacterStatComponent());
	check(CharacterStat != nullptr);

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