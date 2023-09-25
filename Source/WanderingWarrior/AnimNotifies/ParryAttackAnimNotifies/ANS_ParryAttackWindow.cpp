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
}

void UANS_ParryAttackWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	/*APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	UWarriorSkillComponent* SkillComp = Player->GetSkillComponenet();
	if (SkillComp == nullptr)
	{
		return;
	}

	UWWAnimInstance* AnimInstance = &Player->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}*/

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