// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MusouAttackAnimNotifies/ANS_MusouRepeatWindow.h"

#include "Character/PlayerCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

void UANS_MusouRepeatWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_MusouRepeatWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
	
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyTick, Consistent : %d"), Player->GetConsistentMusou());
	Player->SetIsConsistentMusou(false);
	
}

void UANS_MusouRepeatWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	/*APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyEnd, Consistent : %d"), Player->GetIsConsistentMusou());
	if (Player->GetIsConsistentMusou())
	{
		return;
	}

	UWarriorSkillComponent* SkillComp = Player->GetSkillComponenet();
	if (SkillComp == nullptr)
	{
		return;
	}*/

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyEnd, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyEnd, SkillComp == nullptr"));
		return;
	}

	/*UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, AnimInstance == nullptr"));
		return;
	}*/

	SkillComp->ReadyToPlayMusouFinalAttack();
}