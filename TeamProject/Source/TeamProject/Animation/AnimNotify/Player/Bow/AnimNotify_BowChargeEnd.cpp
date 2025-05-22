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
	
	WeaponManagerComponent->SetRightClick(true);

	MeshComp->GetAnimInstance()->Montage_Pause();

	Player_C->GetCharacterMovement()->MaxWalkSpeed = 300;

}
