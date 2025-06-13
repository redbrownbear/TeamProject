// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

class USkeletalMeshComponent;
class APatrolPath;
class ACampFire;
struct FMonsterTableRow;

UCLASS()
class TEAMPROJECT_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY(EditAnywhere, meta = (RowType = "MonsterTableRow"))
    FDataTableRowHandle DataTableRowHandle;
    FMonsterTableRow* MonsterData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APatrolPath> PatrolPath;
    UPROPERTY(EditAnywhere)
    TObjectPtr<ACampFire> CampFire;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGuid SpawnPointGuid;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGuid PatrolPathPointGuid;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGuid CampFirePointGuid;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsAlive = true;


    void SetData(const FDataTableRowHandle& InDataTableRowHandle);
    virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
    virtual void OnConstruction(const FTransform& Transform) override;
};
