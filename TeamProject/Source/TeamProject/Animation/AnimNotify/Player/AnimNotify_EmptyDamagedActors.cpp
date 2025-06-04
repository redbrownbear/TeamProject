// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_EmptyDamagedActors.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"

void UAnimNotify_EmptyDamagedActors::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerCharacter* PC = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		if (AWeaponSword* WeaponSword = Cast<AWeaponSword>(PC->GetSword()->GetChildActor()))
		{
			WeaponSword->EmptyDamagedActors();
			UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_EmptyDamagedActors::Notify"));
		}	
	}
}
