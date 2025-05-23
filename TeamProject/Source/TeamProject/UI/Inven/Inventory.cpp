// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/Inventory.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"

#include "UI/HUD/MainHUD.h"

#include "GameFramework/PC_InGame.h"



void UInventory::OnCreated()
{
    InitUI();
    BindDelegates();
}

void UInventory::ShowUI()
{
    Super::ShowUI();

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

        //HUD������
        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if(HUD)
            HUD->SetMainHUDVisible(false);
    }
}

void UInventory::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    //HUD������

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_InGame);

        AMainHUD* HUD = Cast<AMainHUD>(PC_InGame->GetHUD());
        if (HUD)
            HUD->SetMainHUDVisible(true);

    }

    Super::HideUI(UInventory::StaticClass());
}

void UInventory::InitUI()
{
    check(BP_InvenScroll); // BindWidget�� �� �ƴ��� Ȯ��
    check(BP_InvenEquip);
}

void UInventory::BindDelegates()
{
    UInventoryManager* InvenManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    check(InvenManager);
    if (InvenManager)
    {
        InvenManager->OnInventoryUpdated.AddDynamic(this, &UInventory::RefreshInventory);
    }
}

void UInventory::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    // Deadzone ����
    if (ActionValue.IsNearlyZero())
        return;

    // ���� ���� ���� �ϳ��� �ؼ�
    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        // �¿�
        if (ActionValue.X > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(1, 0));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        // ����
        if (ActionValue.Y > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(0, -1));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(0, 1));
    }
}

void UInventory::OnConfirm(const FInputActionValue& InputActionValue)
{
    HideUI(UInventory::StaticClass());
}

void UInventory::OnCancel(const FInputActionValue& InputActionValue)
{
    HideUI(UInventory::StaticClass());
}

void UInventory::OnCreateItemTest(const FInputActionValue& InputActionValue)
{
    // ���� �ٲ�� ��
    UInventoryManager* InvenManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    check(InvenManager);

    if (InvenManager)
    {
        TArray<TSharedPtr<const FItemData>> FoundRows;
        FItemData Item;


        TArray<FName> RowNames = ItemDataTable->GetRowNames();
        for (const FName& RowName : RowNames)
        {
            FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, "Item");
            if (Row)
            {
                // ���� Row ���纻 ����
                FItemData* NewRow = new FItemData(*Row);
                TSharedPtr<const FItemData> SharedRow = MakeShareable(NewRow);
         
                FoundRows.Add(SharedRow);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to find row for name: %s"), *RowName.ToString());
            }
        }

        if (!FoundRows.IsEmpty())
        {
            if (FoundRows.Num() > 0)
            {
                int32 RandomIndex = FMath::RandRange(0, FoundRows.Num() - 1);
                TSharedPtr<const FItemData> RandomItem = (FoundRows)[RandomIndex];
                InvenManager->AddItem(*RandomItem);

                // ��� ��
                UE_LOG(LogTemp, Log, TEXT("���� Name: %s"), *RandomItem->Name);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("������ �����Ͱ� ���ų� FoundRowsPtr�� null�Դϴ�."));
            }
        }
    }
}

void UInventory::RefreshInventory(const FItemData& ItemData)
{
    BP_InvenScroll->AddItemSlot(ItemData);

    RefreshEquip(ItemData);
}

void UInventory::RefreshEquip(const FItemData& ItemData)
{
    BP_InvenEquip->RefreshDescription(ItemData);
}
