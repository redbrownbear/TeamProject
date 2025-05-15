#include "KoroguFSMComponent.h"

void UKoroguFSMComponent::HandleState(float DeltaTime)
{
	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
	case ENpcState::Patrol:
		UpdatePatrol(DeltaTime);
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

void UKoroguFSMComponent::ChangeState(ENpcState NewState)
{
	if (eCurrentState == NewState) { return; }
	switch (NewState)
	{
	case ENpcState::Idle:
		break;
	case ENpcState::Patrol:
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

void UKoroguFSMComponent::UpdateIdle(float DeltaTime)
{
	Super::UpdateIdle(DeltaTime);
}

void UKoroguFSMComponent::UpdatePatrol(float DeltaTime)
{
	Super::UpdatePatrol(DeltaTime);
}

void UKoroguFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);
}

void UKoroguFSMComponent::UpdateHide(float DeltaTime)
{
}

void UKoroguFSMComponent::UpdatePlay(float DeltaTime)
{
	// @TODO 술래잡기: 상황 봐서 여유되면 추가
};
