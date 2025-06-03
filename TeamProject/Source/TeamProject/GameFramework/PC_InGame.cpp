// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "UI/HUD/MainHUD.h"
#include "SubSystem/PlayerManager.h"

#include "Actors/Npc/Npc.h" 
#include "Components/Character/PlayerMovementComponent.h"

#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"


#include "Actors/Temple/Ice/IcePillar.h"
#include "Actors/Temple/Ice/IcePreview.h"

#include "Actors/Temple/Treasure/TreasureBox.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Actors/Object/MetalActor.h"

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


	{
		IcePillarClass = AIcePillar::StaticClass();
		IcePreviewClass = AIcePreview::StaticClass();
	}

	{
		MetalActorClass = AMetalActor::StaticClass();
	}
}

void APC_InGame::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputContext(EInputContext::IC_InGame);

	if (IcePreviewClass && !IcePreviewActor)
	{
		IcePreviewActor = GetWorld()->SpawnActor<AIcePreview>(IcePreviewClass);
		if (IcePreviewActor)
		{
			IcePreviewActor->SetActorEnableCollision(false);
		}
	}

	if (MetalActorClass && !MetalActor)
	{
		MetalActor = GetWorld()->SpawnActor<AMetalActor>(MetalActorClass);
		if (MetalActor)
		{
			MetalActor->SetActorEnableCollision(false);
		}
	}

	if (!PhysicsHandle)
	{
		PhysicsHandle = NewObject<UPhysicsHandleComponent>(this, UPhysicsHandleComponent::StaticClass(), TEXT("PhysicsHandle"));
		if (PhysicsHandle)
		{
			PhysicsHandle->RegisterComponent();  
			PhysicsHandle->SetIsReplicated(false);  
		}
	}
}

void APC_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);


	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Move,
		ETriggerEvent::Triggered, this, &ThisClass::OnMove);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Move,
		ETriggerEvent::Completed, this, &ThisClass::OnMoveCancel);



	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_LookMouse,
		ETriggerEvent::Triggered, this, &ThisClass::OnLook);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Jump,
		ETriggerEvent::Started, this, &ThisClass::JumpGlide);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Dash,
		ETriggerEvent::Started, this, &ThisClass::StartedDash);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Dash,
		ETriggerEvent::Completed, this, &ThisClass::CompletedDash);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Crouch,
		ETriggerEvent::Triggered, this, &ThisClass::OnCrouch);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Crouch,
		ETriggerEvent::Completed, this, &ThisClass::OnUnCrouch);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_LeftClick,
		ETriggerEvent::Started, this, &ThisClass::LeftClick);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_RightClick,
		ETriggerEvent::Triggered, this, &ThisClass::RightClick);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_RightClick,
		ETriggerEvent::Completed, this, &ThisClass::RightClickEnd);



	// ------------ Weapon Swap -----------------
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipSword,
		ETriggerEvent::Started, this, &ThisClass::EquipSword);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipShield,
		ETriggerEvent::Started, this, &ThisClass::EquipShield);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_EquipBow,
		ETriggerEvent::Started, this, &ThisClass::EquipBow);
	// ------------ Climb ------------------------

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Climb,
		ETriggerEvent::Started, this, &ThisClass::Climb);


	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Interact,
		ETriggerEvent::Started, this, &ThisClass::OnInteract);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Inventory,
		ETriggerEvent::Started, this, &ThisClass::OpenInventory);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Quest,
		ETriggerEvent::Started, this, &ThisClass::OpenQuest);

	// ------------ SuperPower -----------------
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_IceMaker, 
		ETriggerEvent::Started, this, &ThisClass::BeginIcePreview);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Magnesis, 
		ETriggerEvent::Started, this, &ThisClass::ShowMetalActorPreview);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_TrySuperPower, 
		ETriggerEvent::Started, this, &ThisClass::TrySuperPower);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_ControlDistance, 
		ETriggerEvent::Triggered, this, &ThisClass::OnControlDistance);
}

