#include "Actors/Item/Product.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

// Sets default values
AProduct::AProduct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	ProductMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProductMeshComponent"));

	SetRootComponent(CollisionComponent);
	ProductMeshComponent->SetupAttachment(RootComponent);

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProduct::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AProduct::OnEndOverlapWithPlayer);
	}
}

// Called when the game starts or when spawned
void AProduct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProduct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProduct::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			/*PC->Npc = this;
			bPlayerInRange = true;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
				HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}*/
		}
		// Create Interact UI
	}
}

void AProduct::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			/*PC->Npc = nullptr;
			bPlayerInRange = false;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
				HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}*/
		}
	}
}

