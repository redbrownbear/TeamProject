#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempleActorSpawner.generated.h"

//class UTempleBallSpawnerComponent;
class ATempleActor;

UCLASS()
class TEAMPROJECT_API ATempleActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempleActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SpawnActor();

	void ReturnActorToPool(ATempleActor* Actor);

protected:
	TObjectPtr<ATempleActor> GetPooledActor();

protected:
	UPROPERTY()
	TSubclassOf<ATempleActor> TempleActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnAreaExtent;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (RowType = "TempleActorTableRow"))
	FDataTableRowHandle SpawnRowHandle;


private:
	FTimerHandle SpawnTimer;

	UPROPERTY()
	TArray<TObjectPtr<ATempleActor>> ActorPool;

};