void APC_InGame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

	case EInputContext::IC_Shop:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Shop, 3);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;

	case EInputContext::IC_Quest:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Quest, 4);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;

	}

	CurrentInputContext = NewContext;
}

void APC_InGame::BindInventoryInput()
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_InvenNavigate, ETriggerEvent::Started, this, &APC_InGame::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenConfirm, ETriggerEvent::Started, this, &APC_InGame::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenCancel, ETriggerEvent::Started, this, &APC_InGame::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_InvenAddItem, ETriggerEvent::Started, this, &APC_InGame::OnCreateItemTest);
	}
}

void APC_InGame::BindDialogueInput()
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNavigate, ETriggerEvent::Started, this, &APC_InGame::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueConfirm, ETriggerEvent::Started, this, &APC_InGame::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueCancel, ETriggerEvent::Started, this, &APC_InGame::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNext, ETriggerEvent::Started, this, &APC_InGame::OnNextDialogue);
	}
}

void APC_InGame::BindShopInput()
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNavigate, ETriggerEvent::Started, this, &APC_InGame::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueConfirm, ETriggerEvent::Started, this, &APC_InGame::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueCancel, ETriggerEvent::Started, this, &APC_InGame::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNext, ETriggerEvent::Started, this, &APC_InGame::OnNextDialogue);
	}
}

void APC_InGame::BindQuestInput()
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNavigate, ETriggerEvent::Started, this, &APC_InGame::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueConfirm, ETriggerEvent::Started, this, &APC_InGame::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueCancel, ETriggerEvent::Started, this, &APC_InGame::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_DialogueNext, ETriggerEvent::Started, this, &APC_InGame::OnNextDialogue);
	}
}

void APC_InGame::OnMove(const FInputActionValue& InputActionValue)
{

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	if (!Player_C)
	{
		return;
	}
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
	if (Movement->MovementMode == MOVE_None)
	{
		return;
	}
	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);

	// 클라이밍 상태일 때의 캐릭터 무브
	if (Movement->IsClimbing())
	{
		if (Movement->GetClimbMode() == EClimb_State::Land)
		{
			return;
		}
	
		FHitResult HitResult;
		


		FVector Normal_Vec = HitResult.Normal;

		

		const FRotator Rotation = Player_C->K2_GetActorRotation();
		const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
		
		const FVector UpVector = UKismetMathLibrary::GetUpVector(Rotation);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(Rotation);


		P_Anim->ActionValue = ActionValue;

		APawn* ControlledPawn = GetPawn();
		ControlledPawn->AddMovementInput(UpVector, ActionValue.X);
		ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

		Movement->TrySetMoveClimb(ActionValue);

		UE_LOG(LogTemp, Warning, TEXT("Climbing"));

		
	}
	else if (Movement->bIsGliding)
	{
		const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

		P_Anim->ActionValue = ActionValue;
		
		Movement->GlidingMove(ActionValue);
		UE_LOG(LogTemp, Warning, TEXT("Gliding"));
	}
	
	// 노말 상태일 때의 캐릭터 무브
	else
	{

		const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

		P_Anim->ActionValue = ActionValue;

		const FRotator Rotation = K2_GetActorRotation();
		const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);


		

		APawn* ControlledPawn = GetPawn();
		ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
		ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

	}
	
}

void APC_InGame::OnMoveCancel(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	if (!Player_C)
	{
		return;
	}
	if (Player_C->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
	if (Movement->IsClimbing())
	{
		Movement->Velocity = FVector::ZeroVector;
	}


	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);

	const FVector2D ActionValue = FVector2D::Zero();

	UE_LOG(LogTemp, Warning, TEXT("ActionValue %f, %f"), ActionValue.X, ActionValue.Y);
	P_Anim->ActionValue = ActionValue;
}

void APC_InGame::JumpGlide(const FInputActionValue& InputActionValue)
{
	ACharacter* Player_C = Cast<ACharacter>(GetPawn());
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
	if (Movement->IsFalling())
	{
		Movement->SetGlideMode(true);
	}
	else
	{
		Player_C->Jump();
	}
}

