// Fill out your copyright notice in the Description page of Project Settings.


#include "WWCharacter.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/CharacterStatComponent.h"
#include "Components/SkillComponentBase.h"
#include "Components/WarriorSkillComponent.h"
#include "Item/Weapon.h"
#include "PlayerCharacter.h"

#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"

// 옆에서 초기화 할 때는 protected private 순으로 적어줘야 warning이 안뜨나보다
AWWCharacter::AWWCharacter() : InputForwardValue(0), InputRightValue(0), bWIllSweepAttack(false), AttackDamageWithoutWeapon(0.2),
								bIsAnimMoveStart(false), AttackMoveSpeed(5), AttackAnimRate(1), MaxHeightInAir(300)
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterStatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));
	check(CharacterStatComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;//이동 방향으로 캐릭터가 로테이션함
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);//로테이션 속도인듯

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	USkeletalMeshComponent* CharacterMeshComponent = GetMesh();
	check(CharacterMeshComponent);

	CharacterMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterOverlapOnly"));
	GetMesh()->SetGenerateOverlapEvents(true);
	MaxHeightInAir = 100000;
}

void AWWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance = Cast<UWWAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::PostInitializeComponents, AnimInstance == false"));
		return;
	}

	//AnimInstance->OnAnimMoveStartDelegate.AddUObject(this, &AWWCharacter::OnAnimMoveStart);
	//AnimInstance->OnAnimMoveEndDelegate.AddUObject(this, &AWWCharacter::OnAnimMoveEnd);
	//AnimInstance->OnAttackHitcheckDelegate.AddUObject(this, &AWWCharacter::AttackCheck);
	//AnimInstance->SetAttackAnimRate(AttackAnimRate);
	
	//GetCharacterMovement()->GravityScale = 0;
}

void AWWCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float AWWCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (GetIsInvincible())
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::TakeDamage, %s Is Invincible"), *GetName());
		return 0.0f;
	}

	float HPBeforeDamage = CharacterStatComponent->GetHP();
	float HPAfterDamage = HPBeforeDamage - Damage;

	CharacterStatComponent->SetHP(HPAfterDamage);

	if (HPAfterDamage <= 0)
	{
		AnimInstance->SetIsDead(true);
	}
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(DamageCauser);

	if (Player && Player->GetAnimInstance()->GetIsParrying())
	{
		AnimInstance->SetBeingStunned(true);

		return Damage;
	}

	if (AnimInstance->GetBeingStunned())
	{
		AnimInstance->SetBeingStunned(false);
	}
	
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp && SkillComp->IsSkillStarted())
	{
		return Damage;
	}

	//AWWCharacter* DamageCauserCharacter = Cast<AWWCharacter>(DamageCauser);
	//
	//if (DamageCauserCharacter && DamageCauserCharacter->GetComboCount() == 3)
	//{
	//	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//	AnimInstance->StopAllMontages(0);
	//	GetAnimInstance().SetHitAndFly(true);
	//	

	//	//MoveDir.Z = -10.0f;
	//	//StartHitFly(MoveDir, 0.5f, 200, 200, 45);

	//	/*UE_LOG(LogTemp, Warning, TEXT("Tan %f : %f"), 45.0f, FMath::Tan(FMath::DegreesToRadians(45.0f)));
	//	MoveDir.Z = 1.1;
	//	StartKnockback(MoveDir, 500, 0.2);*/

	//	/*float LaunchStrength = 300;
	//	MoveDir.Z = 1;

	//	LaunchCharacter(MoveDir * LaunchStrength, false, false);*/

	//	float LaunchStrength = 1000;
	//	float Angle = 30;
	//	MoveDir.Z = MoveDir.Size() * FMath::Tan(FMath::DegreesToRadians(80));
	//	MoveDir.Normalize();
	//	LaunchCharacter(MoveDir * LaunchStrength, false, false);
	//}
	//else
	//{
	//	MoveDir.Z = 0;
	//	StartKnockback(MoveDir, 1000, 0.1);
	//	
	//	//LaunchCharacter(MoveDir * 1000, false, false);

	//	/*MoveDir.Z = 4;
	//	StartKnockback(MoveDir, 300, 0.2);*/
	//}

	UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::TakeDamage, Damage : %f, ActorHP : %f, Actor : %s"), Damage, HPAfterDamage, *GetName());
	return Damage;
}

