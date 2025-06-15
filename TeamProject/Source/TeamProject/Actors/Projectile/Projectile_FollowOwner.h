// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile/Projectile.h"
#include "Projectile_FollowOwner.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AProjectile_FollowOwner : public AProjectile
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaTime) override;
	
};
