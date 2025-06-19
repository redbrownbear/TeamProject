// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GateComponent/GateComponent.h"
#include "SubSystem/Puzzle/EventManager.h"

// Sets default values for this component's properties
UGateComponent::UGateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGateComponent::BeginPlay()
{
	Super::BeginPlay();

	UEventManager* EventManager = GetWorld()->GetGameInstance()->GetSubsystem<UEventManager>();
	if (EventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventManager found, adding dynamic delegate"));
		EventManager->OnOverlapTempleBall.AddDynamic(this, &UGateComponent::OpenGateSequence);
		EventManager->OnAssasinBossDead.AddDynamic(this, &UGateComponent::OpenGateSequence);
	}
}

void UGateComponent::OpenGate()
{
	UStaticMeshComponent* StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (!StaticMesh) return;

	StartLocation = StaticMesh->GetComponentLocation();
	TargetLocation = StartLocation + FVector::UpVector * StaticMesh->Bounds.BoxExtent.Z * 2.f;
	ElapsedTime = 0.f;

	GetWorld()->GetTimerManager().SetTimer(GateMoveTimer, this, &UGateComponent::MoveGateTick, 0.01f, true);

}

void UGateComponent::MoveGateTick()
{
	UStaticMeshComponent* StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (!StaticMesh) return;

	ElapsedTime += 0.01f;

	float Alpha = ElapsedTime / Duration;
	if (Alpha >= 1.f)
	{
		StaticMesh->SetWorldLocation(TargetLocation);
		GetWorld()->GetTimerManager().ClearTimer(GateMoveTimer);
		return;
	}

	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	StaticMesh->SetWorldLocation(NewLocation);
}

void UGateComponent::OpenGateSequence()
{
	// 문 열리는 시퀀스 재생 

	OpenGate();
}
