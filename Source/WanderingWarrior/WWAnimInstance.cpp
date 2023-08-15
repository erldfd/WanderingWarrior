// Fill out your copyright notice in the Description page of Project Settings.


#include "WWAnimInstance.h"

#include "Character/EnemyCharacter.h"
#include "Character/NPCCharacter.h"
#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"

UWWAnimInstance::UWWAnimInstance():CurentPawnSpeed(0), ChargeAttack3ComboCount(0)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/WarriorAttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_TPGROUND_MOTAGE(TEXT("/Game/Animations/JumpToGround_Montage.JumpToGround_Montage"));
	if (JUMP_TPGROUND_MOTAGE.Succeeded())
	{
		ChargeAttack1 = JUMP_TPGROUND_MOTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CHARACTER_HIT_MONTAGE(TEXT("/Game/Animations/CharacterHitMontage.CharacterHitMontage"));
	if (CHARACTER_HIT_MONTAGE.Succeeded())
	{
		CharacterHitMongtage = CHARACTER_HIT_MONTAGE.Object;
	}

	ChargeAttack3MaxComboCount = 4;
	AttackAnimRate = 1.0f;
}

void UWWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UWWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		CurentPawnSpeed = Pawn->GetVelocity().Size();

		ACharacter* Character = Cast<ACharacter>(Pawn);

		if (Character)
		{
			bIsInAir = Character->GetMovementComponent()->IsFalling();

			if (Cast<AEnemyCharacter>(Character) == nullptr)
			{
				return;
			}
		}
	}
}

void UWWAnimInstance::PlayAttackMontage()
{
	if (Montage_IsPlaying(AttackMontage) == false && bIsPlayingCharacterHitMontage == false)
	{
		Montage_Play(AttackMontage, AttackAnimRate);
		bIsAttacking = true;
		ComboCount = 1;
		bWillPlayNextCombo = false;
	}
}

void UWWAnimInstance::JumpToAttackMontageSection(int32 SectionIndex)
{
	if (Montage_IsPlaying(AttackMontage) == false)
	{
		return;
	}

	bIsAttacking = true;
	bWillPlayNextCombo = false;

	SectionIndex = FMath::Clamp(SectionIndex, 0, 2);
	FName SectionName = FName(*FString::Printf(TEXT("Section%d"), SectionIndex));
	
	Montage_JumpToSection(SectionName, AttackMontage);
	ComboCount++;
}

bool UWWAnimInstance::GetIsDead()
{
	return bIsDead;
}

void UWWAnimInstance::SetIsDead(bool IsDead)
{
	bIsDead = IsDead;
}

bool UWWAnimInstance::GetIsAttacking()
{
	return bIsAttacking;
}

void UWWAnimInstance::SetIsAttacking(bool NewIsAttacking)
{
	bIsAttacking = NewIsAttacking;
}

bool UWWAnimInstance::GetCanCombo()
{
	return bCanComboAttack;
}

bool UWWAnimInstance::GetWillPlayNextCombo()
{
	return bWillPlayNextCombo;
}

void UWWAnimInstance::SetWillPlayNextCombo(bool bWillPlayNextComboNow)
{
	bWillPlayNextCombo = bWillPlayNextComboNow;
}

void UWWAnimInstance::PlayJumpToGrundAnim()
{
	Montage_Play(ChargeAttack1);
	bIsPlayingChargeAttack1Anim = true;
}

bool UWWAnimInstance::IsPlayingSomething()
{
	bool IsPlayingSomething = (bIsAttacking || bIsDead || bIsPlayingChargeAttack1Anim ||
		bIsPlayingCharacterHitMontage || bIsPlayingChargeAttack2Anim || bIsGuarding ||
		bIsGuardHitStart || bIsParrying /* || bIsActingMusou */ );
	
	return IsPlayingSomething;
}

bool UWWAnimInstance::GetIsPlayingChargeAttack1Anim()
{
	return bIsPlayingChargeAttack1Anim;
}

void UWWAnimInstance::SetIsPlayingChargeAttack1Anim(bool bIsPlaying)
{
	bIsPlayingChargeAttack1Anim = bIsPlaying;
}

int32 UWWAnimInstance::GetComboCount()
{
	return ComboCount;
}

bool UWWAnimInstance::GetHitAndFly()
{
	return bIsHitAndFly;
}

