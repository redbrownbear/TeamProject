// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent/StatusComponent.h"

UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UStatusComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDie) { return 0.f; }
	if (FMath::IsNearlyZero(Damage)) { return 0.0; }
	float NewDamage = Damage;

	// Ex. NewDamage -= Armor;
	NewDamage = FMath::Clamp(NewDamage, 0.f, NewDamage);

	HP -= NewDamage;
	HP = FMath::Clamp(HP, 0.f, HP);

	LastInstigator = EventInstigator;
	OnHPChanged.Broadcast(HP, MaxHP);

	if (HP == 0.f)
	{
		bDie = true;
		OnDie.Broadcast();
	}

	return NewDamage;
}

