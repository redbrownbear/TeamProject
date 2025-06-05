// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/PlayerManager.h"

#include "Actors/Npc/Npc.h" 
#include "Components/Character/PlayerMovementComponent.h"

#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"


#include "Actors/Temple/Ice/IcePillar.h"
#include "Actors/Temple/Ice/IcePreview.h"

#include "Actors/Temple/Treasure/TreasureBox.h"

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
}

void APC_InGame::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputContext(EInputContext::IC_InGame);

	if (!IcePreviewActor && IcePreviewClass)
	{
		IcePreviewActor = GetWorld()->SpawnActor<AIcePreview>(IcePreviewClass);
		if (IcePreviewActor)
		{
			IcePreviewActor->SetActorEnableCollision(false);
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
		ETriggerEvent::Triggered, this, &ThisClass::StartedDash);
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

	// ------------ Supernatural -----------------
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_IceMaker,
		ETriggerEvent::Started, this, &ThisClass::BeginIcePreview);

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Build,
		ETriggerEvent::Started, this, &ThisClass::SpawnIcePillar);		
}

void APC_InGame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bQPressed)
	{
		if (IcePreviewActor)
		{
			UpdateIcePreview();
		}
	}
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
	case EInputContext::IC_Popup:
		Subsystem->AddMappingContext(PC_InGameDataAsset->IMC_Dialogue, 4);
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
		UE_LOG(LogTemp, Warning, TEXT("NoPlayer"));
		return;
	}
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
	if (Movement->MovementMode == MOVE_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveNone"));
		return;
	}
	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);


	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	// 클라이밍 상태일 때의 캐릭터 무브
	if (Movement->GetMoveState()==EMove_State::Climb)
	{
		if (Movement->GetClimbMode() == EClimb_State::Land)
		{
			return;
		}
	
		FHitResult HitResult;
		
		FVector Normal_Vec = HitResult.Normal;
		
		const FRotator Rotation = Player_C->K2_GetActorRotation();
				
		const FVector UpVector = UKismetMathLibrary::GetUpVector(Rotation);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(Rotation);


		P_Anim->ActionValue = ActionValue;

		APawn* ControlledPawn = GetPawn();
		ControlledPawn->AddMovementInput(UpVector, ActionValue.X);
		ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

		Movement->TrySetMoveClimb(ActionValue);

		UE_LOG(LogTemp, Warning, TEXT("Climbing"));

		
	}
	else if (Movement->GetMoveState()==EMove_State::Glide)
	{
		

		P_Anim->ActionValue = ActionValue;
		
		Movement->GlidingMove(ActionValue);
		UE_LOG(LogTemp, Warning, TEXT("Gliding"));
	}
	
	// 노말 상태일 때의 캐릭터 무브
	else
	{
		P_Anim->ActionValue = ActionValue;

		const FRotator Rotation = K2_GetActorRotation();
		const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);


		

		APawn* ControlledPawn = GetPawn();
		ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
		ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

		if (Movement->GetMoveState() == EMove_State::Dash)
		{
			UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			PlayerManager->SetStaminaUSe(true);
			float Stemina = PlayerManager->GetStamina();
			float DeltaTime = GetWorld()->DeltaTimeSeconds;

			Stemina -= DeltaTime * STEMINA_USE_SPEED;
			PlayerManager->SetPlayerStamina(Stemina);
			UE_LOG(LogTemp, Warning, TEXT("Dash"));
		}
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

	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	PlayerManager->SetStaminaUSe(false);

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
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
	
	if (Movement->GetMoveState() == EMove_State::Run)
	{
		Movement->MaxWalkSpeed = PLAYER_MOVE_DASH;
		Movement->SetMoveState(EMove_State::Dash);
	}

}

void APC_InGame::CompletedDash(const FInputActionValue& InputActionValue)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

	if (Movement->GetMoveState() == EMove_State::Dash)
	{
		Movement->MaxWalkSpeed = PLAYER_MOVE_NML;
		Movement->SetMoveState(EMove_State::Run); 
		UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
		PlayerManager->SetStaminaUSe(false);
	}
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

	// TreasureBox 열 때
	{		
		if (TreasureBoxActor != nullptr)
		{
			bool bCanOpen = TreasureBoxActor->GetOpenBox();
			if (bCanOpen)
			{
				TreasureBoxActor->OpenTBox();
			}
		}	
	}
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

