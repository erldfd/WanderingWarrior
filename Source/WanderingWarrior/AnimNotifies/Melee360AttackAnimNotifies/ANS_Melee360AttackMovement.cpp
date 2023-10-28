// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/Melee360AttackAnimNotifies/ANS_Melee360AttackMovement.h"

#include "Character/WWCharacter.h"

UANS_Melee360AttackMovement::UANS_Melee360AttackMovement()
{
	MoveSpeed = 200;
}

void UANS_Melee360AttackMovement::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	CurrentMoveSpeed = MoveSpeed;

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackMovement::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	if (Controller)
	{

		FQuat Quat = Character->GetActorQuat();
		Quat.Z = CastChecked<AActor>(Controller)->GetActorQuat().Z;
		Character->SetActorRotation(Quat);
	}

	//const FVector2D& MovementVector = Player->GetMovementVector();
	//Direction = Character->GetMovementVector();
	/*const FRotator Rotation = Character->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);*/

	/*FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Direction = ForwardDirection;
	Direction += RightDirection;*/
	//UE_LOG(LogTemp, Warning, TEXT("UANS_Melee360AttackMovement::BranchingPointNotifyBegin, Dir : %s"), *Direction.ToString());
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

	//FVector Dir(1, Direction.Y, 0);
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