// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/KickAttackAnimNotifies/ANS_KickSkillWindow.h"

#include "WWAnimInstance.h"
#include "WWEnumClassContainer.h"
#include "Components/WarriorSkillComponent.h"
#include "Character/WWCharacter.h"

void UANS_KickSkillWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_KickSkillWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_KickSkillWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	//APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	//if (Player == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, Player == nullptr"));
	//	return;
	//}

	//UWarriorSkillComponent* SkillComp = Cast<UWarriorSkillComponent>(Player->GetSkillComponenet());
	//if (SkillComp == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, SkillComp == nullptr"));
	//	return;
	//}

	//UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	//if (AnimInstance == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, AnimInstance == nullptr"));
	//	return;
	//}

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, SkillComp == nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_KickSkillWindow::NotifyEnd, AnimInstance == nullptr"));
		return;
	}

	SkillComp->SetIsChargeAttack2Started(false);
	//AnimInstance->StopAllMontages(1);
}