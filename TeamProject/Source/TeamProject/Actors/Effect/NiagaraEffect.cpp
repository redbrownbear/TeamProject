// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effect/NiagaraEffect.h"
#include "Data/NiagaraEffectTableRow.h"
#include "Misc/Utils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

ANiagaraEffect::ANiagaraEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;
	NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffectComponent"));
	NiagaraEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	NiagaraEffectComponent->SetAutoActivate(false);
}

void ANiagaraEffect::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FNiagaraEffectTableRow* Data = DataTableRowHandle.GetRow<FNiagaraEffectTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	NiagaraEffectData = Data;
	fLifeTime = NiagaraEffectData->LifeTime;
	NiagaraEffectComponent->SetAsset(NiagaraEffectData->EffectNiagaraSystem);
}
void ANiagaraEffect::BeginPlay()
{
	Super::BeginPlay();
	SetData(DataTableRowHandle);
}

// Called every frame
void ANiagaraEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fCurrentLifeTime += DeltaTime;
	if (fCurrentLifeTime > fLifeTime)
	{
		Destroy();
	}
}
void ANiagaraEffect::SetNiagaraSystem(UNiagaraSystem* InAsset)
{
	NiagaraEffectComponent->SetAsset(NiagaraEffectData->EffectNiagaraSystem);
}

void ANiagaraEffect::SetLifeTime(float _LifeTime)
{
	fLifeTime = _LifeTime;
}

void ANiagaraEffect::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void ANiagaraEffect::PostLoad()
{
	Super::PostLoad();
}

void ANiagaraEffect::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void ANiagaraEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ANiagaraEffect::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}
