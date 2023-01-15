// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "WWAnimInstance.h"
#include "WWGameInstance.h"
#include "WWGameMode.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));

	Tags.Init("", 1);
	Tags[0] = TEXT("Enemy");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter BeginPlay"));

	UWWAnimInstance* PlayerAnimInstance = Cast<AWWGameMode>(GetWorld()->GetAuthGameMode())->GetPlayerAnimInstance();
	ensure(PlayerAnimInstance != nullptr);

	PlayerAnimInstance->OnStartNextComboDelegate.AddLambda([this]()->void {SetIsDamaged(false); });
	PlayerAnimInstance->OnAttackEndDelegate.AddLambda([this]()->void {SetIsDamaged(false); });
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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

void AEnemyCharacter::Attack(float Value)
{
	Super::Attack(Value);
}

bool AEnemyCharacter::GetIsDamaged()
{
	return bIsDamaged;
}

void AEnemyCharacter::SetIsDamaged(bool bNewIsDamaged)
{
	bIsDamaged = bNewIsDamaged;
}