void APC_InGame::StartedDash(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	Player_C->GetCharacterMovement()->MaxWalkSpeed = PLAYER_MOVE_DASH;

}

void APC_InGame::CompletedDash(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	Player_C->GetCharacterMovement()->MaxWalkSpeed = PLAYER_MOVE_NML;
}

void APC_InGame::OnLook(const FInputActionValue& InputActionValue)
{
	if (bIsCameraLocked)
	{
		return; 
	}

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());

	if (Player_C->GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		return;
	}
	
	
	AddYawInput(ActionValue.X);
	AddPitchInput(-ActionValue.Y);
	
	if (!Player_C)
	{
		return;
	}
	if (Player_C->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}


	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);

	P_Anim->SetPitch();

}

void APC_InGame::LeftClick(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);

	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();

	if (!WeaponManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponManagerComponent is Null"));
		return;
	}


	WeaponManagerComponent->LeftClickAction();

}

void APC_InGame::RightClick(const FInputActionValue& InputActionValue)
{

	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);

	UWeaponManagerComponent* WeaponManagerComponent = PlayerCharacter->GetWeaponManagerComponent();

	if (!WeaponManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponManagerComponent is Null"));
		return;
	}

	WeaponManagerComponent->RightClickAction();

}

void APC_InGame::RightClickEnd(const FInputActionValue& InputActionValue)
{

	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(PlayerPawn);

	UWeaponManagerComponent* WeaponManagerComponent = Player_C->GetWeaponManagerComponent();


	if (!WeaponManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponManagerComponent is Null"));
		return;
	}

	WeaponManagerComponent->RightClickEnd();


}

void APC_InGame::Climb(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	
	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

	
	if (Movement->IsClimbing())
	{
		Movement->SetClimbMode(false);
	}

	else
	{	
		if (Movement->bIsGliding)
		{
			return;
		}
		FHitResult HitResult;
		if (Movement->ClimbingLineTrace(HitResult))
		{
			Movement->SetClimbMode(true);
		}
	}
}

void APC_InGame::OnCrouch(const FInputActionValue& InputActionValue)
{
	
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter->GetMovementComponent()->IsFalling()) { return; }
	ControlledCharacter->Crouch();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ControlledCharacter->GetCharacterMovement()->GetMovementName());
}

void APC_InGame::OnUnCrouch(const FInputActionValue& InputActionValue)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	ControlledCharacter->UnCrouch();
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
			if(Npc->GetData()->DialogType == EDialogType::Shop)
			{
				FSM->ChangeState(ENpcState::Sell);
			}
			else
			{
				FSM->ChangeState(ENpcState::Talk);
			}			
		}
	}

	if (TreasureBox && TreasureBox->GetOpenBox())
	{
		TreasureBox->OpenTBox();
	}
}

void APC_InGame::OpenInventory(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->ShowUI(UInventory::StaticClass());
	}

	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		PlayerManager->ShowInvenUI();
	}
}

void APC_InGame::OpenQuest(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->ShowUI(UQuest::StaticClass());
	}

	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		PlayerManager->ShowQuestUI();
	}
}

void APC_InGame::TrySuperPower(const FInputActionValue& InputActionValue)
{
	if (bIceKeyPressed)
	{
		if (!IcePillarClass) return;

		if (bCanSpawn)
		{
			SpawnIcePillar();
		}
	}

	if (bMagnesisKeyPressed)
	{
		if (IsHoldingObject()) return;

		if (!MetalActorClass) return;

		CheckMetalActor();

		if (bCanControlMetal)
		{
			Magnesis();
		}
	}
}

