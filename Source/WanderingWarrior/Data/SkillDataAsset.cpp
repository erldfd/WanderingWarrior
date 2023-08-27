// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SkillDataAsset.h"

UAnimMontage* USkillDataAsset::GetChargeAttack1Montage() const
{
    if (ChargeAttack1Montage == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USkillDataAsset::GetChargeAttack1Montage, ChargeAttack1Montage == nullptr"));
        return nullptr;
    }

    return ChargeAttack1Montage;
}

UAnimMontage* USkillDataAsset::GetChargeAttack2Montage() const
{
    if (ChargeAttack2Montage == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USkillDataAsset::GetChargeAttack1Montage, ChargeAttack2Montage == nullptr"));
        return nullptr;
    }

    return ChargeAttack2Montage;
}

UAnimMontage* USkillDataAsset::GetChargeAttack3Montage() const
{
    if (ChargeAttack3Montage == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USkillDataAsset::GetChargeAttack1Montage, ChargeAttack3Montage == nullptr"));
        return nullptr;
    }

    return ChargeAttack3Montage;
}

UAnimMontage* USkillDataAsset::GetMusouAttackMontage() const
{
    if (MusouAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USkillDataAsset::GetChargeAttack1Montage, MusouAttackMontage == nullptr"));
        return nullptr;
    }

    return MusouAttackMontage;
}

UAnimMontage* USkillDataAsset::GetParryAttackMontage() const
{
    if (ParryAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USkillDataAsset::GetParryAttackMontage, ParryAttackMontage == nullptr"));
        return nullptr;
    }

    return ParryAttackMontage;
}