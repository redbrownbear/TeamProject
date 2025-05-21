// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/UI/UIManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UQuestDialogueManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestDialogueManager>();
    if (QuestManager)
    {
        OnDialogueNextRequested.AddDynamic(QuestManager, &UQuestDialogueManager::HandleNextDialogueRequested);
    }
}