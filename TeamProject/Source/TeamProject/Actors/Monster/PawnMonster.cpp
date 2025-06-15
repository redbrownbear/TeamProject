// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/PawnMonster.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Item/WorldWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h"

#include "UI/UIComponent/MonsterHP.h"

#include "SubSystem/TimeManager.h"



// Sets default values
APawnMonster::APawnMonster()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APawnMonster::OnBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APawnMonster::OnEndOverlap);

	RootComponent = CollisionComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	FRotator NewRotator = FRotator(0.0, 0.0, 0.0);
	SkeletalMeshComponent->SetWorldRotation(NewRotator.Quaternion());

	StatusComponent = CreateDefaultSubobject<UMonsterStatusComponent>(TEXT("StatusComponent"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(RootComponent);

	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HPBarWidget->SetDrawAtDesiredSize(true);

	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetClassFinder(TEXT("/Game/Blueprint/UI/WidgetComponent/BP_MonsterHp"));
	if (HPWidgetClassFinder.Succeeded())
	{
		HPBarWidgetClass = HPWidgetClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void APawnMonster::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	SetData(DataTableRowHandle);

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetPawnMonster(this);
		FSMComponent->BindHitEvent();
	}

	//StatusComponent->OnDie.AddDynamic(this, &ThisClass::OnDie);

	if (HPBarWidgetClass)
	{
		HPBarWidget->SetWidgetClass(HPBarWidgetClass);
		HPBarWidget->InitWidget();
	}

	GetTimeManagerSubsystem();
}

// Called every frame
void APawnMonster::Tick(float DeltaTime)
{
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::Tick(CustumDeltaTime);

	const float Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	if (HPBarWidget)
	{
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector ToCamera = CameraLocation - HPBarWidget->GetComponentLocation();

			FRotator LookAtRotation = FRotationMatrix::MakeFromX(ToCamera).Rotator();
			HPBarWidget->SetWorldRotation(LookAtRotation);
		}
	}
}

UMonsterFSMComponent* APawnMonster::GetFSMComponent() const
{
	if (AMonsterAIController* MonsterAIController = Cast<AMonsterAIController>(GetController()))
	{
		if (UMonsterFSMComponent* MonsterFSMComponent = Cast<UMonsterFSMComponent>(MonsterAIController->GetComponentByClass(UMonsterFSMComponent::StaticClass())))
		{
			return MonsterFSMComponent;
		}
	}

	return nullptr;
}

void APawnMonster::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMonsterTableRow* Data = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	MonsterData = Data;

	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(MonsterData->CollisionSphereRadius);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		//CollisionComponent->RegisterComponent();
	}

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(MonsterData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 모리블린 전용 오프셋
	if (TEXT("Moriblin_Patrol") == DataTableRowHandle.RowName.ToString()
		|| TEXT("Moriblin_TreasureBox") == DataTableRowHandle.RowName.ToString())
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -1.5f * MonsterData->CollisionSphereRadius));
	}
	else
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -MonsterData->CollisionSphereRadius));
	}


	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, (MonsterData->CollisionSphereRadius - 10.0f) * 4.0f));

	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;

	AIControllerClass = MonsterData->AIControllerClass;

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetMonsterGroupType(MonsterData->eMonsterGroupType);
		if (PatrolPath)
		{
			FSMComponent->SetPatrolPath(PatrolPath);
		}
		if (CampFire)
		{
			FSMComponent->SetCampFire(CampFire);
		}
	}




	StatusComponent->SetMaxHP(MonsterData->MaxHP);


	if (!(MonsterData->MeleeWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* MeleeWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			MeleeWeapon->SetDataWithHandle(MonsterData->MeleeWeaponTableRowHandle);
			const FVector Scale = MeleeWeapon->GetActorScale3D() * 2.f;
			MeleeWeapon->SetActorScale3D(Scale);
			MeleeWeapon->AttachToMonster(this, Monster_SocketName::Pod_Melee);
			MeleeWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->SetMeleeWeapon(MeleeWeapon);
				FSMComponent->SheathMeleeWeapon();
			}
		}
	}

	if (!(MonsterData->BowWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* BowWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			BowWeapon->SetDataWithHandle(MonsterData->BowWeaponTableRowHandle);
			const FVector Scale = BowWeapon->GetActorScale3D() * 2.f;
			BowWeapon->SetActorScale3D(Scale);
			BowWeapon->AttachToMonster(this, Monster_SocketName::Pod_Bow);
			BowWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->SetBowWeapon(BowWeapon);
				FSMComponent->SheathBowWeapon();
			}
		}
	}

	int32 MaterialSlotIndex = -1;
	if (DataTableRowHandle.RowName.ToString() == TEXT("Bokoblin_TreasureBox")
		|| DataTableRowHandle.RowName.ToString() == TEXT("Bokoblin_Patrol"))
	{
		MaterialSlotIndex = 2;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_TreasureBox")
		|| DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_Patrol"))
	{
		MaterialSlotIndex = 3;
	}

	if (MaterialSlotIndex != -1)
	{
		UMaterialInterface* CurrentMaterialOnMesh = SkeletalMeshComponent->GetMaterial(MaterialSlotIndex);

		if (!CurrentMaterialOnMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetData: No material found at slot %d. Cannot proceed with DynamicMaterialInstance setup."), MaterialSlotIndex);
			DynamicMaterialInstance = nullptr; // 기존 포인터도 혹시 모르니 null로
			return;
		}

		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentMaterialOnMesh);

		if (!DynamicMaterialInstance)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterialOnMesh, this);

			if (DynamicMaterialInstance)
			{
				SkeletalMeshComponent->SetMaterial(MaterialSlotIndex, DynamicMaterialInstance);
				UE_LOG(LogTemp, Log, TEXT("SetData: Created and assigned new DynamicMaterialInstance (%s) to slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SetData: Failed to create DynamicMaterialInstance using parent %s!"), *CurrentMaterialOnMesh->GetName());
				DynamicMaterialInstance = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetData: Re-using existing DynamicMaterialInstance (%s) at slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SetData: No specific material slot handling for %s."), *DataTableRowHandle.RowName.ToString());
		DynamicMaterialInstance = nullptr; 
	}

	AddBaseColor(FVector(0.0, 0.0, 0.0));
}

