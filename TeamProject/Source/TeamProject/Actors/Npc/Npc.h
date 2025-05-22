#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Data/NPCTableRow.h"
#include "Data/NpcCharacterTableRow.h"
#include "Misc/Utils.h"
#include "Npc.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UNpcFSMComponent;
class AStrollPath;
class UAdvancedFloatingPawnMovement;

UCLASS()
class TEAMPROJECT_API ANpc : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> HeadMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> HairMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> NoseMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UAdvancedFloatingPawnMovement> MovementComponent;

public:
	UNpcFSMComponent* GetFSMComponent() const;

	AStrollPath* GetStrollPath() const { return StrollPath; }

	USkeletalMeshComponent* GetBodyMesh() const { return BodyMeshComponent; }

	//USkeletalMesh* GetBodySkeletalMesh() const { return BodyMeshComponent ? BodyMeshComponent->SkeletalMesh : nullptr; }

	void SetStrollPath(AStrollPath* InPath) { StrollPath = InPath; }

	//void OnTalkKeyPressed(); // 대화 UI

	bool GetCanTalk() { return bPlayerInRange; }

	EQuestCharacter GetNpc() const { return QuestNpc; }

	void SetNpc(EQuestCharacter InQuestNpc) { QuestNpc = InQuestNpc; }

	void AttachToSocket();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	FDataTableRowHandle DataTableRowHandle;
	FNpcCharacterTableRow* NpcData;

public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

public:
	void PlayMontage(ENpcMontage _InEnum, bool bIsLoop = false);
	bool IsMontage(ENpcMontage _InEnum);
	bool IsPlayingMontage(ENpcMontage _InEnum);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AStrollPath> StrollPath;

	UPROPERTY(VisibleAnywhere)
	EQuestCharacter QuestNpc;

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMesh* HeadMeshAsset;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* HairMeshAsset;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* NoseMeshAsset;

private:
	// 상호작용 가능 변수
	UPROPERTY()
	bool bPlayerInRange = false;
protected:
	// 상호작용 
	UFUNCTION()
	void OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void SetSpeedWalk();
	void SetSpeedRun();
};
