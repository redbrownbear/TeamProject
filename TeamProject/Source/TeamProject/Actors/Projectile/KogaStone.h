// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile/Projectile.h"
#include "Misc/Utils.h"
#include "KogaStone.generated.h"

UCLASS()
class TEAMPROJECT_API AKogaStone : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKogaStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	bool bFall = false;
	EKogaStoneKind eKind;

	float OrbitRadius = 300.0f;
	//float OrbitSpeed = 300.0f; 
	//float OrbitPlaneRotationRate = 0.5f; 

public:
	void SetFallTrue();
	void SetKogaStoneKind(EKogaStoneKind InKind) { eKind = InKind; }

private:
	float WaitTime = 0.f;
	FRotator CurrentOrbitRotation = FRotator::ZeroRotator;
	float OrbitAngle = 0.f;
private:
	bool bVelocitySet = false;
	void SetVelocity();
};
