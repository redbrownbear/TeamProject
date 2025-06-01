#include "Actors/Temple/Treasure/TreasureBox.h"
#include "Components/BoxComponent.h"
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
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	SkeletalMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	MaterialInterface = SkeletalMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	SkeletalMeshComponent->SetMaterial(0, DynamicMaterialInstance);

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATreasureBox::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATreasureBox::OnEndOverlapWithPlayer);
	}

	bCanTakeItem = true;
}

void ATreasureBox::OpenTBox()
{
	if (!bCanTakeItem) return;

	GetTreasure();
}

void ATreasureBox::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanTakeItem)
	{
		bCanOpenBox = true;
		GetParticleEffect();
	}
}

void ATreasureBox::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanOpenBox = false;
}

void ATreasureBox::GetTreasure()
{
	if (bCanTakeItem)
	{
		// open Item UI
		// Add Item to Inventory

		DynamicMaterialInstance->SetScalarParameterValue("Color", 1.f);
		bCanTakeItem = false;
	}	
}


void ATreasureBox::GetParticleEffect()
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
	}
}
