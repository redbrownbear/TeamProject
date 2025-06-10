// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inven/Inventory.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"
#include "SubSystem/PlayerManager.h"

#include "Components/CanvasPanelSlot.h"

#include "GameFramework/PC_InGame.h"


void UInventory::OnCreated()
{

}

void UInventory::ShowUI()
{
    Super::ShowUI();

    APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC_InGame)
    {
        PC_InGame->ChangeInputContext(EInputContext::IC_UI);

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC_InGame->SetInputMode(InputMode);
    }

    SetRupeeUI();
    InitUI();
    BindDelegates();
}

void UInventory::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    RemoveDelegate();
    Super::HideUI(UInventory::StaticClass());
}

void UInventory::InitUI()
{
    check(BP_InvenScroll);
    check(BP_InvenEquip);
}

void UInventory::BindDelegates()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnInventoryUpdated.AddDynamic(this, &UInventory::RefreshInventory);
        PlayerManager->OnInventoryAllUpdated.AddDynamic(this, &UInventory::RefreshAllInventory);

        PlayerManager->OnInvenEquipItemAllUpdated.AddDynamic(this, &UInventory::RefreshEquip);
    }

    BP_InvenScroll->OnInventoryDescriptionUpdated.AddDynamic(this, &UInventory::RefreshDescription);
}

void UInventory::RemoveDelegate()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnInventoryUpdated.RemoveDynamic(this, &UInventory::RefreshInventory);
        PlayerManager->OnInventoryAllUpdated.RemoveDynamic(this, &UInventory::RefreshAllInventory);

        PlayerManager->OnInvenEquipItemAllUpdated.RemoveDynamic(this, &UInventory::RefreshEquip);
    }

    BP_InvenScroll->OnInventoryDescriptionUpdated.RemoveDynamic(this, &UInventory::RefreshDescription);
}

void UInventory::SetRupeeUI()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        CoinText->SetText(FText::FromString(FString::FromInt(PlayerManager->GetRupee())));
    }
}

void UInventory::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    if (ActionValue.IsNearlyZero())
        return;

    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        if (ActionValue.X > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(1, 0));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        if (ActionValue.Y > 0)
            BP_InvenScroll->MoveSelection(FIntPoint(0, -1));
        else
            BP_InvenScroll->MoveSelection(FIntPoint(0, 1));
    }
}

void UInventory::OnConfirm(const FInputActionValue& InputActionValue)
{
    if (BP_InvenSelectSet->IsVisible())
        return;

    FVector2D ItemWidgetPosition = BP_InvenScroll->GetItemListLocation();
    FVector2D Offset(100.f, -100.0f); 

    BP_InvenSelectSet->ShowButton(true, BP_InvenScroll->GetCurItemData());

    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BP_InvenSelectSet->Slot))
    {
        CanvasSlot->SetPosition(ItemWidgetPosition + Offset);
    }
}

void UInventory::OnCancel(const FInputActionValue& InputActionValue)
{
    HideUI(UInventory::StaticClass());
}

void UInventory::OnCreateItemInWorld(const FInputActionValue& InputActionValue)
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (!PlayerManager || !WorldItemActorClass) return;

    if (BP_InvenScroll->IsEmptyItem())
        return;

    const FItemData& SelectedItem = BP_InvenScroll->GetCurItemData();

    FItemData DroppedItem = PlayerManager->RemoveItemByUniqueID(SelectedItem.UniqueID);

    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    APawn* Pawn = PC->GetPawn();
    if (!Pawn) return;

    FVector SpawnLocation = Pawn->GetActorLocation() + Pawn->GetActorForwardVector() * 100.f + FVector(0, 0, 50.f);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Pawn;

    AWorldWeapon* SpawnedActor = World->SpawnActor<AWorldWeapon>(WorldItemActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (SpawnedActor)
    {
        SpawnedActor->SetDataWithData(DroppedItem);
    }
}

void UInventory::OnCreateItemTest(const FInputActionValue& InputActionValue)
{
    UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

    if (PlayerManager && InventoryManager)
    {
        TArray<FItemData> FoundRows  = InventoryManager->GetAllItemData();
        if (!FoundRows.IsEmpty())
        {
            if (FoundRows.Num() > 0)
            {
                int32 RandomIndex = FMath::RandRange(0, FoundRows.Num() - 1);
                FItemData RandomItem = (FoundRows)[RandomIndex];
                RandomItem.UniqueID = FGuid::NewGuid().ToString();
                PlayerManager->SetInvenData(RandomItem);

                // ��� ��
                UE_LOG(LogTemp, Log, TEXT("���� Name: %s"), *RandomItem.Name);
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

}

void UInventory::RefreshAllInventory(const TArray<FItemData>& ItemDataList)
{
    BP_InvenScroll->UpdateSlots(ItemDataList);
}

void UInventory::RefreshDescription(const FItemData& ItemData)
{
    BP_InvenEquip->RefreshDescription(ItemData);
}

void UInventory::RefreshEquip(const TArray<FItemData>& ItemDataMap)
{
    BP_InvenEquip->SetEquipMakeData(ItemDataMap);
    BP_InvenScroll->EquipCurrentItem();
}
