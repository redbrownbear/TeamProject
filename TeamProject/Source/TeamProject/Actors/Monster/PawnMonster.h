// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Actors/Monster/MonsterInterface.h"
#include "Misc/Utils.h"
#include "PawnMonster.generated.h"

class UMonsterStatusComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAdvancedFloatingPawnMovement;
class UMonsterFSMComponent;
class USphereComponent;
class APatrolPath;
class ACampFire;

struct FMonsterTableRow;

UCLASS()
class TEAMPROJECT_API APawnMonster : public APawn, public IMonsterInterface
{
	GENERATED_BODY()

public:
    APawnMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<USphereComponent> CollisionComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAdvancedFloatingPawnMovement> MovementComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UMonsterStatusComponent> StatusComponent;

protected:
    UPROPERTY(EditAnywhere, meta = (RowType = "MonsterTableRow"))
    FDataTableRowHandle DataTableRowHandle;
    FMonsterTableRow* MonsterData;


protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<APatrolPath> PatrolPath;
    UPROPERTY(EditAnywhere)
    TObjectPtr<ACampFire> CampFire;


protected:
    virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
    virtual void PostLoad() override;
    virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
    virtual void PostInitializeComponents() override;
    virtual void OnConstruction(const FTransform& Transform);

public:
    virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;
    virtual void SetData(const FName& MonsterName) override;

protected:
    UFUNCTION()
    virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    virtual UMonsterStatusComponent* GetStatusComponent() const { return StatusComponent; }
    virtual APatrolPath* GetPatrolPath() const override;
    virtual ACampFire* GetCampFire() const override;
	virtual FMonsterTableRow* GetMonsterData() const { return MonsterData; }


    // No const to attach WorldWeapon to SkeletalMeshComponent
    virtual UAnimInstance* GetAnimInstance() const;
    virtual USkeletalMeshComponent* GetMonsterMesh() const override { return SkeletalMeshComponent; }
    virtual UMonsterFSMComponent* GetFSMComponent() const;
    virtual void SetSpeedWalk() override;
    virtual void SetSpeedRun() override;

public:
    virtual void OnDie() override;
    virtual void OnDeadEnd() override;

protected:
    UMaterialInterface* MaterialInterface;
    UMaterialInstanceDynamic* DynamicMaterialInstance;

protected:
    virtual UMaterialInterface* GetMaterialInterface() override;
    virtual UMaterialInstanceDynamic* GetDynamicMaterialInstance() override;
public:
    void AddBaseColor(FVector InColor);
};