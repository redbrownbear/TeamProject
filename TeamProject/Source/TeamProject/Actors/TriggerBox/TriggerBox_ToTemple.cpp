// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerBox/TriggerBox_ToTemple.h"
#include "Components/ShapeComponent.h"

#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "SubSystem/PlayerManager.h"

#include "Actors/Character/PlayerCharacter.h"

#include "Misc/Utils.h"

ATriggerBox_ToTemple::ATriggerBox_ToTemple()
{
	bGenerateOverlapEventsDuringLevelStreaming = true;
	UShapeComponent* ShapeComponent = GetCollisionComponent();
	ShapeComponent->SetCollisionProfileName(CollisionProfileName::Trigger);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTrigger);
}

void ATriggerBox_ToTemple::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		UGIS_ASyncLoadingScreen* LoadingScreenSubsystem = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
		if (LoadingScreenSubsystem)
		{
			if (UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>())
			{
				sPlayerStatus Status = PlayerManager->GetPlayerStatus();
				Status.PreviousLoction = Player->GetActorLocation() - Player->GetActorForwardVector() * 200.f;
				PlayerManager->SetPlayerStatus(Status);

				TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/WaterDungeon.WaterDungeon")));
				LoadingScreenSubsystem->OpenLevelWithLoadingScreen(GameMap);
			}
		}
	}
}
