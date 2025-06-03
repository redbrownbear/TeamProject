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
	UInputAction* IA_Jump = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_Dash = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|CharacterMove")
	UInputAction* IA_Crouch = nullptr;


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
	UInputAction* IA_Magnesis = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_TrySuperPower = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_ControlDistance = nullptr;


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
		check(IA_Magnesis);
		check(IA_TrySuperPower);
		check(IA_ControlDistance);
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
	virtual void Tick(float DeltaSeconds) override;

public:
	void ChangeInputContext(EInputContext NewContext);
	void BindInventoryInput();
	void BindDialogueInput();
	void BindShopInput();
	void BindQuestInput();

protected:
	// ---------- Move Mode ------------------------------
	void OnMove(const FInputActionValue& InputActionValue);
	void OnMoveCancel(const FInputActionValue& InputActionValue);

	void JumpGlide(const FInputActionValue& InputActionValue);
	void StartedDash(const FInputActionValue& InputActionValue);
	void CompletedDash(const FInputActionValue& InputActionValue);


	void OnLook(const FInputActionValue& InputActionValue);
	void Climb(const FInputActionValue& InputActionValue);

	void OnCrouch(const FInputActionValue& InputActionValue);
	void OnUnCrouch(const FInputActionValue& InputActionValue);


	// ------------Attack Sequence ------------------------
	void LeftClick(const FInputActionValue& InputActionValue);
	void RightClick(const FInputActionValue& InputActionValue);
	void RightClickEnd(const FInputActionValue& InputActionValue);

	// --------- Weapon Swap ------------------------------

	void EquipSword(const FInputActionValue& InputActionValue);
	void EquipShield(const FInputActionValue& InputActionValue);
	void EquipBow(const FInputActionValue& InputActionValue);


	void OnInteract(const FInputActionValue& InputActionValue);
	void OpenInventory(const FInputActionValue& InputActionValue);
	void OpenQuest(const FInputActionValue& InputActionValue);

	// --------- SuperPower ------------------------------
	void TrySuperPower(const FInputActionValue& InputActionValue);
	void OnControlDistance(const FInputActionValue& InputActionValue);

	// Ice Maker
	void BeginIcePreview(const FInputActionValue& InputActionValue);	
	
	// Magnesis
	void ShowMetalActorPreview(const FInputActionValue& InputActionValue);

	//UI
	void OnNavigate(const FInputActionValue& InputActionValue);
	void OnConfirm(const FInputActionValue& InputActionValue);
	void OnCancel(const FInputActionValue& InputActionValue);
	void OnNextDialogue(const FInputActionValue& InputActionValue);

	void OnCreateItemTest(const FInputActionValue& InputActionValue);
	//UI
	
	
public:
	void SetNpc(class ANpc* InNpc) { Npc = InNpc; }

public:
	UPROPERTY(EditAnywhere)
	UPC_InGameDataAsset* PC_InGameDataAsset;

	UPROPERTY()
	TObjectPtr<class ANpc> Npc = nullptr;

	UPROPERTY()
	TObjectPtr<class ATreasureBox> TreasureBox = nullptr;

	EInputContext CurrentInputContext = EInputContext::IC_Start;

	// --------- SuperPower ----------
protected:
	// 매 프레임 업데이트
	UFUNCTION()
	void UpdateIcePreview();

	// Create IcePillar
	UFUNCTION()
	void SpawnIcePillar();

	UFUNCTION()
	bool IsSurfaceActor(AActor* Actor) const;

	UFUNCTION()
	AActor* FindVisibleActorOnScreen(FHitResult& OutHit);

	// Magnesis
	UFUNCTION()
	void Magnesis();

	// Check Metal
	UFUNCTION()
	void CheckMetalActor();

	// Magnesis 입력 함수
	UFUNCTION()
	void StartMagnetGrab();
	UFUNCTION()
	void StopMagnetGrab();

	//  Magnesis 내부 기능 함수
	UFUNCTION()
	bool TraceForMetal(FHitResult& OutHit);

	UFUNCTION()
	void MoveGrabbedObject();

	// 상태 체크
	UFUNCTION()
	bool IsHoldingObject() const;

	UFUNCTION()
	void ScanMetalActorInView();

protected:
	// ------------ Ice Maker ---------------
	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<class AIcePillar> IcePillarClass;

	UPROPERTY(EditAnywhere, Category = "Cryonis")
	TSubclassOf<class AIcePreview> IcePreviewClass;

	UPROPERTY()
	TObjectPtr<AIcePreview> IcePreviewActor = nullptr;

	// ------------ Magnesis ---------------

	UPROPERTY(EditAnywhere, Category = "Magnesis")
	TSubclassOf<class AMetalActor> MetalActorClass;

	UPROPERTY()
	TObjectPtr<class AMetalActor> MetalActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Magnet | Trace")
	float TraceDistance = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Magnet | Grab")
	float HoldDistance = 0.f;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY()
	UPrimitiveComponent* GrabbedComponent;

	FTimerHandle MoveTimerHandle;

private:
	bool bIsCameraLocked = false;

	bool bIceKeyPressed = false;
	bool bIceMaker = false;
	bool bCanSpawn = false;
	bool bIcePreviewPlaced = false;

	bool bMagnesisKeyPressed = false;
	bool bCanControlMetal = false;

	FHitResult LastHit;
	bool bHitResult = false;
};
