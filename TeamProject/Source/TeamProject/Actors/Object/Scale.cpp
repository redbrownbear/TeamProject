// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/Scale.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScale::AScale()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
    RootComponent = Lever;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset0
    { TEXT("/Engine/BasicShapes/Cone.Cone") };
    if (Asset0.Object)
    {
        Lever->SetStaticMesh(Asset0.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Lever StaticMeshAsset"));
    }

    Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
    Base->SetupAttachment(Lever);

    LeftPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPlate"));
    LeftPlate->SetupAttachment(Base);
    LeftPlate->SetRelativeLocation(FVector(300.f, 0.f, 0.f));
    LeftPlate->SetRelativeScale3D(FVector(1.f, 1.f, 0.1f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset1
    { TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube") };
    if (Asset1.Object)
    {
        LeftPlate->SetStaticMesh(Asset1.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No LeftPlate StaticMeshAsset"));
    }

    RightPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPlate"));
    RightPlate->SetupAttachment(Base);
    RightPlate->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
    RightPlate->SetRelativeScale3D(FVector(1.f, 1.f, 0.1f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset2
    { TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube") };
    if (Asset2.Object)
    {
        RightPlate->SetStaticMesh(Asset2.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No RightPlate StaticMeshAsset"));
    }
    
    LeftWeightArea = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeightArea"));
    LeftWeightArea->SetupAttachment(LeftPlate);
    LeftWeightArea->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

    RightWeightArea = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeightArea"));
    RightWeightArea->SetupAttachment(RightPlate);
    RightWeightArea->SetRelativeLocation(FVector(0.f, 0.f, 100.f));    
}

void AScale::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    static float PrevLeftWeight = LeftWeight;
    static float PrevRightWeight = RightWeight;

    UpdateWeight();

    if (!FMath::IsNearlyEqual(PrevLeftWeight, LeftWeight) || !FMath::IsNearlyEqual(PrevRightWeight, RightWeight))
    {
        ApplyOffsetFromWeight();
        PrevLeftWeight = LeftWeight;
        PrevRightWeight = RightWeight;
    }
}

void AScale::BeginPlay()
{
    Super::BeginPlay();
    
    LeftStart = LeftPlate->GetRelativeLocation();
    RightStart = RightPlate->GetRelativeLocation();
}

void AScale::UpdateWeight()
{
    constexpr float Gravity = 980.0f; 

    TArray<AActor*> LeftOverlappingActors;
    TArray<AActor*> RightOverlappingActors;

    LeftWeightArea->GetOverlappingActors(LeftOverlappingActors);
    for (AActor* Actor : LeftOverlappingActors)
    {
        TArray<UPrimitiveComponent*> Components;
        Actor->GetComponents<UPrimitiveComponent>(Components);
        for (UPrimitiveComponent* Comp : Components)
        {
            if (Comp->IsSimulatingPhysics())
            {
                LeftWeight += Comp->GetMass() * Gravity;
            }
        }
    }

    RightWeightArea->GetOverlappingActors(RightOverlappingActors);
    for (AActor* Actor : RightOverlappingActors)
    {
        TArray<UPrimitiveComponent*> Components;
        Actor->GetComponents<UPrimitiveComponent>(Components);
        for (UPrimitiveComponent* Comp : Components)
        {
            if (Comp->IsSimulatingPhysics())
            {
                RightWeight += Comp->GetMass() * Gravity;
            }
        }
    }
}

void AScale::ApplyOffsetFromWeight()
{
    float MaxOffset = 50.0f; // 최대 이동 거리 (cm 기준)
    float DampingFactor = 5.0f;

    float WeightDiff = LeftWeight - RightWeight;
    float TargetOffset = FMath::Clamp(WeightDiff * MaxOffset * 0.01f, -MaxOffset, MaxOffset);    

    FVector LeftTarget = LeftStart + FVector(0.f, 0.f, -TargetOffset);
    FVector RightTarget = RightStart + FVector(0.f, 0.f, TargetOffset);

    FVector LeftCurrent = LeftPlate->GetRelativeLocation();
    FVector RightCurrent = RightPlate->GetRelativeLocation();

    LeftPlate->SetRelativeLocation(FMath::VInterpTo(LeftCurrent, LeftTarget, GetWorld()->GetDeltaSeconds(), DampingFactor));
    RightPlate->SetRelativeLocation(FMath::VInterpTo(RightCurrent, RightTarget, GetWorld()->GetDeltaSeconds(), DampingFactor));
}
