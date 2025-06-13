// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BurningActor.generated.h"

class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class TEAMPROJECT_API ABurningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABurningActor();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerColliderComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyThisActor();

	void SetBurningActor(bool _bool);

	void SetNiagaraVisibility(bool bFlag);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsBurning = false;

	FTimerHandle DestroyTimerHandle;
};
