#include "CookSet.h"
#include "Components/BoxComponent.h"
#include "Character/TestCharacter.h"

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
    
    // @TODO 시간 때우기 UI 오픈
}

void ACookSet::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATestCharacter* Player = Cast<ATestCharacter>(OtherActor);

    // @TODO 시간 때우기 UI 제거
}

void ACookSet::ReceiveIngredient(const FHandItem& HandItem, const FRecipeData& Recipe)
{
    for (const FRecipeData& Recipe : Recipes)
    {
        if (CheckRecipeMatch(HandItem, Recipe))
        {
            SuccessCooking();
            return;
        }
    }
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
    // @TODO 요리 성공 애니메이션 재생
    // @TODO 요리 아이템 UI 생성
    // @TODO 인벤토리에 요리 아이템 추가
}

void ACookSet::FailedCooking()
{
    // @TODO 요리 실패 애니메이션 재생
    // @TODO 요리 실패 UI 생성

}