void APC_InGame::OnControlDistance(const FInputActionValue& InputActionValue)
{
	if (!bIceKeyPressed && !bMagnesisKeyPressed) return;

	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	if (InputValue.IsNearlyZero()) return;

	const float MoveStep = 10.f;

	if (bMagnesisKeyPressed)
	{		
		const float DistanceStep = 20.f; // HoldDistance 변화량
		const float MinHoldDistance = 100.f; // 너무 가까워지는 것 방지
		const float MaxHoldDistance = 2000.f; // 너무 멀어지지 않도록 제한

		// Magnesis 상태일 경우, HoldDistance 조절
		if (IsHoldingObject())
		{
			// 위로 누르면 증가, 아래로 누르면 감소
			HoldDistance += InputValue.Y * DistanceStep;
			HoldDistance = FMath::Clamp(HoldDistance, MinHoldDistance, MaxHoldDistance);

			//UE_LOG(LogTemp, Warning, TEXT("HoldDistance: %f"), HoldDistance);
			return; // IcePreview 조작은 생략
		}
	}
	else if (bIceKeyPressed)
	{
		// 카메라 기준 방향 추출
		FVector CamLoc;
		FRotator CamRot;
		GetPlayerViewPoint(CamLoc, CamRot);

		// 화면 기준 방향 (Pitch는 제거 → 수평 평면 유지)
		FRotator YawOnlyRot(0.0f, CamRot.Yaw, 0.0f);
		FVector Forward = FRotationMatrix(YawOnlyRot).GetUnitAxis(EAxis::X); // 화면 앞
		FVector Right = -FRotationMatrix(YawOnlyRot).GetUnitAxis(EAxis::Y); // 화면 오른쪽

		// 이동 방향 계산
		FVector MoveDirection = Forward * InputValue.Y + Right * InputValue.X;
		MoveDirection.Z = 0.0f;

		if (MoveDirection.IsNearlyZero()) return;

		if (bIceMaker)
		{
			IcePreviewActor->AddActorWorldOffset(MoveDirection.GetSafeNormal() * MoveStep);
			//UE_LOG(LogTemp, Warning, TEXT("X=%f Y=%f"), InputValue.X, InputValue.Y);
		}
	}	
}

void APC_InGame::SpawnIcePillar()
{	
	if (!LastHit.IsValidBlockingHit()) return;

	FVector SpawnLoc = LastHit.Location;
	FVector Normal = LastHit.Normal;

	AIcePillar* IcePillarActor = GetWorld()->SpawnActor<AIcePillar>(IcePillarClass, SpawnLoc, FRotationMatrix::MakeFromZ(Normal).Rotator());

	if (!IcePillarActor) return;

	IcePillarActor->SetRiseDirection(Normal);
	IcePillarActor->SetPivotLocation(SpawnLoc);

	IcePillarActor->SetActorHiddenInGame(false);

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	Player_C->ZoomOut();

	IcePreviewActor->SetActorHiddenInGame(true);
	SetIgnoreLookInput(false);

	bIcePreviewPlaced = false; // 해제되면 다시 재탐색 가능
	bCanSpawn = false;
	bIceMaker = false;
	bIceKeyPressed = false;
}

void APC_InGame::BeginIcePreview(const FInputActionValue& InputActionValue)
{	
	if (bMagnesisKeyPressed) { return; }
	bIceKeyPressed = !bIceKeyPressed;

	if (!bIceKeyPressed) { bIceKeyPressed = false; }

	AMainHUD* MainHUD = Cast<AMainHUD>(GetHUD());
	if (MainHUD)
	{
		MainHUD->ShowAbilityAimUI(bIceKeyPressed);
	}

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());

	if (bIceKeyPressed)
	{
		Player_C->ZoomIn();

		IcePreviewActor->SetActorHiddenInGame(false);

		//FindVisibleActorOnScreen(LastHit); // Surface에 그리드 표현?

		bIceMaker = true;

		UpdateIcePreview();

		if (bIcePreviewPlaced)
		{
			SetIgnoreLookInput(true);
		}

		bIcePreviewPlaced = false; // 새 위치 탐색 허용

		// 캐릭터 IcePreviewActor에 정면 고정?
	}
	else
	{
		Player_C->ZoomOut();

		IcePreviewActor->SetActorHiddenInGame(true);

		SetIgnoreLookInput(false);

		bIcePreviewPlaced = false; // 해제되면 다시 재탐색 가능
	}

}

