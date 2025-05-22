// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponChildActorComponent/WeaponChildActorComponent.h"
#include "Actors/Weapon/WeaponBase.h"

void UWeaponChildActorComponent::BeginPlay()
{

	Super::BeginPlay();
}

void UWeaponChildActorComponent::SetData(FDataTableRowHandle InDataTableRowHandle)
{
	AWeaponBase* Weapon = Cast<AWeaponBase>(GetChildActor());
	check(Weapon);

	Weapon->SetData(InDataTableRowHandle);
}
