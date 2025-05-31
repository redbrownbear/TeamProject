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

protected:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ATempleActor> TempleActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnAreaExtent;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//UDataTable* TempleActorDataTable;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//FName RowToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (RowType = "TempleActorTableRow"))
	FDataTableRowHandle SpawnRowHandle;

private:
	FTimerHandle SpawnTimer;

};