void UWWAnimInstance::SetHitAndFly(bool NewHitAndFly)
{
	bIsHitAndFly = NewHitAndFly;
}

bool UWWAnimInstance::GetIsPlayingChargeAttack2Anim()
{
	return bIsPlayingChargeAttack2Anim;
}

void UWWAnimInstance::SetIsPlayingChargeAttack2Anim(bool NewIsPlayingChargeAttack2Anim)
{
	bIsPlayingChargeAttack2Anim = NewIsPlayingChargeAttack2Anim;
}

bool UWWAnimInstance::GetWillPlayChargeAttack2Anim()
{
	return bWillPlayChargeAttack2Anim;
}

void UWWAnimInstance::SetWillPlayChargeAttack2Anim(bool NewWillPlayChargeAttack2Anim)
{
	bWillPlayChargeAttack2Anim = NewWillPlayChargeAttack2Anim;
}

void UWWAnimInstance::PlayKickAttackMongate()
{
	Montage_Play(ChargeAttack2);
	bIsPlayingChargeAttack2Anim = true;
}

bool UWWAnimInstance::GetIsPlayingChargeAttack3Anim()
{
	return bIsPlayingChargeAttack3Anim;
}

void UWWAnimInstance::SetIsPlayingChargeAttack3Anim(bool NewIsPlayingChargeAttack3Anim)
{
	bIsPlayingChargeAttack3Anim = NewIsPlayingChargeAttack3Anim;
}

bool UWWAnimInstance::GetWillPlayChargeAttack3Anim()
{
	return bWillPlayChargeAttack3Anim;
}

void UWWAnimInstance::SetWillPlayChargeAttack3Anim(bool NewWillPlayChargeAttack3Anim)
{
	bWillPlayChargeAttack3Anim = NewWillPlayChargeAttack3Anim;
}

