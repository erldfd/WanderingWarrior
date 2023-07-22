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
DECLARE_MULTICAST_DELEGATE(FOnChargeAttack3DamageDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChargeAttack3ComboStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChargeAttack3EndDelegate);
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

	bool GetIsPlayingChargeAttack1Anim();
	void SetIsPlayingChargeAttack1Anim(bool bIsPlaying);

	void PlayCharacterHitMontage();
	void StopCharacterHitMontage();

	bool GetIsPlayingCharacterHitMontage();
	void SetIsPlayingCharacterHitMontage(bool bIsPlaying);

	int32 GetComboCount();

	bool GetHitAndFly();
	void SetHitAndFly(bool NewHitAndFly);

	bool GetIsPlayingChargeAttack2Anim();
	void SetIsPlayingChargeAttack2Anim(bool NewIsPlayingChargeAttack2Anim);

	bool GetWillPlayChargeAttack2Anim();
	void SetWillPlayChargeAttack2Anim(bool NewWillPlayChargeAttack2Anim);

	void PlayKickAttackMongate();
		
	bool GetIsPlayingChargeAttack3Anim();
	void SetIsPlayingChargeAttack3Anim(bool NewIsPlayingChargeAttack3Anim);

	bool GetWillPlayChargeAttack3Anim();
	void SetWillPlayChargeAttack3Anim(bool NewWillPlayChargeAttack3Anim);

	void PlayChargeAttack3Montage();

	bool GetIsHit();
	void SetIsHit(bool NewIsHit);


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

	FOnChargeAttack3DamageDelegate OnChargeAttack3DamageDelegate;
	FOnChargeAttack3ComboStartDelegate OnChargeAttack3ComboStartDelegate;
	FOnChargeAttack3EndDelegate OnChargeAttack3EndDelegate;

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

	UFUNCTION()
	void AnimNotify_Melee360AttackDamage();

	UFUNCTION()
	void AnimNotify_Melee360AttackComboStart();

	UFUNCTION()
	void AnimNotify_Melee360AttackEnd();

	UFUNCTION()
	void AnimNotify_FallingStart();

	UFUNCTION()
	void AnimNotify_FallingEnd();

	UFUNCTION()
	void AnimNotify_StandUpStart();

	UFUNCTION()
	void AnimNotify_StandUpEnd();

	UFUNCTION()
	void AnimNotify_HitStart();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_HitAndFlyStart();

	void InitBoolCondition();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsInAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsHitAndFly : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsFallen : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsHit : 1;

	uint8 bIsAttacking : 1;
	uint8 bCanComboAttack : 1;
	uint8 bWillPlayNextCombo : 1;
	uint8 bIsPlayingChargeAttack1Anim : 1;
	uint8 bIsPlayingCharacterHitMontage : 1;

	uint8 bIsPlayingChargeAttack2Anim : 1;
	uint8 bWillPlayChargeAttack2Anim : 1;

	uint8 bIsPlayingChargeAttack3Anim : 1;
	uint8 bWillPlayChargeAttack3Anim : 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ChargeAttack1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> CharacterHitMongtage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ChargeAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ChargeAttack3;

	int32 ComboCount;

	int32 ChargeAttack3ComboCount;

	UPROPERTY(EditAnywhere, Category = Anim, meta = (AllowPrivateAccess = true))
	int32 ChargeAttack3MaxComboCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float HitAnimRate;
};
