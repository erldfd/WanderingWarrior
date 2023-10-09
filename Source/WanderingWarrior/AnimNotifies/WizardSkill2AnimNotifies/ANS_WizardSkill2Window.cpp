// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardSkill2AnimNotifies/ANS_WizardSkill2Window.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/WizardSkillComponent.h"
#include "Character/PlayerCharacter.h"

void UANS_WizardSkill2Window::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_WizardSkill2Window::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyTick, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyTick, AnimInstance == nullptr"));
		return;
	}

	if (AnimInstance->GetBeingStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyTick, AnimInstance->GetBeingStunned()"));
		SkillComp->SetIsChargeAttack2Started(false);
		AnimInstance->StopAllMontages(1);

		return;
	}
}

void UANS_WizardSkill2Window::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill2Window::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsChargeAttack2Started(false);
}