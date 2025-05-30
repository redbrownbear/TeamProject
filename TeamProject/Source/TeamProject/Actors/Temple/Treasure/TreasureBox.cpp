#include "Actors/Temple/Treasure/TreasureBox.h"
#include "Components/BoxComponent.h"
#include "Data/ParticleEffectTableRow.h"
#include "Actors/Effect/ParticleEffect.h"

// Sets default values
ATreasureBox::ATreasureBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -30.f));
	SkeletalMeshComponent->SetRelativeScale3D(FVector(30.f, 30.f, 30.f));

}

// Called when the game starts or when spawned
void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 플레이어와 충돌
	CollisionComponent->SetGenerateOverlapEvents(true);

	bCanTakeItem = true;
}

void ATreasureBox::OpenTBox()
{
	USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComp)
	{
		if (!ParticleEffectTableRowHandle.IsNull())
		{
			UWorld* World = GetWorld();
			if (!World) return;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(GetActorLocation());
			SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

			AParticleEffect* Effect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (Effect)
			{
				Effect->SetData(ParticleEffectTableRowHandle);
				Effect->FinishSpawning(SpawnTransform);
			}

			GetTreasure();
		}
	}			
}

void ATreasureBox::GetTreasure()
{
	if (bCanTakeItem)
	{
		// open Item UI
		// Add Item to Inventory
	}
	
	bCanTakeItem = false;
}


