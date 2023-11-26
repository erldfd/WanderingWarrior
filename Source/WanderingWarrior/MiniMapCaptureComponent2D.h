// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneCaptureComponent2D.h"

#include "MiniMapCaptureComponent2D.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FExceptConditionSignature, AActor*);
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WANDERINGWARRIOR_API UMiniMapCaptureComponent2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
public:

	void AddToMinimap(AActor* NewActor);
	void AddAllActorsToMinimap();
	void AddAllActorsToMinimap(FExceptConditionSignature ExceptCondition);
	void SetUseShowOnlyActors(bool bShouldUseShowOnlyActors);

protected:

	virtual void BeginPlay() override;
};
