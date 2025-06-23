// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/PlayerManager.h"
#include "SubSystem/UI/QuestManager.h"
#include "UI/HUD/MainHUD.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

void UPlayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    InitStatus();
}

void UPlayerManager::SetQuestData(FQuestDataRow QuestRow)
{
    UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();
    check(QuestManager);

    for (FQuestDataRow Data : QuestList)
    {
        if (Data.QuestNum == QuestRow.QuestNum)
        {
            return;
        }
    }

    AMainHUD* MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (MainHUD)
    {
        MainHUD->ShowQuestOn(QuestRow.bIsComplete, QuestRow.QuestTitle);
    }

    QuestList.Add(QuestRow);
}

void UPlayerManager::SetQuestCompleteData(FQuestDataRow QuestRow)
{
    UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();
    check(QuestManager);

    AMainHUD* MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (MainHUD)
    {
        MainHUD->ShowQuestOn(QuestRow.bIsComplete, QuestRow.QuestTitle);
    }

    for (int32 i = 0; i < QuestList.Num(); ++i)
    {
        if (QuestList[i].QuestNum == QuestRow.QuestNum)
        {
            QuestList.RemoveAt(i);
            break;
        }
    }
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

void UPlayerManager::SetInvenRemoveData(const FItemData& ItemRow)
{
    UpDataRemoveItemInInven(ItemRow);
}

void UPlayerManager::ShowInvenUI()
{
    UpDateInvenUI(ItemInvenList);
}

void UPlayerManager::SetEquipData(const FItemData& ItemRow)
{
    eEquipParts NewParts = ItemRow.GetParts();

    EquipItemList.RemoveAll([&](const FItemData& Item)
        {
            return Item.GetParts() == NewParts;
        });

    EquipItemList.Add(ItemRow);
    SetWeaponMesh(ItemRow.GetParts(), ItemRow.StaticMesh, ItemRow.eWeaponKind);
    UpDateInvenEquipUI(EquipItemList);
}

void UPlayerManager::SetWeaponMesh(eEquipParts Parts, UStaticMesh* _Mesh, EWeaponKind WeaponKind)
{
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    switch (Parts)
    {
    case eEquipParts::NONE:
        break;
    case eEquipParts::HEAD:
        break;
    case eEquipParts::ARMOR:
        break;
    case eEquipParts::UNDER:
        break;
    case eEquipParts::RIGHT:
        
        Player_C->SetWeaponStaticMesh(_Mesh, WeaponKind);
        break;
    case eEquipParts::LEFT:
        Player_C->SetShieldStaticMesh(_Mesh);
        break;
    case eEquipParts::BOWRIGHT:
        Player_C->SetBowStaticMesh(_Mesh);
        break;
    case eEquipParts::ARROWLEFT:
        break;
    default:
        break;
    }
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
            UpDateInvenUI(ItemInvenList);
            return Removed;
        }
    }

    return FItemData();
}

int32 UPlayerManager::GetItemCountByItemCode(const FString& ItemCode)
{
    int32 Count = 0;

    for (int32 i = 0; i < ItemInvenList.Num(); ++i)
    {
        if (ItemInvenList[i].Name == ItemCode)
        {
            Count++;
        }
    }

    return Count;
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
    OnInventoryAllUpdated.Broadcast(ItemRows);
}

void UPlayerManager::UpDateInvenEquipUI(const TArray<FItemData>& ItemList)
{   
    OnInvenEquipItemAllUpdated.Broadcast(ItemList);
}

void UPlayerManager::UpDataQuickSlot(const TArray<FItemData>& ItemList)
{
    OnQuickSlotUpdated.Broadcast(ItemList);
}

void UPlayerManager::UpDataRemoveItemInInven(const FItemData& ItemRow)
{
	OnInventoryRemoveUpdated.Broadcast(ItemRow);
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

    status.Runspeed = 100.f;        
    status.LevelName;               
    status.PlayerTransform;         
    status.PreviousLoction;         
    status.StaminaRegenSpeed = 4.0f;

    status.Rupee = 99999;

    PlayerStatus = status;
}

void UPlayerManager::SetPlayerStamina(float InStamina)
{
    PlayerStatus.Stamina = FMath::Clamp(InStamina, 0.f, PlayerStatus.MaxStamina);
}

void UPlayerManager::TickStamina(float DeltaTime)
{
    if (PlayerStatus.bIsUseStamina)
        return;

    if (PlayerStatus.Stamina >= PlayerStatus.MaxStamina)
        return;

    PlayerStatus.Stamina += PlayerStatus.StaminaRegenSpeed * DeltaTime;
    PlayerStatus.Stamina = FMath::Clamp(PlayerStatus.Stamina, 0.f, PlayerStatus.MaxStamina);
}
