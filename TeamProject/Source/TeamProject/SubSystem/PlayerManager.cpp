// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/PlayerManager.h"
#include "SubSystem/UI/QuestManager.h"

void UPlayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    InitStatus();
}

void UPlayerManager::SetQuestData(FQuestDataRow QuestRow)
{
    //�ӽ��ӽ�!!!!!!
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
    OnInventoryAllUpdated.Broadcast(ItemRows); // UI���� �˸�
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

    status.Runspeed = 100.f;            //�˾Ƽ� ���ϼ�~
    status.LevelName;                   //�ʱⰪ�� ��� �Ұ���?
    status.PlayerTransform;             //��������!
    status.PreviousLoction;             //�˾Ƽ� ����
    status.StaminaRegenSpeed = 4.0f;    //�˾Ƽ�

    status.Rupee = 0;

    PlayerStatus = status;
}

void UPlayerManager::SetPlayerStamina(float InStamina)
{
    PlayerStatus.Stamina = FMath::Clamp(InStamina, 0.f, PlayerStatus.MaxStamina);
}

void UPlayerManager::TickStamina(float DeltaTime)
{
    //���׹̳� ������̸� �� �þ
    if (PlayerStatus.bIsUseStamina)
        return;

    // �̹� �ִ�� �� �þ
    if (PlayerStatus.Stamina >= PlayerStatus.MaxStamina)
        return;

    PlayerStatus.Stamina += PlayerStatus.StaminaRegenSpeed * DeltaTime;
    PlayerStatus.Stamina = FMath::Clamp(PlayerStatus.Stamina, 0.f, PlayerStatus.MaxStamina);
}
