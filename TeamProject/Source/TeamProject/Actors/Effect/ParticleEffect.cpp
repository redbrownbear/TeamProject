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
	ParticleEffectData = Data;
	ParticleEffectComponent->SetTemplate(ParticleEffectData->EffectParticleSystem);
	fLifeTime = ParticleEffectData->LifeTime;
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
	fCurrentLifeTime += DeltaTime;
	if (fCurrentLifeTime > fLifeTime)
	{
		Destroy();
	}
}

