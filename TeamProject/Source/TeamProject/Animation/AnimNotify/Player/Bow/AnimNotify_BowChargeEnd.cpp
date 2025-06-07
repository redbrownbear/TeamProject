// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Player/Bow/AnimNotify_BowChargeEnd.h"
#include "Actors/Character/PlayerCharacter.h"

void UAnimNotify_BowChargeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)

	{
		return;
	}
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!Player_C)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Character"));
		return;
	}
	
	

	UWeaponManagerComponent* WeaponManagerComponent = Player_C->GetWeaponManagerComponent();
	
	EEquip_State EState = WeaponManagerComponent->GetEquipState();

	if (EState == EEquip_State::Bow || EState == EEquip_State::Shield || EState == EEquip_State::Sword_Shield)

	{

		AWeaponBow* Actor_Bow = Cast<AWeaponBow>(WeaponManagerComponent->GetBow()->GetChildActor());

		Player_C->SetArrowVisibility(true);

		WeaponManagerComponent->SetCanShot(true);


	}

	

}
