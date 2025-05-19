#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/TestCharacter.h"
#include "CookSet.generated.h"

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase
{
	GENERATED_BODY()

	// @TODO Ư�� ������(������) �־�� ��

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHandItem> RequiredIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHandItem ResultItem;*/
};

class UBoxComponent;

UCLASS()
class TEAMPROJECT_API ACookSet : public AActor
{
	GENERATED_BODY()
	
	friend class ATestCharacter;

public:
	ACookSet();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:		
	// ĳ���Ͱ� Q ���� �������� �������� �� ȣ��
	void ReceiveIngredient(const FHandItem& HandItem, const FRecipeData& Recipe);

	// ������ üũ(�丮 ����/���� �Ǵ�)
	bool CheckRecipeMatch(const FHandItem& HandItem, const FRecipeData& Recipe);

	void SuccessCooking();

	void FailedCooking();

protected:
	// �丮 ���� ����
	UPROPERTY(BlueprintReadOnly)
	bool bIsCookable = false;

	// ���� ���� ��� ����Ʈ
	UPROPERTY()
	TArray<FHandItem> CurrentIngredients;

	// ���� ������ (��Ī�Ǵ� ��츸 ����)
	UPROPERTY()
	FRecipeData CurrentRecipe;


protected:
	UPROPERTY(EditAnywhere)
	TArray<FRecipeData> Recipes;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayerOverlapping = false;

	UPROPERTY()
	ATestCharacter* OverlappingCharacter;

};
