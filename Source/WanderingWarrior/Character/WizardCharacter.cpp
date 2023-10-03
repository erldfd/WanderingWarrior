// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WizardCharacter.h"

#include "Components/WizardSkillComponent.h"

AWizardCharacter::AWizardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;

	Super::SkillComponent = CreateDefaultSubobject<UWizardSkillComponent>(TEXT("Skill"));

	Tags.Init("Wizard", 1);

	Super::ParryWindow = 1;
}
