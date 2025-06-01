// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/QuestDataRow.h"
#include "Data/NpcCharacterTableRow.h"

#include "QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, const TArray<FQuestDataRow>&, QuestList);
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

	TArray<FQuestDataRow> GetQuestData(EQuestCharacter QuestChar) const;

public:
	void ShowUI();

	//µ•¿Ã≈Õ
	void UpdateQuestData(EQuestCharacter QuestChar, const FQuestDataRow UpdateShopRow);

	//UI
	void UpdateQuest(const TArray<FQuestDataRow>& ShopList);

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* QuestDataTable;

	TMap<EQuestCharacter, TArray<FQuestDataRow>> QuestRowMap;
};
