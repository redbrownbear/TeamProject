// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AB_Stone.generated.h"

UCLASS()
class TEAMPROJECT_API AAB_Stone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAB_Stone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
