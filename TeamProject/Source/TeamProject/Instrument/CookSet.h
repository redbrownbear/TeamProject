#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/TestCharacter/TestCharacter.h"
#include "CookSet.generated.h"

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase
{
	GENERATED_BODY()

	// @TODO 특정 데이터(레시피) 넣어둘 것

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
	// 캐릭터가 Q 눌러 아이템을 전달했을 때 호출
	void ReceiveIngredient(const FHandItem& HandItem, const FRecipeData& Recipe);

	// 레시피 체크(요리 성공/실패 판단)
	bool CheckRecipeMatch(const FHandItem& HandItem, const FRecipeData& Recipe);

	void SuccessCooking();

	void FailedCooking();

protected:
	// 요리 가능 여부
	UPROPERTY(BlueprintReadOnly)
	bool bIsCookable = false;

	// 현재 들어온 재료 리스트
	UPROPERTY()
	TArray<FHandItem> CurrentIngredients;

	// 현재 레시피 (매칭되는 경우만 설정)
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
