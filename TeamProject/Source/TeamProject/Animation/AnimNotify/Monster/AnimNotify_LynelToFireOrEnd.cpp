// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_LynelToFireOrEnd.h"
#include "Actors/Monster/Monster.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"


void UAnimNotify_LynelToFireOrEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		if (ULynelFSMComponent* FSMComponent = Cast<ULynelFSMComponent>(Monster->GetFSMComponent()))
		{
			int32 Times = FSMComponent->GetFireAttackTimes();
			if (Times < LYNEL_FIRE_ATTACK_MAX_TIMES)
			{
				FSMComponent->SetFireAttack(Times + 1);
				Monster->PlayMontage(EMonsterMontage::ATTACK_FIRE);
			}
			else
			{
				FSMComponent->SetFireAttack(0);
				FSMComponent->ChangeState(EMonsterState::Combat);
			}
		}
	}
}
