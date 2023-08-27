// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "SkillComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WANDERINGWARRIOR_API USkillComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	USkillComponentBase();

	virtual void PlayChargeAttack1(float PlayRate);
	virtual void PlayChargeAttack2(float PlayRate);
	virtual void PlayChargeAttack3(float PlayRate);
	virtual void PlayMusouAttack(float PlayRate);
	virtual void PlayParryAttack(float PlayRate);

	virtual bool IsSkillStarted() const;

	virtual bool GetIsChargeAttack1Started() const;
	virtual void SetIsChargeAttack1Started(bool bNewIsChargeAttack1Started);

	virtual bool GetIsChargeAttack2Started() const;
	virtual void SetIsChargeAttack2Started(bool bNewIsChargeAttack2Started);

	virtual bool GetIsChargeAttack3Started() const;
	virtual void SetIsChargeAttack3Started(bool bNewIsChargeAttack3Started);

	virtual bool GetIsMusouAttackStarted() const;
	virtual void SetIsMusouAttackStarted(bool bNewIsMusouAttackStarted);

	virtual bool GetIsParrying() const;
	virtual void SetIsParrying(bool bNewIsParrying);

	virtual void DamageChargeAttack1();
	virtual void DamageChargeAttack2();
	virtual void DamageChargeAttack3();
	virtual void DamageMusouAttack();
	virtual void DamageMusouFinishAttack();
	virtual void DamageParryAttack();

protected:

	UPROPERTY(Category = SkillData, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkillDataAsset> SkillData;

	uint8 bIsChargeAttack1Started: 1;
	uint8 bIsChargeAttack2Started : 1;
	uint8 bIsChargeAttack3Started : 1;
	uint8 bIsMusouAttackStarted : 1;
	uint8 bIsParrying : 1;

};
