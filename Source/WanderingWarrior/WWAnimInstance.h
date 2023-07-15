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
DECLARE_MULTICAST_DELEGATE(FOnKickDamageDelegate);
DECLARE_MULTICAST_DELEGATE(FOnKickEndDelegate);

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UWWAnimInstance();

	virtual void NativeInitializeAnimation() override;
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

	int32 GetComboCount();

	bool GetHitAndFly();
	void SetHitAndFly(bool NewHitAndFly);

	bool GetIsPlayingKickAttackAnim();
	void SetIsPlayingKickAttackAnim(bool NewIsPlayingKickAttackAnim);

	bool GetWillPlayingKickAttackAnim();
	void SetWillPlayingKickAttackAnim(bool NewWillPlayingKickAttackAnim);

	void PlayKickAttackMongate();
		
public:

	FOnAttackStartDelegate OnAttackStartDelegate;
	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnCanInputNextComboDelegate OnCanInputNextComboDelegate;
	FOnStartNextComboDelegate OnStartNextComboDelegate;
	FOnAnimMoveStartDelegate OnAnimMoveStartDelegate;
	FOnAnimMoveEndDelegate OnAnimMoveEndDelegate;
	FOnAttackHitCheckDelegate OnAttackHitcheckDelegate;

	FOnJumpToGroundAnimEndDelegate OnJumpToGroundAnimEndDelegate;

	FOnKickDamageDelegate OnKickDamageDelegate;
	FOnKickEndDelegate OnKickEndDelegate;

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

	UFUNCTION()
	void AnimNotify_KickDamage();

	UFUNCTION()
	void AnimNotify_KickEnd();

	void InitBoolCondition();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsInAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	uint8 bIsHitAndFly : 1;

	uint8 bIsAttacking : 1;
	uint8 bCanComboAttack : 1;
	uint8 bWillPlayNextCombo : 1;
	uint8 bIsPlayingJumpToGroundSkillAnim : 1;
	uint8 bIsPlayingCharacterHitMontage : 1;

	uint8 bIsPlayingKickAttackAnim : 1;
	uint8 bWillPlayingKickAttackAnim : 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ChargeAttack1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> CharacterHitMongtage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ChargeAttack2;

	int32 ComboCount;

	UPROPERTY(EditAnywhere, Category = Anim, meta = (PrivateAccess = true))
	float HitAnimRate;
};
