#include "Actors/Object/MetalActor.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MetalComponent/MetalComponent.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Data/MetalActorTableRow.h"

#include "Misc/Utils.h"

// Sets default values
AMetalActor::AMetalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Game/Temple/MetalActors/PM_MetalActor.PM_MetalActor"));
	//PhysicalMaterial = PhysMaterial.Object;

	MetalComponent = CreateDefaultSubobject<UMetalComponent>(TEXT("MetalComponent"));
}

// Called when the game starts or when spawned
void AMetalActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!DataTableRowHandle.IsNull())
	{
		SetData(DataTableRowHandle);
	}
}

void AMetalActor::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		CollisionComponent->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void AMetalActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void AMetalActor::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMetalActorTableRow* Data = DataTableRowHandle.GetRow<FMetalActorTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MetalActorData = Data;

	// Init Collision
	if (!IsValid(CollisionComponent) && MetalActorData->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, MetalActorData->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		SetRootComponent(CollisionComponent);
		//DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionComponent->SetCanEverAffectNavigation(false);
		if (MetalActorData->CollisionProfileName == TEXT("None"))
		{
			CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
		}
		else
		{
			CollisionComponent->SetCollisionProfileName(MetalActorData->CollisionProfileName);
		}
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

		if (IsValid(PhysicalMaterial))
		{
			if (!HasAnyFlags(RF_ClassDefaultObject))
			{
				CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AMetalActor::SetData // Data's PhysicalMaterial is InValid"));
		}

		if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
		{
			SphereComponent->SetSphereRadius(MetalActorData->CollisionSphereRadius);
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
			BoxComponent->SetBoxExtent(MetalActorData->CollisionBoxExtent);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
		{
			CapsuleComponent->SetCapsuleSize(MetalActorData->CollisionCapsuleRadius, MetalActorData->CollisionCapsuleHalfHeight);
		}

		CollisionComponent->BodyInstance.bUseCCD = true;
		CollisionComponent->SetEnableGravity(true);
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;

		CollisionComponent->SetMassOverrideInKg(NAME_None, MetalActorData->MassInKg);
		CollisionComponent->BodyInstance.bOverrideMass = true;
		CollisionComponent->BodyInstance.UpdateMassProperties();
	}
	else
	{

	}


	// StaticMesh
	if (MetalActorData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(MetalActorData->StaticMesh);
		//StaticMeshComponent->SetWorldScale3D(ItemTableRow->Transform.GetScale3D());
		StaticMeshComponent->SetRelativeTransform(MetalActorData->MeshTransform);
		StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		UMaterialInterface* CurrentMaterialOnMesh = StaticMesh->GetMaterial(0);

		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentMaterialOnMesh);

		if (!DynamicMaterialInstance)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterialOnMesh, this);

			if (DynamicMaterialInstance)
			{
				StaticMeshComponent->SetMaterial(0, DynamicMaterialInstance);
				UE_LOG(LogTemp, Log, TEXT("MetalActor::SetData // Created and assigned new DynamicMaterialInstance"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MetalActor::SetData // Failed to create and assign new DynamicMaterialInstance"));
				DynamicMaterialInstance = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("MetalActor::SetData // Re-using existing DynamicMaterialInstance"));
		}
	}
}

void AMetalActor::SetData(const FName& MetalActorName)
{
	if (!MetalActorDataTable)
	{
		MetalActorDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/ActorData/DT_MetalActor.DT_MetalActor'"));
		check(MetalActorDataTable);
	}
	if (!MetalActorDataTable->GetRowMap().Find(MetalActorName)) { ensure(false); return; }
	DataTableRowHandle.DataTable = MetalActorDataTable;
	DataTableRowHandle.RowName = MetalActorName;

	MetalActorData = DataTableRowHandle.GetRow<FMetalActorTableRow>(DataTableRowHandle.RowName.ToString());

	// Init Collision
	if (!IsValid(CollisionComponent) && MetalActorData->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, MetalActorData->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		SetRootComponent(CollisionComponent);
		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionComponent->SetCanEverAffectNavigation(false);
		if (MetalActorData->CollisionProfileName == TEXT("None"))
		{
			CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
		}
		else
		{
			CollisionComponent->SetCollisionProfileName(MetalActorData->CollisionProfileName);
		}
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

		if (IsValid(PhysicalMaterial))
		{
			if (!HasAnyFlags(RF_ClassDefaultObject))
			{
				CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AMetalActor::SetData // Data's PhysicalMaterial is InValid"));
		}

		if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
		{
			SphereComponent->SetSphereRadius(MetalActorData->CollisionSphereRadius);
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
			BoxComponent->SetBoxExtent(MetalActorData->CollisionBoxExtent);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
		{
			CapsuleComponent->SetCapsuleSize(MetalActorData->CollisionCapsuleRadius, MetalActorData->CollisionCapsuleHalfHeight);
		}

		CollisionComponent->BodyInstance.bUseCCD = true;
		CollisionComponent->SetEnableGravity(true);
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	}
	else
	{

	}


	// StaticMesh
	if (MetalActorData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(MetalActorData->StaticMesh);
		//StaticMeshComponent->SetWorldScale3D(ItemTableRow->Transform.GetScale3D());
		StaticMeshComponent->SetRelativeTransform(MetalActorData->MeshTransform);
		StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMetalActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMetalActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AMetalActor::ThisIsMetal()
{
	// Change Material Color	
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue("Color", 1.f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMetalActor::ThisIsMetal // DynamicMaterialInstance is Invalid"));
	}
}

void AMetalActor::ChangeNomalColor()
{
	// Change Material Color	
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue("Color", 0.f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMetalActor::ChangeNomalColor // DynamicMaterialInstance is Invalid"));
	}
}

