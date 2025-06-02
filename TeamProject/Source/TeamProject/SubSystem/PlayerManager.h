// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Data/QuestDataRow.h"
#include "Data/NpcCharacterTableRow.h"
#include "Data/ItemDataRow.h"

#include "PlayerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, const TArray<FQuestDataRow>&, QuestList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryAllUpdated, const TArray<FItemData>&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const FItemData&, ItemData);

struct sPlayerStatus
{
	int32 MaxHp;
	int32 Hp;
	float MaxStamina;
	float Stamina;
	int32 Rupee;

	float Armor;
	float Damage;
	float Runspeed;

	FTransform PlayerTransform;
	FName LevelName;

	FVector PreviousLoction = FVector::Zero();

	float StaminaRegenSpeed;

	bool bIsUseStamina;
	//�߰��Ұ� �Ͻʼ�~
};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//Quest����
public:
	void SetQuestData(FQuestDataRow QuestRow);
	void ShowQuestUI();
	const TArray<FQuestDataRow>& GetAllQuestData() const { return QuestList; }
private:
	//Quest����

	//Item����
public:
	void SetInvenData(FItemData QuestRow);
	void ShowInvenUI();
	const TArray<FItemData>& GetAllItemData() const { return ItemList; }
private:
	void UpDateInvenUI(const FItemData& ItemData);
	void UpDateInvenUI(const TArray<FItemData>& QuestList);
	//Item����

	//Status����
public:
	void InitStatus();

	void SetPlayerStatus(sPlayerStatus status) { PlayerStatus = status; }
	void SetPlayerHp(int32 Hp) { PlayerStatus.Hp = Hp; }
	void SetRupee(int32 Rupee) { PlayerStatus.Rupee = Rupee; }
	void SetTargetLevel(const TSoftObjectPtr<UWorld>& NewLevel) { TargetLevel = NewLevel; }

	const sPlayerStatus& GetPlayerStatus() { return PlayerStatus; }
	const int32& GetHp() { return PlayerStatus.Hp; }
	const int32& GetMaxHp() { return PlayerStatus.MaxHp; }
	const float& GetStamina() { return PlayerStatus.Stamina; }
	const float& GetMaxStamina() { return PlayerStatus.MaxStamina; }
	const int32& GetRupee() { return PlayerStatus.Rupee; }
	const TSoftObjectPtr<UWorld> GetTargetLevel() { return TargetLevel; }


public:
	void SetPlayerStamina(float InStamina);
	void TickStamina(float DeltaTime);

	void SetStaminaUSe(bool bIsUse) { PlayerStatus.bIsUseStamina = bIsUse; }
	bool IsStaminaUse() const { return PlayerStatus.bIsUseStamina; }
	bool IsStaminaFull() const { return PlayerStatus.Stamina >= PlayerStatus.MaxStamina;}
	//Status����

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryAllUpdated OnInventoryAllUpdated;

private:
	sPlayerStatus PlayerStatus;

	TArray<FQuestDataRow> QuestList;
	TArray<FItemData> ItemList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading")
	TSoftObjectPtr<UWorld> TargetLevel;
};
