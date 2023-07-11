// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "WWAnimInstance.h"
#include "WWGameInstance.h"
#include "WWGameMode.h"
#include "WWEnumClassContainer.h"
#include "WWConstContainer.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "Components/PlayerSkillComponent.h"
#include "Components/CharacterStatComponent.h"
#include "Inventory/InventorySlotWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryTabData.h"
#include "Inventory/InventorySlotData.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/CharacterQuickSlot.h"
#include "Inventory/InventoryComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	check(SpringArm);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	check(Camera);

	SpringArm->bUsePawnControlRotation = true;//SpringArm이 마우스따라 상하로 로테이션하는듯?
	SpringArm->bInheritPitch = true;//로컬 로테이션인가 뭐지..... 암튼 상하
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;//마우스가 움직이면 좌우로 스프링암이 로테이션
	SpringArm->bDoCollisionTest = true;//스프링암이 다른것과 충돌하면 통과하지않음

	// 컨트롤러가 로테이션할때 카메라가 로테이션하지않음 이라네요
	bUseControllerRotationYaw = false;//마우스 움직이면 캐릭터가 좌우로 로테이션함 z 로테이션
	bUseControllerRotationPitch = false;//마우스 움직이면 캐릭터가 상하로 로테이션 x로테이션
	bUseControllerRotationRoll = false;// y로테이션

	UCapsuleComponent* BodyCapsuleComponent = GetCapsuleComponent();
	check(BodyCapsuleComponent);

	SpringArm->SetupAttachment((USceneComponent*)BodyCapsuleComponent);
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));

	PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkill"));

	Tags.Init("Player", 1);

	Inventory = CreateDefaultSubobject<UCharacterInventory>(TEXT("NewInventory"));
	QuickSlot = CreateDefaultSubobject<UCharacterQuickSlot>(TEXT("NewQuickSlot"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TempSwapSlot = NewObject<UInventorySlotData>(this);

	Super::AnimInstance->OnStartNextComboDelegate.AddUObject(this, &APlayerCharacter::OnStartNextCombo);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < SlotCount::WEAPON_TAB_SLOT_COUNT; ++i)
	{
		Inventory->RemoveAllItem(i);
	}
	
	Inventory->ObtainItem(EWeaponName::BlackSword);
	Inventory->ObtainItem(EWeaponName::WhiteSword);
	
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);

	QuickSlot->ObtainItem(EMiscItemName::HPPotion);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter PossessedBy"));

	Cast<AWWGameMode>(GetWorld()->GetAuthGameMode())->SetPlayerAnimInstance(AnimInstance);
	//Super::EquipWeapon(Cast<UWWGameInstance>(GetGameInstance())->GetWeapon(EWeaponName::BlackSword));
}

UPlayerSkillComponent& APlayerCharacter::GetPlayerSkillComponenet()
{
	check(PlayerSkillComponent);
	return *PlayerSkillComponent;
}

class UCharacterQuickSlot& APlayerCharacter::GetQuickSlot()
{
	check(QuickSlot);
	return *QuickSlot;
}

UCharacterInventory& APlayerCharacter::GetInventory() const
{
	return *Inventory;
}

void APlayerCharacter::Attack(float Value)
{
	if (Inventory->IsInventoryVisible())
	{
		return;
	}

	Super::Attack(Value);
}

void APlayerCharacter::OnStartNextCombo()
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector LookVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * Super::InputForwardValue + FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * Super::InputRightValue;

	if (LookVector == FVector::ZeroVector)
	{
		LookVector = GetActorForwardVector();
	}

	SetActorRotation(FRotationMatrix::MakeFromX(LookVector).Rotator());
}
