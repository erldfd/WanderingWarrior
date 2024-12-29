// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapCaptureComponent2D.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

void UMiniMapCaptureComponent2D::AddToMinimap(AActor* NewActor)
{
	ShowOnlyActors.Emplace(NewActor);
}

void UMiniMapCaptureComponent2D::AddAllActorsToMinimap()
{
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), outActors);
	ShowOnlyActors = outActors;
}

void UMiniMapCaptureComponent2D::AddAllActorsToMinimap(FExceptConditionSignature ExceptCondition)
{
	ShowOnlyActors.Empty();

	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), outActors);
	ShowOnlyActors = outActors;
	UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::AddAllActorsToMinimap"));

	ShowOnlyActors.RemoveAllSwap([&](AActor* Actor)->bool {

		if (ExceptCondition.IsBound() == false)
		{
			return false;
		}

		return ExceptCondition.Execute(Actor);

		});

	for (auto& ShowActor : ShowOnlyActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::AddAllActorsToMinimap, ActorName : %s"), *ShowActor->GetName());
	}
}

void UMiniMapCaptureComponent2D::SetUseShowOnlyActors(bool bShouldUseShowOnlyActors)
{
	UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::SetUseShowOnlyActors, bShouldUseShowOnlyActors : %d"), bShouldUseShowOnlyActors);
	if (bShouldUseShowOnlyActors)
	{
		PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	}
	else
	{
		PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	}

	//CaptureScene();
}

void UMiniMapCaptureComponent2D::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay, bCaptureEveryFrame %d, bCaptureOnMovement %d, "), bCaptureEveryFrame, bCaptureOnMovement);
	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	
	FTimerHandle TimeHandle;

	UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay"));

	GetWorld()->GetTimerManager().SetTimer(TimeHandle, FTimerDelegate::CreateLambda(
		[&]()->void 
		{
			/*for (auto& HiddenActor : HiddenActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay, BeforeHiddenActors : %s"), *HiddenActor->GetName());
			}
			
			UGameplayStatics::GetAllActorsOfClass(this, ABrush::StaticClass(), HiddenActors);

			for (auto& HiddenActor : HiddenActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay, AfterHiddenActors : %s"), *HiddenActor->GetName());
			}*/

			for (auto& ShowOnlyActor : ShowOnlyActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay, BeforeShowOnlyActor : %s"), *ShowOnlyActor->GetName());
			}
			TArray<AActor*> outActors;
			UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), outActors);
			ShowOnlyActors = outActors;
			FExceptConditionSignature ExceptCondition;
			ExceptCondition.BindLambda([](AActor* Actor)->bool {

				ABrush* Brush = Cast<ABrush>(Actor);

				return (Brush != nullptr);

				});

			AddAllActorsToMinimap(ExceptCondition);

			/*ShowOnlyActors.RemoveAllSwap([](AActor* Actor)->bool {

				ABrush* Brush = Cast<ABrush>(Actor);

				return (Brush != nullptr);

				});*/

			for (auto& ShowOnlyActor : ShowOnlyActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("UMiniMapCaptureComponent2D::BeginPlay, AfterShowOnlyActor : %s"), *ShowOnlyActor->GetName());
			}

		}), 1, false, 3);
}