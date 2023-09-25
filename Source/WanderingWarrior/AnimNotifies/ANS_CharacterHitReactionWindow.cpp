// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_CharacterHitReactionWindow.h"

#include "WWAnimInstance.h"
void UANS_CharacterHitReactionWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_CharacterHitReactionWindow::BranchingPointNotifyBegin, AnimInstance == nullptr"));
		return;
	}

	AnimInstance->SetIsPlayingCharacterHitMontage(true);
}

void UANS_CharacterHitReactionWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_CharacterHitReactionWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_CharacterHitReactionWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	AnimInstance->SetIsPlayingCharacterHitMontage(false);
	UE_LOG(LogTemp, Warning, TEXT("UANS_CharacterHitReactionWindow::BranchingPointNotifyEnd, SetIsPlayingCharacterHitMontage(false)"));
}
