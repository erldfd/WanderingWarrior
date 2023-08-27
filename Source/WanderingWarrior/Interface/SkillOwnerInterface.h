// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "SkillOwnerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WANDERINGWARRIOR_API ISkillOwnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*virtual void PlayChargeAttack1() = 0;
	virtual	void PlayChargeAttack2() = 0;
	virtual void PlayChargeAttack3() = 0;
	virtual void PlayMusouAttack() = 0;

	virtual class UAnimMontage* GetChargeAttack1Montage() const = 0;
	virtual class UAnimMontage* GetChargeAttack2Montage() const = 0;
	virtual class UAnimMontage* GetChargeAttack3Montage() const = 0;
	virtual class UAnimMontage* GetMusouAttackMontage() const = 0;

	virtual bool GetIsSkillStarted() const = 0;
	virtual void SetIsSkillStarted(bool NewIsSkillStarted) = 0;

	virtual bool GetIsMusouAttackStarted() const = 0;
	virtual void SetIsMusouAttackStarted(bool NewIsMusouAttackStarted) = 0;*/

};
