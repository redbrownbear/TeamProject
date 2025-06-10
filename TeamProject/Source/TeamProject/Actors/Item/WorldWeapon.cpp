// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/WorldWeapon.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Monster/MonsterInterface.h"

#include "Data/ItemDataRow.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Misc/Utils.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AWorldWeapon::AWorldWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterial(TEXT("/Script/PhysicsCore.PhysicalMaterial'/Game/Resources/Item/PhysicsMaterial/PM_WorldWeapon.PM_WorldWeapon'"));
	PhysicalMaterial = PhysMaterial.Object;

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
}

void AWorldWeapon::SetDataWithName(const FName& WorldWeaponName)
{
	if (!ItemDataTable)
	{
		ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/ItemData/DT_WorldWeapon.DT_WorldWeapon'"));
		check(ItemDataTable);
	}
	if (TEXT("None") == WorldWeaponName) return;

	if (!ItemDataTable->GetRowMap().Find(WorldWeaponName)) { ensure(false); return; }
	DataTableRowHandle.DataTable = ItemDataTable;
	DataTableRowHandle.RowName = WorldWeaponName;
	ItemTableRow = DataTableRowHandle.GetRow<FItemData>(DataTableRowHandle.RowName.ToString());

	if (!IsValid(CollisionComponent) && ItemTableRow->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, ItemTableRow->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		//CollisionComponent->RegisterComponent();
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

		// Change RootComponent 
		SetRootComponent(CollisionComponent);
		DefaultSceneRoot->SetRelativeTransform(FTransform::Identity);
		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// check if it's CDO 
		if (!HasAnyFlags(RF_ClassDefaultObject))
		{
			CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
		}
		if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
		{
			SphereComponent->SetSphereRadius(ItemTableRow->CollisionSphereRadius);
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
			BoxComponent->SetBoxExtent(ItemTableRow->CollisionBoxExtent);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
		{
			CapsuleComponent->SetCapsuleSize(ItemTableRow->CollisionCapsuleRadius, ItemTableRow->CollisionCapsuleHalfHeight);
		}

		CollisionComponent->BodyInstance.bUseCCD = true;
		CollisionComponent->SetEnableGravity(true);
		CollisionComponent->SetSimulatePhysics(true);
	}
	else
	{
		SetRootComponent(CollisionComponent);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
	}

	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;


	// StaticMesh
	if (ItemTableRow->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemTableRow->StaticMesh);
		StaticMeshComponent->SetRelativeTransform(ItemTableRow->Transform);
		StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AWorldWeapon::SetDataWithHandle(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FItemData* Data = DataTableRowHandle.GetRow<FItemData>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	ItemTableRow = Data;

	if (!IsValid(CollisionComponent) && ItemTableRow->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, ItemTableRow->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		//CollisionComponent->RegisterComponent();
		SetRootComponent(CollisionComponent);
		//DefaultSceneRoot->SetRelativeTransform(FTransform::Identity);
		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

		if (!HasAnyFlags(RF_ClassDefaultObject))
		{
			CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
		}
		if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
		{
			SphereComponent->SetSphereRadius(ItemTableRow->CollisionSphereRadius);
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
			BoxComponent->SetBoxExtent(ItemTableRow->CollisionBoxExtent);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
		{
			CapsuleComponent->SetCapsuleSize(ItemTableRow->CollisionCapsuleRadius, ItemTableRow->CollisionCapsuleHalfHeight);
		}

		CollisionComponent->BodyInstance.bUseCCD = true;
		CollisionComponent->SetEnableGravity(true);
		CollisionComponent->SetSimulatePhysics(true);
	}
	else
	{
		SetRootComponent(CollisionComponent);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
	}

	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;

	// StaticMesh
	if (ItemTableRow->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemTableRow->StaticMesh);
		//StaticMeshComponent->SetWorldScale3D(ItemTableRow->Transform.GetScale3D());
		StaticMeshComponent->SetRelativeTransform(ItemTableRow->Transform);
		StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AWorldWeapon::SetDataWithData(const FItemData& InItemData)
{
	if (!IsValid(CollisionComponent) && InItemData.CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;

		FTransform WorldTransform = GetActorTransform();

		CollisionComponent = NewObject<UShapeComponent>(this, InItemData.CollisionClass, TEXT("CollisionComponent"));
		//CollisionComponent->RegisterComponent();
		CollisionComponent->SetWorldTransform(WorldTransform);
		SetRootComponent(CollisionComponent);

		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionComponent->SetCanEverAffectNavigation(false);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

		if (!HasAnyFlags(RF_ClassDefaultObject))
		{
			CollisionComponent->SetPhysMaterialOverride(PhysicalMaterial);
		}
		if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
		{
			SphereComponent->SetSphereRadius(InItemData.CollisionSphereRadius);
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
			BoxComponent->SetBoxExtent(InItemData.CollisionBoxExtent);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
		{
			CapsuleComponent->SetCapsuleSize(InItemData.CollisionCapsuleRadius, InItemData.CollisionCapsuleHalfHeight);
		}

		CollisionComponent->BodyInstance.bUseCCD = true;
		CollisionComponent->SetEnableGravity(false);
		CollisionComponent->SetSimulatePhysics(false);
	}
	else
	{
		SetRootComponent(CollisionComponent);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Item);
	}

	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;

	// StaticMesh
	if (InItemData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(InItemData.StaticMesh);
		StaticMeshComponent->SetWorldScale3D(FVector(45.0f, 45.0f, 45.0f));
		StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	ItemTableRow = &InItemData;
}

void AWorldWeapon::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		CollisionComponent->DestroyComponent();	
		SetActorTransform(Backup);

		if(!DataTableRowHandle.IsNull())
			SetDataWithHandle(DataTableRowHandle);
	}
}

void AWorldWeapon::PostLoad()
{
	Super::PostLoad();
}

void AWorldWeapon::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AWorldWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AWorldWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetDataWithHandle(DataTableRowHandle);
	//SetDataWithName(DataTableRowHandle.RowName);
	SetActorTransform(Transform);
}

// Called when the game starts or when spawned
void AWorldWeapon::BeginPlay()
{
	Super::BeginPlay();
	SetDataWithHandle(DataTableRowHandle);

	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>(UAISense_Sight::StaticClass()));
	StimuliSource->RegisterWithPerceptionSystem();
}

void AWorldWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AProjectile* Proj = Cast<AProjectile>(OtherActor))
	{
		if (ProjectileName::Monster_CatchItem == Proj->GetProjectileName())
		{
			if (IMonsterInterface* Monster = Cast<IMonsterInterface>(Proj->GetInstigator()))
			{
				if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
				{
					FSMComponent->SetToCatchWeapon(nullptr);
					FSMComponent->SetCatchedWeapon(this);
					FSMComponent->ChangeState(EMonsterState::Combat);

					bIsCatched = true;

					// if PhyscisSimulates activated, AttachToComponent will fail
					CollisionComponent->SetSimulatePhysics(false);
					// Offset Changed to fix outlook
					StaticMeshComponent->SetRelativeLocation(FVector::Zero());
					bool bSucceeded = this->AttachToComponent(
						Monster->GetMonsterMesh(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						Monster_SocketName::Weapon_Right);


					Proj->Destroy();
				}
			}
		}
	}
}

