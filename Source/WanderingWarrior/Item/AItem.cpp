// Fill out your copyright notice in the Description page of Project Settings.


#include "AItem.h"

#include "WWGameInstance.h"

//#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
// Sets default values
AAItem::AAItem() : MaxItemCount(1)
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

	if (bIsRotatingOnField)
	{
		static double Rad = 0;

		if (DOUBLE_BIG_NUMBER - Rad <= DOUBLE_KINDA_SMALL_NUMBER)
		{
			Rad = 0;
		}
		else
		{
			Rad += 0.001;
		}

		FVector Location = GetActorLocation();
		static const double LocationZ = Location.Z;

		Location.Z = LocationZ + FMath::Sin(Rad) * 10;

		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += 0.2;

		SetActorLocationAndRotation(Location, Rotation);
	}
}

const FText& AAItem::GetItemName()
{
	return ItemName;
}

void AAItem::SetItemName(const FText& NewItemName)
{
	ItemName = NewItemName;
}

UTexture2D* AAItem::GetItemSlotTexture()
{
	return ItemSlotTexture;
}

void AAItem::SetItemSlotTexture(UTexture2D& NewTexture)
{
	ItemSlotTexture = &NewTexture;
}

int AAItem::GetMaxItemCount()
{
	return MaxItemCount;
}

void AAItem::SetMaxItemCount(int32 Count)
{
	MaxItemCount = FMath::Clamp(Count, 0, 1000);//임의로 최대 1000...
}

void AAItem::SetItemType(EItemType NewItemType)
{
	ItemType = NewItemType;
}

int32 AAItem::GetBuyPrice()
{
	return BuyPrice;
}

int32 AAItem::GetSellPrice()
{
	return SellPrice;
}

void AAItem::SetbisRotatingOnField(bool IsRotating)
{
	bIsRotatingOnField = IsRotating;
}

bool AAItem::GetbIsFieldItem()
{
	return bIsFieldItem;
}

void AAItem::SetbIsFieldItem(bool IsFieldItem)
{
	bIsFieldItem = IsFieldItem;
}