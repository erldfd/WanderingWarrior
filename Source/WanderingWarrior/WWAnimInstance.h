// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "WWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitByEnemyDelegate);
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

	bool GetIsDead();
	void SetIsDead(bool IsDead);

	bool IsPlayingSomething();

	void PlayCharacterHitMontage();
	void StopCharacterHitMontage();

	bool GetIsPlayingCharacterHitMontage();
	void SetIsPlayingCharacterHitMontage(bool bIsPlaying);

	bool GetHitAndFly();
	void SetHitAndFly(bool NewHitAndFly);

	bool GetDetectedAttack();
	void SetDetectedAttack(bool NewDetectedAttack);

	bool GetIsIdleOrRun();
	void SetIsIdleOrRun(bool NewIsIdleOrWalk);

	bool GetIsGuarding();
	void SetIsGuarding(bool NewIsGuarding);

	bool GetIsGuardHitStart();
	void SetIsGuardHitStart(bool NewIsGuardHitStart);

	void PlayGuardHitAnim();

	bool GetBeingStunned();
	void SetBeingStunned(bool NewBeingStunned);

	bool GetIsParrying();
	void SetIsParrying(bool NewIsParrying);

	bool GetIsActingMusou();
	void SetIsActingMusou(bool NewIsActingMusou);

	bool GetIsActionCameraMoving();
	void SetIsActionCameraMoving(bool NewIsActionCameraMoving);

	void InitBoolCondition();

public:

	FOnHitByEnemyDelegate OnHitByEnemyDelegate;
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
	void AnimNotify_CharacterHitAnimEndNotify();

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
	void AnimNotify_WarriorIdleStart();

	UFUNCTION()
	void AnimNotify_WarriorRunStart();

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
	TObjectPtr<UAnimMontage> CharacterHitMongtage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> GuardHitReaction;

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
