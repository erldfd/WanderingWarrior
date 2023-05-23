// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "WanderingWarrior/Character/EnemyCharacter.h"
#include "WanderingWarrior/WWAnimInstance.h"
#include "WanderingWarrior/InGameWidget.h"
#include "WanderingWarrior/Controller/WWPlayerController.h"
#include "WanderingWarrior/WWGameMode.h"
#include "WanderingWarrior/Components/CharacterStatComponent.h"
#include "WanderingWarrior/Character/PlayerCharacter.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Mesh Overlap Fn Bound? %d"), BoxComponent->OnComponentBeginOverlap.IsBound());
	
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
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	ensure(EnemyCharacter != nullptr);

	AWWGameMode* GameMode = Cast<AWWGameMode>(GetWorld()->GetAuthGameMode());
	ensure(GameMode != nullptr);

	UWWAnimInstance* PlayerAnimInstance = GameMode->GetPlayerAnimInstance();
	ensure(PlayerAnimInstance != nullptr);

	bool IsAttacking = PlayerAnimInstance->GetIsAttacking();

	if (EnemyCharacter->GetIsDamaged() == false && IsAttacking)
	{
		EnemyCharacter->SetIsDamaged(true);

		FDamageEvent DamageEvent;
		EnemyCharacter->Super::TakeDamage(AttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), this);

		AWWPlayerController* PlayerController = Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
		ensure(PlayerController != nullptr);

		UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
		ensure(PlayerInGameWidget != nullptr);

		PlayerInGameWidget->SetEnemyHPBarPercent(EnemyCharacter->GetCharacterStatComponent()->GetHPRatio());
		PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(EnemyCharacter->GetCharacterName()));
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
	check(&World != nullptr);

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(World.GetFirstPlayerController());
	check(PlayerController != nullptr);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	check(PlayerCharacter != nullptr);

	PlayerCharacter->EquipWeapon(this);
}
