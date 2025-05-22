#include "CookSet.h"
#include "Components/BoxComponent.h"
#include "Actors/TestCharacter/TestCharacter.h"

ACookSet::ACookSet()
{
    PrimaryActorTick.bCanEverTick = false;

}

void ACookSet::BeginPlay()
{
    Super::BeginPlay();

    UBoxComponent* Box = FindComponentByClass<UBoxComponent>();
    if (Box)
    {
        Box->OnComponentBeginOverlap.AddDynamic(this, &ACookSet::OnOverlapBegin);
        Box->OnComponentEndOverlap.AddDynamic(this, &ACookSet::OnOverlapEnd);
    }
}

void ACookSet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATestCharacter* Player = Cast<ATestCharacter>(OtherActor);
    
    // @TODO
}

void ACookSet::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATestCharacter* Player = Cast<ATestCharacter>(OtherActor);

    // @TODO 
}

void ACookSet::ReceiveIngredient(const FHandItem& HandItem, const FRecipeData& Recipe)
{
    /*for (const FRecipeData& Recipe : Recipes)
    {
        if (CheckRecipeMatch(HandItem, Recipe))
        {
            SuccessCooking();
            return;
        }
    }*/
    FailedCooking();
}

bool ACookSet::CheckRecipeMatch(const FHandItem& HandItem, const FRecipeData& Recipe)
{
    /*if (HandItem.ItemID == Recipe.ItemID &&
        HandItem.ItemName == Recipe.ItemName)
    {
        return true;
    }
    else
    {
       return false;
    }*/

    return true;
}

void ACookSet::SuccessCooking()
{
    // @TODO 
}

void ACookSet::FailedCooking()
{
    // @TODO 

}
