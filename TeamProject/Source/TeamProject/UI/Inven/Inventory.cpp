// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/Inventory.h"


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
        FInputActionBinding& Bind = PC->InputComponent->BindAction("CloseInven", IE_Pressed, this, &UInventory::CloseInven);
    }
}

void UInventory::InitUI()
{
    //Scroll 불러오기
    InvenScroll = Cast<UInventoryScroll>(GetWidgetFromName(TEXT("InvenScroll")));
    if (!InvenScroll)
    {
        check(false);
        return;
    }
}

void UInventory::CloseInven()
{
    if (!IsInViewport())
        return;

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        UIManager->RemoveUI(this);
    }
}

void UInventory::RefreshInventory(FItemData ItemData)
{

}
