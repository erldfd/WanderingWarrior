// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/AN_AttackHitCheck.h"

#include "Character/WWCharacter.h"

void UAN_AttackHitCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_AttackHitCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	Character->AttackCheck();
}