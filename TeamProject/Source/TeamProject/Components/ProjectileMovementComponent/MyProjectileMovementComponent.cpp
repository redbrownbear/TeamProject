// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProjectileMovementComponent/MyProjectileMovementComponent.h"

#include "Subsystem/TimeManager.h"

void UMyProjectileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMyProjectileMovementComponent::TickComponent // No Owner Actor"));
		return;
	}

	static UTimeManagerSubsystem* TimeManager = GetOwner()->GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>();
	const float CustumDeltaTime = TimeManager->GetCustomDeltaTime();

	Super::TickComponent(CustumDeltaTime, TickType, ThisTickFunction);
}
