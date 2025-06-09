// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/QuestDataRow.h"
#include "Data/NpcCharacterTableRow.h"

#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UQuestManager();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadQuestData(UDataTable* DataTable);

	TArray<FQuestDataRow> GetQuestData() const;

	FQuestDataRow GetQuestDataByNum(int32 QuestNum) const;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* QuestDataTable;

	TArray<FQuestDataRow> QuestArr;
};
