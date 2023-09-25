// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/Melee360AttackAnimNotifies/ANS_Melee360AttackSkillWindow.h"

#include "Character/PlayerCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"
#include "WWAnimInstance.h"

UANS_Melee360AttackSkillWindow::UANS_Melee360AttackSkillWindow()
{
}

void UANS_Melee360AttackSkillWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_Melee360AttackSkillWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_Melee360AttackSkillWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
	
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackSkillWindow::BranchingPointNotifyEnd, MeshComp == nullptr"));
	}

	/*APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackSkillWindow::BranchingPointNotifyEnd, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = Cast<UWarriorSkillComponent>(Player->GetSkillComponenet());
	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackSkillWindow::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackSkillWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}*/

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::BranchingPointNotifyEnd, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, AnimInstance == nullptr"));
		return;
	}
	
	AnimInstance->SetIsPlayingCharacterHitMontage(false);
	SkillComp->SetIsChargeAttack3Started(false);
	//AnimInstance->StopAllMontages(1);
}