void APC_InGame::SpawnIcePillar(const FInputActionValue& InputActionValue)
{
	if (!IcePillarClass) return;

	if (!bQPressed) return;

	// 수면 체크: 지형 위라면 충돌, 월드 정적에 한정
	CheckSurface();

	if (!bCanSpawn) return;

	FVector SpawnLoc = Hit.Location;
	FVector Normal = Hit.Normal;

	AIcePillar* IcePillarActor = GetWorld()->SpawnActor<AIcePillar>(IcePillarClass);

	// 호출 순서 중요
	IcePillarActor->SetRiseDirection(Hit.Normal);
	IcePillarActor->SetPivotLocation(Hit.Location);

	// 노멀 방향 회전 적용
	FRotator SpawnRot = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
	IcePillarActor->SetActorRotation(SpawnRot);
	IcePillarActor->SetActorHiddenInGame(false); // 보이도록	
	IcePreviewActor->SetActorHiddenInGame(true);

	bCanSpawn = false;
}

void APC_InGame::BeginIcePreview(const FInputActionValue& InputActionValue)
{	
	bQPressed = !bQPressed;

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
	UCharacterMovementComponent* C_Movement = Player_C->GetCharacterMovement();

	if (bQPressed)
	{
		// 캐릭터 이동 및 회전
		C_Movement->MaxWalkSpeed = PLAYER_MOVE_BOW_ZOOM;

		Player_C->bUseControllerRotationYaw = true; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

		// 이동 방향으로 자동 회전 비활성화
		C_Movement->bOrientRotationToMovement = false;

		USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();

		Player_C->ZoomIn();

		bIsCameraLocked = true;

		// show icepreview
		IcePreviewActor->SetActorHiddenInGame(false);	

		//// 애니메이션
		//UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());
		//AnimInst->Montage_Play(ChargingMTG);
	}
	else
	{
		Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

		// 이동 방향으로 자동 회전 비활성화
		Player_C->GetCharacterMovement()->bOrientRotationToMovement = true;

		Player_C->GetCharacterMovement()->MaxWalkSpeed = PLAYER_MOVE_NML;

		Player_C->ZoomOut();
		bIsCameraLocked = false;

		// hide icepreview
		IcePreviewActor->SetActorHiddenInGame(true);
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
	UPopupGetItem* PopupUI = nullptr;

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

	case EInputContext::IC_Popup:
		PopupUI = UIManager->FindUI<UPopupGetItem>();
		if (PopupUI)
			PopupUI->OnCancel();

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

void APC_InGame::UpdateIcePreview()
{
	if (!IcePreviewActor) return;

	CheckSurface();

	if (bHitResult)
	{
		IcePreviewActor->SetPivotLocation(Hit.Location);

		// 노멀 방향 회전 적용
		FRotator PreviewRot = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
		IcePreviewActor->SetActorRotation(PreviewRot);
		IcePreviewActor->SetActorHiddenInGame(false); // 보이도록	

		IcePreviewActor->SetRiseDirection(Hit.Normal);
	}
	else
	{
		IcePreviewActor->SetActorHiddenInGame(true); // 일시적으로 숨김 
	}
}

void APC_InGame::CheckCollision()
{
	FHitResult HitResult;
	bHitResult = this->GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		HitResult);

	if (bHitResult)
	{
		Hit = HitResult;
	}
}

void APC_InGame::CheckSurface()
{
	CheckCollision();

	if (bHitResult)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;

#if WITH_EDITOR
		FString ActorName = HitActor->GetActorLabel(); // Editor에서 Item Label 사용
#else
		FString ActorName = HitActor->GetName(); // 게임 런타임에서는 fallback
#endif

		if (!ActorName.StartsWith(TEXT("Surface")))
		{
			bCanSpawn = false;
			if (IcePreviewActor)
			{
				IcePreviewActor->GetMaterialInstance()->SetScalarParameterValue("Color", 1.0f);
			}
			return;
		}

		if (IcePreviewActor)
		{
			IcePreviewActor->GetMaterialInstance()->SetScalarParameterValue("Color", 0.0f);
		}

		bCanSpawn = true;
		IcePreviewActor->SetCanSpawn(bCanSpawn);
	}
}

