// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SkillComponentBase.h"

#include "WizardSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWizardSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()
	
public:

	UWizardSkillComponent();

	//virtual void BeginPlay() override;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlayChargeAttack1(float PlayRate) override;
	virtual void PlayChargeAttack2(float PlayRate) override;
	virtual void PlayChargeAttack3(float PlayRate) override;
	virtual void PlayMusouAttack(float PlayRate) override;
	virtual void PlayParryAttack(float PlayRate) override;

	
	//void PlayParryAttackInternal(float PlayRate);

	//void ReadyToPlayMelee360AttackLastSection();
	//void ReadyToPlayMusouFinalAttack();

	virtual void DamageChargeAttack1() override;
	virtual void DamageChargeAttack2() override;
	virtual void DamageChargeAttack3() override;
	virtual void DamageMusouAttack() override;
	virtual void DamageMusouFinishAttack() override;
	virtual void DamageParryAttack() override;

	void DamageWizardSkill3JumpAttack();

	//void DamageParryAttackInternal();

	//int32 GetMelee360AttackRepeatCount() const;
	//void SetMelee360AttackRepeatCount(int32 NewRepeatCount);

private:

	void PlayWizardSkill1(float PlayRate);
	void PlayWizardSkill2(float PlayRate);
	void PlayWizardSkill3(float PlayRate);
	void PlayWizardMusouAttack(float PlayRate);

	void DamageWizardSkill1();
	void DamageWizardSkill2();
	void DamageWizardSkill3();

	void DamageWizardMusouAttack();
	void DamageWizardMusouFinalAttack();

private:

	UPROPERTY(EditAnywhere, Category = Skill)
	float WizardSkill1Radius;

	UPROPERTY(EditAnywhere, Category = Skill)
	float WizardSkill1Damage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float WizardSkill1HeightLimit;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMagicBall> WizardSkill2MagicBall;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMagicBall> WizardSkill3MagicBall;

	UPROPERTY(EditAnywhere, Category = Skill)
	float WizardSkill3JumpDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float WizardSkill3HeightLimit;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMagicBall> WizardMusouSmallMagicBall;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMagicBall> WizardMusouBigMagicBall;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float KickAttackRange;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float KickAttackExtent;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float KickAttackDamage;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//int32 Melee360AttackRepeatCount;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float Melee360AttackRadius;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float Melee360AttackDamage;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float Melee360AttackHeightLimit;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float MusouAttackRadius;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float MusouAttackDamage;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float MusouAttackHeightLimit;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float ParryAttackRange;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float ParryAttackExtent;

	//UPROPERTY(EditAnywhere, Category = Skill)
	//float ParryAttackDamage;

};
