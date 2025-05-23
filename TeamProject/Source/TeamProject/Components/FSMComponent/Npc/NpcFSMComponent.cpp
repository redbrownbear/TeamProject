#include "NpcFSMComponent.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "UI/NpcDialogue/NPCDialogue.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UNpcFSMComponent::BeginPlay()
{
	Super::BeginPlay();
		
	if (!Owner)
	{
		Controller = Cast<ANpcController>(GetOwner());
		if (Controller)
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
	case ENpcState::Sit:
		UpdateSit(DeltaTime);
		break;
	case ENpcState::Stand:
		UpdateStand(DeltaTime);
		break;
	case ENpcState::Walk:
		UpdateWalk(DeltaTime);
		break;
	case ENpcState::Run:
		UpdateRun(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	case ENpcState::Hide:
		UpdateHide(DeltaTime);
		break;
	case ENpcState::Sell:
		UpdateSell(DeltaTime);
		break;
	case ENpcState::End:
		UpdateEnd(DeltaTime);
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
	case ENpcState::Sit:
		break;
	case ENpcState::Stand:
		break;
	case ENpcState::Walk:
		Owner->SetSpeedWalk();
		break;
	case ENpcState::Run:
		Owner->SetSpeedRun();
		break;
	case ENpcState::Talk:
		break;	
	case ENpcState::Hide:
		break;
	case ENpcState::Sell:
		break;
	case ENpcState::End:
		break;
	default:
		break;
	}
	eCurrentState = NewState;

}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	if (eCurrentState != ENpcState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Idle'"));
		return;
	}
}

void UNpcFSMComponent::UpdateSit(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateStand(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateWalk(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateRun(float DeltaTime)
{	
	if (eCurrentState != ENpcState::Run)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Run'"));
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

		//Controller->GetConversationManager()->StartConversation(Owner, Player);
	}			

	// ��ȭ ���� ��	
	/*if (!Dialogue->GetDialogueState())
	{
		Dialogue->CloseUI();
		Controller->GetConversationManager()->UnlockCharacters(Owner, Player);
	}*/
}

void UNpcFSMComponent::UpdateHide(float DeltaTime)
{

}

void UNpcFSMComponent::UpdateSell(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateEnd(float DeltaTime)
{
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
