// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardMusouAttackAnimNotifies/ANS_WizardMusouFinalAttackCheck.h"

#include "Components/WizardSkillComponent.h"
#include "Character/WWCharacter.h"
#include "WWEnumClassContainer.h"

void UANS_WizardMusouFinalAttackCheck::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_WizardMusouFinalAttackCheck::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_WizardMusouFinalAttackCheck::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* Mesh = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(Mesh->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouFinalAttackCheck::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouFinalAttackCheck::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageMusouFinishAttack();
}
