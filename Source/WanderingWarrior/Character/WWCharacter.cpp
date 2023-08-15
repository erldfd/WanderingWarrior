// Fill out your copyright notice in the Description page of Project Settings.


#include "WWCharacter.h"

#include "WWAnimInstance.h"
#include "Components/CharacterStatComponent.h"
#include "Item/Weapon.h"
#include "PlayerCharacter.h"

#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"

// Sets default values
// 옆에서 초기화 할 때는 protected private 순으로 적어줘야 warning이 안뜨나보다
AWWCharacter::AWWCharacter() : InputForwardValue(0), InputRightValue(0), bWIllSweepAttack(false), AttackDamageWithoutWeapon(0.2),
								bIsAnimMoveStart(false), AttackMoveSpeed(5), AttackAnimRate(1), MaxHeightInAir(300)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));
	check(CharacterStatComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;//이동 방향으로 캐릭터가 로테이션함
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);//로테이션 속도인듯

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	USkeletalMeshComponent& CharacterMeshComponent = *GetMesh();
	check(&CharacterMeshComponent);

	CharacterMeshComponent.SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard"));
	if (MESH.Succeeded())
	{
		CharacterMeshComponent.SetSkeletalMesh(MESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Game/Animations/BP_WarriorAnim.BP_WarriorAnim_C"));
	CharacterMeshComponent.SetAnimInstanceClass(Anim.Class);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterOverlapOnly"));
	GetMesh()->SetGenerateOverlapEvents(true);
	MaxHeightInAir = 100000;
}

void AWWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UWWAnimInstance>(GetMesh()->GetAnimInstance());
	if (ensure(AnimInstance) == false) return;

	AnimInstance->OnAnimMoveStartDelegate.AddUObject(this, &AWWCharacter::OnAnimMoveStart);
	AnimInstance->OnAnimMoveEndDelegate.AddUObject(this, &AWWCharacter::OnAnimMoveEnd);
	AnimInstance->OnAttackHitcheckDelegate.AddUObject(this, &AWWCharacter::AttackCheck);
	AnimInstance->SetAttackAnimRate(AttackAnimRate);

	//GetCharacterMovement()->GravityScale = 0;
}

// Called when the game starts or when spawned
void AWWCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float AWWCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	float HPBeforeDamage = CharacterStatComponent->GetHP();
	float HPAfterDamage = HPBeforeDamage - Damage;

	CharacterStatComponent->SetHP(HPAfterDamage);

	//AnimInstance->SetIsHit(true);
	if (HPAfterDamage <= 0)
	{
		AnimInstance->SetIsDead(true);
	}
	
	APlayerCharacter& Player = *Cast<APlayerCharacter>(DamageCauser);

	if (&Player && Player.GetAnimInstance().GetIsParrying())
	{
		AnimInstance->SetBeingStunned(true);

		return Damage;
	}

	if (AnimInstance->GetBeingStunned())
	{
		AnimInstance->SetBeingStunned(false);
	}
	
	bIsAnimMoveStart = false;
	FVector MoveDir = GetActorLocation() - DamageCauser->GetActorLocation();
	MoveDir.Normalize();

	AWWCharacter* DamageCauserCharacter = Cast<AWWCharacter>(DamageCauser);
	
	if (DamageCauserCharacter && DamageCauserCharacter->GetAnimInstance().GetComboCount() == 3)
	{
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->StopAllMontages(0);
		GetAnimInstance().SetHitAndFly(true);
		

		//MoveDir.Z = -10.0f;
		//StartHitFly(MoveDir, 0.5f, 200, 200, 45);

		/*UE_LOG(LogTemp, Warning, TEXT("Tan %f : %f"), 45.0f, FMath::Tan(FMath::DegreesToRadians(45.0f)));
		MoveDir.Z = 1.1;
		StartKnockback(MoveDir, 500, 0.2);*/

		/*float LaunchStrength = 300;
		MoveDir.Z = 1;

		LaunchCharacter(MoveDir * LaunchStrength, false, false);*/

		float LaunchStrength = 1000;
		float Angle = 30;
		MoveDir.Z = MoveDir.Size() * FMath::Tan(FMath::DegreesToRadians(80));
		MoveDir.Normalize();
		LaunchCharacter(MoveDir * LaunchStrength, false, false);
	}
	else
	{
		AnimInstance->PlayCharacterHitMontage();
		MoveDir.Z = 0;
		StartKnockback(MoveDir, 1000, 0.1);
		
		//LaunchCharacter(MoveDir * 1000, false, false);

		/*MoveDir.Z = 4;
		StartKnockback(MoveDir, 300, 0.2);*/
	}

	/*float LaunchStrength = 1000;
	float Angle = 30;
	MoveDir.Z = MoveDir.Size() * FMath::Tan(FMath::DegreesToRadians(80));
	MoveDir.Normalize();
	LaunchCharacter(MoveDir * LaunchStrength, false, false);*/

	UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::TakeDamage, Damage : %f, ActorHP : %f, Actor : %s"), Damage, HPAfterDamage, *GetName());
	return Damage;
}

