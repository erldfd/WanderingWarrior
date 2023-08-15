// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCharacterStatComponent::GetHP()
{
	return HP;
}

void UCharacterStatComponent::SetHP(float CharacterHP)
{
	HP = FMath::Clamp(CharacterHP, 0.0f, MaxHP);
	
	float NewHPRatio = HP / MaxHP;
	SetHPRatio(NewHPRatio);

	OnHPChangedDelegate.Broadcast();
}

float UCharacterStatComponent::GetHPRatio()
{
	return HPRatio;
}

void UCharacterStatComponent::SetHPRatio(float NewHPRatio)
{
	HPRatio = NewHPRatio;
}

float UCharacterStatComponent::GetAttackDamage()
{
	return AttackDamage;
}

void UCharacterStatComponent::SetAttackDamage(float Damage)
{
	AttackDamage = Damage;
}

float UCharacterStatComponent::GetMaxHP()
{
	return MaxHP;
}

void UCharacterStatComponent::SetMaxHP(float NewMaxHP)
{
	MaxHP = NewMaxHP;
}

float UCharacterStatComponent::GetMP()
{
	return MP;
}

void UCharacterStatComponent::SetMP(float CharacterMP)
{
	MP = FMath::Clamp(CharacterMP, 0.0f, MaxMP);

	float NewMPRatio = MP / MaxMP;
	SetMPRatio(NewMPRatio);

	OnMPChangedDelegate.Broadcast();
}

float UCharacterStatComponent::GetMPRatio()
{
	return MPRatio;
}

void UCharacterStatComponent::SetMPRatio(float NewMPRatio)
{
	MPRatio = NewMPRatio;
}

float UCharacterStatComponent::GetMaxMP()
{
	return MaxMP;
}

void UCharacterStatComponent::SetMaxMP(float NewMaxMP)
{
	MaxMP = NewMaxMP;
}
