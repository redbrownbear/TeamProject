// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/TextUtil.h"
#include "SubSystem/TextManager.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

FString TextU(int32 TextID)
{
	if (!GEngine) return TEXT(" ");

	UWorld* World = GEngine->GetCurrentPlayWorld();
	if (!World)
	{
		// ��ü ���: UGameplayStatics
		World = GEngine->GetWorldContexts().Num() > 0 ? GEngine->GetWorldContexts()[0].World() : nullptr;
	}
	if (!World) return TEXT(" ");

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return TEXT(" ");

	UTextManager* TextManager = GameInstance->GetSubsystem<UTextManager>();
	if (!TextManager) return TEXT(" ");

	return TextManager->GetTextByID(TextID);
}