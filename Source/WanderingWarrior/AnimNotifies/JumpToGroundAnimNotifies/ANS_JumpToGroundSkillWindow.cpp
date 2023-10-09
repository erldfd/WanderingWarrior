// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/JumpToGroundAnimNotifies/ANS_JumpToGroundSkillWindow.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/WarriorSkillComponent.h"
#include "Character/PlayerCharacter.h"

void UANS_JumpToGroundSkillWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_JumpToGroundSkillWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyTick, Character == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyTick, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyTick, AnimInstance == nullptr"));
		return;
	}

	if (AnimInstance->GetBeingStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyTick, AnimInstance->GetBeingStunned()"));
		SkillComp->SetIsChargeAttack1Started(false);
		AnimInstance->StopAllMontages(1);

		return;
	}
	
}

void UANS_JumpToGroundSkillWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyEnd, Character == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundSkillWindow::NotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsChargeAttack1Started(false);
}
