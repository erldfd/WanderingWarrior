// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "WWAnimInstance.h"
#include "WWGameInstance.h"
#include "WWGameMode.h"
#include "WWEnumClassContainer.h"
#include "WWConstContainer.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "Components/PlayerSkillComponent.h"
#include "Components/CharacterStatComponent.h"
#include "Inventory/InventorySlotWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryTabData.h"
#include "Inventory/InventorySlotData.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/CharacterQuickSlot.h"
#include "Inventory/InventoryComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	check(SpringArm);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	check(Camera);

	SpringArm->bUsePawnControlRotation = true;//SpringArm이 마우스따라 상하로 로테이션하는듯?
	SpringArm->bInheritPitch = true;//로컬 로테이션인가 뭐지..... 암튼 상하
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;//마우스가 움직이면 좌우로 스프링암이 로테이션
	SpringArm->bDoCollisionTest = true;//스프링암이 다른것과 충돌하면 통과하지않음

	// 컨트롤러가 로테이션할때 카메라가 로테이션하지않음 이라네요
	bUseControllerRotationYaw = false;//마우스 움직이면 캐릭터가 좌우로 로테이션함 z 로테이션
	bUseControllerRotationPitch = false;//마우스 움직이면 캐릭터가 상하로 로테이션 x로테이션
	bUseControllerRotationRoll = false;// y로테이션

	UCapsuleComponent* BodyCapsuleComponent = GetCapsuleComponent();
	check(BodyCapsuleComponent);

	SpringArm->SetupAttachment((USceneComponent*)BodyCapsuleComponent);
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));

	PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkill"));

	Tags.Init("Player", 1);

	Inventory = CreateDefaultSubobject<UCharacterInventory>(TEXT("NewInventory"));
	QuickSlot = CreateDefaultSubobject<UCharacterQuickSlot>(TEXT("NewQuickSlot"));

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHitToSomething);
	if (GetCapsuleComponent()->OnComponentHit.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::APlayerCharacter, GetCapsuleComponent()->OnComponentHit is NOT bound"));
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlapWithSomething);
	if (GetCapsuleComponent()->OnComponentBeginOverlap.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::APlayerCharacter, GetCapsuleComponent()->OnComponentBeginOverlap is NOT bound"));
	}

	OnActorEndOverlap.AddDynamic(this, &APlayerCharacter::OnEndOverlapWithSomething);
	if (OnActorEndOverlap.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::APlayerCharacter, OnActorEndOverlap is NOT bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::APlayerCharacter, OnActorEndOverlap is bound"));
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TempSwapSlot = NewObject<UInventorySlotData>(this);

	Super::AnimInstance->OnStartNextComboDelegate.AddUObject(this, &APlayerCharacter::OnStartNextCombo);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (SubSystem && CharacterInput)
	{
		SubSystem->AddMappingContext(CharacterInput, 0);
	}

	for (int i = 0; i < SlotCount::WEAPON_TAB_SLOT_COUNT; ++i)
	{
		Inventory->RemoveAllItem(i);
	}
	
	Inventory->ObtainItem(EWeaponName::BlackSword);
	Inventory->ObtainItem(EWeaponName::WhiteSword);
	
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);

	QuickSlot->ObtainItem(EMiscItemName::HPPotion);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (bIsWet)
	{
		if (FootprintDelay > 0)
		{
			FootprintDelay += 1 * DeltaTime;

			if (FootprintDelay >= 0.25)
			{
				FootprintDelay = 0;
			}

			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Tick, bIsReadyToLeftFootprint : %d, bIsMoved : %d "), bIsReadyToLeftFootprint, bIsMoved);
		if (bIsMoved == false || GetMovementComponent()->IsFalling())
		{
			return;
		}

		FootprintDelay += 1 * DeltaTime;
		FVector DecalLocation = GetActorLocation();
		DecalLocation.Z -= 90.0;

		if (bIsReadyToLeftFootprint)
		{
			DecalLocation -= GetActorRightVector() * 20;
		}
		else
		{
			DecalLocation += GetActorRightVector() * 20;
		}

		ADecalActor* FootprintDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, GetActorRotation());
		if (FootprintDecal == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Tick, No decal spawned"));
			return;
		}

		if (bIsReadyToLeftFootprint)
		{
			FootprintDecal->SetDecalMaterial(LeftFootprintMaterialInstance);
			bIsReadyToLeftFootprint = false;
		}
		else
		{
			FootprintDecal->SetDecalMaterial(RightFootprintMaterialInstance);
			bIsReadyToLeftFootprint = true;
		}

		FootprintDecal->SetLifeSpan(2.0f);
		FootprintDecal->GetDecal()->DecalSize = FVector(16.0f, 32.0f, 32.0f);
		bIsMoved = false;
	}*/
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponenet->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponenet->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponenet->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

	EnhancedInputComponenet->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWWCharacter::Attack);

	EnhancedInputComponenet->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	EnhancedInputComponenet->BindAction(ChargeAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoChargeAttack);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter PossessedBy"));

	Cast<AWWGameMode>(GetWorld()->GetAuthGameMode())->SetPlayerAnimInstance(AnimInstance);
	//Super::EquipWeapon(Cast<UWWGameInstance>(GetGameInstance())->GetWeapon(EWeaponName::BlackSword));
}

