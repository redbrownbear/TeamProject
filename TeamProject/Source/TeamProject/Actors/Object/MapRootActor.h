// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapRootActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class TEAMPROJECT_API AMapRootActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapRootActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
    // 장식용 메쉬들 (콜리전 없음)
    UPROPERTY(VisibleAnywhere)
    TArray<UStaticMeshComponent*> DecorationMeshes;

    // 밟을 수 있는 메쉬들
    UPROPERTY(VisibleAnywhere)
    TArray<UStaticMeshComponent*> WalkableMeshes;

    // 플레이어만 막는 메쉬들
    UPROPERTY(VisibleAnywhere)
    TArray<UStaticMeshComponent*> BlockPlayerOnlyMeshes;

    // 메쉬를 추가하는 함수들
    void AddDecorationMesh(UStaticMesh* Mesh);
    void AddWalkableMesh(UStaticMesh* Mesh);
    void AddBlockPlayerOnlyMesh(UStaticMesh* Mesh);
};
