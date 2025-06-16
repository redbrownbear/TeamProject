// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Projectile.h"
#include "Data/ProjectileTableRow.h"
#include "Data/NiagaraEffectTableRow.h"
#include "Data/ParticleEffectTableRow.h"

#include "Misc/Utils.h"
//#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "Components/ProjectileMovementComponent/MyProjectileMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Effect/NiagaraEffect.h"
#include "Actors/Effect/ParticleEffect.h"
#include "Actors/Object/TorchStand.h"

#include "SubSystem/PlayerManager.h"
#include "SubSystem/TimeManager.h"

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

	ProjectileMovementComponent = CreateDefaultSubobject<UMyProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
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

	const FDataTableRowHandle NiagaraEffectDataTable = ProjectileTableRow->TrailEffectTableRowHandle;
	if (!NiagaraEffectDataTable.IsNull())
	{
		FNiagaraEffectTableRow* Data = NiagaraEffectDataTable.GetRow<FNiagaraEffectTableRow>(NiagaraEffectDataTable.RowName.ToString());

		NiagaraEffectComponent = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), TEXT("NiagaraEffectComponent"));
		NiagaraEffectComponent->SetAsset(Data->EffectNiagaraSystem);
		NiagaraEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NiagaraEffectComponent->SetRelativeTransform(Data->Transform);
		//NiagaraEffectComponent->SetRelativeLocation(Data->Transform.GetLocation());
		//NiagaraEffectComponent->SetRelativeRotation(GetActorForwardVector().Rotation().Quaternion());
		NiagaraEffectComponent->RegisterComponent();
	}

	//Trail

	const FDataTableRowHandle TrailEffectTable = ProjectileTableRow->NiagaraEffectTableRowHandle;
	if (!TrailEffectTable.IsNull())
	{
		FNiagaraEffectTableRow* Data = NiagaraEffectDataTable.GetRow<FNiagaraEffectTableRow>(TrailEffectTable.RowName.ToString());
		if (Data)
		{

			TrailEffectFX = Data->EffectNiagaraSystem;

			for (auto& SocketName : ProjectileTableRow->TrailSockets)
			{

				UNiagaraComponent* NewTrail = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), SocketName);
				NewTrail->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				NewTrail->SetAsset(TrailEffectFX);


				Trails.Add(NewTrail);
			}
		}
	}

	const FDataTableRowHandle ParticleEffectDataTable = ProjectileTableRow->ParticleEffectTableRowHandle;
	if (!ParticleEffectDataTable.IsNull())
	{
		FParticleEffectTableRow* Data = ParticleEffectDataTable.GetRow<FParticleEffectTableRow>(ParticleEffectDataTable.RowName.ToString());

		ParticleEffectComponent = NewObject<UParticleSystemComponent>(this, UParticleSystemComponent::StaticClass(), TEXT("ParticleEffectComponent"));
		ParticleEffectComponent->SetTemplate(Data->EffectParticleSystem);
		ParticleEffectComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ParticleEffectComponent->SetRelativeTransform(Data->Transform);
		//ParticleEffectComponent->SetRelativeLocation(Data->Transform.GetLocation());
		//ParticleEffectComponent->SetWorldRotation(GetActorForwardVector().Rotation().Quaternion());
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
		if (Player_C->GetIsParry())
		{
			if (DataTableRowHandle.RowName == ProjectileName::Monster_Attack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_LynelAttack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_AL_Attack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_LynelHorn)
			{
				static UTimeManagerSubsystem* TimeManager = GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
				TimeManager->SetTimeScale(TIMESCALE_JUST);
				TimeManager->SetJust();

				if (GetOwner()->IsA<APawnMonster>())
				{
					APawnMonster* PawnMonster = Cast<APawnMonster>(GetOwner());
					if (UMonsterFSMComponent* MonsterFSMComponent = PawnMonster->GetFSMComponent())
					{
						MonsterFSMComponent->ChangeState(EMonsterState::Stun);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap // No FSMComponent"));
					}
				}
				else if (GetOwner()->IsA<ACharacterMonster>())
				{
					ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(GetOwner());
					if (UMonsterFSMComponent* MonsterFSMComponent = CharacterMonster->GetFSMComponent())
					{
						MonsterFSMComponent->ChangeState(EMonsterState::Stun);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap // No FSMComponent"));
						check(false);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap // Unexpected MonsterType"));
					check(false);
				}
			}
			// Undefendable attack
			else
			{
				Player_C->Damaged(GetDamage());
			}
		}


		if (Player_C->GetIsHoldingShield())
		{
			if (DataTableRowHandle.RowName == ProjectileName::Monster_Attack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_LynelAttack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_AL_Attack
				|| DataTableRowHandle.RowName == ProjectileName::Monster_Arrow
				)
			{
				// Do not take damage;
			}
			else
			{
				Player_C->Damaged(GetDamage());
			}
		}
		else
		{
			Player_C->Damaged(GetDamage());
		}
		Destroy();
		return;
	}

	if (DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStone
		|| DataTableRowHandle.RowName == ProjectileName::Monster_AB_KogaStoneBig
		|| DataTableRowHandle.RowName == ProjectileName::Monster_HinoxStone)
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
	}
	if (DataTableRowHandle.RowName == ProjectileName::Player_Arrow
		|| DataTableRowHandle.RowName == ProjectileName::Player_FireArrow
		)
	{
		if (UWorld* World = GetWorld())
		{
			AParticleEffect* ParticleEffect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			const FDataTableRowHandle ParticleEffectDataTable = ProjectileTableRow->ParticleEffectTableRowHandle;
			FTransform NewTransform;
			ParticleEffect->SetData(ParticleEffectDataTable);

			const FVector Location = GetActorLocation();
			NewTransform.SetLocation(Location);

			ParticleEffect->FinishSpawning(NewTransform);
		}
	}

	if (!(DataTableRowHandle.RowName == ProjectileName::Monster_PlayerAlert)
		&& !OtherActor->IsA<ATorchStand>()
		)
	{
		Destroy();
	}


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

	//FVector CurrentVelocity = ProjectileMovementComponent->Velocity;
	//if (!CurrentVelocity.IsNearlyZero())
	//{
	//	FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(CurrentVelocity);
	//	SetActorRotation(NewRotation);
	//}
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
			if (UMonsterFSMComponent* MonsterFSMComponent = PM->GetFSMComponent())
			{
				if (const AWorldWeapon* WW = MonsterFSMComponent->GetCurrentWeapon())
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
				UE_LOG(LogTemp, Warning, TEXT("AProjectile::GetDamage // No MonsterFSMComponent"));
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
	if (bFlag)
	{
		ProjectileMovementComponent->Activate();
	}
	else
	{
		ProjectileMovementComponent->Deactivate();
	}
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
	if (!NiagaraEffectComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectile::SetNiagaraVisibility // No NiagaraEffectComponent"))
			return;
	}

	if(bFlag)
	{
		NiagaraEffectComponent->Activate();
	}
	else
	{
		NiagaraEffectComponent->Deactivate();
	}
}

