#include "FurikoFSMComponent.h"

void UFurikoFSMComponent::HandleState(float DeltaTime)
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
	case ENpcState::Play: // �������: ��Ȳ ���� �����Ǹ� �߰�
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
	case ENpcState::Patrol:
		break;
	case ENpcState::Talk:
		break;
	case ENpcState::Hide:
		break;
	case ENpcState::Play: // �������: ��Ȳ ���� �����Ǹ� �߰�
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

void UFurikoFSMComponent::UpdatePatrol(float DeltaTime)
{
	Super::UpdatePatrol(DeltaTime);
}

void UFurikoFSMComponent::UpdateTalk(float DeltaTime)
{
	Super::UpdateTalk(DeltaTime);
}

void UFurikoFSMComponent::UpdateHide(float DeltaTime)
{
}

void UFurikoFSMComponent::UpdatePlay(float DeltaTime)
{
	// @TODO �������: ��Ȳ ���� �����Ǹ� �߰�
};
