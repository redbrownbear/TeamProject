#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Npc.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UNpcFSMComponent;
class UFloatingPawnMovement;
class AStrollPath;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> FaceMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> HairMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> NoseMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

public:
	UNpcFSMComponent* GetFSMComponent() const;

	AStrollPath* GetStrollPath() const { return StrollPath; }

	void SetStrollPath(AStrollPath* InPath) { StrollPath = InPath; }

	void OnTalkKeyPressed(); // 대화 UI

	bool GetCanTalk() { return bPlayerInRange; }

	void DisableMovement(); // 움직임 제한
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AStrollPath> StrollPath;

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
};
