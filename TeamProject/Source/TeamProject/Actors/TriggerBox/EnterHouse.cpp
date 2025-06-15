// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerBox/EnterHouse.h"
#include "Components/ShapeComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

void AEnterHouse::BeginPlay()
{
	UShapeComponent* ShapeComponent = GetCollisionComponent();
	//ShapeComponent->SetCollisionProfileName(CollisionProfileName::MyTrigger);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTrigger);
}

void AEnterHouse::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(OtherActor);
	Player_C->SetSpringArmTargetArmLength(SPRINGARM_TARGETARM_LENGTH_HOUSE);
	
}