UWWAnimInstance& AWWCharacter::GetAnimInstance()
{
	return *AnimInstance;
}

// Called every frame
void AWWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z > MaxHeightInAir)
	{
		GetCharacterMovement()->Velocity.Z = -10;
	}

	if (bIsStartedHitFly)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		ElapsedTime += 1 * DeltaTime;

		FVector FrontVelocity = (FlyingDestination - FlyingOrigin) / FlyingTime;

		//HeightVelocity.Z -= (FlyingHeight / FlyingTime + FlyingAcceleration * FlyingTime / 4) * DeltaTime;
		HeightVelocity.Z -= (4 * FlyingDistance * FMath::Tan(FlyingAngle) / FlyingTime) * DeltaTime;
		FVector Velocity = FrontVelocity + HeightVelocity;

		FHitResult Hit;
		AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);

		if (ElapsedTime >= FlyingTime)
		{
			StopHitFly();
		}

		if (Hit.IsValidBlockingHit())
		{
			AWWCharacter* Character = Cast<AWWCharacter>(Hit.GetActor());
			if (Character == nullptr)
			{
				StopHitFly();
			}
		}
	}

	if (bIsAnimMoveStart)
	{
		FHitResult Hit;
		AddActorWorldOffset(GetActorForwardVector() * AttackMoveSpeed, true, &Hit);

		if (Hit.bBlockingHit)
		{
			AttackMoveSpeed = 0;
		}
		else
		{
			AttackMoveSpeed = 5;
		}
	}

	bool bIsPlayingHitMontage = AnimInstance->GetIsPlayingCharacterHitMontage();
	bool bIsHitAndFly = AnimInstance->GetHitAndFly();
	bool bIsInAir = GetMovementComponent()->IsFalling();
	EMovementMode MovementMode = GetCharacterMovement()->MovementMode.GetValue();

	if (AnimInstance->GetIsDead() && bIsInAir == false)
	{
		SetActorEnableCollision(false);
	}

	if (bIsKnockbackStarted)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), GetActorLocation() + KnockbackDirection * KnockbackStrength, DeltaTime, 1.0f);
		FHitResult Hit;
		SetActorLocation(NewLocation, true, &Hit);
		if (KnockbackDirection.Z > 0)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}

		if (Hit.IsValidBlockingHit())
		{
			AWWCharacter* Character = Cast<AWWCharacter>(Hit.GetActor());
			if (Character == nullptr)
			{
				bIsKnockbackStarted = false;
			}
		}
	}
	//else if (MovementMode == EMovementMode::MOVE_Falling && bIsHitAndFly)
	//{
	//	FHitResult Hit;
	//	AddActorWorldOffset(-GetActorForwardVector() * AttackMoveSpeed, true, &Hit);

	//	if (Hit.bBlockingHit)
	//	{
	//		AttackMoveSpeed = 0;
	//	}
	//	else
	//	{
	//		AttackMoveSpeed = 5;
	//	}
	//}
	/*else if (AnimInstance->GetHitAndFly() && GetCharacterMovement()->MovementMode.GetValue() == EMovementMode::MOVE_Walking)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else if (AnimInstance->GetHitAndFly() == false && GetCharacterMovement()->MovementMode.GetValue() == EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}*/

	//UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Tick, %s MovemetMode : %d, Air? : %d"), *GetName(), GetCharacterMovement()->MovementMode.GetValue(), bIsInAir);
}

