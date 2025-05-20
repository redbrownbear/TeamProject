#include "Npc.h"
#include "Actors/Controller/Npc/NpcController.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

#include "Animation/Npc/ConversationManagerComponent.h"

ANpc::ANpc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	BodyMeshComponent->SetupAttachment(RootComponent);

	FaceMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	FaceMeshComponent->SetupAttachment(BodyMeshComponent);

	HairMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMeshComponent->SetupAttachment(BodyMeshComponent, TEXT("Head"));

	NoseMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Nose"));
	NoseMeshComponent->SetupAttachment(BodyMeshComponent, TEXT("Head"));

	// Movement
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent; // 충돌 기준 컴포넌트 설정

	// Collision Setting
	BodyMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyMeshComponent->SetCollisionProfileName(TEXT("Pawn"));

}

void ANpc::BeginPlay()
{
	Super::BeginPlay();

	AIControllerClass = ANpcController::StaticClass();

	// Collision Overlap Event Binding
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ANpc::OnEndOverlapWithPlayer);
	}
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

	return nullptr;

}

void ANpc::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->Npc = this;
		}

		bPlayerInRange = true;
		// Create Interact UI
	}
}

void ANpc::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->Npc = nullptr;
		}

		bPlayerInRange = false;
		// Delete Interact UI
	}
}

void ANpc::OnTalkKeyPressed()
{
	if (bPlayerInRange && IsValid(NpcFSMComponent))
	{
		NpcFSMComponent->ChangeState(ENpcState::Talk);
		// Delete Interact UI And Create Talk UI		
	}
}
