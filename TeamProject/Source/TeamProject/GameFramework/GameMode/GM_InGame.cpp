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

	//플레이어를 소환할때 이쪽에서 저장해둔 Transform값을 넣어주면 된다.
	//FTransform SavedTransform;
	//RestartPlayerAtTransform(NewPlayer, SavedTransform);

	UGIS_ASyncLoadingScreen * LoadingManager = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
	if (LoadingManager)
		LoadingManager->SetLoadingUI(LoadingWidgetClass);
	

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
		UIManager->PostWorldInitialize();
}
