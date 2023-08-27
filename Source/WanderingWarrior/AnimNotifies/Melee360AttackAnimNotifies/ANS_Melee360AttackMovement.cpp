// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/Melee360AttackAnimNotifies/ANS_Melee360AttackMovement.h"

#include "Character/PlayerCharacter.h"

UANS_Melee360AttackMovement::UANS_Melee360AttackMovement()
{
	MoveSpeed = 200;
}

void UANS_Melee360AttackMovement::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	CurrentMoveSpeed = MoveSpeed;

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackMovement::BranchingPointNotifyBegin, Player == nullptr"));
		return;
	}

	//const FVector2D& MovementVector = Player->GetMovementVector();
	Direction = Player->GetMovementVector();
	const FRotator Rotation = Player->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	/*FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Direction = ForwardDirection;
	Direction += RightDirection;*/
	UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackMovement::BranchingPointNotifyBegin, Dir : %s"), *Direction.ToString());
}

void UANS_Melee360AttackMovement::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackMovementWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	FVector Dir(1, Direction.Y, 0);
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

void UANS_Melee360AttackMovement::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
}