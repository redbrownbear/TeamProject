// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureBox.generated.h"

class UBoxComponent;

UCLASS()
class TEAMPROJECT_API ATreasureBox : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATreasureBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OpenTBox();

protected:
	UFUNCTION()
	void OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UFUNCTION()
	void GetTreasure();

	void GetParticleEffect();

public:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (RowType = "ParticleEffectTableRow"))
	FDataTableRowHandle ParticleEffectTableRowHandle;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

private:
	bool bCanTakeItem = false;
	bool bCanOpenBox = false;
};