void APC_InGame::UpdateIcePreview()
{
	if (!IcePreviewActor || bIcePreviewPlaced) return;

	// 화면 중앙 기준으로 라인트레이스
	FVector Start;
	FRotator Rot;
	GetPlayerViewPoint(Start, Rot);

	FVector End = Start + Rot.Vector() * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		IcePreviewActor->SetPivotLocation(HitResult.Location);
		IcePreviewActor->SetRiseDirection(HitResult.Normal);

		FRotator PreviewRot = FRotationMatrix::MakeFromZ(HitResult.Normal).Rotator();
		IcePreviewActor->SetActorRotation(PreviewRot);
		IcePreviewActor->SetActorHiddenInGame(false);

		// 중심에 있는 액터가 Surface인지 여부 판단 → 색상 결정
		bool bCenterIsSurface = IsSurfaceActor(HitResult.GetActor());

		IcePreviewActor->GetMaterialInstance()->SetScalarParameterValue("Color", bCenterIsSurface ? 0.0f : 1.0f);

		IcePreviewActor->SetCanSpawn(bCenterIsSurface);
		bCanSpawn = true;	

		// 위치 저장
		LastHit = HitResult;
		bIcePreviewPlaced = true; // 한 번만 배치
	}
	else
	{
		IcePreviewActor->SetActorHiddenInGame(true);
		IcePreviewActor->SetCanSpawn(false);
		bCanSpawn = false;
	}
}

bool APC_InGame::IsSurfaceActor(AActor* Actor) const
{
	if (!Actor) return false;

#if WITH_EDITOR
	FString ActorName = Actor->GetActorLabel();
#else
	FString ActorName = Actor->GetName();
#endif

	return ActorName.StartsWith(TEXT("Surface"));
}

AActor* APC_InGame::FindVisibleActorOnScreen(FHitResult& OutHit)
{
	const int GridSize = 5;
	const float ScreenStep = 1.0f / GridSize;

	int32 ViewX, ViewY;
	GetViewportSize(ViewX, ViewY);

	for (int X = 0; X <= GridSize; ++X)
	{
		for (int Y = 0; Y <= GridSize; ++Y)
		{
			float ScreenX = X * ScreenStep * ViewX;
			float ScreenY = Y * ScreenStep * ViewY;

			FVector WorldOrigin;
			FVector WorldDirection;

			if (DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldOrigin, WorldDirection))
			{
				FVector End = WorldOrigin + WorldDirection * TraceDistance;

				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(GetPawn());

				if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, End, ECC_Visibility, Params))
				{
					AMetalActor* HitMetal = Cast<AMetalActor>(HitResult.GetActor());
					if (HitMetal)
					{
						OutHit = HitResult;
						return HitMetal;
					}
				}
			}
		}
	}

	return nullptr;
}

void APC_InGame::ShowMetalActorPreview(const FInputActionValue& InputActionValue)
{
	if (bIceKeyPressed) { return; }

	bMagnesisKeyPressed = !bMagnesisKeyPressed;

	AMainHUD* MainHUD = Cast<AMainHUD>(GetHUD());
	if (MainHUD)
	{
		MainHUD->ShowAbilityAimUI(bMagnesisKeyPressed);
	}

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());

	if (bMagnesisKeyPressed)
	{
		Player_C->ZoomIn();

		GetWorld()->GetTimerManager().SetTimer(
			MoveTimerHandle,
			this,
			&APC_InGame::ScanMetalActorInView,
			0.1f,  // 주기 (초)
			true   // 반복
		);
	}
	else
	{
		Player_C->ZoomOut();

		if (MetalActor)
		{
			MetalActor->ChangeNomalColor();
		}

		if (IsHoldingObject())
		{
			StopMagnetGrab();
		}

		// 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);

		MetalActor = nullptr;
	}

}


