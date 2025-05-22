// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_MonsterFindEnd.h"

#include "Actors/Monster/Monster.h"
#include "Actors/Projectile/Projectile.h"

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Misc/Utils.h"

UAnimNotify_MonsterFindEnd::UAnimNotify_MonsterFindEnd()
{
}

void UAnimNotify_MonsterFindEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

    if (AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
    {
        if (UMonsterFSMComponent* FSMComponent = Monster->GetFSMComponent())
        {
            FSMComponent->ChangeState(EMonsterState::Signal);
        }
    }
}
