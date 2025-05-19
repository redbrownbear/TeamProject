// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/UITestGameMode.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"
#include "UI/Test/TestPopupUI.h"
#include "UI/Inven/Inventory.h"


void AUITestGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreatePopup();
    CreateInven();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
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
        FString Path = TEXT("/Game/BluePrint/UI/BP_TestPopup.BP_TestPopup_C");
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
        FString Path = TEXT("/Game/BluePrint/UI/BP_InvenLayout.BP_InvenLayout_C");
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
        const FItemData* RowData = ItemDataTable->FindRow<FItemData>(FName("testItem"), TEXT("LookupItem"));
  
        if (RowData)
        {     
            FItemData Item;
            Item = *RowData;

            InvenManager->AddItem(Item);
                     
        }
    }
}