// Called to bind functionality to input
void AWWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	/*PlayerInputComponent->BindAxis("AttackHold", this, &AWWCharacter::Attack);*/

	/*PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AWWCharacter::InputMoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AWWCharacter::InputMoveRight);*/

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	/*PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);*/

	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &AWWCharacter::TestAction);
}

void AWWCharacter::InputMoveForward(float Value)
{
	if (AnimInstance->IsPlayingSomething())
	{
		return;
	}

	InputForwardValue = Value;
	MoveForward(Value);
}

void AWWCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWWCharacter::InputMoveRight(float Value)
{
	if (AnimInstance->IsPlayingSomething())
	{
		return;
	}

	InputRightValue = Value;

	MoveRight(Value);
}

void AWWCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AWWCharacter::AttackCheck()
{
	if (bWIllSweepAttack)
	{
		return;
	}

	float AttackRange = 200.0f;
	float AttackRadius = 50;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVector = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVector * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRotation, DrawColor, false, DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			/*if (Tags[0] == "Enemy" && HitResult.GetActor()->Tags[0] == "Enemy")
			{
				return;
			}*/

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(AttackDamageWithoutWeapon, DamageEvent, GetController(), this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bResult == false"));
	}
}

void AWWCharacter::Attack(float Value)
{
	if (Value != 1)
	{
		return;
	}

	bool bIsAttacking = AnimInstance->GetIsAttacking();
	bool bCanCombo = AnimInstance->GetCanCombo();
	bool bWillPlayNextCombo = AnimInstance->GetWillPlayNextCombo();
	bool bIsDead = AnimInstance->GetIsDead();
	bool bIsPlayingJumpToGroundSkill = AnimInstance->GetIsPlayingChargeAttack1Anim();

	if (bIsDead || bIsPlayingJumpToGroundSkill)
	{
		return;
	}

	if (bIsAttacking == false)
	{
		AnimInstance->PlayAttackMontage();
	}
	else if (bCanCombo && bWillPlayNextCombo == false)
	{
		AnimInstance->SetWillPlayNextCombo(true);
	}
}

void AWWCharacter::Attack()
{
	bool bIsAttacking = AnimInstance->GetIsAttacking();
	bool bCanCombo = AnimInstance->GetCanCombo();
	bool bWillPlayNextCombo = AnimInstance->GetWillPlayNextCombo();
	bool bIsDead = AnimInstance->GetIsDead();
	bool bIsPlayingJumpToGroundSkill = AnimInstance->GetIsPlayingChargeAttack1Anim();

	bool bIsPlayingChargeAttack2Anim = AnimInstance->GetIsPlayingChargeAttack2Anim();
	bool bWillPlayChargeAttack2Anim = AnimInstance->GetWillPlayChargeAttack2Anim();

	bool bIsPlayingChargeAttack3Anim = AnimInstance->GetIsPlayingChargeAttack3Anim();
	bool bWillPlayChargeAttack3Anim = AnimInstance->GetWillPlayChargeAttack3Anim();

	bool bIsGuarding = AnimInstance->GetIsGuarding();
	bool bIsGuardHitStart = AnimInstance->GetIsGuardHitStart();
	bool bBeingStunned = AnimInstance->GetBeingStunned();
	bool bIsActingMusou = AnimInstance->GetIsActingMusou();

	bool bIsJumping = GetMovementComponent()->IsFalling();

	if (bIsDead || bIsPlayingJumpToGroundSkill || bIsPlayingChargeAttack2Anim || bWillPlayChargeAttack2Anim ||
		bIsPlayingChargeAttack3Anim || bWillPlayChargeAttack3Anim || bIsGuarding|| bIsGuardHitStart ||
		bBeingStunned || bIsActingMusou ||bIsJumping)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Attack, %d %d %d %d %d %d %d %d %d "), bIsDead, bIsPlayingJumpToGroundSkill, bIsPlayingChargeAttack2Anim ,bWillPlayChargeAttack2Anim,
			bIsPlayingChargeAttack3Anim, bWillPlayChargeAttack3Anim ,bIsGuarding ,bIsGuardHitStart,
			bBeingStunned, bIsActingMusou);
		return;
	}

	if (bIsAttacking == false)
	{
		AnimInstance->PlayAttackMontage();
	}
	else if (bCanCombo && bWillPlayNextCombo == false)
	{
		AnimInstance->SetWillPlayNextCombo(true);
	}
}

