#include "FurikoFSMComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Npc/Npc.h"

void UFurikoFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ANpc>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UFurikoFSMComponent::BeginPlay - Owner is not ANpc"));
		return;
	}

	FurikoController = Cast<AFurikoController>(Owner->GetController());
	if (!FurikoController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFurikoFSMComponent::BeginPlay - Controller is not AFurikoController"));
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStrollPath::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		StrollPathActor = Cast<AStrollPath>(FoundActors[0]);
	}

	SetNpcState(ENpcState::Stroll);
}

void UFurikoFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
	case ENpcState::Stroll:
		UpdateStroll(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	case ENpcState::Hide:
		UpdateHide(DeltaTime);
		break;
	case ENpcState::Play: // 술래잡기: 상황 봐서 여유되면 추가
		UpdatePlay(DeltaTime); 
		break;
	default:
		break;
	}
}

void UFurikoFSMComponent::ChangeState(ENpcState NewState)
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
	case ENpcState::Hide:
		break;
	case ENpcState::Play: // 술래잡기: 상황 봐서 여유되면 추가
		break;
	default:
		break;
	}
	eCurrentState = NewState;
}

void UFurikoFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UFurikoFSMComponent::UpdateStroll(float DeltaTime)
{	
	Super::UpdateStroll(DeltaTime);
	
}

void UFurikoFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);

	if (FurikoController->bPlayHide)
	{
		FurikoController->bTalk = false;
		SetNpcState(ENpcState::Hide);
	}

}

void UFurikoFSMComponent::UpdateHide(float DeltaTime)
{
	if (FurikoController->bTalk)
	{
		SetNpcState(ENpcState::Talk);
	}
}

void UFurikoFSMComponent::UpdatePlay(float DeltaTime)
{
	// @TODO 술래잡기: 상황 봐서 여유되면 추가
};
