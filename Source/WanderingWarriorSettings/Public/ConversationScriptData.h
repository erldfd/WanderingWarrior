// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"

#include "ConversationScriptData.generated.h"

UENUM(BlueprintType)
enum class EBehaviorIndex : uint8
{
	OpenStore,
	JustClose,
	Test,
};

USTRUCT(BlueprintType)
struct FConversationScriptDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Data")
	int32 Index;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FString> NPCScriptArray;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FString> PlayerScriptArray;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<int32> PlayerAnswerIndexArray;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<EBehaviorIndex> BehaviorArray;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIORSETTINGS_API UConversationScriptData : public UObject
{
	GENERATED_BODY()
	
public:

	UConversationScriptData();

	const TArray<FConversationScriptDataRow*>& GetConversationScriptDataRowArray() const;

private:

	TArray<FConversationScriptDataRow*> ConversationScriptDataRowArray;
};
