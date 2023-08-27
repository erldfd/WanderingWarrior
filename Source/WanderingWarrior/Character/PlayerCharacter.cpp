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
#include "Data/SkillDataAsset.h"
#include "Components/WarriorSkillComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	check(SpringArm);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	check(Camera);

	ActionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ACTIONCAMERA"));
	check(ActionCamera);

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
	ActionCamera->SetupAttachment((USceneComponent*)BodyCapsuleComponent);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));

	//PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkill"));
	SkillComponent = CreateDefaultSubobject<UWarriorSkillComponent>(TEXT("Skill"));

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

	ParryWindow = 1;

	CameraTransformArrowOrigin = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraArrowOrigin"));
	CameraTransformArrowTarget = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraArrowTarget"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TempSwapSlot = NewObject<UInventorySlotData>(this);

	Super::AnimInstance->OnStartNextComboDelegate.AddUObject(this, &APlayerCharacter::OnStartNextCombo);
}

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

	CharacterStatComponent->SetMP(CharacterStatComponent->GetMaxMP());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetIsMusouAttackStarted() && CharacterStatComponent->GetMP() > 0)
	{
		CharacterStatComponent->SetMP(CharacterStatComponent->GetMP() - 1 * DeltaTime);
	}
	else if (GetIsMusouAttackStarted() == false && CharacterStatComponent->GetMP() < CharacterStatComponent->GetMaxMP())
	{
		CharacterStatComponent->SetMP(CharacterStatComponent->GetMP() + 1 * DeltaTime);
	}

	if (ParryLeftTime > 0 && GetIsGuarding())
	{
		ParryLeftTime -= DeltaTime;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponenet->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
	EnhancedInputComponenet->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponenet->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

	EnhancedInputComponenet->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWWCharacter::Attack);

	EnhancedInputComponenet->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	EnhancedInputComponenet->BindAction(ChargeAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoChargeAttack);

	EnhancedInputComponenet->BindAction(MusouAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoMusouAttack);

	EnhancedInputComponenet->BindAction(GuardAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoGuard);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Cast<AWWGameMode>(GetWorld()->GetAuthGameMode())->SetPlayerAnimInstance(AnimInstance);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AnimInstance->GetIsParrying())
	{
		return 0.0f;
	}

	if (ParryLeftTime > 0 && GetIsGuarding() && GetSkillComponent()->IsSkillStarted() == false)
	{
		SetIsParrySucceeded(true);
		//AnimInstance->PlayParryAttackAnim();
		PlayParryAttack();

		return 0.0f;
	}

	if (GetIsGuarding() || AnimInstance->GetIsGuardHitStart())
	{
		AnimInstance->PlayGuardHitAnim();

		FVector Diraction = GetActorLocation() - DamageCauser->GetActorLocation();
		Diraction.Normalize();

		Super::StartKnockback(Diraction, 100, 0.2);

		return 0.0f;
	}

	if (AnimInstance->GetIsActingMusou())
	{
		return 0.0f;
	}

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

//UPlayerSkillComponent& APlayerCharacter::GetPlayerSkillComponenet()
//{
//	check(PlayerSkillComponent);
//	return *PlayerSkillComponent;
//}

class UCharacterQuickSlot& APlayerCharacter::GetQuickSlot()
{
	check(QuickSlot);
	return *QuickSlot;
}

UCharacterInventory& APlayerCharacter::GetInventory() const
{
	return *Inventory;
}

//void APlayerCharacter::Attack()
//{
//	Super::Attack();
//}

//void APlayerCharacter::DoChargeAttack()
//{
//	Super::DoChargeAttack();
//}

UCameraComponent* APlayerCharacter::GetCamera()
{
	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::GetCamera, Camera == nullptr"));
	}

	return Camera;
}

UCameraComponent& APlayerCharacter::GetActionCamera()
{
	check(ActionCamera);
	return *ActionCamera;
}

bool APlayerCharacter::GetIsInWater()
{
	return bIsInWater;
}

bool APlayerCharacter::GetIsWet()
{
	return bIsWet;
}

//void APlayerCharacter::DoMusouAttack()
//{
//	float CurrentMP = CharacterStatComponent->GetMP();
//	float MaxMP = CharacterStatComponent->GetMaxMP();
//	bool bIsPlayingMusou = GetIsMusouAttackStarted();
//
//	if (bIsPlayingMusou == false && CurrentMP < MaxMP || GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	if (bIsPlayingMusou == false && CurrentMP == MaxMP)
//	{
//		PlayMusouAttack();
//		SetIsMusouAttackStarted(true);
//
//		SetIsParrySucceeded(false);
//		SkillComponenet->SetIsChargeAttack1Started(false);
//		SkillComponenet->SetIsChargeAttack2Started(false);
//		SkillComponenet->SetIsChargeAttack3Started(false);
//		SetIsAttacking(false);
//		SetIsGuarding(false);
//		AnimInstance->SetIsGuardHitStart(false);
//		SetWillPlayNextCombo(false);
//		SetWillPlayChargeAttack2(false);
//		SetWillPlayChargeAttack3(false);
//	}
//	else if (bIsPlayingMusou && CurrentMP < MaxMP && CurrentMP > 0)
//	{
//		bIsConsistentMusou = true;
//	}
//
//	/*FMotionWarpingTarget Target;
//	float Distance = 500.0f;
//	Target.Name = FName("AttackTarget");
//	Target.Location = GetActorLocation() + GetActorForwardVector() * Distance;
//	Target.Rotation = GetActorRotation();
//
//	MotionWarpComponent->AddOrUpdateWarpTarget(Target);*/
//}

