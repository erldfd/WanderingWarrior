// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponentBase.h"

#include "Data/SkillDataAsset.h"

// Sets default values for this component's properties
USkillComponentBase::USkillComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillComponentBase::PlayChargeAttack1(float PlayRate)
{
}

void USkillComponentBase::PlayChargeAttack2(float PlayRate)
{
}

void USkillComponentBase::PlayChargeAttack3(float PlayRate)
{
}

void USkillComponentBase::PlayMusouAttack(float PlayRate)
{
}

void USkillComponentBase::PlayParryAttack(float PlayRate)
{
}

bool USkillComponentBase::IsSkillStarted() const
{
	UE_LOG(LogTemp, Warning, TEXT("USkillComponentBase::IsSkillStarted, %d, %d, %d, %d, %d"), GetIsChargeAttack1Started(),
		GetIsChargeAttack2Started(), GetIsChargeAttack3Started(), GetIsMusouAttackStarted(), GetIsParrying());

	return (GetIsChargeAttack1Started() || GetIsChargeAttack2Started() || GetIsChargeAttack3Started() ||
		GetIsMusouAttackStarted() || GetIsParrying());
}

bool USkillComponentBase::GetIsChargeAttack1Started() const
{
	return bIsChargeAttack1Started;
}

void USkillComponentBase::SetIsChargeAttack1Started(bool bNewIsChargeAttack1Started)
{
	bIsChargeAttack1Started = bNewIsChargeAttack1Started;
}

bool USkillComponentBase::GetIsChargeAttack2Started() const
{
	return bIsChargeAttack2Started;
}

void USkillComponentBase::SetIsChargeAttack2Started(bool bNewIsChargeAttack2Started)
{
	bIsChargeAttack2Started = bNewIsChargeAttack2Started;
}

bool USkillComponentBase::GetIsChargeAttack3Started() const
{
	return bIsChargeAttack3Started;
}

void USkillComponentBase::SetIsChargeAttack3Started(bool bNewIsChargeAttack3Started)
{
	bIsChargeAttack3Started = bNewIsChargeAttack3Started;
}

bool USkillComponentBase::GetIsMusouAttackStarted() const
{
	return bIsMusouAttackStarted;
}

void USkillComponentBase::SetIsMusouAttackStarted(bool bNewIsMusouAttackStarted)
{
	bIsMusouAttackStarted = bNewIsMusouAttackStarted;
}

bool USkillComponentBase::GetIsParrying() const
{
	return bIsParrying;
}

void USkillComponentBase::SetIsParrying(bool bNewIsParrying)
{
	bIsParrying = bNewIsParrying;
}

void USkillComponentBase::DamageChargeAttack1()
{
}

void USkillComponentBase::DamageChargeAttack2()
{
}

void USkillComponentBase::DamageChargeAttack3()
{
}

void USkillComponentBase::DamageMusouAttack()
{
}

void USkillComponentBase::DamageMusouFinishAttack()
{

}

void USkillComponentBase::DamageParryAttack()
{
}

