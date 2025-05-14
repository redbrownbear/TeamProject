#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/TestCharacter.h"
#include "TestPC.generated.h"

UCLASS()
class TEAMPROJECT_API ATestPC : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	// Ű ���ε���
	void OnPressedE();
	void OnPressedQ();

public:
	// ��ǲ ����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Cook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* OpenInventory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* StartCooking;

protected:
	// @TODO Open Inventory
	void ShowInventory(bool bShow);

private:
	bool bInventory = false;

protected:
	UPROPERTY()
	TArray<FHandItem> CurrentHeldItem;
};