void APawnMonster::SetData(const FName& MonsterName)
{
	static UDataTable* MonsterDataTable = nullptr;
	if (!MonsterDataTable)
	{
		MonsterDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/MonsterData/DT_Monster.DT_Monster'"));
		check(MonsterDataTable);
	}

	if (!MonsterDataTable->GetRowMap().Find(MonsterName)) { ensure(false); return; }
	DataTableRowHandle.DataTable = MonsterDataTable;
	DataTableRowHandle.RowName = MonsterName;
	MonsterData = DataTableRowHandle.GetRow<FMonsterTableRow>(DataTableRowHandle.RowName.ToString());

	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(MonsterData->CollisionSphereRadius);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		//CollisionComponent->RegisterComponent();
	}

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(MonsterData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(MonsterData->MeshTransform.GetScale3D());
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 모리블린 전용 오프셋
	if (TEXT("Moriblin_Patrol") == DataTableRowHandle.RowName.ToString()
		|| TEXT("Moriblin_TreasureBox") == DataTableRowHandle.RowName.ToString())
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -1.5f * MonsterData->CollisionSphereRadius));
	}
	else
	{
		SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -MonsterData->CollisionSphereRadius));
	}


	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, (MonsterData->CollisionSphereRadius - 10.0f) * 4.0f));

	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;

	AIControllerClass = MonsterData->AIControllerClass;

	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		FSMComponent->SetMonsterGroupType(MonsterData->eMonsterGroupType);
	}

	StatusComponent->SetMaxHP(MonsterData->MaxHP);

	if (!(MonsterData->MeleeWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* MeleeWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			MeleeWeapon->SetDataWithHandle(MonsterData->MeleeWeaponTableRowHandle);
			const FVector Scale = MeleeWeapon->GetActorScale3D() * 2.f;
			MeleeWeapon->SetActorScale3D(Scale);
			MeleeWeapon->AttachToMonster(this, Monster_SocketName::Pod_Melee);
			MeleeWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->SetMeleeWeapon(MeleeWeapon);
				FSMComponent->SheathMeleeWeapon();
			}
		}
	}

	if (!(MonsterData->BowWeaponTableRowHandle.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			AWorldWeapon* BowWeapon = World->SpawnActorDeferred<AWorldWeapon>(AWorldWeapon::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			BowWeapon->SetDataWithHandle(MonsterData->BowWeaponTableRowHandle);
			const FVector Scale = BowWeapon->GetActorScale3D() * 2.f;
			BowWeapon->SetActorScale3D(Scale);
			BowWeapon->AttachToMonster(this, Monster_SocketName::Pod_Bow);
			BowWeapon->FinishSpawning(FTransform::Identity);

			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{
				FSMComponent->SetBowWeapon(BowWeapon);
				FSMComponent->SheathBowWeapon();
			}
		}
	}

	int32 MaterialSlotIndex = -1;
	if (DataTableRowHandle.RowName.ToString() == TEXT("Bokoblin_TreasureBox")
		|| DataTableRowHandle.RowName.ToString() == TEXT("Bokoblin_Patrol"))
	{
		MaterialSlotIndex = 2;
	}
	else if (DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_TreasureBox")
		|| DataTableRowHandle.RowName.ToString() == TEXT("Moriblin_Patrol"))
	{
		MaterialSlotIndex = 3;
	}

	if (MaterialSlotIndex != -1)
	{
		UMaterialInterface* CurrentMaterialOnMesh = SkeletalMeshComponent->GetMaterial(MaterialSlotIndex);

		if (!CurrentMaterialOnMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetData: No material found at slot %d. Cannot proceed with DynamicMaterialInstance setup."), MaterialSlotIndex);
			DynamicMaterialInstance = nullptr; // 기존 포인터도 혹시 모르니 null로
			return;
		}

		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentMaterialOnMesh);

		if (!DynamicMaterialInstance)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterialOnMesh, this);

			if (DynamicMaterialInstance)
			{
				SkeletalMeshComponent->SetMaterial(MaterialSlotIndex, DynamicMaterialInstance);
				UE_LOG(LogTemp, Log, TEXT("SetData: Created and assigned new DynamicMaterialInstance (%s) to slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SetData: Failed to create DynamicMaterialInstance using parent %s!"), *CurrentMaterialOnMesh->GetName());
				DynamicMaterialInstance = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetData: Re-using existing DynamicMaterialInstance (%s) at slot %d"), *DynamicMaterialInstance->GetName(), MaterialSlotIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SetData: No specific material slot handling for %s."), *DataTableRowHandle.RowName.ToString());
		DynamicMaterialInstance = nullptr;
	}

	AddBaseColor(FVector(0.0, 0.0, 0.0));
}

