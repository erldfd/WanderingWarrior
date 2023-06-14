// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WWCharacter.h"

#include "NPCCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API ANPCCharacter : public AWWCharacter
{
	GENERATED_BODY()
	
public:

	ANPCCharacter();

	int32 GetConversationIndex() const;
	void SetConversationIndex(const int32& InIndex);

	const FString& GetNPCName() const;

	class UMarchantInventory& GetInventory();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<class UMarchantInventory> Inventory;

	UPROPERTY(EditAnywhere)
	int32 ConversationIndex;

	UPROPERTY(EditAnywhere)
	FString NPCName;
};
