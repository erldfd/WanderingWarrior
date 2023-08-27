// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/ANS_AttackTotalWindow.h"

#include "Character/WWCharacter.h"

void UANS_AttackTotalWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
}

void UANS_AttackTotalWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_AttackTotalWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackTotalWindow::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackTotalWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	//AnimInstance->StopAllMontages(1);
	Character->SetComboCount(0);
	UE_LOG(LogTemp, Warning, TEXT("UANS_AttackTotalWindow::BranchingPointNotifyEnd, ComboCount : %d"), Character->GetComboCount());
	Character->SetWillPlayNextCombo(false);
	Character->SetIsAttacking(false);
}
