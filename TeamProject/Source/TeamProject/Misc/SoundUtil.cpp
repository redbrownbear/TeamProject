// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/SoundUtil.h"
#include "SubSystem/Sound/SoundManager.h"

void SoundU(ESoundType SoundType)
{
	if (!GEngine)
		return;

	UWorld* World = GEngine->GetCurrentPlayWorld();
	if (!World)
	{
		World = GEngine->GetWorldContexts().Num() > 0 ? GEngine->GetWorldContexts()[0].World() : nullptr;
	}
	if (!World) 
		return;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) 
		return;

	USoundManager* SoundManager = GameInstance->GetSubsystem<USoundManager>();
	if (!SoundManager) 
		return;

	SoundManager->PlaySoundByID(SoundType);
}
