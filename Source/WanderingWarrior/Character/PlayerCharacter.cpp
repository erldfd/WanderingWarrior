// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "WanderingWarrior/WWAnimInstance.h"
#include "WanderingWarrior/WWGameInstance.h"
#include "WanderingWarrior/WWGameMode.h"
#include "WanderingWarrior/Inventory/InventoryComponent.h"
#include "WanderingWarrior/Item/Weapon.h"
#include "WanderingWarrior/Item/MiscItem.h"
#include "WanderingWarrior/Components/PlayerSkillComponent.h"
#include "WanderingWarrior/Inventory/InventorySlotWidget.h"
#include "WanderingWarrior/Inventory/InventoryWidget.h"
#include "WanderingWarrior/Inventory/InventoryTabObject.h"
#include "WanderingWarrior/Inventory/InventorySlotObject.h"
#include "WanderingWarrior/WWEnumClassContainer.h"
#include "WanderingWarrior/Inventory/CharacterInventory.h"
#include "WanderingWarrior/Inventory/CharacterQuickSlot.h"

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
	check(SpringArm != nullptr);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	check(Camera != nullptr);

	SpringArm->bUsePawnControlRotation = true;//SpringArm�� ���콺���� ���Ϸ� �����̼��ϴµ�?
	SpringArm->bInheritPitch = true;//���� �����̼��ΰ� ����..... ��ư ����
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;//���콺�� �����̸� �¿�� ���������� �����̼�
	SpringArm->bDoCollisionTest = true;//���������� �ٸ��Ͱ� �浹�ϸ� �����������

	// ��Ʈ�ѷ��� �����̼��Ҷ� ī�޶� �����̼��������� �̶�׿�
	bUseControllerRotationYaw = false;//���콺 �����̸� ĳ���Ͱ� �¿�� �����̼��� z �����̼�
	bUseControllerRotationPitch = false;//���콺 �����̸� ĳ���Ͱ� ���Ϸ� �����̼� x�����̼�
	bUseControllerRotationRoll = false;// y�����̼�

	UCapsuleComponent* BodyCapsuleComponent = GetCapsuleComponent();
	check(BodyCapsuleComponent != nullptr);

	SpringArm->SetupAttachment((USceneComponent*)BodyCapsuleComponent);
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));

	PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkill"));

	Tags.Init("Player", 1);

	TempSwapSlot = NewObject<UInventorySlotObject>();

	Inventory = CreateDefaultSubobject<UCharacterInventory>(TEXT("NewInventory"));
	QuickSlot = CreateDefaultSubobject<UCharacterQuickSlot>(TEXT("NewQuickSlot"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Super::AnimInstance->OnStartNextComboDelegate.AddUObject(this, &APlayerCharacter::OnStartNextCombo);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

UPlayerSkillComponent* APlayerCharacter::GetPlayerSkillComponenet()
{
	check(PlayerSkillComponent != nullptr);
	return PlayerSkillComponent;
}

class UCharacterQuickSlot* APlayerCharacter::GetQuickSlot()
{
	check(QuickSlot != nullptr);
	return QuickSlot;
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
