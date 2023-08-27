// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/ANS_AttackMovementWindow.h"

#include "GameFramework/Character.h"

UANS_AttackMovementWindow::UANS_AttackMovementWindow()
{
	MoveSpeed = 100;
}

void UANS_AttackMovementWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	CurrentMoveSpeed = MoveSpeed;
}

void UANS_AttackMovementWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackMovementWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	FHitResult Hit;
	Character->AddActorWorldOffset(Character->GetActorForwardVector() * CurrentMoveSpeed * FrameDeltaTime, true, &Hit);

	if (Hit.bBlockingHit)
	{
		CurrentMoveSpeed = 0;
	}
	else
	{
		CurrentMoveSpeed = MoveSpeed;
	}
}

void UANS_AttackMovementWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
}