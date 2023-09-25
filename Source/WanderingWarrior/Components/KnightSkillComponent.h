// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponentBase.h"
#include "KnightSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UKnightSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()
	
public:

	UKnightSkillComponent();

	virtual void PlayChargeAttack1(float PlayRate) override;
	virtual void PlayChargeAttack2(float PlayRate) override;
	virtual void PlayChargeAttack3(float PlayRate) override;
	virtual void PlayMusouAttack(float PlayRate) override;
	virtual void PlayParryAttack(float PlayRate) override;

	virtual void DamageChargeAttack1() override;
	virtual void DamageChargeAttack2() override;
	virtual void DamageChargeAttack3() override;
	virtual void DamageMusouAttack() override;
	virtual void DamageMusouFinishAttack() override;
	virtual void DamageParryAttack() override;
	
};
