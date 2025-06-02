// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/PlayerManager.h"
#include "SubSystem/UI/QuestManager.h"

void UPlayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    InitStatus();
}

void UPlayerManager::SetQuestData(FQuestDataRow QuestRow)
{
    //임시임시!!!!!!
    UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();
    check(QuestManager);

    QuestList = QuestManager->GetQuestData();

    //for (FQuestDataRow Data : QuestArr)
    //{
    //    if (Data.QuestNum == QuestRow.QuestNum)
    //    {
    //        return;
    //    }
    //}

    //QuestArr.Add(QuestRow);
}

void UPlayerManager::ShowQuestUI()
{
    OnQuestUpdated.Broadcast(QuestList);
}

void UPlayerManager::SetInvenData(FItemData ItemRow)
{
    ItemList.Add(ItemRow);  

    UpDateInvenUI(ItemRow);
}

void UPlayerManager::ShowInvenUI()
{
    UpDateInvenUI(ItemList);
}

void UPlayerManager::UpDateInvenUI(const FItemData& ItemData)
{
    OnInventoryUpdated.Broadcast(ItemData);
}

void UPlayerManager::UpDateInvenUI(const TArray<FItemData>& ItemRows)
{
    OnInventoryAllUpdated.Broadcast(ItemRows); // UI에게 알림
}

void UPlayerManager::InitStatus()
{
    sPlayerStatus status;
    status.Hp = 9;
    status.MaxHp = 16;
    status.Stamina = 10.0f;
    status.MaxStamina = 50.0f;
    status.Damage = 0.0f;
    status.Armor = 0.0f;

    status.Runspeed = 100.f;            //알아서 정하쇼~
    status.LevelName;                   //초기값을 어디서 할건지?
    status.PlayerTransform;             //마찬가지!
    status.PreviousLoction;             //알아서 쓰쇼
    status.StaminaRegenSpeed = 4.0f;    //알아서

    status.Rupee = 0;

    PlayerStatus = status;
}

void UPlayerManager::SetPlayerStamina(float InStamina)
{
    PlayerStatus.Stamina = FMath::Clamp(InStamina, 0.f, PlayerStatus.MaxStamina);
}

void UPlayerManager::TickStamina(float DeltaTime)
{
    //스테미너 사용중이면 안 늘어남
    if (PlayerStatus.bIsUseStamina)
        return;

    // 이미 최대면 안 늘어남
    if (PlayerStatus.Stamina >= PlayerStatus.MaxStamina)
        return;

    PlayerStatus.Stamina += PlayerStatus.StaminaRegenSpeed * DeltaTime;
    PlayerStatus.Stamina = FMath::Clamp(PlayerStatus.Stamina, 0.f, PlayerStatus.MaxStamina);
}
