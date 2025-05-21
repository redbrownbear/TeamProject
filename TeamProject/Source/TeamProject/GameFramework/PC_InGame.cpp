// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Npc 인터렉트(임시 생성): 윤정
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

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(PC_InGameDataAsset->IMC, 0);
	Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Interact, 1);	
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



	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Talk,
		ETriggerEvent::Triggered, this, &ThisClass::OnTalk);
	

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
	if (m_State == EEquip_State::Shield || m_State == EEquip_State::Sword_Shield)
	{
		return;
	}
	WeaponManagerComponent->SetNextWeaponType(EWeapon_Type::Shield);

	WeaponManagerComponent->TryEquipWeapon();
}

void APC_InGame::EquipBow(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();
	EEquip_State m_State = WeaponManagerComponent->GetEquipState();
	if (m_State == EEquip_State::Bow)
	{
		return;
	}
	WeaponManagerComponent->SetNextWeaponType(EWeapon_Type::Bow);

	WeaponManagerComponent->TryEquipWeapon();
}

void APC_InGame::OnTalk(const FInputActionValue& InputActionValue)
{
	if (Npc && Npc->GetCanTalk())
	{
		if (UNpcFSMComponent* FSM = Npc->GetFSMComponent())
		{
			FSM->ChangeState(ENpcState::Talk);
			// Play Player Talk Animation
			// Limit Moving
		}
	}
	
}


