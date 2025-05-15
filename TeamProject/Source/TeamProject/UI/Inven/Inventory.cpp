// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem//UI/UIManager.h"
#include "UI/Inven/Inventory.h"


void UInventory::OnCreated()
{
    Super::OnCreated();
    InitUI();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind = PC->InputComponent->BindAction("CloseInven", IE_Pressed, this, &UInventory::CloseInven);
    }
}

void UInventory::InitUI()
{
    //Scroll 불러오기
    InvenScroll = Cast<UUserWidget>(GetWidgetFromName(TEXT("InvenScroll")));
    if (!InvenScroll)
    {
        check(false);
        return;
    }

    //Slot 불러오기
    InvenSlot = Cast<UUserWidget>(GetWidgetFromName(TEXT("InvenSlot")));
    if (!InvenSlot)
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
