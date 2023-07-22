// Fill out your copyright notice in the Description page of Project Settings.


#include "WWAnimInstance.h"

#include "Character/EnemyCharacter.h"
#include "Character/NPCCharacter.h"
#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

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
	HitAnimRate = 1.0f;
}

void UWWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	/*APawn& Pawn = *TryGetPawnOwner();

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
	}*/
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
	bIsPlayingChargeAttack1Anim = true;
}

bool UWWAnimInstance::IsPlayingSomething()
{
	bool IsPlayingSomething = (bIsAttacking || bIsDead ||
		bIsPlayingChargeAttack1Anim || bIsPlayingCharacterHitMontage || bIsPlayingChargeAttack2Anim);
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

bool UWWAnimInstance::GetIsHit()
{
	return bIsHit;
}

void UWWAnimInstance::SetIsHit(bool NewIsHit)
{
	bIsHit = NewIsHit;
}

void UWWAnimInstance::PlayCharacterHitMontage()
{
	if (ComboCount == 3 || bIsPlayingChargeAttack1Anim || bIsPlayingChargeAttack2Anim || bIsPlayingChargeAttack3Anim || bIsInAir)
	{
		return;
	}

	InitBoolCondition();
	bIsPlayingCharacterHitMontage = true;

	if (ensure(CharacterHitMongtage) == false) return;

	Montage_Play(CharacterHitMongtage, HitAnimRate);
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
	OnChargeAttack3EndDelegate.Broadcast();
}

void UWWAnimInstance::AnimNotify_FallingStart()
{
	if (bIsDead)
	{
		return;
	}

	StopAllMontages(0);
	InitBoolCondition();
}

void UWWAnimInstance::AnimNotify_FallingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_FallingEnd"));

	if (bIsDead)
	{
		return;
	}

	bIsFallen = true;
}

void UWWAnimInstance::AnimNotify_StandUpStart()
{
	
}

void UWWAnimInstance::AnimNotify_StandUpEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_StandUpEnd"));
	bIsFallen = false;
	bIsHitAndFly = false;
}

void UWWAnimInstance::AnimNotify_HitStart()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_HitStart"));
}

void UWWAnimInstance::AnimNotify_HitEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_HitEnd"));
}

void UWWAnimInstance::AnimNotify_HitAndFlyStart()
{
	Montage_Stop(0, CharacterHitMongtage);
}

void UWWAnimInstance::InitBoolCondition()
{
	bIsAttacking = false;
	bCanComboAttack = false;
	bWillPlayNextCombo = false;
	bIsPlayingChargeAttack1Anim = false;
	bIsPlayingCharacterHitMontage = false;

	bWillPlayChargeAttack2Anim = false;
	bIsPlayingChargeAttack2Anim = false;

	bWillPlayChargeAttack3Anim = false;
	bIsPlayingChargeAttack3Anim = false;
	ChargeAttack3ComboCount = 0;

	Montage_Stop(0, AttackMontage);
	Montage_Stop(0, ChargeAttack1);
	Montage_Stop(0, ChargeAttack2);
	Montage_Stop(0, ChargeAttack3);
}