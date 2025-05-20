// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/Inventory.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"


void UInventory::OnCreated()
{
    Super::OnCreated();

    InitUI();

    UInventoryManager* InvenManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    check(InvenManager);
    if (InvenManager)
    {
        InvenManager->OnInventoryUpdated.AddDynamic(this, &UInventory::RefreshInventory);
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind = PC->InputComponent->BindAction("CloseInven", IE_Pressed, this, &UInventory::CloseUI);
    }
}

void UInventory::CloseUI()
{
    Super::CloseUI();
}

void UInventory::InitUI()
{
    //Scroll 불러오기
    check(BP_InvenScroll); // BindWidget이 잘 됐는지 확인
}

void UInventory::RefreshInventory(const FItemData& ItemData)
{
    BP_InvenScroll->AddItemSlot(ItemData);
}
