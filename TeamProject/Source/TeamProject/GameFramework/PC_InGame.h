// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "CM_InGame.h"
#include "PC_InGame.generated.h"



UCLASS()
class TEAMPROJECT_API UPC_InGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input|InputMappingContext")
	UInputMappingContext* IMC = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input|InputAction")
	UInputAction* IA_LookMouse = nullptr;

	void CheckValid() const
	{
		check(IMC);
		check(IA_Move);
		check(IA_LookMouse);
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


public:
	UPROPERTY(EditAnywhere)
	UPC_InGameDataAsset* PC_InGameDataAsset;
};
