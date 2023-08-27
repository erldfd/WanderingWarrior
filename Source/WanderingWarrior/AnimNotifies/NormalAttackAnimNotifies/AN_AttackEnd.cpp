// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/AN_AttackEnd.h"

#include "Character/WWCharacter.h"

void UAN_AttackEnd::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_AttackEnd::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_AttackEnd::BranchingPointNotify, AnimInstance == nullptr"));
		return;
	}

	AnimInstance->StopAllMontages(1);
	Character->SetComboCount(0);
	UE_LOG(LogTemp, Warning, TEXT("UAN_AttackEnd::BranchingPointNotify, ComboCount : %d"), Character->GetComboCount());
	Character->SetWillPlayNextCombo(false);
	Character->SetIsAttacking(false);
}