// Called every frame
void AWorldWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AWorldWeapon::GetWorldWeaponName() const
{
	return DataTableRowHandle.RowName;
}

EWeaponKind AWorldWeapon::GetWorldWeaponKind() const
{
	return ItemTableRow->eWeaponKind;
}

void AWorldWeapon::AddForce(FVector _Direction, float Force)
{
	CollisionComponent->AddForce(Force * _Direction);
}

void AWorldWeapon::AttachToMonster(IMonsterInterface* Monster, FName SocketName)
{
	if (!Monster) { return; }

	if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
	{
		AActor* MonsterActor = Cast<AActor>(Monster);
		SetOwner(MonsterActor);

		bIsCatched = true;

		// if PhyscisSimulates activated, AttachToComponent will fail
		CollisionComponent->SetSimulatePhysics(false);
		// Offset Changed to fix outlook
		StaticMeshComponent->SetRelativeLocation(FVector::Zero());
		const bool bSucceeded = this->AttachToComponent(
			Monster->GetMonsterMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);

		if (!bSucceeded)
		{
			check(false);
		}
	}
}

void AWorldWeapon::DetachFromMonster()
{
	SetOwner(nullptr);

	bIsCatched = false;

	StaticMeshComponent->SetRelativeLocation(ItemTableRow->Transform.GetLocation());
	StaticMeshComponent->SetRelativeScale3D(ItemTableRow->Transform.GetScale3D());

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (!IsValid(CollisionComponent) || !CollisionComponent->IsRegistered())
	{
		UE_LOG(LogTemp, Error, TEXT("AWorldWeapon: CollisionComponent is not valid or not registered after DetachFromActor!"));
		if (IsValid(CollisionComponent) && !CollisionComponent->IsRegistered())
		{
			CollisionComponent->RegisterComponentWithWorld(GetWorld());
			UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon: Attempted to re-register CollisionComponent."));
		}
		else
		{
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AWorldWeapon: CollisionComponent is valid and registered after DetachFromActor."));
	}

	CollisionComponent->SetSimulatePhysics(true);
	UE_LOG(LogTemp, Log, TEXT("AWorldWeapon: Physics simulation enabled."));

	CollisionComponent->WakeRigidBody();
}

float AWorldWeapon::GetDamage() const
{
	return ItemTableRow->Damage;
}
