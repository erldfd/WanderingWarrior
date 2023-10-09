// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardSkill3AnimNotifies/ANS_WizardSkill3Window.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/WizardSkillComponent.h"
#include "Character/WWCharacter.h"

void UANS_WizardSkill3Window::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_WizardSkill3Window::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyTick, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyTick, AnimInstance == nullptr"));
		return;
	}

	if (AnimInstance->GetBeingStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyTick, AnimInstance->GetBeingStunned()"));
		SkillComp->SetIsChargeAttack3Started(false);
		AnimInstance->StopAllMontages(1);

		return;
	}
}

void UANS_WizardSkill3Window::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardSkill3Window::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsChargeAttack3Started(false);
}