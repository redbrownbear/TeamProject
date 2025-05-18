// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Misc/Utils.h"
#include "Monster.generated.h"

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
class TEAMPROJECT_API AMonster : public APawn
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UMonsterStatusComponent> StatusComponent;
    UPROPERTY(EditAnywhere)
    TObjectPtr<USphereComponent> CollisionComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAdvancedFloatingPawnMovement> MovementComponent;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

public:
    UMonsterStatusComponent* GetStatusComponent() const { return StatusComponent; }
    UMonsterFSMComponent* GetFSMComponent() const;

protected:
    UPROPERTY(EditAnywhere)
    FDataTableRowHandle DataTableRowHandle;
    TObjectPtr<FMonsterTableRow> MonsterData;

public:
    virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
    virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
    virtual void PostLoad() override;
    virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
    virtual void PostInitializeComponents() override;
    virtual void OnConstruction(const FTransform& Transform);

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<APatrolPath> PatrolPath;
public:
    APatrolPath* GetPatrolPath() const { return PatrolPath; }

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<ACampFire> CampFire;
public:
    ACampFire* GetCampFire() const { return CampFire; }

public:
    void PlayMontage(MONSTER_MONTAGE _InEnum, bool bIsLoop = false);
    bool IsMontage(MONSTER_MONTAGE _InEnum);
    bool IsPlayingMontage(MONSTER_MONTAGE _InEnum);


};