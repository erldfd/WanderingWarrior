// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "SkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	 class UAnimMontage* GetChargeAttack1Montage() const;
	 class UAnimMontage* GetChargeAttack2Montage() const;
	 class UAnimMontage* GetChargeAttack3Montage() const;
	 class UAnimMontage* GetMusouAttackMontage() const;
	 class UAnimMontage* GetParryAttackMontage() const;
	
private:

	UPROPERTY(EditAnywhere, Category = SkillAnimMontage)
	TObjectPtr<class UAnimMontage> ChargeAttack1Montage;

	UPROPERTY(EditAnywhere, Category = SkillAnimMontage)
	TObjectPtr<class UAnimMontage> ChargeAttack2Montage;

	UPROPERTY(EditAnywhere, Category = SkillAnimMontage)
	TObjectPtr<class UAnimMontage> ChargeAttack3Montage;

	UPROPERTY(EditAnywhere, Category = SkillAnimMontage)
	TObjectPtr<class UAnimMontage> MusouAttackMontage;

	UPROPERTY(EditAnywhere, Category = SkillAnimMontage)
	TObjectPtr<class UAnimMontage> ParryAttackMontage;
};