//void APlayerCharacter::DoGuard(const FInputActionValue& Value)
//{
//	if (AnimInstance->GetIsPlayingCharacterHitMontage() || bPressedJump)
//	{
//		return;
//	}
//
//	if (GetIsParrySucceeded())
//	{
//		AnimInstance->SetIsGuarding(false);
//		return;
//	}
//
//	bIsGuarding = Value.Get<bool>();
//	
//	if (bIsGuarding == false)
//	{
//		ParryLeftTime = ParryWindow;
//	}
//
//	AnimInstance->SetIsGuarding(bIsGuarding);
//}

//bool APlayerCharacter::GetPlayingMusou()
//{
//	return bIsPlayingMusou;
//}
//
//void APlayerCharacter::SetPlayingMusou(bool NewPlayingMusou)
//{
//	bIsPlayingMusou = NewPlayingMusou;
//}

//bool APlayerCharacter::GetIsConsistentMusou() const
//{
//	return bIsConsistentMusou;
//}
//
//void APlayerCharacter::SetIsConsistentMusou(bool bNewIsConsistentMusou)
//{
//	bIsConsistentMusou = bNewIsConsistentMusou;
//}

class UArrowComponent& APlayerCharacter::GetCameraTransformArrowOrigin()
{
	return *CameraTransformArrowOrigin;
}

class UArrowComponent& APlayerCharacter::GetCameraTransformArrowTarget()
{
	return *CameraTransformArrowTarget;
}

//bool APlayerCharacter::GetIsParrySucceeded() const
//{
//	return bIsParrySucceeded;
//}
//
//void APlayerCharacter::SetIsParrySucceeded(bool NewIsParrySucceeded)
//{
//	bIsParrySucceeded = NewIsParrySucceeded;
//}

//UWarriorSkillComponent* APlayerCharacter::GetSkillComponenet()
//{
//	return SkillComponenet;
//}

//bool APlayerCharacter::GetWillPlayChargeAttack2() const
//{
//	return bWillPlayChargeAttack2;
//}
//
//void APlayerCharacter::SetWillPlayChargeAttack2(bool bNewWillPlayChargeAttack2)
//{
//	bWillPlayChargeAttack2 = bNewWillPlayChargeAttack2;
//}
//
//bool APlayerCharacter::GetWillPlayChargeAttack3() const
//{
//	return bWillPlayChargeAttack3;
//}
//
//void APlayerCharacter::SetWillPlayChargeAttack3(bool bNewWillPlayChargeAttack3)
//{
//	bWillPlayChargeAttack3 = bNewWillPlayChargeAttack3;
//}
//
//bool APlayerCharacter::GetIsGuarding() const
//{
//	return bIsGuarding;
//}
//
//void APlayerCharacter::SetIsGuarding(bool bNewIsGuarding)
//{
//	bIsGuarding = bNewIsGuarding;
//}

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
	Super::Move(Value);
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

//void APlayerCharacter::PlayChargeAttack1()
//{
//	if (GetSkillComponenet()->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	float PlayRate = 1.0f;
//	GetSkillComponenet()->PlayChargeAttack1(PlayRate);
//}

//void APlayerCharacter::PlayChargeAttack2()
//{
//	if (GetSkillComponenet()->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	float PlayRate = 1.0f;
//	GetSkillComponenet()->PlayChargeAttack2(PlayRate);
//}

//void APlayerCharacter::PlayChargeAttack3()
//{
//	if (GetSkillComponenet()->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	float PlayRate = 1.0f;
//	GetSkillComponenet()->PlayChargeAttack3(PlayRate);
//}

//void APlayerCharacter::PlayMusouAttack()
//{
//	if (GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	float PlayRate = 1;
//	GetSkillComponenet()->PlayMusouAttack(PlayRate);
//}

//void APlayerCharacter::PlayParryAttack()
//{
//	if (GetSkillComponenet()->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
//	{
//		return;
//	}
//
//	float PlayRate = 1;
//	GetSkillComponenet()->PlayParryAttack(PlayRate);
//}


//bool APlayerCharacter::GetIsMusouAttackStarted() const
//{
//	return SkillComponenet->GetIsMusouAttackStarted();
//}

//void APlayerCharacter::SetIsMusouAttackStarted(bool NewIsMusouAttackStarted)
//{
//	SkillComponenet->SetIsMusouAttackStarted(NewIsMusouAttackStarted);
//}

const FVector2D& APlayerCharacter::GetMovementVector() const
{
	return MovementVector;
}
