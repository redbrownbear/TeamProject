// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/BossLockedGate.h"
#include "SubSystem/Puzzle/EventManager.h"
#include "Components/GateComponent/GateComponent.h"

// Sets default values
ABossLockedGate::ABossLockedGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	GateComponent = CreateDefaultSubobject<UGateComponent>(TEXT("GateComponent"));
}

// Called when the game starts or when spawned
void ABossLockedGate::BeginPlay()
{
	Super::BeginPlay();
	
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}

	/*UEventManager* EventManager = GetGameInstance()->GetSubsystem<UEventManager>();
	if (EventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventManager found, adding dynamic delegate"));
		
		EventManager->OnAssasinBossDead.AddDynamic(this, &ABossLockedGate::OpenFinalGate);
	}*/
}

//void ABossLockedGate::OpenFinalGate()
//{
//	StartLocation = StaticMeshComponent->GetComponentLocation();
//	TargetLocation = StartLocation + FVector::BackwardVector * GetComponentsBoundingBox().GetSize().Y;
//	ElapsedTime = 0.f;
//
//	GetWorldTimerManager().SetTimer(GateMoveTimer, this, &ABossLockedGate::MoveGateTick, 0.01f, true);
//}
//
//void ABossLockedGate::ClearDungeon()
//{
//	// 클리어 되었다는 시퀀스 재생
//}
//
//void ABossLockedGate::MoveGateTick()
//{
//	ElapsedTime += 0.01f;
//
//	float Alpha = ElapsedTime / Duration;
//	if (Alpha >= 1.f)
//	{
//		StaticMeshComponent->SetWorldLocation(TargetLocation);
//		GetWorldTimerManager().ClearTimer(GateMoveTimer);
//		return;
//	}
//
//	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
//	StaticMeshComponent->SetWorldLocation(NewLocation);
//}