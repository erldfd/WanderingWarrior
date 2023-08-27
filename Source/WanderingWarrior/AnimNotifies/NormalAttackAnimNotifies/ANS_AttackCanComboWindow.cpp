// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/ANS_AttackCanComboWindow.h"

#include "Character/WWCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Components/WarriorSkillComponent.h"

#include "Camera/CameraComponent.h"

void UANS_AttackCanComboWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackCanComboWindow::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	Character->SetCanCombo(true);
}

void UANS_AttackCanComboWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	//USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
}

void UANS_AttackCanComboWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	
	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackCanComboWindow::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackCanComboWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);

	Character->SetCanCombo(false);
	
	if (Character->GetWillPlayNextCombo())
	{
		int32 ComboCount = Character->GetComboCount();
		FName SectionName = FName(*FString::Printf(TEXT("Section%d"), ComboCount));

		UAnimMontage* AttackMontage = Character->GetAttackMontage();
		if (AttackMontage == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UANS_AttackCanComboWindow::BranchingPointNotifyEnd, AttackMontage == nullptr"));
			return;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);

		Character->SetComboCount(++ComboCount);
		Character->SetWillPlayNextCombo(false);
	}
	else if (Character->GetWillPlayChargeAttack2())
	{
		Character->PlayChargeAttack2();

		Character->SetComboCount(0);
		Character->SetCanCombo(false);
		Character->SetWillPlayChargeAttack2(false);
		Character->SetIsAttacking(false);
	}
	else if (Character->GetWillPlayChargeAttack3())
	{
		Character->PlayChargeAttack3();

		Character->SetComboCount(0);
		Character->SetCanCombo(false);
		Character->SetWillPlayChargeAttack3(false);
		Character->SetIsAttacking(false);
	}

	if (Player == nullptr)
	{
		return;
	}

	UCameraComponent* PlayerCamera = Player->GetCamera();
	if (PlayerCamera)
	{
		FRotator Rotator = PlayerCamera->GetComponentRotation();
		Rotator.Pitch = Player->GetActorRotation().Pitch;
		Player->SetActorRotation(Rotator);
	}
}