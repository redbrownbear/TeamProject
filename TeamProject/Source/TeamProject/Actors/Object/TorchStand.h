// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchStand.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class TEAMPROJECT_API ATorchStand : public AActor
{
	GENERATED_BODY()
	
public:	
	ATorchStand();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> NukiStaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> TriggerColliderComponent;



protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;
	 
	void SetTorchStandFire(bool _bool);

	void SetNiagaraVisibility(bool bFlag);

	bool GetTorchStandFire() { return bTorchOnFire; }

private:
	bool bTorchOnFire = false;
};