void APC_InGame::Magnesis()
{	
	if (!bMagnesisKeyPressed) return;

	if (!IsHoldingObject() && bCanControlMetal)
	{
		StartMagnetGrab();
	}
}

void APC_InGame::CheckMetalActor()
{
	// 화면 중앙 기준 라인트레이스
	FVector Start;
	FRotator Rot;
	GetPlayerViewPoint(Start, Rot);

	FVector End = Start + Rot.Vector() * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

#if WITH_EDITOR
		FString ActorName = HitActor->GetActorLabel(); // Editor에서 Item Label 사용
#else
		FString ActorName = HitActor->GetName(); // 게임 런타임에서는 fallback
#endif

		if (!ActorName.StartsWith(TEXT("Metal")))
		{
			bCanControlMetal = false;
			return;
		}

		bCanControlMetal = true;

		// LastHit 업데이트
		LastHit = HitResult;
	}
	else
	{
		bCanControlMetal = false;
	}
}

void APC_InGame::StartMagnetGrab()
{
	if (IsHoldingObject()) return;

	//FHitResult HitResult;
	if (TraceForMetal(LastHit))
	{
		if (UPrimitiveComponent* HitComp = LastHit.GetComponent())
		{
			if (HitComp->IsSimulatingPhysics())
			{
				// 플레이어 위치와 MetalActor 위치 기준 거리 측정
				ACharacter* PlayerChar = Cast<ACharacter>(GetPawn());
				if (PlayerChar)
				{
					FVector PlayerLocation = PlayerChar->GetActorLocation();
					FVector TargetLocation = HitComp->GetOwner()->GetActorLocation();
					HoldDistance = FVector::Distance(PlayerLocation, TargetLocation);
				}

				PhysicsHandle->GrabComponentAtLocation(HitComp, NAME_None, LastHit.ImpactPoint);
				GrabbedComponent = HitComp;

				// 주기적으로 위치 갱신
				GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &APC_InGame::MoveGrabbedObject, 0.01f, true);
			}
		}
	}
}

void APC_InGame::StopMagnetGrab()
{
	if (!IsHoldingObject()) return;

	PhysicsHandle->ReleaseComponent();
	GrabbedComponent = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
}

bool APC_InGame::TraceForMetal(FHitResult& OutHit)
{
	FVector Start;
	FRotator Rot;
	GetPlayerViewPoint(Start, Rot);

	FVector End = Start + Rot.Vector() * TraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_PhysicsBody, Params);

	if (bHit && OutHit.GetActor()->IsA(MetalActorClass))
	{
		return true;
	}

	return false;
}

void APC_InGame::MoveGrabbedObject()
{
	if (!IsHoldingObject()) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TargetLocation = CameraLocation + CameraRotation.Vector() * HoldDistance;

	// 현재 위치
	//FVector CurrentLocation = GrabbedComponent->GetComponentLocation();
	FVector CurrentLocation = GrabbedComponent->GetOwner()->GetActorLocation();

	// 부드럽게 따라가게 보간 처리
	FVector SmoothedLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 0.01f);

	PhysicsHandle->SetTargetLocation(TargetLocation);	

}

bool APC_InGame::IsHoldingObject() const
{
	return PhysicsHandle->GrabbedComponent != nullptr;
}

void APC_InGame::ScanMetalActorInView()
{
	if (!bMagnesisKeyPressed) return;

	FHitResult MetalHit;
	AMetalActor* FoundMetal = Cast<AMetalActor>(FindVisibleActorOnScreen(MetalHit));

	// 변경이 감지된 경우만 업데이트
	if (FoundMetal && FoundMetal != MetalActor)
	{
		if (MetalActor)
		{
			MetalActor->ChangeNomalColor(); // 기존 액터 색상 복구
		}

		MetalActor = FoundMetal;
		MetalActor->ThisIsMetal();
	}
}

