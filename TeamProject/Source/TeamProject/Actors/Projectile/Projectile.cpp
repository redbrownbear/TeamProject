// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Projectile.h"
#include "Data/ProjectileTableRow.h"
#include "Data/NiagaraEffectTableRow.h"
#include "Data/ParticleEffectTableRow.h"

#include "Misc/Utils.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/Character/PlayerMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Effect/ParticleEffect.h"
#include "Actors/Character/PlayerCharacter.h"
#include "SubSystem/PlayerManager.h"

#include "Actors/Effect/NiagaraEffect.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 3.f;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCanEverAffectNavigation(false);
	RootComponent = CollisionComponent;
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	StaticMeshComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
}

void AProjectile::SetData(const FName& ProjectileName, FName ProfileName)
{
	if (!ProjectileDataTable)
	{
		ProjectileDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/ProjectileData/DT_Projectile.DT_Projectile'"));
		check(ProjectileDataTable);
	}
	if (!ProjectileDataTable->GetRowMap().Find(ProjectileName)) { ensure(false); return; }
	DataTableRowHandle.DataTable = ProjectileDataTable;
	DataTableRowHandle.RowName = ProjectileName;
	ProjectileTableRow = DataTableRowHandle.GetRow<FProjectileTableRow>(DataTableRowHandle.RowName.ToString());

	// ignore projectile's owner 
	StaticMeshComponent->MoveIgnoreActors.Empty();
	StaticMeshComponent->MoveIgnoreActors.Add(GetOwner());

	if (ProjectileTableRow->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(ProjectileTableRow->StaticMesh);
		StaticMeshComponent->SetRelativeTransform(ProjectileTableRow->Transform);
	}

	CollisionComponent->SetCollisionProfileName(ProfileName);

	ProjectileMovementComponent->MaxSpeed = ProjectileTableRow->MaxSpeed;
	ProjectileMovementComponent->InitialSpeed = ProjectileTableRow->InitialSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = ProjectileTableRow->GravityScale;

	if (USphereComponent* SphereCom = Cast<USphereComponent>(CollisionComponent))
	{
		SphereCom->SetSphereRadius(ProjectileTableRow->CollisionSphereRadius);
	}

	SetLifeSpan(ProjectileTableRow->LifeSpan);

	if (ProjectileName::Monster_Attack == ProjectileName
	|| ProjectileName::Monster_Arrow == ProjectileName
	|| ProjectileName::Monster_LynelAttack == ProjectileName
	|| ProjectileName::Monster_AL_Attack == ProjectileName
	|| ProjectileName::Monster_AL_AttackBig == ProjectileName)

	{
		bGetDamageFromWeapon = true;
	}


	////////////////////////////
	// Effect

	const FDataTableRowHandle NiagaraEffectDataTable = ProjectileTableRow->NiagaraEffectTableRowHandle;
	if (!NiagaraEffectDataTable.IsNull())
	{
		FNiagaraEffectTableRow* Data = NiagaraEffectDataTable.GetRow<FNiagaraEffectTableRow>(NiagaraEffectDataTable.RowName.ToString());

		NiagaraEffectComponent = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), TEXT("NiagaraEffectComponent"));
		NiagaraEffectComponent->SetAsset(Data->EffectNiagaraSystem);
		NiagaraEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NiagaraEffectComponent->SetRelativeTransform(Data->Transform);
		NiagaraEffectComponent->RegisterComponent();
	}

	const FDataTableRowHandle ParticleEffectDataTable = ProjectileTableRow->ParticleEffectTableRowHandle;
	if (!ParticleEffectDataTable.IsNull())
	{
		FParticleEffectTableRow* Data = ParticleEffectDataTable.GetRow<FParticleEffectTableRow>(ParticleEffectDataTable.RowName.ToString());

		ParticleEffectComponent = NewObject<UParticleSystemComponent>(this, UParticleSystemComponent::StaticClass(), TEXT("ParticleEffectComponent"));
		ParticleEffectComponent->SetTemplate(Data->EffectParticleSystem);
		ParticleEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ParticleEffectComponent->SetRelativeTransform(Data->Transform);
		ParticleEffectComponent->RegisterComponent();
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(this)) { return; }

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(OtherActor);
	if (Player_C)
	{
		
		Player_C->Damaged(GetDamage());
	}
	if (!(DataTableRowHandle.RowName == ProjectileName::Monster_PlayerAlert))
	{
		Destroy();
	}

	//if (DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStone
	//	|| DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStoneBig)
	//{
	//	Destroy();
	//}


}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStone
		|| DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStoneBig
		|| DataTableRowHandle.RowName == ProjectileName::Monster_HinoxStone
		)
	{
		if (UWorld* World = GetWorld())
		{
			AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			ParticleEffect->SetData(ParticleEffectName::Hinox_AttackShockWave);

			const FVector Location = GetActorLocation();
			NewTransform.SetLocation(Location);

			ParticleEffect->FinishSpawning(NewTransform);
		}
		Destroy();
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
}

FVector AProjectile::GetVelocity()
{
	FVector Direction = ProjectileMovementComponent->Velocity;
	return Direction;
}

FName AProjectile::GetProjectileName()
{
	return DataTableRowHandle.RowName;
}

float AProjectile::GetDamage()
{
	if (bGetDamageFromWeapon)
	{
		if (ACharacterMonster* CM = Cast<ACharacterMonster>(GetOwner()))
		{
			if (const AWorldWeapon* WW = CM->GetFSMComponent()->GetCurrentWeapon())
			{
				float fDamage = WW->GetDamage();
				return fDamage;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AProjectile::GetDamage // No WorldWeapon"));
				return 1.f;
			}
		}
		else if (APawnMonster* PM = Cast<APawnMonster>(GetOwner()))
		{
			if (const AWorldWeapon* WW = PM->GetFSMComponent()->GetCurrentWeapon())
			{
				float fDamage = WW->GetDamage();
				return fDamage;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AProjectile::GetDamage // No WorldWeapon"));
				return 1.f;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AProjectile::GetDamage // Not Monster"));
			return 1.f;
		}
	}
	else
	{
		if (ProjectileTableRow)
		{
			return ProjectileTableRow->Damage;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AProjectile::GetDamage // No ProjectileTableRow"));
			return 1.f;
		}
	}
}

void AProjectile::SetProjectileMovementActivate(bool bFlag)
{
	ProjectileMovementComponent->Deactivate();
}

void AProjectile::SetGravityScale(float Scale)
{
	ProjectileMovementComponent->ProjectileGravityScale = Scale;
}

void AProjectile::SetStaticMeshVisibility(bool bFlag)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetVisibility(bFlag);
	}
}

void AProjectile::SetNiagaraVisibility(bool bFlag)
{
	if(bFlag)
	{
		NiagaraEffectComponent->Activate();
	}
	else
	{
		NiagaraEffectComponent->Deactivate();
	}
}

