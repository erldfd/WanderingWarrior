// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "WWAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitByEnemyDelegate);

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

	bool GetIsDead() const;
	void SetIsDead(bool IsDead);

	bool IsPlayingSomething();

	void PlayCharacterHitMontage();
	void StopCharacterHitMontage();

	bool GetIsPlayingCharacterHitMontage() const;
	void SetIsPlayingCharacterHitMontage(bool bIsPlaying);

	bool GetHitAndFly() const;
	void SetHitAndFly(bool bNewHitAndFly);

	bool GetIsAttackDetected() const;
	void SetDetectedAttack(bool NewDetectedAttack);

	bool GetIsIdleOrRun() const;
	void SetIsIdleOrRun(bool NewIsIdleOrWalk);

	bool GetIsGuarding() const;
	void SetIsGuarding(bool NewIsGuarding);

	bool GetIsGuardHitStart() const;
	void SetIsGuardHitStart(bool NewIsGuardHitStart);

	void PlayGuardHitAnim();

	bool GetBeingStunned() const;
	void SetBeingStunned(bool NewBeingStunned);

	bool GetIsParrying() const;
	void SetIsParrying(bool NewIsParrying);

	bool GetIsActingMusou() const;
	void SetIsActingMusou(bool NewIsActingMusou);

	bool GetIsActionCameraMoving() const;
	void SetIsActionCameraMoving(bool NewIsActionCameraMoving);

	void InitBoolCondition();

public:

	FOnHitByEnemyDelegate OnHitByEnemyDelegate;

private:

	UFUNCTION()
	void AnimNotify_FallingStart();

	UFUNCTION()
	void AnimNotify_FallingEnd();

	UFUNCTION()
	void AnimNotify_StandUpStart();

	UFUNCTION()
	void AnimNotify_StandUpEnd();

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

	UFUNCTION()
	void AnimNotify_IdleOrRun();

	UFUNCTION()
	void AnimNotify_WizardHitAndFlyEnd();

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

	uint8 bIsAttackDetected : 1;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> CharacterHitMongtage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> GuardHitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> FallenToStandMontage;

	int32 ComboCount;

	int32 ChargeAttack3ComboCount;

	float AttackAnimRate;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<class UMaterialInstance> LeftFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<class UMaterialInstance> RightFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = VisualEffect)
	TObjectPtr<UParticleSystem> PS_WaterSplash;

	float AnimTimer;
};
