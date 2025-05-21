// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "CM_InGame.h"
#include "PC_InGame.generated.h"

// 2025-05-19 Yunjung: 임시로 대화 IMC 등록 시키는 중(현석 오빠랑 의논 필요)

UCLASS()
class TEAMPROJECT_API UPC_InGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC = nullptr;

	// --------- Npc 인터렉트(임시 생성): 윤정 ----------
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC_Interact = nullptr;
	// --------------------------------------------------

public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LookMouse = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LeftClick = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_RightClick = nullptr;
	// --------- Npc 인터렉트(임시 생성): 윤정 ----------
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	UInputAction* IA_Talk = nullptr;
	// --------------------------------------------------

	void CheckValid() const
	{
		check(IMC);
		check(IMC_Interact);
		check(IA_Move);
		check(IA_LookMouse);
		check(IA_LeftClick);
		check(IA_RightClick);
		check(IA_Talk); // Npc 인터렉트(임시 생성): 윤정
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


protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void TryAttack(const FInputActionValue& InputActionValue);

	// --------- Npc 인터렉트(임시 생성): 윤정 ----------
	void OnTalk(const FInputActionValue& InputActionValue);
	// --------------------------------------------------

public:
	UPROPERTY(EditAnywhere)
	UPC_InGameDataAsset* PC_InGameDataAsset;

	// --------- Npc 인터렉트(임시 생성): 윤정 ----------
	bool bCanInteractWithNpc = false;

	UPROPERTY()
	TObjectPtr<class ANpc> Npc = nullptr;
	// --------------------------------------------------
};
