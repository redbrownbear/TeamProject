// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Misc/Utils.h"
#include "Actors/Monster/MonsterInterface.h"
#include "CharacterMonster.generated.h"


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
class TEAMPROJECT_API ACharacterMonster : public ACharacter, public IMonsterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterMonster();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UMonsterStatusComponent> StatusComponent;
    UPROPERTY(EditAnywhere)
    TObjectPtr<APatrolPath> PatrolPath;
    UPROPERTY(EditAnywhere)
    TObjectPtr<ACampFire> CampFire;
    UPROPERTY(EditAnywhere)
    TArray<USphereComponent*> AdditionalColliders;

protected:
    UPROPERTY(EditAnywhere, meta = (RowType = "MonsterTableRow"))
    FDataTableRowHandle DataTableRowHandle;
	// as it doestn't UObject or AActor, it cannot be TObjectPtr
    FMonsterTableRow* MonsterData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
    virtual void PostLoad() override;
    virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
    virtual void PostInitializeComponents() override;
    virtual void OnConstruction(const FTransform& Transform);

public:
    virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

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
    virtual USkeletalMeshComponent* GetMonsterMesh() const override { return GetMesh(); }
    virtual UMonsterFSMComponent* GetFSMComponent() const;
    virtual void SetSpeedWalk() override;
    virtual void SetSpeedRun() override;


protected:
    UFUNCTION()
    virtual void OnDie() override;
    UFUNCTION()
    virtual void OnDeadEnd() override;

protected:
    TObjectPtr<AActor> ThrownObject = nullptr;
public:
    void SetThrownObject(AActor * InThrownObject) { ThrownObject = InThrownObject; }
    void DeleteThrownObject() { if (ThrownObject) ThrownObject->Destroy(); }
};
