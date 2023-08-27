// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SkillPlayWindow.h"

//#include "Interface/SkillOwnerInterface.h"
#include "WWAnimInstance.h"

void UAnimNotifyState_SkillPlayWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UAnimNotifyState_SkillPlayWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_SkillPlayWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(MeshComp->GetAnimInstance());

	if (AnimInstance == nullptr)
	{
		return;
	}
	
	/*ISkillOwnerInterface* SkillOwner = Cast<ISkillOwnerInterface>(AnimInstance->GetOwningActor());

	if (SkillOwner == nullptr)
	{
		return;
	}

	SkillOwner->SetIsSkillStarted(false);
	AnimInstance->StopAllMontages(0);*/
}
