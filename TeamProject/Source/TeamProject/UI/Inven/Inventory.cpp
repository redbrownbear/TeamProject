// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/Inventory.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"

#include "GameFramework/PC_InGame.h"



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
}

void UInventory::CloseUI()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);
    }

    Super::CloseUI();

}

void UInventory::InitUI()
{
    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {

        PC_InGame->ChangeInputContext(EInputContext::IC_Inventory);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        
        PC_InGame->SetInputMode(InputMode);
        PC_InGame->BindInventoryInput(this);
    }
   
    check(BP_InvenScroll); // BindWidget이 잘 됐는지 확인
}

void UInventory::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    // Deadzone 방지
    if (ActionValue.IsNearlyZero())
        return;

    // 가장 강한 방향 하나만 해석
    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        // 좌우
        if (ActionValue.X > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(1, 0));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        // 상하
        if (ActionValue.Y > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(0, -1));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(0, 1));
    }
}

void UInventory::OnConfirm(const FInputActionValue& InputActionValue)
{
}

void UInventory::OnCancel(const FInputActionValue& InputActionValue)
{
}

void UInventory::OnCreateItemTest(const FInputActionValue& InputActionValue)
{
    UInventoryManager* InvenManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    check(InvenManager);

    if (InvenManager)
    {
        const FItemData* RowData = ItemDataTable->FindRow<FItemData>(FName("testItem"), TEXT("LookupItem"));

        if (RowData)
        {
            FItemData Item;
            Item = *RowData;

            InvenManager->AddItem(Item);

        }
    }
}

void UInventory::RefreshInventory(const FItemData& ItemData)
{
    BP_InvenScroll->AddItemSlot(ItemData);
}
