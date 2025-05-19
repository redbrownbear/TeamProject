// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/GameMode/UITestGameMode.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/InventoryManager.h"
#include "SubSystem//UI/QuestDialogueManager.h"

#include "UI/Test/TestPopupUI.h"
#include "UI/Inven/Inventory.h"
#include "UI/NpcDialogue/NPCDialogue.h"


void AUITestGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreateDialogueBox();
    CreateInven();


    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
}

void AUITestGameMode::CreateDialogueBox()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind1 = PC->InputComponent->BindAction("OpenPopup", IE_Pressed, this, &AUITestGameMode::OnOepnDialogueBox);
        FInputActionBinding& Bind2 = PC->InputComponent->BindAction("Dialogue", IE_Pressed, this, &AUITestGameMode::OnpenQuest);
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

void AUITestGameMode::OnOpenInven()
{
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        //Inventory
        FString Path = TEXT("/Game/BluePrint/UI/Inventory/BP_InvenLayout.BP_InvenLayout_C");
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

void AUITestGameMode::OnOepnDialogueBox()
{
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        FString Path = TEXT("/Game/BluePrint/UI/NpcDialogue/BPD_NpcDialogue.BPD_NpcDialogue_C");
        TSubclassOf<UNPCDialogue> PopupUIBPClass = LoadClass<UBaseUI>(nullptr, *Path);

        UNPCDialogue* NewUI = UIManager->CreateUI(GetWorld(), PopupUIBPClass);
        if (!NewUI)
        {
            check(NewUI);
        }
    }
}

void AUITestGameMode::OnpenQuest()
{
    //Çª¸®ÄÚ ÀÓ½Ã

    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    check(QuestManager);
    if (QuestManager)
    {

        QuestManager->ShowDialogue(EQuestCharacter::Furiko);
    }

}
