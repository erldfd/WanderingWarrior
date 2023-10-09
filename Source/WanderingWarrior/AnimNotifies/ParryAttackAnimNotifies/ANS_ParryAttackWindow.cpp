// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ParryAttackAnimNotifies/ANS_ParryAttackWindow.h"

#include "Character/PlayerCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"

void UANS_ParryAttackWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_ParryAttackWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotifyTick, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotifyTick, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotifyTick, AnimInstance == nullptr"));
		return;
	}

	if (AnimInstance->GetBeingStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotifyTick, AnimInstance->GetBeingStunned()"));
		Character->SetIsParrySucceeded(false);
		SkillComp->SetIsParrying(false);
		AnimInstance->SetIsParrying(false);
		AnimInstance->SetIsGuarding(false);
		AnimInstance->StopAllMontages(1);

		return;
	}
}

void UANS_ParryAttackWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotify, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_ParryAttackWindow::BranchingPointNotify, AnimInstance == nullptr"));
		return;
	}

	Character->SetIsParrySucceeded(false);
	//Character->SetIsGuarding(false);
	SkillComp->SetIsParrying(false);
	AnimInstance->SetIsParrying(false);
	AnimInstance->SetIsGuarding(false);
}