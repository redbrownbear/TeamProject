// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scale.generated.h"

class UBoxComponent;

UCLASS()
class TEAMPROJECT_API AScale : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScale();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> SeesawMesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

};
