// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"
#include "SubSystem/UI/ShopManager.h"
#include "SubSystem/UI/QuestManager.h"

#include "Actors/Npc/Npc.h" 
#include "Components/Character/PlayerMovementComponent.h"

#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"


#include "Actors/Temple/Ice/IcePillar.h"
#include "Actors/Temple/Ice/IcePreview.h"

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
	if (UIManager)
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

	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Move,
		ETriggerEvent::Completed, this, &ThisClass::OnMoveCancel);



	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_LookMouse,
		ETriggerEvent::Triggered, this, &ThisClass::OnLook);




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
	if (Player_C->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}


	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);


	P_Anim->ActionValue = ActionValue;

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

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


	UAnimInstance* Anim = Player_C->GetMesh()->GetAnimInstance();

	UPlayerAnimInstance* P_Anim = Cast<UPlayerAnimInstance>(Anim);

	const FVector2D ActionValue = FVector2D();

	P_Anim->ActionValue = ActionValue;

}

void APC_InGame::OnLook(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	AddYawInput(ActionValue.X);
	AddPitchInput(-ActionValue.Y);
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetPawn());
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

	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

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
	check(UIManager);

	if (UIManager)
	{
		UIManager->ShowUI(UInventory::StaticClass());
	}
}

void APC_InGame::OpenQuest(const FInputActionValue& InputActionValue)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	if (UIManager)
	{
		UIManager->ShowUI(UQuest::StaticClass());
	}

	UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();
	if (QuestManager)
	{
		QuestManager->ShowUI();
	}
}

void APC_InGame::SpawnIcePillar()
{
	if (!IcePillarClass) return;

	if (!bQPressed) return;
	
	FVector Start, Dir;
	DeprojectMousePositionToWorld(Start, Dir);

	FVector End = Start + Dir * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 수면 체크: 지형 위라면 충돌, 월드 정적에 한정
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		FVector SpawnLoc = Hit.Location;

		// 최대 개수 초과 시 제거
		if (IceList.Num() >= MaxIceCount)
		{
			ClearOldestPillar();
		}

		AIcePillar* NewPillar = GetWorld()->SpawnActor<AIcePillar>(IcePillarClass, SpawnLoc, FRotator::ZeroRotator);
		if (NewPillar)
		{
			IceList.Add(NewPillar);
		}
	}
}

void APC_InGame::ClearOldestPillar()
{
	if (IceList.Num() == 0) return;

	if (IceList[0].IsValid())
	{
		IceList[0]->Destroy();
	}

	IceList.RemoveAt(0);;
}

void APC_InGame::BeginIcePreview(const FInputActionValue& InputActionValue)
{
	bQPressed = true; 

	if (!IcePreviewActor && IcePreviewClass)
	{
		IcePreviewActor = GetWorld()->SpawnActor<AIcePreview>(IcePreviewClass);
		if (IcePreviewActor)
		{
			IcePreviewActor->SetActorEnableCollision(false);
		}
	}

}

void APC_InGame::EndIcePreview(const FInputActionValue& InputActionValue)
{
	bQPressed = false;

	if (IcePreviewClass)
	{
		IcePreviewActor->Destroy();
		IcePreviewActor = nullptr;
	}

	SpawnIcePillar();
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

void APC_InGame::UpdateIcePreview()
{
	if (!IcePreviewClass) return;

	FVector Start, Dir;
	DeprojectMousePositionToWorld(Start, Dir);
	FVector End = Start + Dir * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		FVector HitLoc = Hit.Location;
		IcePreviewActor->SetActorLocation(HitLoc);
	}
}


