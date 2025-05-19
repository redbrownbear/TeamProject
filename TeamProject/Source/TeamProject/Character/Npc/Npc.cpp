#include "Npc.h"
#include "Controller/Npc/NpcController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/FSM/Npc/NpcFSMComponent.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

ANpc::ANpc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	BodyMeshComponent->SetupAttachment(CollisionComponent);

	FaceMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	FaceMeshComponent->SetupAttachment(BodyMeshComponent);

	HairMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMeshComponent->SetupAttachment(BodyMeshComponent);

	NoseMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Nose"));
	NoseMeshComponent->SetupAttachment(BodyMeshComponent);

	// Movement
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent; // 충돌 기준 컴포넌트 설정

	/*NpcFSMComponent = CreateDefaultSubobject<UNpcFSMComponent>(TEXT("NpcFSMComponent"));*/
}

void ANpc::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = ANpcController::StaticClass();
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//const float Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
}

UNpcFSMComponent* ANpc::GetFSMComponent() const
{
	if (ANpcController* NpcController = Cast<ANpcController>(GetController()))
	{
		if (UNpcFSMComponent* FSMComponent = Cast<UNpcFSMComponent>(NpcController->GetComponentByClass(UNpcFSMComponent::StaticClass())))
		{
			return FSMComponent;
		}
	}

	//if (NpcFSMComponent) { return NpcFSMComponent; }

	return nullptr;


}

void ANpc::OnPlayerInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
	{
		bPlayerInRange = true;
	}*/
}

void ANpc::OnTalkKeyPressed()
{
	/*if (bPlayerInRange && NpcFSMComponent)
	{
		if (NpcFSMComponent->GetNpcState() == ENpcState::Stroll)
		{
			NpcFSMComponent->SetNpcState(ENpcState::Talk);
		}
	}*/
}

