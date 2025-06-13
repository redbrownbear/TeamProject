// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameMode/GM_InGame.h"
#include "GameFramework/PC_InGame.h"

#include "SubSystem/UI/UIManager.h"
#include "SubSystem/PlayerManager.h"
#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"

AGM_InGame::AGM_InGame()
{
	PlayerControllerClass = APC_InGame::StaticClass();
}

void AGM_InGame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UGIS_ASyncLoadingScreen * LoadingManager = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
	if (LoadingManager)
	{
		LoadingManager->SetLoadingUI(LoadingWidgetClass);
		LoadingManager->SetLoadingUI(LoadingWidgetToTitleClass);

	}

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
		UIManager->PostWorldInitialize();
}
