// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/PlayerManager.h"
#include "SubSystem/UI/QuestManager.h"

void UPlayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    InitStatus();
}

void UPlayerManager::SetQuestData(FQuestDataRow QuestRow)
{
    //ÔøΩ”ΩÔøΩÔøΩ”ΩÔøΩ!!!!!!
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
    ItemInvenList.Add(ItemRow);

    UpDateInvenUI(ItemRow);
}

void UPlayerManager::ShowInvenUI()
{
    UpDateInvenUI(ItemInvenList);
}

void UPlayerManager::SetEquipData(const FItemData& ItemRow)
{
    eEquipParts NewParts = ItemRow.GetParts();

    // ∞∞¿∫ ∫Œ¿ß æ∆¿Ã≈€ ¡¶∞≈
    EquipItemList.RemoveAll([&](const FItemData& Item)
        {
            return Item.GetParts() == NewParts;
        });

    // ªı æ∆¿Ã≈€ √ﬂ∞°
    EquipItemList.Add(ItemRow);

    UpDateInvenEquipUI(EquipItemList);
}

void UPlayerManager::ShowEquipUI()
{
    UpDateInvenEquipUI(EquipItemList);
}

bool UPlayerManager::IsEquipPart(eEquipParts Parts)
{
    for (FItemData ItemData : EquipItemList)
    {
        switch (ItemData.eItemCategory)
        {
        case EItemCategory::IT_Weapon:
            if (Parts == eEquipParts::RIGHT)
                return true;
        case EItemCategory::IT_Shield:
            if (Parts == eEquipParts::LEFT)
                return true;
        case EItemCategory::IT_Arrow:           
            if (ItemData.bIsArrow ? Parts == eEquipParts::ARROWLEFT : Parts == eEquipParts::BOWRIGHT)
                return true;
        case EItemCategory::IT_Armor:
            switch (ItemData.eArmorKind)
            {
            case EArmorKind::HEAD:
                if (Parts == eEquipParts::HEAD)
                    return true;
            case EArmorKind::ARMOR:
                if (Parts == eEquipParts::ARMOR)
                    return true;
            case EArmorKind::LEG:
                if (Parts == eEquipParts::UNDER)
                    return true;
            }
            break;
        }
    }
    return false;
}

FItemData UPlayerManager::GetItemByUniqueID(const FString& UniqueItemID)
{
    for (const FItemData& ItemData : EquipItemList)
    {
        if (ItemData.UniqueID == UniqueItemID)
        {
            return ItemData;
        }
    }

    return FItemData();
}

FItemData UPlayerManager::RemoveItemByUniqueID(FString UniqueID)
{
    for (int32 i = 0; i < ItemInvenList.Num(); ++i)
    {
        if (ItemInvenList[i].UniqueID == UniqueID)
        {
            FItemData Removed = ItemInvenList[i];
            ItemInvenList.RemoveAt(i);
            UpDateInvenUI(ItemInvenList); // UI µø±‚»≠
            return Removed;
        }
    }

    return FItemData();
}

void UPlayerManager::ShowQuickSlot()
{
    UpDataQuickSlot(ItemInvenList);
}

void UPlayerManager::UpDateInvenUI(const FItemData& ItemData)
{
    OnInventoryUpdated.Broadcast(ItemData);
}

void UPlayerManager::UpDateInvenUI(const TArray<FItemData>& ItemRows)
{
    OnInventoryAllUpdated.Broadcast(ItemRows); // UIÔøΩÔøΩÔøΩÔøΩ ÔøΩÀ∏ÔøΩ
}

void UPlayerManager::UpDateInvenEquipUI(const TArray<FItemData>& ItemList)
{   
    OnInvenEquipItemAllUpdated.Broadcast(ItemList);
}

void UPlayerManager::UpDataQuickSlot(const TArray<FItemData>& ItemList)
{
    OnQuickSlotUpdated.Broadcast(ItemList);
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

    status.Runspeed = 100.f;            //ÔøΩÀæ∆ºÔøΩ ÔøΩÔøΩÔøΩœºÔøΩ~
    status.LevelName;                   //ÔøΩ ±‚∞™ÔøΩÔøΩ ÔøΩÔøΩÔø?ÔøΩ“∞ÔøΩÔøΩÔøΩ?
    status.PlayerTransform;             //ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ!
    status.PreviousLoction;             //ÔøΩÀæ∆ºÔøΩ ÔøΩÔøΩÔøΩÔøΩ
    status.StaminaRegenSpeed = 4.0f;    //ÔøΩÀæ∆ºÔøΩ

    status.Rupee = 0;

    PlayerStatus = status;
}

void UPlayerManager::SetPlayerStamina(float InStamina)
{
    PlayerStatus.Stamina = FMath::Clamp(InStamina, 0.f, PlayerStatus.MaxStamina);
}

void UPlayerManager::TickStamina(float DeltaTime)
{
    //ÔøΩÔøΩÔøΩ◊πÃ≥ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÃ∏Ôø?ÔøΩÔøΩ ÔøΩ√æÓ≥≤
    if (PlayerStatus.bIsUseStamina)
        return;

    // ÔøΩÃπÔøΩ ÔøΩ÷¥ÔøΩÔø?ÔøΩÔøΩ ÔøΩ√æÓ≥≤
    if (PlayerStatus.Stamina >= PlayerStatus.MaxStamina)
        return;

    PlayerStatus.Stamina += PlayerStatus.StaminaRegenSpeed * DeltaTime;
    PlayerStatus.Stamina = FMath::Clamp(PlayerStatus.Stamina, 0.f, PlayerStatus.MaxStamina);
}
