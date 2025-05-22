#include "NpcController.h"
#include "Actors/Npc/Npc.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"

ANpcController::ANpcController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	ConversationManager = CreateDefaultSubobject<UConversationManagerComponent>(TEXT("ConversationManager"));

	// 시야 설정
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = MONSTER_AISENSECONFIG_SIGHT_SIGHTRADIUS;
	SightConfig->LoseSightRadius = MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS;
	SightConfig->PeripheralVisionAngleDegrees = MONSTER_SIGHT_ANGLE;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANpcController::OnTargetPerceptionUpdated);
}

void ANpcController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANpcController::BeginPlay()
{
	Super::BeginPlay();

	if (NpcFSMComponent)
	{
		NpcFSMComponent->SetOwner(Cast<ANpc>(GetPawn()));
		NpcFSMComponent->SetPlayer(nullptr);
	}
}

void ANpcController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	APlayerCharacter* Player = nullptr;

	for (AActor* SeenActor : UpdatedActors)
	{
		if (APlayerCharacter* DetectedPlayer = Cast<APlayerCharacter>(SeenActor))
		{
			Player = DetectedPlayer;
			break; // 플레이어 감지
		}
	}

	if (Player)
	{
		NpcFSMComponent->SetPlayer(Player);
	}
	else
	{
		NpcFSMComponent->SetPlayer(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("ANpcController::OnPerceptionUpdated Player set Null"));
	}
}

void ANpcController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지됨
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if (!Player) return;
		NpcFSMComponent->SetPlayer(Player);
	}
}
