// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Temple/Ice/IcePillar.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TimerManager.h"

// Sets default values
AIcePillar::AIcePillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	RiseTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RiseTimeline"));
}

// Called when the game starts or when spawned
void AIcePillar::BeginPlay()
{
	Super::BeginPlay();
	
	if (RiseCurve)
	{
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("AnimateRise"));
		RiseTimeline->AddInterpFloat(RiseCurve, Progress);
		RiseTimeline->PlayFromStart();
	}

	// 일정 시간 후 자동 파괴
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AIcePillar::DestroyPillar, LifeTime, false);
		});
}

// Called every frame
void AIcePillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RiseTimeline)
	{
		RiseTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

void AIcePillar::AnimateRise(float Value)
{
	FVector Location = GetActorLocation();
	Location.Z = Location.Z + (MaxHeight * Value);
	SetActorLocation(Location);
}

void AIcePillar::DestroyPillar()
{
	if (BreakEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), BreakEffect, GetActorLocation(), FRotator::ZeroRotator
		);
	}

	Destroy();
}

