// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputSubsystems.h"

#include "UI/Inven/Inventory.h"
#include "UI/NpcDialogue/NPCDialogue.h"

#include "CM_InGame.h"
#include "PC_InGame.generated.h"

// 2025-05-19 Yunjung: 임시로 대화 IMC 등록 시키는 중(현석 오빠랑 의논 필요)

enum class EInputContext
{
	IC_Start,
	IC_InGame,
	IC_Inventory,
	IC_Dialogue,
	//필요하면 추가해서 사용합니다.

	IC_End,
};

UCLASS()
class TEAMPROJECT_API UPC_InGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//InGame
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_InGame = nullptr;

	//Inventory
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Inventory = nullptr;

	//Dialogue
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Dialogue = nullptr;

	//Player
public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LookMouse = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Attack = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Interact = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Inventory = nullptr;

	//Inven
public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_InvenNavigate = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_InvenConfirm = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_InvenCancel = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_InvenAddItem = nullptr;

	//Dialogue
public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_DialogueNavigate = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_DialogueConfirm = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_DialogueCancel = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_DialogueNext = nullptr;



	void CheckValid() const
	{
		check(IMC_InGame);
		check(IMC_Inventory);
		check(IMC_Dialogue);
		check(IA_Move);
		check(IA_LookMouse);
		check(IA_Interact);
		check(IA_Inventory);
		check(IA_InvenNavigate);
		check(IA_InvenConfirm);
		check(IA_InvenCancel);
		check(IA_InvenAddItem);
		check(IA_DialogueNavigate);
		check(IA_DialogueConfirm);
		check(IA_DialogueCancel);
		check(IA_DialogueNext);
	}
};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API APC_InGame : public APlayerController
{
	GENERATED_BODY()
	
public:
	APC_InGame();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	void ChangeInputContext(EInputContext NewContext);
	void BindInventoryInput(UInventory* Inventory);
	void BindDialogueInput(UNPCDialogue* NpcDialogue);

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void TryAttack(const FInputActionValue& InputActionValue);
	void OnInteract(const FInputActionValue& InputActionValue);
	void OpenInventory(const FInputActionValue& InputActionValue);

public:
	UPROPERTY(EditAnywhere)
	UPC_InGameDataAsset* PC_InGameDataAsset;	

	UPROPERTY()
	TObjectPtr<class ANpc> Npc = nullptr;

	EInputContext CurrentInputContext = EInputContext::IC_Start;

};