UPlayerSkillComponent& APlayerCharacter::GetPlayerSkillComponenet()
{
	check(PlayerSkillComponent);
	return *PlayerSkillComponent;
}

class UCharacterQuickSlot& APlayerCharacter::GetQuickSlot()
{
	check(QuickSlot);
	return *QuickSlot;
}

UCharacterInventory& APlayerCharacter::GetInventory() const
{
	return *Inventory;
}

void APlayerCharacter::Attack(float Value)
{
	if (Inventory->IsInventoryVisible())
	{
		return;
	}

	Super::Attack(Value);
}

void APlayerCharacter::DoChargeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::DoChargeAttack"));

	bool bIsAttacking = AnimInstance->GetIsAttacking();
	bool bCanCombo = AnimInstance->GetCanCombo();
	bool bWillPlayNextCombo = AnimInstance->GetWillPlayNextCombo();
	bool bIsDead = AnimInstance->GetIsDead();

	bool bIsPlayingJumpToGroundSkill = AnimInstance->GetIsPlayingChargeAttack1Anim();

	bool bIsPlayingChargeAttack2Anim = AnimInstance->GetIsPlayingChargeAttack2Anim();
	bool bWillPlayChargeAttack2Anim = AnimInstance->GetWillPlayChargeAttack2Anim();

	bool bIsPlayingChargeAttack3Anim = AnimInstance->GetIsPlayingChargeAttack3Anim();
	bool bWillPlayChargeAttack3Anim = AnimInstance->GetWillPlayChargeAttack3Anim();

	if (bIsDead || bIsPlayingJumpToGroundSkill || bIsPlayingChargeAttack2Anim || bIsPlayingChargeAttack3Anim ||
		bIsAttacking == false || bCanCombo == false || bWillPlayNextCombo)
	{
		return;
	}

	int ComboCount = AnimInstance->GetComboCount();

	if (bWillPlayChargeAttack2Anim == false && ComboCount == 1)
	{

		AnimInstance->SetWillPlayChargeAttack2Anim(true);
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::DoChargeAttack, SetWillKick"));
	}
	else if (bWillPlayChargeAttack3Anim == false && ComboCount == 2)
	{
		AnimInstance->SetWillPlayChargeAttack3Anim(true);
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::DoChargeAttack, SetWillKick"));
	}
}

UCameraComponent& APlayerCharacter::GetCamera()
{
	check(Camera);
	return *Camera;
}

bool APlayerCharacter::GetIsInWater()
{
	return bIsInWater;
}

bool APlayerCharacter::GetIsWet()
{
	return bIsWet;
}

void APlayerCharacter::OnStartNextCombo()
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector LookVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * Super::InputForwardValue + FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * Super::InputRightValue;

	if (LookVector == FVector::ZeroVector)
	{
		LookVector = GetActorForwardVector();
	}

	SetActorRotation(FRotationMatrix::MakeFromX(LookVector).Rotator());
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Super::AnimInstance->IsPlayingSomething())
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

	bIsMoved = true;
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::OnHitToSomething(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->Tags.IsEmpty() || OtherActor->Tags.IsValidIndex(0) == false)
	{
		return;
	}

	if (OtherActor->Tags[0] == "Water")
	{
		
	}
}

void APlayerCharacter::OnBeginOverlapWithSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->Tags.IsEmpty() || OtherActor->Tags.IsValidIndex(0) == false)
	{
		return;
	}

	if (OtherActor->Tags[0] == "Water" && bIsWet == false)
	{
		bIsWet = true;
		bIsInWater = true;
	}
}

void APlayerCharacter::OnEndOverlapWithSomething(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->Tags.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::OnEndOverlapWithSomething, Empty"))
	}
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::OnEndOverlapWithSomething, OtherActor Name : %s"), *OtherActor->Tags[0].ToString());
}

