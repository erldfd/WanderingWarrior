// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_ForTest.h"

void UANS_ForTest::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//EventReference.GetNotify()->Notify->
	UE_LOG(LogTemp, Warning, TEXT("UANS_ForTest::NotifyBegin"));
}

void UANS_ForTest::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_ForTest::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("UANS_ForTest::NotifyEnd"));
}

void UANS_ForTest::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	//Super::BranchingPointNotifyBegin(BranchingPointPayload);
	//UE_LOG(LogTemp, Warning, TEXT("UANS_ForTest::BranchingPointNotifyBegin"), BranchingPointPayload.bReachedEnd);
}

void UANS_ForTest::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);
}

void UANS_ForTest::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	//Super::BranchingPointNotifyEnd(BranchingPointPayload);
	//UE_LOG(LogTemp, Warning, TEXT("UANS_ForTest::BranchingPointNotifyEnd, Reached : %d"), BranchingPointPayload.bReachedEnd);
}