void APawnMonster::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		//CollisionComponent->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void APawnMonster::PostLoad()
{
	Super::PostLoad();
}

void APawnMonster::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void APawnMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APawnMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void APawnMonster::SetSpeedWalk()
{
	MovementComponent->MaxSpeed = MonsterData->WalkMovementMaxSpeed;
}

void APawnMonster::SetSpeedRun()
{
	MovementComponent->MaxSpeed = MonsterData->RunMovementMaxSpeed;
}

void APawnMonster::OnDie()
{
	IMonsterInterface::OnDie();
	// Maybe there's no Dead Montage;
	OnDeadEnd();
}

void APawnMonster::OnDeadEnd()
{
	IMonsterInterface::OnDeadEnd();
}

UAnimInstance* APawnMonster::GetAnimInstance() const
{
	return GetMonsterMesh()->GetAnimInstance();
}

void APawnMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		if (ProjectileName::Monster_PlayerAlert == Projectile->GetProjectileName())
		{
			if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
			{

				FSMComponent->ChangeState(EMonsterState::FindWeapon);
				if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn()))
					{
						FSMComponent->SetPlayer(Player);
					}
					else
					{
						check(false);
					}
				}
				else
				{
					check(false);
				}
			}
		}
		else if (ProjectileName::Player_Arrow == Projectile->GetProjectileName()
			|| ProjectileName::Player_FireArrow == Projectile->GetProjectileName()
			)
		{
			// float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
			FDamageEvent DamageEvent;

			if (UWorld* World = GetWorld())
			{
				if (AController* PlayerController = World->GetFirstPlayerController())
				{
					if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn()))
					{
						GetFSMComponent()->SetPlayer(Player);
						IMonsterInterface::TakeDamage(Projectile->GetDamage(), DamageEvent, PlayerController, Player);
					}
				}
			}
		}
	}
}


void APawnMonster::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

APatrolPath* APawnMonster::GetPatrolPath() const
{
	return PatrolPath;
}

ACampFire* APawnMonster::GetCampFire() const
{
	return CampFire;
}

UMaterialInterface* APawnMonster::GetMaterialInterface()
{
	return MaterialInterface;
}

UMaterialInstanceDynamic* APawnMonster::GetDynamicMaterialInstance()
{
	return DynamicMaterialInstance;
}

void APawnMonster::AddBaseColor(FVector InColor)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("AddColor"), InColor);
	}
}

void APawnMonster::ShowHpUI(float CurHp, float MaxHp)
{
	if (!HPBarWidget)
		return;

	UUserWidget* Widget = HPBarWidget->GetUserWidgetObject();
	if (!Widget)
		return;

	UMonsterHP* HPWidget = Cast<UMonsterHP>(Widget);
	if (HPWidget)
	{
		HPWidget->ShowUI(CurHp, MaxHp);
	}
}

UTimeManagerSubsystem* APawnMonster::GetTimeManagerSubsystem()
{
	if (!TimeManager)
	{ 
		TimeManager = GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
	}

	return TimeManager;
}