float AWWCharacter::TakeDamageWithLaunch(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FVector LaunchVelocity, bool bIsInitializedVelocity, float MaxHeight)
{
	float Damage = TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsInitializedVelocity)
	{
		GetCharacterMovement()->Velocity = FVector(0, 0, 0);
	}

	Launch(LaunchVelocity, MaxHeight);

	return Damage;
}

float AWWCharacter::TakeDamageWithKnockback(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FVector NewKnockbackVelocity, float KnockbackDuration, bool bWillPlayHitReaction)
{
	float Damage = TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bool bIsPlayingSkill = GetSkillComponent()->GetIsChargeAttack1Started() || GetSkillComponent()->GetIsChargeAttack2Started() || GetSkillComponent()->GetIsChargeAttack3Started() || GetSkillComponent()->GetIsMusouAttackStarted() || GetSkillComponent()->GetIsParrying();
	if (bWillPlayHitReaction && Damage > 0.0f && bIsPlayingSkill == false)
	{
		AnimInstance->PlayCharacterHitMontage();
	}

	StartKnockback(NewKnockbackVelocity, KnockbackDuration);

	return Damage;
}

void AWWCharacter::Jump()
{
	if (GetSkillComponent()->IsSkillStarted() || GetIsAttacking())
	{
		return;
	}

	MaxHeightInAir = 100000;

	Super::Jump();
}

UWWAnimInstance* AWWCharacter::GetAnimInstance() const
{
	return AnimInstance;
}

// Called every frame
void AWWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z > MaxHeightInAir)
	{
		GetCharacterMovement()->Velocity.Z = -10;
	}

	//if (bIsAnimMoveStart)
	//{
	//	FHitResult Hit;
	//	AddActorWorldOffset(GetActorForwardVector() * AttackMoveSpeed, true, &Hit);

	//	if (Hit.bBlockingHit)
	//	{
	//		AttackMoveSpeed = 0;
	//	}
	//	else
	//	{
	//		AttackMoveSpeed = 5;
	//	}
	//}

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
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), GetActorLocation() + KnockbackVelocity, DeltaTime, 1.0f);
		FHitResult Hit;
		SetActorLocation(NewLocation, true, &Hit);
		if (KnockbackVelocity.Z > 0)
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

	if (AnimInstance->GetIsGuarding() == false)
	{
		GuardStartCheckCount = 0;
	}
}

void AWWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &AWWCharacter::TestAction);
}

