// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerBox/TriggerBox_ToAssasinHideout.h"
#include "Components/ShapeComponent.h"

#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "SubSystem/PlayerManager.h"

#include "Actors/Character/PlayerCharacter.h"

#include "Misc/Utils.h"

ATriggerBox_ToAssasinHideout::ATriggerBox_ToAssasinHideout()
{
	bGenerateOverlapEventsDuringLevelStreaming = true;
	UShapeComponent* ShapeComponent = GetCollisionComponent();
	ShapeComponent->SetCollisionProfileName(CollisionProfileName::Trigger);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTrigger);
}

void ATriggerBox_ToAssasinHideout::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
				PlayerManager->SetLevelName(TextU(3003));

				TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/AssasinHideout.AssasinHideout")));
				LoadingScreenSubsystem->OpenLevelWithLoadingScreen(GameMap);
			}
		}
	}
}