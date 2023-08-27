// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "WWAnimInstance.h"
#include "WWGameInstance.h"
#include "WWGameMode.h"
#include "WWEnumClassContainer.h"
#include "Components/CharacterStatComponent.h"
#include "Item/MiscItem.h"
#include "Item/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter() : bIsDamaged(false)
{
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));

	Tags.Init("", 1);
	Tags[0] = TEXT("Enemy");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWWAnimInstance& PlayerAnimInstance = *Cast<AWWGameMode>(GetWorld()->GetAuthGameMode())->GetPlayerAnimInstance();
	if (ensure(&PlayerAnimInstance) == false) return;

	PlayerAnimInstance.OnInitIsDamaged.AddLambda([this]()->void {
		
		SetIsDamaged(false);
	
	});
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float HPBeforeDamage = CharacterStatComponent->GetHP();
	float HPAfterDamage = HPBeforeDamage - Damage;

	if (Super::AnimInstance->GetIsDead())
	{
		//TODO : drop item randomly
		int RandomInt = FMath::RandRange(0, 3);
		UE_LOG(LogTemp, Warning, TEXT("RandomInt : %d"), RandomInt);

		if (RandomInt == 0)
		{
			return Damage;
		}

		UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
		check(&GameInstance);

		if (RandomInt == 1)
		{
			AWeapon& Item = *Cast<AWeapon>(&GameInstance.SpawnWeapon(EWeaponName::BlackSword, GetActorLocation()));
			UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Item.GetName());

			Item.SetbisRotatingOnField(true);

		}
		else if (RandomInt == 2)
		{
			AWeapon& Item = *Cast<AWeapon>(&GameInstance.SpawnWeapon(EWeaponName::WhiteSword, GetActorLocation()));
			UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Item.GetName());

			Item.SetbisRotatingOnField(true);
		}
		else if (RandomInt == 3)
		{
			AMiscItem& Item = *Cast<AMiscItem>(&GameInstance.SpawnMiscItem(EMiscItemName::HPPotion, GetActorLocation()));
			UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Item.GetName());

			Item.SetbisRotatingOnField(true);
		}
	}

	return Damage;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

//void AEnemyCharacter::Attack(float Value)
//{
//	Super::Attack(Value);
//}

bool AEnemyCharacter::GetIsDamaged()
{
	return bIsDamaged;
}

void AEnemyCharacter::SetIsDamaged(bool bNewIsDamaged)
{
	bIsDamaged = bNewIsDamaged;
}

