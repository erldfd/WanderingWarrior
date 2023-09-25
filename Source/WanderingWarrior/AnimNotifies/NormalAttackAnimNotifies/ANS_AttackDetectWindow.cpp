// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/NormalAttackAnimNotifies/ANS_AttackDetectWindow.h"

#include "Character/WWCharacter.h"
#include "Item/Weapon.h"
#include "WWAnimInstance.h"
#include "WWGameMode.h"

#include "Kismet/GameplayStatics.h"

void UANS_AttackDetectWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackDetectWindow::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	AWeapon* CurrentWeapon = Character->GetCurrentWeapon();
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->SetBoxComponentCollision(ECollisionEnabled::QueryOnly);
}

void UANS_AttackDetectWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	//USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
}

void UANS_AttackDetectWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	/*AWWGameMode* GameMode = Cast<AWWGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackDetectWindow::BranchingPointNotifyEnd, GameMode == nullptr"));
		return;
	}
	UWWAnimInstance* AnimInstance = GameMode->GetPlayerAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackDetectWindow::BranchingPointNotifyEnd, AnimInstance == nullptr"));
		return;
	}

	AnimInstance->OnInitIsDamaged.Broadcast();*/

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_AttackDetectWindow::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	AWeapon* CurrentWeapon = Character->GetCurrentWeapon();
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->SetBoxComponentCollision(ECollisionEnabled::NoCollision);
}