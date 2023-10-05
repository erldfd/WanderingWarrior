// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardMusouAttackAnimNotifies/AN_WizardMusouAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WizardSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_WizardMusouAttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardMusouAttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_WizardMusouAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageMusouAttack();
}