void APC_InGame::OnNavigate(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UInventory* InvenUI = nullptr;
	UShop* ShopUI = nullptr;
	UNPCDialogue* DialogUI = nullptr;
	UQuest* QuestUI = nullptr;

	switch (CurrentInputContext)
	{
	case EInputContext::IC_Inventory:
		InvenUI = UIManager->FindUI<UInventory>();
		if (InvenUI)	
			InvenUI->OnNavigate(InputActionValue);
		
		break;
	case EInputContext::IC_Shop:
		ShopUI = UIManager->FindUI<UShop>();
		if (ShopUI)		
			ShopUI->OnNavigate(InputActionValue);
		
		break;
	case EInputContext::IC_Dialogue:
		DialogUI = UIManager->FindUI<UNPCDialogue>();
		if (DialogUI)
			DialogUI->OnNavigate(InputActionValue);
		
		break;
	case EInputContext::IC_Quest:
		QuestUI = UIManager->FindUI<UQuest>();
		if (QuestUI)
			QuestUI->OnNavigate(InputActionValue);
		
		break;
	}
}

void APC_InGame::OnConfirm(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UInventory* InvenUI = nullptr;
	UShop* ShopUI = nullptr;
	UNPCDialogue* DialogUI = nullptr;
	UQuest* QuestUI = nullptr;

	switch (CurrentInputContext)
	{
	case EInputContext::IC_Inventory:
		 InvenUI = UIManager->FindUI<UInventory>();
		if (InvenUI)
			InvenUI->OnConfirm(InputActionValue);
		
		break;
	case EInputContext::IC_Shop:
		 ShopUI = UIManager->FindUI<UShop>();
		if (ShopUI)
			ShopUI->OnConfirm();
		
		break;
	case EInputContext::IC_Dialogue:
		 DialogUI = UIManager->FindUI<UNPCDialogue>();
		if (DialogUI)
			DialogUI->OnConfirm();

		break;
	case EInputContext::IC_Quest:
		QuestUI = UIManager->FindUI<UQuest>();
		if (QuestUI)
			QuestUI->OnConfirm();

		break;
	}
}

void APC_InGame::OnCancel(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UInventory* InvenUI = nullptr;
	UShop* ShopUI = nullptr;
	UNPCDialogue* DialogUI = nullptr;
	UQuest* QuestUI = nullptr;

	switch (CurrentInputContext)
	{
	case EInputContext::IC_Inventory:
		InvenUI = UIManager->FindUI<UInventory>();
		if (InvenUI)	
			InvenUI->OnCancel(InputActionValue);
		
		break;
	case EInputContext::IC_Shop:
		ShopUI = UIManager->FindUI<UShop>();
		if (ShopUI)	
			ShopUI->OnCancel();
		
		break;
	case EInputContext::IC_Dialogue:
		DialogUI = UIManager->FindUI<UNPCDialogue>();
		if (DialogUI)	
			DialogUI->OnCancel();
		
		break;

	case EInputContext::IC_Quest:
		QuestUI = UIManager->FindUI<UQuest>();
		if (QuestUI)
			QuestUI->OnCancel();

		break;
	}
}

void APC_InGame::OnNextDialogue(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UNPCDialogue* DialogUI = nullptr;

	switch (CurrentInputContext)
	{
	case EInputContext::IC_Inventory:
		break;
	case EInputContext::IC_Shop:
		break;
	case EInputContext::IC_Dialogue:
		DialogUI = UIManager->FindUI<UNPCDialogue>();
		if (DialogUI)	
			DialogUI->OnNextDialogue(InputActionValue);
		
		break;
	}
}

void APC_InGame::OnCreateItemTest(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	UInventory* InvenUI = nullptr;

	switch (CurrentInputContext)
	{
	case EInputContext::IC_Inventory:
		InvenUI = UIManager->FindUI<UInventory>();
		if (InvenUI)	
			InvenUI->OnCreateItemTest(InputActionValue);
		
		break;
	case EInputContext::IC_Shop:
		break;
	case EInputContext::IC_Dialogue:
		break;
	}
}

