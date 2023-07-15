// Fill out your copyright notice in the Description page of Project Settings.


#include "WWAnimInstance.h"

#include "Character/EnemyCharacter.h"
#include "Character/NPCCharacter.h"
#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

UWWAnimInstance::UWWAnimInstance():CurentPawnSpeed(0)
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
}

void UWWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn& Pawn = *TryGetPawnOwner();

	if (IsValid(&Pawn) == false)
	{
		return;
	}

	AEnemyCharacter& Enemy = *Cast<AEnemyCharacter>(&Pawn);

	if (&Enemy)
	{
		HitAnimRate = 0.8f;
		return;
	}
	
	APlayerCharacter& Player = *Cast<APlayerCharacter>(&Pawn);

	if (&Player)
	{
		HitAnimRate = 1.5f;
	}
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
		}
	}
}

void UWWAnimInstance::PlayAttackMontage()
{
	if (Montage_IsPlaying(AttackMontage) == false && bIsPlayingCharacterHitMontage == false)
	{
		Montage_Play(AttackMontage, 1);
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
	bIsPlayingJumpToGroundSkillAnim = true;
}

bool UWWAnimInstance::IsPlayingSomething()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::IsPlayingSomething, %d %d %d %d %d "), bIsAttacking, bIsDead, bIsPlayingJumpToGroundSkillAnim, bIsPlayingCharacterHitMontage, bIsPlayingKickAttackAnim);
	return (bIsAttacking || bIsDead || bIsPlayingJumpToGroundSkillAnim || bIsPlayingCharacterHitMontage || bIsPlayingKickAttackAnim);
}

bool UWWAnimInstance::GetIsPlayingJumpToGroundSkillAnim()
{
	return bIsPlayingJumpToGroundSkillAnim;
}

void UWWAnimInstance::SetIsPlayingJumpToGroundSkillAnim(bool bIsPlaying)
{
	bIsPlayingJumpToGroundSkillAnim = bIsPlaying;
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

bool UWWAnimInstance::GetIsPlayingKickAttackAnim()
{
	return bIsPlayingKickAttackAnim;
}

void UWWAnimInstance::SetIsPlayingKickAttackAnim(bool NewIsPlayingKickAttackAnim)
{
	bIsPlayingKickAttackAnim = NewIsPlayingKickAttackAnim;
}

bool UWWAnimInstance::GetWillPlayingKickAttackAnim()
{
	return bWillPlayingKickAttackAnim;
}

void UWWAnimInstance::SetWillPlayingKickAttackAnim(bool NewWillPlayingKickAttackAnim)
{
	bWillPlayingKickAttackAnim = NewWillPlayingKickAttackAnim;
}

void UWWAnimInstance::PlayKickAttackMongate()
{
	Montage_Play(ChargeAttack2);
	bIsPlayingKickAttackAnim = true;
}

void UWWAnimInstance::PlayCharacterHitMontage()
{
	InitBoolCondition();
	bIsPlayingCharacterHitMontage = true;

	if (ensure(CharacterHitMongtage) == false) return;

	Montage_Play(CharacterHitMongtage, HitAnimRate);
}

bool UWWAnimInstance::GetIsPlayingCharacterHitMontage()
{
	return bIsPlayingCharacterHitMontage;
}

void UWWAnimInstance::SetIsPlayingCharacterHitMontage(bool bIsPlaying)
{
	bIsPlayingCharacterHitMontage = bIsPlaying;
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
	else if (bWillPlayingKickAttackAnim && ComboCount == 1)
	{
		PlayKickAttackMongate();
	}

	OnStartNextComboDelegate.Broadcast();
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
	OnKickEndDelegate.Broadcast();
}

void UWWAnimInstance::InitBoolCondition()
{
	bIsAttacking = false;
	bCanComboAttack = false;
	bWillPlayNextCombo = false;
	bIsPlayingJumpToGroundSkillAnim = false;
	bIsPlayingCharacterHitMontage = false;

	bWillPlayingKickAttackAnim = false;
	bIsPlayingKickAttackAnim = false;

	Montage_Stop(0, AttackMontage);
	Montage_Stop(0, ChargeAttack1);
	Montage_Stop(0, ChargeAttack2);
}