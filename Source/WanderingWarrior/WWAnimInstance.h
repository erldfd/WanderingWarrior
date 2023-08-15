// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "WWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHitByEnemyDelegate);
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
DECLARE_MULTICAST_DELEGATE(FOnInitIsDamaged);
DECLARE_MULTICAST_DELEGATE(FOnMusouAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMusouFinishAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnParryAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnParryAttackStartDelegate);
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
	void SetIsAttacking(bool NewIsAttacking);

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

	void SetAttackAnimRate(float NewAttackAnimRate);

	bool GetDetectedAttack();
	void SetDetectedAttack(bool NewDetectedAttack);

	void PlayMusouAnim();

	bool GetIsIdleOrRun();
	void SetIsIdleOrRun(bool NewIsIdleOrWalk);

	bool GetIsGuarding();
	void SetIsGuarding(bool NewIsGuarding);

	bool GetIsGuardHitStart();
	void SetIsGuardHitStart(bool NewIsGuardHitStart);

	void PlayGuardHitAnim();

	bool GetBeingStunned();
	void SetBeingStunned(bool NewBeingStunned);

	void PlayParryAttackAnim();

	bool GetIsParrying();
	void SetIsParrying(bool NewIsParrying);

	bool GetIsActingMusou();
	void SetIsActingMusou(bool NewIsActingMusou);

	bool GetIsActionCameraMoving();
	void SetIsActionCameraMoving(bool NewIsActionCameraMoving);

	void InitBoolCondition();

public:

	FOnAttackStartDelegate OnAttackStartDelegate;
	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnHitByEnemyDelegate OnHitByEnemyDelegate;
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

	FOnInitIsDamaged OnInitIsDamaged;

	FOnMusouAttackCheckDelegate OnMusouAttackCheckDelegate;
	FOnMusouFinishAttackCheckDelegate OnMusouFinishAttackCheckDelegate;

	FOnParryAttackCheckDelegate OnParryAttackCheckDelegate;
	FOnParryAttackStartDelegate OnParryAttackStartDelegate;

private:

	UFUNCTION()
	void AnimNotify_AttackDetectStart();

	UFUNCTION()
	void AnimNotify_AttackDetectEnd();

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

	UFUNCTION()
	void AnimNotify_RunRightFoot();

	UFUNCTION()
	void AnimNotify_RunLeftFoot();

	UFUNCTION()
	void AnimNotify_MusouReadyStart();

	UFUNCTION()
	void AnimNotify_ActionCameraMoveEnd();

	UFUNCTION()
	void AnimNotify_MusouReadyEnd();

	UFUNCTION()
	void AnimNotify_MusouAttackCheck();

	UFUNCTION()
	void AnimNotify_MusouAttackEnd();

	UFUNCTION()
	void AnimNotify_MusouFinishDown();

	UFUNCTION()
	void AnimNotify_MusouFinishAttackCheck();

	UFUNCTION()
	void AnimNotify_MusouFinishAttackEnd();

	UFUNCTION()
	void AnimNotify_WarriorIdleStart();

	UFUNCTION()
	void AnimNotify_WarriorRunStart();

	UFUNCTION()
	void AnimNotify_GuardHit();

	UFUNCTION()
	void AnimNotify_GuardHitStart();

	UFUNCTION()
	void AnimNotify_GuardHitEnd();

	UFUNCTION()
	void AnimNotify_ParryAttackCheck();

	UFUNCTION()
	void AnimNotify_ParryAttackStart();

	UFUNCTION()
	void AnimNotify_ParryAttackEnd();

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
	uint8 bIsGuarding : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsGuardHitStart : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bBeingStunned : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	uint8 bIsActingMusou : 1;

	uint8 bIsDetectedAttack : 1;
	uint8 bIsAttacking : 1; // about attack anim

	uint8 bCanComboAttack : 1;
	uint8 bWillPlayNextCombo : 1;
	uint8 bIsPlayingChargeAttack1Anim : 1;
	uint8 bIsPlayingCharacterHitMontage : 1;

	uint8 bIsPlayingChargeAttack2Anim : 1;
	uint8 bWillPlayChargeAttack2Anim : 1;

	uint8 bIsPlayingChargeAttack3Anim : 1;
	uint8 bWillPlayChargeAttack3Anim : 1;

	uint8 bIsIdleOrRun : 1;

	uint8 bIsParrying : 1;

	uint8 bIsActionCameraMoving : 1;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> MusouAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> GuardHitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ParryAttack;

	int32 ComboCount;

	int32 ChargeAttack3ComboCount;

	UPROPERTY(EditAnywhere, Category = Anim, meta = (AllowPrivateAccess = true))
	int32 ChargeAttack3MaxComboCount;

	float AttackAnimRate;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<class UMaterialInstance> LeftFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<class UMaterialInstance> RightFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<UParticleSystem> PS_WaterSplash;
};
