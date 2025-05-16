// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UITestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AUITestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;


	//�����׽�Ʈ��
public:
	void CreatePopup();
	void OnOpenPopup();

	void CreateInven();
	void OnOpenInven();

	void CreateItem();


protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/TeamProject.ItemDataTable"))
	FDataTableRowHandle DataTableRowHandle;
};
