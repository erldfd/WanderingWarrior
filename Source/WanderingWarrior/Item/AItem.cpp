// Fill out your copyright notice in the Description page of Project Settings.


#include "AItem.h"

#include "WanderingWarrior/WWGameInstance.h"

//#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
// Sets default values
AAItem::AAItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AAItem::GetItemName()
{
	return ItemName;
}

void AAItem::SetItemName(FName NewItemName)
{
	ItemName = NewItemName;
}

UTexture2D* AAItem::GetItemSlotTexture()
{
	return ItemSlotTexture;
}

void AAItem::SetItemSlotTexture(UTexture2D* NewTexture)
{
	ItemSlotTexture = NewTexture;
}

int AAItem::GetMaxItemCount()
{
	return MaxItemCount;
}

void AAItem::SetMaxItemCount(int Count)
{
	MaxItemCount = FMath::Clamp(Count, 0, 1000);//임의로 최대 1000...
}

void AAItem::SetItemType(EItemType NewItemType)
{
	ItemType = NewItemType;
}