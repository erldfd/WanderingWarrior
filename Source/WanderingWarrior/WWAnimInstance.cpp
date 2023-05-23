// Fill out your copyright notice in the Description page of Project Settings.


#include "WWAnimInstance.h"

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
		JumpToGrundAnim = JUMP_TPGROUND_MOTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CHARACTER_HIT_MONTAGE(TEXT("/Game/Animations/CharacterHitMontage.CharacterHitMontage"));
	if (CHARACTER_HIT_MONTAGE.Succeeded())
	{
		CharacterHitMongtage = CHARACTER_HIT_MONTAGE.Object;
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
	Montage_Play(JumpToGrundAnim);
	bIsPlayingJumpToGroundSkillAnim = true;
}

bool UWWAnimInstance::IsPlayingSomething()
{
	return (bIsAttacking || bIsDead || bIsPlayingJumpToGroundSkillAnim || bIsPlayingCharacterHitMontage);
}

bool UWWAnimInstance::GetIsPlayingJumpToGroundSkillAnim()
{
	return bIsPlayingJumpToGroundSkillAnim;
}

void UWWAnimInstance::SetIsPlayingJumpToGroundSkillAnim(bool bIsPlaying)
{
	bIsPlayingJumpToGroundSkillAnim = bIsPlaying;
}

void UWWAnimInstance::PlayCharacterHitMontage()
{
	InitBoolCondition();
	bIsPlayingCharacterHitMontage = true;
	ensure(CharacterHitMongtage != nullptr);
	Montage_Play(CharacterHitMongtage);
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

	if (bWillPlayNextCombo == false)
	{
		return;
	}

	OnStartNextComboDelegate.Broadcast();

	JumpToAttackMontageSection(ComboCount);
	UE_LOG(LogTemp, Warning, TEXT("WWAnimInstasnce StartNextCombo, Character : %s"), *GetOwningActor()->GetName());
}

void UWWAnimInstance::AnimNotify_AttackEndNotify()
{
	bCanComboAttack = false;
	bIsAttacking = false;
	bWillPlayNextCombo = false;
	ComboCount = 0;

	OnAttackEndDelegate.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("WWAnimInstasnce AttackEnd"));
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

void UWWAnimInstance::InitBoolCondition()
{
	bIsAttacking = false;
	bCanComboAttack = false;
	bWillPlayNextCombo = false;
	bIsPlayingJumpToGroundSkillAnim = false;
	bIsPlayingCharacterHitMontage = false;

	Montage_Stop(0, AttackMontage);
	Montage_Stop(0, JumpToGrundAnim);
}