// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Loading/LoadingLevelScriptActor.h"
#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"

ALoadingLevelScriptActor::ALoadingLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALoadingLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	if (UGIS_ASyncLoadingScreen* LoadingSystem = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>())
	{
		// �� Subsystem���� ����� �������� �����ͼ� BeginLoading
		LoadingSystem->BeginLoading(LoadingSystem->GetPendingLevelName());
	}
}

