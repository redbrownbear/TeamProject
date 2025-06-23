// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/AnimNotify_EmptyDamagedActors.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponSpear.h"
#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"

void UAnimNotify_EmptyDamagedActors::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerCharacter* PC = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		if (AWeaponSword* WeaponSword = Cast<AWeaponSword>(PC->GetWeapon()->GetChildActor()))
		{
			WeaponSword->EmptyDamagedActors();
			UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_EmptyDamagedActors::Notify"));
		}
		if (AWeaponSpear* WeaponSpear = Cast<AWeaponSpear>(PC->GetWeapon()->GetChildActor()))
		{
			WeaponSpear->EmptyDamagedActors();
			UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_EmptyDamagedActors::Notify"));
		}
	}
}
