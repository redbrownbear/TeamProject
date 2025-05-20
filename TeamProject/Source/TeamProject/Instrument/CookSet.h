#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/TestCharacter/TestCharacter.h"
#include "CookSet.generated.h"

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase
{
	GENERATED_BODY()
	

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
	void ReceiveIngredient(const FHandItem& HandItem, const FRecipeData& Recipe);

	bool CheckRecipeMatch(const FHandItem& HandItem, const FRecipeData& Recipe);

	void SuccessCooking();

	void FailedCooking();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsCookable = false;

	UPROPERTY()
	TArray<FHandItem> CurrentIngredients;

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
