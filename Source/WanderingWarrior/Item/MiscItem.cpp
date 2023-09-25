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

void AMiscItem::Use(const UWorld& World)
{
	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(World.GetFirstPlayerController());
	check(PlayerController);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	check(PlayerCharacter);

	UCharacterStatComponent* CharacterStat = Cast<UCharacterStatComponent>(PlayerCharacter->GetCharacterStatComponent());
	check(CharacterStat);

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