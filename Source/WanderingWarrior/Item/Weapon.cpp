// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Character/EnemyCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Character/BossCharacter.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "WWGameMode.h"
#include "Controller/WWPlayerController.h"
#include "Components/CharacterStatComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AWeapon::AWeapon() : AttackDamage(1)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMeshComponent"));
	RootComponent = ItemSkeletalMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("PlayerWeaponProfile"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnMeshBeginOverlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	if (BoxComponent->OnComponentBeginOverlap.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::AWeapon, BoxComponent->OnComponentBeginOverlap is NOT bound"));
	}
	
	BoxComponent->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsFieldItem)
	{
		return;
	}

	if (bIsSwingStarted == false)
	{
		return;
	}
	
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, Owner == nullptr"));
		return;
	}

	if (WeaponOwner == OtherActor)
	{
		return;
	}

	AWWCharacter* OverlappedCharacter = Cast<AWWCharacter>(OtherActor);
	if (OverlappedCharacter == nullptr)
	{
		return;
	}

	if (DamagedTargetSet.Contains(OverlappedCharacter))
	{
		return;
	}

	DamagedTargetSet.Emplace(OverlappedCharacter);

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(WeaponOwner->GetInstigatorController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, PlayerController == nullptr"));
		return;
	}

	FVector MoveDir = OverlappedCharacter->GetActorLocation() - WeaponOwner->GetActorLocation();
	MoveDir.Normalize();

	FDamageEvent DamageEvent;
	OverlappedCharacter->TakeDamageWithKnockback(AttackDamage, DamageEvent, PlayerController, WeaponOwner, MoveDir * 1000, 0.1f, true);

	UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
	if (PlayerInGameWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, PlayerInGameWidget == nullptr"));
		return;
	}

	PlayerInGameWidget->SetEnemyHPBarPercent(OverlappedCharacter->GetCharacterStatComponent()->GetHPRatio());
	PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(OverlappedCharacter->GetCharacterName()));
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::SetAttackDamage(float NewDamage)
{
	AttackDamage = NewDamage;
}

bool AWeapon::Use(AWWCharacter* ItemUser)
{
	ItemUser->EquipWeapon(this);
	return true;
}

void AWeapon::SetBoxComponentCollision(ECollisionEnabled::Type NewType)
{
	BoxComponent->SetCollisionEnabled(NewType);
}

void AWeapon::StartSwing()
{
	bIsSwingStarted = true;
	SetActorEnableCollision(true);
}

void AWeapon::EndSwing()
{
	bIsSwingStarted = false;
	SetActorEnableCollision(false);

	int32 SetNum = DamagedTargetSet.Num();

	if (SetNum > ExpectedNumElements)
	{
		ExpectedNumElements = SetNum;
	}

	DamagedTargetSet.Empty(ExpectedNumElements);
}
