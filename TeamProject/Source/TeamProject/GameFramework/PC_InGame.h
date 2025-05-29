// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputSubsystems.h"

#include "UI/Inven/Inventory.h"
#include "UI/NpcDialogue/NPCDialogue.h"
#include "UI/Shop/Shop.h"
#include "UI/Quest/Quest.h"

#include "CM_InGame.h"
#include "PC_InGame.generated.h"

enum class EInputContext
{
	IC_Start,
	IC_InGame,
	IC_Inventory,
	IC_Dialogue,
	IC_Shop,
	IC_Quest,
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

	//Shop
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Shop = nullptr;

	//Quest
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Quest = nullptr;

	//Player
public:
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_LookMouse = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_Climb = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_LeftClick = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
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

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Quest = nullptr;

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

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Build = nullptr;

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
		check(IA_Quest);
		check(IA_InvenNavigate);
		check(IA_InvenConfirm);
		check(IA_InvenCancel);
		check(IA_InvenAddItem);
		check(IA_DialogueNavigate);
		check(IA_DialogueConfirm);
		check(IA_DialogueCancel);
		check(IA_DialogueNext);
		check(IA_IceMaker);		
		check(IA_Build);
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
	virtual void Tick(float DeltaSeconds) override;

public:
	void ChangeInputContext(EInputContext NewContext);
	void BindInventoryInput();
	void BindDialogueInput();
	void BindShopInput();
	void BindQuestInput();

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnMoveCancel(const FInputActionValue& InputActionValue);


	void OnLook(const FInputActionValue& InputActionValue);
	void LeftClick(const FInputActionValue& InputActionValue);
	void RightClick(const FInputActionValue& InputActionValue);
	void RightClickEnd(const FInputActionValue& InputActionValue);
	void Climb(const FInputActionValue& InputActionValue);


	// --------- Weapon Swap ------------------------------

	void EquipSword(const FInputActionValue& InputActionValue);
	void EquipShield(const FInputActionValue& InputActionValue);
	void EquipBow(const FInputActionValue& InputActionValue);


	void OnInteract(const FInputActionValue& InputActionValue);
	void OpenInventory(const FInputActionValue& InputActionValue);
	void OpenQuest(const FInputActionValue& InputActionValue);

	// --------- Ice Maker ------------------------------

	void BeginIcePreview(const FInputActionValue& InputActionValue);
	void EndIcePreview(const FInputActionValue& InputActionValue);

	//UI
	void OnNavigate(const FInputActionValue& InputActionValue);
	void OnConfirm(const FInputActionValue& InputActionValue);
	void OnCancel(const FInputActionValue& InputActionValue);
	void OnNextDialogue(const FInputActionValue& InputActionValue);

	void OnCreateItemTest(const FInputActionValue& InputActionValue);
	//UI

	void SpawnIcePillar(const FInputActionValue& InputActionValue);
	
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
	// 매 프레임 업데이트
	UFUNCTION()
	void UpdateIcePreview();

	// 충돌 체크
	UFUNCTION()
	void CheckCollision();

	// 수면 체크
	UFUNCTION()
	void CheckSurface();


protected:
	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<AIcePillar> IcePillarClass;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<AIcePreview> IcePreviewClass;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	float TraceDistance = 300.0f;

	UPROPERTY()
	TObjectPtr<AIcePreview> IcePreviewActor = nullptr;

private:
	bool bQPressed = false;
	bool bCanSpawn = false;
	bool bHitResult = false;

	FHitResult Hit;
};
