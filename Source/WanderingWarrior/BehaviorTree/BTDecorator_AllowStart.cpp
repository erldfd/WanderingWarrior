// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_AllowStart.h"

#include "Kismet/GameplayStatics.h"
#include "WWGameInstance.h"

bool UBTDecorator_AllowStart::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("wStart::CalculateRawConditionValue, GameInstance == nullptr"));
		return false;
	}

	return GameInstance->AllowAIStart;
}