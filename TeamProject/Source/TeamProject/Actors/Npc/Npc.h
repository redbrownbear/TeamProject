#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "Data/NpcCharacterTableRow.h"
#include "Misc/Utils.h"
#include "Npc.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UNpcFSMComponent;
class AStrollPath;
class UAdvancedFloatingPawnMovement;
class UConversationManagerComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ConverSation")
	TObjectPtr<UConversationManagerComponent> ConversationManager;

public:
	UNpcFSMComponent* GetFSMComponent() const;

	AStrollPath* GetStrollPath() const { return StrollPath; }

	USkeletalMeshComponent* GetBodyMesh() const { return BodyMeshComponent; }

	UConversationManagerComponent* GetConversationManager() const { return ConversationManager; }

	bool GetCanTalk() { return bPlayerInRange; }

	bool GetIsConfirmed() { return bIsConfirm; }

	bool GetIsHide() { return bIsHide; }

	bool GetDoQuest() { return bDoQuest; }

	bool GetShopping() { return bShopping; }

	bool GetBuy() { return bBuy; }

	EQuestCharacter GetQuestCharacterType() const { return QuestNpc; }

public:
	void SetStrollPath(AStrollPath* InPath) { StrollPath = InPath; }

	void SetNpc(EQuestCharacter InQuestNpc) { QuestNpc = InQuestNpc; }

	void SetIsConfirmed(bool IsConfirm) { bIsConfirm = IsConfirm; }

	void SetIsHide(bool IsHide) { bIsHide = IsHide; }

	void SetDoQuest(bool IsQuest) { bDoQuest = IsQuest; }

	void SetShopping(bool IsShopping) { bShopping = IsShopping; }

	void SetBuy(bool IsBuy) { bBuy = IsBuy; }

	void SetSell(bool IsSell) { bSell = IsSell; }

	void AttachToSocket();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	FDataTableRowHandle DataTableRowHandle;
	FNpcCharacterTableRow* NpcData;

public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	const FNpcCharacterTableRow* GetData() const { return NpcData; }

	void SetCurrentDialogueType(EDialogType InDialogueType) { CurrentDialogueType = InDialogueType; }
	const EDialogType GetCurrentDialogueType() const { return CurrentDialogueType; }

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
	TObjectPtr<USkeletalMesh> HeadMeshAsset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> HairMeshAsset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> NoseMeshAsset;

private:
	// 상호작용 가능 변수
	UPROPERTY()
	bool bPlayerInRange = false;

	UPROPERTY() // 퀘스트 수락 시 
	bool bIsConfirm = false;

	UPROPERTY()
	bool bIsHide = false;

	// 콘텐츠 변수
	UPROPERTY()
	bool bDoQuest = false;

	UPROPERTY()
	bool bShopping = false;

	UPROPERTY()
	bool bBuy = false;

	UPROPERTY()
	bool bSell = false;

protected:
	// 상호작용 
	UFUNCTION()
	void OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EDialogType CurrentDialogueType;

public:
	void SetSpeedWalk();
	void SetSpeedRun();
};
