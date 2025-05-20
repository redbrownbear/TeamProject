// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PC_InGame.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "SubSystem/UI/UIManager.h"
#include "UI/NpcDialogue/NPCDialogue.h"

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
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Attack,
		ETriggerEvent::Started, this, &ThisClass::TryAttack);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Interact,
		ETriggerEvent::Triggered, this, &ThisClass::OnInteract);
	EnhancedInputComponent->BindAction(PC_InGameDataAsset->IA_Inventory,
		ETriggerEvent::Triggered, this, &ThisClass::OpenInventory);

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
	}

	CurrentInputContext = NewContext;
}

void APC_InGame::BindInventoryInput(UInventory* Inventory)
{
	// 인풋 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PC_InGameDataAsset->IA_Navigate, ETriggerEvent::Started, Inventory, &UInventory::OnNavigate);
		EIC->BindAction(PC_InGameDataAsset->IA_Confirm, ETriggerEvent::Triggered, Inventory, &UInventory::OnConfirm);
		EIC->BindAction(PC_InGameDataAsset->IA_Cancel, ETriggerEvent::Triggered, Inventory, &UInventory::OnCancel);
		EIC->BindAction(PC_InGameDataAsset->IA_AddItem, ETriggerEvent::Started ,Inventory, &UInventory::OnCreateItemTest);
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

void APC_InGame::TryAttack(const FInputActionValue& InputActionValue)
{
	APawn* PlayerPawn = GetPawn();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	
	PlayerCharacter->Play_Sword_Attack();

}

void APC_InGame::OnInteract(const FInputActionValue& InputActionValue)
{
	if (Npc && Npc->GetCanTalk())
	{
		if (UNpcFSMComponent* FSM = Npc->GetFSMComponent())
		{
			FSM->ChangeState(ENpcState::Talk);


			UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
			check(UIManager);

			if (UIManager)
			{
				FString Path = TEXT("/Game/BluePrint/UI/NpcDialogue/BP_NpcDialogue.BP_NpcDialogue_C");
				TSubclassOf<UNPCDialogue> PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);

				UNPCDialogue* NewUI = UIManager->CreateUI(GetWorld(), PopupUIBPClass);
				if (!NewUI)
				{
					check(NewUI);

					//이후에 QuestDialogueManager ->ShowDialogue 부분에서 NPCTableRow에 EQuestCharacter를 참조해서 넣어주면 됨
					//에디터에 원하는 데이터를 넣어서 사용하세요
					//추가로 대사 넘거가는 기능은 따로 만들예정
				}
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
		//Inventory
		FString Path = TEXT("/Game/BluePrint/UI/Inventory/BP_InvenLayout.BP_InvenLayout_C");
		TSubclassOf<UInventory> PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);

		UInventory* NewUI = UIManager->CreateUI(GetWorld(), PopupUIBPClass);
		if (!NewUI)
		{
			check(NewUI);
		}
	}
}