void UWWAnimInstance::PlayChargeAttack3Montage()
{
	if (ChargeAttack3ComboCount == ChargeAttack3MaxComboCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayChargeAttack3Montage, %d"), ChargeAttack3ComboCount);
		ChargeAttack3ComboCount = 0;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayChargeAttack3Montage, In"));
	if (ChargeAttack3ComboCount > 0)
	{
		Montage_Play(ChargeAttack3, 1, EMontagePlayReturnType::MontageLength, 0.3f);
	}
	else
	{
		Montage_Play(ChargeAttack3);
	}
	
	bIsPlayingChargeAttack3Anim = true;

	ChargeAttack3ComboCount++;
}

void UWWAnimInstance::SetAttackAnimRate(float NewAttackAnimRate)
{
	AttackAnimRate = NewAttackAnimRate;
}

bool UWWAnimInstance::GetDetectedAttack()
{
	return bIsDetectedAttack;
}

void UWWAnimInstance::SetDetectedAttack(bool NewDetectedAttack)
{
	bIsDetectedAttack = NewDetectedAttack;
}

void UWWAnimInstance::PlayMusouAnim()
{
	Montage_Play(MusouAttack);
}

bool UWWAnimInstance::GetIsIdleOrRun()
{
	return bIsIdleOrRun;
}

void UWWAnimInstance::SetIsIdleOrRun(bool NewIsIdleOrWalk)
{
	bIsIdleOrRun = NewIsIdleOrWalk;
}

bool UWWAnimInstance::GetIsGuarding()
{
	return bIsGuarding;
}

void UWWAnimInstance::SetIsGuarding(bool NewIsGuarding)
{
	bIsGuarding = NewIsGuarding;
}

bool UWWAnimInstance::GetIsGuardHitStart()
{
	return bIsGuardHitStart;
}

void UWWAnimInstance::SetIsGuardHitStart(bool NewIsGuardHitStart)
{
	bIsGuardHitStart = NewIsGuardHitStart;
}

void UWWAnimInstance::PlayGuardHitAnim()
{
	Montage_Play(GuardHitReaction);
}

bool UWWAnimInstance::GetBeingStunned()
{
	return bBeingStunned;
}

void UWWAnimInstance::SetBeingStunned(bool NewBeingStunned)
{
	bBeingStunned = NewBeingStunned;
}

void UWWAnimInstance::PlayParryAttackAnim()
{
	Montage_Play(ParryAttack);
}

bool UWWAnimInstance::GetIsParrying()
{
	return bIsParrying;
}

void UWWAnimInstance::SetIsParrying(bool NewIsParrying)
{
	bIsParrying = NewIsParrying;
}

bool UWWAnimInstance::GetIsActingMusou()
{
	return bIsActingMusou;
}

void UWWAnimInstance::SetIsActingMusou(bool NewIsActingMusou)
{
	bIsActingMusou = NewIsActingMusou;
}

bool UWWAnimInstance::GetIsActionCameraMoving()
{
	return bIsActionCameraMoving;
}

void UWWAnimInstance::SetIsActionCameraMoving(bool NewIsActionCameraMoving)
{
	bIsActionCameraMoving = NewIsActionCameraMoving;
}

void UWWAnimInstance::PlayCharacterHitMontage()
{
	if (bIsPlayingChargeAttack1Anim || bIsPlayingChargeAttack2Anim || bIsPlayingChargeAttack3Anim || bIsInAir)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayCharacterHitMontage, %d, %d, %d, %d"), bIsPlayingChargeAttack1Anim, bIsPlayingChargeAttack2Anim, bIsPlayingChargeAttack3Anim, bIsInAir);
		return;
	}

	StopAllMontages(0);

	InitBoolCondition();
	bIsPlayingCharacterHitMontage = true;

	if (ensure(CharacterHitMongtage) == false) return;

	Montage_Play(CharacterHitMongtage, 1);
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayCharacterHitMontage, HitMontage"));
	OnHitByEnemyDelegate.Broadcast();
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::StopCharacterHitMontage()
{
	Montage_Stop(0, CharacterHitMongtage);
}

bool UWWAnimInstance::GetIsPlayingCharacterHitMontage()
{
	return bIsPlayingCharacterHitMontage;
}

void UWWAnimInstance::SetIsPlayingCharacterHitMontage(bool bIsPlaying)
{
	bIsPlayingCharacterHitMontage = bIsPlaying;
}

void UWWAnimInstance::AnimNotify_AttackDetectStart()
{
	bIsDetectedAttack = true;
}

void UWWAnimInstance::AnimNotify_AttackDetectEnd()
{
	bIsDetectedAttack = false;
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::AnimNotify_CanComboNotify()
{
	bCanComboAttack = true;
	OnCanInputNextComboDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_StartNextComboNotify()
{
	bCanComboAttack = false;

	if (bWillPlayNextCombo)
	{
		JumpToAttackMontageSection(ComboCount);
	}
	else if (bWillPlayChargeAttack2Anim && ComboCount == 1)
	{
		PlayKickAttackMongate();
	}
	else if (bWillPlayChargeAttack3Anim && ComboCount == 2)
	{
		PlayChargeAttack3Montage();
	}
	else
	{
		return;
	}

	OnStartNextComboDelegate.Broadcast();
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::AnimNotify_AttackEndNotify()
{
	bCanComboAttack = false;
	bIsAttacking = false;
	bWillPlayNextCombo = false;
	ComboCount = 0;

	OnAttackEndDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_AnimMoveStartNotify()
{
	OnAnimMoveStartDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_AnimMoveEndNotify()
{
	OnAnimMoveEndDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_AttackHitCheckNotify()
{
	OnAttackHitcheckDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_JumpToGroundEndNotify()
{
	OnJumpToGroundAnimEndDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_CharacterHitAnimEndNotify()
{
	bIsPlayingCharacterHitMontage = false;
}

void UWWAnimInstance::AnimNotify_KickDamage()
{
	OnKickDamageDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_KickEnd()
{
	bIsAttacking = false;

	SetIsPlayingChargeAttack2Anim(false);
	InitBoolCondition();
	OnKickEndDelegate.Broadcast();
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::AnimNotify_Melee360AttackDamage()
{
	OnChargeAttack3DamageDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_Melee360AttackComboStart()
{
	OnChargeAttack3ComboStartDelegate.Broadcast();

	if (ChargeAttack3ComboCount == ChargeAttack3MaxComboCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_Melee360AttackComboStart, %d"), ChargeAttack3ComboCount);
		ChargeAttack3ComboCount = 0;
		return;
	}

	PlayChargeAttack3Montage();
}

void UWWAnimInstance::AnimNotify_Melee360AttackEnd()
{
	bIsAttacking = false;
	SetIsPlayingChargeAttack3Anim(false);
	InitBoolCondition();
	OnChargeAttack3EndDelegate.Broadcast();
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::AnimNotify_FallingStart()
{
	if (bIsDead)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_FallingStart, IsInAir : %d"), bIsInAir);
	StopAllMontages(0);
	InitBoolCondition();
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_FallingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_FallingEnd"));

	if (bIsDead)
	{
		return;
	}
	
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_StandUpStart()
{
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_StandUpEnd()
{
	SetHitAndFly(false);
}

void UWWAnimInstance::AnimNotify_HitStart()
{

}

void UWWAnimInstance::AnimNotify_HitEnd()
{

}

void UWWAnimInstance::AnimNotify_HitAndFlyStart()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_HitAndFlyStart, IsInAir %d"), bIsInAir);
	Montage_Stop(0, CharacterHitMongtage);

	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_RunRightFoot()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter.GetIsWet())
	{
		FVector DecalLocation = PlayerCharacter.GetActorLocation();
		DecalLocation.Z -= 90.0;

		DecalLocation += PlayerCharacter.GetActorRightVector() * 20;
		
		ADecalActor* FootprintDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, PlayerCharacter.GetActorRotation());
		if (FootprintDecal == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_RunRightFoot, No decal spawned"));
			return;
		}

		FootprintDecal->SetDecalMaterial(RightFootprintMaterialInstance);
		FootprintDecal->SetLifeSpan(2.0f);
		FootprintDecal->GetDecal()->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	}

	if (PlayerCharacter.GetIsInWater())
	{
		FVector ParticleLocation = PlayerCharacter.GetActorLocation();
		ParticleLocation.Z -= 70.0;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_WaterSplash, ParticleLocation, PlayerCharacter.GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
	}
}

void UWWAnimInstance::AnimNotify_RunLeftFoot()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter.GetIsWet())
	{
		FVector DecalLocation = PlayerCharacter.GetActorLocation();
		DecalLocation.Z -= 90.0;

		DecalLocation -= PlayerCharacter.GetActorRightVector() * 20;

		ADecalActor* FootprintDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, PlayerCharacter.GetActorRotation());
		if (FootprintDecal == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_RunLeftFoot, No decal spawned"));
			return;
		}

		FootprintDecal->SetDecalMaterial(LeftFootprintMaterialInstance);
		FootprintDecal->SetLifeSpan(2.0f);
		FootprintDecal->GetDecal()->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	}

	if (PlayerCharacter.GetIsInWater())
	{
		FVector ParticleLocation = PlayerCharacter.GetActorLocation();
		ParticleLocation.Z -= 70.0;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_WaterSplash, ParticleLocation, PlayerCharacter.GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
	}
}

void UWWAnimInstance::AnimNotify_MusouReadyStart()
{
	SetIsActingMusou(true);
	SetIsActionCameraMoving(true);

	APlayerCharacter& Player = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&Player == nullptr)
	{
		return;
	}

	Player.CustomTimeDilation = 10;
	UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);

	UCameraComponent& Camera = Player.GetCamera();
	UCameraComponent& ActionCamera = Player.GetActionCamera();

	if (&Camera == nullptr || &ActionCamera == nullptr)
	{
		return;
	}

	if ((Camera.IsActive()))
	{
		Camera.Deactivate();
		ActionCamera.Activate();
	}
	else
	{
		ActionCamera.Deactivate();
		Camera.Activate();
	}
}

void UWWAnimInstance::AnimNotify_ActionCameraMoveEnd()
{
	SetIsActionCameraMoving(false);

	APlayerCharacter& Player = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&Player == nullptr)
	{
		return;
	}

	UCameraComponent& Camera = Player.GetCamera();
	UCameraComponent& ActionCamera = Player.GetActionCamera();

	if (&Camera == nullptr || &ActionCamera == nullptr)
	{
		return;
	}

	if ((Camera.IsActive()))
	{
		Camera.Deactivate();
		ActionCamera.Activate();
	}
	else
	{
		ActionCamera.Deactivate();
		Camera.Activate();
	}
}

void UWWAnimInstance::AnimNotify_MusouReadyEnd()
{
	APlayerCharacter& Player = *Cast<APlayerCharacter>(TryGetPawnOwner());
	if (&Player)
	{
		Player.CustomTimeDilation = 1;
		UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
	}

	Montage_JumpToSection(TEXT("Musou"), MusouAttack);
	Montage_SetNextSection(TEXT("Musou"), TEXT("Musou"), MusouAttack);

	FAnimMontageInstance* MontageInstance = GetActiveMontageInstance();
	MontageInstance->PushDisableRootMotion();
	/*MusouAttack->EnableRootMotionSettingFromMontage(false, ERootMotionRootLock::RefPose);
	MusouAttack->bEnableRootMotionTranslation = false;
	MusouAttack->bEnableRootMotionRotation = false;*/
}

void UWWAnimInstance::AnimNotify_MusouAttackCheck()
{
	APlayerCharacter& Player = *Cast<APlayerCharacter>(GetOwningActor());
	if (&Player == nullptr)
	{
		return;
	}

	Player.SetConsistentMusou(false);

	OnMusouAttackCheckDelegate.Broadcast();
	//MusouAttack->EnableRootMotionSettingFromMontage(true, ERootMotionRootLock::RefPose);
	//MusouAttack->bEnableRootMotionTranslation = true;
	//MusouAttack->bEnableRootMotionRotation = true;
}

void UWWAnimInstance::AnimNotify_MusouAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_MusouAttackEnd"));

	APlayerCharacter& Player = *Cast<APlayerCharacter>(GetOwningActor());
	if (&Player == nullptr)
	{
		return;
	}

	if (Player.GetConsistentMusou() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_MusouAttackEnd, Consistent == false"));
		Montage_JumpToSection(TEXT("MusouFinish"), MusouAttack);

		Player.LaunchCharacter(FVector(0, 0, 500), false, false);

		FAnimMontageInstance* MontageInstance = GetActiveMontageInstance();
		MontageInstance->PopDisableRootMotion();
	}
}

void UWWAnimInstance::AnimNotify_MusouFinishDown()
{
	APlayerCharacter& Player = *Cast<APlayerCharacter>(GetOwningActor());
	if (&Player == nullptr)
	{
		return;
	}

	Player.LaunchCharacter(FVector(0, 0, -500), false, false);
}

void UWWAnimInstance::AnimNotify_MusouFinishAttackCheck()
{
	OnMusouFinishAttackCheckDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_MusouFinishAttackEnd()
{
	APlayerCharacter& Player = *Cast<APlayerCharacter>(GetOwningActor());
	if (&Player == nullptr)
	{
		return;
	}

	Player.SetPlayingMusou(false);
	SetIsActingMusou(false);
}

void UWWAnimInstance::AnimNotify_WarriorIdleStart()
{
	SetIsIdleOrRun(true);
}

void UWWAnimInstance::AnimNotify_WarriorRunStart()
{
	SetIsIdleOrRun(true);
}

void UWWAnimInstance::AnimNotify_GuardHit()
{
}

void UWWAnimInstance::AnimNotify_GuardHitStart()
{
	SetIsGuardHitStart(true);
}

void UWWAnimInstance::AnimNotify_GuardHitEnd()
{
	SetIsGuardHitStart(false);
}

void UWWAnimInstance::AnimNotify_ParryAttackCheck()
{
	OnParryAttackCheckDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_ParryAttackStart()
{
	SetIsParrying(true);

	float GamePlayRate = 0.5f;
	UGameplayStatics::SetGlobalTimeDilation(this, GamePlayRate);

	OnParryAttackStartDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_ParryAttackEnd()
{
	SetIsParrying(false);
	SetIsGuardHitStart(false);
}

void UWWAnimInstance::InitBoolCondition()
{
	bIsAttacking = false;
	bIsDetectedAttack = false;
	bCanComboAttack = false;
	bWillPlayNextCombo = false;
	bIsPlayingChargeAttack1Anim = false;
	bIsPlayingCharacterHitMontage = false;

	bWillPlayChargeAttack2Anim = false;
	bIsPlayingChargeAttack2Anim = false;

	bWillPlayChargeAttack3Anim = false;
	bIsPlayingChargeAttack3Anim = false;
	ChargeAttack3ComboCount = 0;
	ComboCount = 0;

	Montage_Stop(0, AttackMontage);
	Montage_Stop(0, ChargeAttack1);
	Montage_Stop(0, ChargeAttack2);
	Montage_Stop(0, ChargeAttack3);
}