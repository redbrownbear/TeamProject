// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"
#include "UI/Test/TestPopupUI.h"
#include "UI/Inven/Inventory.h"

#include "GameMode/UITestGameMode.h"


void AUITestGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreatePopup();
    CreateInven();
}

void AUITestGameMode::CreatePopup()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind = PC->InputComponent->BindAction("OpenPopup", IE_Pressed, this, &AUITestGameMode::OnOpenPopup);
    }
}

void AUITestGameMode::CreateInven()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind1 = PC->InputComponent->BindAction("OpenInven", IE_Pressed, this, &AUITestGameMode::OnOpenInven);
        FInputActionBinding& Bind2 = PC->InputComponent->BindAction("CreateItem", IE_Pressed, this, &AUITestGameMode::CreateItem);
    }
}

void AUITestGameMode::OnOpenPopup()
{
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        FString Path = TEXT("/Game/UIBluePrint/BP_TestPopup.BP_TestPopup_C");
        TSubclassOf<UTestPopupUI> PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);

        UTestPopupUI* NewUI = UIManager->CreateUI(GetWorld(), PopupUIBPClass);
        if (NewUI)
        {
            FText Title = FText::FromString(TEXT("Hi"));
            FText Main = FText::FromString(TEXT("Welcome"));

            NewUI->SetPopupText(Title, Main);
        }
        else
        {
            check(NewUI);
        }
    }
}

void AUITestGameMode::OnOpenInven()
{
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        //Inventory
        FString Path = TEXT("/Game/UIBluePrint/BP_InvenLayout.BP_InvenLayout_C");
        TSubclassOf<UInventory> PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);

        UInventory* NewUI = UIManager->CreateUI(GetWorld(), PopupUIBPClass);
        if (!NewUI)
        {
            check(NewUI);
        }
    }
}

void AUITestGameMode::CreateItem()
{
    UInventoryManager* InvenManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
    check(InvenManager);

    if(InvenManager)
    {
        if (DataTableRowHandle.IsNull()) { return; }
        FItemData* Data = DataTableRowHandle.GetRow<FItemData>(TEXT("testItem"));
        if (!Data) { ensure(false); return; }

        FItemData Item;
        Item.Name = Data->Name;
        Item.Icon = Data->Icon;

        InvenManager->AddItem(Item);
    }
}
