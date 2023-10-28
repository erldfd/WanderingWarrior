// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MusouAttackAnimNotifies/ANS_MusouRepeatWindow.h"

#include "Character/WWCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

#include "Kismet/GameplayStatics.h"

void UANS_MusouRepeatWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(Character->GetWorld(), 1.0f);
	Character->CustomTimeDilation = 1.0f;
	Character->bUseControllerRotationYaw = true;
}

void UANS_MusouRepeatWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
	
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}
	
	Character->SetIsConsistentMusou(false);
	Character->AddActorWorldOffset(Character->GetActorForwardVector() * 700 * FrameDeltaTime, true);
}

void UANS_MusouRepeatWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouRepeatWindow::BranchingPointNotifyEnd, Character == nullptr"));
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

	if (Character->GetIsConsistentMusou())
	{
		return;
	}

	SkillComp->ReadyToPlayMusouFinalAttack();
	Character->bUseControllerRotationYaw = false;
}