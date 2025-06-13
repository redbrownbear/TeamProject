// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/BurningActor.h"
#include "Actors/Object/LockedGate.h"
#include "Actors/Projectile/Arrow/Projectile_Arrow.h"

#include "SubSystem/Puzzle/TorchManager.h"

#include "Components/BoxComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Misc/Utils.h"

// Sets default values
ABurningActor::ABurningActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset0
    { TEXT("/Game/Resources/Map/AssasinHideout/FldObj_AssassinDoor_A_02.FldObj_AssassinDoor_A_02") };
    if (Asset0.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset0.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ABurningActor::ABurningActor // No BurningActor StaticMeshAsset"));
    }

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireAsset
    { TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Free_Fire/Shared/Particles/NS_Fire_Small.NS_Fire_Small'") };

    if (FireAsset.Object)
    {
        NiagaraComponent->SetAsset(FireAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACampFire::ACampFire // No Fire NiagaraAsset"));
    }

    NiagaraComponent->SetupAttachment(RootComponent);
    NiagaraComponent->SetRelativeLocation(FVector(0.f, 230.f, 0.f)); 
    NiagaraComponent->SetRelativeScale3D(FVector(10.f, 35.f, 1.f));
    NiagaraComponent->Deactivate();

    TriggerColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerColliderComponent"));
    TriggerColliderComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
    TriggerColliderComponent->SetupAttachment(RootComponent);
    TriggerColliderComponent->SetRelativeLocation(FVector(0.f, 310.f, 0.f));
    TriggerColliderComponent->SetRelativeScale3D(FVector(3.f, 10.f, 1.f));
    TriggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ABurningActor::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ABurningActor::BeginPlay()
{
	Super::BeginPlay();
	
    
}

// Called every frame
void ABurningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABurningActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AProjectile_Arrow* Arrow = Cast<AProjectile_Arrow>(OtherActor))
    {
        if (ProjectileName::Player_FireArrow != Arrow->GetProjectileName()) return;

        SetBurningActor(true);
    }
}

void ABurningActor::DestroyThisActor()
{
    Destroy();
}

void ABurningActor::SetBurningActor(bool _bool)
{
    bIsBurning = _bool;

    if (_bool)
    {
        SetNiagaraVisibility(true);

        if (NiagaraComponent && NiagaraComponent->IsActive())
        {
            GetWorldTimerManager().SetTimer(
                DestroyTimerHandle,
                this,
                &ABurningActor::DestroyThisActor,
                3.0f,
                false
            );
        }
    }
    else
    {
        SetNiagaraVisibility(false);
    }

}

void ABurningActor::SetNiagaraVisibility(bool bFlag)
{
    if (!NiagaraComponent)
    {
        return;
    }

    if (bFlag)
    {
        NiagaraComponent->Activate();
    }
    else
    {
        NiagaraComponent->Deactivate();
    }
}