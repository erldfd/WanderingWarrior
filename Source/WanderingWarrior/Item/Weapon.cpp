// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Character/EnemyCharacter.h"
#include "Character/PlayerCharacter.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "WWGameMode.h"
#include "Controller/WWPlayerController.h"
#include "Components/CharacterStatComponent.h"

#include "Components/BoxComponent.h"

// Sets default values
AWeapon::AWeapon() : AttackDamage(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Super::bIsFieldItem)
	{
		return;
	}
	
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	ensure(EnemyCharacter);

	AWWGameMode* GameMode = Cast<AWWGameMode>(GetWorld()->GetAuthGameMode());
	ensure(GameMode);

	UWWAnimInstance* PlayerAnimInstance = GameMode->GetPlayerAnimInstance();
	ensure(PlayerAnimInstance);

	bool IsAttacking = PlayerAnimInstance->GetIsAttacking();

	if (EnemyCharacter->GetIsDamaged() == false && IsAttacking)
	{
		EnemyCharacter->SetIsDamaged(true);

		FDamageEvent DamageEvent;
		EnemyCharacter->TakeDamage(AttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

		AWWPlayerController* PlayerController = Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
		if (ensure(PlayerController)) return;

		UInGameWidget& PlayerInGameWidget = PlayerController->GetInGameWidget();
		if (ensure(&PlayerInGameWidget)) return;

		PlayerInGameWidget.SetEnemyHPBarPercent(EnemyCharacter->GetCharacterStatComponent().GetHPRatio());
		PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(EnemyCharacter->GetCharacterName()));
	}
}

// Called every frame
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