UCharacterStatComponent& AWWCharacter::GetCharacterStatComponent()
{
	check(CharacterStatComponent);
	return *CharacterStatComponent;
}

const FName& AWWCharacter::GetCharacterName() const
{
	return CharacterName;
}

void AWWCharacter::EquipWeapon(AWeapon* Weapon)
{
	check(Weapon);

	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Weapon->SetOwner(this);
		CurrentWeapon = Weapon;
	}
}

void AWWCharacter::Launch(FVector Velocity, float MaxHeight)
{
	SetMaxHeightInAir(MaxHeight);
	LaunchCharacter(Velocity, false, false);
}

void AWWCharacter::SetMaxHeightInAir(float NewMaxHeight)
{
	MaxHeightInAir = NewMaxHeight;
}

void AWWCharacter::SetCustomTimeDilation(float NewTimeDilation)
{
	CustomTimeDilation = NewTimeDilation;
}

void AWWCharacter::StartKnockback(FVector Direction, float Strength, float Duration)
{
	bIsKnockbackStarted = true;
	KnockbackDirection = Direction;
	KnockbackStrength = Strength;

	GetWorldTimerManager().SetTimer(KnockbackTimerHandle, this, &AWWCharacter::StopKnockback, Duration, false);
}

void AWWCharacter::StopKnockback()
{
	bIsKnockbackStarted = false;
	KnockbackDirection = FVector::Zero();
	KnockbackStrength = 0;
}

void AWWCharacter::StartHitFly(FVector Direction, float NewFlyingTime, float NewFlyingHeight, float NewFlyingDistance, float NewFlyingAngle)
{
	bIsStartedHitFly = true;

	FlyingAngle = FMath::Atan(NewFlyingHeight / (NewFlyingDistance - 2 * NewFlyingDistance * NewFlyingTime));
	FlyingHeight = FMath::Tan(FMath::DegreesToRadians(NewFlyingAngle)) * (NewFlyingDistance - 2 * NewFlyingDistance * FlyingTime);
	UE_LOG(LogTemp, Warning, TEXT(" FlyingAngle : %f, FlyingHeight : %f"), FMath::RadiansToDegrees(FlyingAngle), FlyingHeight);

	ElapsedTime = 0;
	FlyingTime = NewFlyingTime;
	FlyingHeight = NewFlyingHeight;
	//FlyingAcceleration = 
	FlyingDistance = NewFlyingDistance;
	
	FlyingAngle = FMath::DegreesToRadians(NewFlyingAngle);

	FlyingOrigin = GetActorLocation();
	FlyingDestination = FlyingOrigin + Direction.Normalize() * FlyingDistance;
	//HeightVelocity = FVector(0, 0, 2 * FlyingHeight / FlyingTime + FlyingAcceleration * FlyingTime / 2);
	HeightVelocity = FVector(0, 0, FlyingDistance * FMath::Tan(FlyingAngle) / FlyingTime);
}

void AWWCharacter::StopHitFly()
{
	bIsStartedHitFly = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void AWWCharacter::OnAnimMoveStart()
{
	bIsAnimMoveStart = true;
}

void AWWCharacter::OnAnimMoveEnd()
{
	bIsAnimMoveStart = false;
}

void AWWCharacter::TestAction()
{
	UE_LOG(LogTemp, Warning, TEXT("TestAction"));
	AnimInstance->PlayCharacterHitMontage();
}