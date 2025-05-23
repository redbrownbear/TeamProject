// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"

#include "UI/NpcDialogue/NPCDialogue.h"
#include "UI/Inven/Inventory.h"

#include "Actors/Npc/Npc.h" 
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"

APC_InGame::APC_InGame()
{
	{
		static ConstructorHelpers::FObjectFinder<UPC_InGameDataAsset> Asset
		{ TEXT("/Script/TeamProject.PC_InGameDataAsset'/Game/GameFramework/IMC/PC_DataAsset.PC_DataAsset'") };
		check(Asset.Object);

		PC_InGameDataAsset = Asset.Object;
		PC_InGameDataAsset->CheckValid();
	}
	PlayerCameraManagerClass = ACM_InGame::StaticClass();


}

void APC_InGame::BeginPlay()
{
	Super::BeginPlay();

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if(UIManager)
		UIManager->PostWorldInitialize();

	ChangeInputContext(EInputContext::IC_InGame);
}

void APC_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);


	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Move,
		ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_LookMouse,
		ETriggerEvent::Triggered, this, &ThisClass::OnLook);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_LeftClick,
		ETriggerEvent::Started, this, &ThisClass::LeftClick);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_RightClick,
		ETriggerEvent::Started, this, &ThisClass::RightClick);


	// ------------ Weapon Swap -----------------
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipSword,
		ETriggerEvent::Started, this, &ThisClass::EquipSword);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipShield,
		ETriggerEvent::Started, this, &ThisClass::EquipShield);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipBow,
		ETriggerEvent::Started, this, &ThisClass::EquipBow);



	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Interact,
		ETriggerEvent::Started, this, &ThisClass::OnInteract);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Inventory,
		ETriggerEvent::Started, this, &ThisClass::OpenInventory);

}

void APC_InGame::ChangeInputContext(EInputContext NewContext)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	// 우선 기존 맵핑 제거
	Subsystem->ClearAllMappings();

	// 새 컨텍스트 적용
	switch (NewContext)
	{
	case EInputContext::IC_InGame:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_InGame, 0);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		break;

	case EInputContext::IC_Inventory:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Inventory, 1);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;

	case EInputContext::IC_Dialogue:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Dialogue, 2);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;

	}

	CurrentInputContext = NewContext;
}

void APC_InGame::BindInventoryInput(UInventory* Inventory)
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_InvenNavigate, ETriggerEvent::Started, Inventory, &UInventory::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenConfirm, ETriggerEvent::Started, Inventory, &UInventory::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenCancel, ETriggerEvent::Started, Inventory, &UInventory::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenAddItem, ETriggerEvent::Started ,Inventory, &UInventory::OnCreateItemTest);
	}
}

void APC_InGame::BindDialogueInput(UNPCDialogue* NpcDialogue)
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNavigate, ETriggerEvent::Started, NpcDialogue, &UNPCDialogue::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueConfirm, ETriggerEvent::Started, NpcDialogue, &UNPCDialogue::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueCancel, ETriggerEvent::Started, NpcDialogue, &UNPCDialogue::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNext, ETriggerEvent::Started, NpcDialogue, &UNPCDialogue::OnNextDialogue);
	}
}

void APC_InGame::OnMove(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	if(!Player_C)
	{
		return;
	}
	if (Player_C->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}
	

	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();
	if (Anim->Montage_IsPlaying(nullptr) == true) {
		Anim->Montage_Stop(0.f);
	}
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
}

void APC_InGame::OnLook(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	AddYawInput(ActionValue.X);
	AddPitchInput(-ActionValue.Y);
}

void APC_InGame::LeftClick(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);

	PlayerCharacter->LeftClickAction();

}

void APC_InGame::RightClick(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);

	PlayerCharacter->RightClickAction();
}

void APC_InGame::EquipSword(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	EEquip_State m_State = WeaponManagerComponent->GetEquipState();

	WeaponManagerComponent->SetNextWeaponType(EWeapon_Type::Sword);

	WeaponManagerComponent->TryEquipWeapon();
}

void APC_InGame::EquipShield(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	EEquip_State m_State = WeaponManagerComponent->GetEquipState();

	WeaponManagerComponent->SetNextWeaponType(EWeapon_Type::Shield);

	WeaponManagerComponent->TryEquipWeapon();
}

void APC_InGame::EquipBow(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	EEquip_State m_State = WeaponManagerComponent->GetEquipState();

	WeaponManagerComponent->SetNextWeaponType(EWeapon_Type::Bow);

	WeaponManagerComponent->TryEquipWeapon();
}


void APC_InGame::OnInteract(const FInputActionValue& InputActionValue)
{
	if (Npc && Npc->GetCanTalk())
	{
		if (UNpcFSMComponent* FSM = Npc->GetFSMComponent())
		{
			FSM->ChangeState(ENpcState::Talk);			
		}
	}	
}

void APC_InGame::OpenInventory(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	if (UIManager)
	{
		UIManager->ShowUI(UInventory::StaticClass());
	}
}

void APC_InGame::ShowDialogueUI()
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
	check(QuestManager);

	if (UIManager && QuestManager)
	{
		if (QuestManager->IsConversation())
			return;

		UIManager->ShowUI(UNPCDialogue::StaticClass());
		QuestManager->ShowDialogue(EQuestCharacter::Furiko, 0);
	}
}


