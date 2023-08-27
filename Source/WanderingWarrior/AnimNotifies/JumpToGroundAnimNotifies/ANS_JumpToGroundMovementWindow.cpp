// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/JumpToGroundAnimNotifies/ANS_JumpToGroundMovementWindow.h"

#include "GameFramework/Character.h"

UANS_JumpToGroundMovementWindow::UANS_JumpToGroundMovementWindow()
{
	MoveSpeed = 5;
}

void UANS_JumpToGroundMovementWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CurrentMoveSpeed = MoveSpeed;
}

void UANS_JumpToGroundMovementWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_JumpToGroundMovementWindow::NotifyTick, Character == nullptr"));
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

void UANS_JumpToGroundMovementWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}