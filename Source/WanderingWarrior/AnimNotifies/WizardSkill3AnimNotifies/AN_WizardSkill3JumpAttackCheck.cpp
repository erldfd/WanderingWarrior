// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardSkill3AnimNotifies/AN_WizardSkill3JumpAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WizardSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_WizardSkill3JumpAttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardSkill3JumpAttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardSkill3JumpAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageWizardSkill3JumpAttack();
}