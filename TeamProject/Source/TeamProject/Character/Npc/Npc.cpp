#include "Npc.h"
#include "Controller/Npc/NpcController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/Npc/NpcFSMComponent.h"

#include "Kismet/KismetMathLibrary.h"

ANpc::ANpc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 충돌
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	// 메시
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Furiko"));
	SkeletalMeshComponent->SetupAttachment(CollisionComponent);

	FaceMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	FaceMeshComponent->SetupAttachment(SkeletalMeshComponent);

	HairMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMeshComponent->SetupAttachment(SkeletalMeshComponent);

	NoseMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Nose"));
	NoseMeshComponent->SetupAttachment(SkeletalMeshComponent);

	// FSM 
	FSMComponent = CreateDefaultSubobject<UNpcFSMComponent>(TEXT("FSMComponent"));
}

void ANpc::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ANpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UNpcFSMComponent* ANpc::GetFSMComponent() const
{
	if (ANpcController* NpcController = Cast<ANpcController>(GetController()))
	{
		if (UNpcFSMComponent* NpcFSMComponent = Cast<UNpcFSMComponent>(NpcController->GetComponentByClass(UNpcFSMComponent::StaticClass())))
		{
			return NpcFSMComponent;
		}
	}

	return nullptr;

}

void ANpc::OnPlayerInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
	{
		bPlayerInRange = true;
	}
}

void ANpc::OnTalkKeyPressed()
{
	if (bPlayerInRange && FSMComponent)
	{
		if (FSMComponent->GetNpcState() == ENpcState::Stroll)
		{
			FSMComponent->SetNpcState(ENpcState::Talk);
		}
	}
}

