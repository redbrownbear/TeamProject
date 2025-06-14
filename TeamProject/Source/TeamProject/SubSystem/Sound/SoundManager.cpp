// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/Sound/SoundManager.h"

#include "Data/SoundDataRow.h"
#include "Kismet/GameplayStatics.h"

USoundManager::USoundManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> SoundTableAsset(TEXT("/Game/Data/Sound/DT_Sound.DT_Sound"));
	if (SoundTableAsset.Succeeded())
	{
		SoundDataTable = SoundTableAsset.Object;
	}
}

void USoundManager::PlaySoundByID(ESoundType SoundType) const
{
	if (!SoundDataTable || !GEngine)
		return;

	UWorld* World = GEngine->GetCurrentPlayWorld();
	if (!World)
	{
		// 대체 방법: 월드 컨텍스트가 없으면 첫 번째 컨텍스트 시도
		if (GEngine->GetWorldContexts().Num() > 0)
		{
			World = GEngine->GetWorldContexts()[0].World();
		}
	}
	if (!World)
		return;

	const int32 TargetID = static_cast<int32>(SoundType);

	TArray<FSoundData*> AllRows;
	SoundDataTable->GetAllRows(TEXT("SoundLookup"), AllRows);

	for (FSoundData* Row : AllRows)
	{
		if (Row && Row->SoundID == TargetID)
		{
			USoundBase* Sound = nullptr;

			if (Row->Sound.IsValid())
				Sound = Row->Sound.Get();
			else
				Sound = Row->Sound.LoadSynchronous();

			if (Sound)
			{
				UGameplayStatics::PlaySound2D(World, Sound, Row->Volume, Row->Pitch);
			}
			break;
		}
	}
}
