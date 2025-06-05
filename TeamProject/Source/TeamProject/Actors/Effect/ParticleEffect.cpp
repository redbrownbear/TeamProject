// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effect/ParticleEffect.h"
#include "Data/ParticleEffectTableRow.h"
#include "Misc/Utils.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AParticleEffect::AParticleEffect(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
    RootComponent = DefaultSceneComponent;
    ParticleEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffectComponent"));
    ParticleEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AParticleEffect::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
    DataTableRowHandle = InDataTableRowHandle;
    if (DataTableRowHandle.IsNull()) { return; }
    FParticleEffectTableRow* Data = DataTableRowHandle.GetRow<FParticleEffectTableRow>(DataTableRowHandle.RowName.ToString());
    if (!Data) { return; }
    ParticleEffectTableRow = Data;
    ParticleEffectComponent->SetTemplate(ParticleEffectTableRow->EffectParticleSystem);

    if (ParticleEffectTableRow->bIsLifeTime)
    {
        fLifeTime = ParticleEffectTableRow->LifeTime;
    }

    ParticleEffectComponent->SetRelativeTransform(ParticleEffectTableRow->Transform);
    //ParticleEffectComponent->SetRelativeLocation(ParticleEffectTableRow->Transform.GetLocation());
    //ParticleEffectComponent->SetWorldRotation(ParticleEffectTableRow->Transform.GetRotation());
    //ParticleEffectComponent->SetWorldScale3D(ParticleEffectTableRow->Transform.GetScale3D());
}

void AParticleEffect::SetData(const FName& ParticleEffectName)
{
    if (!ParticleEffectDataTable)
    {
        ParticleEffectDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/EffectData/DT_ParticleEffect.DT_ParticleEffect'"));
        check(ParticleEffectDataTable);
    }
    if (!ParticleEffectDataTable->GetRowMap().Find(ParticleEffectName)) { ensure(false); return; }
    DataTableRowHandle.DataTable = ParticleEffectDataTable;
    DataTableRowHandle.RowName = ParticleEffectName;
    ParticleEffectTableRow = DataTableRowHandle.GetRow<FParticleEffectTableRow>(DataTableRowHandle.RowName.ToString());

    ParticleEffectComponent->SetTemplate(ParticleEffectTableRow->EffectParticleSystem);

    if (ParticleEffectTableRow->bIsLifeTime)
    {
        fLifeTime = ParticleEffectTableRow->LifeTime;
    }

    ParticleEffectComponent->SetRelativeTransform(ParticleEffectTableRow->Transform);
}

void AParticleEffect::SetParticleSystem(UParticleSystem* ParticleSystem)
{
    ParticleEffectComponent->SetTemplate(ParticleSystem);
}

void AParticleEffect::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
    Super::PostDuplicate(DuplicateMode);

    if (DuplicateMode == EDuplicateMode::Normal)
    {
        FTransform Backup = GetActorTransform();
        SetData(DataTableRowHandle);
        SetActorTransform(Backup);
    }
}

void AParticleEffect::PostLoad()
{
    Super::PostLoad();
}

void AParticleEffect::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
    Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AParticleEffect::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void AParticleEffect::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    SetData(DataTableRowHandle);
    SetActorTransform(Transform);
}

// Called when the game starts or when spawned
void AParticleEffect::BeginPlay()
{
    Super::BeginPlay();
    SetData(DataTableRowHandle);
}

// Called every frame
void AParticleEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ParticleEffectTableRow->bIsLifeTime)
    {
        fCurrentLifeTime += DeltaTime;
        if (fCurrentLifeTime > fLifeTime)
        {
            Destroy();
        }
    }
}