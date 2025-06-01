// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerBox/TorchTriggerBox.h"
#include "Components/ShapeComponent.h"

void ATorchTriggerBox::BeginPlay()
{
	UShapeComponent* ShapeComponent = GetCollisionComponent();
	//ShapeComponent->SetCollisionProfileName(CollisionProfileName::MyTrigger);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTrigger);
}

void ATorchTriggerBox::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
