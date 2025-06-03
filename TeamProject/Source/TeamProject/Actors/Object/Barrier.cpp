// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/Barrier.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/Projectile/Projectile.h"

#include "Misc/Utils.h"

ABarrier::ABarrier()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;
    StaticMeshComponent->SetCollisionProfileName(CollisionProfileName::Monster);
    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset
    { TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'") };

    if (Asset.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ABarrier::ABarrier // No StaticMesh"));
    }

    StaticMeshComponent->SetWorldScale3D(FVector(0.05, 1.2, 1.6));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset
    { TEXT("/Script/Engine.Material'/Game/Resources/Monster/Assasin_Boss/M_Barrier.M_Barrier'") };

    if (MaterialAsset.Object)
    {
        // StaticMeshComponent의 0번 슬롯에 머티리얼 적용
        StaticMeshComponent->SetMaterial(0, MaterialAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ABarrier::ABarrier // Failed to load Material."));
    }
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    LifeTime += DeltaTime;

    if (LifeTime > ASSASIN_BOSS_BARRIER_MAX_TIME)
    {
        Destroy();
    }

}

void ABarrier::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
    {
        Projectile->Destroy();
    }
}

