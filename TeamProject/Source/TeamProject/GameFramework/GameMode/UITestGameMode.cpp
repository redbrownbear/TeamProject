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

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
}

