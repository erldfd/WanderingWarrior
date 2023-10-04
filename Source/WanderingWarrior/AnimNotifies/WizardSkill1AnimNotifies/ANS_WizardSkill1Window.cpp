// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardSkill1AnimNotifies/ANS_WizardSkill1Window.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/WizardSkillComponent.h"
#include "Character/PlayerCharacter.h"

void UANS_WizardSkill1Window::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_WizardSkill1Window::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_WizardSkill1Window::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill1Window::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill1Window::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill1Window::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsChargeAttack1Started(false);
}