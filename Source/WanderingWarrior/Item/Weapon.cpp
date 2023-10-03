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
	//BoxComponent->SetBoxExtent(FVector(32, 32, 32));
	//BoxComponent->SetRelativeScale3D(FVector(1, 0.625, 2.375));
	//BoxComponent->SetRelativeLocation(FVector(0, 14, 43));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlappedComponent : %s"), *OverlappedComponent->GetName());
	if (Super::bIsFieldItem)
	{
		return;
	}

	ABossCharacter* Boss = Cast<ABossCharacter>(OtherActor);
	if (Boss) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, Boss"));
	}

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, EnemyCharacter == nullptr"));
		return;
	}

	AWWGameMode* GameMode = Cast<AWWGameMode>(GetWorld()->GetAuthGameMode());
	if(ensure(GameMode) == false) return;

	UWWAnimInstance* PlayerAnimInstance = GameMode->GetPlayerAnimInstance();
	if(ensure(PlayerAnimInstance) == false) return;

	//bool IsAttacking = PlayerAnimInstance->GetIsAttacking();
	
	bool IsDetectedAttack = PlayerAnimInstance->GetIsAttackDetected();
	UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnMeshBeginOverlap, Damaged : %d, DetectedAttack : %d"), EnemyCharacter->GetIsDamaged(), IsDetectedAttack);

	if (true/*EnemyCharacter->GetIsDamaged() == false*//* &&*/ /*IsAttacking*//*IsDetectedAttack*/)
	{
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.5f);
		EnemyCharacter->SetIsDamaged(true);

		AActor* WeaponOwner = GetOwner();
		if (ensure(WeaponOwner) == false) return;

		AWWPlayerController* PlayerController = Cast<AWWPlayerController>(WeaponOwner->GetInstigatorController());
		if (ensure(PlayerController) == false) return;

		FVector MoveDir = EnemyCharacter->GetActorLocation() - WeaponOwner->GetActorLocation();
		MoveDir.Normalize();

		FDamageEvent DamageEvent;
		EnemyCharacter->TakeDamageWithKnockback(AttackDamage, DamageEvent, PlayerController, WeaponOwner, MoveDir * 1000, 0.1f, true);

		UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
		if (ensure(PlayerInGameWidget) == false) return;

		PlayerInGameWidget->SetEnemyHPBarPercent(EnemyCharacter->GetCharacterStatComponent()->GetHPRatio());
		PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(EnemyCharacter->GetCharacterName()));
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::SetAttackDamage(float NewDamage)
{
	AttackDamage = NewDamage;
}

void AWeapon::Use(const UWorld& World)
{
	check(&World);

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(World.GetFirstPlayerController());
	check(PlayerController);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	check(PlayerCharacter);

	PlayerCharacter->EquipWeapon(this);
}

void AWeapon::SetBoxComponentCollision(ECollisionEnabled::Type NewType)
{
	BoxComponent->SetCollisionEnabled(NewType);
}