// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MetalComponent/MetalComponent.h"

#include "Actors/Projectile/Projectile.h"
#include "Actors/Object/MetalActor.h"

UMetalComponent::UMetalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMetalComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UMetalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMetalComponent::SetIsControlled(bool bFlag)
{
	bIsControlled = bFlag;
}

bool UMetalComponent::GetIsControlled() const
{
	return bIsControlled;
}

void UMetalComponent::SetColorNormal()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor->IsA<AProjectile>())
	{
		AProjectile* Projectile = Cast<AProjectile>(OwnerActor);
		Projectile->SetColorNormal();
	}
	else if (OwnerActor->IsA<AMetalActor>())
	{
		AMetalActor* MetalActor = Cast<AMetalActor>(OwnerActor);
		MetalActor->ChangeNomalColor();
	}
}

void UMetalComponent::SetColorScanned()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor->IsA<AProjectile>())
	{
		AProjectile* Projectile = Cast<AProjectile>(OwnerActor);
		Projectile->SetColorScanned();
	}
	else if (OwnerActor->IsA<AMetalActor>())
	{
		AMetalActor* MetalActor = Cast<AMetalActor>(OwnerActor);
		MetalActor->ThisIsMetal();
	}
}

