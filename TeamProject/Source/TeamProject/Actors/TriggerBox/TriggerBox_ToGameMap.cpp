// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerBox/TriggerBox_ToGameMap.h"
#include "Components/ShapeComponent.h"
#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Misc/Utils.h"

ATriggerBox_ToGameMap::ATriggerBox_ToGameMap()
{
	bGenerateOverlapEventsDuringLevelStreaming = true;
	UShapeComponent* ShapeComponent = GetCollisionComponent();
	ShapeComponent->SetCollisionProfileName(CollisionProfileName::Trigger);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTrigger);
}

void ATriggerBox_ToGameMap::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		UGIS_ASyncLoadingScreen* LoadingScreenSubsystem = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
		//UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Loading.UI_Loading_C'"));
		//LoadingScreenSubsystem->ShowLoadingScreen(WidgetClass);

		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameMap"));
	}
}
