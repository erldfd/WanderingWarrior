// Fill out your copyright notice in the Description page of Project Settings.


#include "WWCharacter.h"

// Sets default values
AWWCharacter::AWWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