void AWWCharacter::Attack()
{
	bool bIsDead = AnimInstance->GetIsDead();
	bool bIsSkillStarted = (GetSkillComponent()) ? GetSkillComponent()->IsSkillStarted() : false;
	bool bIsGuardHitStart = AnimInstance->GetIsGuardHitStart();
	bool bBeingStunned = AnimInstance->GetBeingStunned();
	bool bIsJumping = (GetMovementComponent()->IsFalling() || bPressedJump);
	bool bIsPlayingCharacterHit = AnimInstance->GetIsPlayingCharacterHitMontage();
	bool bIsGuarding = AnimInstance->GetIsGuarding();

	if (bIsDead  || bIsGuarding || bIsGuardHitStart || bBeingStunned || bIsSkillStarted || bIsJumping || bIsPlayingCharacterHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Attack, %d %d %d %d %d"), bIsDead, bIsSkillStarted,
			 bIsGuarding ,bIsGuardHitStart, bBeingStunned);
		return;
	}

	if (GetIsAttacking() == false)
	{
		SetIsAttacking(true);

		if (AttackMontage == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Attack, AttackMontage == nullptr"));
		}
		PlayAnimMontage(AttackMontage, AttackAnimRate);
		//AnimInstance->Montage_Play(AttackMontage, AttackAnimRate);
		SetComboCount(1);
	}
	else if (GetCanCombo() && GetWillPlayNextCombo() == false)
	{
		SetWillPlayNextCombo(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Attack, GetIsAttacking : %d, GetCanCombo : %d, GetWillPlayNextCombo : %d"), GetIsAttacking(), GetCanCombo(), GetWillPlayNextCombo());
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
		AWWCharacter* HitCharacter = Cast<AWWCharacter>(HitResult.GetActor());

		if (HitCharacter->IsValidLowLevel())
		{
			FVector MoveDir = HitCharacter->GetActorLocation() - GetActorLocation();
			MoveDir.Normalize();

			FDamageEvent DamageEvent;
			HitCharacter->TakeDamageWithKnockback(AttackDamageWithoutWeapon, DamageEvent, GetController(), this, MoveDir * 1000, 0.1f, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::AttackCheck, Not Valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::AttackCheck, bResult == false"));
	}
}

UCharacterStatComponent* AWWCharacter::GetCharacterStatComponent()
{
	if (CharacterStatComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::GetCharacterStatComponent, CharacterStatComponent == nullptr"))
	}

	return CharacterStatComponent;
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
	AnimInstance->SetHitAndFly(true);
	AnimInstance->StopAllMontages(0.1f);
}

void AWWCharacter::SetMaxHeightInAir(float NewMaxHeight)
{
	MaxHeightInAir = NewMaxHeight;
}

bool AWWCharacter::GetIsAttacking() const
{
	return bIsAttacking;
}

void AWWCharacter::SetIsAttacking(bool bNewIsAttacking)
{
	bIsAttacking = bNewIsAttacking;
}

bool AWWCharacter::GetCanCombo() const
{
	return bCanCombo;
}

void AWWCharacter::SetCanCombo(bool bNewCanCombo)
{
	bCanCombo = bNewCanCombo;
}

bool AWWCharacter::GetWillPlayNextCombo() const
{
	return bWillPlayNextCombo;
}

void AWWCharacter::SetWillPlayNextCombo(bool bNewWillPlayNextCombo)
{
	bWillPlayNextCombo = bNewWillPlayNextCombo;
}

int32 AWWCharacter::GetComboCount() const
{
	return ComboCount;
}

void AWWCharacter::SetComboCount(int32 NewComboCount)
{
	ComboCount = NewComboCount;
}

UAnimMontage* AWWCharacter::GetAttackMontage() const
{
	return AttackMontage;
}

AWeapon* AWWCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool AWWCharacter::GetIsInvincible()
{
	return bIsInvincible;
}

void AWWCharacter::SetIsInvincible(bool bNewIsInvincible)
{
	bIsInvincible = bNewIsInvincible;
}

void AWWCharacter::StartKnockback(FVector Velocity, float Duration)
{
	bIsKnockbackStarted = true;
	KnockbackVelocity = Velocity;

	GetWorldTimerManager().SetTimer(KnockbackTimerHandle, this, &AWWCharacter::StopKnockback, Duration, false);
}

void AWWCharacter::StopKnockback()
{
	bIsKnockbackStarted = false;
	KnockbackVelocity = FVector::Zero();
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
	//AnimInstance->StopAllMontages(0);
	AnimInstance->PlayCharacterHitMontage();
	//PlayAnimMontage(AttackMontage);
}

void AWWCharacter::DoChargeAttack()
{
	bool bIsDead = AnimInstance->GetIsDead();
	bool bIsJumping = (GetMovementComponent()->IsFalling() || bPressedJump);
	bool bIsSkillStarted = (GetSkillComponent()) ? GetSkillComponent()->IsSkillStarted() : false;
	bool bIsPlayingCharacterHit = AnimInstance->GetIsPlayingCharacterHitMontage();

	if (bIsDead || GetWillPlayNextCombo() || GetIsMusouAttackStarted() || bIsJumping || bIsSkillStarted || bIsPlayingCharacterHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::DoChargeAttack, IsDead : %d, WillPlayNextCombo : %d, IsJumping : %d,  IsSkillStarted : %d"),
			bIsDead, GetWillPlayNextCombo(), bIsJumping, bIsSkillStarted);
		return;
	}

	
	if (bIsSkillStarted == false && GetComboCount() == 0)
	{
		PlayChargeAttack1();
	}
	else if (GetWillPlayChargeAttack2() == false && GetComboCount() == 1)
	{
		SetWillPlayChargeAttack2(true);
	}
	else if (GetWillPlayChargeAttack3() == false && GetComboCount() == 2)
	{
		SetWillPlayChargeAttack3(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::DoChargeAttack, IsSkillStarted : %d, ComboCount : %d, WillPlayChargeAttack2 : %d, WillPlayChargeAttack3 : %d"),
			bIsSkillStarted, GetComboCount(), GetWillPlayChargeAttack2(), GetWillPlayChargeAttack3());
	}
}

void AWWCharacter::DoMusouAttack()
{
	float CurrentMP = CharacterStatComponent->GetMP();
	float MaxMP = CharacterStatComponent->GetMaxMP();
	bool bIsPlayingMusou = GetIsMusouAttackStarted();

	if (bIsPlayingMusou == false && CurrentMP < MaxMP || GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	if (bIsPlayingMusou == false && CurrentMP == MaxMP)
	{
		PlayMusouAttack();
		SetIsMusouAttackStarted(true);

		SetIsParrySucceeded(false);

		//USkillComponentBase* SkillComp = Cast<USkillComponentBase>(GetSkillComponent());
		SkillComponent->SetIsChargeAttack1Started(false);
		SkillComponent->SetIsChargeAttack2Started(false);
		SkillComponent->SetIsChargeAttack3Started(false);

		SetIsAttacking(false);
		AnimInstance->SetIsGuarding(false);
		AnimInstance->SetIsGuardHitStart(false);
		AnimInstance->SetIsPlayingCharacterHitMontage(false);
		SetWillPlayNextCombo(false);
		SetWillPlayChargeAttack2(false);
		SetWillPlayChargeAttack3(false);
	}
	else if (bIsPlayingMusou && CurrentMP < MaxMP && CurrentMP > 0)
	{
		bIsConsistentMusou = true;
	}

	/*FMotionWarpingTarget Target;
	float Distance = 500.0f;
	Target.Name = FName("AttackTarget");
	Target.Location = GetActorLocation() + GetActorForwardVector() * Distance;
	Target.Rotation = GetActorRotation();

	MotionWarpComponent->AddOrUpdateWarpTarget(Target);*/
}

void AWWCharacter::DoGuard(const struct FInputActionValue& Value)
{
	GuardStartCheckCount++;

	if (AnimInstance->GetIsPlayingCharacterHitMontage() || bPressedJump || GetMovementComponent()->IsFalling())
	{
		return;
	}

	if (GetIsParrySucceeded())
	{
		AnimInstance->SetIsGuarding(false);
		return;
	}

	bool bIsGuarding = Value.Get<bool>();

	if (GuardStartCheckCount == 1)
	{
		ParryLeftTime = ParryWindow;
	}

	AnimInstance->SetIsGuarding(bIsGuarding);
}

void AWWCharacter::PlayChargeAttack1()
{
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp == nullptr)
	{
		return;
	}

	if (SkillComp->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	float PlayRate = 1.0f;
	SkillComp->PlayChargeAttack1(PlayRate);
}

void AWWCharacter::PlayChargeAttack2()
{
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp == nullptr)
	{
		return;
	}

	if (SkillComp->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	float PlayRate = 1.0f;
	SkillComp->PlayChargeAttack2(PlayRate);
}

void AWWCharacter::PlayChargeAttack3()
{
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp == nullptr)
	{
		return;
	}

	if (SkillComp->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	float PlayRate = 1.0f;
	SkillComp->PlayChargeAttack3(PlayRate);
}

void AWWCharacter::PlayMusouAttack() 
{
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp == nullptr)
	{
		return;
	}

	if (GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	float PlayRate = 1;
	SkillComp->PlayMusouAttack(PlayRate);
}

void AWWCharacter::PlayParryAttack()
{
	USkillComponentBase* SkillComp = GetSkillComponent();
	if (SkillComp == nullptr)
	{
		return;
	}

	if (SkillComp->IsSkillStarted() || GetMovementComponent()->IsFalling() || bPressedJump)
	{
		return;
	}

	float PlayRate = 1;
	SkillComp->PlayParryAttack(PlayRate);
}

bool AWWCharacter::GetIsConsistentMusou() const
{
	return bIsConsistentMusou;
}

void AWWCharacter::SetIsConsistentMusou(bool bNewIsConsistentMusou)
{
	bIsConsistentMusou = bNewIsConsistentMusou;
}

bool AWWCharacter::GetIsParrySucceeded() const
{
	return bIsParrySucceeded;
}

void AWWCharacter::SetIsParrySucceeded(bool bNewIsParrySucceeded)
{
	bIsParrySucceeded = bNewIsParrySucceeded;
}

bool AWWCharacter::GetWillPlayChargeAttack2() const
{
	return bWillPlayChargeAttack2;
}

void AWWCharacter::SetWillPlayChargeAttack2(bool bNewWillPlayChargeAttack2)
{
	bWillPlayChargeAttack2 = bNewWillPlayChargeAttack2;
}

bool AWWCharacter::GetWillPlayChargeAttack3() const
{
	return bWillPlayChargeAttack3;
}

void AWWCharacter::SetWillPlayChargeAttack3(bool bNewWillPlayChargeAttack3)
{
	bWillPlayChargeAttack3 = bNewWillPlayChargeAttack3;
}

//bool AWWCharacter::GetIsGuarding() const
//{
//	return bIsGuarding;
//}
//
//void AWWCharacter::SetIsGuarding(bool bNewIsGuarding)
//{
//	bIsGuarding = bNewIsGuarding;
//}

bool AWWCharacter::GetIsMusouAttackStarted() const
{
	if (GetSkillComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::GetIsMusouAttackStarted, Faild to Get SkillComp"));
		return false;
	}

	bool bIsMusouAttackStarted = GetSkillComponent()->GetIsMusouAttackStarted();

	return bIsMusouAttackStarted;
}

void AWWCharacter::SetIsMusouAttackStarted(bool NewIsMusouAttackStarted)
{
	if (GetSkillComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::SetIsMusouAttackStarted, Faild to Get SkillComp"));
		return;
	}

	GetSkillComponent()->SetIsMusouAttackStarted(NewIsMusouAttackStarted);
}

void AWWCharacter::Move(const FInputActionValue& Value)
{
	if (AnimInstance->IsPlayingSomething() || AnimInstance->GetIsActionCameraMoving() || GetIsAttacking() || AnimInstance->GetHitAndFly())
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Move, IsPlayingSomething : %d, IsActingCameraMoving : %d, IsAttacking : %d, HitAndFly : %d"),
			AnimInstance->IsPlayingSomething(), AnimInstance->GetIsActionCameraMoving(), GetIsAttacking(), AnimInstance->GetHitAndFly());
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetSkillComponent()->IsSkillStarted())
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::Move,IsSkillStarted : %d"), GetSkillComponent()->IsSkillStarted());
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}

USkillComponentBase* AWWCharacter::GetSkillComponent() const
{
	if (SkillComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWCharacter::GetSkillComponent, Fail to Get SkillComp"));
		return nullptr;
	}

	return SkillComponent;
}

ESkillCompType AWWCharacter::GetSkillCompType() const
{
	return SkillCompType;
}