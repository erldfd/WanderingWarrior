// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BossCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WarriorSkillComponent.h"

ABossCharacter::ABossCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	SkillComponent = CreateDefaultSubobject<UWarriorSkillComponent>(TEXT("BossSkill"));
}