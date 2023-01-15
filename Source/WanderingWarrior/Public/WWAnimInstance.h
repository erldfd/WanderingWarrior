// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "WWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCanInputNextComboDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStartNextComboDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAnimMoveStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAnimMoveEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpToGroundAnimEndDelegate);

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UWWAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	bool GetIsDead();
	void SetIsDead(bool IsDead);

	bool GetIsAttacking();
	
	bool GetCanCombo();

	bool GetWillPlayNextCombo();
	void SetWillPlayNextCombo(bool bWillPlayNextComboNow);

	void PlayJumpToGrundAnim();

	bool IsPlayingSomething();

	bool GetIsPlayingJumpToGroundSkillAnim();
	void SetIsPlayingJumpToGroundSkillAnim(bool bIsPlaying);

	void PlayCharacterHitMontage();

	bool GetIsPlayingCharacterHitMontage();
	void SetIsPlayingCharacterHitMontage(bool bIsPlaying);

public:

	FOnAttackStartDelegate OnAttackStartDelegate;
	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnCanInputNextComboDelegate OnCanInputNextComboDelegate;
	FOnStartNextComboDelegate OnStartNextComboDelegate;
	FOnAnimMoveStartDelegate OnAnimMoveStartDelegate;
	FOnAnimMoveEndDelegate OnAnimMoveEndDelegate;
	FOnAttackHitCheckDelegate OnAttackHitcheckDelegate;

	FOnJumpToGroundAnimEndDelegate OnJumpToGroundAnimEndDelegate;

private:

	UFUNCTION()
	void AnimNotify_CanComboNotify();

	UFUNCTION()
	void AnimNotify_StartNextComboNotify();

	UFUNCTION()
	void AnimNotify_AttackEndNotify();

	UFUNCTION()
	void AnimNotify_AnimMoveStartNotify();

	UFUNCTION()
	void AnimNotify_AnimMoveEndNotify();

	UFUNCTION()
	void AnimNotify_AttackHitCheckNotify();

	UFUNCTION()
	void AnimNotify_JumpToGroundEndNotify();

	UFUNCTION()
	void AnimNotify_CharacterHitAnimEndNotify();

	void InitBoolCondition();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	bool bIsAttacking;
	bool bCanComboAttack;
	bool bWillPlayNextCombo;
	bool bIsPlayingJumpToGroundSkillAnim;
	bool bIsPlayingCharacterHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> JumpToGrundAnim;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> CharacterHitMongtage;

	int ComboCount;

	
};
