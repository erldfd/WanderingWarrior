// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "WWAnimInstance.h"
#include "WWGameInstance.h"
#include "WWGameMode.h"
#include "InventoryComponent.h"
#include "Weapon.h"
#include "MiscItem.h"
#include "PlayerSkillComponent.h"
//#include "QuickSlotComponent.h"
#include "InventorySlotWidget.h"
#include "InventoryWidget.h"
#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "QuickSlotWidget.h"
#include "WWEnumClassContainer.h"
#include "CharacterInventory.h"
#include "CharacterQuickSlot.h"

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
	check(BodyCapsuleComponent != nullptr);

	SpringArm->SetupAttachment((USceneComponent*)BodyCapsuleComponent);
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));

	static ConstructorHelpers::FClassFinder<AWeapon> Weapon(TEXT("/Game/Weapon/BP_BlackSword.BP_BlackSword_C"));
	if (Weapon.Succeeded())
	{
		WeaponClass = Weapon.Class;
	}

	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkill"));
	//QuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlot"));

	Tags.Init("", 1);
	Tags[0] = TEXT("Player");

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

	TArray<UInventorySlotWidget*> WeaponTabWidgetArray = Inventory->GetInventoryWidget()->GetSlotWidgetArray(ETabType::WeaponTab);
	TArray<UInventorySlotWidget*> MiscTabWidgetArray = Inventory->GetInventoryWidget()->GetSlotWidgetArray(ETabType::MiscTab);
	TArray<UInventorySlotWidget*> QuickSlotWidgetArray = QuickSlot->GetInventoryWidget()->GetSlotWidgetArray(ETabType::QuickSlotTab);
	for (int i = 0; i < WeaponTabWidgetArray.Num(); ++i)
	{
		if (WeaponTabWidgetArray.IsValidIndex(i) == false ||
			WeaponTabWidgetArray[i] == nullptr)
		{
			continue;
		}

		WeaponTabWidgetArray[i]->OnDragDropDelegate.BindUObject(this, &APlayerCharacter::OnDragDropInventorySlot);
	}

	for (int i = 0; i < MiscTabWidgetArray.Num(); ++i)
	{
		if (MiscTabWidgetArray.IsValidIndex(i) == false ||
			MiscTabWidgetArray[i] == nullptr)
		{
			continue;
		}

		MiscTabWidgetArray[i]->OnDragDropDelegate.BindUObject(this, &APlayerCharacter::OnDragDropInventorySlot);
	}

	for (int i = 0; i < QuickSlotWidgetArray.Num(); ++i)
	{
		if (QuickSlotWidgetArray.IsValidIndex(i) == false ||
			QuickSlotWidgetArray[i] == nullptr)
		{
			continue;
		}

		QuickSlotWidgetArray[i]->OnDragDropDelegate.BindUObject(this, &APlayerCharacter::OnDragDropInventorySlot);
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

void APlayerCharacter::OnDragDropInventorySlot(int DragStartSlotIndex, int DragEndSlotIndex, int DragStartSlotTabType, int DragEndSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("Start : %d, End : %d, StartTab : %d, EndTab : %d"), DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType);
	ExchangeOrMoveSlotItem(DragStartSlotIndex, DragEndSlotIndex, (ETabType)DragStartSlotTabType, (ETabType)DragEndSlotTabType);
}

void APlayerCharacter::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragStartSlotTabType, ETabType DragEndSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

	UInventorySlotObject* DragStartSlot;
	UInventorySlotObject* DragEndSlot;

	TArray<UInventorySlotWidget*> DragStartSlotWidgetArray;
	TArray<UInventorySlotWidget*> DragEndSlotWidgetArray;

	UInventoryWidget* InventoryWidget = Inventory->GetInventoryWidget();
	UInventoryWidget* QuickSlotWidget = QuickSlot->GetInventoryWidget();

	if (DragStartSlotTabType == ETabType::WeaponTab || DragStartSlotTabType == ETabType::MiscTab)
	{
		DragStartSlot = Inventory->GetInventoryComponent()->GetTabArray()[(int)DragStartSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
		DragStartSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragStartSlotTabType);
	}
	else
	{
		DragStartSlot = QuickSlot->GetInventoryComponent()->GetTabArray()[(int)DragStartSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
		DragStartSlotWidgetArray = QuickSlotWidget->GetSlotWidgetArray(DragStartSlotTabType);
	}
	
	if (DragEndSlotTabType == ETabType::WeaponTab || DragEndSlotTabType == ETabType::MiscTab)
	{
		DragEndSlot = Inventory->GetInventoryComponent()->GetTabArray()[int(DragEndSlotTabType)]->GetSlotFromIndex(DragEndSlotIndex);
		DragEndSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragEndSlotTabType);
	}
	else
	{
		DragEndSlot = QuickSlot->GetInventoryComponent()->GetTabArray()[(int)DragStartSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
		DragEndSlotWidgetArray = QuickSlotWidget->GetSlotWidgetArray(DragEndSlotTabType);
	}

	check(DragStartSlot != nullptr);
	check(DragEndSlot != nullptr);
	check(DragStartSlotWidgetArray.IsValidIndex(DragStartSlotIndex));
	check(DragEndSlotWidgetArray.IsValidIndex(DragEndSlotIndex));

	TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
	TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());
	int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();

	if (DragEndSlot->GetHeldItemCount() == 0)
	{
		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

		DragStartSlot->SetHeldItemCount(0);
		//DragStartSlot->SetSlotItemData(nullptr);

		if (DragStartSlotTabType == ETabType::WeaponTab || DragStartSlotTabType == ETabType::MiscTab)
		{
			InventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex);
			InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
		}
		else
		{
			QuickSlotWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex);
			QuickSlotWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
		}

		if (DragEndSlotTabType == ETabType::WeaponTab || DragEndSlotTabType == ETabType::MiscTab)
		{
			InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
			InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
		}
		else
		{
			QuickSlotWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
			QuickSlotWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
		}
		
		DragStartSlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
		DragEndSlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
	}
	else
	{
		DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
		DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());
		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

		if (DragStartSlotTabType == ETabType::WeaponTab || DragStartSlotTabType == ETabType::MiscTab)
		{
			InventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
			InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
		}
		else
		{
			QuickSlotWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
			QuickSlotWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
		}

		if (DragEndSlotTabType == ETabType::WeaponTab || DragEndSlotTabType == ETabType::MiscTab)
		{
			InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
			InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
		}
		else
		{
			QuickSlotWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
			QuickSlotWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
		}
	}
}