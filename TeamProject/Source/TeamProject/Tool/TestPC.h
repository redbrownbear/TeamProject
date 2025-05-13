#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/ItemData.h"
#include "TestPC.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ATestPC : public APlayerController
{
	GENERATED_BODY()

public:
	void CheckItemAndTriggerAction();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemData ItemDataTable;
};
