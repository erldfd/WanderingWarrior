// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardSkill1AnimNotifies/AN_WizardSkill1AttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WizardSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_WizardSkill1AttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardSkill1AttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardSkill1AttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageChargeAttack1();
}