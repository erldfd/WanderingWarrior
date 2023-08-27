// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SkillComponentBase.h"

#include "WarriorSkillComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class WANDERINGWARRIOR_API UWarriorSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()
	
public:

	UWarriorSkillComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlayChargeAttack1(float PlayRate) override;
	virtual void PlayChargeAttack2(float PlayRate) override;
	virtual void PlayChargeAttack3(float PlayRate) override;
	virtual void PlayMusouAttack(float PlayRate) override;
	virtual void PlayParryAttack(float PlayRate) override;

	void PlayJumpToGroundSkill(float PlayRate);
	void PlayKickAttack(float PlayRate);
	void PlayMelee360Attack(float PlayRate);
	void PlayHurricaneKickAttack(float PlayRate);
	void PlayParryAttackInternal(float PlayRate);

	void ReadyToPlayMelee360AttackLastSection();
	void ReadyToPlayMusouFinalAttack();

	//virtual bool IsSkillStarted() override;

	/*virtual bool GetIsChargeAttack1Started() const override;
	virtual void SetIsChargeAttack1Started(bool bNewIsChargeAttack1Started);

	virtual bool GetIsChargeAttack2Started() const override;
	virtual void SetIsChargeAttack2Started(bool bNewIsChargeAttack2Started);

	virtual bool GetIsChargeAttack3Started() const override;
	virtual void SetIsChargeAttack3Started(bool bNewIsChargeAttack3Started);

	virtual bool GetIsMusouAttackStarted() const override;
	virtual void SetIsMusouAttackStarted(bool bNewIsMusouAttackStarted);

	virtual bool GetIsParrying() const override;
	virtual void SetIsParrying(bool bNewIsParrying);*/

	virtual void DamageChargeAttack1() override;
	virtual void DamageChargeAttack2() override;
	virtual void DamageChargeAttack3() override;
	virtual void DamageMusouAttack() override;
	virtual void DamageMusouFinishAttack() override;
	virtual void DamageParryAttack() override;

	void DamageJumpToGrundSkill();

	void DamageKickAttack();

	void DamageMelee360Attack();

	void DamageMusouAttackInternal();

	void DamageMusouFinishAttackInternal();

	void DamageParryAttackInternal();

	int32 GetMelee360AttackRepeatCount() const;
	void SetMelee360AttackRepeatCount(int32 NewRepeatCount);

private:

	void SetPlayerCameraFOV(float FOV);

	void ShakeWithCameraFOV(float FOV, float Duration);

private:

	UPROPERTY(EditAnywhere, Category = Skill)
	float JumpToGrundRadius;

	UPROPERTY(EditAnywhere, Category = Skill)
	float JumpToGrundDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float JumpToGrundHeightLimit;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackRange;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackExtent;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 Melee360AttackRepeatCount;

	UPROPERTY(EditAnywhere, Category = Skill)
	float Melee360AttackRadius;

	UPROPERTY(EditAnywhere, Category = Skill)
	float Melee360AttackDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float Melee360AttackHeightLimit;

	UPROPERTY(EditAnywhere, Category = Skill)
	float MusouAttackRadius;

	UPROPERTY(EditAnywhere, Category = Skill)
	float MusouAttackDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float MusouAttackHeightLimit;

	UPROPERTY(EditAnywhere, Category = Skill)
	float ParryAttackRange;

	UPROPERTY(EditAnywhere, Category = Skill)
	float ParryAttackExtent;

	UPROPERTY(EditAnywhere, Category = Skill)
	float ParryAttackDamage;


	/*uint8 bIsJumpToGroundSkillStarted : 1;
	uint8 bIsKickAttackSkillStarted : 1;
	uint8 bIsMelee360AttackSkillStarted : 1;
	uint8 bIsHurricaneKickAttackSkillStarted : 1;
	uint8 bIsParrying : 1;*/

	float CameraMoveTimeRatio;

	uint8 bIsStartedShake : 1;
	uint8 bIsDecreaseingFOV : 1;
	float FOVAlpha = 0;
};
