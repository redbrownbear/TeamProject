#include "FurikoFSMComponent.h"
#include "Actors/Npc/Furiko/Furiko.h"

void UFurikoFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (!Owner)
	{
		Owner = Cast<AFuriko>(GetOwner()); 
		if (!Owner)
		{
			UE_LOG(LogTemp, Error, TEXT("UFurikoFSMComponent::BeginPlay - Owner is null even after fallback"));
			return;
		}
	}*/

	//FurikoController = Cast<AFurikoController>(Owner->GetController());
	//if (!FurikoController)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UFurikoFSMComponent::BeginPlay - Controller is not AFurikoController"));
	//}

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
	case ENpcState::Play: // 술래잡기: 시간 되면 추가
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
	case ENpcState::Play: // 술래잡기: 시간 되면 추가
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

	/*if (FurikoController->bPlayHide)
	{
		FurikoController->bTalk = false;
		SetNpcState(ENpcState::Hide);
	}*/

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
	// 술래잡기: 시간 되면 추가
};
