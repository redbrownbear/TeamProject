#include "NpcFSMComponent.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Navigation/PathFollowingComponent.h"
#include "Animation/Npc/ConversationManagerComponent.h"

#include "Kismet/GameplayStatics.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UNpcFSMComponent::BeginPlay()
{
	Super::BeginPlay();
		
	if (!Owner)
	{
		if (Controller = Cast<ANpcController>(GetOwner()))
		{
			Owner = Cast<ANpc>(Controller->GetPawn());
		}
	}

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay // Owner (ANpc) is null!"));
		return;
	}

	Controller = Cast<ANpcController>(Owner->GetController());
	if (Controller && Controller->GetConversationManager())
	{
		UConversationManagerComponent* ConversationManager = Controller->GetConversationManager();

		if (!ConversationManager)
		{
			UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay // ConversationManager is null (from NPC)!"));
		}
	}
	
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player = PlayerChar;
	}
}

void UNpcFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UNpcFSMComponent::HandleState(float DeltaTime)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::HandleState // No Owner"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case ENpcState::Stroll:
		UpdateStroll(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	default:
		break;
	}
	
}

void UNpcFSMComponent::ChangeState(ENpcState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case ENpcState::Idle:
		break;
	case ENpcState::Stroll:
		break;
	case ENpcState::Talk:
		break;	
	default:
		break;
	}
	eCurrentState = NewState;

	UE_LOG(LogTemp, Warning, TEXT("eCurrentState = %s"), *UEnum::GetValueAsString(eCurrentState));
}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	if (eCurrentState != ENpcState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Idle'"));
		return;
	}
}

void UNpcFSMComponent::UpdateStroll(float DeltaTime)
{	
	if (eCurrentState != ENpcState::Stroll)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Stroll'"));
		return;
	}
	
}

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
	if (eCurrentState != ENpcState::Talk)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Talk'"));
		return;
	}

	if (Player)
	{	
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NpcLocation = Owner->GetActorLocation();
		SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);
		SmoothRotateActorToDirection(Player, NpcLocation, DeltaTime);

		Controller->GetConversationManager()->StartConversation(Owner, Player);
	}				

}

void UNpcFSMComponent::MoveToLocation(const FVector& InLocation)
{
	ANpcController* NpcController = Cast<ANpcController>(Owner->GetController());
	if (NpcController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(InLocation);
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

		FNavPathSharedPtr NavPath;
		NpcController->MoveTo(MoveRequest, &NavPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent::MoveToLocation // No NpcController"));
		check(false);
	}
}
