// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardMusouAttackAnimNotifies/ANS_WizardMusouWindow.h"

#include "Character/WWCharacter.h"
#include "Components/WizardSkillComponent.h"
#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"

void UANS_WizardMusouWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_WizardMusouWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyTick, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyTick, AnimInstance == nullptr"));
		return;
	}

	if (AnimInstance->GetBeingStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyTick, AnimInstance->GetBeingStunned()"));

		SkillComp->SetIsMusouAttackStarted(false);

		FRotator CharacterRoation = Character->GetActorRotation();
		CharacterRoation.Pitch = 0;
		Character->SetActorRotation(CharacterRoation);

		AnimInstance->StopAllMontages(1);

		return;
	}
	
}

void UANS_WizardMusouWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UWizardSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WizardSkillComponent)
	{
		SkillComp = Cast<UWizardSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsMusouAttackStarted(false);
	
	FRotator CharacterRoation = Character->GetActorRotation();
	CharacterRoation.Pitch = 0;
	Character->SetActorRotation(CharacterRoation);
}