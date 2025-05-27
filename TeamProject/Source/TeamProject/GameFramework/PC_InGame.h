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

enum class EInputContext
{
	IC_Start,
	IC_InGame,
	IC_Inventory,
	IC_Dialogue,
	//필요하면 추가해서 사용합니다.
	IC_Supernatural,

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

	//Supernatural
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Supernatural = nullptr;

	//Player
public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LookMouse = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LeftClick = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_RightClick = nullptr;


	// --------- Weapon Swap-----------------------------

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_EquipSword = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_EquipShield = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_EquipBow = nullptr;



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

	//Supernatural
public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_IceMaker = nullptr;

	void CheckValid() const
	{
		check(IMC_InGame);
		check(IMC_Inventory);
		check(IMC_Dialogue);
		check(IA_Move);
		check(IA_LookMouse);
		check(IA_LeftClick);
		check(IA_RightClick);
		check(IA_EquipSword);
		check(IA_EquipShield);
		check(IA_EquipBow);
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
		check(IA_IceMaker);
	}
};


class AIcePillar;
class AIcePreview;
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
	void ShowDialogueUI();

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnMoveCancel(const FInputActionValue& InputActionValue);


	void OnLook(const FInputActionValue& InputActionValue);
	void LeftClick(const FInputActionValue& InputActionValue);
	void RightClick(const FInputActionValue& InputActionValue);

	// --------- Weapon Swap ------------------------------

	void EquipSword(const FInputActionValue& InputActionValue);
	void EquipShield(const FInputActionValue& InputActionValue);
	void EquipBow(const FInputActionValue& InputActionValue);


	void OnInteract(const FInputActionValue& InputActionValue);
	void OpenInventory(const FInputActionValue& InputActionValue);

public:
	void SetNpc(class ANpc* InNpc) { Npc = InNpc; }

public:
	UPROPERTY(EditAnywhere)
	UPC_InGameDataAsset* PC_InGameDataAsset;	

	UPROPERTY()
	TObjectPtr<class ANpc> Npc = nullptr;

	EInputContext CurrentInputContext = EInputContext::IC_Start;

	// --------- Supernatural ----------
protected:
	// 우클릭 누르고 있을 때 호출
	UFUNCTION()
	void BeginIcePreview(const FInputActionValue& Value);

	// 우클릭 뗄 때 호출
	UFUNCTION()
	void EndIcePreview(const FInputActionValue& Value);

	UFUNCTION()
	void SpawnIcePillar();

	UFUNCTION()
	void ClearOldestPillar(); 

	// 매 프레임 업데이트
	void UpdateIcePreview();

protected:
	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<AIcePillar> IcePillarClass;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<AIcePreview> IcePreviewClass;

	UPROPERTY()
	TObjectPtr<AIcePreview> IcePreviewActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float TraceDistance = 300.0f;

private:
	TArray<TWeakObjectPtr<AIcePillar>> IceList;
	uint64 MaxIceCount = 3; // 한 번에 만들 수 있는 얼음 기둥 개수

private:
	UPROPERTY()
	bool bIsQHeld = false;

};
