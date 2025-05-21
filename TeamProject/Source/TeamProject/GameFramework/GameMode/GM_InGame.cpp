// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameMode/GM_InGame.h"
#include "GameFramework/PC_InGame.h"

#include "SubSystem/UI/InventoryManager.h"
#include "SubSystem/UI/QuestDialogueManager.h"


AGM_InGame::AGM_InGame()
{
	PlayerControllerClass = APC_InGame::StaticClass();
}
