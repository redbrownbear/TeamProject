// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/Barrier.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

#include "Data/NiagaraEffectTableRow.h"

#include "Misc/Utils.h"

ABarrier::ABarrier()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;
    StaticMeshComponent->SetCollisionProfileName(CollisionProfileName::Monster);
    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

    ConstructorHelpers::FObjectFinder<UStaticMesh> Asset
    { TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'") };

    if (Asset.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ABarrier::ABarrier // No StaticMesh"));
    }

    StaticMeshComponent->SetWorldScale3D(FVector(0.05, 1.8, 2.2));
    ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset
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

    ParticleEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffectComponent"));
    ParticleEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    ParticleEffectComponent->SetAutoActivate(true);

    ConstructorHelpers::FObjectFinder<UParticleSystem> EffectAsset
    { TEXT("/Script/Engine.ParticleSystem'/Game/Vefects/FXVarietyPack/Particles/BarrierEffect.BarrierEffect'") };

    if (EffectAsset.Object)
    {
        ParticleEffectComponent->SetTemplate(EffectAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ABarrier::ABarrier // Load Effect Asset failed"));
    }

    ParticleEffectComponent->SetRelativeScale3D(FVector(5.0, 5.0, 5.0));
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
        if (AActor* OwnerActor = Projectile->GetOwner())
        {
            if (OwnerActor->IsA<APlayerCharacter>())
            {
                Projectile->Destroy();
            }
        }
    }
}

